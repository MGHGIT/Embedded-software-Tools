#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(void)
{
	pid_t pid;
	int bound = 100;

	if ((pid = fork()) == -1){
	perror("fork");
	exit(EXIT_FAILURE);
	}
	else if(pid == 0){
	puts("This is chill process");
	bound += 200;
	printf("pid=%d,ppid=%d\n",getpid(),getppid());
	printf("bound=%d\n\n",bound);
	exit(EXIT_SUCCESS);
	}
	else{
	puts("This is parant process");
	bound += 800;
	printf("pid=%d,ppid=%d\n",getpid(),getppid());
	printf("bound=%d\n\n",bound);
	exit(EXIT_SUCCESS);
	}
	printf("This process is over\n");
}
