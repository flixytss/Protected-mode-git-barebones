// src/kernel.c
void print(const char* str, int row, int col) {
    volatile char* video = (volatile char*) 0xB8000;
    int offset = (row * 80 + col) * 2;

    for (int i = 0; str[i] != '\0'; i++) {
        video[offset++] = str[i];   // Caracter
        video[offset++] = 0x07;     // Atributo (blanco sobre negro)
    }
}

void _start() {
    print("Hola", 10, 10);

    while (1);
}