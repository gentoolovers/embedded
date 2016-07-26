#include <stdio.h>
#include <stdlib.h>
#include "LinkStack.h"
typedef struct _tag_LinkStack
{

  LinkStackNode header;
  int length;
}TLinkStack;

LinkStack* LinkStack_Create()
{

  TLinkStack* ret=(TLinkStack*)malloc(sizeof(TLinkStack));
  if(ret == NULL)
    {

      printf("malloc LinkStack failed");
    }else{
    ret->length=0;
    ret->header.next=NULL;

  }
  return ret;
}

int LinkStack_Push(LinkStack* stack,LinkListNode* node)
{
  TLinkStack* TStack=(TLinkStack*)stack;
  int ret=(TStack != NULL) && (node != NULL);
  if(ret)
    {
      LinkStackNode* current=(LinkStackNode*)TStack;
      node->next=current->next;
      current->next=node;
      TStack->length++;
    }
  return ret;
}

LinkStackNode* LinkStack_Pop(LinkStack* stack)
{

  TLinkStack* TStack=(TLinkStack*)stack;
  LinkStackNode* ret=NULL;
  if(TStack != NULL && TStack->length>0)
    {

      LinkListNode* current=(LinkListNode*)TStack;
          ret=current->next;

  
    }
}
void* LinkStack_Top(LinkStack* stack)
{

  TLinkStack* TStack=(TLinkStack*)stack;
  LinkStackNode* node=(LinkStackNode*)TStack;
  void* ret=NULL;
  if(TStack !=NULL && node->next != NULL )
    {
      ret=node->next;

    }
  return ret;
}
