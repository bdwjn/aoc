#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *f = fopen("input", "r");
	char line[1000];

	int part1 = 0, part2 = 0;

	while (fgets(line, 1000, f)) {
		char *p;
		int id = strtol(line+5, &p, 10), r=0, g=0, b=0;

		while (*p - '\n') {
			p += 2;
			int num = strtol(p, &p, 10);
			p++;
			r = *p-'r' || (p+=3) && num<r ? r : num;
			g = *p-'g' || (p+=5) && num<g ? g : num;
			b = *p-'b' || (p+=4) && num<b ? b : num;
		}

		r<=12 & g<=13 & b<=14 && (part1 += id);
		part2 += r * g * b;
	}

	printf("Part 1: %d\nPart 2: %d\n", part1, part2);
}