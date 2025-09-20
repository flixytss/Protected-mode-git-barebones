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
In 32 Bits, Here we can use 16 Bits registers but not 64 Bits

The 32 Bits registers are: **RAX RDX RCX RBX**
And the segments: **RBP RSP RSI RDI EIP**

## BootLoader

Here, We finally arrive at BootLoader!
The BootLoader File is the boot.asm
That is in the folder **"src"**

### The beginning

``` Assembly
[org 0x7C00]
bits 16

jmp start
nop
```

Firts we define our Bootloader direction `[org 0x7C00]`
And then our Bootloader bits, in this case: 16 Bits `bits 16`
And finally we jump to our main function `jmp start`

``` Assembly
start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
```

In our tag `start` we have a cli that disable the hardware interrupts,
It give more security when we modify the segments
because when we start the bootloader the BIOS can do some interrupts, and while we are modifying
our segments, It can overwrite some importants stuff

``` Assembly
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
```

Firts we revert ax to 0, Then we move the pile to ax

``` Assembly
    mov sp, 0x7C00
```

Then we initialize the pile right in the Bootloader direction

``` Assembly
    mov ax, 0x0003
    int 0x10
```

`mov ax, 0x0003` There we move the register ax to 0x0003
And then we use the interruption 10

#### What's does that do?
It just clean all the text from the screen

#### What's does the interruption 10 do?
This's a quick question, The interruption 10 control the graphics stuff like colors, text, etc

## Boot Sectors

``` Assembly
    mov ah, 0x02
    mov al, 14
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x00
    mov bx, 0x1000
    mov es, bx
    xor bx, bx
    int 0x13
    jc disk_error
```
`mov ah, 0x02` Move ah to the function **"Read Sector"**.  
`mov al, 14` Move al to 14, That's gonna be the numbers of sector to read.  
`mov ch, 0` Move ch to 0, The cylinder.  
`mov cl, 2` Move cl to 2, The Sector to load, The First sector is our bootloader and the next is the kernel.  
`mov dh, 0` Move dh to 0, The head.  
`mov dl, 0x00` Move dl to 0x00, The Next Sector direction.  
`mov bx, 0x1000` Move bx tp 0x1000, offset where to save.  
`mov es, bx` Move es to bx, The Segment.  
`xor bx, bx` Restart bx, Define bx to 0.  
`int 0x13` Call the BIOS with the arguments already passed.  
`jc disk_error` If there is an error, jump to disk_error.  

## Loading the GDT (Global Descriptor Table)
### What's the GDT (Global Descriptor Table)?
Is a fundamental data structure for x86 processors that provides information to the CPU about memory segments
It's obligatory if you want to jump to protected mode

`cli` Disable the interruptions to active the GDT  
`lgdt [gdt_descriptor]` Load the GDT

``` Assembly
    mov eax, cr0
    or eax, 1
    mov cr0, eax
```

Now, Just active the cr0 with a One, Like that slice of code

``` Assembly
    jmp 0x08:protected_mode
```

And finally jump to the protected mode, The `0x08:` indicate that is a FAR jump

## Disk Error Message (Optional)
### Read this if you want more security when you do the Sectors

``` Assembly
disk_error:
    mov si, err_msg
.print_err:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp .print_err
.done:
    jmp $
```

It's just a simple Print that use the variable err_msg db `'Disk Error!', 0`

## GDT Variables
### Here're the GDT Variables

``` Assembly
gdt_start:
    dq 0

    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start -1
    dd gdt_start
```

There isnt explain for this  
Those're just variables that the GDT need