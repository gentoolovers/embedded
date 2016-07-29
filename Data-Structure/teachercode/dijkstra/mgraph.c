#include <stdio.h>
#include <stdlib.h>
#include "mgraph.h"

mgraph *mgraph_create(int n, int matrix[][n])
{
    int i, j;

    mgraph *mg;
    mg = (mgraph *)malloc(sizeof(mgraph));
    mg->n = n;
    
    for (i = 0; i < n; i++)
    {
        mg->vertex[i] = i;
        for (j = 0; j < n; j++)
            mg->edge[i][j] = matrix[i][j];
    }
    
    return mg;
}

void dijkstra(mgraph *mg, int v, int *s, int *dist, pathtype *path)
{
    int i, count, min, u, w;

    for (i = 0; i < mg->n; i++)
    {
        s[i] = 0;
        dist[i] = mg->edge[v][i];
        path[i].pi[0] = v;
        path[i].end = 0;
    }
    s[v] = 1;
    count = 1;

    while (count < mg->n)
    {
        min = M;
        for (u = 0; u < mg->n; u++)
        {
            if (!s[u] && min > dist[u])
            {
                min = dist[u];
                w = u;
            }
        }
        s[w] = 1;
        dist[w] = min;
        path[w].end++;
        path[w].pi[path[w].end] = w;
        count++;

        for (u = 0; u < mg->n; u++)
        {
            if (!s[u] && dist[w]+ mg->edge[w][u] < dist[u])
            {
                dist[u] = dist[w] + mg->edge[w][u];
                path[u] = path[w];
            }
        }
    }
}
