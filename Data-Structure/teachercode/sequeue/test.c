#include <stdio.h>
#include <stdlib.h>
#include "sequeue.h"

int main(int argc, const char *argv[])
{
	sequeue * sq;

	if ((sq = queue_create()) == NULL) {
		return 0;
	}
	
	enqueue(sq, 30);
	enqueue(sq, 60);
	enqueue(sq, 90);
	enqueue(sq, 120);
	enqueue(sq, 150);

	while (!queue_empty(sq)) {
		printf("%d ", dequeue(sq));
	}
	puts("");


	free(sq);
	sq = NULL;

	return 0;
}
