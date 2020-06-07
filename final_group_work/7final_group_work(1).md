|       实验题⽬:    综合应用实验          | 
| :-----------------------------------: |
|     **⼩组名称：    MGH小组**         |
| **小组成员：龚周闯宇、贺居獒、孟昭泰** |
 


# 一、实验目的

能够综合应用课程所学的技术与工具，包括：
* socket通信
* 多进程、多线程编程
* 交叉调试目标端程序
* 磁盘分区与文件系统创建
* 模块与驱动编程
# 二、实验内容

+ 1.将树莓派设为智能家居Linux服务器，可用来采集并维护环境数据，如PM2.5、温度、湿度、气味、电器状态数据等。在实际环境中数据来自相应的传感器，本次试验中用scull设备模拟。有条件的小组鼓励使用真实设备采集数据。
+ 2.要求创建2个以上的scull设备，设备驱动可选择从内核源码树外(Kbuild)编译安装，或加入到内核源码树内。 驱动函数要求包括： open, release, read, write, llseek, ioctl。
+ 3.实验中的环境数据存储在特定文件系统中。该文件系统要求具备属性： 在线写入、 持久性、 断电可靠性。
+ 4.PC机、移动设备或另外一个树莓派用作远程客户端，随时请求获取环境数据，客户端和服务器之间采用Socket通信。
+ 5.APP编译采用交叉编译，用gdb-gdbserver交叉调试APP。

# 三、实验过程与结果

本项目整体实现流程图如下：

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/23.png)
 
#### 1、文件系统

文件系统要求具备在线写入、持久性、断电可靠性的属性，选用nfs 作为存储环境数据的文件系统。
功能：树莓派向nfs文件夹中实时写入txt文件，在pc端可以查看
（1）开启树莓派与pc的nfs功能：

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/1.png)
 
（2）挂载到远程NFS服务器

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/2.png)
 
（3）树莓派向文件中写入日期文件，这里写入两次

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/3.png)
 
（4）在树莓派端进行查看

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/4.png)
 
（5）在PC端进行查看，并查看成功

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/5.png)
 
#### 2、socket通信

这里以树莓派作为服务器，PC机作为远程客户端，PC机可以随时向服务器请求获取环境数据，客户端与服务器之间采用Socket通信。

（1）开启通信前， 需关闭虚拟机防火墙：

sudo ufw disable

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/6.png)
 

（2）树莓派和虚拟机可正常连接：

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/7.png)
 
#### 3、scull设备：

我们创建scull虚拟设备，模拟温度传感器和空调开关。

（1）在安装好对应的linux-sourse和配置好内核的前提下，我们创建Makefile并和模块程序放在同一目录中，然后输入make指令来：

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/8.png)
 
（2）通过insmod scull。Ko和cat指令来将设备安装到内核源码树外：
 
 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/9.png)

（3）在使用scull之前需要通过mknod scull_ts c 240 0来创建字符设备，需要注意主次设备号要和驱动中的一致。

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/10.png)

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/11.png)
 

（4）如果make出现问题，可能是/lib/modules/`uname -r`/build下没有build文件。

我们需要通过sudo apt install raspberrypi-kernel-headers指令进行安装。此时需要注意headers版本与内核版本（可以通过uname -r 指令查看）的一致。

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/12.png)

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/13.png)

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/14.png)
 
 #### scull驱动代码：

Open（）函数打开两个温度传感器和空调控制器

    
   ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/33.png)

当温度低时，关空调；write（）函数写入空调状态 ，read（）函数读出空调状态。

    
   ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/34.png)

当温度高时，开空调；lseek()函数指定文件的读写位置。

    
   ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/35.png)


使用完毕之后close（）函数关闭温度传感器和空调控制器
 
   
   ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/36.png)



#### 4、交叉编译

我们在虚拟机的linux系统上进行交叉编译

（1）交叉编译指令：arm-linux-gnueabihf-gcc final_pid.c –o pid_final –pthread -lm

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/15.png)

（2）通过FileZilla软件将文件传送给树莓派

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/16.png)

（3）树莓派运行可执行文件：

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/17.png)

（4）远程调试：

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/18.png)

（5）将pid_final_g传送给树莓派

树莓派设置gdb服务器节点（此处为1）

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/19.png)

（6）虚拟机进行gdb连接

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/20.png)

（7）树莓派响应虚拟机连接：

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/21.png)



（8）开始调试运行。

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/22.png)

#### 5、实验结果：（综合各个功能成为系统）

系统功能示意图：

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/23.png)

树莓派的准备：

（1）打开nfs功能（见1.1） 

（2）执行可执行文件

 ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/24.png)



PC的准备：

（1）打开并连接nfs（见1.1） 

（2）打开socket接收端 

  ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/25.png)

