#include <stdio.h>
#include<stdlib.h>
#include"stack.h"
#include"linkqueue.h"

int check_queue(linkqueue * lq) {
	linklist p = lq->front->next;

	while (p && p->next) {
		if (p->data < p->next->data) {
			p = p->next;
		}
		else {
			return 0;
		}
	}
	return 1;
}


int main(int argc, const char *argv[])
{
	linkqueue *  lq;
	linkstack s_hour, s_five, s_min;
	int i, t = 0;
	
	lq = queue_create();
	s_hour = stack_create();
	s_five = stack_create();
	s_min = stack_create();

	if (!lq  || !s_hour || !s_five || !s_min) {
		return 0;
	}

	for (i = 1; i <= 27; i++) {
		enqueue(lq, i);
	}
	
	while (1) {
		t++;
		i = dequeue(lq);

		if (stack_length(s_min) < 4) {
			push(s_min, i);
		}
		else {
			while (!stack_empty(s_min)) {
				enqueue(lq, pop(s_min));
			}
			if (stack_length(s_five) < 11) {
				push(s_five, i);
			}
			else {
				while (!stack_empty(s_five)) {
					enqueue(lq, pop(s_five));
				}
				if (stack_length(s_hour) < 11) {
					push(s_hour, i);
				}
				else {
					while (!stack_empty(s_hour)) {
						enqueue(lq, pop(s_hour));
					}
					enqueue(lq, i);
					if (check_queue(lq)) {
						break;
					}
				}
			}
		}
	}
	printf("t=%d\n", t);
	
	return 0;
}
