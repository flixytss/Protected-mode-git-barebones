#ifndef STR_H
#define STR_H

#include "memory.h"

extern inline void AppendChar(char** text, char letter){
    int pr=0;
    while((*text)[pr]!='\0')pr++;
    char* group = (char*) malloc((pr+2)*sizeof(char));
    for(int i=0;i<pr;i++)group[i]=(*text)[i];
    group[pr]=letter;
    group[pr+1]='\0';
    (*text)=group;
}
extern inline void AppendStr(char** text, char* str){
    int pr=0;
    while(str[pr]!='\0')pr++;
    for(int i=0;i<pr;i++)AppendChar(&(*text), str[i]);
}
extern inline size_t CharLen(char* text){
    size_t pr=0;
    while(text[pr]!='\0')pr++;
    return pr;
}
static inline int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

#endif