typedef int datatype;

typedef struct node {
	datatype data;
	struct node * next;
}listnode, * linklist;

typedef struct {
	linklist front;//listnode * front;  //struct node * front;
	linklist rear;
}linkqueue;

linkqueue * queue_create();
int queue_empty(linkqueue * lq);
int enqueue(linkqueue *lq, datatype value);
datatype dequeue(linkqueue *lq);
int queue_clear(linkqueue *lq);
int queue_free(linkqueue *lq);

