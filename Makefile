# 当前目录
PWD = $(shell pwd)
BUSYBOX_DIR = src/busybox
LINUX_DIR= src/linux
# 主机 arch
HOST_ARCH = $(shell uname -m)
# 编译的线程数
THREADS ?= 32
# 编译时内存限制
MEM_LIMIT ?= 64G
# 默认目标体系结构就是主机体系结构
# 如若想交叉编译,则需要指定
TARGET_ARCH ?= ${HOST_ARCH}
# 待编译的 linux 源码版本
LINUX_VERSION ?= linux-4.9.229
# docker 镜像名
DOCKER_IMAGE = linux-lib-${TARGET_ARCH}-${LINUX_VERSION}:latest
# docker 容器名
DOCKER_CONTAINER = linux-lib-${TARGET_ARCH}-${LINUX_VERSION}
# GCC 依赖库
GCCLIB_PATH = /lib/x86_64-linux-gnu

# 环境检测
## 1. 编译主机必须是 x64 体系结构
ifeq ($(findstring x86_64,$(HOST_ARCH)),)
$(error "The system has detected that your host is not an x64 architecture.");
endif

# 动态变量设置
## 1. 根据目标体系结构判断 linux 的对应目录名
LINUX_ARCH ?=
LINUX_CONSOLE ?=
LINUX_IMAGE ?=
QEMU_MACHINE ?=
GDB_ARCH ?=
### USB 主机控制器
QEMU_USB_HCI ?=
ifeq ("$(TARGET_ARCH)", "x86_64")
	LINUX_ARCH = x86_64
	LINUX_CONSOLE = ttyS0
	LINUX_IMAGE = bzImage
	GDB_ARCH = i386:x86-64
	QEMU_USB_HCI = nec-usb-xhci
else ifeq ("$(TARGET_ARCH)", "aarch64")
	LINUX_ARCH = arm64
	LINUX_CONSOLE = ttyAMA0
	LINUX_IMAGE = Image
	GDB_ARCH = aarch64
	QEMU_MACHINE = -machine virt,gic-version=2 -cpu cortex-a53
	QEMU_USB_HCI = qemu-xhci
	ifeq ("$(LINUX_VERSION)", "linux-2.6.34")
		$(error "linux-2.6.34 does not support aarch64!!!");
	endif
else
	$(error "unkown arch!!!");
endif

## 2. 根据本地体系结构与目标体系结构判断是否需要交叉编译
CROSS_COMPILER_PERFIX ?=
ifneq ("$(HOST_ARCH)", "$(TARGET_ARCH)")
	ifeq ("$(LINUX_VERSION)", "linux-3.12")
		CROSS_COMPILER_PERFIX = /opt/gcc-linaro-4.9-2016.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
		GCCLIB_PATH = /opt/gcc-linaro-4.9-2016.02-x86_64_aarch64-linux-gnu/aarch64-linux-gnu/libc/lib
	else ifeq ("$(LINUX_VERSION)", "linux-4.9.229")
		CROSS_COMPILER_PERFIX = /opt/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
		GCCLIB_PATH = /opt/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-linux-gnu/aarch64-linux-gnu/libc/lib
	else
		$(error "does not support $(LINUX_VERSION) !!!");
	endif
endif

### 待编译的 busybox 源码
BUSYBOX ?=
## 3. 根据待编译linux版本做选择 busybox 版本
ifeq ("$(LINUX_VERSION)", "linux-2.6.34")
	BUSYBOX = busybox-1.15.3
else ifeq ("$(LINUX_VERSION)", "linux-3.12")
	BUSYBOX = busybox-1.22.0
else ifeq ("$(LINUX_VERSION)", "linux-4.9.229")
	BUSYBOX = busybox-1.30.0
else
	$(error "does not support $(LINUX_VERSION) !!!");
endif

