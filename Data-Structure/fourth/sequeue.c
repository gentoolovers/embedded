#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "sequeue.h"

sequeue * queue_create() {
	sequeue * sq;

	if ((sq = (sequeue *)malloc(sizeof(sequeue))) == NULL) {
		printf("malloc\n");
		return NULL;
	}
	bzero(sq->data, sizeof(sq->data));
	sq->front = sq->rear = 0;
	return sq;
}

int queue_empty(sequeue *sq){
	if (sq->front == sq->rear) {
		return 1;
	}
	else {
		return 0;
	}
}

int enqueue(sequeue * sq, datatype value){
	if ((sq->rear+1) % N == sq->front) {
		printf("full\n");
		return -1;
	}
	sq->data[sq->rear] = value;
	sq->rear = (sq->rear + 1) % N;

	return 0;
}

datatype dequeue(sequeue * sq){
	datatype ret;

	ret = sq->data[sq->front];

	sq->front = (sq->front+1) % N;

	return ret;
}


int queue_full(sequeue * sq){
	if ((sq->rear+1) % N == sq->front) {
		return 1;
	}
	else {
		return 0;
	}
}

int queue_clear(sequeue * sq){
	bzero(sq->data, sizeof(sq->data));
	sq->front = sq->rear;

	return 0;
}
