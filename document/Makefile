CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
-pedantic-errors -fstack-protector-all
/*PUBTESTS = public01 public02 public03 public04 public05 public06 public07*/
PROGS = user_interface

.PHONY: all clean backup

all: $(PUBTESTS)

clean:
        rm -f *.o $(PROGS) *.tmp

$(PROGS): document.o
user_interface: user_interface.o

/*public%: public%.o*/

document.o: document.h
/*public%.o: document.h*/
