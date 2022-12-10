#include <stdio.h>

// answer: 833

int main(void)
{
	int count=0, a, b, c, d;

	while (scanf("%d-%d,%d-%d", &a, &b, &c, &d) == 4)
		count += a<=d && b>=c || c<=b && d>=a;

	printf("%d\n", count);
}