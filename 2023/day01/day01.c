#include <stdio.h>
#include <string.h>

int main(void)
{
	FILE *f = fopen("input", "r");
	char line[10000];

	const char *strs[] = {
		"3one","3two","5three","4four","4five","3six","5seven","5eight","4nine"
	};

	int part1=0, part2=0;
	
	while (fgets(line, 10000, f)) {
		int first1 = -1, last1 = -1, first2 = -1, last2 = -1;

		for (char *p=line; *p-'\n'; p++) {
			int digit1 = *p>='0' && *p<='9' ? *p-'0' : -1;
			int digit2 = digit1;

			for (int i=0; i<9; i++)
				digit2 = strncmp(p, strs[i]+1, *strs[i]-'0') ? digit2 : i+1;
			
			first1 = first1+1 ? first1 : digit1;
			last1  = digit1+1 ? digit1 : last1;
			first2 = first2+1 ? first2 : digit2;
			last2  = digit2+1 ? digit2 : last2;
		}

		part1 += 10 * first1 + last1;
		part2 += 10 * first2 + last2;
	}

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);
}
