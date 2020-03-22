|       实验题⽬:    多进程编程          | 
| :-----------------------------------: |
|     **⼩组名称：    MGH小组**         |
| **小组成员：龚周闯宇、贺居獒、孟昭泰** |
 
⼩组成员：

#一、实验目的
* 学会创建多进程程序，理解fork()，vfork()创建的子进程的特点。
* 学会使用有名管道和无名管道实现进程间通信。
* 学会使用信号机制实现进程间通信。

#二、实验内容

+ 编写fork()，vfork()多进程程序，通过输出结果比较所创建的子进程的不同之处。

+ 编写fork()多进程程序，进程间模拟信号通信，包括信号发送、捕捉与处理。

+ 编写fork()多进程程序，进程间实现有名管道和无名管道通信。具体要求如下:

    + 编写1个服务器程序server.c和2个客户端程序client1.c,client2.c，服务器和客户端之间采用FIFO管道通信;

    + 服务器和客户端各自创建自己的子进程，父进程负责完成FIFO管道通信，子进程负责输出接收到的信息，父子进程间采用无名管道通信。

#三、实验过程与结果

####3.1编写fork()，vfork()多进程程序，通过输出结果比较所创建的子进程的不同之处
```
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

```
![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/1.png)

仅仅是将fork（）函数改为vfork，则输出结果大有不同。
    
fork函数中，子进程输出300，父进程输出900，他们的数据互不干扰。

vfork函数中，子进程输出300，父进程在加上600后，输出却为1100，说明其数据受子进程影响。

原因是，fork（）函数是在该函数开始，将下面的程序代码和内存、数据结构全部复制一遍成为子进程。
此时父子进程的运行是独立的。但是vfork（）函数是在该函数后创建的子进程是和⽗进程执共享数据段，⽽⾮拷⻉。
两者的数据段不是相对独⽴的。因此，对⽗进程的情况下，只有在⼦进程在调⽤exec或exit退出之后，⽗进程才可能被调度运⾏。

####3.2编写fork()多进程程序，进程间模拟信号通信，包括信号发送、捕捉与处理。
**信号的发送**

 int kill(pid_t pid, int signo); 将信号发送给进程或者进程组


int raise(int signo); 向自身发送信号


unsigned int alarm(unsigned int seconds)；设置一个定时器，到时间后，产生SIGALRM信号

**信号的接收**

int pause(void);使调用进程挂起直至捕捉到一个信号

sleep（seconds）;睡眠指定秒数

while（1）;循环

**信号的处理**
 
 sighandler_t signal(int signum, sighandler_t handler);该函数会根据signum 指定的信号来设置对应的处理函数，当该信号到达时会跳转到handler指定的函数执行。

struct sigaction{ void (*sa_handler) (int); sigset_t sa_mask; int sa_flags; };信号处理函数，SIG_DFL，SIG_IGN//暂时将sa_mask指定的信号屏蔽

```
#include <stdio.h> 
#include <stdlib.h> 
#include <signal.h> 
void myfun(int signum) 
{ 
if(signum==SIGALRM) printf("SIGALRM is captured!\n"); 
if(signum==SIGINT) raise(SIGSTOP);

if(signum==SIGTSTP) raise(SIGSTOP); 
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

	printf("you can type control+z to stop me\n");
	signal(SIGTSTP, myfun); 
	//while(1)printf("timer is %d",i++);
	//exit(EXIT_SUCCESS);
	}
 
	while(1){ 
	printf("The task is processing...\n"); 
	sleep(1);
	} 
}
```

![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/2.png)

####3.3编写fork()多进程程序，进程间实现有名管道和无名管道通信。
Server端：
```
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
```
client端：
```

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
```

![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/3.png)

刚开始使用了信号量函数，所以运行时，需添加-pthread语句

![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/4.png)

在实验中，先在shell或者程序中创建mkfifo文件。

在两个用户端之间，用有名管道进行通信。服务器向有名管道进行写。客户端进行读取。

