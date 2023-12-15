#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int compare_uint64(const void *_a, const void *_b) {
	uint64_t a = *(uint64_t*)_a, b = *(uint64_t*)_b;
	return (a<b) ? -1 : (a>b) ? 1 : 0;
}

int main(void) {
	FILE *f = fopen("input", "r");
	char line[1000];

	uint64_t cards[2][1000];
	int n=0;

	while (fgets(line, 1000, f)) {
		uint64_t t[5], types[4] = {0}, jokers = 0, c1 = 0, c2 = 0;
		
		for (uint64_t *p=t; p<t+5; p++) {
			*p = line[p-t];
			*p = *p-'T' ? *p-'J' ? *p-'Q' ? *p-'K' ? *p-'A' ? *p-'0' : 14 : 13 : 12 : 11 : 10;
			c1 = (c1 << 8) | *p;
			c2 = (c2 << 8) | (*p==11 ? 0 : *p);
		}

		for (int i=0; i<5; i++) jokers += t[i]==11;

		for (int i=0; i<5; i++) {
			if (!t[i]) continue;

			int found = 1;
			for (int j=i+1; j<5; j++) {
				if (t[j]==t[i]) t[j]=0, found++;
			}

			for (int j=0; j < 3 - (t[i]==11); j += 2) {
				if (found > types[j]) {
					types[j+1] = types[j];
					types[j]   = found;
				} else if (found > types[j+1]) {
					types[j+1] = found;
				}
			}
		}
		types[2] += jokers;

		int bid = strtol(line+6, NULL, 10);
		c1 = (types[0] << 60) | (types[1] << 56) | (c1 << 16) | bid;
		c2 = (types[2] << 60) | (types[3] << 56) | (c2 << 16) | bid;

		cards[0][n]   = c1;
		cards[1][n++] = c2;
	}

	for (int i=0; i<2; i++) {
		qsort(cards[i], n, 8, compare_uint64);

		uint64_t sum = 0;
		for (int j=0; j<n; j++) {
			uint64_t c = cards[i][j];
			sum += (j+1) * (c & 0xFFFF);
		}

		printf("Part %d: %lu\n", i+1, sum);
	}
}