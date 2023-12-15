#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Entry {
	char *label;
	int value;
	struct Entry *next;
} map[256] = {0};

int main(void)
{
	char data[100000];
	FILE *f = fopen("input", "r");
	int bytes = fread(data, 1, 100000, f);
	fclose(f);

	unsigned long part1 = 0, part2 = 0, hash = 0;
	char *label = data;

	for (int i=0; i<bytes; i++) {
		if (data[i] == '=' || data[i] == '-') {
			int num = data[i+1] - '0';
			int len = data + i - label;
			
			struct Entry *e = map + hash;
			while (e->next && strncmp(e->next->label, label, len)) e = e->next;

			if (e->next) {
				if (data[i] == '=') {
					e->next->value = num;
				} else {
					struct Entry *tmp = e->next;
					e->next = e->next->next;
					free(tmp);
				}
			} else {
				if (data[i] == '=') {
					e->next = malloc(sizeof *e);
					*(e->next) = (struct Entry) { .label = label, .value = num, .next = NULL};
				}
			}
			label = data + i + 2 + (data[i] == '=');
		}
		if (data[i] == ',' || data[i] == '\n') {
			part1 += hash;
			hash = 0;
		} else {
			hash = (hash + data[i]) * 17 & 0xFF;
		}
	}

	for (int i=0; i<256; i++) {
		if (!map[i].next) continue;
		struct Entry *s = map + i;

		for (int slot = 1; s = s->next; slot++) {
			part2 += (i+1) * slot * s->value;
		}
	}

	printf("Part 1: %lu\n", part1);
	printf("Part 2: %lu\n", part2);
}