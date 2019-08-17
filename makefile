all:
	mkdir build
	nasm -f bin boot/boot.asm -o build/boot.bin

build:
	nasm -f bin boot/boot.asm -o build/boot.bin

clean:
	rm -rf build