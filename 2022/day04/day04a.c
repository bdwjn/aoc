#include <stdio.h>

// answer: 494

int main(void)
{
	int count=0, a, b, c, d;

	while (scanf("%d-%d,%d-%d", &a, &b, &c, &d) == 4)
		count += a>=c && b<=d || c>=a && d<=b;

	printf("%d\n", count);
}