|       实验题⽬:    构建目标系统          | 
| :-----------------------------------: |
|     **⼩组名称：    MGH小组**         |
| **小组成员：龚周闯宇、贺居獒、孟昭泰** |
 


# 一、实验目的
* 掌握裁剪Linux内核的方法，理解内核选项的意义；
* 熟悉编译内核并加载内核到目标系统的方法与过程；
* 了解模块与内核的关系，掌握内核模块配置编译、安装与卸载流程，为进一步编程，如驱动编程打下基础；
* 掌握创建、安装(卸载)并使用文件系统的方法。


# 二、实验内容

+ 首先用默认配置重新编译一遍已安装到开发板的内核，将新的内核替换现有内核，检查是否通过！
    + 在原始版本基础上，重新配置Linux内核，构建一个嵌入式的Linux内核；
    + 编译安装重新配置后的内核、模块及dtbs设备树；
    + 针对默认配置中不少于10个kernel feature进行重新配置（裁剪为主、偶有增加），并解释理由；(Y=>N，N=>Y)
    + 保留必要的模块安装，剩余(占多数)取消；(M=>N)
    + make后将新的内核安装到开发板运行测试；
    + 选择至少二个模块加载与卸载，检查是否加载、卸载成功；
    + 构建并安装至少一款不同于根文件系统、用于应用开发的其它文件系统。

# 三、实验过程与结果

构建目标系统：

可能需要的准备工作：

本次实验为构建目标系统，会更改树莓派原有系统。如果想要备份原系统文件，可以使用Win32DiskImager来将原树莓派SD卡中的系统读取为备份的.img文件。操作方式如下，选中SD卡，设置备份的文件名，点击“读取”（注意，向SD写入系统时选择“写入”）。
构建目标系统：

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/1.png)

#### part1.内核编译

下面开始内核编译（交叉编译）：

在上次实验中我们已经将编译环境加入系统环境中，具体步骤如下：

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/2.png)
 
下载树莓派linux源码（本次编译在Ubuntu系统中）：

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/3.png)
 
进入文件夹linux,输入命令 

KERNEL=kernel7

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcm2709_defconfig

 
![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/4.png)
 
欲编译生成内核zImage,模块module和设备树dtbs，需输入命令：

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules dtbs

此过程持续时间较长
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/5.png)
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/6.png)
 
 
可在/linux/arch/arm/boot文件夹中看到相关文件
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/7.png)
 
到此，内核编译完成。接下来将编译好的树莓派内核写进SD卡。

将带SD卡的读卡器插入电脑，选择由虚拟机读入设备，输入命令lsblk查看设备：

插入读卡器前：
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/8.png)
 
插入读卡器后：sdb1,sdb2即为SD的分区
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/9.png)
 
将原SD的分区取消挂载

在当前文件夹（linux）下创建子文件夹mnt/fat32, mnt/ext4,分别对应/dev/sdb1,/dev/sdb2并挂载
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/10.png)
 
输入命令，安装模块：

export ARCH=arm

export CROSS_COMPILE=arm-linux-gnueabihf-

export INSTALL_MOD_PATH=mnt/ext4

sudo make

系统显示如下，此过程持续时间较长：
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/11.png)
 


最后，复制内核和设备树SD卡
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/12.png)
 
此处复制的内核可能需要以下操作：
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/13.png)
 
将zImage文件处理为kernel.img文件（在tools/mkimages文件夹中可找到相关文件）：
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/14.png)
 
问题为，编译的内核写入SD卡不能被树莓派识别。（可能与文末提到的“待解决的问题有关”）

 
#### part2.内核剪裁：

在剪切内核前，应该清楚接下来要搭建一个怎样的系统，需要什么功能，从而得出需要哪些模块。

本实验以构建“基于树莓派控制的自平衡小车”系统为目标，对树莓派的内核模块进行删选：

硬件平台：树莓派/PC

软件平台：linux/c


![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/15.png)
 
由于本次实验中sudo make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=mnt/ext4 modules_install 命令出现问题（见文末“待解决的问题”），所以改输入命令

export ARCH=arm

export CROSS_COMPILE=arm-linux-gnueabihf-

