#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include"LinkQueue.h"
/*
  先在堆空间上申请表头,申请成功后初始化表头
  然后在堆空间上申请头指针和尾指针,申请成功后初始化.
  空队列头指针和尾指针都指向表头

*/
linkqueue* queue_create() {
  linklist list=(linklist)malloc(sizeof(listnode));
  linkqueue* ret=(linkqueue*)malloc(sizeof(linkqueue));

  if(list !=NULL)
    {

      list->data=0;
      list->next=NULL;
    }else
    {
      printf("malloc listnode failed");
      return NULL;
    }


  if(ret !=NULL)
    {

      ret->front=list;
      ret->rear=list;
    }else
    {
      printf("malloc linkqueue failed");
      return NULL;

    }

  return ret;
}


int queue_empty(linkqueue *lqueue) {

  return (lqueue !=NULL) && (lqueue->front ==lqueue->rear ? 1:0);
}

/*
  首先在堆空间上申请元素空间,申请成功后.
  申请成功后赋值,因为是尾插,所以指针域指向空,数据与赋值为传进来的数据
  然后把上个元素的 next 指针指向插入的元素
  最后把标识尾部的 rear 指针指向该元素
*/
int enqueue(linkqueue *lqueue,datatype value) {
  linklist node=(linklist)malloc(sizeof(listnode));
  if(node != NULL)
    {

      node->data=value;
      node->next=NULL;
      lqueue->rear->next=node;
      lqueue->rear=node;
      return 0;
    }else
    {

      printf("malloc linklist failed");
      return -1;
    }

}
/*
   先把表头指向的第一个元素(即要删除释放的元素)赋值给一个临时变量
   然后把 front 指向临时变量的 next(即临时变量的下一个元素),因为 front 指针是用来标识第一个元素的.  
   然后释放临时变量(tmp)的空间,把 tmp 置空

   返回的值是当前表头所值的第一个元素)(即队列的第一个元素)  
*/
datatype dequeue(linkqueue *lqueue) {

  if(lqueue != NULL)
    {
      listnode* tmp=lqueue->front;

      lqueue->front=tmp->next;
      free(tmp);
      tmp=NULL;
    }
  return (lqueue->front->data);
}


/*
  相当与删除所有元素,与删除的原理相同
*/


int queue_clear(linkqueue *lqueue) {

  if(lqueue !=NULL)
    {
      listnode* tmp=lqueue->front;

      while(lqueue->front != NULL)
        {
          tmp=lqueue->front;
          lqueue->front=tmp->next;
          free(tmp);
          tmp=NULL;

        }
    }
  return 0;
}
/*
  清空队列后,再加一步释放 front rear 指针
*/
int queue_free(linkqueue *lqueue) {
  if(lqueue !=NULL)
    {

      listnode* tmp=lqueue->front;
      while(lqueue->front != NULL)
        {
          tmp=lqueue->front;
          lqueue->front=tmp->next;
          free(tmp);
          tmp=NULL;
        }
      free(lqueue);
      lqueue=NULL;
    }
  return 0;

}
