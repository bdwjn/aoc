#include <stdio.h>
#include <string.h>

// answer: 7811

int main(void)
{
	char line[1024];
	int sum = 0;
	
	while (fgets(line, 1024, stdin)) {
		int len = strlen(line) - 1;

		unsigned long count[2] = { 0, 0 };

		for (int i=0; i<len; i++) {
			char a = line[i];
			count[i >= len/2] |= 1LU << (a>='a' ? a-'a' : a-'A'+26);
		}

		count[0] &= count[1];

		while (count[0]) {
			int idx = __builtin_ctzll(count[0]);

			sum += idx + 1;
			count[0] ^= 1LU << idx;
		}
	}

	printf("%d\n", sum);
}
