FROM gcc:13.2.0

RUN set -eux; \
        apt-get update; \
        apt-get install -y --no-install-recommends \
            clangd \
            clang-format \
            clang-tidy \
            cmake \
            cppcheck \
            curl \
            doxygen \
            gcovr \
            gdb \
            iwyu \
            lcov \
            llvm \
            ninja-build \
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
