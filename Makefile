all:
	mkdir build
	nasm -f bin boot/boot.asm -o build/boot.bin

build:
	nasm -f bin boot/boot.asm -o build/boot.bin

run:
	qemu-system-i386 -drive format=raw,file=build/boot.bin

clean:
	rm -rf build