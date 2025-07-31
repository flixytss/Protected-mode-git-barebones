// src/kernel.c
void _start() {
    char* video = (char*) 0xB8000;
    video[0] = 'K';
    video[1] = 0x07;

    while (1);
}