# debug 提示
$(info 编译信息如下：);
$(info 主机体系结构 = ${HOST_ARCH});
$(info 目标体系结构 = ${TARGET_ARCH});
$(info 编译线程数 = ${THREADS});
$(info 编译时内存限制 = ${MEM_LIMIT});
$(info 编译Linux版本 = ${LINUX_VERSION});
$(info 编译BUSYBOX版本 = ${BUSYBOX});

# docker image
.PHONY += build-image run-image clean-image
# config
.PHONY += defconfig menuconfig fs-defconfig fs-menuconfig
# compile and run
.PHONY += image devel dump all cleanall
# 运行在容器 使用串口 
.PHONY += run flush
# 运行在主机 使用串口 
.PHONY += drun dflush
# 运行在主机 使用图形化终端
.PHONY += grun gflush
# debug
.PHONY += gdb-start gdb-stop gdb-restart gdb-attch debug
.PHONY += image-in-docker rootfs-in-docker
.PHONY += grun-in-docker run-in-docker
.PHONY += defconfig-in-docker menuconfig-in-docker
.PHONY += fs-defconfig-in-docker fs-menuconfig-in-docker
.PHONY += clean-in-docker distclean-in-docker
.PHONY += gdb-start-in-docker dump-in-docker

defconfig-after-in-docker-x86_64 :
	cd ${LINUX_DIR}/${LINUX_VERSION} && \
	scripts/config --enable DEBUG_INFO && \
	scripts/config --disable USB_OHCI_HCD && \
	scripts/config --disable USB_UHCI_HCD && \
	scripts/config --disable USB_EHCI_HCD && \
	scripts/config --enable USB_XHCI_HCD && \
	scripts/config --disable XHCI_HCD_DEBUGGING && \
	scripts/config --disable USB_XHCI_HCD_DEBUGGING && \
	scripts/config --enable BLK_DEV_RAM && \
	scripts/config --set-val BLK_DEV_RAM_COUNT 16 && \
	scripts/config --set-val BLK_DEV_RAM_SIZE 65536

defconfig-after-in-docker-aarch64 :
	cd ${LINUX_DIR}/${LINUX_VERSION} && \
	scripts/config --set-val DRM y && \
	scripts/config --set-val VIRTIO y && \
	scripts/config --set-val DRM_VIRTIO_GPU y && \
	scripts/config --enable DEBUG_INFO && \
	scripts/config --disable USB_OHCI_HCD && \
	scripts/config --disable USB_UHCI_HCD && \
	scripts/config --disable USB_EHCI_HCD && \
	scripts/config --enable USB_XHCI_HCD && \
	scripts/config --enable XHCI_HCD_DEBUGGING && \
	scripts/config --enable USB_XHCI_HCD_DEBUGGING && \
	scripts/config --enable BLK_DEV_RAM && \
	scripts/config --set-val BLK_DEV_RAM_COUNT 16 && \
	scripts/config --set-val BLK_DEV_RAM_SIZE 65536 && \
	scripts/config --disable ARM64_UAO

fs-defconfig-after-in-docker-x86_64 :
	cd ${BUSYBOX_DIR}/${BUSYBOX}

fs-defconfig-after-in-docker-aarch64 :
	cd ${BUSYBOX_DIR}/${BUSYBOX} && \
	scripts/config --set-str CROSS_COMPILER_PREFIX "${CROSS_COMPILER_PERFIX}"

# 在镜像环境内的操作
defconfig-in-docker :
	cd ${LINUX_DIR}/${LINUX_VERSION} && make ARCH=${LINUX_ARCH} defconfig && cd - && \
	make defconfig-after-in-docker-${TARGET_ARCH} && \
	cd ${LINUX_DIR}/${LINUX_VERSION} && make ARCH=${LINUX_ARCH} localmodconfig && cd - 

menuconfig-in-docker :
	cd ${LINUX_DIR}/${LINUX_VERSION} && make menuconfig

distclean-in-docker :
	cd ${LINUX_DIR}/${LINUX_VERSION} && make distclean

clean-in-docker :
	cd ${LINUX_DIR}/${LINUX_VERSION} && make clean

