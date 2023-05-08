# 编译的线程数
THREADS ?= 32
# 编译时内存限制
MEM ?= 4G

# 真实 arch
RARCH ?= $(shell uname -m)
# 对应 linux 路径名
LARCH ?= x86
# 待编译的 linux 源码
KERNEL ?= linux-2.6.34
# 待编译的 busybox 源码
BUSYBOX ?= busybox-1.15.3
# 运行时选择的 console
CONSOLE ?= ttyS0
# 镜像名
IMAGE ?= bzImage


ifeq ("$(RARCH)", "x86_64")
	LARCH = x86
else ifeq ("$(RARCH)", "aarch64")
	KERNEL = linux-4.9.229
	BUSYBOX = busybox-1.30.0
	CONSOLE = ttyAMA0
	LARCH = arm64
	IMAGE = Image
else
	$(error "unkown arch!!!");
endif

$(info THREADS[${THREADS}] MEM[${MEM}]);
$(info IMAGE[${IMAGE}] CONSOLE[${CONSOLE}]);
$(info RARCH[${RARCH}] LARCH[${LARCH}] KERNEL[${KERNEL}] BUSYBOX[${BUSYBOX}]);
.PHONY += build-image run-image clean-image
.PHONY += defconfig image devel run restartgdb rungdb stopgdb
.PHONY += defconfig-in-docker distclean-in-docker image-in-docker gdb-in-docker

# 在镜像环境内的操作
defconfig-in-docker :
	cd src/${KERNEL} && make defconfig

menuconfig-in-docker :
	cd src/${KERNEL} && make menuconfig

distclean-in-docker :
	cd src/${KERNEL} && make distclean

image-in-docker :
	cd src/${KERNEL} && make ${IMAGE} -j$(THREADS)

gdb-in-docker :
	qemu-system-x86_64 -kernel src/${KERNEL}/arch/${LARCH}/boot/bzImage -s -S -append "console=ttyS0" -nographic

fs-defconfig-in-docker :
	cd src/${BUSYBOX} && make defconfig

fs-menuconfig-in-docker :
	cd src/${BUSYBOX} && make menuconfig

rootfs-in-docker :
	cd src/${BUSYBOX} && make -j$(THREADS) && make install && bash rootfs.sh

fs-distclean-in-docker :
	cd src/${BUSYBOX} && make distclean

run-in-docker :
	qemu-system-${RARCH}  \
		-nographic \
		-smp 4 -m 2G \
		-kernel ./src/${KERNEL}/arch/${LARCH}/boot/${IMAGE} \
		-initrd src/${BUSYBOX}/rootfs.img.gz \
		-append "root=/dev/ram console=${CONSOLE} init=/linuxrc"

# 在镜像外的操作
build-image :
	sudo docker build -t linux-lib-${RARCH}:latest Docker/${RARCH} --build-arg BUILDKIT_INLINE_CACHE=1 

run-image :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	-it linux-lib-${RARCH}:latest \
	/bin/bash

clean-image :
	sudo docker rmi linux-lib-${RARCH}:latest

run :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib-${RARCH}:latest \
	make RARCH=${RARCH} run-in-docker; \
	sudo docker rm buildlinux

menuconfig :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib-${RARCH}:latest \
	make RARCH=${RARCH} menuconfig-in-docker; \
	sudo docker rm buildlinux

defconfig :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib-${RARCH}:latest \
	make RARCH=${RARCH} defconfig-in-docker; \
	sudo docker rm buildlinux

fs-defconfig :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib-${RARCH}:latest \
	make RARCH=${RARCH} fs-defconfig-in-docker; \
	sudo docker rm buildlinux

fs-menuconfig :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib-${RARCH}:latest \
	make RARCH=${RARCH} fs-menuconfig-in-docker; \
	sudo docker rm buildlinux

# 这里必须要加 --privileged, 否则挂载文件时会提示无权限。
rootfs : 
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--privileged \
	--name buildlinux \
	-it linux-lib-${RARCH}:latest \
	make RARCH=${RARCH} rootfs-in-docker; \
	sudo docker rm buildlinux

fs-distclean :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib-${RARCH}:latest \
	make RARCH=${RARCH} fs-distclean-in-docker; \
	sudo docker rm buildlinux

image :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	--memory-reservation ${MEM} \
	-it linux-lib-${RARCH}:latest \
	make RARCH=${RARCH} image-in-docker; \
	sudo docker rm buildlinux;
	

distclean :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib-${RARCH}:latest \
	make RARCH=${RARCH} distclean-in-docker; \
	sudo docker rm buildlinux

devel : build-image

rungdb :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	-p 1234:1234 \
	--name gdb \
	-itd linux-lib-${RARCH}:latest \
	make gdb-in-docker

stopgdb :
	sudo docker stop gdb; \
	sudo docker rm gdb

restartgdb : stopgdb rungdb

debug :
	gdb src/${KERNEL}/vmlinux -q -ex "target remote localhost:1234"