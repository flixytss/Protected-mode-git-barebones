#include "libraries/libscratch.h"

typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

//IDT
struct idt_entry_struct{
    u16int base_lo;
    u16int sel;
    u8int alway0;
    u8int flags;
    u16int base_hi;
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct{
    u16int limit;
    u32int base;
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

extern void isr0 ();
extern void isr31();

extern void idt_flush(u32int);
static void init_idt();
static void idt_set_gate(u8int,u32int,u16int,u8int);

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

//outb: send a byte to a specified port

void outb(u16int port, u8int value){
    __asm__ volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

void move_cursor(u8int x, u8int y){
    u16int cursor_location = y*80+x;
    outb(0x3D4, 14);
    outb(0x3D5, cursor_location>>8);
    outb(0x3D4, 15);
    outb(0x3D5, cursor_location);
}
void printchar(u8int x, u8int y, char c){
    char* video = (char*)0xB8000;
    u16int location = (y*80+x)*2;
    video[location]=c;
    video[location+1]=0x07;
}
void print(u8int x, u8int y, const char* text){
    u8int size=0;
    while(text[size]!='\0')size++;
    for(u8int i=0;i<size;i++)printchar(x+i, y, text[i]);
};

__attribute__((section(".text.start"))) void _start(){
    int* new_group = (int*)malloc(3*sizeof(int));
    for(unsigned long i=0;i<3;i++){
        print(0+i, 0, IntToChar(new_group[i]));
    }
    
}