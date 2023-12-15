#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int partcount[1000][1000] = {0};
int partmul[1000][1000]   = {0};

int main(void)
{
	char data[1000000];
	int bytes = fread(data, 1, 1000000, fopen("input","r"));

	int W = strchr(data,'\n')-data;

	int part1=0, part2=0;

	for (int y=0; y<W; y++) {
		for (int x=0; x<W; x++) {
			char *p = data + x + (W+1) * y;
			int digits=0, num=0, is_part=0;

			while (*p>='0' && *p<='9') num = 10 * num + *p - '0', digits++, p++;

			if (!digits) continue;

			char sym='.';
			for (int dx=-1; dx<1+digits; dx++) {
				for (int dy=-1; dy<=1; dy++) {
					if (x+dx>=0 && x+dx<W && y+dy>=0 && y+dy<W) {
						char s = data[x+dx+(W+1)*(y+dy)];

						is_part |= (s!='.' && (s<'0' || s>'9'));

						if (s=='*' && (s<'0' || s>'9')) {
							switch (partcount[x+dx][y+dy]) {
								case 0: {
									partmul[x+dx][y+dy] = num;
									break;
								}
								case 1: {
									int mul = (partmul[x+dx][y+dy] *= num);
									part2 += mul;
									break;
								}
								case 2: {
									part2 -= partmul[x+dx][y+dy];
									break;
								}
							}
							partcount[x+dx][y+dy]++;
						}
					}
				}
			}

			if (is_part) part1 += num;

			x += digits-1;
		}
	}

	printf("Part1: %d\n", part1);
	printf("Part2: %d\n", part2);
}
