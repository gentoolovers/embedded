#include <stdio.h>
#include "mgraph.h"

#define N 6

int main(int argc, const char *argv[])
{
	char * 	v[N] = {"V0", "V1", "V2", "V3", "V4", "V5"};
	int edge[N][N]={
	  /*		 v0 v1 v2 v3 v4 v5						*/	
	  /*v0*/	{0, 1, 0, 1, 1, 0},
	  /*v1*/	{0, 0, 0, 0, 0, 1},
	  /*v2*/	{0, 1, 0, 0, 0, 1},
	  /*v3*/	{0, 0, 0, 0, 0, 0},
	  /*v4*/	{0, 0, 0, 1, 0, 1},
	  /*v5*/	{0, 0, 0, 0, 0, 0}};

	mgraph * mg;
	mg = mgraph_create(N, v, edge);

	if (topsort(mg)) {
		printf("not cycle\n");
	}
	else {
		printf("cycle\n");
	}

	return 0;
}
