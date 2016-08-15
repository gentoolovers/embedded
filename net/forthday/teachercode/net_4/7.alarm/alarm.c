#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#define err_log(errlog) do{perror(errlog); exit(1);}while(0) 

// ./server  192.168.2.196 10000

#define  N  128

void handler(int sig)
{

}

int main(int argc, const char *argv[])
{

	alarm(10);
	printf("Brefore alarm 10.\n");
	sleep(2);
	alarm(0);
	printf("After alarm 0.\n");

	while(1)
	{
		printf("+++\n");
		sleep(1);
	}

	return 0;
}
