#include <head.h>

int test()
{
	printf("Hello ");
	//exit(10);
	return 10;
	printf("Workd\n");
}

int main(int argc, const char *argv[])
{
	test();
	printf("Main\n");

	exit(EXIT_SUCCESS);
}
