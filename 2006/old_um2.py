# -*- coding: utf-8 -*-

import psyco
import struct
import sys
import cProfile

# how many registers the universal machine should employ
N_REGISTERS = 8
# maximum value for a 32 bit integer
INT_MAX = 2**32

def bin2dec(bitstring):
    """
    Converts a bitstring to decimal
    """
    return int(bitstring, 2)

def dec2bin(num):
    """
    Converts a decimal number to a bitstring
    """
    return ''.join([str((num >> i) & 1) for i in range(31,-1,-1)])

def nand(n1, n2):
    """
    Nands two integers
    """
    return ~(n1 & n2) % INT_MAX

class UniversalMachine:
    def __init__(self, scrollname=None):
        self.arrays = {}
        self.registers = [0] * N_REGISTERS
        self.functions = [
                          (self.__cond_move,   "Conditional Move"),
                          (self.__array_index, "Array Index"     ),
                          (self.__array_amend, "Array Amendment" ),
                          (self.__add,         "Addition"        ),
                          (self.__multiply,    "Multiplication"  ),
                          (self.__divide,      "Division"        ),
                          (self.__nand,        "Not-And"         ),
                          (self.__halt,        "Halt"            ),
                          (self.__allocate,    "Allocation"      ),
                          (self.__abandon,     "Abandonment"     ),
                          (self.__output,      "Output"          ),
                          (self.__input,       "Input"           ),
                          (self.__load,        "Load Program"    ),
                          (self.__orthography, "Orthography"     ),
                         ]
        self.finger = 0
        if scrollname:
            self.load_scroll(scrollname)

    def load_scroll(self, scrollname):
        operations = map(lambda x: bin2dec(x.strip()), open(scrollname, "r").readlines())
        zero_array = self.arrays.setdefault(0, [])
        zero_array.extend(operations)

    def spin(self):
        i = 0
        while self.cycle() and i < 1000000:
            i += 1
            pass

    def cycle(self):
        op = self.arrays[0][self.finger]
        self.finger += 1
        return self.dispatch(op)

    def dispatch(self, op):
        opnum = op >> 28
        function, description = self.functions[opnum]
        if 0 <= opnum <= 12:
            # regular operation
            A = (op & (0x00000007 << 6)) >> 6
            B = (op & (0x00000007 << 3)) >> 3
            C = op & 0x00000007
            return function(A, B, C)
        else:
            # special operation
            A = (op & 0x0e000000) >> 25
            value = op & 0x01ffffff
            return function(A, value)

    def __cond_move(self, A, B, C):
        if self.registers[C]:
            self.registers[A] = self.registers[B]
        return True

    def __array_index(self, A, B, C):
        offset = self.registers[C]
        arrayid = self.registers[B]
        self.registers[A] = self.arrays[arrayid][offset]
        return True

    def __array_amend(self, A, B, C):
        offset = self.registers[B]
        arrayid = self.registers[A]
        self.arrays[arrayid][offset] = self.registers[C]
        return True

    def __add(self, A, B, C):
        self.registers[A] = (self.registers[B] + self.registers[C]) % (INT_MAX)
        return True

    def __multiply(self, A, B, C):
        self.registers[A] = (self.registers[B] * self.registers[C]) % (INT_MAX)
        return True

    def __divide(self, A, B, C):
        self.registers[A] = (self.registers[B] / self.registers[C]) % (INT_MAX)
        return True

    def __nand(self, A, B, C):
        self.registers[A] = nand(self.registers[B], self.registers[C])
        return True

    def __halt(self, A, B, C):
        return False

    def __allocate(self, A, B, C):
        capacity = self.registers[C]
        newarray = [0] * capacity
        arrayid = 1
        while arrayid < INT_MAX:
            if arrayid in self.arrays:
                arrayid += 1
            else:
                self.arrays[arrayid] = newarray
                self.registers[B] = arrayid
                return True
        return False

    def __abandon(self, A, B, C):
        del self.arrays[self.registers[C]]
        return True

    def __output(self, A, B, C):
        sys.stdout.write(chr(self.registers[C]))
        return True

    def __input(self, A, B, C):
        input = raw_input("> ")
        if input:
            self.registers[C] = input
        else:
            self.registers[C] = INT_MAX - 1
        return True

    def __load(self, A, B, C):
        arrayid = self.registers[B]
        if arrayid != 0:
            self.arrays[0] = self.arrays[arrayid][:]
        self.finger = self.registers[C]
        return True

    def __orthography(self, A, value):
        self.registers[A] = value
        return True

def main():
    um = UniversalMachine("sandmark.umz.bits")
    um.spin()
    print

if __name__ == "__main__":
    main()
