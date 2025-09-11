typedef unsigned long long size_t;

void printchar(char* buf);
void scanf(char* buffer);

#define MEMORY_SIZE     1024
#define NULL            ((void*)0)

char memory[MEMORY_SIZE];

typedef struct Block{
    unsigned long size;
    int free;
    struct Block* next;
} Block;

Block* Heap = (void*)memory;

void init_memory(){
    Heap=(Block*)memory;
    Heap->free=1;
    Heap->size=sizeof(memory)-sizeof(Block);
    Heap->next=NULL;
}
void* malloc(unsigned long size){
    Block* curr = Heap;
    while(curr!=NULL){
        if(curr->free&&curr->size>=size){
            if(curr->size >= size + sizeof(Block) + 1){
                Block* new_block = (Block*)((char*)(curr + 1) + size);
                new_block->size = curr->size - size - sizeof(Block);
                new_block->free = 1;
                new_block->next = curr->next;
                curr->size = size;
                curr->next = new_block;
            }
            curr->free=0;
            return (void*)(curr+1);
        }
        curr=curr->next;
    }
    return NULL;
}
void free(void* ptr){
    if(!ptr)return;
    Block* curr = ((Block*)ptr)-1;
    curr->free=1;
}
void printf(const char* buffer){
    unsigned int size = 0;
    while(buffer[size]!='\0')size++;
    for(unsigned int i=0;i<size;i++){
        printchar((char*)&buffer[i]);
    }
}
void CharToInt(char *buffer, int *out){
    int nta;
    *out=0;
    unsigned long n = 0;
    while(buffer[n]!='\0')n++;
    for(unsigned long i=0;i<n;i++){
        if(buffer[i]!='-')nta = buffer[i]-'0';
        else *out=-(*out);
        *out = (*out * 10) + nta;
    }
}
char* IntToChar(long n){
    int negative=0;
    if(n<0){
        negative=1;
        n=-n;
    }
    char* buffer = (char*)malloc(64*sizeof(char));
    if(buffer==NULL)return NULL;
    long digit = n;
    int divisor = 1;
    while(digit>=10){
        digit/=10;
        divisor*=10;
    }
    int i=1;
    if(negative)buffer[0]='-';
    else i=0;
    while(divisor>0){
        int newdigit=n/divisor;
        buffer[i]=(char)newdigit+48;
        n%=divisor;
        divisor/=10;
        i++;
    }
    buffer[i]='\0';
    return buffer;
}
unsigned long strlen(const char* str){
    unsigned long i=0;
    while(str[i]!='\0')i++;
    return i;
}
void* memcpy(void* ptr, void** to, size_t size){
    unsigned char* a = (unsigned char*)ptr;
    unsigned char* b = (unsigned char*)*to;

    for(size_t i=0;i<size;i++) {
        b[i]=a[i];
    }

    return to;
}

void* realloc(void* ptr, size_t size) {
    if (ptr == NULL) {
        return malloc(size);
    }
    if (size == 0) {
        free(ptr);
        return NULL;
    }

    Block* curr = ((Block*)ptr) - 1;

    if (curr->size >= size) {
        return ptr;
    }

    if (curr->next && curr->next->free &&
        curr->size + sizeof(Block) + curr->next->size >= size) {
        
        curr->size += sizeof(Block) + curr->next->size;
        curr->next = curr->next->next;

        if (curr->size >= size) {
            return ptr;
        }
    }

    void* new_ptr = malloc(size);
    if (new_ptr == NULL) {
        return NULL;
    }

    memcpy(new_ptr, ptr, curr->size);

    free(ptr);

    return new_ptr;
}