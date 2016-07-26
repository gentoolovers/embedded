#include <stdio.h>
#include <stdlib.h>
#include "CircleList.h"

struct Value
{
    CircleListNode header;
    int v;
};

void CircleList_joseph(CircleList* list,int start,int n)
{

    if(list !=NULL)
    {
        int i=0;
        CircleList_Reset(list);

        for(i=0;i<start;i++)
        {
            CircleList_Next(list);

        }

        while(0<CircleList_Length(list))
        {
            int j=0;
            struct Value* show = (struct Value*)CircleList_DeleteNode(list,  CircleList_Current(list));
            printf("%d\n", show->v);

            for(j=0;(j<n) &&( CircleList_Current(list) !=NULL);j++)
            {

                CircleList_Next(list);
            }

        }

    }
}

int main(int argc, char *argv[])
{
    CircleList* list = CircleList_Create();

    struct Value v1;
    struct Value v2;
    struct Value v3;
    struct Value v4;
    struct Value v5;
    struct Value v6;
    struct Value v7;
    struct Value v8;

    int i = 0;

    v1.v = 1;
    v2.v = 2;
    v3.v = 3;
    v4.v = 4;
    v5.v = 5;
    v6.v = 6;
    v7.v = 7;
    v8.v = 8;

    CircleList_Insert(list, (CircleListNode*)&v1, 0);
    CircleList_Insert(list, (CircleListNode*)&v2, 0);
    CircleList_Insert(list, (CircleListNode*)&v3, 0);
    CircleList_Insert(list, (CircleListNode*)&v4, 0);
    CircleList_Insert(list, (CircleListNode*)&v5, 0);
    CircleList_Insert(list, (CircleListNode*)&v6, 0);
    CircleList_Insert(list, (CircleListNode*)&v7, 0);
    CircleList_Insert(list, (CircleListNode*)&v8, 0);

    for(i=0; i<CircleList_Length(list); i++)
    {
        struct Value* pv = (struct Value*)CircleList_Get(list, i);

        printf("%d\n", pv->v);
    }
    puts(" ");
    CircleList_joseph(list,0,2);

    printf("\n");

    CircleList_Destroy(list);

    return 0;
}
