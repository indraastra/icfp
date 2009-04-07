    return 0;
                             break; 
                             FINGER = REGISTERS[C];
                             }
                                 printf("Allocated!\n");
                                 memcpy(ZERO, PLATTER_ARRAY(REGISTERS[B]), size);
                                 ZERO = allocate_array(size);
                                 unsigned int size = array_size(PLATTER_ARRAY(REGISTERS[B]));
                             if (REGISTERS[B] != 0) {
                             printf("Allocating elements to copy from platter %u!\n", REGISTERS[B]);
                case 12: { 
                         break;
                case 11: REGISTERS[C] = getchar();
                         break;
                case 10: putchar(REGISTERS[C]);
                         break;
                case 9 : free(PLATTER_ARRAY(REGISTERS[C]));
                         break;
                case 8 : REGISTERS[B] = (unsigned int)allocate_array(REGISTERS[C]);
                         break;
                case 7 : exit(0);
                         break;
                case 6 : REGISTERS[A] = ~(REGISTERS[B] & REGISTERS[C]);
                         break;
                case 5 : REGISTERS[A] =  (REGISTERS[B] / REGISTERS[C]);
                         break;
                case 4 : REGISTERS[A] =  (REGISTERS[B] * REGISTERS[C]);
                         break;
                case 3 : REGISTERS[A] =  (REGISTERS[B] + REGISTERS[C]);
                         break;
                case 2 : PLATTER_ARRAY(REGISTERS[A])[REGISTERS[B]] = REGISTERS[C];
                         break;
                case 1 : REGISTERS[A] = PLATTER_ARRAY(REGISTERS[B])[REGISTERS[C]];
                         break;
                case 0 : if (REGISTERS[C]) REGISTERS[A] = REGISTERS[B];
            switch (opcode) {
            unsigned int C =  op &  0x00000007;
            unsigned int B = (op & (0x00000007 << 3)) >> 3;
            unsigned int A = (op & (0x00000007 << 6)) >> 6;
        if (opcode >= 0 && opcode <= 12) {
        printf("Opcode: %u\n", opcode);
        unsigned int opcode = op >> 28;
        unsigned int op = ZERO[FINGER];
    while (1) {
    exit(0);
    printf("element 0: %u\n", reverseInt(ZERO[0]));
    printf("Size of 0 array: %u\n", array_size(ZERO));
    fread(ZERO, 4, file_size/4, scroll);
    ZERO = allocate_array(file_size/4);
    file_size = st.st_size;
    fstat(fileno(scroll), &st);
    struct stat st;
    int file_size;
    }
        exit(-1);
        printf("Please provide a valid scroll name!\n");
    if (scroll == NULL) {
    FILE* scroll = fopen(argv[1], "rb");
    }
        exit(-1);
        printf("Please provide a scroll to read!\n");
    if (argc < 2) {
int main(int argc, char* argv[]) {
    }
        return (c1 << 24) + (c2 << 16) + (c3 << 8) + c4;
        c4 = (i >> 24) & 255;
        c3 = (i >> 16) & 255;
        c2 = (i >> 8) & 255;
        c1 = i & 255;
    } else {
        return i;
    if (is_bigendian()) {
    unsigned char c1, c2, c3, c4#define is_bigendian() ( (*(char*)&i) == 0 )
const int i = 1;
    return (unsigned int)(*(array - 1));
unsigned int array_size(unsigned int* array) {
    return array + 1;
    array[0] = size;
    unsigned int* array = (unsigned int*) calloc(size + 1, 4);
    //printf("Allocating array for %u+1 elements\n", size);
unsigned int* allocate_array(unsigned int size) {
unsigned int* ZERO;
unsigned int  FINGER = 0;
unsigned int  REGISTERS[8];
#define PLATTER_ARRAY(id) ((unsigned int *)id)
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
    return 0;
    close(scroll);
    }
        FINGER++;
        }
            }
                exit(-1);
                printf("Unrecognized opcode!: %u\n", opcode);
            } else {
                REGISTERS[A] = value;
            if (opcode == 13) {
            unsigned int value =  op & 0x01ffffff;
            unsigned int A     = (op & 0x0e000000) >> 25;
        } else {
            }
                         }
                             break; 
                             FINGER = REGISTERS[C];
                             }
                                 //printf("Allocated!\n");
                                 memcpy(ZERO, PLATTER_ARRAY(REGISTERS[B]), size);
                                 ZERO = allocate_array(size);
                                 unsigned int size = array_size(PLATTER_ARRAY(REGISTERS[B]));
                             if (REGISTERS[B] != 0) {
                             //printf("Allocating elements to copy from platter %u!\n", REGISTERS[B]);
                case 12: { 
                         break;
                case 11: REGISTERS[C] = getchar();
                         break;
                case 10: //putchar(REGISTERS[C]);
                         break;
                case 9 : free(PLATTER_ARRAY(REGISTERS[C]));
                         break;
                case 8 : REGISTERS[B] = (unsigned int)allocate_array(REGISTERS[C]);
                         break;
                case 7 : exit(0);
                         break;
                case 6 : REGISTERS[A] = ~(REGISTERS[B] & REGISTERS[C]);
                         break;
                case 5 : REGISTERS[A] =  (REGISTERS[B] / REGISTERS[C]);
                         break;
                case 4 : REGISTERS[A] =  (REGISTERS[B] * REGISTERS[C]);
                         break;
                case 3 : REGISTERS[A] =  (REGISTERS[B] + REGISTERS[C]);
                         break;
                case 2 : PLATTER_ARRAY(REGISTERS[A])[REGISTERS[B]] = REGISTERS[C];
                         break;
                case 1 : REGISTERS[A] = PLATTER_ARRAY(REGISTERS[B])[REGISTERS[C]];
                         break;
                case 0 : if (REGISTERS[C]) REGISTERS[A] = REGISTERS[B];
            switch (opcode) {
            unsigned int C =  op &  0x00000007;
            unsigned int B = (op & (0x00000007 << 3)) >> 3;
            unsigned int A = (op & (0x00000007 << 6)) >> 6;
        if (opcode >= 0 && opcode <= 12) {
        print_registers(); // IMPLEMENT!
        printf("Opcode: %u\n", opcode);
        printf("Read instruction: %u\n", op);
        unsigned int opcode = op >> 28;
        unsigned int op = ZERO[FINGER];
    while (1) {
    }
        ZERO[j] = reverseInt(ZERO[j]);
    for (j = 0; j < file_size/4; j++) {
    int j;
    fread(ZERO, 4, file_size/4, scroll);
    ZERO = allocate_array(file_size/4);
    file_size = st.st_size;
    fstat(fileno(scroll), &st);
    struct stat st;
    int file_size;
    }
        exit(-1);
        printf("Please provide a valid scroll name!\n");
    if (scroll == NULL) {
    FILE* scroll = fopen(argv[1], "rb");
    }
        exit(-1);
        printf("Please provide a scroll to read!\n");
    if (argc < 2) {
int main(int argc, char* argv[]) {
    }
        return (c1 << 24) + (c2 << 16) + (c3 << 8) + c4;
        c4 = (i >> 24) & 255;
        c3 = (i >> 16) & 255;
        c2 = (i >> 8) & 255;
        c1 = i & 255;
    } else {
        return i;
    if (is_bigendian()) {
    unsigned char c1, c2, c3, c4;
unsigned int reverseInt (unsigned int i) {
