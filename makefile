SECTORS = $(shell echo $$(( ( $$(stat -c%s out/kernel.bin) + 511 ) / 512 )))

bake:
	nasm -f bin src/boot.asm -o out/boot.bin

	i386-elf-gcc -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -fno-builtin -fno-exceptions -fno-rtti -c src/kernel.c -o out/kernel.o
	i386-elf-ld -T linker.ld -m elf_i386 -o out/kernel.bin out/kernel.o --oformat binary

	dd if=/dev/zero of=emulate/os.img bs=512 count=2880
	dd if=out/boot.bin of=emulate/os.img bs=512 conv=notrunc
	dd if=out/kernel.bin of=emulate/os.img bs=512 seek=1 conv=notrunc

	qemu-system-i386 -fda emulate/os.img -drive file=emulate/hdd.img,format=raw,index=0,media=disk
clean:
	rm out/*
all: bake clean