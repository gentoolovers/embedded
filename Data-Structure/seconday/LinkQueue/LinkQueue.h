typedef int datatype;
typedef struct node {
  datatype data;
  struct node *next;
}listnode,*linklist;

typedef struct {

  linklist front;
  linklist rear;
}linkqueue;
linkqueue*  queue_create();
int queue_empty(linkqueue *lqueue);
int enqueue(linkqueue *lq,datatype value);
datatype dequeue(linkqueue *lqueue);
int queue_clear(linkqueue *lqueue);
