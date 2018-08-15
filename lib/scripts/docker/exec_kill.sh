#!/bin/bash

unset UAS_AT_UCLA_IMAGE
UAS_AT_UCLA_IMAGE=$(docker ps \
  --filter status=running \
  --filter name=uas_env \
  --format "{{.ID}}" \
  --latest)

docker exec -t $UAS_AT_UCLA_IMAGE sh -c "for f in /tmp/docker-exec-*.pid;do PID=\$(cat \"\$f\");echo \"KILLING \$PID\";kill -15 \$PID || true;done"
