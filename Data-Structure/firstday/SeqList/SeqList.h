#ifndef  _SEQLIST_H
#define  _SEQLIST_H
#define N 50
#define SIZE 100
typedef Book datatype;
typedef struct book {
  int no;
  char name[N];

}Book;

typedef struct {

  DataType data[SIZE];
  int last;
}SeqList;
SeqList* SeqList_Create();
int SeqList_Empty(SeqList *L);
int SeqList_Locate(SeqList *L,datatype value);
int SeqList_Insert(SeqList *L,datatype value,int Pos);
int SeqList_DeLete(SeqList *L,int Pos);
int SeqList_Show(SqList *L);
#endif
