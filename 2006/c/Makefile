CC = gcc
CFLAGS = -O3

all: um balance-vm

um: um.c
	$(CC) $(CFLAGS) $? -o $@

balance-vm: balance.c
	$(CC) $(CFLAGS) $? -o $@

clean:
	rm um
	rm balance-vm
