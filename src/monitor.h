#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

u16int cursor_x, cursor_y = 0;

static void monitor_put(char c){
    char* video = (char*)0xB8000;
    if(c=='\n'){
        cursor_y++;
        cursor_x=0;
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