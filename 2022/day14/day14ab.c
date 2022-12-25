#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// Part 1: 768
// Part 2: 26686

int main(void)
{
	static char cave[1000][1000]={0};
	
	int x1, y1, x2, y2, ymax=0, nl=1;

	int rock=0, cavities=0, sand=0;

	while (scanf("%d,%d", &x2, &y2) == 2) {
		if (!nl) {
			int dx = (x2>x1) - (x2<x1), dy = (y2>y1) - (y2<y1);

			for (int i=x1, j=y1; i!=x2+dx || j!=y2+dy; i+=dx, j+=dy) {
				rock += (cave[i][j]==0);
				cave[i][j] = 1;
			}
			ymax = y1>ymax ? y1 : ymax;
			ymax = y2>ymax ? y2 : ymax;
		}

		x1 = x2; y1 = y2;
		nl = scanf(" ->%c", (char*)&nl) != 1;
	}

	int px[1000] = {500}, py[1000] = {0}, n=1;
	
	int x, y;
	do {
		x = px[n-1];
		y = py[n-1];

		if      (!cave[x  ][y+1]) px[n] = x,   py[n++] = y+1;
		else if (!cave[x-1][y+1]) px[n] = x-1, py[n++] = y+1;
		else if (!cave[x+1][y+1]) px[n] = x+1, py[n++] = y+1;
		else cave[x][y] = 1, sand++, n--;
	} while (y<ymax);

	for (y=0; y<ymax+1; y++) {
		for (x=500-y; x<=500+y; x++) {
			if (cave[x][y] && cave[x-1][y] && cave[x+1][y]) {
				if (!cave[x][y+1]) {
					cavities++;
					cave[x][y+1] = 1;
				}
			}
		}
	}

	int area = (ymax+2) * (ymax+2);

	printf("Part 1: %d\n", sand);
	printf("Part 2: %d\n", area - rock - cavities);
}