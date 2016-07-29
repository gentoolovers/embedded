#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "sequeue.h"
#include "stack.h"
#include "mgraph.h"

mgraph * mgraph_create(int n, char * v[], int edge[][n]) {
	mgraph * mg;
	int i, j;

	if ((mg = (mgraph *)malloc(sizeof(mgraph))) == NULL) {
		printf("malloc failed\n");
		return NULL;
	}

	bzero(mg, sizeof(mgraph));

	for (i = 0; i < n; i++) {
		mg->vertex[i] = v[i];
		for (j = 0; j < n; j++) {
			mg->edge[i][j] = edge[i][j];
		}
	}
	mg->n = n;

	return mg;
}

int firstadj(mgraph * mg, int v) {
	int i;

	for (i = 0; i < mg->n; i++) {
		if (mg->edge[v][i] ) {
			return i;
		}
	}
	return -1;
}

int nextadj(mgraph * mg, int v, int u) {
	int i;

	for (i = u+1; i < mg->n; i++) {
		if (mg->edge[v][i] ) {
			return i;
		}
	}
	return -1;
}

void _DFS(mgraph *mg, int v) {
	int u;
	static int visited[MAXN] = {0};

	printf("%s ", mg->vertex[v]);
	visited[v] = 1;

	u = firstadj(mg, v);
	while (u != -1) {
		if (!visited[u]) {
			_DFS(mg, u);
		}
		u = nextadj(mg, v, u);
	}
}

void BFS(mgraph * mg, int v) {
	sequeue * sq;
	int visited[MAXN] = {0}, u;
	
	sq = queue_create();

	printf("%s ", mg->vertex[v]);
	visited[v] = 1;
	enqueue(sq, v);

	while (! queue_empty(sq)) {
		v = dequeue(sq);
		u = firstadj(mg, v);
		while (u != -1) {
			if (!visited[u]) {
				printf("%s ", mg->vertex[u]);
				visited[u] = 1;
				enqueue(sq, u);
			}
			u = nextadj(mg, v, u);
		}
	}
	puts("");
}

void _BFS(mgraph * mg, int v) {
	sequeue * sq;
	int visited[MAXN] = {0}, u;
	
	sq = queue_create();

	printf("%s ", mg->vertex[v]);
	visited[v] = 1;
	enqueue(sq, v);

	while (! queue_empty(sq)) {
		v = dequeue(sq);
		for (u = 0; u < mg->n; u++) {
			if (mg->edge[v][u] && !visited[u]) {
				printf("%s ", mg->vertex[u]);
				visited[u] = 1;
				enqueue(sq, u);
			}
		}
	}
	puts("");
}

/*
 * 1-not cycle  0-cycle  -1-error
 * */
int topsort(mgraph * mg) {
	linkstack s;

	s = stack_create();
	if (s == NULL) {
		return -1;
	}
}

