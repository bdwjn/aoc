#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

enum Dir { NORTH, EAST, WEST, SOUTH };

int solve(char *data, int w, int h, int x, int y, enum Dir to) {

	char beams[w][h][4];
	memset(beams, 0, sizeof(beams));
	
	struct Beam {
		int x, y;
		enum Dir to;
	} todo[w*h];
	 
	todo[0] = (struct Beam){ .x=x, .y=y, .to=to };
	int n=1;

	int energized = 0;

	while (n) {
		struct Beam *b = todo + (n-1);

		if (b->x < 0 || b->x >= w || b->y < 0 || b->y >= h || (beams[b->x][b->y][b->to])) {
			n--;
			continue;
		}

		energized += !(beams[b->x][b->y][0] | beams[b->x][b->y][1] | beams[b->x][b->y][2] | beams[b->x][b->y][3]);

		beams[b->x][b->y][b->to] = 1;// |= 1 << b->to;

		switch (data[b->x + b->y * (w+1)]) {
		case '/':
			b->to ^= 1;
			break;
		case '\\':
			b->to ^= 2;
			break;
		case '|':
			if (b->to == WEST || b->to == EAST) {
				todo[n++] = (struct Beam){ .x = b->x, .y = b->y + 1, .to = SOUTH };
				b->to = NORTH;
			}
			break;
		case '-':
			if (b->to == NORTH || b->to == SOUTH) {
				todo[n++] = (struct Beam){ .x = b->x + 1, .y = b->y, .to = EAST };
				b->to = WEST;
			}
			break;
		}

		b->x += (b->to == EAST)  ? 1 : (b->to == WEST)  ? -1 : 0;
		b->y += (b->to == SOUTH) ? 1 : (b->to == NORTH) ? -1 : 0;
	}

	return energized;
}

int main(void)
{
	FILE *f = fopen("input", "r");

	long part1 = 0, part2 = 0;

	char data[100000];
	int bytes = fread(data, 1, 100000, f);

	int w = strchr(data, '\n') - data;
	int h = bytes / (w+1);

	part1 = solve(data, w, h, 0, 0, EAST);

	part2 = 0;
	for (int test, y=0; y<h; y++) {
		test = solve(data, w, h, 0, y, EAST);
		if (test>part2) part2 = test;
		test = solve(data, w, h, w-1, y, WEST);
		if (test>part2) part2 = test;
	}

	for (int test, x=0; x<w; x++) {
		test = solve(data, w, h, x, 0, SOUTH);
		if (test>part2) part2 = test;
		test = solve(data, w, h, x, h-1, NORTH);
		if (test>part2) part2 = test;
	}

	printf("Part 1: %lu\n", part1);
	printf("Part 2: %lu\n", part2);
}