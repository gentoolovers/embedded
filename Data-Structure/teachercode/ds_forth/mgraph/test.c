#include <stdio.h>
#include "mgraph.h"

#define N 9

int main(int argc, const char *argv[])
{
	char * 	v[N] = {"V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8"};
    int edge[N][N]={   //v0 v1 v2 v3 v4 v5 v6 v7 v8
                 /*v0*/ {0, 1, 1, 0, 0, 0, 0, 0, 0},
                 /*v1*/ {1, 0, 0, 1, 0, 1, 0, 0, 0},
                 /*v2*/ {1, 0, 0, 0, 0, 0, 1, 1, 0},
                 /*v3*/ {0, 1, 0, 0, 1, 0, 0, 0, 0},
                 /*v4*/ {0, 0, 0, 1, 0, 1, 0, 0, 1},
                 /*v5*/ {0, 1, 0, 0, 1, 0, 0, 0, 0},
                 /*v6*/ {0, 0, 1, 0, 0, 0, 0, 1, 0},
                 /*v7*/ {0, 0, 1, 0, 0, 0, 1, 0, 0},
                 /*v8*/ {0, 0, 0, 0, 1, 0, 0, 0, 0}
    };
	mgraph * mg;

	mg = mgraph_create(N, v, edge);

	//int visited[N] = {0};
	//DFS(mg, 0, visited);
	_DFS(mg, 0);
	puts("");

	_BFS(mg, 0);

	if (topsort(mg)) {
		printf("not cycle\n");
	}
	else {
		printf("cycle\n");
	}

	//int (*p)[N];
//	p = edge;
#if 0
	
	int u = firstadj(mg, 7);//u=1
	printf("%d %s \n", u, v[u]);//1 V1

	u = nextadj(mg, 7, u);//u=2
	printf("%d %s \n", u, v[u]);//1 V1

#endif

	return 0;
}
