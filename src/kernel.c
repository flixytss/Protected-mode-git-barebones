// src/kernel.c
void print(int y, char* string){
    char* video = (char*) 0xB8000;
    int x = 0;
    while (string[x] != '\0') {
        int offset = (y * 80 + x) * 2;
        video[offset] = string[x];
        video[offset + 1] = 0x07;
        x++;
    }
}

void _start() {
    char* text = "Hola";

    print(10, text);

    while (1);
}