FROM debian:bookworm-slim

RUN set -eux; \
        apt-get update; \
        apt-get install -y --no-install-recommends \
            build-essential \
            ca-certificates \
            clangd \
            clang-format \
            clang-tidy \
            cmake \
            cppcheck \
            curl \
            doxygen \
            gcovr \
            gdb \
            git \
            graphviz \
            iwyu \
            lcov \
            llvm \
            ninja-build \
            pkg-config \
            python3 \
            tar \
            unzip \
            zip \
        ; \
        rm -rf /var/lib/apt/lists/*

ENV VCPKG_FORCE_SYSTEM_BINARIES "1"
ENV VCPKG_ROOT "$HOME/vcpkg"

RUN git clone https://github.com/Microsoft/vcpkg.git && \
    ./vcpkg/bootstrap-vcpkg.sh

WORKDIR /sandbox
