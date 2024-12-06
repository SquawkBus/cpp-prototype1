# Ubuntu 24.04 (noble)
FROM mcr.microsoft.com/devcontainers/base:noble

RUN apt update

# Get a recent compiler.
RUN apt install -y \
        g++-14 \
        clang \
        gdb

# Get uuid and yaml libraries.
RUN apt install -y \
        uuid-dev \
        libyaml-cpp-dev

# Get the htpasswd utility
RUN apt install -y \
        apache2-utils
    