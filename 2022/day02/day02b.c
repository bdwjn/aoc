#include <stdio.h>

// answer: 13433

int main(int c, char **v)
{
	while (fgets(*v, 5, stdin))
		c += (**v + 2[*v] - 1) % 3 + 3 * 2[*v] - 263;
	
	printf("%d\n", --c);
}
