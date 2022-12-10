#include <stdio.h>

// answer: 13484

int main(int c, char **v)
{
	while (fgets(*v, 5, stdin))
		c += (int[]){ 4, 1, 7, 8, 5, 2, 3, 9, 6 }[ **v - 'A' + 3 * (2[*v]-'X') ];

	printf("%d\n", --c);
}
