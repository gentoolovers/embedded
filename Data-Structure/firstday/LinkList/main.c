#include <stdio.h>
#include "LinkList.h"
int main()
{

  LinkList * H = NULL;
  H = LinkList_Create();
  int i = 0;
  for(i = 0; i < 5;i++)
    LinkList_Insert(H,i,0);


  LinkList_Show(H);
  return 0;
}