（3）查看nfs内文件

  ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/26.png)

系统效果展示：

  ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/27.png)

左侧是PC通过socket收到的信息，中间是树莓派实时写入nfs中的温度信息（实时变化），右侧为树莓派提示信息（提醒socket发送，与nfs文件写入）。



  #### 关键代码：

* 创建Socket的结构体：

   ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/28.png)

* 发送socket的线程：

   ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/29.png)

其中通过write（）和read（）函数写入读出空调状态，

通过send（）函数将空调状态socket发送到PC端。


* 向nfs中写入数据的线程：

   ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/30.png)

通过read（）和write（）函数写入读取温度。

再通过fopen（）和fprintf（）还有fclose（）函数进行文件的打开、写入和关闭。


* 主函数：

   ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/31.png)
   
   ![Image]( https://github.com/gzcy13/GMH/blob/master/fw/32.png)

通过socket（）函数和connect（）函数来连接服务器和客户端。

recv（）函数接收服务器端信息

通过信号量sem进行线程间通信。





# 四、实验总结

#### 1、总结实验收获



（1）更加深入地了解了linux，增加了嵌入式的知识积累。

（2）学会了socket通信、多进程、多线程编程、交叉调试目标端程序、磁盘分区与文件系统创建和模块与驱动编程等知识技能。

（3）提高了在线上小组合作完成课程任务的能力。



#### 2、下一步尝试计划

1、通过编译内核加入scull设备，增加scull设备数目

2、将试图将各模块功能封装成分成子执行文件，可以只运行其中部分功能

# 五、附实验源码

#### 1、分模块测试用代码及最终程序final_pid.c请见地址：https://github.com/gzcy13/GMH/tree/master/final_work_code
#### 2、单独贴上最终程序 final_pid.c
```buildoutcfg

// 建立两个scull设备 scull_ac(空调) scull_ts（温度传感器）
// 空调的开关状态通过socket发送到PC， 树莓派为socket服务器
// 室内当时时间与温度写入ts.txt,PC可通过nfs查看该文件，树莓派为nfs服务器
// 室内温度高于30时，空调开启；室内温度低于16时，空调关闭
// 发送socket 和 写nfs文件 封装进两个线程中
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>    /* BSD and Linux */
#include <stropts.h>    /* XSI STREAMS */
#include <pthread.h>
#include <semaphore.h>
#define AIRCONDITION_ON 1
#define AIRCONDITION_OFF 0
#define ON_TEMP 30
#define OFF_TEMP 16
#define SEM_send "send_socket_pc"
#define SEM_write "write_to_nfs"
#define SEM_finish "finish_to_send_and_wirte"
void *send_socket(void *args);    //发送socket信息
void *nfs_data_write(void *args); //向nfs中写入数据

typedef struct socket_p_t{
	int fd_ts;  			// 温度传感器 指针
	int fd_ac;  			// 空调开关  指针
	int ac_state;			// 空调开关状态
	int retval;
	int sockfd;				// socket 指针
	char W_buffer_ts[2];
	char R_buffer_ts[2];
	char W_buffer_ac[2];
	char R_buffer_ac[2];
	char buf[BUFSIZ];
	
	pthread_mutex_t my_mutex;  //互斥锁
}socket_p;

int cur_temp = 25;
int main(int argc,char *argv[])
{
	sem_t * pSemsend = sem_open(SEM_send, O_CREAT, 0666, 0); 
	sem_t * pSemwrite = sem_open(SEM_write, O_CREAT, 0666, 0);
	sem_t * pSemfinish = sem_open(SEM_finish, O_CREAT, 0666, 0);
	pthread_t pt[2];
	socket_p *mysocket = (socket_p *)malloc(sizeof(socket_p));
	mysocket->ac_state = AIRCONDITION_OFF;
	pthread_create(&(pt[0]), NULL,(void *)send_socket, (void*)mysocket);
	pthread_create(&(pt[1]), NULL,(void *)nfs_data_write, (void*)mysocket);
	pthread_mutex_init(&mysocket->my_mutex, NULL); // 互斥锁，使两个线程不同时发生（交替进行）
	
	
    int numbytes;
    
    struct sockaddr_in their_addr;
    printf("break!");
    while((mysocket->sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1);
    printf("We get the sockfd~\n");
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(7777);
    their_addr.sin_addr.s_addr=inet_addr("192.168.43.139");
    bzero(&(their_addr.sin_zero), 8);
    
    while(connect(mysocket->sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr)) == -1);
    printf("Get the Server~Cheers!\n");
    numbytes = recv(mysocket->sockfd, mysocket->buf, BUFSIZ,0);//接收服务器端信息  
    mysocket->buf[numbytes]='\0';  
    printf("%s\n",mysocket->buf);
	
	
	int time = 0;
	// 打开两个温度传感器和空调控制器
	mysocket->fd_ts=open("/dev/scull_ts", O_RDWR);
	if(0 == mysocket->fd_ts)
	{
		printf("open temsensor failed!\n");
		
	}
	else printf("succed to open temsensor!\n");
	
	mysocket->fd_ac=open("/dev/scull_ac", O_RDWR);
	if(0 == mysocket->fd_ac)
	{
		printf("open aircondition failed!\n");
		
	}
	else printf("succed to open aircondition!\n");	
	
	
    while(time < 20)
    {

		sem_post(pSemsend);
		sem_post(pSemwrite);
		
		// 室温默认缓慢上升
		cur_temp = cur_temp+2;
		time++;
		sleep(2);
    }
	
	
	close(mysocket->fd_ts);
	close(mysocket->fd_ac);	
    close(mysocket->sockfd);
    return 0;
}
// 发送socket信息
void *send_socket(void *args)
{
	socket_p *mysocket = (socket_p *)args ;
	
	sem_t * pSemsend = sem_open(SEM_send, O_CREAT, 0666, 0); 	

	while(1)
	{
		sem_wait(pSemsend);	
		//pthread_mutex_lock(&(mysocket->my_mutex));// 上锁
		
		// socket 
		// 温度低，关空调；温度高，开空调
		if(cur_temp <= OFF_TEMP)
		{
			// 写入空调状态
			mysocket->W_buffer_ac[0] = AIRCONDITION_OFF
			write(mysocket->fd_ac, mysocket->W_buffer_ac, 1);
			mysocket->retval = lseek(mysocket->fd_ac, 0, 0);
			// 读出空调状态
			mysocket->ac_state = read(mysocket->fd_ac, mysocket->R_buffer_ac, 1);
			mysocket->ac_state = mysocket->R_buffer_ac[0];
			
		}	
		else if(cur_temp >= ON_TEMP)
		{
			// 写入空调状态
			mysocket->W_buffer_ac[0] = AIRCONDITION_ON
			write(mysocket->fd_ac, mysocket->W_buffer_ac, 1);
			mysocket->retval = lseek(mysocket->fd_ac, 0, 0);
			// 读出空调状态
			mysocket->ac_state = read(mysocket->fd_ac, mysocket->R_buffer_ac, 1);
			mysocket->ac_state = mysocket->R_buffer_ac[0];
			
			// 空调开启，温度大幅下降
			cur_temp = cur_temp - 5;
		}
		else
		{
			if(AIRCONDITION_ON == mysocket->ac_state)
				cur_temp = cur_temp - 5;
				
		}
		printf("ac_state is %d!\n", mysocket->ac_state);	
		// 将空调状况通过socket发送到PC
		// ……
		printf("send to socket!\n");
		mysocket->buf[0]=mysocket->ac_state + 48;
		mysocket->buf[1]='\n';
		mysocket->buf[2]='\0';
		send(mysocket->sockfd, mysocket->buf, strlen(mysocket->buf), 0);
		// socket end
		

		//pthread_mutex_unlock(&(mysocket->my_mutex));// 解锁
		usleep(50);
	}
}

// 向nfs中写入数据
void *nfs_data_write(void *args)
{
	socket_p *mysocket = (socket_p *)args ;
	
	
	struct timeval tv;
    char mytime[20] = "";
	FILE *fp = NULL;
	sem_t * pSemwrite = sem_open(SEM_write, O_CREAT, 0666, 0); 
	sem_t * pSemfinish = sem_open(SEM_finish, O_CREAT, 0666, 0);	

	while(1)
	{
		sem_wait(pSemwrite);	
		//pthread_mutex_lock(&(mysocket->my_mutex));// 上锁
	
		// nfs
		// 写入温度
		mysocket->W_buffer_ts[0] = cur_temp;
		write(mysocket->fd_ts, mysocket->W_buffer_ts, 1);
		mysocket->retval = lseek(mysocket->fd_ts, 0, 0);
		// 读出温度
		mysocket->retval = read(mysocket->fd_ts, mysocket->R_buffer_ts, 1);
		printf("the current temperature is  %d\n",mysocket->R_buffer_ts[0]);		
		
		gettimeofday(&tv,NULL);
		strftime(mytime,sizeof(mytime),"%Y-%m-%d %T",localtime(&tv.tv_sec));		
		// a+ 在末尾续写
		fp = fopen("/media/nfs/test.txt", "a+");		
		fprintf(fp, "%s: temp is %d\n",mytime,mysocket->retval);	
		printf("write to nfs\n");
		fclose(fp);
		// nfs end
		
		//pthread_mutex_unlock(&(mysocket->my_mutex));// 解锁
		
		//sem_post(pSemfinish); //启动主函数的循环
		usleep(50);
	}
}


```

