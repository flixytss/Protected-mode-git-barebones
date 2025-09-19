#ifndef MONITOR_H
#define MONITOR_H

typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

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

u16int cursor_x, cursor_y = 0;

static void monitor_put(char c){
    char* video = (char*)0xB8000;
    if(c=='\n'){
        cursor_y++;
        return;
    }
    u16int location = (cursor_y*80+cursor_x)*2;
    video[location]=c;
    video[location+1]=0x07;
    cursor_x++;
}
static void monitor_write(char* text){
    u8int size=0;
    while(text[size]!='\0')size++;
    for(u8int i=0;i<size;i++){
        monitor_put(text[i]);
        if(text[i]=='\n')cursor_y++;
    }
}
static void outb(u16int port, u8int value){
    __asm__ volatile("outb %1, %0" : : "dN"(port), "a"(value));
}
static u8int inb(u16int port) {
    u8int ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
static void move_cursor(){
    u16int cursor_location = cursor_y*80+cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, cursor_location>>8);
    outb(0x3D4, 15);
    outb(0x3D5, cursor_location);
}
static char get_key(){
    while(!(inb(0x64) & 1));
    char sc = inb(0x60);
    if(sc&0x80)return 0;
    return scancode_to_ascii[sc];
}

#endif