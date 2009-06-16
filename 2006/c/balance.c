#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// VM opcodes
#define SCIENCE  0
#define MATH     1
#define LOGIC    2
#define PHYSICS  3

// instruction masks
#define opcode(i) ( i & 0b11100000 ) >> 5
#define D(i)      ( i & 0b00010000 ) >> 4
#define S1(i)     ( i & 0b00001100 ) >> 2
#define S2(i)     ( i & 0b00000011 )
#define IMM(i)    ( i & 0b00011111 )

// VM memory size
#define _MEMSIZE 256

typedef unsigned char uchar;
typedef unsigned char instruction;
typedef struct {
    uchar M[_MEMSIZE]; // memory array
    uchar IP;          // instruction pointer
    uchar IS;          // instruction speed
    uchar sR[4];       // source registers
    uchar dR[2];       // destination register
} balance_vm;          //! VM data structure !\\

// number of instructions read
static int STEPS = 0;

// converts signed 5-bit int to signed 8-bit int
char eval_imm(char imm) {
    return (imm >> 4 ? (imm ^ 0b11100000) : imm);
}

// from http://everything2.com/title/counting%25201%2520bits
uchar nbits(int b) {
    uchar n;
    for(n = 0; b; n++) {
        b &= b - 1;
    }
    return n;
}

// print out VM state, from memory to register content
void vm_dump_state(balance_vm *vm) {
    printf("After machine step %d:\n", STEPS);
    printf(" === machine state ===\n");
    printf("mem M:\n");
    int i, j;
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 16; j++) {
            printf("%02X ", vm->M[(i * 16) + j]);
        }
        printf("\n");
    }
    printf("src regs sR: { %02X %02X %02X %02X }\n", vm->sR[0], vm->sR[1], vm->sR[2], vm->sR[3]);
    printf("dst regs dR: { %02X %02X }\n", vm->dR[0], vm->dR[1]);
    printf("IS: %d  IP: %d\n\n", vm->IS, vm->IP);
}

void vm_die(int exit_status) {
    printf("Simulation terminated after %d steps.\n", STEPS);
    exit(exit_status);
}

void vm_shift(balance_vm *vm, uchar bitmask) {
    uchar n = nbits(bitmask);
    if (n == 1) {
        return;
    }
    // make a array of n pointers to registers that need to be shifted
    uchar* a[n];
    a[0] = vm->sR;
    int c = 1;
    // exactly n of these conditionals should execute
    if ((0b010000 & bitmask) >> 4) { a[c] = vm->sR + 1; c++; }  // sR!
    if ((0b001000 & bitmask) >> 3) { a[c] = vm->sR + 2; c++; }  // "
    if ((0b000100 & bitmask) >> 2) { a[c] = vm->sR + 3; c++; }  // "
    if ((0b000010 & bitmask) >> 1) { a[c] = vm->dR    ; c++; }  // dR!
    if ((0b000001 & bitmask)     ) { a[c] = vm->dR + 1;      }  // "
    // shift registers using pointers
    uchar sR0 = vm->sR[0];
    for (c = 0; c < (n - 1); c++) {
        *(a[c]) = *(a[c + 1]);
    }
    // the first element goes to the end!
    *(a[n - 1]) = sR0;
}

void vm_execute(balance_vm *vm, instruction i) {
    printf("Executing Instruction: ");
    switch (opcode(i)) {
        case MATH: {
            char s1 = S1(i);
            char s2 = S2(i);
            char d  = D(i);
            printf("MATH %u %u %u\n", d, s1, s2);
            vm->M[ vm->dR[d+1 % 2] ] = vm->M[ vm->sR[s1+1 % 2] ] - vm->M[ vm->sR[s2+1 % 2] ];
            vm->M[ vm->dR[d] ]       = vm->M[ vm->sR[s1] ]       - vm->M[ vm->sR[s2] ];
            break;
        }
        case LOGIC: {
            char s1 = S1(i);
            char s2 = S2(i);
            char d  = D(i);
            printf("LOGIC %u %u %u\n", d, s1, s2);
            vm->M[ vm->dR[d+1 % 2] ] = vm->M[ vm->sR[s1+1 % 2] ] ^ vm->M[ vm->sR[s2+1 % 2] ];
            vm->M[ vm->dR[d] ]       = vm->M[ vm->sR[s1] ]       & vm->M[ vm->sR[s2] ];
            break;
        }
        case SCIENCE: {
            char imm  = IMM(i);
            signed char e_imm = eval_imm(imm);
            printf("SCIENCE %d (%d)\n", imm, e_imm);
            if ( vm->M[ vm->sR[0] ] != 0 ) {
                vm->IS = e_imm;
            }
            if ( vm->IS == 0 ) {
                vm_die(0);
            }
            break;
        }
        case PHYSICS: {
            uchar imm  = IMM(i);
            signed char e_imm = eval_imm(imm);
            printf("PHYSICS %d (%d)\n", imm, e_imm);
            vm->sR[0] += e_imm;
            // shift registers according to 6-bit bitmask
            if (imm != 0) {
                // sR[0] bit (leftmost) is always set to 1
                vm_shift( vm, 0b100000 + imm );
            }
            break;
        }
    }
}

instruction read_or_die() {
    char str_instr[2];
    int ret = scanf("%s", str_instr);
    if (ret == EOF) {
        printf("EOF detected - returning to...UMIX???\n");
        vm_die(0);
    }
    return (instruction)strtol(str_instr, NULL, 16);
}

// we can override the initial memory and registers using these array literals
uchar _M[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uchar _sR[4] = { 0x00, 0x00, 0x00, 0x00 };
uchar _dR[2] = { 0x00, 0x00 };
uchar _IS = 1;
uchar _IP = 0;

void vm_init(balance_vm *vm) {
    memcpy(vm->M,  _M , _MEMSIZE);
    memcpy(vm->sR, _sR, 4);
    memcpy(vm->dR, _dR, 2);
    vm->IS = _IS;
    vm->IP = _IP;
}

int main() {
    // create and initialize VM
    balance_vm vm;
    vm_init(&vm);

    // execute instructions until EOF
    while (1) {
        vm_dump_state(&vm);
        instruction i = read_or_die();
        vm_execute(&vm, i);
        STEPS++;
    }
    return 0;
}
