void _start() {
    char* video = (char*)0xB8000;
    video[0]='H';
    video[1]=0x07;
    video[2]='I';
    video[3]=0x07;


    while (1);
}
void print(){}