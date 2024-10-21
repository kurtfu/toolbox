FROM ubuntu:24.04

RUN set -eux; \
        apt-get update; \
        apt-get install -y --no-install-recommends \
            build-essential \
            ca-certificates \
            clangd \
            clang-format \
            clang-tidy \
            cmake \
            curl \
            doxygen \
            gdb \
            git \
            graphviz \
            lcov \
            pkg-config \
            python3 \
            tar \
            unzip \
            valgrind \
            vim \
            wget \
            zip \
        ; \
        rm -rf /var/lib/apt/lists/*

WORKDIR /root

RUN set -eux; \
        git clone https://github.com/Microsoft/vcpkg.git; \
        ./vcpkg/bootstrap-vcpkg.sh; \
        echo "export VCPKG_ROOT=\$HOME/vcpkg" >> .bashrc; \
        echo "export PATH=\$VCPKG_ROOT:\$PATH" >> .bashrc

WORKDIR /root/sandbox
