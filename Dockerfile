# Creator:    VPR
# Created:    February 20th, 2022
# Updated:    June 3rd, 2022

FROM ubuntu:20.04

# Set env to avoid user input interruption during installation
env tz=america/chicago
run ln -snf /usr/share/zoneinfo/$tz /etc/localtime && echo $tz > /etc/timezone

# install normal goodies
run apt-get update && apt upgrade -y
run apt-get install -y --no-install-recommends \
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
    gdb

# Change login shell to zsh
RUN chsh -s /bin/zsh $(whoami)

# Create working environment
ENV HOME=/root
WORKDIR /var/pidjeon-dev/pidjeon

# Install CUnit
RUN mkdir -p ~/downloads
RUN wget --no-check-certificate https://mirror.msys2.org/mingw/mingw64/mingw-w64-x86_64-cunit-2.1.3-3-any.pkg.tar.xz -O ~/downloads/mingw-w64-x86_64-cunit-2.1.3-3-any.pkg.tar.xz
RUN tar -xf ~/downloads/mingw-w64-x86_64-cunit-2.1.3-3-any.pkg.tar.xz -C /usr/local/share
