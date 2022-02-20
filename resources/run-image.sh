#!/bin/sh

# Creator:    VPR
# Created:    February 20th, 2022
# Updated:    February 20th, 2022

# Description:
#     Mounts local current working directory to /root in the container

set -e pipefail
set -e errexit
set -e nounset
set -e xtrace

IMAGE="pidjeon-dev"
MOUNT_DIR="${PWD}"

docker run -itv "${MOUNT_DIR}:/root:z" "${IMAGE}:${IMAGE}"
