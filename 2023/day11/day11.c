#include <stdio.h>
#include <string.h>

int main(void) {
	FILE *f = fopen("input", "r");

	char data[100000];
	int bytes = fread(data, 1, 100000, f);

	int w = strchr(data, '\n') - data, h = bytes / (w+1);

	int cols[1000] = {0};
	int rows[1000] = {0};

	struct Galaxy { int x, y; } galaxies[1000];
	int n = 0;

	for (int y=0; y<h; y++) {
		for (int x=0; x<w; x++) {
			int g = (data[x+y*(w+1)] == '#');
			cols[x] |= g;
			rows[y] |= g;

			if (g) {
				galaxies[n] = galaxies[n+1] = (struct Galaxy) { .x=x, .y=y };
				n += 2;
			}
		}
	}

	for (int y=h-1; y>=0; y--) {
		if (!rows[y]) {
			for (int i=0; i<n; i++) {
				if (galaxies[i].y >= y) galaxies[i].y += i&1 ? 999999 : 1;
			}
		}
	}

	for (int x=w-1; x>=0; x--) {
		if (!cols[x]) {
			for (int i=0; i<n; i++) {
				if (galaxies[i].x >= x) galaxies[i].x += i&1 ? 999999 : 1;
			}
		}
	}

	unsigned long part[2] = {0};

	for (int i=0; i<n; i++) {
		for (int j=i+2; j<n; j+=2) {
			long dx = galaxies[i].x - galaxies[j].x;
			if (dx<0) dx = -dx;
			long dy = galaxies[i].y - galaxies[j].y;
			if (dy<0) dy = -dy;

			part[i&1] += dx + dy;
		}
	}

	printf("Part 1: %ld\n", part[0]);
	printf("Part 2: %ld\n", part[1]);
}