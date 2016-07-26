#include <stdio.h>
#include <stdlib.h>
#include "DLinkList.h"
/*
 定义表头,包含数据域,和指针域;
  */
 typedef struct _tag_DLinkList
 {
   DLinkListNode header;
   DLinkListNode* slider;
   int length;
 }TDLinkList;

 /*
 创建空双向循环链表: 从堆里申请空间,申请成功后初始化,表头的前指针(pre)和后指针(next)都指向空,游标 slider 也指向空.因为表头的数据域存放长度,把 DLinkList 定义为 void 比较安全. 

  */
 DLinkList* DLinkList_Create()
 {

   TDLinkList* ret=(TDLinkList*)malloc(sizeof(TDLinkList));
   if(ret !=NULL)
     {
       ret->length=0;
       ret->header.next=NULL;
       ret->header.pre=NULL;
       ret->slider=NULL;

       return ret;
     }else
     {

       printf("malloc DLinkList failed");
       return NULL;
     }

 }

 void DLinkList_Destroy(DLinkList* list)
 {

   free(list);
 }

 void DLinkList_Clear(DLinkList* list)
 {
   TDLinkList* TDList=(TDLinkList*)list;
   if(TDList != NULL)
     {

       TDList->length=0;
       TDList->header.next=NULL;
       TDList->header.pre=NULL;
       TDList->slider=NULL;
     }

 }

 int DLinkList_Length(DLinkList* list)
 {
   TDLinkList* TDList=(TDLinkList*)list;
   int ret= -1;
   if(TDList != NULL)
     {
       ret=TDList->length;
 
     }
   return ret;
 }


 /*
 注意双向链的第一个节点的 pre 指向空,尾节点的 next 指向空,空节点没有 next 和 pre  
 首先是一些合法性检测,然后用 current 移动到要插入位置的前一个位置,然后有 Next 指针指向 current 的下一个位置,因为前指针的链接需要用到这个位置.
 开始插入:因为是双向,只要不是节点一头的两条链都断,链表就不会段.但最好先链接好一个方向再断另一个方向.
 + 首先 current 的 next 指向要插入的元素
 + 其次 node 的 next 指向 Next.这样就完成了一个方向
 + 插前指针需要判断一下 Next 是否为空,为空则没有 pre 指针,假如不为空,则 Next 的 pre 指向 node
 + node 的 pre 指向 current
 + 链表长度为零的时候,slider 指向插入的元素
 + 假如 current 指向的是表头则 node 的 pre 指向空



  */
 int DLinkList_Insert(DLinkList* list,DLinkListNode* node,int pos)
 {

   TDLinkList* TDList=(TDLinkList*)list;
   int ret=(TDList !=NULL) && (node != NULL) && (pos>=0);
   int i=0;
   if(ret)
     {

       DLinkListNode* current=(DLinkListNode*)TDList;
       DLinkListNode* Next=NULL;
       for(i=0;(i<pos) && (current->next !=NULL);i++)
         {

           current=current->next;
         }
       Next=current->next;

       current->next=node;
       node->next=Next;
       if(Next != NULL)
         {

           Next->pre=node;
         }

       node->pre=current;

       if(TDList->length == 0)
         {

           TDList->slider = node;
         }
 
       if(current == (DLinkListNode*)TDList)
         {

           node->pre= NULL;
         }
       TDList->length++;

     }
   return ret;
 }

 DLinkListNode* DLinkList_Get(DLinkList* list,int pos)
 {

   TDLinkList* TDList=(TDLinkList*)list;
   DLinkListNode* ret=NULL;
   int i=0;
   if((TDList != NULL) && (0 <= pos) && (pos<TDList->length))
     {

       DLinkListNode* current=(DLinkListNode*)TDList;
       for(i=0;i<pos;i++)
         {

           current=current->next;
         }
       ret=current->next;
     }
   return ret;
 }
 /*
   +   删除头节点,下一个元素的 pre 指向空
   +   删除尾节点,Next(即 NULL)没有 pre
   +   删除游标 slider 所在的节点时,slider 指向下一个元素
  */
 DLinkListNode* DLinkList_Delete(DLinkList* list,int pos)
 {

   TDLinkList* TDList=(TDLinkList*)list;
   DLinkListNode* ret=NULL;
   int i=0;
   if((TDList != NULL) && (0<=pos)&& (pos>TDList->length))
     {

       DLinkListNode* current=(DLinkListNode*)TDList;
       DLinkListNode* Next=NULL;
       for(i=0;i<pos;i++)
         {

           current=current->next;
         }

       ret=current->next;
       Next=ret->next;

       current->next=Next;
       if(Next != NULL)
         {

           Next->pre=current;
           if(current ==(DLinkListNode*)TDList)
             {

               Next->pre=NULL;
             }
         }
       if(TDList->slider ==ret)
         {

           TDList->slider=Next;
         }
       TDList->length--;
     }
   return ret;
 }

 DLinkListNode* DLinkList_DeleteNode(DLinkList* list,DLinkListNode* node)
 {
   TDLinkList* TDList=(TDLinkList*)list;
   DLinkListNode* ret= NULL;
   int i=0;
   if(TDList !=NULL)
     {

       DLinkListNode* current =(DLinkListNode*)TDList;
       for(i=0;i<TDList->length;i++)
         {

           if(current->next==node)
             {

               ret=current->next;
               break;
             }
           current=current->next;
         }
       if(ret !=NULL)
         {

           DLinkList_Delete(TDList,i);
         }
     }
   return ret;

 }

 DLinkListNode* DLinkList_Reset(DLinkList* list)
 {

   TDLinkList* TDList=(TDLinkList*)list;
   DLinkListNode* ret=NULL;
   if(TDList != NULL)
     {

       TDList->slider=TDList->header.next;
       ret=TDList->slider;
     }
   return ret;

 }
 DLinkListNode* DLinkList_Current(DLinkList* list)
 {

   TDLinkList* TDList=(TDLinkList*)list;
   DLinkListNode* ret=NULL;
   if(TDList != NULL)
     {

       ret=TDList->slider;
     }
   return ret;
 }
 DLinkListNode* DLinkList_Next(DLinkList* list)
 {

   TDLinkList* TDList=(TDLinkList*)list;
   DLinkListNode* ret=NULL;
   if((TDList != NULL) && (TDList->slider != NULL))
     {
       ret=TDList->slider;
       TDList->slider=ret->next;

     }
   return ret;
 }
 DLinkListNode* DLinkList_pre(DLinkList* list)
 {

   TDLinkList* TDList=(TDLinkList*)list;
   DLinkListNode* ret=NULL;
   if((TDList != NULL) && (TDList->slider != NULL))
     {

       ret= TDList->slider;
       TDList->slider =ret->pre;
     }
   return ret;
 }
