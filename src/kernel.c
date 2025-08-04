void printchar(unsigned int x, unsigned int y, char c){
    volatile char* video = (volatile char*)0xB8000;
    int offset = (y*80+x)*2;

    video[offset++]=c;
    video[offset++]=0x07;
}
void print(unsigned int x, unsigned int y, const char* c){
    volatile char* video = (volatile char*)0xB8000;
    int offset = (y*80+x)*2;

    int pr=0;
    while(c[pr]!='\0')pr++;

    for(int i=0;i<pr;i++){
        printchar(x, y, c[i]);
        x++;
    }
}
const char scancode_ascii[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'z', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b',
    'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};
char scancode_to_ascii(unsigned char c){
    return scancode_ascii[c];
}
static inline unsigned char inb(unsigned short port){
    unsigned char result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
};
static inline void outb(unsigned short port, unsigned char value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}
void move_cursor(unsigned int x, unsigned int y) {
    unsigned short pos = y * 80 + x;

    outb(0x3D4, 14);
    outb(0x3D5, (unsigned char)(pos >> 8));

    outb(0x3D4, 15);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
}

unsigned char read_scancode(){return inb(0x60);}
unsigned char key = 0;
char key_ascii = 0;
int x=10;
void input(){
    move_cursor(x, 0);

    while (1) {
        key = read_scancode();

        if (key >= sizeof(scancode_ascii)) continue;
        key_ascii = scancode_ascii[key];
        if (key_ascii == 0) continue;

        printchar(x, 0, key_ascii);
        move_cursor(x, 0);
        x++;
        break;
    }
    input();
}
__attribute__((section(".text.start"))) void _start(){
    print(0, 0, "Teclado: ");

    input();

    while (1);
}