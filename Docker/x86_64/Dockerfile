FROM ubuntu:18.04
MAINTAINER author "vizdl@qq.com"
# 更换源操作,目前无其他合适的国内源,更换后会导致失败。
# COPY sources.list /etc/apt/
# COPY linux-2.6.34 /opt/linux-2.6.34/
ENV DEBIAN_FRONTEND=noninteractive
WORKDIR /workdir
RUN	apt update
RUN	apt install gcc-4.8 -y
RUN apt install gdb -y
RUN apt install qemu -y
RUN apt install make -y
RUN apt install dos2unix -y
RUN apt install global -y
RUN	apt install build-essential -y
RUN	apt install kernel-package -y
RUN	apt install libncurses5-dev -y
RUN	ln -sf /usr/bin/gcc-4.8 /usr/bin/gcc
