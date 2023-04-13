FROM ubuntu:18.04
MAINTAINER author "vizdl@qq.com"
# COPY sources.list /etc/apt/ # 更换源操作,目前无其他合适的国内源,更换后会导致失败。
# COPY linux-2.6.34 /opt/linux-2.6.34/
ENV DEBIAN_FRONTEND=noninteractive
# WORKDIR /workdir
RUN	apt-get update
RUN	apt-get install gcc-4.8 gdb qemu make dos2unix global -y
RUN	apt-get install build-essential kernel-package libncurses5-dev -y
RUN	ln -sf /usr/bin/gcc-4.8 /usr/bin/gcc
