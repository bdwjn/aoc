#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#ifndef N
#	define N    5000000
#endif

int main() {
	uint64_t a = 883, b = 879, c = 0;

	for (int i=0; i<N; i++) {
		do {
			a *= 16807;
			a %= 2147483647;
		} while (a & 3);

		do {
			b *= 48271;
			b %= 2147483647;
		} while (b & 7);

		c += (a & 0xFFFF) == (b & 0xFFFF);
	}

	printf("Part 2: %" PRIu64 "\n", c);

}
