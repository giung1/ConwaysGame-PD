#!/bin/bash

CONTAINER_NAME="PowerDEVS-Ubuntu-Agustin"

./startContainer.sh

# inicia como mi usuario
docker exec -it --user=$USER $CONTAINER_NAME bash
# docker exec -it $CONTAINER_NAME bash
#docker stop $CONTAINER_NAME
