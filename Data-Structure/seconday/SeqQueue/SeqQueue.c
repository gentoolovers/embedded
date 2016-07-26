#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SeqQueue.h"

sequeue * queue_create() {
  sequeue* ret=NULL;
  ret=(sequeue*)malloc(sizeof(sequeue));
  if(ret=NULL)
    {

      printf("malloc sequeue failed");
      return ret;
    }
  bzero(ret->data,sizeof(ret->data));
  ret->front=0;
  ret->rear=0;
  return ret;
}
int queue_empty(sequeue *sq) {

  int ret=(sq != NULL);
  if(ret)
    {

      if(sq->front==sq->rear)
        {

          return ret;
        }else{
        return 0;
      }

    }
  return ret;
}
int inqueue(sequeue * sq, datatype value)
{
  int ret=(sq != NULL) && (sq->rear+1)%N != sq->front;
  if(ret)
    {
      sq->data[sq->rear]=value;
      sq->rear=(sq->rear+1)/N;

    }
  return ret;
}
datatype dequeue(sequeue * sq)
{
  int ret=(sq != NULL)&& (sq->front != sq->rear);
  if(ret) {
    sq->rear=sq->data[sq->front];
    sq->front=(sq->front+1)%N;

    }
  return ret;
}
int queue_full(sequeue * sq){
  int ret=(sq != NULL);
  if(ret && (sq->rear+1)%N == sq->front)
    {
      return ret;

    }
  return ret;
}
int queue_clear(sequeue * sq){
    int ret=(sq != NULL);
    if(ret) {

      sq->front == sq->rear;
    }
  return ret;
}
