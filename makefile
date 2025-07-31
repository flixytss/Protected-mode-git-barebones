# Makefile

all: os.img

os.img: out/boot.bin out/kernel.bin
	dd if=/dev/zero of=os.img bs=512 count=2880
	dd if=out/boot.bin of=os.img conv=notrunc
	dd if=out/kernel.bin of=os.img bs=512 seek=1 conv=notrunc

out/boot.bin: src/boot.asm
	nasm -f bin src/boot.asm -o out/boot.bin

out/kernel.bin: src/kernel.c linker.ld
	i386-elf-gcc -ffreestanding -m32 -c src/kernel.c -o out/kernel.o
	i386-elf-ld -m elf_i386 -T linker.ld -o out/kernel.elf out/kernel.o
	objcopy -O binary out/kernel.elf out/kernel.bin
	truncate -s 512 out/kernel.bin

run: os.img
	qemu-system-i386 -fda os.img

clean:
	rm -rf out *.img
