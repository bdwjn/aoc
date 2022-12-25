#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Part 1: 281
// Part 2: 807

int main(void) {
	char data[10000];
	int n = fread(data, 1, 10000, stdin);

	int LW= strchr(data,'\n') - data + 1;
	int W = LW - 3;
	int H = n / LW - 2;

	struct Blizzard { char x, y, dx, dy; } blizzard[W * H];
	int nbliz=0;

	for (int y=1; y<H+1; y++) {
		for (int x=1; x<W+1; x++) {
			if (data[x+LW*y] != '.') {
				int dx = (data[x+LW*y]=='>') ? 1 : (data[x+LW*y]=='<') ? W-1 : 0;
				int dy = (data[x+LW*y]=='v') ? 1 : (data[x+LW*y]=='^') ? H-1 : 0;
				blizzard[nbliz++] = (struct Blizzard){ .x=x-1, .y=y-1, .dx = dx, .dy = dy };
			}
		}
	}

	char map[W*H], buf1[W*H], buf2[W*H];
	char *cur = buf1, *prev = buf2;

	memset(prev, 0, W * H);

	int minute=1, phase=0;
	do {
		// clear blizzard map
		memset(map, 0, W*H);

		// move blizzards, draw on map
		for (int i=0; i<nbliz; i++) {
			blizzard[i].x = ( blizzard[i].x + blizzard[i].dx) % W;
			blizzard[i].y = ( blizzard[i].y + blizzard[i].dy) % H;
			map[blizzard[i].x + W * blizzard[i].y] = 1;
		}

		// clear possible positions for this minute
		memset(cur, 0, W * H);
		// add start cell, if there's no blizzard (top-left or bottom-right, depending on phase)
		cur[0]     = (phase==0 || phase==2) && !map[0];
		cur[W*H-1] = phase==1 && !map[W*H-1];

		// for each position we were in the last minute, check all 5 possible moves
		int idx=0;
		for (int y=0; y<H; y++)
		for (int x=0; x<W; idx++, x++) {
			if (!prev[idx]) continue;

			if (!map[idx])            cur[idx]   = 1;
			if (x>0   && !map[idx-1]) cur[idx-1] = 1;
			if (x<W-1 && !map[idx+1]) cur[idx+1] = 1;
			if (y>0   && !map[idx-W]) cur[idx-W] = 1;
			if (y<H-1 && !map[idx+W]) cur[idx+W] = 1;
		}

		if ((phase==0 || phase==2) && prev[H*W-1]) {
			printf("Part %d: %d\n", 1+(phase==2), minute);
			memset(cur, 0, W*H);
			memset(prev, 0, W*H);
			phase++;
		} else if (phase==1 && prev[0]) {
			memset(cur, 0, W*H);
			memset(prev, 0, W*H);
			phase++;
		}

		char *tmp = cur; cur = prev; prev = tmp;
		minute++;
	} while (phase < 3);
}