AS = $(HOME)/Public/cross/bin/i686-elf-as
CC = $(HOME)/Public/cross/bin/i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra

OBJS:=boot.o kernel.o
CRTI_OBJ=crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=crtn.o
 
OBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)
INTERNAL_OBJS:=$(CRTI_OBJ) $(OBJS) $(CRTN_OBJ)

all:
	${AS} boot.s -o boot.o
	${CC} ${CFLAGS} -c kernel.c -o kernel.o
	# ${CC} -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
	$(CC) -o myos.bin $(OBJ_LINK_LIST) -nostdlib -lgcc
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
	rm -f $(INTERNAL_OBJS)