image-in-docker :
	cd ${LINUX_DIR}/${LINUX_VERSION} && make ARCH=${LINUX_ARCH} CROSS_COMPILE=${CROSS_COMPILER_PERFIX} ${LINUX_IMAGE} -j$(THREADS)

gdb-start-in-docker :
	qemu-system-${TARGET_ARCH}  \
		-nographic \
		${QEMU_MACHINE} \
		-smp 4 -m 2G \
		-device ${QEMU_USB_HCI} \
		-drive file=${PWD}/usbdisk.img,if=none,id=my_usb_disk \
		-usb -device usb-storage,drive=my_usb_disk \
		-kernel ${LINUX_DIR}/${LINUX_VERSION}/arch/${LINUX_ARCH}/boot/${LINUX_IMAGE} \
		-initrd ${BUSYBOX_DIR}/${BUSYBOX}/rootfs.img.gz \
		-s -S \
		-append "root=/dev/ram console=${LINUX_CONSOLE} init=/linuxrc"

fs-defconfig-in-docker :
	cd ${BUSYBOX_DIR}/${BUSYBOX} && make defconfig && \
	cd - && make fs-defconfig-after-in-docker-${TARGET_ARCH}

fs-menuconfig-in-docker :
	cd ${BUSYBOX_DIR}/${BUSYBOX} && make menuconfig

fs-clean-in-docker :
	cd ${BUSYBOX_DIR}/${BUSYBOX} && make clean

rootfs-in-docker :
	cd ${BUSYBOX_DIR}/${BUSYBOX} && make -j$(THREADS) && make install && bash rootfs-${LINUX_ARCH}.sh ${GCCLIB_PATH}

fs-distclean-in-docker :
	cd ${BUSYBOX_DIR}/${BUSYBOX} && make distclean \
	&& rm -rf _install/ \
	&& rm -rf rootfs.ext3 \
	&& rm -rf rootfs.img.gz

dump-in-docker :
	objdump -s -d ${LINUX_DIR}/${LINUX_VERSION}/vmlinux > dump.s

# 串口运行
run-in-docker :
	qemu-system-${TARGET_ARCH}  \
		-nographic \
		${QEMU_MACHINE} \
		-smp 4 -m 2G \
		-device ${QEMU_USB_HCI} \
		-drive file=${PWD}/usbdisk.img,if=none,id=my_usb_disk \
		-usb -device usb-storage,drive=my_usb_disk \
		-kernel ${LINUX_DIR}/${LINUX_VERSION}/arch/${LINUX_ARCH}/boot/${LINUX_IMAGE} \
		-initrd ${BUSYBOX_DIR}/${BUSYBOX}/rootfs.img.gz \
		-append "root=/dev/ram console=${LINUX_CONSOLE} init=/linuxrc"

# 图形化运行
grun-in-docker :
	qemu-system-${TARGET_ARCH}  \
		${QEMU_MACHINE} \
		-smp 4 -m 2G \
		-device ${QEMU_USB_HCI} \
		-drive file=${PWD}/usbdisk.img,if=none,id=my_usb_disk \
		-usb -device usb-storage,drive=my_usb_disk \
		-device usb-kbd \
		-kernel ${LINUX_DIR}/${LINUX_VERSION}/arch/${LINUX_ARCH}/boot/${LINUX_IMAGE} \
		-initrd ${BUSYBOX_DIR}/${BUSYBOX}/rootfs.img.gz \
		-device virtio-gpu -display gtk,gl=on \
		-append "root=/dev/ram console=tty0 init=/linuxrc"

usbdisk-in-docker:
	bash scripts/create_usbdisk.sh

gdb-connect-in-docker:
	gdb-multiarch ${LINUX_DIR}/${LINUX_VERSION}/vmlinux -q \
	-ex "set architecture ${GDB_ARCH}" \
	-ex " target remote localhost:1234"

# 在镜像外的操作
build-image :
	sudo docker build -t ${DOCKER_IMAGE} Docker/${TARGET_ARCH}/${LINUX_VERSION} --build-arg BUILDKIT_INLINE_CACHE=1 

