# Creator:    VPR
# Created:    February 20th, 2022
# Updated:    May 6th, 2023

FROM ubuntu:22.04

# Set env to avoid user input interruption during installation
ENV TZ=America/Chicago
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# install normal goodies
RUN apt-get update && apt upgrade -y
RUN apt-get install -y --no-install-recommends \
    zsh \
    zsh-autosuggestions \
    git \
    curl \
    wget \
    vim \
    tree \
    make \
    cmake \
    build-essential \
    mingw-w64-common \
    mingw-w64-i686-dev \
    mingw-w64-tools \
    mingw-w64-x86-64-dev \
    mingw-w64 \
    python3 \
    gdb

# Change login shell to zsh
RUN chsh -s /bin/zsh $(whoami)

# Create working environment
ENV HOME=/root
WORKDIR /var/vpr-pidjeon-dev/vpr-pidjeon
