#include <stdio.h>
#include "BTree.h"
#include "LinkQueue.h"
int main(int argc, const char *argv[])
{
    bitree * r;

    //r = tree_create();

    char ch[]={' ','a','b','c','d','e','f'};
    int n=sizeof(ch)/sizeof(char);
    int i=1;
    r=fulltree_create(ch,n,i);

    preorder(r);
    puts("");

    inorder(r);
    puts("");

    postorder(r);
    puts("");

    layerorder(r);
    return 0;
}
