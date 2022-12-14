#include <stdio.h>
#include <stdlib.h>

int cmp (const char *p1, const char *p2) {
	int depth1=0, depth2=0;

	while (*p1 != '\n') {
		if (*p1 == '[')
			if (*p2 == '[') p1++, p2++;
			else if (*p2 == ']') return 1;
			else depth1++, p1++;
		else if (*p1 == ']')
			if (*p2 == '[') return -1;
			else if (*p2==']') p1++, p2++;
			else return -1;
		else {
			if (*p2 == '[') p2++, depth2++;
			else if (*p2 == ']') return 1;
			else {
				int num1 = *p1, num2 = *p2;

				if (p1[1]=='0') ++p1, num1 += 9;
				if (p2[1]=='0') ++p2, num2 += 9;

				if (num1 == num2) {
					while (depth1 && p1[1]==']') p1++, depth1--;
					if (depth1) return 1;
					while (depth2 && p2[1]==']') p2++, depth2--;
					if (depth2) return -1;

					p1++; p1 += (*p1==',');
					p2++; p2 += (*p2==',');
				}
				else return (num1<num2) ? -1 : 1;
			}
		}
	}
	return 0;
}

int main(void)
{
	int idx=1, sum=0, mul=1, n=0;

	char line1[1000], line2[1000];

	int two_idx = 1, six_idx = 2;
	const char *two = "[[2]]\n", *six = "[[6]]\n";

	do {
		if (!fgets(line1, 1000, stdin)) return 0;
		if (!fgets(line2, 1000, stdin)) return 0;

		sum += cmp(line1, line2) == -1 ? idx : 0;

		if (cmp(line1, two) < 0) two_idx++, six_idx++;
		else if (cmp(line1, six) < 0) six_idx ++;
		if (cmp(line2, two) < 0) two_idx++, six_idx++;
		else if (cmp(line2, six) < 0) six_idx ++;
	} while (idx++, getchar() != EOF);

	printf("Part 1: %d\n", sum);
	printf("Part 2: %d\n", two_idx * six_idx);
}