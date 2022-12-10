#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// to be used with https://github.com/JeremyGrosser/advent/blob/master/2022/input/gen_jumbo_day5.py

// answer: LVIBGPPZMTHWIJDKGUXNOPSESSJPYFGF

int main(void)
{
	char line[1000];	
	char *stacks[100]; int sp[100] = {0}, n = 0;

	// 640 kB ought to be enough for anyone
	for (int i=0; i<100; i++) stacks[i] = malloc(640 * 1024);

	while (fgets(line, 1000, stdin))
	{
		if (line[1] == '1' || !line[1]) break; // jumbo file is missing the "1 2 3" line

		for (n=0; line[4*n]; n++)
			if (line[4*n+1] != ' ')
				stacks[n][sp[n]++] = line[4*n+1];
	}

	// the stacks were read in reverse because of the silly ascii art, so flip them
	for (int i=0; i<n; i++)
	{
		char *s = stacks[i], *e = stacks[i] + sp[i] - 1;
		while (s < e) *s ^= *e, *e ^= *s, *s ^= *e, s++, e--;
	}

	// skip the empty line, but only if this isn't the jumbo file
	if (line[1]=='1' && !fgets(line, 1000, stdin)) return -1;

	int count, from, to;
	while (fscanf(stdin, "move %d from %d to %d\n", &count, &from, &to) == 3)
	{
		to--; from--;
		memcpy(stacks[to] + sp[to], stacks[from] + sp[from] - count, count);

		sp[to]   += count;
		sp[from] -= count;
	}

	for (int i=0; i<n; i++) putchar(stacks[i][sp[i]-1]);
	puts("");
}
