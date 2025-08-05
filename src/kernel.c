#define MEMORY_SIZE (1024*8)
#define MAX_BUFFER 64

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
    while (c[pr]!='\0')pr++;
    for(int i=0;i<pr;i++){
        printchar(x+i, y, c[i]);
    }
}
//Memory

//Cursor
void outb(unsigned short port, unsigned char value){__asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));}
void move_cursor(unsigned int x, unsigned int y){
    unsigned short pos = y*80+x;

    outb(0x3D4, 14);
    outb(0x3D5, (unsigned char)(pos>>8));

    outb(0x3D4, 15);
    outb(0x3D5, (unsigned char)(pos&0xFF));
}

//Input
const char scancode_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0
};

static inline char ScanCode_To_Ascii(unsigned char inb){return scancode_ascii[inb];}
static inline unsigned char inb(){
    char result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"((unsigned short)0x60));
    return result;
}
char input(){
    unsigned char scancode = 0;
    char ascii = 0;
    while (1) {
        scancode = inb();
        if (scancode & 0x80) continue;
        if (scancode < sizeof(scancode_ascii)){
            ascii = scancode_ascii[scancode];
            if (ascii != 0) {
                while ((inb() & 0x80) == 0);
                if(scancode==0x1C)return 0x1C;
                if(scancode==0x0E)return 0x0E;
                else return ascii;
            }
        }
    }
}

__attribute__((section(".text.start"))) void _start(){
    int x=0;
    int y=0;
    char s;

    char buffer[MAX_BUFFER];

    int BUFFER = 0;

    while(1){
        s = input();

        if((unsigned char)s==0x0E){
            printchar(x-1, y, ' ');
            move_cursor(x-1, y);
            x--;
            buffer[BUFFER]=' ';
            BUFFER--;
        }
        else if((unsigned char)s==0x1C){
            printchar(x, y, '\n');
            printchar(x, y, ' ');
            y++;
            x=0;
            move_cursor(x, y);
        }
        else{
            printchar(x, y, s);
            move_cursor(x+1, y);
            x++;
            buffer[BUFFER]=s;
            BUFFER++;
        }
    }

    while (1);
}
//#include <> 0x1C