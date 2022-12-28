#include <stdio.h>

unsigned short lut[] = { 0xFF00, 0xFF01, 0xFF02, 0x00FE, 0x00FF, 0, 0x0001, 0x0002, 0x01FE, 0x01FF, 0x0100 };

int main(void) {
	signed char data[4096] = { '\n' }, sum[30] = { 0 }, *p = data + 1, *end, v, *sp, carry;

	if (!fread(data+1, 1, sizeof data - 1, stdin)) return -1;

	do {
		do; while (*++p != '\n');
		end = p;

		sp = sum + sizeof sum - 1;

		p--;
		while (*p != '\n') {
			v = "432567merryxmas!3"[*p - '-'] - '0';
			p--;

			perform_carry: {}
			int l = lut[*sp + v + carry];
			*sp-- = l;
			carry = l >> 8;
		}
		if (carry) { v=5; goto perform_carry; }

		p = end + 1;
	} while (*p);

	for (int i=0, nz=0; i < sizeof sum; i++) {
		nz |= sum[i] && putchar("=-012"[sum[i] + 2]);
	}
}
