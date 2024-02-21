# 主机 arch
HOST_ARCH = $(shell uname -m)
# 编译的线程数
THREADS ?= 128
# 编译时内存限制
MEM ?= 64G
# 默认目标体系结构就是主机体系结构
# 如若想交叉编译,则需要指定
TARGET_ARCH ?= ${HOST_ARCH}
# 待编译的 linux 源码
LINUX_VERSION ?= linux-4.9.229
# docker 镜像名
DOCKER_IMAGE = linux-lib-${HOST_ARCH}-${LINUX_VERSION}:latest


# 对应 linux 体系结构名
LINUX_ARCH ?=
CONSOLE ?=
IMAGE ?=
MACHINE ?=
# 根据目标体系结构判断 linux 的对应目录名
ifeq ("$(TARGET_ARCH)", "x86_64")
	LINUX_ARCH = x86_64
	CONSOLE = ttyS0
	IMAGE = bzImage
else ifeq ("$(TARGET_ARCH)", "aarch64")
	LINUX_ARCH = arm64
	CONSOLE = ttyAMA0
	IMAGE = Image
	MACHINE = -machine virt,gic-version=2 -cpu cortex-a53
	ifeq ("$(LINUX_VERSION)", "linux-2.6.34")
		$(error "linux-2.6.34 does not support aarch64!!!");
	endif
else
	$(error "unkown arch!!!");
endif


# 根据本地体系结构与目标体系结构判断是否需要交叉编译
CROSS_COMPILER_PERFIX ?=
ifneq ("$(HOST_ARCH)", "$(TARGET_ARCH)")
	CROSS_COMPILER_PERFIX = /opt/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
endif

# 待编译的 busybox 源码
BUSYBOX ?=
# 根据待编译linux版本做选择 busybox 版本
ifeq ("$(LINUX_VERSION)", "linux-2.6.34")
	BUSYBOX = busybox-1.15.3
else ifeq ("$(LINUX_VERSION)", "linux-4.9.229")
	BUSYBOX = busybox-1.30.0
else
	$(error "does not support $(LINUX_VERSION) !!!");
endif

# debug 提示
# $(info IMAGE[${IMAGE}] CONSOLE[${CONSOLE}]);
$(info THREADS[${THREADS}] MEM[${MEM}]);
$(info HOST_ARCH[${HOST_ARCH}] LINUX_ARCH[${LINUX_ARCH}] LINUX_VERSION[${LINUX_VERSION}] BUSYBOX[${BUSYBOX}]);

.PHONY += build-image run-image clean-image
.PHONY += defconfig menuconfig fs-defconfig fs-menuconfig
.PHONY += image devel run dump restartgdb rungdb stopgdb all
.PHONY += image-in-docker rootfs-in-docker 
.PHONY += defconfig-in-docker menuconfig-in-docker
.PHONY += fs-defconfig-in-docker fs-menuconfig-in-docker
.PHONY += clean-in-docker distclean-in-Docker
.PHONY += gdb-in-docker dump-in-docker

defconfig-after-in-docker-x86_64 :
	cd src/${LINUX_VERSION} && \
	scripts/config --enable BLK_DEV_RAM && \
	scripts/config --set-val BLK_DEV_RAM_COUNT 16 && \
	scripts/config --set-val BLK_DEV_RAM_SIZE 65536

defconfig-after-in-docker-aarch64 :
	cd src/${LINUX_VERSION} && \
	scripts/config --enable BLK_DEV_RAM && \
	scripts/config --set-val BLK_DEV_RAM_COUNT 16 && \
	scripts/config --set-val BLK_DEV_RAM_SIZE 65536 && \
	scripts/config --disable ARM64_UAO

fs-defconfig-after-in-docker-x86_64 :
	cd src/${BUSYBOX} && \
	scripts/config --enable STATIC && \
	scripts/config --set-str CROSS_COMPILER_PREFIX "${CROSS_COMPILER_PERFIX}"

fs-defconfig-after-in-docker-aarch64 :
	cd src/${BUSYBOX} && scripts/config --enable STATIC

# 在镜像环境内的操作
defconfig-in-docker :
	cd src/${LINUX_VERSION} && make ARCH=${LINUX_ARCH} defconfig && \
	cd - && make defconfig-after-in-docker-${HOST_ARCH}

menuconfig-in-docker :
	cd src/${LINUX_VERSION} && make menuconfig

distclean-in-docker :
	cd src/${LINUX_VERSION} && make distclean

clean-in-docker :
	cd src/${LINUX_VERSION} && make clean

image-in-docker :
	cd src/${LINUX_VERSION} && make ARCH=${LINUX_ARCH} CROSS_COMPILE=${CROSS_COMPILER_PERFIX} ${IMAGE} -j$(THREADS)

gdb-in-docker :
	qemu-system-x86_64 -kernel src/${LINUX_VERSION}/arch/${LINUX_ARCH}/boot/bzImage -s -S -append "console=ttyS0" -nographic

