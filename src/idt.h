#ifndef IDT_H
#define IDT_H

#include "common.h"
#include "memory.h"

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

extern void init_idt();

#endif