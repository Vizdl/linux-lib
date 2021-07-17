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
        
## 容器快速安装
```
curl -fsSL https://get.docker.com | bash -s docker --mirror Aliyun
```

## 如何构建 linux-lib docker 镜像?
    sudo make build-Docker-Image

## 如何运行镜像
    sudo make run-Docker-Imag
## 推荐书籍
推荐书籍清单PDF请点击[链接](https://gitee.com/Vizdl/os-books.git)。支持正版,PDF仅供学习使用,有能力的朋友买实体书多多支持一下。