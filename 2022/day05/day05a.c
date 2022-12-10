#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

// answer: BSDMQFLSP

int main(void)
{
	char line[100];
	
	char stacks[20][200]; int sp[20] = {0};
 
	while (fgets(line, 100, stdin)) {
		if (line[1] == '1') break;

		for (int i=0; line[4*i]; i++)
			if (line[4*i+1] != ' ')
				for (int j=++sp[i]; --j+1;)
					stacks[i][j] = j==0 ? line[4*i+1] : stacks[i][j-1];
	}
	
	if (!fgets(line, 100, stdin)) return -1;

	int n, from, to;
	while (fscanf(stdin, "move %d from %d to %d\n", &n, &from, &to) == 3) {
		from--; to--;
		for (int i=0; i<n; i++)
			stacks[to][sp[to]++] = stacks[from][--sp[from]];
	}

	for (int i=0; i<9; i++) putchar(stacks[i][sp[i]-1]);

}
