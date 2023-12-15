#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int main(void)
{
	FILE *f = fopen("input", "r");
	char line[1000];
	
	static struct Range { long from, to; enum { PART1, PART2 } part; } ranges[2][100000];
	int nranges[2]={0};

	fgets(line, 1000, f);
	char *p = line + 6;

	do {
		long s1 = strtol(p, &p, 10);
		long s2 = strtol(p, &p, 10);

		ranges[0][nranges[0]++] = (struct Range) { .from = s1, .to = s1,          .part = PART1 };
		ranges[0][nranges[0]++] = (struct Range) { .from = s2, .to = s2,          .part = PART1 };
		ranges[0][nranges[0]++] = (struct Range) { .from = s1, .to = s1 + s2 - 1, .part = PART2 };
	} while (*p != '\n');

	// ranges[from] are the ranges from the previous round.
	// ranges[to] is where all the updated ranges are inserted.
	// these flip after each round (detected by an empty line).
	int from = 1, to = 0;

	while (fgets(line, 1000, f)) {
		if (*line == '\n') {
			from = 1 - from;
			to = 1 - to;
			fgets(line, 1000, f); // skip the "a-to-b map:" line

			// copy any unmapped ranges from the previous round to the current one
			for (int i=0; i<nranges[to]; i++) {
				ranges[from][nranges[from]++] = ranges[to][i];
			}

			nranges[to] = 0;
			continue;
		}

		char *p = line;
		long map_dest = strtol(p, &p, 10); p++;
		long map_from = strtol(p, &p, 10); p++;
		long map_to   = strtol(p, &p, 10) + map_from - 1L; p++;

		for (int i=0; i<nranges[from]; i++) {
			struct Range *r = ranges[from] + i;

			long start = r->from;
			long end   = r->to;

			if (map_from <= end && map_to >= start) {
				long from2 = start > map_from ? start : map_from;
				long to2   = end   < map_to   ? end   : map_to;

				ranges[to][nranges[to]++] = (struct Range) {
					.from = from2 + map_dest - map_from,
					.to   = to2   + map_dest - map_from,
					.part = r->part
				};

				if (map_from <= start && map_to >= end) {
					// case 1: nothing left, remove the entire range
					*r = ranges[from][--nranges[from]];
					i -= i != nranges[from];
				} else if (map_from <= start) {
					// case 2: (map_to+1)..end unchanged, update range's "from"
					r->from = map_to + 1;
				} else if (map_to >= end) {
					// case 3: start..(map_from-1) unchanged, update range's "to"
					r->to = map_from - 1;
				} else {
					// case 4: start..(map_from-1) and (map_to+1)..end unchanged:
					//   update ranges[from][i] to start..(map_from-1)
					//   add a new range for (map_to+1)..end
					r->to = map_from  - 1;
					ranges[from][nranges[from]++] = (struct Range) { .from = map_to + 1, .to = end, .part = r->part };
				}
			}
		}
	}

	long part1 = LONG_MAX, part2 = LONG_MAX;

	for (int j=0; j<=1; j++) {
		for (int i=0; i<nranges[j]; i++) {
			if (ranges[j][i].part == PART1 && ranges[j][i].from < part1)
				part1 = ranges[j][i].from;
			if (ranges[j][i].part == PART2 && ranges[j][i].from < part2)
				part2 = ranges[j][i].from;
		}
	}

	printf("Part 1: %ld\n", part1);
	printf("Part 2: %ld\n", part2);
}