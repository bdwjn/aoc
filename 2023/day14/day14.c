#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define HASHSIZE 1234567

void spin(char *data, int stride_x, int stride_y, int w, int h)
{
	char *px = data;
	for (int x=0; x<w; x++) {
		char *p = px;
		char *pt = p;

		for (int y=0; y<h; y++) {
			if (*p == '#') pt = p + stride_y;
			else if (*p == 'O') {
				*p = *pt;
				*pt = 'O';
				pt += stride_y;
			}

			p += stride_y;
		}

		px += stride_x;
	}
}

unsigned long north_weight(char *data, int w, int h)
{
	unsigned long rval = 0;
	for (int y=0; y<h; y++)
		for (int x=0; x<w; x++) {
			if (data[x+y*(w+1)] == 'O') {
				rval += h - y;
			}
		}
	return rval;
}

int main(void)
{
	FILE *f = fopen("input", "r");

	long part1 = 0, part2 = 0;

	char data[100000];
	int bytes = fread(data, 1, 100000, f);

	int w = strchr(data, '\n') - data;

	int h = bytes / (w+1);

	static struct {
		unsigned long tag;
		int value;
	} hashes[HASHSIZE] = {0};

	for (int q=0; q<1000000000; q++) {
		spin(data,1,w+1,w,h); // north
		if (part1 == 0) part1 = north_weight(data, w, h);

		spin(data,w+1,1,h,w); // west
		spin(data+(h-1)*(w+1),1,-w-1,w,h); // south
		spin(data+w-1,w+1,-1,h,w); // east

		unsigned long hash = 0;
		for (int y=0; y<h; y++) {
			for (int x=0; x<w; x++) {
				hash ^= (hash<<13);
				hash ^= (hash>>7);
				hash ^= (hash<<17);
				hash ^= data[x+y*(w+1)];
			}
		}

		if (hashes[hash % HASHSIZE].tag == hash) {
			int period = q - hashes[hash % HASHSIZE].value;
			int skip_rounds = (1000000000 - q) / period;
			q += skip_rounds * period;
		} else if (hashes[hash%HASHSIZE].tag) {
			fprintf(stderr, "HASH COLLISION\n"); return 1;
		}
		hashes[hash % HASHSIZE].tag = hash;
		hashes[hash % HASHSIZE].value = q;
	}

	part2 = north_weight(data, w, h);

	printf("Part 1: %lu\n", part1);
	printf("Part 2: %lu\n", part2);
}