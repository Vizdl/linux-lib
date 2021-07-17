# <center>搭建个人Linux环境</center>
-------
## <center>简述</center>
#### Linux内核代码阅读的困难 :
1. 由于Windows文件不分辨大小写,但Linux区分。导致Linux源代码放到Windows上部分会发生文件名冲突
2. 由于Linux需要支持很多体系结构和兼容各种情况,使用大量宏来编译。在阅读跳转时,会导致跳转选项太多,需要浪费时间来挑选(很多时候阅读代码并不想去考虑所有情况)。
3. Linux项目太大了,简单搜索一个字段可能也需要花费很久的时间。
#### 搭建的环境
宿主机为`Win10`,通过虚拟机运行`Ubuntu`,将Linux源代码放置在`Ubuntu`上并开启SSH,凭借`Win10`的`映射网络驱动器`与`SSHF`将`Ubuntu`文件系统挂载在`Win10`上,在`Win10`利用`Source Inlight`配置宏定义与需要加载进项目的文件再去阅读源代码。
#### win10下载安装虚拟机与镜像
1. 首先要安装虚拟机
2. 安装一个Ubuntu或者其他Linux发行版本
#### win10 下载安装 sshf
1. 下载安装 https://github.com/billziss-gh/winfsp/
2. 下载安装 https://github.com/billziss-gh/sshfs-win/
3. 安装完毕后即可,程序会自动在后台运行。
#### win10 下载安装 `Source Inlight 4.0`
尽力支持正版,百度下载安装过程即可
#### Ubuntu SSH 服务配置
1. 下载 SSH 服务程序
2. 配置 SSH ROOT可登陆(为避免权限问题,用root登陆比较省事)
3. 开启 SSH 服务
#### win10 配置映射网络驱动器
打开此电脑,点击文件那项可看到如下画面 :
![映射网络驱动器1](./images/映射网络驱动器1.png)
点击映射网络驱动器可看到如下画面 :
![映射网络驱动器2](./images/映射网络驱动器2.png)
驱动器可以随便选,文件夹格式如图 :
```
\\sshfs\root@192.168.70.134
```
root表示ssh以root用户形式登陆
`192.168.70.134`是我的内网IP。
建议以`root`权限去运行,这样的话不会出现因为权限无法修改或者文件无法显示问题。
![映射网络驱动器3](./images/映射网络驱动器3.png)
可以看到当前挂载的就是 Ubuntu 上对应用户的根目录。
#### Ubuntu 拉取 Linux 源代码与容器编译环境
```
git clone https://github.com/Vizdl/linux-lib.git
```
#### Ubuntu 生成对应的 `Source Inlight` 配置文件
docker 容器安装
```
curl -fsSL https://get.docker.com | bash -s docker --mirror Aliyun
```
制造编译镜像
```
sudo make build-Docker-Image
```
进入容器内编译内核并生成对应的 `Source Inlight` 配置
```
sudo make run-Docker-Image
make read-config
```
#### 利用 配置文件 配置 linux源代码项目
1. 将生成的配置文件放到windows上
2. 进入 source Inlight 点击 New project
![new-project](./images/new-project.png)
3. 按照下图顺序分别填写 项目名 项目路径(建议默认,不要填到挂载Ubuntu硬盘) 点击 OK
![new-project2](./images/new-project2.png)
4. 蓝色框填源代码的真实路径(挂载Ubuntu目录的win10路径) 橙色框填写生成的配置文件 file list的路径 最后点击黄色框
![new-project3](./images/new-project3.png)
5. 点击 import -> replace all conditions -> load -> 选择生成的配置文件
![new-project4](./images/new-project4.png)
6. 最后同步文件即可
![new-project5](./images/new-project5.png)