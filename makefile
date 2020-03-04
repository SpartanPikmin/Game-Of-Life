CC = gcc
CFLAGS = -g -pthread
CFLAGSMP = -g -fopenmp
PROG = life
PROG2 = lifemp

all:$(PROG) $(PROG2)

life: life.o
	$(CC) $(CFLAGS) -o life life.o
	chmod a+rx life

life.o: life.c const.h life.h
	$(CC) $(CFLAGS) -c life.c

lifemp: lifemp.o
	$(CC) $(CFLAGSMP) -o mplife lifemp.o
	chmod a+rx mplife

lifemp.o: lifemp.c const.h lifemp.h
	$(CC) $(CFLAGSMP) -c lifemp.c

clean:
	rm -f $(PROG) $(PROG2) *.o *~ \#*