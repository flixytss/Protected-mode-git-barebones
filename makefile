AS      = nasm
CC      = i386-elf-gcc
LD      = i386-elf-ld
OBJCOPY = i386-elf-objcopy
QEMU    = qemu-system-i386

CFLAGS  = -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib \
          -fno-builtin -fno-exceptions -fno-rtti
LDFLAGS = -T linker.ld -m elf_i386

SRC     = src
BUILD   = build
EMU     = emulate

BOOT    = boot.asm
KERNELC = kernel.c
LDTASM  = ldt.asm

BOOTBIN = $(BUILD)/boot.bin
KERNELELF = $(BUILD)/kernel.elf
KERNELBIN = $(BUILD)/kernel.bin
OSIMG   = $(EMU)/os.img

OBJS = $(BUILD)/kernel.o $(BUILD)/ldt.o

all: $(OSIMG)

$(BOOTBIN): $(SRC)/$(BOOT)
	$(AS) -f bin $< -o $@
$(BUILD)/ldt.o: $(SRC)/$(LDTASM)
	$(AS) -f elf32 $< -o $@
$(BUILD)/kernel.o: $(SRC)/$(KERNELC)
	$(CC) $(CFLAGS) -c $< -o $@
$(KERNELELF): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^
$(KERNELBIN): $(KERNELELF)
	$(OBJCOPY) -O binary $< $@
$(OSIMG): $(BOOTBIN) $(KERNELBIN)
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(BOOTBIN) of=$@ conv=notrunc
	dd if=$(KERNELBIN) of=$@ bs=512 seek=1 conv=notrunc
run: $(OSIMG)
	$(QEMU) -fda $(OSIMG)
clean:
	rm -f $(BUILD)/*
all: clean run