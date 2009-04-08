# -*- coding: utf-8 -*-

import psyco
import struct
import sys
import logging

# set up logging
logging.basicConfig(level=logging.CRITICAL,
                    format="%(asctime)s %(levelname)-8s %(message)s",
                    filename="um.log",
                    filemode="w")

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
        logging.info("Loading scroll %s" % scrollname)
        operations = map(lambda x: bin2dec(x.strip()), open(scrollname, "r").readlines())
        logging.info("Loaded %d operations" % len(operations))
        zero_array = self.arrays.setdefault(0, [])
        zero_array.extend(operations)

    def spin(self):
        while self.cycle():
            logging.info("Registers: %s" % self.registers)
            for arrayid in self.arrays:
                if arrayid != 0:
                    logging.debug("Array %d: %s" % (arrayid, self.arrays[arrayid]))
        logging.info("Spin cycle complete")

    def cycle(self):
        zero_array = self.arrays.get(0, None)
        if zero_array and self.finger < len(zero_array):
            op = zero_array[self.finger]
            logging.info("Retrieved op %s under execution finger at position %d" % (op, self.finger))
            opnum = self.extract_opnum(op)
            args  = self.extract_args(opnum, op)
            logging.info("Translated op %s to %s%s" % (op, opnum, args))
            self.finger += 1
            result = self.dispatch(opnum, args)
            return result
        return False

    def dispatch(self, opnum, args):
        function, description = self.functions[opnum]
        logging.info("Running operation %d [%s] on %s" % (opnum, description, args))
        return apply(function, args)

    def extract_opnum(self, op):
        return op >> 28

    def extract_args(self, opnum, op):
        if 0 <= opnum <= 12:
            # regular operation
            arg_mask = 0x00000007
            return ( (op & (arg_mask << 6)) >> 6,
                     (op & (arg_mask << 3)) >> 3,
                      op & arg_mask )
        else:
            # special operation
            arg1_mask = 0x0e000000
            arg2_mask = 0x01ffffff
            return ( (op & arg1_mask) >> 25,
                      op & arg2_mask )

    def __cond_move(self, A, B, C):
        if self.registers[C] != 0:
            value = self.registers[B]
            logging.info("Copying value %d from register %d to register %d"
                         % (value, B, A))
            self.registers[A] = value
        return True

    def __array_index(self, A, B, C):
        offset = self.registers[C]
        arrayid = self.registers[B]
        value = self.arrays[arrayid][offset]
        logging.info("Loading value %d from array %d[%d] into register %d"
                     % (value, arrayid, offset, A))
        self.registers[A] = value
        return True

    def __array_amend(self, A, B, C):
        value = self.registers[C]
        offset = self.registers[B]
        arrayid = self.registers[A]
        logging.critical("Storing value %d from register %d in array %d[%d]"
                     % (value, C, arrayid, offset))
        self.arrays[arrayid][offset] = value
        return True

    def __add(self, A, B, C):
        b = self.registers[B]
        c = self.registers[C]
        a = (b + c) % (INT_MAX)
        self.registers[A] = a
        logging.info("[ %d + %d = %d ] Storing result in register %d"
                     % (b, c, a, A))
        return True

    def __multiply(self, A, B, C):
        b = self.registers[B]
        c = self.registers[C]
        a = (b * c) % (INT_MAX)
        self.registers[A] = a
        logging.info("[ %d * %d = %d ] Storing result in register %d"
                     % (b, c, a, A))
        return True

    def __divide(self, A, B, C):
        b = self.registers[B]
        c = self.registers[C]
        a = (b / c) % (INT_MAX)
        self.registers[A] = a
        logging.info("[ %d / %d = %d ] Storing result in register %d"
                     % (b, c, a, A))
        return True

    def __nand(self, A, B, C):
        b = self.registers[B]
        c = self.registers[C]
        a = nand(b, c)
        self.registers[A] = a
        logging.info("[ ~( %d & %d ) = %d ] Storing result in register %d"
                     % (b, c, a, A))
        return True

    def __halt(self, A, B, C):
        logging.info("Halting UM!")
        return False

    def __allocate(self, A, B, C):
        capacity = self.registers[C]
        newarray = [0] * capacity
        arrayid = 1
        while arrayid < INT_MAX:
            if arrayid in self.arrays:
                arrayid += 1
            else:
                logging.info("Allocating array %d with capacity %d" % (arrayid, capacity))
                self.arrays[arrayid] = newarray
                self.registers[B] = arrayid
                return True
        logging.info("Out of array allocation memory! Halting...")
        return False

    def __abandon(self, A, B, C):
        arrayid = self.registers[C]
        logging.info("Abandoning array %d" % arrayid)
        del self.arrays[arrayid]
        return True

    def __output(self, A, B, C):
        character = self.registers[C]
        logging.info("Printing character %d" % character)
        sys.stdout.write(chr(character))
        return True

    def __input(self, A, B, C):
        input = raw_input("> ")
        if input:
            logging.info("Storing input %s in register %d" % (input, C))
            self.registers[C] = input
        else:
            logging.info("EOF reached before input; filling register %d with 1 bits" % C)
            self.registers[C] = INT_MAX - 1
        return True

    def __load(self, A, B, C):
        arrayid = self.registers[B]
        if arrayid != 0:
            self.arrays[0] = self.arrays[arrayid][:]
        self.finger = self.registers[C]
        logging.info("Replacing '0' array with copy of array %d; placing finger at %d"
                     % (arrayid, self.finger))
        return True

    def __orthography(self, A, value):
        logging.info("Storing value %d in register %d" % (value, A))
        self.registers[A] = value
        return True

if __name__ == "__main__":
    um = UniversalMachine("sandmark.umz.bits")
    um.spin()
    print
