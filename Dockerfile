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

# Install CUnit
RUN mkdir -p ~/downloads
RUN wget --no-check-certificate https://mirror.msys2.org/mingw/mingw64/mingw-w64-x86_64-cunit-2.1.3-3-any.pkg.tar.xz -O ~/downloads/mingw-w64-x86_64-cunit-2.1.3-3-any.pkg.tar.xz
RUN tar -xf ~/downloads/mingw-w64-x86_64-cunit-2.1.3-3-any.pkg.tar.xz -C /usr/local/share

# Add non-root user & create working environment
RUN groupadd -g 1000 dev
RUN useradd -u 1000 -g dev vpr-pidjeon-dev
USER vpr-pidjeon-dev
ENV HOME=/home/vpr-pidjeon-dev
WORKDIR /var/vpr-pidjeon-dev/vpr-pidjeon
