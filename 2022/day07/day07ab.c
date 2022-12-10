#include <stdio.h>
#include <stdlib.h>

// answer part 1: 1432936
// answer part 2: 272298

/* This version assumes depth-first traversal, and assumes "ls" won't get called twice. */

char cmd[100];
int dirsizes[1000000], ndirs = 0;

int parse(void) {
	int sum = 0;
	while (fgets(cmd, 100, stdin)) {
		if (cmd[0] == '$' && cmd[2] == 'c') {
			if (cmd[5] == '.') break;
			sum += parse();
		} else if (cmd[0] >= '0' && cmd[0] <= '9') {
			sum += atoi(cmd);
		}
	}

	dirsizes[ndirs++] = sum;

	return sum;
}

int main(void) {
	while (getchar() != '\n');
	
	int total = parse(), part1 = 0, part2 = total;

	for (int i=0; i<ndirs; i++) {
		if (dirsizes[i] <= 100000) part1 += dirsizes[i];
		if (dirsizes[i] >= total-40000000 && part2 > dirsizes[i]) part2 = dirsizes[i];
	}

	printf("part 1: %d\npart 2: %d\n", part1, part2);
};