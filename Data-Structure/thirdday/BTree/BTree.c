#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"
#include "LinkQueue.h"
bitree * tree_create()
{
    bitree* ret;
    char ch;
    printf("please input tree");
    scanf("%c",&ch);
    if(ch != '#')
    {
        ret=(bitree*)malloc(sizeof(bitree));

        if(ret !=NULL)
        {

            ret->data=ch;
            ret->left=tree_create();
            ret->right=tree_create();
        }else
        {

            printf("malloc bitree failed");
            return NULL;
        }

    }else
    {

        return NULL;
    }

    return ret;

}
bitree* fulltree_create(char ch[],int n,int i)
{

            bitree* ret=(bitree*)malloc(sizeof(bitree));
            if(ret ==NULL)
            {

                return NULL;
            }else
            {
                ret->data=ch[i];
                if(2 * i <= n)
                {
                    ret->left=fulltree_create( ch, n, 2*i);

                }
                if(2 * i + 1 <=n)
                {
                    ret->right=fulltree_create(ch,n, 2*i+1);
                }

            }

            return ret;


    }



void preorder(bitree * r)
{
    if(r != NULL)
    {

        printf("%c",r->data);
        preorder(r->left);
        preorder(r->right);
    }

}
void inorder(bitree * r)
{
    if(r !=NULL)
    {

        inorder(r->left);
        printf("%c",r->data);
        inorder(r->right);
    }
}
void postorder(bitree * r)
{
    if(r != NULL)
    {

        inorder(r->left);
        inorder(r->right);
        printf("%c",r->data);
    }
}
void layerorder(bitree * r)
{

    if(r != NULL)
    {
        linkqueue* queue=queue_create();
        enqueue(queue,r);
        while(!queue_empty(queue))
        {
            bitree* trNode=dequeue(queue);
            printf("%c",trNode->data);
            if(trNode->left)
            {
                enqueue(queue,trNode->left);
            }

            if(r->right)
            {
                enqueue(queue,trNode->right);
            }

        }


    }
}
