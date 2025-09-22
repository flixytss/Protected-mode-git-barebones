Read this if you already had read the Bootloader md

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
First define the _start function that begin when you call it from the boot, Then
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
Then we use the magic char (**0x07**), It's just a char in hexadecimal

### How to put character in a specified location (X, Y)?
The **framebuffer** is 80x25, 80 rows and 25 colums, Ok First we define the variable "location"
```C
    u16int location = (y*80+x)*2;
```
Why it's multiplied by 2? Because each element is 2 bytes (16 bits) long. If you're indexing an array of 16-bit values, for example, your index would just be y*80+x.  

And with the location defined, To put a char on that location, You should do
```C
    video[location]='A';
    video[location+1]=0x07;
```

### How to print a sentence?
Finally, With all that, We can do our print!

```C
void monitor_put(u8int x, u8int y, char c){
    char* video = (char*)0xB8000;
    u16int location = (y*80+x)*2;
    video[location] = c;
    video[location+1] = 0x07;
}
```
The only thing to explain there is the void function, It's just a void function with the parameters x, y and c as the character to print

```C
void monitor_write(u8int x, u8int y, const char* str){
    size_t str_size = 0;
    while(str[str_size]!='\0')str_size++;

    for(size_t i=0;i < str_size;i++)monitor_put(x+i, y, str[i]);
}
```
`size_t str_size = 0;` The variable that gonna contain the string's size  
`while(str[str_size]!='\0')str_size++;` That gonna make a loop, If the array str in the direction that str_size say. Isnt '\0' (Because on C every String or a char array end with a \0) It gonna increment the variable "str_size"  
`for(size_t i=0;i < str_size;i++)monitor_put(x+i, y, str[i]);` This is a "for" loop (If i isnt str_size then increment it) that print the char that i say of the parameter str

### Outb and inb
After this, A lot of things gonna need the outb and the inb, Like the keyboard input

#### What's outb?
"outb" Send a byte to a specified port, For example:
``` Assembly
_start:
    mov al, 0x3D4
    outb al, 2
```
That Send 2 to the port 0x3D4

#### What's inb?
"inb" Read a port value and save where you say, For example:
``` Assembly
_start:
    inb al, 0x60
```
Here, We read the port 0x60 and save it in the register al

### How to move the cursor?
We are gonna be using the outb, To send bytes!

``` C
void move_cursor(u8int x, u8int y){
    u16int location = (y*80)+x;
    outb(0x3D4, 14);
    outb(0x3D5, location>>8);
    outb(0x3D4, 15);
    outb(0x3D5, location);
}
```
`u16int location = (y*80)+x;` Firts calculate the location with the formula `(y*80)+x`  
`outb(0x3D4, 14);` Indicate the Cursor manager port that we gonna send the higher byte of the location to the receiving port (0x3D5)  
`outb(0x3D5, location>>8);` Send it  
`outb(0x3D4, 15);` Indicate the Cursor manager port that we gonna send the lower byte  
`outb(0x3D5, location);` Send it  

And finally we move our cursor position


##### Why "x>>8" Means the high byte from something?
Look, When we do `u16int x = 0x1234;` That's in binary **00010010 00110100**
The **00010010** is the higher byte and the **00110100**

###### What's does ">>" means?
In the C arithmetic that means move to the right or left (It depends of the sign) the bytes from a binary,
When you do `u16int x = 0x1;` X in binary is **0001**, And when you do `u8int y = x>>4;` Y's **0000**, ¿Why?
Because 0001 move to the right 4 times (0001 -> 0000) give **0000**, If you didn't understood. Find more examples on internet
, It isn't easy to understand at the first try

So when you move 8 times **00010010 00110100** to the right you will have **00000000 00010010**

### How to make an input?
Now we're gonna instead of send a byte, We're gonna read one with **inb**