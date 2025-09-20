# OS from scratch Barebones
## Mini Kernel with idt

Hi there!
Here we have a mini kernel writed in C and a little bit of Asm as the boot and the LDT

## Introducing the bootloader

Ok, Firts we need to understand how does a computer work

What happend when we turn on a computer?

Firts the computer load the BIOS/UEFI (A small firmware program) That take care of the Computer low level settings
The BIOS find if everything is okay, Like the BIOS Battery and stuff like that
Then the BIOS Or UEFI (It depends in what you use) will find a bootable OS
And then run the Bootloader inside the OS
Like the **Bootx64.efi** for 64 bits OS

## What's the Bootloader?

That take care of jump to the protected mode (32 Bits) and then load the kernel
It's on 16 Bits that mean the BIOS support it with the interruptions (int)
They are usually writed in Asm

## What's the 16 Bits?

The Bootloaders begins at 16 Bits
16 Bits are supported by the BIOS with the interruptions (int)
### What're the Interruptions (int)
Are call to the BIOS that support with a low level function
Like print an character in a bootloader

### 16 Bits Registers
In 16 Bits, Obviously we don't use 32 or 64 Bits registers

In 16 Bits we use the registers: **AX DX CX BX**
And the segments: **BP SP SI DI CS DS SS ES FS GS**

## What's the 32 Bits?

Here we do the big jump to do EVERYTHING from scratch
But there're are good news too, We can do whatever we want now
But it gonna be more hard, And you can use 4Gb of ram

### 32 Bits Registers
In 32 Bits, Here we can use 16 Bits registers but no 64 Bits

The 32 Bits registers are: **RAX RDX RCX RBX**
And the segments: **RBP RSP RSI RDI EIP**

## BootLoader

Here, We finally arrive at BootLoader!
The BootLoader File is the boot.asm
That is in the folder **"src"**

### Start:

``` Assembly
[org 0x7C00]
bits 16

jmp start
nop
```

Firts we define our Bootloader direction `[org 0x7C00]`
And then our Bootloader bits, in this case: 16 Bits `bits 16`