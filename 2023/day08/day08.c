#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	FILE *f = fopen("input", "r");

	long part1 = 0;

	char moves[1000], line[1000];

	fgets(moves, 1000, f);
	int n = strchr(moves, '\n') - moves;

	static int left[1<<24], right[1<<24]; // left['AAA'] = new index after moving left at AAA
	int a[100], an=0; // all nodes ending with an A

	fgets(line, 1000, f); // skip newline
	while (fgets(line, 1000, f)) {
		int id = (line[0] <<16) + (line[1] <<8) + line[2];
		int le = (line[7] <<16) + (line[8] <<8) + line[9];
		int ri = (line[12]<<16) + (line[13]<<8) + line[14];

		if (line[2]=='A') a[an++] = id;

		left[id] = le; right[id] = ri;
	}

	for (int i=0; i<an; i++) {
		int pos = a[i];

		unsigned long idx = 0;
		do {
			int instr = moves[idx % n];
			pos = instr=='L' ? left[pos] : right[pos];
			idx++;
		} while((pos&0xFF) != 'Z');

		if (a[i] == 'A' * 0x010101) part1 = idx;
		a[i] = idx;
	}

	long part2 = a[0];
	for (int i=1; i<an; i++) {
		long gcd = part2, tmp=a[i];
		while (tmp) {
			long tmp2 = tmp;
			tmp = gcd % tmp;
			gcd = tmp2;
		}

		part2 = a[i] / gcd * part2;
	}	

	printf("Part 1: %ld\n", part1);
	printf("Part 2: %ld\n", part2);
}