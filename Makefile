.PHONY : build-Docker-Image run-Docker-Image clean-Docker-Image

build-Docker-Image :
	sudo docker build -t linux-lib:latest Docker

run-Docker-Image :
	docker run \
	-w /workdir \
	--volume=${PWD}/src:/workdir:rw \
	-it linux-lib:latest \
	/bin/bash

clean-Docker-Image :
	docker rmi linux-lib:latest
