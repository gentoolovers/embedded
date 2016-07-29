#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int data;
	struct node * next;
}listnode, *linklist;

linklist list_create(int n) ;
void list_show(linklist H);
void joseph(linklist H, int k, int m);

int main(int argc, const char *argv[])
{
	int n = 8, k = 1, m = 1;
	linklist H;

	if ((H = list_create(n)) == NULL) {
		return 0;
	}
	list_show(H);
	joseph(H, k, m);

	return 0;
}

linklist list_create(int n)  {
	linklist H, r, p;
	int i;

	if (n <= 0) {
		return NULL;
	}

	if ((H = (linklist)malloc(sizeof(listnode))) == NULL) {
		return NULL;
	}
	H->data = 1;
	H->next = H;
	r = H;
	
	i = 2;
	while (i <= n) {
		if ((p = (linklist)malloc(sizeof(listnode))) == NULL) {
			return NULL;
		}
		p->data = i;
	    r->next = p;
		p->next = H;
		r = p;

		i++;
	}
	
	return H;
}

void list_show(linklist H) {
	linklist p = H;

	while (p->next != H) {
		printf("%d ", p->data);
		p = p->next;
	}

	printf("%d\n", p->data);
	puts("");
}


void joseph(linklist H, int k, int m) {
	linklist p = H, q;
	int i;

	while (p->next->data != k) {
		p = p->next;
	}
	printf("%d, k=%d\n", p->data, p->next->data);

	printf("free:");
	while (p->next!= p) {
		for (i = 0; i < m-1; i++) {
			p = p->next;
		}

		q = p->next;
		p->next = q->next;
		printf("%d ", q->data);
		free(q);
	}

	printf("%d\n", p->data);
	free(p);
	p = NULL;
}

