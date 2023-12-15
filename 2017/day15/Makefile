
all: day15b

run: day15b
	./day15b

day15b: day15b.c get_random.o
	nasm -felf64 get_random.asm
	gcc -o day15b -O3 -no-pie day15b.c get_random.o

day15b-baseline: day15b-baseline.c
	gcc -o day15b-baseline -O3 day15b-baseline.c

get_random.o: get_random.asm vperm_lut.asm
	nasm -felf64 get_random.asm
	
day15b-perf: get_random.o
	gcc -o day15b-perf -O3 -DN=500000000 -no-pie day15b.c get_random.o

perf: day15b-perf
	sudo perf stat -e cycles,instructions,branches,branch-misses,l2_rqsts.references,l2_rqsts.miss ./day15b-perf
