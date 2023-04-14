.PHONY : build-Docker-Image run-Docker-Image clean-Docker-Image

build-Docker-Image :
	sudo docker build -t linux-lib:latest Docker --build-arg BUILDKIT_INLINE_CACHE=1 

run-Docker-Image :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	-it linux-lib:latest \
	/bin/bash

clean-Docker-Image :
	docker rmi linux-lib:latest

defconfig :
	cd src/linux && make x86_64_defconfig

image :
	cd src/linux && make bzImage -j64

gdb :
	qemu-system-x86_64 -kernel src/linux/arch/x86_64/boot/bzImage -s -S -append "console=ttyS0" -nographic

dockerdefconfig :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib:latest \
	make defconfig && \
	sudo docker rm buildlinux

dockerimage :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	--name buildlinux \
	-it linux-lib:latest \
	make image && \
	sudo docker rm buildlinux

dockergdb :
	sudo docker run \
	--volume=${PWD}:/workdir:rw \
	-p 1234:1234 \
	--name gdb \
	-itd linux-lib:latest \
	make gdb

dockersgdb :
	sudo docker stop gdb; \
	sudo docker rm gdb

dockerdbg :
	gdb src/linux/vmlinux -q -ex "target remote localhost:1234"