FROM jenkins/inbound-agent:latest

USER root

# Install C++ toolchain and common build tools
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        gdb \
        git \
        ninja-build \
        pkg-config && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

USER jenkins
