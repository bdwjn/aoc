#include <stdio.h>

int main(void) {
	int x = 1, cycle = 0, signal = 0, op;

	while (op = getchar(), op != EOF) {
		int arg = 0, optime;

		if (op == 'a') scanf("ddx %d\n", &arg), optime = 2;
		else           scanf("oop\n"),          optime = 1;

		while (optime--) {
			int cx = cycle % 40;
			putchar( cx-x==0 || cx-x==1 || cx-x==-1 ? '#' : '.' );

			if (cx == 19) signal += (cycle+1) * x;

			if (cx == 39) putchar('\n');
			cycle++;
		}

		if (op == 'a') x += arg;
	}

	printf("\nPart 1: %d\n", signal);
}
