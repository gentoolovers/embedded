#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

linkstack stack_create() {
	linkstack s;

	if ((s = (linkstack)malloc(sizeof(listnode))) == NULL) {
		printf("malloc faild\n");
		return NULL;
	}
	s->data = 0;
	s->next = NULL;

	return s;
}

int push(linkstack s, datatype value) {
	linkstack p;

	if ((p = (linkstack)malloc(sizeof(listnode))) == NULL) {
		printf("malloc faild\n");
		return -1;
	}
	p->data = value;

	p->next = s->next;
	s->next = p;
	
	return 0;
}

datatype pop(linkstack s) {
	linkstack p;
	datatype ret;

	p = s->next;
	s->next = p->next;

	ret = p->data;
	free(p);
	p = NULL;

	return ret;
}

int stack_free(linkstack s) {
	linkstack p;

	printf("free:");
	while (s != NULL) {
		p = s;
		s = s->next;
		printf("%d ", p->data);
		free(p);
		p = NULL;
	}
	puts("");

	return 0;
}

/*
 * ret:1 empty   
 * */
int stack_empty(linkstack s) {
	return  (s->next == NULL);
}

datatype top(linkstack s) {
	return (s->next->data);
}
