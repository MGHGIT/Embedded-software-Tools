|       实验题⽬: 构建主机端开发环境(下) | 
| :-----------------------------------: |
|     **⼩组名称：    MGH小组**         |
| **小组成员：龚周闯宇、贺居獒、孟昭泰** |


# 一、实验目的

* 学会下载安装Linux及相关工具到目标机或目标机仿真环境。

* 学会使用交叉编译器编译应用程序，并能够将可执行程序下载到目标机运行。

* 学会使用交叉调试工具gdb-gdbserver调试应用程序。

# 二、实验内容

+ Host-Target 方案1： 下载安装Linux系统到开发板，其中包括bootloader、内核、模块、库、文件系统等； 建立host-target连接 ，常见连接有SSH，VNC， Putty。

+ Host-Target 方案2：下载安装目标机仿真环境qemu， 安装qemu树莓派系统。

+ 构建并测试开发板交叉编译器。

+ 建立gdb-gdbserver交叉调试环境并能够调试应用程序。

# 三、实验过程与结果
# （因为过程重点图片太多，所以我们组将word里的图片合在一起存为截图上传了。）
#### 总结

|                                       | 
| :-----------------------------------: |
|   平台------------------------------ 实验内容-------------------------结果     | 
|     **Qemu----------------------------安装树莓派镜像------------------√**          |
|         **Qemu--------------通过ssh等方式连接树莓派------------------×** |
| **Qemu-----------建立gdb-gdbserver环境并调试--------------×** |
| **开发版（3b+）------------------安装树莓派镜像------------------	√** |
| **开发版（3b+）-------------通过ssh等方式连接树莓派----------	√** |
| **开发版（3b+）-----建立gdb-gdbserver环境并调试---------	√** |

####  Part1.在linux虚拟机中通过qemu安装树莓派镜像

![Image](https://github.com/gzcy13/GMH/blob/master/12345678/1.png)

![Image](https://github.com/gzcy13/GMH/blob/master/12345678/2.png)

![Image](https://github.com/gzcy13/GMH/blob/master/12345678/3.png)

####   Part2.树莓派3b+开发板安装镜像
![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/4.png)

![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/5.png)

####   Part3.linux安装交叉编译环境

![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/6.png)

![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/7.png)

![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/8.png)


####   Part4.远程gdb-gdbsever调试（基于树莓派开发板）

![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/9.png)

![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/10.png)

![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/11.png)

![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/12.png)


####   未解决的问题：
Qemu下的树莓派虚拟机无法通过ip进行ssh连接

![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/13.png)

![Image](https://raw.githubusercontent.com/gzcy13/GMH/master/14.png)
