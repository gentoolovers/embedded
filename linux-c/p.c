#include <stdio.h>
#include <stdlib.h>

#define N 100

static int compar_int(const void *, const void *);

int main(int argc, const char *argv[])
{
  int a[] = {9, 45, 1, 6, 9, 2}, i;

  qsort(a, sizeof(a)/sizeof(int), sizeof(int), compar_int);

  for (i = 0; i < sizeof(a)/sizeof(int); i++) {
    printf("%d\n", a[i]);
  }

  return 0;
}

 static int compar_int(const void *p1, const void *p2)
       {
        return *((const int *)p1) - *((const int *)p2);

       }
