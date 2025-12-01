#!/bin/bash

xhost +local:docker

# IMAGE_NAME=camisa/powerdevs_devel:latest  # public image in docker:hub (758 MB)
#IMAGE_NAME=camisa/powerdevs_devel:eclipse # public image with eclipse (1GB)
#IMAGE_NAME=camisa/powerdevs_devel:personal # private image with personal eclipse settings 
#IMAGE_NAME=powerdevs_devel_img_$USER     # local image (built locally with ./buildPowerdevsImage.sh)
IMAGE_NAME=powerdevs_devel_img_ubuntu_$USER

CONTAINER_NAME="PowerDEVS-Ubuntu-Agustin"

if [ ! "$(docker ps -aq -f name=$CONTAINER_NAME)" ]; then
	echo "Creating container $CONTAINER_NAME ..."

	# note: this is VERY UNSECURE and RISKY. It adjust the permissions the X server host
	# see: http://wiki.ros.org/docker/Tutorials/GUI for other more secure options
	xhost +local:root # for the lazy and reckless
	xhost +local:$USER # for the lazy and reckless

	# create container	
	docker create -it \
	    --env="DISPLAY" \
	    --env="QT_X11_NO_MITSHM=1" \
	    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
	    -v $PWD/../..:/powerdevs \
	    --workdir=/powerdevs \
	    --cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
	    --name $CONTAINER_NAME \
	    $IMAGE_NAME

	# create the local user inside container (this is to avoid running with root inside container)
	docker start $CONTAINER_NAME 
	docker exec $CONTAINER_NAME groupadd --gid=$(id -g) $(id -gn) 
	docker exec $CONTAINER_NAME useradd --uid=$(id -u) --gid=$(id -g) $USER
fi

if [ "$(docker ps -aq -f status=exited -f name=$CONTAINER_NAME)" ]; then
	#start container
	docker start $CONTAINER_NAME
fi

echo "Container $CONTAINER_NAME is now running..."


