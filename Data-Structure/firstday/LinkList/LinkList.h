typedef void LinkList;
typedef int datatype;
 
LinkList* LinkList_Create();

int LinkList_Insert(LinkList* list,datatype value,int pos);

LinkList* LinkList_Delete(LinkList* list,int pos);

void LinkList_Show(LinkList *list);

int LinkList_Sort(LinkList* list);

int LinkList_Reverse(LinkList* list);
