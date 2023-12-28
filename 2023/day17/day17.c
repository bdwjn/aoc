#include <stdio.h>
#include <string.h>
#include <limits.h>

enum Dir { NORTH, EAST, SOUTH, WEST };

char data[100000];              // input file
int w, h;                       // width and height of the map

struct State {
	struct State *prev, *next;
	int cost;
} nodes[200][200][4][11];       // nodes[x][y][direction][steps_taken]

struct State *unvisited[10000]; // for each cost: a linked list of unvisited States

// check if (x,y) is within bounds, then update the node's cost
// and move it to the correct linked list.
void update(int x, int y, int dir, int taken, int oldcost) {
	if (x<0 || x>w-1 || y<0 || y>h-1) {
		return;
	}

	int newcost = oldcost + data[x+y*(w+1)] - '0';

	struct State *s = &(nodes[x][y][dir][taken]);

	if (nodes[x][y][dir][taken].cost <= newcost) {
		return;
	}

	// remove it from the unvisited[oldcost] list
	if (unvisited[oldcost] == s) unvisited[oldcost] = s->next;
	if (s->prev) s->prev->next = s->next;
	if (s->next) s->next->prev = s->prev;

	// and add it to unvisited[cost]
	s->cost = newcost;
	s->prev = NULL;
	s->next = unvisited[newcost];
	unvisited[s->cost] = s;
}

int solve(int minsteps, int maxsteps) {

	struct State *first = (struct State *)nodes;
	
	for (int i=0; i<200*200*4*11; i++) first[i].cost = INT_MAX;

	nodes[0][0][EAST][0]  = (struct State) { .next = NULL, .cost = 0 };
	nodes[0][0][SOUTH][0] = (struct State) { .next = &(nodes[0][0][EAST][0]), .cost = 0 };

	memset(unvisited, 0, sizeof unvisited);
	unvisited[0] = &(nodes[0][0][SOUTH][0]);

	for (int cost=0 ;; cost++) {
		struct State *c;

		while ((c = unvisited[cost])) {
			int x   = (c-first) / 200 / 44;
			int y   = (c-first) / 44 % 200;
			int dir = (c-first) / 11 % 4;
			int taken = (c - first) % 11;

			int dx = (dir == EAST)  ? 1 : (dir == WEST)  ? -1 : 0;
			int dy = (dir == SOUTH) ? 1 : (dir == NORTH) ? -1 : 0;

			if (x == w-1 && y == h-1 && taken>=minsteps) {
				return cost;
			}

			if (taken < maxsteps)
				update(x + dx, y + dy, dir, taken + 1, c->cost);

			if (taken >= minsteps) {
				update(x - dy, y + dx, (dir+1) % 4, 1, c->cost);
				update(x + dy, y - dx, (dir+3) % 4, 1, c->cost);
			}

			unvisited[cost] = unvisited[cost]->next; // remove current node
		}
	}
}

int main(void)
{
	FILE *f = fopen("input", "r");

	int bytes = fread(data, 1, 100000, f);

	w = strchr(data, '\n') - data;
	h = bytes / (w+1);

	printf("Part 1: %d\n", solve(0, 3));
	printf("Part 2: %d\n", solve(4, 10));
}