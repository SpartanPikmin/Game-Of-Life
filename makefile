CC = gcc
CFLAGS = -g -pthread -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
        -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
        -Wdeclaration-after-statement -std=c99 -D_XOPEN_SOURCE=600
CFLAGSMP = -fopenmp
PROG = life
PROG2 = lifemp

all:$(PROG) $(PROG2)

life: life.o
	$(CC) $(CFLAGS) -o life life.o
	chmod a+rx life

life.o: life.c const.h life.h
	$(CC) $(CFLAGS) -c life.c

lifemp: lifemp.o
	$(CC) $(CFLAGSMP) $(CFLAGS) -o mplife lifemp.o
	chmod a+rx mplife

lifemp.o: lifemp.c const.h lifemp.h
	$(CC) $(CFLAGSMP) $(CFLAGS) -c lifemp.c

clean:
	rm -f $(PROG) mplife *.o *~ \#*
