Src =		src
Build =		build
.o =		.o
Emulate =	emulate
Libraries =	libraries

gcc =		i386-elf-gcc
ld =		i386-elf-ld
as =		nasm
qemu =		qemu-system-i386
ar =		ar

boot =		boot.asm
kernel =	kernel.c
linker =	linker.ld
tools =		tools.c

#Outs bins
kernel_bin =kernel.bin
kernel_o =	kernel.o
boot_bin =	boot.bin
os =		os.img
kernel_bin_elf=kernel.elf
hdd =		hdd.img

tools_o =	tool.o
libsl =		libsl.a

gcc_flags =	-m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -fno-builtin -fno-exceptions -fno-rtti
qemu_flags =-drive file=${Emulate}/${hdd},format=raw,index=0,media=disk -display gtk

${Build}/${Libraries}/${tools_o}:
	${gcc} ${gcc_flags} -c ${Src}/${Libraries}/${tools} -o ${Build}/${Libraries}/${tools_o}
${Build}/${Libraries}/${libsl}: ${Build}/${Libraries}/${tools_o}
	${ar} rcs $@ $^
${Build}/${.o}/${boot_bin}:
	${as} -f bin ${Src}/${boot} -o ${Build}/${.o}/${boot_bin}
${Build}/${.o}/${kernel_o}: ${Build}/${Libraries}/${libsl}
	${gcc} ${gcc_flags} -c ${Build}/${Libraries}/${libsl} ${Src}/${kernel} -o ${Build}/${.o}/${kernel_o}
${Build}/${.o}/${kernel_bin}: ${Build}/${.o}/${kernel_o} ${Build}/${Libraries}/${libsl}
	${ld} -T ${linker} -m elf_i386 -o ${Build}/${.o}/${kernel_bin_elf} ${Build}/${.o}/${kernel_o}
	${objcopy} -O binary ${Build}/${.o}/${kernel.bin.elf} ${Build}/${.o}/${kernel_bin}
${Emulate}/${os}: ${Build}/${.o}/${kernel_bin} ${Build}/${.o}/${boot_bin}
	dd if=/dev/zero of=${Emulate}/${os} bs=512 count=2880
	dd if=${Build}/${.o}/${boot_bin} of=${Emulate}/${os} bs=512 conv=notrunc
	dd if=${Build}/${.o}/${kernel_bin} of=${Emulate}/${os} bs=512 seek=1 conv=notrunc

run: ${Emulate}/${os}
	${qemu} -fda ${Emulate}/${os} ${qemu_flags}
clean:
	rm ${Build}/${.o}/*
all: clean run
