#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	FILE *f = fopen("input", "r");

	char line[1000];

	unsigned long part1=0, part2=0, copies[1000]={0};

	for (int cardidx=0; fgets(line,1000,f); cardidx++) {
		copies[cardidx]++;

		int win[100], nWin=0;
		int num[100], nNum=0;

		char *p = strchr(line,':') + 1, p2;

		while (p[1] != '|') { win[nWin++] = strtol(p+1, &p, 10); }
		p += 2;
		while (*p != '\n') { num[nNum++] = strtol(p+1, &p, 10); }

		int count=0;
		for (int i=0; i<nWin; i++) {
			for (int j=0; j<nNum; j++) {
				if (win[i] == num[j]) {
					count++;
				}
			}
		}

		for (int i=cardidx+1; i<cardidx+1+count; i++) {
			copies[i] += copies[cardidx];
		}

		part1 += (count>0) << count-1;
		part2 += copies[cardidx];
	}

	printf("Part 1: %lu\n", part1);
	printf("Part 2: %lu\n", part2);
}