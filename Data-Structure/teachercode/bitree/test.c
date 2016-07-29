#include <stdio.h>
#include "bitree.h"

int main(int argc, const char *argv[])
{
	bitree * r;
	char ch[] = {' ', 'A', 'B', 'G', 'D', 'F', 'E', 'C', 'K'};
	int n;


	n = sizeof(ch)/sizeof(char)-1;
	//r = tree_create();
	r = _tree_create(ch, n, 1) ;

	preorder(r);
	puts("");

	inorder(r);
	puts("");

	postorder(r);
	puts("");

	layerorder(r);
	return 0;
}
