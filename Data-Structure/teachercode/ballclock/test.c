#include <stdio.h>
#include<stdlib.h>
#include"stack.h"
#include"linkqueue.h"

int main(int argc, const char *argv[])
{
	linkqueue * lq;
	linkstack s_hour, s_five, s_min;
	int i, t = 0;
	
	if ((lq = queue_create()) == NULL) {
		return 0;
	}
	
	if ((s_hour = stack_create()) == NULL) {
		return 0;
	}

	if ((s_five = stack_create()) == NULL) {
		return 0;
	}

	if ((s_min = stack_create()) == NULL) {
		return 0;
	}

	for (i = 1; i <= 27; i++) {
		enqueue(lq, i);
	}
	
	while (1) {
		t++;
		i = dequeue(lq);
		//...
	}
	printf("t=%d\n", t);
	
	return 0;
}
