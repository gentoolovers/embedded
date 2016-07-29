#ifndef _BITREE_H_
#define _BITREE_H_

typedef char _datatype;

typedef struct _node{
	_datatype data;
	struct _node * left;
	struct _node * right;
}bitree;

bitree * tree_create();
bitree * _tree_create(_datatype ch[], int n, int i);
void preorder(bitree * r);
void inorder(bitree * r);
void postorder(bitree * r);
void layerorder(bitree * r);

#endif