export INSTALL_MOD_PATH=mnt/ext4

sudo make

在编译中，系统有如下提示，选择是否安装相关模块：
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/16.png)
 
问题为，编译的内核写入SD卡不能被树莓派识别。（可能与文末提到的“待解决的问题有关”）
 
#### part3.构建NFS文件系统：

NFS文件系统由服务器端和客户端构成，本实验中树莓派作为服务器端（server），Ubuntu作为客户端（client）

服务器端所需的准备：

sudo apt-get insall ntf-kernel-server，下载nfs服务器端
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/17.png)
 
sudo apt-get isntall portmap
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/18.png)
 
建立用于共享的文件夹，此处为/media/nfs
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/19.png)
 
更改配置文件，/etc/exports
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/20.png)
 
添加内容：/media/nfs *(rw,sync,no_root_squash)
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/21.png)
 
最后，开启nfs服务：

sudo /etc/init.d/rpcbing restart

sudo /etc/init.d/nfs-kernel-server restart
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/22.png)
 
客户端所需要的准备：

下载nfs客户端： sudo apt-get install nfs-common
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/23.png)
 
建立用于nfs共享的文件夹：
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/24.png)
 
将该文件夹挂载到服务器端的共享文件夹下，192.168.43.212为树莓派ip（注：此处欲ping通树莓派，需要虚拟机和树莓派在同一网段。由于电脑和树莓派均连接手机热点，需将虚拟机网络方式选择为“桥接”）
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/25.png)
 
到此，nfs系统搭建完成。

下面开始验证nfs系统效果：

在Ubuntu中/mnt/nfs文件夹下创建两个文件
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/26.png)
 
在树莓派对应文件夹下成功收到两个文件，文件内容显示正常。


![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/27.png)
 
#### 遇到的问题：

##### 问题1：

在执行命令make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcm2709_defconfig时报错，为缺少某环境包所致，下载即可
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/28.png)
 
解决办法：
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/29.png)
 
##### 问题2（同上）：
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/30.png)
 
解决办法：
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/31.png)
 

##### 问题3（同上）：
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/32.png)
 
解决办法：
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/33jjff.png)
 
##### 问题4与解决方法：

当虚拟机读取SD卡时，无法挂载SD卡分区。原因为已挂载，取消原挂载即可
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/34.png)
 
##### 问题5：

向SD卡中复制内核与设备数时，报错：read-only file system，显示只读。
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/35.png)
 
解决方法：只读是因为U盘驱动故障，linux读入自动权限设置为只读；解决办法为让pc读取U盘，会提示修复U盘，按照提示修复U盘后再由linux读取即可
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/36.png)
 
##### 问题6：

系统不识别命令 /etc/init.d/pormap restart

解决方法：根据系统提示，将rpcbing替换protmap即可

即：sudo /etc/init.d/rpcbing restart
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/37.png)
 
##### 问题7：

根据网络教程搜索结果，nfs系统客户端开启nfs功能需要输入命令 sudo service nfs-common stat。但显示服务被屏蔽。尝试关闭防火墙也没有效果。

无奈下忽略该命令，发现不影响nfs功能。猜测新版中，nfs客户端无需开启指令。
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/38.png)
 
解决方法，忽略该命令，不影响操作……

未解决的问题：

输入命令

sudo make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=mnt/ext4 modules_install时，报错；网上查询并尝试多种方法未果
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/39.png)
 
但输入命令能识别arm-linux-gnueabihf-gcc：
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/40.png)
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/41.png)
 
 
且，引用的文件夹下的确有该文件：
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/42.png)
 

##### 尝试1：在根目录的/.bashrc文件中添加编译环境
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/43.png)
 
##### 尝试2：下载32位兼容编译包
 

![Image]( https://github.com/gzcy13/GMH/blob/master/0510picture/44.png)
 
最终的尝试：

最后，在命令行依次输入

export ARCH=arm

export CROSS_COMPILE=arm-linux-gnueabihf-

export INSTALL_MOD_PATH=mnt/ext4

sudo make

后，系统开始正常编译。但最终编译后的内核似乎无法被树莓派开发板识别。
