#ifndef _MGRAPH_H_
 #define _MGRAPH_H_

 #define MAXN 64
 typedef char * vtype;
 typedef int adjtype;
 typedef struct {
     vtype vertex[MAXN];//{"V0", "V1",...}
     adjtype edge[MAXN][MAXN];
     int n;
 }mgraph;

 mgraph * mgraph_create(int n, char * v[], int edge[][n]);
 int firstadj(mgraph * mg, int v);
 int nextadj(mgraph * mg, int v, int u);
 void DFS(mgraph *mg, int v);
void BFS(mgraph * mg, int v);
int topsort(mgraph *mg);

 #endif
