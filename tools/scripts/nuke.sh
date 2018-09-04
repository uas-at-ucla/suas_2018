#!/bin/bash

# Remove all running UAS@UCLA docker containers.
while true
do
  docker ps -a &> /dev/null
  if [[ $? > 0 ]]
  then
    # Docker is not running.
    break
  fi

  UAS_AT_UCLA_ENV_DOCKER_RUNNING_CONTAINER=$(docker ps \
    --filter name=uas_env \
    --filter status=running \
    --format "{{.ID}}" \
    --latest \
  )

  if [[ ! -z $UAS_AT_UCLA_ENV_DOCKER_RUNNING_CONTAINER ]]
  then
    echo "Killing docker container: $UAS_AT_UCLA_ENV_DOCKER_RUNNING_CONTAINER"
    docker kill $UAS_AT_UCLA_ENV_DOCKER_RUNNING_CONTAINER
  fi

  UAS_AT_UCLA_ENV_DOCKER_CONTAINER=$(docker ps \
    --filter name=uas_env \
    --format "{{.ID}}" \
    --latest \
  )

  if [[ -z $UAS_AT_UCLA_ENV_DOCKER_CONTAINER ]]
  then
    break
  else
    echo "Removing docker container: $UAS_AT_UCLA_ENV_DOCKER_CONTAINER"
    docker rm $UAS_AT_UCLA_ENV_DOCKER_CONTAINER
  fi
done

# Remove all docker-machines.
if [[ $(uname -s) == "Darwin" ]]
then
  UAS_AT_UCLA_ENV_DOCKER_RUNNING_MACHINE=$(
    docker-machine ls \
      --filter "name=uas-env" \
      --filter "state=Running" \
      --format "{{.Name}}"
  )

  UAS_AT_UCLA_ENV_DOCKER_MACHINE=$(
    docker-machine ls \
      --filter "name=uas-env" \
      --format "{{.Name}}"
  )

  # Kill running machines.
  if [[ ! -z $UAS_AT_UCLA_ENV_DOCKER_RUNNING_MACHINE ]]
  then
    echo "Killing docker machine: $UAS_AT_UCLA_ENV_DOCKER_RUNNING_MACHINE"
    docker-machine kill "uas-env"
  fi

  # Delete all machines.
  if [[ ! -z $UAS_AT_UCLA_ENV_DOCKER_MACHINE ]]
  then
    echo "Removing docker machine: $UAS_AT_UCLA_ENV_DOCKER_MACHINE"
    docker-machine rm -y "uas-env"
  fi
fi

# Remove all cache.
echo "\nRemoving all cache..."
rm -rf tools/cache
