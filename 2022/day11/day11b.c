#include <stdio.h>
#include <stdlib.h>

// answer: 21800916620

int main(void)
{
	char line[1024];

	struct Monkey {
		unsigned long items[50], nItems;
		int add, mul, square;
		int test, monkey_true, monkey_false;
		int count;
	} monkeys[100];

	int n=0, mod=1;

	char *p; int num;
	while (fgets(line, 1024, stdin)) {
		monkeys[n] = (struct Monkey){ .nItems=0, .add=0, .mul=1, .square=0, .count=0 };

		if (!fgets(line, 1024, stdin)) return -1; // "  Starting items: 71, 56, 50, 73"
		for (p=line+16; *p!='\n';) {
			monkeys[n].items[monkeys[n].nItems++] = strtol(p+=2, &p, 10);
		}

		if (!fgets(line, 1024, stdin)) return -1; // "  Operation: new = old * 11"

		if (line[25] == 'o') {
			monkeys[n].square = 1;
		} else {
			if (line[23] == '+') monkeys[n].add = strtol(line+25, &p, 10);
			else                 monkeys[n].mul = strtol(line+25, &p, 10);
		}

		if (!fgets(line, 1024, stdin)) return -1; // "  Test: divisible by 13"
    	monkeys[n].test = strtol(line+21, &p, 10);
		mod *= monkeys[n].test;

		if (!fgets(line, 1024, stdin)) return -1; // "    If true: throw to monkey 3"
		monkeys[n].monkey_true = strtol(line+29, &p, 10);

		if (!fgets(line, 1024, stdin)) return -1; // "    If false: throw to monkey 3"
		monkeys[n].monkey_false = strtol(line+30, &p, 10);

		n++;
		if (!fgets(line, 1024, stdin)) break; // last monkey does not have a newline
	}

	unsigned long best = 0, second_best = 0;

	for (int round=0; round<10000; round++) {
		for (int i=0; i<n; i++) {
			for (int item=0; item<monkeys[i].nItems; item++) {
				monkeys[i].count++;
				if (monkeys[i].count>best) best = monkeys[i].count;
				else if (monkeys[i].count>second_best) second_best = monkeys[i].count;

				monkeys[i].items[item] = monkeys[i].items[item] * monkeys[i].mul + monkeys[i].add;
				if (monkeys[i].square) monkeys[i].items[item] *= monkeys[i].items[item];

				monkeys[i].items[item] %= mod;

				int to = (monkeys[i].items[item] % monkeys[i].test) ? monkeys[i].monkey_false : monkeys[i].monkey_true;			
				monkeys[to].items[monkeys[to].nItems++] = monkeys[i].items[item];
			}

			monkeys[i].nItems=0;
		}
	}

	printf("%lu\n", best * second_best);
}
