#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
	FILE *f = fopen("input","r");
	char line[1000], *p;
	
	long time[100] = {0}, dist[100] = {0}, *buf = time;
	long part1 = 1, prev = -1, races = 0;

	do {
		p = 10 + fgets(line, 1000, f);
		while (*p++ - '\n') *p>='0' && *p<='9' && (*buf = 10 * *buf + *p-'0');
		p = 10 + line;
		for (races=1; *p-'\n';) buf[races++] = strtol(p, &p, 10);
	} while (buf = dist, !++prev);

	while (races--) {
		long b = time[races], c = -dist[races];
		long discrim = b * b + 4 * c;

		long startx = (-b + 2 * sqrt(discrim-1) - sqrt(discrim)) / -2 + 1;
		long endx   = b - startx;

		part1 *= prev;
		prev = endx - startx + 1;
	}

	printf("Part 1: %ld\n", part1);
	printf("Part 2: %ld\n", prev);
}