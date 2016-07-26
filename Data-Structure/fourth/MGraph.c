#include <stdio.h>
#include <stdlib.h>
#include "MGraph.h"
#include "sequeue.h"


mgraph * mgraph_create(int n, char * v[], int edge[][n])
{
    mgraph* ret=(mgraph*)malloc(sizeof(mgraph));
    int i,j;
    if(ret ==NULL)
    {
        printf("malloc mgraph failed");
        return NULL;

    }else
    {
        ret->n=n;
        for(i=0;i<n;i++)
        {

            ret->vertex[i]=v[i];
            for(j=0;j<n;j++)
            {
                ret->edge[i][j]=edge[i][j];
            }
        }


    }
    return ret;
}

int firstadj(mgraph * mg, int v)
{
    int i=0;

    for(i=0;i<mg->n;i++)
    {

        if(mg->edge[v][i])
        {
            return i;
        }
    }
    return -1;
}

int nextadj(mgraph * mg, int v, int u)
{
    int i;
    for(i=u+1;i<mg->n;i++)
    {

        if(mg->edge[v][i])
        {
            return i;
        }
    }
    return -1;

}

void DFS(mgraph *mg, int v)
{
    int u;
    static int visted[MAXN]={0};
    printf("%s\n",mg->vertex[v]);
    visted[v]=1;
    u=firstadj(mg,v);
    while(u != -1)
    {
        if(!visted[u])
        {
            DFS(mg,u);
        }
        u=nextadj(mg,v,u);
    }


}
void BFS(mgraph * mg, int v) {
    sequeue * sq;
    sq = queue_create();
    static int visted[MAXN]={0};
    int node ,u;
    printf("%s",mg->vertex[v]);
    enqueue(sq,v);
    visted[v]=1;
    while(queue_empty(sq))
    {

        node=dequeue(sq);

       u= firstadj(mg,node);
       while(u != -1)
       {
           if(!visted[u])
           {
               printf("%s",mg->vertex[u]);
               enqueue(sq,u);
               visted[u]=1;
           }

           u=nextadj(mg,node,u);

       }

    }
    puts(" ");

}

/*

|    | V0 | V1 | V2 | V3 | V4 | V5 |
| V0 |  0 |  1 |  0 |  1 |  1 |  0 |
| V1 |  0 |  0 |  0 |  0 |  0 |  1 |
| V2 |  0 |  1 |  0 |  0 |  0 |  1 |
| V3 |  0 |  0 |  0 |  0 |  0 |  0 |
| V4 |  0 |  0 |  0 |  1 |  0 |  1 |
| V5 |  0 |  0 |  0 |  0 |  0 |  0 |

 */
int topsort(mgraph *mg)
{

    sequeue* seq=queue_create();
    static int svisted[MAXN]={0};
    static int gree[MAXN];
    int i,j,k=0,ret=0;

    for(i=0;i<mg->n;i++)
    {
        int count=0;
        for(j=0;j<mg->n;j++)
        {

             count= count +mg->edge[j][k];

        }

        gree[k]=count;

        if(0==count)
        {
            printf("%s\n",mg->vertex[k]);
            svisted[k]=1;
            enqueue(seq,k);
        }
        k++;
    }
    while(!queue_empty(seq) )
    {
        int line = dequeue(seq);
        ret ++;
        for(i=0;i<mg->n;i++)
        {
            if(mg->edge[line][i]==1)
            {
                if((0 ==  --gree[i])&&(svisted[i] != 1))
                {
                    printf("%s\n",mg->vertex[i]);
                    svisted[i]=1;
                    enqueue(seq,i);
                }
            }
        }

    }


    if((ret ==mg->n) ? 1 :0)
    {
        printf("not circle");
    }else
    {
        printf("circle");
    }
    puts(" ");
    puts(" ");

    return ((ret ==mg->n) ? 1 :0);
}
