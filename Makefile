DARCH=x86_64
.PHONY += build-image run-image clean-image
.PHONY += defconfig build image

build-image :
	sudo docker build -t linux-lib-${DARCH}:latest Docker/${DARCH} --build-arg BUILDKIT_INLINE_CACHE=1 

run-image :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	-it linux-lib-${DARCH}:latest \
	/bin/bash &

clean-image :
	sudo docker rmi linux-lib-${DARCH}:latest

config :
	cd src/linux && make defconfig

clean :
	cd src/linux && make distclean

image :
	cd src/linux && make bzImage -j64

# gdb :
# 	qemu-system-x86_64 -kernel src/linux/DARCH/x86_64/boot/bzImage -s -S -append "console=ttyS0" -nographic

		# -machine ubuntu \
		# -append "root=/dev/ram0" \
		# -initrd ./build/rootfs.img \

qemu :
	qemu-system-x86_64 -nographic \
		-smp 4 -m 2G \
		-kernel ./src/linux/arch/x86/boot/bzImage

defconfig :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib-${DARCH}:latest \
	make DARCH=${DARCH} config; \
	sudo docker rm buildlinux

build :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib-${DARCH}:latest \
	make DARCH=${DARCH} image; \
	sudo docker rm buildlinux

distclean :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib-${DARCH}:latest \
	make DARCH=${DARCH} clean; \
	sudo docker rm buildlinux

# dockergdb :
# 	sudo docker run \
# 	--volume=${PWD}:/workdir:rw \
# 	-p 1234:1234 \
# 	--name gdb \
# 	-itd linux-lib:latest \
# 	make gdb

# dockersgdb :
# 	sudo docker stop gdb; \
# 	sudo docker rm gdb

# dockerdbg :
# 	gdb src/linux/vmlinux -q -ex "target remote localhost:1234"