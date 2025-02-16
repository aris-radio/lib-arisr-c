# Use an official Ubuntu-based image
FROM ubuntu:22.04

# Install required packages: GCC, Valgrind, Make, Clang, and additional compilers
RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    make \
    cmake \
    valgrind \
    binutils \
    clang \
    lld \
    mingw-w64 \
    gcc-arm-none-eabi \
    gdb-multiarch \
    git \
    curl \
    wget \
    automake \
    autoconf \
    xz-utils \
    patch \
    libxml2-dev \
    libssl-dev \
    zlib1g-dev \
    gpg \
    && rm -rf /var/lib/apt/lists/*

# Set working directory inside the container
WORKDIR /app

# -------------------- Commented out for now --------------------
# ------------- macOS Compilation not supported yet -------------
# ---------------------------------------------------------------
# Clone and install osxcross for macOS cross-compilation
# RUN git clone https://github.com/tpoechtrager/osxcross.git /opt/osxcross && \
#     cd /opt/osxcross && \
#     wget -P tarballs/ "https://github.com/phracker/MacOSX-SDKs/releases/download/11.3/MacOSX10.15.sdk.tar.xz" && \
#     tar -xf tarballs/MacOSX10.15.sdk.tar.xz -C tarballs/ && rm tarballs/MacOSX10.15.sdk.tar.xz && \
#     SDK_VERSION=10.15 UNATTENDED=1 ./build.sh && \
#     ln -s target/bin/o64-clang /usr/local/bin/o64-clang

# Ensure the tmp directory exists for logs and temporary files
RUN mkdir -p /app/tmp

# Copy the entrypoint script and grant execution permissions
COPY ./entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

# Configure environment variables for osxcross
ENV PATH="/opt/osxcross/target/bin:$PATH"
ENV OSX_SDK_PATH="/opt/osxcross/target/SDK/MacOSX-SDK"

# Set the entrypoint for the container
ENTRYPOINT ["/bin/bash", "/entrypoint.sh"]
