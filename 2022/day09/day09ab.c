#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// answer part 1: 5878
// answer part 2: 2405

#define LEN 10 // (2 for part 1, 10 for part 2)
#define HASHSIZE 9999991LU

int main(void) {
	char *data = malloc(1e9), *p=data;
	int size = fread(data, 1, 1e9, stdin);

	unsigned long *hash = calloc(1, HASHSIZE * sizeof *hash);
	unsigned int count=0, kx[LEN]={0}, ky[LEN]={0};

	int frame=0;

	do {
		int dir = *p;
		int steps = 0; p += 2;
		do steps = 10*steps + *(p++) - '0'; while (*p != '\n');
		p++;

		for (int i=0; i<steps; i++) {
			kx[0] += dir=='R' ? 1 : dir=='L' ? -1 : 0;
			ky[0] += dir=='U' ? 1 : dir=='D' ? -1 : 0;

			for (int j=1; j<LEN; j++) {
				int idx = 5 * (ky[j-1] - ky[j]) + kx[j-1] - kx[j] + 12;
				kx[j] += (int[]){ -1,-1, 0, 1, 1, -1, 0, 0, 0, 1,  -1, 0, 0, 0, 1,  -1, 0, 0, 0, 1,  -1,-1, 0, 1, 1 }[idx];
				ky[j] += (int[]){ -1,-1,-1,-1,-1, -1, 0, 0, 0,-1,   0, 0, 0, 0, 0,   1, 0, 0, 0, 1,   1, 1, 1, 1, 1 }[idx];
			}

			unsigned long hval = kx[LEN-1] + (0x80000000LU + ky[LEN-1] << 32LU);
			unsigned long idx = hval % HASHSIZE;
			while (hash[idx] && hash[idx] != hval) idx = (idx+1) % HASHSIZE;
			if (!hash[idx]) count++, hash[idx]=hval;
		}
	} while (p-data-size);

	printf("%d\n", count);
}
