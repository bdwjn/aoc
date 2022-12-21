#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// Part 1: 85616733059734
// Part 2: 3560324848168

#define HSIZE 8191 // 8191 * 16 = ~128 kB, should fit in L2 cache easily

struct Monkey {
	uint32_t id; char op, human;
	int64_t value;
} monkeys[HSIZE] = {0};

int64_t hash(uint32_t id) {
	int idx = id % HSIZE;
	while (monkeys[idx].id && monkeys[idx].id != id) {
		idx=(idx+1) % HSIZE;
	}
	monkeys[idx].id = id;
	return idx;
}

int64_t eval(int idx) {
	if (monkeys[idx].op == '=') {
		monkeys[idx].human = (monkeys[idx].id == *(uint32_t*)"humn");
		return monkeys[idx].value;
	}

	uint32_t op = monkeys[idx].op, idx1 = monkeys[idx].value>>32, idx2 = monkeys[idx].value;

	int64_t val1 = eval(idx1), val2 = eval(idx2);

	monkeys[idx].human = monkeys[idx1].human || monkeys[idx2].human;

	int64_t rval = op=='+' ? val1+val2 : op=='-' ? val1-val2 : op=='*' ? val1*val2 : val1/val2;

	if (!monkeys[idx].human)
		monkeys[idx].value = rval;

	return rval;
}

int64_t evalTo(int idx, int64_t value) {
	if (monkeys[idx].op == '=') return value;

	uint32_t op = monkeys[idx].op, idx1 = monkeys[idx].value>>32, idx2 = monkeys[idx].value;

	if (monkeys[idx1].human) {
		int64_t val2 = monkeys[idx2].value;
		return evalTo(idx1, op=='+' ? value-val2 : op=='-' ? value+val2 : op=='*' ? value/val2 : value*val2);
	} else {
		int64_t val1 = monkeys[idx1].value;
		return evalTo(idx2, op=='+' ? value-val1 : op=='-' ? val1-value : op=='*' ? value/val1 : value*val1);
	}
}

int main(void) {
	char line[1<<10];

	int root=0;
	while (fgets(line, 1<<10, stdin)) {
		uint32_t id = *(uint32_t*)line, idx = hash(id);
		struct Monkey *m = monkeys + hash(id);

		if (id == *(uint32_t*)"root") root = idx;

		if (line[6] <= '9') {
			m->op = '=';
			m->value = atoi(line + 6);
		} else {
			m->op = line[11];
			m->value = ((hash(*(uint32_t*)(line+6)))<<32) | (hash(*(uint32_t*)(line+13)));
		}
	}

	printf("Part 1: %li\n", eval(root));
	
	monkeys[root].op = '-';
	printf("Part 2: %li\n", evalTo(root, 0));
}