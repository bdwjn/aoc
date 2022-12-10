#include <stdio.h>
#include <stdlib.h>

int main(void) {

	char *data = malloc(1e9);
	int size = fread(data, 1, 1e9, stdin);

	int W = 0; do W++; while (data[W]-'\n');
	int H = size / (W+1);
 	
	int best = 0;
	for (int row=0; row<H; row++) {
		for (int col=0; col<W; col++) {
			int l=0, r=0, t=0, b=0, max = data[col+(W+1)*row];

			for (int i=col-1; i>=0; i--) { l++; if (data[i+(W+1)*row] >= max) break; }
			for (int i=col+1; i<W;  i++) { r++; if (data[i+(W+1)*row] >= max) break; }
			for (int i=row-1; i>=0; i--) { t++; if (data[col+(W+1)*i] >= max) break; }
			for (int i=row+1; i<H;  i++) { b++; if (data[col+(W+1)*i] >= max) break; }

			int score = l*r*t*b;
			if (score>best) best = score;
		}
	}

	printf("%d\n", best);
}