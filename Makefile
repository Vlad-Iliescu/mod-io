# Environment 
CC=gcc
CFLAGS = -O3

DEPS = i2c.h logger.h main.h
OBJ = tmp/i2c.o tmp/logger.o tmp/main.o

# Script to run before the make starts
PRE_MAKE:=$(shell mkdir -p tmp > /dev/null)

# how to buils tmp/*.o objects for *.c files
tmp/%.o: %.c $(DEPS)
	$(CC) -c -Wall -w -o $@ $< $(CFLAGS)

mod-io: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf mod-io tmp
