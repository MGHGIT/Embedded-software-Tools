#include <stdio.h> 
#include <stdlib.h> 
#include <signal.h> 
void myfun(int signum) 
{ 
if(signum==SIGALRM) printf("SIGALRM is captured!\n"); 
if(signum==SIGINT)  printf("SIGINT is captured!\n"); 

if(signum==SIGTSTP)  printf("SIGTSTP is captured!\n");  
} 

int main() 
{ 
	int flag; 
	int pid;
	int childpid;
	int parentpid;
	pid = fork();

	if (pid == -1){
	perror("fork");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0){//child
	puts("This is child process");
	childpid = getpid();
	printf("pid=%d,ppid=%d\n",childpid,getppid());
	exit(EXIT_SUCCESS);
	}
	else{//parent
	puts("This is parant process");
	parentpid = getpid();
	printf("pid=%d,ppid=%d\n",parentpid,getppid());
	sleep(3);//信号延迟3秒
	kill(pid,SIGKILL); //父进程杀死子进程

	printf("you can type  to show\n");
	signal(SIGTSTP, myfun); 
	signal(SIGTSTP, myfun); 
	//while(1)printf("timer is %d",i++);
	//exit(EXIT_SUCCESS);
	}
 
	while(1){ 
	printf("The task is processing...\n"); 
	sleep(1);
	} 
} 
