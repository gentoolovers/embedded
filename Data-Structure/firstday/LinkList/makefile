OBJS=LinkList.o main.o
CFLAGS=-g -Wall
cc=gcc
LinkList:$(OBJS)
    $(cc) $(CFLAGS) -o $@ $^
SeqList.o:SeqList.c
    $(cc) $(CFLAGS) -o $@ $<
main.o:main.c
    $(cc) $(CFLAGS) -o $@ $<
