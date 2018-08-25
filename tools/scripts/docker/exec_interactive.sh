#!/bin/bash

function docker_exec {
    unset UAS_AT_UCLA_IMAGE
    UAS_AT_UCLA_IMAGE=$(docker ps \
      --filter status=running \
      --filter name=uas_env \
      --format "{{.ID}}" \
      --latest)

    if [ -z $UAS_AT_UCLA_IMAGE ]
    then
      echo "Could not find uas env docker image. Exiting..."
      exit 1
    fi

    PIDFILE=/tmp/docker-exec-$$
    NAMEFILE=/tmp/docker-exec-$$

    docker exec -it -u $(id -u):$(id -g) $UAS_AT_UCLA_IMAGE \
      bash -c "echo \"\$\$\" > \"$PIDFILE\".pid; echo \"$*\" > \"$NAMEFILE\".name;$*"

    exit $?
}

docker_exec "$@"
