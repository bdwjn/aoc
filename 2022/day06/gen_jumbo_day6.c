#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIST 14

/* Outputs a 10 MB input, guaranteed to have at least one repetition every DIST characters.
   At the end of the 10 MB it outputs a..z for create a solution.
*/

int main(void)
{
	int last[256]={0}, clear=DIST-1, c;

	srand(0);

	char *test = malloc(10000000);

	for (int i=0; i<10000000; i++) {
		do {
			test[i] = 'a' + (rand() % 26);

			if (last[test[i]] + DIST > clear) {
				clear = last[test[i]] + DIST;
			}
		} while (i == clear);

		last[test[i]] = i;
	}

	for (int i=10000000-26; i<10000000; i++) test[i] = 'a' + i - 10000000 + 26;

	fwrite(test, 1, 10000000, stdout);
}
