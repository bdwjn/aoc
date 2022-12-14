#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp (const void *_p1, const void *_p2) {
	const char *p1 = *(const char**)_p1, *p2 = *(const char**)_p2;

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
				int val1 = *p1, val2 = *p2;

				if (p1[1]=='0') ++p1, val1 += 9;
				if (p2[1]=='0') ++p2, val2 += 9;

				if (val1 == val2) {
					while (depth1 && p1[1]==']') p1++, depth1--;
					if (depth1) return 1;
					while (depth2 && p2[1]==']') p2++, depth2--;
					if (depth2) return -1;

					p1++; p1 += (*p1==',');
					p2++; p2 += (*p2==',');
				}
				else return (val1<val2) ? -1 : 1;
			}
		}
	}
	return 0;
}

int main(void)
{
	int idx=1, sum=0, mul=1, n=0;

	char *lines[1000];

	do {
		lines[n] = malloc(1000);
		fgets(lines[n++], 1000, stdin);
		lines[n] = malloc(1000);
		fgets(lines[n++], 1000, stdin);

		sum += cmp(lines + (n-2), lines + (n-1)) == -1 ? idx : 0;
	} while (idx++, getchar() != EOF);

	lines[n++] = "[[2]]\n";
	lines[n++] = "[[6]]\n";

	qsort(lines, n, sizeof(char*), cmp);

	for (int i=0; i<n; i++)
	{
		if (!strcmp(lines[i], "[[2]]\n") || !strcmp(lines[i], "[[6]]\n")) mul *= (i+1);
	}

	printf("Part 1: %d\n", sum);
	printf("Part 2: %d\n", mul);
}