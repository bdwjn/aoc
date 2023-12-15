#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILENAME "input"
#pragma GCC diagnostic ignored "-Wmultichar"

/*
	Instead of checking one char at a time, causing a lot of branching, this version
	keeps a history of 3 characters and compares them to 'one', 'two', 'thr' etc.

	Uses (non-standard, but widely supported) multichar constants, but that's more
	readable than writing 'one' as 0x6f6e65... And goto, considered harmful, whatever.
*/

int main(void)
{
	unsigned char *data = malloc(1<<30), *p=data;
	uint64_t bytes = fread(data, 1, 1<<30, fopen(FILENAME, "r"));

	unsigned char *eof = data + bytes;

	long part2=0;

	do {
		unsigned char *start = p, *end = strchr(p, '\n');

		int triplet, first = 0, last = 0;

		if (*p >= '1' && *p <= '9') { first = *p - '0'; goto find_last; } else triplet = *p++;
		if (*p >= '1' && *p <= '9') { first = *p - '0'; goto find_last; } else triplet = (triplet<<8) | *p++;
		if (*p >= '1' && *p <= '9') { first = *p - '0'; goto find_last; } else triplet = (triplet<<8) | *p++;

		do {
			if (triplet == 'one'           )              { first = 1;      break; }
            if (triplet == 'two'           )              { first = 2;      break; }
            if (triplet == 'six'           )              { first = 6;      break; }
			if (triplet == 'fou' && *p=='r')              { first = 4;      break; }
			if (triplet == 'fiv' && *p=='e')              { first = 5;      break; }
			if (triplet == 'nin' && *p=='e')              { first = 9;      break; }
			if (triplet == 'sev' && *p=='e' && p[1]=='n') { first = 7;      break; }
			if (triplet == 'thr' && *p=='e' && p[1]=='e') { first = 3;      break; }
			if (triplet == 'eig' && *p=='h' && p[1]=='t') { first = 8;      break; }
			if (*p>='1' && *p<='9'         )              { first = *p-'0'; break; }
			
			triplet = ((triplet << 8) & 0xFFFF00) + *p++;
		} while (1);

find_last:
		p = end - 1;

		if (*p >= '1' && *p <= '9') { last = *p - '0'; goto finish; } else triplet = *p--;
		if (*p >= '1' && *p <= '9') { last = *p - '0'; goto finish; } else triplet = (triplet<<8) | *p--;
		if (*p >= '1' && *p <= '9') { last = *p - '0'; goto finish; } else triplet = (triplet<<8) | *p--;

		do {
			if (triplet == 'eno'           )               { last = 1;      break; }
            if (triplet == 'owt'           )               { last = 2;      break; }
            if (triplet == 'xis'           )               { last = 6;      break; }
			if (triplet == 'ruo' && *p=='f')               { last = 4;      break; }
			if (triplet == 'evi' && *p=='f')               { last = 5;      break; }
			if (triplet == 'eni' && *p=='n')               { last = 9;      break; }
			if (triplet == 'nev' && *p=='e' && p[-1]=='s') { last = 7;      break; }
			if (triplet == 'eer' && *p=='h' && p[-1]=='t') { last = 3;      break; }
			if (triplet == 'thg' && *p=='i' && p[-1]=='e') { last = 8;      break; }
			if (*p>='1' && *p<='9'         )               { last = *p-'0'; break; }
			
			triplet = ((triplet << 8) & 0xFFFF00) + *p--;
		} while (1);

finish:
		p = end + 1;
		part2 += 10 * first + last;
	} while (p != eof);
	
	printf("Part 2: %ld\n", part2);}
