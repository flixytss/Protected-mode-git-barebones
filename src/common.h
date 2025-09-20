#ifndef COMMON_H
#define COMMON_H

typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

#define NULL    ((void*)0)

typedef unsigned long long size_t;

static const char scancode_to_ascii[128] = {
    0,    27,  '1', '2', '3', '4', '5', '6', // 0x00 - 0x07
   '7',  '8', '9', '0', '-', '=', '\b', '\t', // 0x08 - 0x0F
   'q',  'w', 'e', 'r', 't', 'y', 'u', 'i', // 0x10 - 0x17
   'o',  'p', '[', ']', '\n', 0,  'a', 's', // 0x18 - 0x1F
   'd',  'f', 'g', 'h', 'j', 'k', 'l', ';', // 0x20 - 0x27
   '\'', '`', 0,  '\\', 'z', 'x', 'c', 'v', // 0x28 - 0x2F
   'b',  'n', 'm', ',', '.', '/', 0,   '*', // 0x30 - 0x37
    0,   ' ', 0,    0,   0,   0,   0,   0,  // 0x38 - 0x3F
    0,    0,  0,    0,   0,   0,   0,   0,  // 0x40 - 0x47
    0,    0,  0,    0,   0,   0,   0,   0,  // 0x48 - 0x4F
    0,    0,  0,    0,   0,   0,   0,   0,  // 0x50 - 0x57
    0,    0,  0,    0,   0,   0,   0,   0   // 0x58 - 0x5F
};

#endif