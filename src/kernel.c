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

//Input
const char scancode_ascii[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'z', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b',
    'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};
static inline char ScanCode_To_Ascii(unsigned char inb){return scancode_ascii[inb];}
static inline unsigned char inb(){
    char result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"((unsigned short)0x60));
    return result;
}
char input(){
    char s=0;
    do{s=inb();}while(s==0);

    printchar(0, 0, ScanCode_To_Ascii(s));
}

__attribute__((section(".text.start"))) void _start(){
    input();

    while (1);
}
//#include <>