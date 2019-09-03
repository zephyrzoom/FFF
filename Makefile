CC = $(HOME)/Public/cross/bin/i686-elf-gcc

all:
	${CC} -std=gnu99 -ffreestanding -g -c start.s -o start.o
	${CC} -std=gnu99 -ffreestanding -g -c kernel.c -o kernel.o
	${CC} -ffreestanding -nostdlib -g -T linker.ld start.o kernel.o -o mykernel.elf -lgcc
	qemu-system-i386 -kernel mykernel.elf