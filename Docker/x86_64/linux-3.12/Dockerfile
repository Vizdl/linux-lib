FROM swr.cn-north-4.myhuaweicloud.com/ddn-k8s/docker.io/ubuntu:18.04
MAINTAINER author "dinglei@kernelsoft.com"
# 更换源操作,目前无其他合适的国内源,更换后会导致失败。
# COPY sources.list /etc/apt/
ENV DEBIAN_FRONTEND=noninteractive
WORKDIR /workdir
RUN	apt-get update
RUN	apt-get install gcc-4.8 -y
RUN	apt-get install gdb-multiarch -y
RUN apt-get install qemu -y
RUN apt-get install make -y
RUN apt-get install dos2unix -y
RUN apt-get install global -y
RUN	apt-get install build-essential -y
RUN	apt-get install kernel-package -y
RUN	apt-get install libncurses5-dev -y
RUN	apt-get install bc -y
RUN	ln -sf /usr/bin/gcc-4.8 /usr/bin/gcc
RUN	apt-get install wget -
RUN	apt-get install expect -y
RUN	apt-get install e2fsprogs -y
RUN	apt-get install kpartx -y
RUN	apt-get install gdisk -y