CC = gcc
CFLAGS = -g -pthread
PROG = life

all:$(PROG)

life: life.o
	$(CC) $(CFLAGS) -o life life.o
	chmod a+rx life

life.o: life.c const.h life.h
	$(CC) $(CFLAGS) -c life.c

clean:
	rm -f $(PROG) *.o *~ \#*