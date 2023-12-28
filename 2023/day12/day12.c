#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

struct Cache {
	uint64_t val;
	int tag;
} cache[1000][100][100];

int tag;

uint64_t solve(char *c, int pos, int len, int must_take, int *nums, int numpos, int n)
{
	if (cache[pos][numpos][must_take].tag == tag) {
		return cache[pos][numpos][must_take].val;
	}

	uint64_t rval;

	if (pos == len) {
		rval = (numpos==n) && (must_take<=1) ? 1 : 0;
	}
	else if (c[pos] == '#') {
		if (must_take == 1) { rval = 0; }
		else if (must_take > 1) { rval = solve(c, pos+1, len, must_take-1, nums, numpos, n); }
		else if (numpos<n) { rval = solve(c, pos+1, len, nums[numpos], nums, numpos+1, n); }
		else rval = 0;
	} else if (c[pos] == '.') {
		if (must_take > 1) rval = 0;
		else rval = solve(c, pos+1, len, 0, nums, numpos, n);
	} else {
		if (must_take > 0) {
			rval = solve(c, pos+1, len, must_take-1, nums, numpos, n);
		} else {
			uint64_t take = numpos==n ? 0 : solve(c, pos+1, len, nums[numpos], nums, numpos+1, n);
			rval = take + solve(c, pos+1, len, 0, nums, numpos, n);
		}
	}

	cache[pos][numpos][must_take] = (struct Cache) { .val= rval, .tag = tag };

	return rval;
}

int main(void)
{
	FILE *f = fopen("input", "r");
	char line[1000];
	uint64_t part1 = 0, part2=0;

	while (fgets(line, 1000, f)) {

		int len = strchr(line, ' ') - line;
		line[len] = 0;
		
		int nums[100], n=0;

		char *p = line + len + 1;
		while (*p) { nums[n++] = strtol(p, &p, 10); p++; }

		tag++; // clear cache
		uint64_t solve1 = solve(line, 0, len, 0, nums, 0, n);
		part1 += solve1;

		// expand for part 2
		char line_exp[1000];
		int nums_exp[100], len_exp = 5*len+4, n_exp = 5*n;

		for (int i=0; i<5; i++) {
			memcpy(line_exp + i*(len+1), line, len);
			if (i != 4) line_exp[(i+1)*(len+1)-1] = '?';

			memcpy(nums_exp + i * n, nums, n * sizeof(int));
		}

		tag++; // clear cache
		uint64_t solve2 = solve(line_exp, 0, len_exp, 0, nums_exp, 0, n_exp);
		part2 += solve2;
	}

	printf("Part 1: %lu\n", part1);
	printf("Part 2: %lu\n", part2);
}