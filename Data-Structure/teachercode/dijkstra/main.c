#include <stdio.h>
#include <string.h>
#include "mgraph.h"

#define N 6

int main()
{
    mgraph *mg;
    int s[N], i, j;
    int dist[N];
    pathtype path[N];
    
	int matrix[N][N]={
	  /*		 v0 v1 v2 v3 v4 v5						*/	
	  /*v0*/	{0, 20,15,M, M, M},
	  /*v1*/	{2, 0, 4, M, 10,30},
	  /*v2*/	{M, M, 0, M, M, 10},
	  /*v3*/	{M, M, M, 0, M, M},
	  /*v4*/	{M, M, M, 15,0, 10},
	  /*v5*/	{M, M, M, 4, M, 0}};

    mg = mgraph_create(N, matrix);
    dijkstra(mg, 0, s, dist, path);

    for (i = 1; i < N; i++)
    {
        printf("L=%d   ", dist[i]);
        for (j = 0; j < path[i].end+1; j++)
            printf("V%d ", path[i].pi[j]);
        printf("\n");
    }


	return 0;
}
