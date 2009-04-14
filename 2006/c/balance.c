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

typedef unsigned char instruction;
typedef struct {
    unsigned char M[_MEMSIZE]; // memory array
    unsigned char IP;          // instruction pointer
    unsigned char IS;          // instruction speed
    unsigned char sR[4];       // source registers
    unsigned char dR[2];       // destination register
} balance_vm;         //! VM data structure !\\

// number of instructions read
static int STEPS = 0;

// converts signed 5-bit int to signed 32-bit int
int eval_imm(char imm) {
    return (imm >> 4 ? -(imm & 0b01111) : imm);
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

void vm_execute(balance_vm *vm, instruction i) {
    printf("Executing Instruction: ");
    switch (opcode(i)) {
        case MATH: {
            char s1 = S1(i);
            char s2 = S2(i);
            char d  = D(i);
            printf("MATH %u %u %u\n", d, s1, s2);
            vm->M[ vm->dR[d+1 % 2] ] = vm->M[ vm->sR[s1+1 % 2] ] - vm->M[ vm->sR[s2+1 % 2] ];
            vm->M[ vm->dR[d] ] = vm->M[ vm->sR[s1] ] - vm->M[ vm->sR[s2] ];
            break;
        }
        case LOGIC: {
            char s1 = S1(i);
            char s2 = S2(i);
            char d  = D(i);
            printf("LOGIC %u %u %u\n", d, s1, s2);
            vm->M[ vm->dR[d+1 % 2] ] = vm->M[ vm->sR[s1+1 % 2] ] ^ vm->M[ vm->sR[s2+1 % 2] ];
            vm->M[ vm->dR[d] ] = vm->M[ vm->sR[s1] ] & vm->M[ vm->sR[s2] ];
            break;
        }
        case SCIENCE: {
            char imm  = IMM(i);
            int e_imm = eval_imm(imm);
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
            char imm  = IMM(i);
            int e_imm = eval_imm(imm);
            printf("PHYSICS %d (%d)\n", imm, e_imm);
            // TODO: IMPLEMENT!
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
unsigned char _M[256] = {
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x79, 0x61, 0x6E, 0x67, 0x3A, 0x55, 0x2B, 0x32, 0x36, 0x32, 0x46, 0x3A, 0x2F, 0x68, 0x6F, 0x6D,
    0x65, 0x2F, 0x79, 0x61, 0x6E, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x67, 0x61, 0x72, 0x64, 0x65, 0x6E, 0x65, 0x72, 0x3A, 0x6D, 0x61, 0x74, 0x68, 0x65, 0x6D, 0x61,
    0x6E, 0x74, 0x69, 0x63, 0x61, 0x3A, 0x2F, 0x68, 0x6F, 0x6D, 0x65, 0x2F, 0x67, 0x61, 0x72, 0x64,
    0x65, 0x6E, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x6F, 0x68, 0x6D, 0x65, 0x67, 0x61, 0x3A, 0x62, 0x69, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x69,
    0x6F, 0x6E, 0x61, 0x6C, 0x3A, 0x2F, 0x68, 0x6F, 0x6D, 0x65, 0x2F, 0x6F, 0x68, 0x6D, 0x65, 0x67,
    0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned char _sR[4] = { 0x01, 0x02, 0x03, 0x04 };
unsigned char _dR[2] = { 0x05, 0xFF };
unsigned char _IS = 1;
unsigned char _IP = 0;

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
