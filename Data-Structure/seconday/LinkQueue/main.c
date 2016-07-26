#include <stdio.h>
#include <stdlib.h>
#include "LinkQueue.h"

int main(int argc, const char *argv[])
{
  linkqueue * lq;

  lq = queue_create();

  enqueue(lq, 1);
  enqueue(lq, 91);
  enqueue(lq, 7);
  enqueue(lq, 86);

  //queue_free(lq);
  //queue_clear(lq);
  
  while (!queue_empty(lq)) {
    printf("%d ", dequeue(lq));
  }
  //queue_free(lq);
  puts("");


  return 0;
}
