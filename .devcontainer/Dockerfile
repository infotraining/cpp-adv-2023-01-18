FROM mcr.microsoft.com/devcontainers/cpp:0-ubuntu-22.04

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.22.2"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends g++-12

RUN vcpkg install Catch2

RUN sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 10
RUN sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 20
RUN sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 10
RUN sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 20
RUN sudo update-alternatives --install /usr/bin/cc cc /usr/bin/gcc 30
RUN sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++ 30
RUN sudo update-alternatives --set cc /usr/bin/gcc
RUN sudo update-alternatives --set c++ /usr/bin/g++


# [Optional] Uncomment this section to install additional packages.
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>
