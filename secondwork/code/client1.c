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
	int fd[2],fd_s2c1,fd_s2c2;
	char process_inter = 0;
	int ret,read_count;
	char par_buf[128]={0};
	char chi_buf[128]={0};
	ret = pipe(fd);
	
	fd_s2c1 = open("./client1fifo",O_RDONLY);
	if(fd_s2c1 < 0)
	{
		printf("error!\n");
		return -1;
	}
	printf("open client1fifo seccess\n");
	
	
		pid=fork();
	
    if(0 == pid )//child
	{
		while(1)
		{

			printf("child--");
			memset(chi_buf,0,128);
			read_count = read(fd[0],chi_buf,PIPE_MAX);
			
			printf("reading: %s\n",chi_buf);// read from parent
			

		}
	}
    else if(pid > 0)//parent
	{
		while(1)
		{
			memset(par_buf,0,128);
			
            
			read(fd_s2c1,&par_buf,PIPE_MAX);// read form fifo
			write(fd[1],par_buf,strlen(par_buf));// write to child
			usleep(100);

		}
	}
	

	
	return 0;

}
