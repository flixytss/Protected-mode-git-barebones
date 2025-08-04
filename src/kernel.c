void printchar(unsigned int x, unsigned int y, char c){
    volatile char* video = (volatile char*)0xB8000;
    int offset = (y*80+x)*2;

    video[offset++]=c;
    video[offset++]=0x07;
}
void print(unsigned int x, unsigned int y, const char* c){
    volatile char* video = (volatile char*)0xB8000;
    int offset = (y*80+x)*2;

    video[offset++]=c;
    video[offset++]=0x07;
}
__attribute__((section(".text.start"))) void _start(){
    printchar(0, 0, 'L');


    while (1);
}