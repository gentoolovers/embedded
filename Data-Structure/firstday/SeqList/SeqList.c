#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SeqList.h"
SeqList* SeqList_Create()
{

  SeqList* L=NULL;
  if((L=(SeqList*)malloc(sizeof(SeqList))) == NULL)
    {

      printf("malloc failed\n");
      return NULL;
    }
  L->last = -1;
  return L;
}
/*
  如果线性表是空的返回 0,如果不是空的返回线性表的长度
 */
int SeqList_Empty(SeqList *L)
{

  if(L->last == -1)
    {

      return 0; 
    }
  return L->last+1;
}
/*
  假如有这个元素返回该元素的下标,没有返回-1;
 */
int SeqList_Locate(SeqList *L,datatype value)
{
  int i=0;
  for(i=0;i<=L->last,i++)
    {
      if(memcmp(&L->data[i],&value,sizeof(Book)))
        {
          return i;
        }
    }
  return -1;

}
int SeqList_Insert(SeqList *L,datatype value,int Pos)
{
  int ret=(0<=Pos)&&((L->last +1)<sizeof(L));
    if(ret && (Pos>=L->last+1))
      {
        Pos=L->last+1;

      }
    for(i=L->last;i>=Pos;i++)
    {
      L->data[i+1]=L->data[i];

    }
    L->data[Pos]=value;
    L->last++;
    return ret;
}
int SeqList_DeLete(SeqList *L,int Pos)
{

  int ret=(0<=Pos)&&(Pos<=L->last);
  int i=0;

    if(ret)
      {
         for(i=Pos;i<=L->last;i++)
            {

               L->data[i]=L->data[i+1];
            }
             L->last--; 
      }
  return ret;
}
int SeqList_Show(SqList *L)
{
  int i=0;
  for(i=0;i<=L->last;i++)
    {

      printf("book number is %d\nbook name is %s",L->data[i].no,L->data[i].name);
    }
  return 0;

}
