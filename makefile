Src =		src
Build =		build
.o =		.o
Emulate =	emulate

gcc =		i386-elf-gcc
ld =		i386-elf-ld
as =		nasm
qemu =		qemu-system-i386

boot =		boot.asm
kernel =	kernel.c
linker =	linker.ld

#Outs bins
kernel_bin =kernel.bin
kernel_o =	kernel.o
boot_bin =	boot.bin
os =		os.img
hdd =		hdd.img

gcc_flags =	-m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -fno-builtin -fno-exceptions -fno-rtti
qemu_flags =-drive file=${Emulate}/${hdd},format=raw,index=0,media=disk -display gtk

${Build}/${.o}/${boot_bin}:
	${as} -f bin ${Src}/${boot} -o ${Build}/${.o}/${boot_bin}
${Build}/${.o}/${kernel_o}:
	${gcc} ${gcc_flags} -c ${Src}/${kernel} -o ${Build}/${.o}/${kernel_o}
${Build}/${.o}/${kernel_bin}: ${Build}/${.o}/${kernel_o}
	${ld} -T ${linker} -m elf_i386 -o ${Build}/${.o}/${kernel_bin} ${Build}/${.o}/${kernel_o} --oformat binary
${Emulate}/${os}: ${Build}/${.o}/${kernel_bin} ${Build}/${.o}/${boot_bin}
	dd if=/dev/zero of=${Emulate}/${os} bs=512 count=2880
	dd if=${Build}/${.o}/${boot_bin} of=${Emulate}/${os} bs=512 conv=notrunc
	dd if=${Build}/${.o}/${kernel_bin} of=${Emulate}/${os} bs=512 seek=1 conv=notrunc
run: ${Emulate}/${os}
	${qemu} -fda ${Emulate}/${os} ${qemu_flags}
clean:
	rm ${Build}/${.o}/*