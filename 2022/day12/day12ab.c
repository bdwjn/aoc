#include <stdio.h>
#include <string.h>
#include <inttypes.h>

int main(void)
{
	char data[100000];
	int size = fread(data, 1, 100000, stdin);

	int w = strchr(data, '\n') - data + 1, h = size / w;
	int s = strchr(data, 'S') - data, e = strchr(data, 'E') - data;

	data[s] = 'a';
	data[e] = 'z';

	int seen[100000]  = { 0 }, front[100000] = { e }, n = 1;

	seen[e] = 1;

	int min = 1<<30;
	for (int i=0; i<n; i++) {
		int cur = front[i];

		if (cur == s) { printf("Part 1: %d\n", seen[cur]-1); }
		if (data[cur] == 'a' && seen[cur] < min) min = seen[cur];

		if (cur % w > 0     && (data[cur] <= data[cur-1]+1) && !seen[cur-1]) seen[cur-1] = seen[cur] + 1, front[n++] = cur - 1;
		if (cur % w < (w-1) && (data[cur] <= data[cur+1]+1) && !seen[cur+1]) seen[cur+1] = seen[cur] + 1, front[n++] = cur + 1;
		if (cur < w*(h-1)   && (data[cur] <= data[cur+w]+1) && !seen[cur+w]) seen[cur+w] = seen[cur] + 1, front[n++] = cur + w;
		if (cur > w         && (data[cur] <= data[cur-w]+1) && !seen[cur-w]) seen[cur-w] = seen[cur] + 1, front[n++] = cur - w;
	}

	printf("Part 2: %d\n", min-1);
}