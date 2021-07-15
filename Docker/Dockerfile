FROM ubuntu:18.04
MAINTAINER author "vizdl@qq.com"
# COPY sources.list /etc/apt/ # 更换源操作,目前无其他合适的国内源,更换后会导致失败。
# COPY linux-2.6.34 /opt/linux-2.6.34/
ENV DEBIAN_FRONTEND=noninteractive
RUN	apt-get update \
	&& apt-get install gcc-4.8 gdb qemu make vim ctags -y \
	&& apt-get install build-essential kernel-package libncurses5-dev -y \
	# 晚一些改,避免前面需要安装时会覆盖。
	&& ln -sf /usr/bin/gcc-4.8 /usr/bin/gcc \
	&& ln -sf /usr/bin/qemu-system-x86_64 /usr/bin/qemu
