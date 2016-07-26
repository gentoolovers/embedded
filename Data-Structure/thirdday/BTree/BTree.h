#ifndef _BTREE_H_
#define _BTREE_H_
typedef char btreenode;

  typedef struct _tag_bitree{
      btreenode data;
      struct _tag_bitree * left;
      struct _tag_bitree * right;
  }bitree;

  bitree * tree_create();
  void preorder(bitree * r);
  void inorder(bitree * r);
  void postorder(bitree * r);
  void layerorder(bitree * r);
bitree* fulltree_create(char ch[],int n,int i);
#endif
