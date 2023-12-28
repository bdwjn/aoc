#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(void) {
	FILE *f = fopen("input", "r");

	long part1 = 0, part2 = 0;

	char line[1000];
	long a[100][100], n;

	while (fgets(line, 1000, f)) {
		n=0;

		char *p = line;
		while (*p != '\n') {
			a[0][n++] = strtol(p, &p, 10);
		}

		long z=0, r, sum=a[0][n-1], sum2 = 0;
		for (r=1; r<100 && !z; r++) {
			z = 1;
			for (int i=0; i<n-r; i++) {
				a[r][i] = a[r-1][i+1] - a[r-1][i];
				z &= !a[r][i];
			}
			sum += a[r][n-r-1];
		}
		part1 += sum;

		for (int i=r-1; i>=0; i--) sum2 = a[i][0] - sum2;
		part2 += sum2;
	}

	printf("Part 1: %ld\n", part1);
	printf("Part 2: %ld\n", part2);
}