//std-c
#include <stdio.h>
#include <stdlib.h>
#include "../bloom_filter.h"
#include "../log.h"
#include "../str.h"
#include <vector>
#include <string>


int testBloomFilter() {

	FILE* f = fopen("../bloom_filter.c", "r");
	fseek(f,0L,SEEK_END);
	size_t len = ftell(f);
	rewind(f);
	char* buffer = (char*)malloc(len+1);
	fread(buffer, len, 1, f);
	fclose(f);

	std::vector<std::string> words;
	split(buffer, " ", words);
	free(buffer);

	struct bloom_filter* bfilter = bopen(words.size(), 0.05);
	check(bfilter != NULL);

	info("OK\n");
	for (int i = 0; i < words.size(); i++) {
		bset(bfilter, words[i].c_str());
		info("set: %s\n", words[i].c_str());
	}

	info("Reliability : %.3f\n", breliability(bfilter));
	for (int i = 0; i < words.size(); i++) {
		if ( ! bfind(bfilter, words[i].c_str())) {
			error("BadBloomFilter: %s\n", words[i].c_str());
		}
	}

	bclose(bfilter);

	return 0;
}

void testbloomN(struct bloom_filter* bfilter, int _n, float zratio) {
	bfilter = bopen(_n, zratio);
	bclose(bfilter);
}

int testBloomFilterBopen() {
	struct bloom_filter* bfilter = NULL;

	testbloomN(bfilter, 3000,  0.05);
	testbloomN(bfilter, 3000,  0.1);
	testbloomN(bfilter, 3000,  0.2);
	testbloomN(bfilter, 6000,  0.3);
	testbloomN(bfilter, 12000, 0.4);
	testbloomN(bfilter, 24000, 0.5);

	return 0;
}


int main(int argc, const char *argv[])
{
	log_init("debug.log", LOG_CONSOLE);

	testBloomFilterBopen();

	testBloomFilter();

	return EXIT_SUCCESS;
}