fs-defconfig-in-docker :
	cd src/${BUSYBOX} && make defconfig && \
	cd - && make fs-defconfig-after-in-docker-${HOST_ARCH}

fs-menuconfig-in-docker :
	cd src/${BUSYBOX} && make menuconfig

fs-clean-in-docker :
	cd src/${BUSYBOX} && make clean

rootfs-in-docker :
	cd src/${BUSYBOX} && make -j$(THREADS) && make install && bash rootfs.sh

fs-distclean-in-docker :
	cd src/${BUSYBOX} && make distclean

dump-in-docker :
	objdump -s -d src/${LINUX_VERSION}/vmlinux > dump.s

run-in-docker :
	qemu-system-${TARGET_ARCH}  \
		-nographic \
		${MACHINE} \
		-smp 4 -m 2G \
		-kernel ./src/${LINUX_VERSION}/arch/${LINUX_ARCH}/boot/${IMAGE} \
		-initrd src/${BUSYBOX}/rootfs.img.gz \
		-append "root=/dev/ram console=${CONSOLE} init=/linuxrc"

# 在镜像外的操作
build-image :
	sudo docker build -t ${DOCKER_IMAGE} Docker/${HOST_ARCH}/${LINUX_VERSION} --build-arg BUILDKIT_INLINE_CACHE=1 

run-image :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	-it ${DOCKER_IMAGE} \
	/bin/bash

clean-image :
	sudo docker rmi ${DOCKER_IMAGE}

run :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it ${DOCKER_IMAGE} \
	make HOST_ARCH=${HOST_ARCH}  LINUX_VERSION=${LINUX_VERSION} run-in-docker; \
	sudo docker rm buildlinux

menuconfig : defconfig
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it ${DOCKER_IMAGE} \
	make HOST_ARCH=${HOST_ARCH}  LINUX_VERSION=${LINUX_VERSION} menuconfig-in-docker; \
	sudo docker rm buildlinux

defconfig :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it ${DOCKER_IMAGE} \
	make HOST_ARCH=${HOST_ARCH}  LINUX_VERSION=${LINUX_VERSION} defconfig-in-docker; \
	sudo docker rm buildlinux

fs-defconfig :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it ${DOCKER_IMAGE} \
	make HOST_ARCH=${HOST_ARCH}  LINUX_VERSION=${LINUX_VERSION} fs-defconfig-in-docker; \
	sudo docker rm buildlinux

fs-menuconfig : fs-defconfig
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it ${DOCKER_IMAGE} \
	make HOST_ARCH=${HOST_ARCH}  LINUX_VERSION=${LINUX_VERSION} fs-menuconfig-in-docker; \
	sudo docker rm buildlinux

# 这里必须要加 --privileged, 否则挂载文件时会提示无权限。
rootfs : 
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--privileged \
	--name buildlinux \
	-it ${DOCKER_IMAGE} \
	make HOST_ARCH=${HOST_ARCH}  LINUX_VERSION=${LINUX_VERSION} rootfs-in-docker; \
	sudo docker rm buildlinux

fs-distclean :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it ${DOCKER_IMAGE} \
	make HOST_ARCH=${HOST_ARCH}  LINUX_VERSION=${LINUX_VERSION} fs-distclean-in-docker; \
	sudo docker rm buildlinux

clean :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it ${DOCKER_IMAGE} \
	make HOST_ARCH=${HOST_ARCH}  LINUX_VERSION=${LINUX_VERSION} clean-in-docker ; \
	sudo docker rm buildlinux

fs-clean :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it ${DOCKER_IMAGE} \
	make HOST_ARCH=${HOST_ARCH}  LINUX_VERSION=${LINUX_VERSION} fs-clean-in-docker ; \
	sudo docker rm buildlinux

image :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	--memory-reservation ${MEM} \
	-it ${DOCKER_IMAGE} \
	make HOST_ARCH=${HOST_ARCH}  LINUX_VERSION=${LINUX_VERSION} image-in-docker; \
	sudo docker rm buildlinux;
	

distclean :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it ${DOCKER_IMAGE} \
	make HOST_ARCH=${HOST_ARCH}  LINUX_VERSION=${LINUX_VERSION} distclean-in-docker; \
	sudo docker rm buildlinux

devel : build-image

dump :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it ${DOCKER_IMAGE} \
	make HOST_ARCH=${HOST_ARCH}  LINUX_VERSION=${LINUX_VERSION} dump-in-docker; \
	sudo docker rm buildlinux

all : defconfig fs-defconfig rootfs image run

cleanall :	distclean fs-distclean

rungdb :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	-p 1234:1234 \
	--name gdb \
	-itd ${DOCKER_IMAGE} \
	make gdb-in-docker

stopgdb :
	sudo docker stop gdb; \
	sudo docker rm gdb

restartgdb : stopgdb rungdb

debug :
	gdb src/${LINUX_VERSION}/vmlinux -q -ex "target remote localhost:1234"