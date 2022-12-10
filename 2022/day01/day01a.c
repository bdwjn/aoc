#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// answer 68467

int main(void)
{
	char line[1024];
	int sum=0, max = -1;

	while (fgets(line, 1024, stdin)) {
		if (*line=='\n') {
			if (sum>max) max = sum;
			sum = 0;
		} else {
			sum += atoi(line);
		}
	}

	if (sum>max) max = sum;

	printf("%d\n", max);
}
