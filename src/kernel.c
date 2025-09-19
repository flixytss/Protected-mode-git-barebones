#include "monitor.h"

static void *memset(void *s, int c, size_t size) {
    unsigned char *p = (unsigned char*)s;
    for (size_t i = 0; i < size; i++) {
        p[i] = (unsigned char)c;
    }
    return s;
}

extern void input();

typedef struct register_t {
    u32int int_code;
    u32int err_code;
} register_t;

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

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

static void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags) {
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].alway0  = 0;
    idt_entries[num].flags   = flags;
}

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void print(u8int x, u8int y, const char* text);

static void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
    idt_ptr.base  = (u32int)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

    idt_set_gate( 0, (u32int)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (u32int)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (u32int)isr2 , 0x08, 0x8E);
    idt_set_gate( 3, (u32int)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (u32int)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (u32int)isr5 , 0x08, 0x8E);
    idt_set_gate( 6, (u32int)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (u32int)isr7 , 0x08, 0x8E);
    idt_set_gate( 8, (u32int)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (u32int)isr9 , 0x08, 0x8E);
    idt_set_gate( 10, (u32int)isr10 , 0x08, 0x8E);
    idt_set_gate( 11, (u32int)isr11 , 0x08, 0x8E);
    idt_set_gate( 12, (u32int)isr12 , 0x08, 0x8E);
    idt_set_gate( 13, (u32int)isr13 , 0x08, 0x8E);
    idt_set_gate( 14, (u32int)isr14 , 0x08, 0x8E);
    idt_set_gate( 15, (u32int)isr15 , 0x08, 0x8E);
    idt_set_gate( 16, (u32int)isr16 , 0x08, 0x8E);
    idt_set_gate( 17, (u32int)isr17 , 0x08, 0x8E);
    idt_set_gate( 18, (u32int)isr18 , 0x08, 0x8E);
    idt_set_gate( 19, (u32int)isr19 , 0x08, 0x8E);
    idt_set_gate( 20, (u32int)isr20 , 0x08, 0x8E);
    idt_set_gate( 21, (u32int)isr21 , 0x08, 0x8E);
    idt_set_gate( 22, (u32int)isr22 , 0x08, 0x8E);
    idt_set_gate( 23, (u32int)isr23 , 0x08, 0x8E);
    idt_set_gate( 24, (u32int)isr24 , 0x08, 0x8E);
    idt_set_gate( 25, (u32int)isr25 , 0x08, 0x8E);
    idt_set_gate( 26, (u32int)isr26 , 0x08, 0x8E);
    idt_set_gate( 27, (u32int)isr27 , 0x08, 0x8E);
    idt_set_gate( 28, (u32int)isr28 , 0x08, 0x8E);
    idt_set_gate( 29, (u32int)isr29 , 0x08, 0x8E);
    idt_set_gate( 30, (u32int)isr30 , 0x08, 0x8E);
    idt_set_gate( 31, (u32int)isr31 , 0x08, 0x8E);

    idt_flush((u32int)&idt_ptr);
}

//outb: send a byte to a specified port

void isr_handler(u16int int_code) {
    monitor_write("Interrupt received!");
}


__attribute__((section(".text.start"))) void _start(){
    init_idt();
    monitor_put('\n');
    monitor_write("Are u crazy gng");

    while(1){
        char c;
        c=get_key();
        monitor_put(c);
    }
}