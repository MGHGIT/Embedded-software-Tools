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
	
	sem_t * pSemp2c = sem_open(SEM_P2C, O_CREAT, 0666, 0);
	sem_t * pSemc2p = sem_open(SEM_C2P, O_CREAT, 0666, 0);	
	
	fd_s2c1 = open("./client2fifo",O_RDONLY);
	if(fd_s2c1 < 0)
	{
		printf("error!\n");
		return -1;
	}
	printf("open client2fifo seccess\n");
	
	
		pid=fork();
	
	if(0 == pid )
	{
		while(1)
		{
			//sem_wait(pSemp2c);// sem from parent to child
			printf("child--");
			memset(chi_buf,0,128);
			read_count = read(fd[0],chi_buf,PIPE_MAX);
			
			printf("reading: %s\n",chi_buf);// read from parent
			
			//sem_post(pSemc2p);
		}
	}
	else if(pid > 0)
	{
		while(1)
		{
			memset(par_buf,0,128);
			
			read(fd_s2c1,&par_buf,PIPE_MAX);// read form fifo
			write(fd[1],par_buf,strlen(par_buf));// write to child
			usleep(100);
			//sem_post(pSemp2c);
			//sem_wait(pSemc2p);
		}
	}
	

	
	return 0;

}
