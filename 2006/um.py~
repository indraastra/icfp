# -*- coding: utf-8 -*-

import psyco
from psyco.classes import *
import struct
import sys
import cProfile

psyco.full()

# how many registers the universal machine should employ
N_REGISTERS = 8
# maximum value for a 32 bit integer
INT_MAX = 2**32
# debug mode print statements
DEBUG = True

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

class UniversalMachine:
    def __init__(self, scrollname=None):
        self.arrays = {}
        self.registers = [0] * N_REGISTERS
        self.finger = 0
        if scrollname:
            self.load_scroll(scrollname)

    def load_scroll(self, scrollname):
        operations = [bin2dec(x.strip()) for x in open(scrollname, "r").readlines()]
        zero_array = self.arrays.setdefault(0, [])
        zero_array.extend(operations)

    def spin(self, max=None):
        count = 0
        while self.cycle():
            count += 1
            if count == max:
                return

    def cycle(self):
        if DEBUG:
            print ' '.join(map(str,self.registers))
        op = self.arrays[0][self.finger]
        self.finger += 1
        opnum = op >> 28

        if 0 <= opnum <= 12:
            # regular operation
            A = (op & (0x00000007 << 6)) >> 6
            B = (op & (0x00000007 << 3)) >> 3
            C = op & 0x00000007
            if DEBUG:
                print opnum, "; A:", A, "B:", B, "C:", C
            if opnum == 0:
                if self.registers[C]:
                    self.registers[A] = self.registers[B]
            elif opnum == 1:
                offset = self.registers[C]
                arrayid = self.registers[B]
                self.registers[A] = self.arrays[arrayid][offset]
            elif opnum == 2:
                offset = self.registers[B]
                arrayid = self.registers[A]
                self.arrays[arrayid][offset] = self.registers[C]
            elif opnum == 3:
                self.registers[A] = (self.registers[B] + self.registers[C]) % (INT_MAX)
            elif opnum == 4:
                self.registers[A] = (self.registers[B] * self.registers[C]) % (INT_MAX)
            elif opnum == 5:
                self.registers[A] = (self.registers[B] / self.registers[C]) % (INT_MAX)
            elif opnum == 6:
                self.registers[A] = ~(self.registers[B] & self.registers[C]) % INT_MAX
            elif opnum == 7:
                return False
            elif opnum == 8:
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
            elif opnum == 9:
                del self.arrays[self.registers[C]]
            elif opnum == 10:
                sys.stdout.write(chr(self.registers[C]))
            elif opnum == 11:
                input = raw_input("> ")
                if input:
                    self.registers[C] = input
                else:
                    self.registers[C] = INT_MAX - 1
            elif opnum == 12:
                arrayid = self.registers[B]
                if arrayid != 0:
                    self.arrays[0] = self.arrays[arrayid][:]
                self.finger = self.registers[C]
            return True
        else:
            # special operation
            A = (op & 0x0e000000) >> 25
            value = op & 0x01ffffff
            if DEBUG:
                print opnum, "; A:", A, "value:", value
            if opnum == 13:
                self.registers[A] = value
            return True

def main():
    um = UniversalMachine("sandmark.umz.bits")
    um.spin(max=100000)

if __name__ == "__main__":
    main()
