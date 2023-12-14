
all:
	nasm -felf64 get_random.asm
	gcc -o day15b -O3 -no-pie day15b.c get_random.o

perf:
	sudo perf stat \
		-e cycles,instructions,branches,branch-misses,l2_rqsts.references,l2_rqsts.miss \
		./day15b
