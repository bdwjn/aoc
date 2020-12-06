all:
	nasm -felf32 -o day2a.o day2a.asm
	nasm -felf32 -o day2b.o day2b.asm
	ld -melf_i386 -o day2a day2a.o
	ld -melf_i386 -o day2b day2b.o
	rm day2a.o day2b.o

run:
	strace -e exit ./day2a
	strace -e exit ./day2b
