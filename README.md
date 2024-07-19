# linux-lib
## linux-lib 做了什么?
    1) linux-lib 通过 docker 搭建了可依赖 qemu + gdb 调试 linux-2.6.34 源代码的环境。
    2) linux-lib 致力于对低版本的 linux 源代码进行注释与脚本测试等一系列加深对 linux 低版本源代码的理解工作。
## 什么人适合学习与使用 linux-lib ?
    1) 具有一定的 linux 内核原理理论与较为深入的使用基础的人。
    2) 对 docker 简单掌握的人。
    3) 具有一定编程基础的人。
## linux-lib 的前置学习
    1) docker 的学习与安装
        请参照 : https://docs.docker.com/
    2) linux-2.6 理论知识学习
        建议阅读书籍 : Linux内核设计与实现
    3) linux 的使用
        略

## 准备开始
### 1. 容器快速安装
linux-lib 是依赖容器来编译内核的,所以需要先安装 docker
```
curl -fsSL https://get.docker.com | bash -s docker --mirror Aliyun
```
### 2. 选定需要编译的内核版本以及体系结构
例如需要编译内核版本 linux-4.9.299 运行在 aarch64 上, 则需要使用脚本 aarch64-4.9.sh
执行
```c
sudo bash aarch64-4.9.sh help
```
查看支持指令
### 3. 如何构建 linux-lib docker 镜像?
```bash
sudo bash aarch64-4.9.sh build-image
```
如若构建失败一般是网络资源问题,请重试。
### 3. 清除编译中间产物
每次编译失败或者切换体系结构编译时都需要清除上一次编译中间产物
```bash
sudo bash aarch64-4.9.sh cleanall
```
### 3. 编译 linux
#### 3.1 编译生成 linux 默认配置
```bash
sudo bash aarch64-4.9.sh defconfig
```
编译 linux 默认配置时,该项目通过 `scripts/config` 修改了默认配置, 将 RAM dist size 设置为 65536。
#### 3.3 编译生成 linux 镜像
```bash
sudo bash aarch64-4.9.sh image
```
### 4. 编译 busybox 作为 rootfs
#### 4.1 编译生成 busybox 默认配置
```bash
sudo bash aarch64-4.9.sh fs-defconfig
```
该项目生成的 busybox 默认配置就将 busybox 二进制文件设置为静态编译。
#### 4.3 编译生成 rootfs 文件
```bash
sudo bash aarch64-4.9.sh rootfs
```
### 5. 生成 usb 磁盘
```bash
sudo bash aarch64-4.9.sh usbdisk
```
### 6. 运行
```bash
sudo bash aarch64-4.9.sh run
```
### 7. 正常退出
```bash
poweroff
```
### 8. 错误退出
在卡死时,另起一个终端 :
```bash
ps axu | grep "/bin/sh -c qemu-system-" | sed -n "1, 1p" | awk '{print $2}' | xargs -I {} sudo kill -9 {}
```
或者
```bash
sudo docker stop buildlinux
```
## 推荐书籍
推荐书籍清单PDF请点击[链接](https://gitee.com/Vizdl/os-books.git)。支持正版,PDF仅供学习使用,有能力的朋友买实体书多多支持一下。
