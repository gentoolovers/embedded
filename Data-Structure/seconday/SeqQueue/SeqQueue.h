typedef int datatype;
#define N 5

typedef struct {
  datatype data[N];
  int front;
  int rear;
}sequeue;

sequeue * queue_create();
int queue_empty(sequeue *sq);
int inqueue(sequeue * sq, datatype value);
datatype dequeue(sequeue * sq);
int queue_full(sequeue * sq);
int queue_clear(sequeue * sq);
typedef int datatype;
#define N 5
