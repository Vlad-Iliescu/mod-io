# Environment 
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin

all:	    mod-io
	
mod-io:	    Tmp/mod-io.o Tmp/i2c.o
	arm-linux-gnueabihf-gcc -Wall -g -w -o mod-io Tmp/mod-io.o Tmp/i2c.o
	
Tmp/mod-io.o:	main.c
	mkdir -p Tmp > /dev/null
	arm-linux-gnueabihf-gcc -c -Wall -g -w -o Tmp/mod-io.o main.c
	
Tmp/i2c.o:	i2c.c
	mkdir -p Tmp > /dev/null
	arm-linux-gnueabihf-gcc -c -I/var/www/anpr_new/anpr_ioboard/ -Wall -g -w -o Tmp/i2c.o i2c.c

# include project implementation makefile
#include nbproject/Makefile-impl.mk

# include project make variables
#include nbproject/Makefile-variables.mk

clean:
	rm -rf mod-io Tmp/mod-io.o Tmp/i2c.o
