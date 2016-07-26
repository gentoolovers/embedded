OBJS=SeqList.o main.o
cc=gcc
CFLAGS= -g -Wall
SeqList:$(OBJS)
    $(cc) $(CFLAGS)  -o  $@ $^
SeqList.o:SeqList.c SeqList.h
    $(cc) $(CFLAGS) -c $@ $<
main.o:main.c SeqList.h
    $(cc) $(CFLAGS) -c $@ $<
