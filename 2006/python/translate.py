#!/usr/bin/python

import sys
import struct

def translate_scroll(infile, outfile):
    infh = open(infile, "rb")
    outfh = open(outfile, "w")
    integers = []
    while True:
        next = infh.read(4)
        if next:
            integer = struct.unpack(">L", next)[0]
            integers.append(integer)
        else:
            break
    bytes = map(lambda x: bitstring(x) + "\n", integers)
    outfh.writelines(bytes)

def bitstring(integer):
    return __bitstring(integer).zfill(32)

def __bitstring(integer):
    if 0 <= integer <= 1:
        return str(integer)
    else:
        return __bitstring(integer / 2) + str(integer % 2)

if __name__ == "__main__":
    infile = sys.argv[1]
    if len(sys.argv) > 2:
        outfile = sys.argv[2]
    else:
        outfile = infile + ".bits"
    translate_scroll(infile, outfile)
