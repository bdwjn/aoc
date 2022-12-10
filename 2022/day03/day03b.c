#include <stdio.h>
#include <inttypes.h>

// answer: 2639

int main(void)
{
	int64_t set[3] = {0}, sum = 0, line = 0, c;

	while (c = getchar(), c != EOF)
		c == '\n'
			&& ((++line < 3) || (line = 0, sum += __builtin_ctzll(set[0] & set[1] & set[2]) + 1))
			&& ~(set[line] = 0)
			|| (set[line] |= 1ull << (c >= 'a' ? c - 'a' : c - 'A' + 26));

	printf("%"PRIi64"\n", sum);
}