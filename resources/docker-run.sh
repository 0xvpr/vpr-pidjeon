#!/bin/sh

# Creator:    VPR
# Created:    June 3rd, 2022
# Updated:    June 3rd, 2022

set -e pipefail
set -e errexit
set -e nounset
set -e xtrace

PROJECT="pidjeon"

DOCKER_IMAGE="${PROJECT}-dev"
DOCKER_WORK_DIR="/var/${DOCKER_IMAGE}/${PROJECT}"

docker run -itv "${PWD}:${DOCKER_WORK_DIR}" "${DOCKER_IMAGE}"
