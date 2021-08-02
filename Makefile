.PHONY : build-Docker-Image-Base build-Docker-Image-High build-Docker-Image-Low \
		run-Docker-Image clean-Docker-Image

build-Docker-Image-Base :
	sudo docker build -t linux-lib-base:latest -f ./Docker/Dockerfile-Base ./Docker/source

build-Docker-Image-High :
	sudo docker build -t linux-lib-high:latest -f ./Docker/Dockerfile-High ./Docker/source

build-Docker-Image-Low :
	sudo docker build -t linux-lib-low:latest -f ./Docker/Dockerfile-Low ./Docker/source

run-Docker-Image :
	docker run \
	-w /workdir \
	--volume=${PWD}/src:/workdir:rw \
	-it linux-lib:latest \
	/bin/bash

clean-Docker-Image :
	docker rmi linux-lib:latest
