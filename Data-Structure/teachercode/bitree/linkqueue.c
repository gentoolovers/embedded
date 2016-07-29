#include <stdio.h>
#include <stdlib.h>
#include "linkqueue.h"

linkqueue * queue_create() {
	linklist p;
	linkqueue * lq;

	if ((p = (linklist)malloc(sizeof(listnode))) == NULL) {
		return NULL;
	}
	p->data = 0;
	p->next = NULL;

	if ((lq = (linkqueue*)malloc(sizeof(linkqueue))) == NULL) {
		return NULL;
	}
	lq->front = p;
	lq->rear = p;

//	printf("%p\n", lq);
	return lq;
}

int queue_empty(linkqueue * lq) {
	return (lq->front->next == NULL);
	//return (lq->front == lq->rear);
}

int enqueue(linkqueue *lq, datatype value) {
	linklist p;

	if ((p = (linklist)malloc(sizeof(listnode))) == NULL) {
		return -1;
	}
	p->data = value;
	p->next = NULL;

	lq->rear->next = p;
	lq->rear = p;

	return 0;
}

datatype dequeue(linkqueue *lq) {
	linklist p;
	
	p  = lq->front;
	lq->front = p->next;
	free(p);
	p = NULL;

	return (lq->front->data);
}

int queue_clear(linkqueue *lq) {
	linklist p;

	printf("clear:");
	while (lq->front->next != NULL) {
		p = lq->front;
		lq->front = p->next;
	//	printf("%d ", p->data);
		free(p);
	}

	return 0;
}

int queue_free(linkqueue *lq) {
	linklist p;

	p = lq->front;

	printf("free:");
	while (p != NULL) {
		lq->front = p->next;
		//printf("%d ", p->data);
		free(p);
		p = lq->front;
	}
	puts("");

	printf("%p\n", lq);
	free(lq);

	return 0;
}
