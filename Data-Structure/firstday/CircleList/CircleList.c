#include <stdio.h>
#include <stdlib.h>
#include "CircleList.h"

typedef struct CircleList
{
  CircleListNode header;
    CircleListNode* slider;
    int length;
}TCircleList;

CircleList* CircleList_Create()
{
    TCircleList* ret=(TCircleList*)malloc(sizeof(TCircleList));
    if(ret)
    {

        ret->header.next=NULL;
        ret->slider=NULL;
        ret->length=0;
    }else
    {

        printf("malloc CircleList failed");
        return NULL;
    }
    return ret;

}

void CircleList_Destroy(CircleList* list)
{

    free(list);
    list=NULL;
}

void CircleList_Clear(CircleList* list)
{

    TCircleList* CList=(TCircleList*)list;
    if(CList != NULL)
    {

        CList->header.next=NULL;
        CList->slider=NULL;
        CList->length=0;
    }
}

int CircleList_Length(CircleList* list)
{
    TCircleList* CList=(TCircleList*)list;
    int ret=-1;
    if(CList !=NULL)
    {
        ret=CList->length;

    }
    return ret;

}

int CircleList_Insert(CircleList* list, CircleListNode* node, int pos)
{

    TCircleList* CList=(TCircleList*)list;
    int ret=(CList != NULL) && (node != NULL) && (pos >=0);
    int i=0;
    if(ret)
    {
        CircleListNode* current=(CircleListNode*)CList;
        for(i=0;(i<pos) && (current->next !=NULL);i++)
        {

            current=current->next;
        }
        node->next=current->next;
        current->next=node;
        if(0==CList->length)
        {

            CList->slider=node;
        }
        CList->length++;
        if(current ==(CircleListNode*)CList)
        {

            CircleListNode* last=CircleList_Get(CList,CList->length-1);
            last->next=current->next;
        }

    }
    return ret;
}

CircleListNode* CircleList_Get(CircleList* list, int pos)
{

    TCircleList* CList=(TCircleList*)list;
    CircleListNode* ret=NULL;
    int i=0;
    if((CList != NULL) && (0<=pos)&& (CList->length>0))
    {
        CircleListNode* current=(CircleListNode*)CList;
        for(i=0;i<pos;i++)
        {
            current=current->next;

        }
        ret=current->next;

    }
    return ret;
}
/*
删除第一个元素时,出了需要让链表的链跳过要删除的元素之外,还需要让最后一个元素的next指向第一个元素.
删除slider指向的元素时,需要把游标指向下一个元素.
删除整个链表的最后一个元素时,需要把游标指向空,表头的next也指向空.

 */
CircleListNode* CircleList_Delete(CircleList* list, int pos)
{

    TCircleList* CList=(TCircleList*)list;
    CircleListNode* ret=NULL;
    int i=0;
    if((CList != NULL) && (0<=pos) &&(CList->length >0))
    {
        CircleListNode* current=(CircleListNode*)CList;
        CircleListNode* last=NULL;
        for(i=0;i<pos;i++)
        {

            current=current->next;
        }
        if(current ==(CircleListNode*)CList)
        {
            last=CircleList_Get(CList,CList->length-1);

        }
        ret=current->next;
        current->next=ret->next;
        CList->length--;

        if(last != NULL)
        {

            CList->header.next=ret->next;
            last->next=ret->next;
        }
        if(CList->slider ==ret)
        {

            CList->slider=ret->next;
        }
        if(0==CList->length)
        {

            CList->header.next=NULL;
            CList->slider=NULL;
        }

    }
    return ret;
}

CircleListNode* CircleList_DeleteNode(CircleList* list, CircleListNode* node)
{

    TCircleList* CList=(TCircleList*)list;
    CircleListNode* ret=NULL;
    int i=0;
    if((CList != NULL)&& (node != NULL))
    {
        CircleListNode* current=(CircleListNode*)CList;
        for(i=0;i<CList->length;i++)
        {

            if(current->next == node)
            {

                ret=current->next;
                break;
            }
            current=current->next;
        }
        if(ret)
        {
            CircleList_Delete(CList,i);
        }

    }
    return ret;
}

CircleListNode* CircleList_Reset(CircleList* list)
{
    TCircleList* CList=(TCircleList*)list;
    CircleListNode* ret=NULL;
    if(CList !=NULL)
    {

        CList->slider=CList->header.next;
        ret=CList->slider;
    }
    return ret;


}

CircleListNode* CircleList_Current(CircleList* list)
{

    TCircleList* CList=(TCircleList*)list;
    CircleListNode* ret=NULL;
    if(CList != NULL)
    {

        ret=CList->slider;

    }
    return ret;
}

CircleListNode* CircleList_Next(CircleList* list)
{

    TCircleList* CList=(TCircleList*)list;
    CircleListNode* ret=NULL;
    if((CList != NULL)&& (CList->slider !=NULL))
    {
        CList->slider=CList->slider->next;
        ret=CList->slider;
    }
    return ret;
}




int CircleList_GetPos(CircleList* list,CircleListNode* node)
{
    TCircleList* CList=(TCircleList*)list;
    int i=0;
    if((CList != NULL))
    {

        CircleListNode* current=(CircleListNode*)CList;
        for(i=0;i<CList->length;i++)
        {

            if(current->next=node)
            {

                return i;
            }
            current=current->next;
        }

    }
    return -1;

}
