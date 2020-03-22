#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>
#include<semaphore.h>
#include <pthread.h>
#include<string.h>

#define PIPE_MAX 128
#define SEM_P2C "parent_to_child"
#define SEM_C2P "child_to_parent" 

int main()
{
	pid_t pid;
	int fd[2],fd_s,fd_s2c1,fd_s2c2;
	int ret,status;
	char par_buf[128]={0};
	char *c_sever = "message from sever.c";
	ret = pipe(fd);	


	mkfifo("./client1fifo",0777);
	mkfifo("./client2fifo",0777);
	fd_s2c1 = open("./client1fifo",O_WRONLY);
	fd_s2c2 = open("./client2fifo",O_WRONLY);
	if(fd_s2c1 < 0)
	{
		printf("error!\n");
		return -1;
	}
	
	printf("open client1fifo seccess\n");
	
	pid=fork();
	
	if(0 == pid )
	{
		while(1)
		{

		}
	}
	else if(pid > 0)
	{
		while(1)
		{
			
			printf("input your message to client1:  ");
			fgets(par_buf,PIPE_MAX,stdin);
            
            		write(fd_s2c1,&par_buf,strlen(par_buf));
			printf("client1 writed!\n");
			printf("input your message to client2: ");
			fgets(par_buf,PIPE_MAX,stdin);
            
           		 write(fd_s2c2,&par_buf,strlen(par_buf));
			printf("client2 writed!\n");

			

		}
	}
	

	
	return 0;

}

