# Kernel

We finally arrived to the kernel! (If you read the Bootloader info)

Here, We gonna be using the kernel file (**kernel.c**) that is inside of the folder **src**  
And We gonna be using it's Tools files (**The .h files**)

## The basic stuff

We will go step per step, We first learn about how to print an character

### How the screen work

To manipulate the screen, The basic teory is the **framebuffer**
#### What's the framebuffer?
The **framebuffer** is the screen memory direction (**0xB8000**), With that we can manipulate characters, ONLY Characters  
not pixels, We manipulate it like it's just normal RAM  
But it's not simple RAM, It's part of the VGA video controller Dedicated memory

It's just a simple array of 16 Bits, Each 16 Bits value represent a value on the screen  
In the 16 Bits values, The character use 8 Bits from that 16 Bits

What happends with the restants Bits?

The VGA video controller use the 8 Bits restant, To Specify the ForeGround and the BackGround, Both taking 4 bits

### How to put a character on the screen?
Like we said, The **framebuffer** is just an array, We can control it by the same way
```C
__attribute__((section(".text.start"))) void _start(){...}
```
First define the _start function that begin when you call it from the boot  
Then
```C
    char* video = (char*)0xB8000;
```
There we use the **framebuffer** like a char array  
Because it just use char/s

Now to print a character is like this
```C
    video[0]='A';
    video[1]=0x07;
```
First, We define in the variable video the char that we want to print, In this case it would be 'A',
Then we use the magic char (**0x07**)