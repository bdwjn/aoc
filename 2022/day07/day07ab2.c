#include <stdio.h>
#include <stdlib.h>

/* The file `input_special` contains a special case where a directory gets
    visited twice, and "ls" is called twice. Its directory structure is:

	/
		dir1/
			file1.txt (size 38000000)
			dir_1/
				file2.txt (size 1234)
		dir2/
			file3.txt (size 20000000)

	and the correct answers should be 1234 (part 1) and 20000000 (part 2).
*/

#define HASHSIZE 100000

char cmd[100];
struct Hash { unsigned int key, size, list_calls; } hash[HASHSIZE] = {0};

int parse(unsigned int key) {
	struct Hash *h = hash + key % HASHSIZE;
	while (h->key && h->key != key) h++;
	h->key = key;

	int newsum = 0;

	while (fgets(cmd, 100, stdin)) {
		if (cmd[0] == '$' && cmd[2] == 'c') {
			if (cmd[5] == '.') break;

			unsigned int key1 = key;
			for (char *c=cmd + 5; *c; c++) key1 = key1 << 5 ^ key1 >> 27 ^ *c;

			newsum += parse(key1);
		} else if (cmd[0] == '$' && cmd[2] == 'l') {
			h->list_calls++;
		} else if (cmd[0] >= '0' && cmd[0] <= '9') {
			if (h->list_calls < 2) newsum += atoi(cmd);
		}
	}

	h->size += newsum;
	return newsum;
}

int main(void) {
	while (getchar() != '\n');
	
	int total = parse(0xC0FFEE), part1 = 0, part2 = total;

	for (int i=0; i<HASHSIZE; i++) {
		if (!hash[i].key) continue;
		if (hash[i].size <= 100000) part1 += hash[i].size;
		if (hash[i].size >= total-40000000 && part2 > hash[i].size) part2 = hash[i].size;
	}

	printf("part 1: %d\npart 2: %d\n", part1, part2);
}