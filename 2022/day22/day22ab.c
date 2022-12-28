#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#define W 150
#define H 200

const int R=0, D=1, L=2, U=3, BLOCK=0xFFFFFFFF;

int main(void) {
	static char data[100000], *p = data;
	fread(data, 1, 100000, stdin);

	uint32_t map_part2[W*H][4], map_part1[W*H][4];
	memset(map_part2, 0xFF, sizeof map_part2);

	int x=0, y=0;
	do {
		while (*p != '\n') {
			if (*p == '.') {
				map_part2[x+W*y][R] = R<<30 | x+1 + W * y;
				map_part2[x+W*y][D] = D<<30 | x   + W * (y+1);
				map_part2[x+W*y][L] = L<<30 | x-1 + W * y;
				map_part2[x+W*y][U] = U<<30 | x   + W * (y-1);
			}
			x++; p++;
		}
		x=0; y++;
		p++;
	} while (*p != '\n');
	p++;

	memcpy(map_part1, map_part2, sizeof map_part2);

	/* stitch the edges to their counterpart... hard-coded edges... yuck. */
	for (int i=0; i<50; i++) {
		int ps[] = {
			50+i*150,       (149-i)*150,    149+i*150,
			50+(50+i)*150,  i+100*150,      99+(50+i)*150,
			(150+i)*150,    50+i,           49+(150+i)*150,
			99+(149-i)*150, 149+i*150,      (149-i)*150,
			50+i+149*150,   49+(150+i)*150, 50+i,
			i+199*150,      100+i,          i+100*150,
			100+i+49*150,   99+(50+i)*150,  100+i
		};
		int ds[] = { L,L,L, L,U,L, L,U,L, R,R,R, D,R,D, D,U,D, D,R,D };

		for (int j=0; j<21; j+=3) {
			int pt = ps[j], part2 = ps[j+1], part1 = ps[j+2];
			int d1 = ds[j], d2    = ds[j+1], d3    = ds[j+2];

			if (*map_part2[pt] != BLOCK) {
				map_part2[pt][d1] = (d2+2) % 4 << 30 | part2;
				map_part1[pt][d3] = d3 << 30 | part1;
			}
			if (*map_part2[part2] != BLOCK) {
				map_part2[part2][d2]       = (d1+2) % 4 << 30 | pt;
			}
			if (*map_part1[part1] != BLOCK) {
				map_part1[part1][(d3+2)%4] = (d3+2) % 4 << 30 | pt;
			}
		}
	}

	/* apply the force fields */
	for (int y=0; y<H; y++) {
		for (int x=0; x<W; x++) {
			if (*map_part2[x+W*y] == BLOCK) continue;

			for (int dir=0; dir<4; dir++) {
				/* if we can't move in this direction, "move" to the current pos */
				if (*map_part2[map_part2[x+W*y][dir] & 0xFFFF] == BLOCK) {
					map_part2[x+W*y][dir] = dir << 30 | x + W * y;
				}
				if (*map_part1[map_part1[x+W*y][dir] & 0xFFFF] == BLOCK) {
					map_part1[x+W*y][dir] = dir << 30 | x + W * y;
				}
			}
		}
	}

	uint32_t pos_p1 = 50, pos_p2 = 50;

	do {
		int count = 0;
		do count = 10 * count + *p++ - '0'; while (*p >= '0' && *p <= '9');

		/* perform the actual moves */
		for (int i=0; i<count; i++) {
			pos_p1 = map_part1[pos_p1 & 0xFFFF][pos_p1 >> 30];
			pos_p2 = map_part2[pos_p2 & 0xFFFF][pos_p2 >> 30];
		}
		/* direction change */
		if (*p == 'R') pos_p1 += (D<<30), pos_p2 += (D<<30);
		if (*p == 'L') pos_p1 += (U<<30), pos_p2 += (U<<30);
	} while (*p++ != '\n');

	int sx = 1 + (pos_p1 & 0xFFFF) % W;
	int sy = 1 + (pos_p1 & 0xFFFF) / W;
	fprintf(stderr, "Part 1: %d\n", 1000 * sy + 4 * sx + (pos_p1>>30));

	sx = 1 + (pos_p2 & 0xFFFF) % W;
	sy = 1 + (pos_p2 & 0xFFFF) / W;
	fprintf(stderr, "Part 2: %d\n", 1000 * sy + 4 * sx + (pos_p2>>30));
}