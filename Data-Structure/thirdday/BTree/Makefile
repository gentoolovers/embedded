OBJS=BTree.o main.o
CC=gcc
CFLAGS= -g -Wall
BTree:BTree.o main.o
    $(CC) $(CFLAGS) -o $@ @^
BTree.o:BTree.c
    $(CC) $(CFLAGS) -c $@ $<
main.o:main.c
    $(CC) $(CFLAGS) -c $@ $<
clean:
    rm *.o BTree
