# Creator:    VPR
# Created:    February 20th, 2022
# Updated:    February 20th, 2022

FROM ubuntu:20.04

# Set env to avoid user input interruption during installation
ENV TZ=America/Chicago
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install normal goodies
RUN apt-get update && apt upgrade -y
RUN apt-get install -y --no-install-recommends zsh \
                                               zsh-autosuggestions \
                                               git \
                                               curl \
                                               wget \
                                               vim \
                                               tree \
                                               make \
                                               cmake \
                                               build-essential \
                                               mingw-w64 \
                                               gdb \
                                               python3-dev


# Change login shell to zsh
RUN chsh -s /bin/zsh $(whoami)

# Create Utopia user && dir
ENV HOME=/root
WORKDIR /root

# Install CUnit
RUN mkdir -p ~/downloads
RUN wget --no-check-certificate https://mirror.msys2.org/mingw/mingw64/mingw-w64-x86_64-cunit-2.1.3-3-any.pkg.tar.xz -O ~/downloads/mingw-w64-x86_64-cunit-2.1.3-3-any.pkg.tar.xz
RUN tar -xf ~/downloads/mingw-w64-x86_64-cunit-2.1.3-3-any.pkg.tar.xz -C /usr/local/share
