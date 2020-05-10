FROM ubuntu:18.04

LABEL Description="This image is used to build PolyEngine for Linux" Vendor="PolyEngine team" Version="1.0"

RUN apt-get update -qq

# Gcc9 repository
RUN apt-get install -y software-properties-common
RUN add-apt-repository -y ppa:jonathonf/gcc-9.2
RUN apt-get update -qq

# Core tools
RUN apt-get install -y \
    git \
    make \
    cmake \
    python3 \
    python3-pip \
    clang-9 \
    g++-9 \
    gcc-9

# Engine dependancies
RUN apt-get install -y \
    mesa-common-dev \
    libgl1-mesa-dev \
    qt5-default

RUN pip3 install gitpython appdirs

RUN mkdir -p /root/workspace
