/*
 * A UM Implementation in C
 *
 * (much credit for C tricks and tactics used given to ICFP 2006 official
 * solution code)
 *
 */

#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NREGISTERS 8
#define PLATTER_ARRAY(id) (id ? (unsigned int *)id : ZERO)

static const int i = 1;

static unsigned int* ZERO;
static unsigned int  FINGER = 0;
static unsigned int  REGISTERS[NREGISTERS];

// allocates a UM array, with an extra element for the size
unsigned int* allocate_array(unsigned int size) {
    // calloc zeroes out memory
    unsigned int* array = (unsigned int*) calloc(size + 1, 4);
    array[0] = size;
    return array + 1;
}

// frees a UM array
void free_array(unsigned int* array) {
    free(array - 1);
}

// returns the size of a UM array (the first element is the size)
unsigned int array_size(unsigned int* array) {
    return (unsigned int)(*(array - 1));
}

// reverses the endianness of an integer
// from http://www.ibm.com/developerworks/aix/library/au-endianc/index.html?ca=drs-
unsigned int reverseInt(unsigned int i) {
    unsigned char c1, c2, c3, c4;
    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;
    return (c1 << 24) + (c2 << 16) + (c3 << 8) + c4;
}

// dumps the register content to stdout
void print_registers() {
    int j;
    for (j = 0; j < NREGISTERS; j++) {
        if (j == NREGISTERS - 1) {
            printf("%u\n", REGISTERS[j]);
        } else {
            printf("%u ", REGISTERS[j]);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Please provide a scroll to read!\n$ ./um [scroll]\n");
        exit(-1);
    }

    FILE* scroll = fopen(argv[1], "rb");

    if (scroll == NULL) {
        printf("Please provide a valid scroll name!: %s\n", argv[1]);
        exit(-1);
    }

    struct stat st;
    fstat(fileno(scroll), &st);
    int file_size = st.st_size;
    ZERO = allocate_array(file_size/4);
    int bytes_read = fread(ZERO, 4, file_size/4, scroll);
    if (bytes_read != file_size/4) {
    	exit(-1);
    }

    // reverse the endian-ness of the initial program
    int j;
    for (j = 0; j < file_size/4; j++) {
        ZERO[j] = reverseInt(ZERO[j]);
    }

    while (1) {
        unsigned int op = ZERO[FINGER];
        unsigned int opcode = op >> 28;

        FINGER++;
        //printf("Read instruction: %u\n", op);
        //printf("Opcode: %u\n", opcode);
        //printf("Finger: %u\n", FINGER);
        //print_registers();
        //fflush(stdout);

        if (opcode >= 0 && opcode <= 12) {
            unsigned int A = (op & (0x00000007 << 6)) >> 6;
            unsigned int B = (op & (0x00000007 << 3)) >> 3;
            unsigned int C =  op &  0x00000007;
            switch (opcode) {
				// conditional move
                case 0 : if (REGISTERS[C]) REGISTERS[A] = REGISTERS[B];
                         break;
                // array index
                case 1 : REGISTERS[A] = PLATTER_ARRAY(REGISTERS[B])[REGISTERS[C]];
                         break;
				// array amendment
                case 2 : PLATTER_ARRAY(REGISTERS[A])[REGISTERS[B]] = REGISTERS[C];
                         break;
				// addition
                case 3 : REGISTERS[A] =  (REGISTERS[B] + REGISTERS[C]);
                         break;
                // multiplication
                case 4 : REGISTERS[A] =  (REGISTERS[B] * REGISTERS[C]);
                         break;
                // division
                case 5 : REGISTERS[A] =  (REGISTERS[B] / REGISTERS[C]);
                         break;
                // NAND
                case 6 : REGISTERS[A] = ~(REGISTERS[B] & REGISTERS[C]);
                         break;
                // halt
                case 7 : exit(0);
                         break;
                // allocation
                case 8 : REGISTERS[B] = (unsigned int)allocate_array(REGISTERS[C]);
                         break;
                // abandonment
                case 9 : free_array(PLATTER_ARRAY(REGISTERS[C]));
                         break;
				// output
                case 10: putchar(REGISTERS[C]);
                         break;
                // input
                case 11: REGISTERS[C] = getchar();
                         break;
                // load program
                case 12: {
                             //printf("Allocating elements to copy from platter %u!\n", REGISTERS[B]);
                             if (REGISTERS[B] != 0) {
                                 unsigned int size = array_size(PLATTER_ARRAY(REGISTERS[B]));
                                 ZERO = allocate_array(size);
                                 memcpy(ZERO, PLATTER_ARRAY(REGISTERS[B]), size * 4);
                                 //printf("Allocated!\n");
                             }
                             FINGER = REGISTERS[C];
                             break;
                         }
            }
        } else {
            unsigned int A     = (op & 0x0e000000) >> 25;
            unsigned int value =  op & 0x01ffffff;
            // orthography
            if (opcode == 13) {
                REGISTERS[A] = value;
            } else {
                printf("Unrecognized opcode!: %u\n", opcode);
                exit(-1);
            }
        }
        //printf("\n");
    }
    fclose(scroll);
    return 0;
}
