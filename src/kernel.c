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

__attribute__((section(".text.start"))) void _start(){
    print(0, 0, "HolaA");

    while (1);
}
//#include <>