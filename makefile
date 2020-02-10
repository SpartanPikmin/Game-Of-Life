CC = gcc
CFLAGS = -g
PROG = life

all:$(PROG)

life: life.o
			$(CC) -o life life.o

life.o: life.c const.h life.h
			$(CC) $(CFLAGS) -c life.c

clean:
			rm -f $(PROG) *.o *~ \#*