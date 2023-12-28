#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(void) {
	FILE *f = fopen("input", "r");

	long part1 = 0, part2 = 0;

	char data[100000];
	int bytes = fread(data, 1, 100000, f);

	char *p = data;
	while (p < data + bytes) {
		int w = strchr(p, '\n') - p;
		char *end = strstr(p, "\n\n");

		int h = ((end?end:data+bytes)-p+1) / (w+1);

		for (int y=0; y<h-1; y++) {
			/* compare row y with y+1, y-1 with y+2, etc */
			int ncompare = y<h/2 ? y+1 : h - y - 1;

			int i = 0, smudges = 0;
			for (i=0; i<ncompare; i++) {
				int row1 = y-i, row2 = y+1+i;
				for (int x=0; x<w; x++) {
					if (p[x+row1*(w+1)] != p[x+row2*(w+1)]) {
						smudges++;
					}
				}

				if (smudges > 1) break;	
			}

			if (smudges == 0) part1 += 100 * (y+1);
			if (smudges == 1) part2 += 100 * (y+1);
		}

		for (int x=0; x<w-1; x++) {
			/* compare col x with x+1, x-1 with x+2, etc */
			int ncompare = x<w/2 ? x+1 : w - x - 1;

			int i = 0, smudges = 0;
			for (i=0; i<ncompare; i++) {
				int col1 = x-i, col2 = x+1+i;
				for (int y=0; y<h; y++) {
					if (p[col1+y*(w+1)] != p[col2+y*(w+1)]) smudges++;
				}

				if (smudges > 1) break;
			}

			if (smudges == 0) part1 += (x+1);
			if (smudges == 1) part2 += (x+1);
		}

		p += (w+1)*h + 1;
	}

	printf("Part 1: %ld\n", part1);
	printf("Part 2: %ld\n", part2);
}