run-image :
	sudo docker run \
	--privileged \
	--volume=${PWD}:/workdir:rw \
	-it ${DOCKER_IMAGE} \
	/bin/bash

clean-image :
	sudo docker rmi ${DOCKER_IMAGE}

run :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name ${DOCKER_CONTAINER} \
	-it ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH} LINUX_VERSION=${LINUX_VERSION} run-in-docker; \
	sudo docker rm ${DOCKER_CONTAINER}

stop :
	sudo docker stop ${DOCKER_CONTAINER}

drun :
	make TARGET_ARCH=${TARGET_ARCH} LINUX_VERSION=${LINUX_VERSION} run-in-docker;

grun :
	make TARGET_ARCH=${TARGET_ARCH} LINUX_VERSION=${LINUX_VERSION} grun-in-docker;

menuconfig : defconfig
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name ${DOCKER_CONTAINER} \
	-it ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} menuconfig-in-docker; \
	sudo docker rm ${DOCKER_CONTAINER}

defconfig :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name ${DOCKER_CONTAINER} \
	-it ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} defconfig-in-docker; \
	sudo docker rm ${DOCKER_CONTAINER}

fs-defconfig :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name ${DOCKER_CONTAINER} \
	-it ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} fs-defconfig-in-docker; \
	sudo docker rm ${DOCKER_CONTAINER}

fs-menuconfig : fs-defconfig
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name ${DOCKER_CONTAINER} \
	-it ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} fs-menuconfig-in-docker; \
	sudo docker rm ${DOCKER_CONTAINER}

# 这里必须要加 --privileged, 否则挂载文件时会提示无权限。
rootfs : 
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--privileged \
	--name ${DOCKER_CONTAINER} \
	-it ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} rootfs-in-docker; \
	sudo docker rm ${DOCKER_CONTAINER}

fs-distclean :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name ${DOCKER_CONTAINER} \
	-it ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} fs-distclean-in-docker; \
	sudo docker rm ${DOCKER_CONTAINER}

clean :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name ${DOCKER_CONTAINER} \
	-it ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} clean-in-docker ; \
	sudo docker rm ${DOCKER_CONTAINER}

fs-clean :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name ${DOCKER_CONTAINER} \
	-it ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} fs-clean-in-docker ; \
	sudo docker rm ${DOCKER_CONTAINER}

image :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name ${DOCKER_CONTAINER} \
	--memory-reservation ${MEM_LIMIT} \
	-it ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} image-in-docker; \
	sudo docker rm ${DOCKER_CONTAINER};
	
usbdisk:
	sudo docker run \
	--privileged \
	--volume=${PWD}:/workdir:rw \
	--name ${DOCKER_CONTAINER} \
	--memory-reservation ${MEM_LIMIT} \
	-it ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} usbdisk-in-docker; \
	sudo docker rm ${DOCKER_CONTAINER};

distclean :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name ${DOCKER_CONTAINER} \
	-it ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} distclean-in-docker; \
	sudo docker rm ${DOCKER_CONTAINER}

devel : build-image

dump :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name ${DOCKER_CONTAINER} \
	-it ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} dump-in-docker; \
	sudo docker rm ${DOCKER_CONTAINER}

all : defconfig fs-defconfig rootfs image run

cleanall :	distclean fs-distclean

flush : image run

dflush : image drun

gflush : image grun

fs-flush : fs-distclean fs-defconfig rootfs run

gdb-start :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name gdb \
	-itd ${DOCKER_IMAGE} \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} gdb-start-in-docker

gdb-stop :
	sudo docker stop gdb; \
	sudo docker rm gdb

gdb-attch :
	sudo docker exec -it gdb /bin/bash

gdb-restart : gdb-stop gdb-start

gdb-connect :
	sudo docker exec -it gdb \
	make TARGET_ARCH=${TARGET_ARCH}  LINUX_VERSION=${LINUX_VERSION} gdb-connect-in-docker