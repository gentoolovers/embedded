typedef int datatype;
typedef  void LinkStack ;
typedef _tag_LinkStackNode LinkStackNode;
struct _tag_LinkStackNode {
  LinkStackNode* next;
}

LinkStack* LinkStack_Create();

int LinkStack_Push(LinkStack* stack,LinkListNode* node);
void* LinkStack_Pop(LinkStack* stack);
int LinkStack_Clear(LinkStack* stack);
int LinkStack_Empty(LinkStack* stack);
void* LinkStack_Top(LinkStack* stack);
