#include <stdio.h>
#include <string.h>

#define P(X,Y) data[(X)+(Y)*(w+1)]
#define LOOP(X,Y) loop[(X)+(Y)*(w+1)]

int main(void) {
	FILE *f = fopen("input", "r");

	char data[100000], loop[100000] = {0};
	int bytes = fread(data, 1, 100000, f);

	int w = strchr(data, '\n') - data, h = bytes / (w+1);

	char *start = strchr(data, 'S');
	int sx = (start-data) % (w+1), sy = (start-data) / (w+1);
	int px, py;

	enum { NORTH, WEST, SOUTH, EAST } from;

	int left   = (P(sx-1,sy)=='F' || P(sx-1,sy) == '-' || P(sx-1,sy) == 'L');
	int top    = (P(sx,sy-1)=='F' || P(sx,sy-1) == '|' || P(sx,sy-1) == '7');
	int bottom = (P(sx,sy+1)=='L' || P(sx,sy+1) == '|' || P(sx,sy+1) == 'J');

	from = NORTH;
	if (left) {
		P(sx,sy) = top ? 'J' : bottom ? '7' : '-';
		from = bottom ? SOUTH : top ? from : WEST;
	} else if (top) {
		P(sx,sy) = bottom ? '|' : 'L';
	} else {
		P(sx,sy) = 'F';
		from = EAST;
	}

	px = sx; py = sy;

	int part1 = 0, part2 = 0;
	do {
		LOOP(px,py) = 1;

		switch (P(px,py)) {
		case 'F':
			if (from == EAST) py++, from = NORTH; else px++, from = WEST;
			break;
		case '7':
			if (from == WEST) py++, from = NORTH; else px--, from = EAST;
			break;
		case 'L':
			if (from == NORTH) px++, from = WEST; else py--, from = SOUTH;
			break;
		case 'J':
			if (from == NORTH) px--, from = EAST; else py--, from = SOUTH;
			break;
		case '|':
			if (from == NORTH) py++; else py--;
			break;
		case '-':
			if (from == WEST) px++; else px--;
			break;
		}
		part1++;
	} while ( px!=sx || py!=sy);
	part1 /= 2;

	for (int y=0; y<h; y++) {
		char state = '.';
		int inside = 0;
		for (int x=0; x<w; x++) {
			char c = LOOP(x,y) ? P(x,y) : '.';

			if (c=='|' || c=='J' || c=='7') {
				inside ^= (state=='.') | (state=='F') ^ (c=='7');
				state = '.';
			} else if (c=='L' || c == 'F') {
				state = c;
			} else if (c=='.') {
				part2 += inside;
			}
		}
	}

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);
}