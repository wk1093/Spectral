FROM docker.io/fedora:latest AS build

# RUN set -ex; \
#     pacman --noconfirm -Syu; \
#     pacman --noconfirm -S base-devel git pkg-config ninja cmake libx11 libxrandr libxinerama libxcursor libxi libglvnd libgl mesa zlib; \
#     mkdir -p /usr/src/; \
#     cd /usr/src/;

RUN set -ex; \
    dnf -y update; \
    dnf -y install git pkg-config ninja-build cmake libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel libGL-devel mesa-libGLU-devel zlib-devel gcc gcc-c++; \
    mkdir -p /usr/src/; \
    cd /usr/src/;

WORKDIR /usr/src/

RUN set -ex; \
    git clone https://github.com/wk1093/spectral.git; \
    cd spectral; \
    mkdir build; \
    cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -B build .; \
    cmake --build build;

CMD ["/bin/bash", "-c", "cd /usr/src/spectral/bin; ./Spectral.lin_x86_64 glfw_gl glad"]