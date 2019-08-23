AS = $(HOME)/Public/cross/bin/i686-elf-as
GCC = $(HOME)/Public/cross/bin/i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
all:
	${AS} boot.s -o boot.o
	${GCC} ${CFLAGS} -c kernel.c -o kernel.o

clean:
	rm -rf *.o