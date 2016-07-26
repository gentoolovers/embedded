#include <stdio.h>

#include "MGraph.h"

#define N 9
#define SN 6

int main(int argc, const char *argv[])
{
    char *  v[N] = {"V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8"};
    char *  sv[SN] = {"V0", "V1", "V2", "V3", "V4", "V5"};
    /*V0 V1 V2 V3 V4 V5 V6 V7 V8*/
    int edge[N][N] = {
        /*V0*/{1, 1, 1, 0, 0, 0, 0, 0, 1},
        /*V1*/{0, 1, 0, 0, 0, 0, 0, 1, 0},
        /*V2*/{0, 0, 1, 0, 0, 0, 1, 0, 0},
        /*V3*/{0, 0, 0, 1, 0, 1, 0, 0, 0},
        /*V4*/{0, 0, 0, 0, 1, 0, 0, 0, 0},
        /*V5*/{0, 0, 0, 1, 0, 1, 0, 0, 0},
        /*V6*/{0, 0, 1, 0, 0, 0, 1, 0, 0},
        /*V7*/{0, 1, 0, 0, 0, 0, 0, 1, 0},
        /*V8*/{1, 0, 0, 0, 0, 0, 0, 0, 1}};

    int sedge[SN][SN] = {
        /*V0*/{0, 1, 0, 1, 1, 0, },
        /*V1*/{0, 0, 0, 0, 0, 1, },
        /*V2*/{0, 1, 0, 0, 0, 1, },
        /*V3*/{0, 0, 0, 0, 0, 0, },
        /*V4*/{0, 0, 0, 1, 0, 1, },
        /*V5*/{0, 0, 0, 0, 0, 0, }};

    mgraph * mg;
   // int visited[N] = {0};

    mg = mgraph_create(N, v, edge);
  mgraph*  nmg= mgraph_create(SN,sv,sedge);

    topsort(nmg);

    DFS(mg, 0);

   BFS(mg, 0);



    //int (*p)[N];
//  p = edge;

    return 0;
}
