RARCH ?= x86_64
ARCH ?= x86
THREADS ?= $(nproc)
MEM ?= 4G

ifeq ("$(RARCH)", "x86_64")
	ARCH = x86
endif

.PHONY += build-image run-image clean-image
.PHONY += defconfig image devel run restartgdb rungdb stopgdb
.PHONY += defconfig-in-docker distclean-in-docker image-in-docker gdb-in-docker

# 在镜像环境内的操作
defconfig-in-docker :
	cd src/linux && make defconfig

menuconfig-in-docker :
	cd src/linux && make menuconfig

distclean-in-docker :
	cd src/linux && make distclean

image-in-docker :
	cd src/linux && make bzImage -j$(THREADS)

gdb-in-docker :
	qemu-system-x86_64 -kernel src/linux/arch/${ARCH}/boot/bzImage -s -S -append "console=ttyS0" -nographic

fs-defconfig-in-docker :
	cd src/busybox-1.15.3 && make defconfig

fs-menuconfig-in-docker :
	cd src/busybox-1.15.3 && make menuconfig

fs-image-in-docker :
	cd src/busybox-1.15.3 && make -j32 && make install && sudo bash rootfs.sh

fs-distclean-in-docker :
	cd src/busybox-1.15.3 && make distclean

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
	sudo qemu-system-x86_64  \
		-smp 4 -m 2G \
		-kernel ./src/linux/arch/x86/boot/bzImage \
		-initrd src/busybox-1.15.3/rootfs.img.gz \
		-append "root=/dev/ram init=/linuxrc"	\
		-serial file:output.txt
		# -nographic

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

fs-image : 
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib-${RARCH}:latest \
	make RARCH=${RARCH} fs-image-in-docker; \
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
	gdb src/linux/vmlinux -q -ex "target remote localhost:1234"