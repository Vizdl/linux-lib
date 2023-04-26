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

distclean-in-docker :
	cd src/linux && make distclean

image-in-docker :
	cd src/linux && make bzImage -j$(THREADS)

gdb-in-docker :
	qemu-system-x86_64 -kernel src/linux/arch/${ARCH}/boot/bzImage -s -S -append "console=ttyS0" -nographic


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
	qemu-system-x86_64 -nographic \
		-smp 4 -m 2G \
		-kernel ./src/linux/arch/x86/boot/bzImage
		# -machine ubuntu \
		# -append "root=/dev/ram0" \
		# -initrd ./build/rootfs.img \

defconfig :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib-${RARCH}:latest \
	make RARCH=${RARCH} defconfig-in-docker; \
	sudo docker rm buildlinux

image :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	--memory-reservation ${MEM} \
	-it linux-lib-${RARCH}:latest \
	make RARCH=${RARCH} image-in-docker; \
	sudo docker rm buildlinux

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