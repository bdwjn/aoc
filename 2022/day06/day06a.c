#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIST 4

int main(void)
{
	int last[256] = {0}, clear=0, c, i=0;

	while (i++, c = getchar(), c != EOF) {
		clear = (last[c] + DIST > clear) ? last[c] + DIST : clear;
		last[c] = i;
		if (i == clear) break;
	}

	printf("%d\n", i);
}
