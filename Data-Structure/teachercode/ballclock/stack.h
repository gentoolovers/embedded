typedef int datatype;

typedef struct _node{
	datatype data;
	struct _node * next;
}_listnode, *linkstack;

linkstack stack_create();
int push(linkstack s, datatype value);
datatype pop(linkstack s);
int stack_free(linkstack s);
int stack_empty(linkstack s);
datatype top(linkstack s);
