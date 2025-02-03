FROM ubuntu:latest AS build

RUN set -ex; \
    apt-get update; \
    apt-get install -y build-essential git pkg-config ninja-build cmake libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libopengl-dev libglx-dev libgl1-mesa-dev libz-dev; \
    mkdir -p /usr/src/; \
    cd /usr/src/; \
    git clone https://github.com/wk1093/spectral.git; \
    cd spectral; \
    mkdir build; \
    cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -B build .; \
    cmake --build build;

WORKDIR /usr/src/Spectral/build

CMD ["/bin/bash", "-c", "ls", "/usr/src/Spectral/build"]

# COPY --from=build /usr/src/Spectral/build/Spectral.lin_x86_64 ./Spectral.lin_x86_64