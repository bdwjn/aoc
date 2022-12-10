#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// answer: 1538

int main(void)
{
	char *data = malloc(1e9), *visible = malloc(1e9);
	int size = fread(data, 1, 1e9, stdin);
	memset(visible, 0, size);

	int W = strchr(data, '\n')-data;
	int H = size / (W+1);
	
	int count=0;
	for (int row=0; row<H; row++) {
		int maxl=0, maxr=0, maxt=0, maxb=0;
		for (int col=0; col<W; col++) {
			if (data[col+(W+1)*row]>maxl) { 
				count += !visible[col+W*row];
				visible[col+W*row] = 1;
				maxl = data[col+(W+1)*row];
			}
			if (data[(W+1)*row+W-1-col]>maxr) {
				count += !visible[(W-1)-col+W*row];
				visible[W-1-col+W*row] = 1;
				maxr = data[(W+1)*row+W-1-col];
			}

			if (data[row+(W+1)*col]>maxt) {
				count += !visible[row+W*col];
				visible[row+W*col] = 1;
				maxt = data[row+(W+1)*col];
			}
			if (data[(W+1)*(W-1-col)+row]>maxb) {
				count += !visible[row+W*(W-1-col)];
				visible[row+W*(W-1-col)] = 1;
				maxb = data[(W+1)*(W-1-col)+row];
			}
		}
	}

	printf("%d\n", count);
}