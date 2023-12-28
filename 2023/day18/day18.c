#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

struct Segment {
	long x, y_start, y_end;
};

int compare_y_start(const void *_a, const void *_b) {
	struct Segment *a = (struct Segment*)_a, *b = (struct Segment*)_b;
	return (a->y_start > b->y_start) - (a->y_start < b->y_start);
}

long solve(struct Segment *segments, int n) {
	long result = 0, completed=0, active=0, new=0, prevWidth=0, yPrev=-1, firstEnd = 0;

	qsort(segments, n, sizeof(struct Segment), compare_y_start);

	do {
		int yCur = (new<n && segments[new].y_start < firstEnd)
			? segments[new].y_start
			: firstEnd;

		// Add starting segments
		while (new<n && segments[new].y_start == yCur) {		
			int pos=new;
			// keep them sorted by x
			while (pos>completed && segments[pos-1].x > segments[pos].x) {
				struct Segment tmp = segments[pos-1];
				segments[pos-1]    = segments[pos];
				segments[pos]      = tmp;
				pos--;
			}
			active++; new++;
		}

		// Calculate current row
		int inside=0, skip = -1;
		for (int i=completed; i<new-1; i++) {
			if (
				(i != skip) &&
				((segments[i].y_start == yCur && segments[i+1].y_end   == yCur) ||
				(segments[i].y_end    == yCur && segments[i+1].y_start == yCur))
			) {
				result += segments[i+1].x - segments[i].x;
				skip = i+1;
				inside = !inside;
			} else if (
				(i != skip) &&
				((segments[i].y_start == yCur && segments[i+1].y_start == yCur) ||
				(segments[i].y_end    == yCur && segments[i+1].y_end   == yCur))
			) {
				result += segments[i+1].x - segments[i].x;
				skip = i+1;
			} else if (inside) {
				result++;
			} else {
				result += segments[i+1].x - segments[i].x;
			}
			inside = !inside;
		}
		result++;

		// Remove finished segments
		firstEnd = LONG_MAX;
		for (int i=completed; i<new; i++) {

			if (segments[i].y_end == yCur) {
				for (int j=i; j>completed; j--) segments[j] = segments[j-1];

				completed++;
				active--;
			}
			else if (segments[i].y_end < firstEnd) {
				firstEnd = segments[i].y_end;
			}
		}

		result += (yCur - yPrev - 1) * prevWidth;

		// We now have a multiple of 2 segments, switching between "inside" and "outside"
		// the shape. Calculate the width of all the inner parts (to use for the area in
		// the next iteration).
		prevWidth = 0;
		for (int i=completed; i<new; i+=2) {
			prevWidth += segments[i+1].x - segments[i].x + 1;
		}

		yPrev = yCur;
	} while (completed<n);
	
	return result;
}

int main(void) {
	FILE *f = fopen("input", "r");
	char line[1000];

	struct Segment part[2][1000] = {0};

	long x[2] = {0}, y[2] = {0}, n[2] = {0};

	while (fgets(line, 1000, f)) {
		char *c = line;

		long dec = strtol(c+2, &c, 10);
		long hex = strtol(c+3, NULL, 16);

		for (int i=0; i<2; i++) {
			long len = i==0 ? dec : hex >> 4;
			long dir = i==0 ? line[0] : "RDLU"[hex & 15];

			x[i] += (dir=='L') ? len : (dir=='R') ? -len : 0;

			if (dir=='U') {
				part[i][n[i]++] = (struct Segment) { .x=x[i], .y_start=y[i]-len, .y_end=y[i] };
				y[i] -= len;
			} else if (dir=='D') {
				part[i][n[i]++] = (struct Segment) { .x=x[i], .y_start=y[i], .y_end=y[i]+len };
				y[i] += len;
			}
		}
	}

	printf("Part 1: %ld\n", solve(part[0], n[0]));
	printf("Part 2: %ld\n", solve(part[1], n[1]));
}