在父子进程中，用无名管道进行通信。客户端的父进程收到信号后，将信号通过无名管道传给子进程，打印出来。

管道的局限性：

+ 数据自己读不能自己写。

+  数据一旦被读走，便不在管道中存在，不可反复读取。

+  由于管道采用半双工通信方式。因此，数据只能在一个方向上流动。

+  无名管道只能在父子进程间使用。

#四、实验总结 

##总结实验收获 



####4.1 学会了用fork创建多进程程序。

fork（）函数是在该函数开始，将下面的程序代码和内存、数据结构全部复制一遍成为子进程。父子进程的运行是独立的。

vfork（）函数是在该函数后创建的子进程是和⽗进程执共享数据段，⽽⾮拷⻉,两者的数据段不是相对独⽴的。

####4.2 学会了使⽤管道实现进程间通信。 

管道的局限性：

+ 数据自己读不能自己写。

+  数据一旦被读走，便不在管道中存在，不可反复读取。

+  由于管道采用半双工通信方式。因此，数据只能在一个方向上流动。

+ 只能在有公共祖先的进程间使用管道。

常见的通信方式有，单工通信、半双工通信、全双工通信。

####4.3 学会了使⽤信号机制实现进程间通信。 
**信号的发送**

 int kill(pid_t pid, int signo); 


int raise(int signo); 


unsigned int alarm(unsigned int seconds)；

**信号的接收**

int pause(void);

sleep（seconds）;

while（1）;

**信号的处理**
 
 sighandler_t signal(int signum, sighandler_t handler);

struct sigaction{ void (*sa_handler) (int); sigset_t sa_mask; int sa_flags; };

####4.4学会了进程与线程的区别

进程相当于一个任务，线程是程序执行的最小单位，而进程是操作系统分配资源的最小单位；

一个进程由一个或多个线程组成，线程是一个进程中代码的不同执行路线；

进程之间相互独立，但同一进程下的各个线程之间共享程序的内存空间(包括代码段，数据集，堆等)及一些进程级的资源(如打开文件和信号等)，某进程内的线程在其他进程不可见；

调度和切换：线程上下文切换比进程上下文切换要快得多。

####4.5wait和wxit函数对进程的影响。

wait()如果一个进程调用了它，会立即阻塞自己一直到该进程的某个子进程退出才会继续运行，Wait（）可以杀死僵尸进程

Exit（）终结进程，调用了它，进程将处于EXIT_ZOMBIE状态。exit()是结束当前程序并且刷新缓冲区到标准输出，而_exit()是结束档当前程序但是不将缓冲区的内容刷新到标准输出。

####4.6Linux编译错误

若对‘sem_wait’未定义的引用，问题在多线程pthread里。

解决方法：在编译器指令链接选项中加入-pthread即可。


#未解决的问题

服务端和客户端之间的双向通信还需要探索，目前能实现服务端写，客户端读的单向通信。

#五、附实验源码 

####5.1 fork()多进程程序：

```


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include "main.h"

int Myfork(void)
{
pid_t pid;
int bound = 100;
if((pid = fork()) == -1){
perror("fork");
exit(EXIT_FAILURE);
}else if(pid == 0){
printf("This is the child process\n");
bound +=300;
}else{
printf("This is the parent process\n");
bound +=600;
}
printf("pid=%d,ppid=%d\n", getpid(), getppid());
printf("bound=%d\n\n",bound);

}
```
####5.2vfork()多进程程序：

```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include "main.h"

int Myvfork(void)
{
pid_t pid;
int bound = 100;
if((pid = vfork()) == -1){
perror("vfork");
exit(EXIT_FAILURE);
}else if(pid == 0){
printf("This is the child process\n");
bound +=300;
}else{
printf("This is the parent process\n");
bound +=600;
}
printf("pid=%d,ppid=%d\n", getpid(), getppid());
printf("bound=%d\n\n",bound);

}
```
####5.3进程间模拟信号通信程序：

```
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
```

####5.4进程间管道通信程序server.c：
```

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
```
####5.5进程间管道通信程序client1.c：

```
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


```