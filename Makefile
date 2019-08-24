AS = $(HOME)/Public/cross/bin/i686-elf-as
GCC = $(HOME)/Public/cross/bin/i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
all:
	${AS} boot.s -o boot.o
	${GCC} ${CFLAGS} -c kernel.c -o kernel.o
	${GCC} -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir
	qemu-system-i386 -cdrom myos.iso
valid:
	if [ -z $(grub-file --is-x86-multiboot myos.bin) ];then	\
		echo "multiboot confirmed";							\
	else													\
		echo "the file is not multiboot";					\
	fi

clean:
	rm -rf *.o