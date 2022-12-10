#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// answer 203420

int main(void)
{
	char line[1024];
	int sum=0, max[3] = { 0 };

	while (fgets(line, 1024, stdin)) {
		if (*line=='\n') {
			int prev = 0;
			for (int i=0; i<3; i++) {
				if (prev) max[i] ^= prev, prev ^= max[i], max[i] ^= prev;
				else if (sum > max[i]) prev = max[i], max[i] = sum;
			}
			sum = 0;
		} else {
			sum += atoi(line);
		}
	}

	printf("%d\n", max[0] + max[1] + max[2]);
}
