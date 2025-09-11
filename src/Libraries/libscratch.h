#pragma once

#define NULL            ((void*)0)
typedef unsigned long long size_t;

void printf(const char* buffer);
void init_memory(void);
void* malloc(unsigned long size);
char* IntToChar(long n);
long scanf(char* buffer);
void CharToInt(char *buffer, int *out);
unsigned long strlen(const char* str);
void* memcpy(void* ptr, void** to, size_t size);
void* realloc(void* ptr, size_t size);