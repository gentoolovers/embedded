OBJS=SeqQueue.o main.o
CC=gcc
CFLAGS= -g -Wall
SeqQueue:$(OBJS)
    $(CC) $(CFLAGS) -o $@ $^
SeqQueue.o:SeqQueue.c SeqQueue.h
    $(CC) $(CFLAGS) -o $@ $^
main.o:main.c SeqQueue.h
    $(CC) $(CFLAGS) -o $@ $^
