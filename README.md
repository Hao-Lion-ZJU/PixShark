# PixShark V1.0 开发固件
___
## 1.前言
本项目采用cmake + make + arm-none-eabi-gcc 交叉编译工具链开发，下载程序到单片机使用openocd
`请事先下载好对应环境`</br>
**Linux环境安装**</br>
~~sudo apt-get install arm-none-eabi-gcc~~</br>
~~sudo apt-get install openocd~~

`apt下载的openocd可能版本会比较旧，无法下载H7系列，建议进入openocd官网下载最新版本源码编译`</br>

接着安装stlink驱动:
```bash
#安装依赖库
sudo apt-get install git make cmake libusb-1.0-0-dev
sudo apt-get install gcc build-essential
#下载编译
git clone https://github.com/stlink-org/stlink
cd stlink
cmake
make
#添加环境
cd bin
sudo cp st-* /usr/local/bin
cd ../lib
sudo cp *.so* /lib32
sudo cp stlink/config/udev/rules.d/49-stlinkv* /etc/udev/rules.d/
```

输入st-info --probe查看时候识别到stlink



**Win环境安装**</br>
交叉编译器：进入ARM官网下载[下载链接](https://developer.arm.com/downloads/-/gnu-rm)</br>
openocd：[下载链接](https://github.com/xpack-dev-tools/openocd-xpack/releases/)</br>
整个windows环境下工具链开发可以参考文章：[参考文章](https://zhuanlan.zhihu.com/p/576972892)</br>

win11自带stlink驱动，win10的同学自行安装

`项目使用make，所以需要安装mingw，无需安装Ninja作为构建工具`


## 2.食用
**win环境下需要安装Git**
```bash
git clone git@github.com:Hao-Lion-ZJU/PixShark.git
mkdir build -p && cd build
#windows环境下可能需要指定构建工具，否则win下cmake会自动帮你选择nmake
#然而这不是我们期望的，所以需要 -G "Unix Makefiles"指定，Linux环境下不需要
cmake -G "Unix Makefiles" ..
#根据你CPU的最大核心数决定
make -j8
#连接仿真器下载,项目使用STLINK，如果更换，请修改openocd.cfg中的配置
openocd -f ./openocd.cfg -c download
```
## 3.简介
### 设备 PixShark系列开发板
|硬件|型号|参数|
|---|---|---|
|主控芯片|STM32H743VGT6|480MHZ,1MB Flash,1MB RAM|
### 软件环境

 - Toolchain/IDE : cmake + make + arm-none-eabi-gcc / VSCode
 - STM32CubeMx:6.8.0
 - package version: STM32Cube FW_H7 V1.11.0
 - FreeRTOS version: 10.3.1
 - LWIP version: 2.1.2
 - CMSIS-RTOS version: 1.02
### 编程规范
- APP层和设备驱动代码使用C++11进行重构，引入很多高级语言特性，同时底层部分用C混编，不影响代码性能
- 对硬件依赖完全解耦了，可以方便以后移植到其他平台的MCU，去除了冗余代码代码也结构化逻辑更加清晰
- 变量和函数命名方式遵循 Unix/Linux 风格
- 不需要精确计时的任务，采用自行实现的软件定时器实现，定时精度受任务调度影响
### 与干端通信
- PixSharkV1.0采用MQTT通讯协议，c++封装了LWIP自带的MQTT接口，使用更加方便
- 具体各个设备互传数据采用JSON格式，详见doc文档

## 4.整体框架
### 文件树  
```
PixShark/
├── bsp					(板级支持包)
├── components
│   ├──algorithm （存放常用数学算法）
│   ├── controller（存放控制器实现）
│   └── devices（存放水下机器人上设备的驱动文件）
│   ├── support（存放一些插件）
├── Core
    ├── Inc		（CUBEMX生成外设头文件）
    ├── Src		（CUBEMX生成外设源文件）
    └── Startup	(启动文件)
├── doc	(详细说明文件和记录)
├── Drivers	(CUBEMX生成HAL库驱动)
├── LWIP	(CUBEMX生成LWIP用户接口)
├── Middlewares（CUBEMX生成LWIP、FREERTOS等三方库）
├── Robot
│   ├── *_task.cpp/hpp（各个任务的处理函数文件）
├── Drivers	(CUBEMX生成HAL库驱动)
├── USB_DEVICE	(CUBEMX生成USB用户接口)
├── UserAPP	    (用户二次开发水下机器人接口)
├── openocd.cfg	(openocd配置文件，指定仿真器类型，端口等等)
├── pixshark.ioc（CUBEMX工程）
├── STM32H743VGTX_FLASH.ld（链接脚本）
└── STM32H743x.svd（CPU对应的外设地址映射表，供DEBUG查看寄存器状态时使用）
#其余文件与项目无关

```
