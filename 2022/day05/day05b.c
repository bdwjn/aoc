#include <stdio.h>
#include <string.h>

// answer: PGSQBFLDP

int main(void)
{
	char line[100];
	
	char stacks[20][200]; int sp[20] = {0}, n;
 
	while (fgets(line, 100, stdin))
	{
		if (line[1] == '1') break;

		for (n=0; line[4*n]; n++)
			if (line[4*n+1] != ' ')
				for (int j=++sp[n]; --j+1;)
					stacks[n][j] = j==0 ? line[4*n+1] : stacks[n][j-1];
	}
	
	fgets(line, 100, stdin);

	int count, from, to;
	while (scanf("move %d from %d to %d\n", &count, &from, &to) == 3) {
		to--; from--;
		memcpy(stacks[to] + sp[to], stacks[from] + sp[from] - count, count);
		sp[to]   += count;
		sp[from] -= count;
	}

	for (int i=0; i<n; i++) putchar(stacks[i][sp[i]-1]);

}
