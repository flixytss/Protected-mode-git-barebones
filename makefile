SECTORS = $(shell echo $$(( ( $$(stat -c%s out/kernel.bin) + 511 ) / 512 )))

bake:
	echo $(SECTORS)
	nasm -f bin src/boot.asm -o out/boot.bin

	i386-elf-gcc -ffreestanding -m32 -c src/kernel.c -o out/kernel.o
	i386-elf-ld -T linker.ld -m elf_i386 -o out/kernel.bin out/kernel.o --oformat binary

	dd if=/dev/zero of=os.img bs=512 count=2880
	dd if=out/boot.bin of=os.img bs=512 conv=notrunc
	dd if=out/kernel.bin of=os.img bs=512 seek=1 conv=notrunc

	qemu-system-i386 -fda os.img
clean:
	rm os.img
	rm out/*
all: bake clean