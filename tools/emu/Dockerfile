# Headless PS1 emulator for runtime observation (T-20500): PCSX-Redux with its open-source
# OpenBIOS (MIT), no proprietary BIOS. Built natively for the Docker host (arm64 on Apple
# Silicon). Driven by tools/emu/emu.py through tools/emu/run.sh; see wiki/emulator.md.
#
# Stages: OpenBIOS needs a mipsel cross compiler, which Debian ships for arm64 hosts and Ubuntu
# does not; the emulator is built like upstream's own build image (Ubuntu 26.04); the final image
# keeps only the binary, the BIOS and the runtime packages (the build tree is about 7 GB).
# Builds run with -j2 (CPU headroom for other jobs on the host).

ARG REDUX_REV=8288e927e66f3f1148053136b4cca48a24b6f0b2
# nugget (src/mips submodule of PCSX-Redux at REDUX_REV): OpenBIOS source
ARG NUGGET_REV=d93840921b5ad7ed515e95b0e414f1b1e2038b1a

FROM debian:trixie AS openbios
ARG NUGGET_REV
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
      ca-certificates git make g++-mipsel-linux-gnu \
    && rm -rf /var/lib/apt/lists/*
RUN git clone -q https://github.com/pcsx-redux/nugget.git /nugget \
    && cd /nugget && git checkout -q ${NUGGET_REV} \
    && git submodule update -q --init third_party/uC-sdk
RUN make -C /nugget/openbios -j2 PREFIX=mipsel-linux-gnu FORMAT=elf32-tradlittlemips \
    && ls -l /nugget/openbios/openbios.bin

FROM ubuntu:26.04 AS build
ARG REDUX_REV
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
      ca-certificates git make pkg-config g++ \
      libavcodec-dev libavformat-dev libavutil-dev libswresample-dev libcapstone-dev \
      libcurl4-openssl-dev libfreetype-dev libsdl3-dev libuv1-dev zlib1g-dev \
      python3 gdb-multiarch \
    && rm -rf /var/lib/apt/lists/*
RUN git clone -q https://github.com/grumpycoders/pcsx-redux.git /opt/redux \
    && cd /opt/redux && git checkout -q ${REDUX_REV} \
    && git submodule update -q --init --recursive --jobs 2
RUN cd /opt/redux && make -j2 pcsx-redux
# Upstream fixes picked onto REDUX_REV (incremental rebuild):
# 3a24bbcc "spu: save a stopped ADPCM cursor as its own sentinel" (a state saved with a finished
# voice crashed in decodeBlock when loaded into a new process)
ARG REDUX_PICKS="3a24bbcc56ee98615a55f878d46050ec3ab32ce4"
RUN cd /opt/redux && for c in ${REDUX_PICKS}; do git cherry-pick -n "$c"; done && make -j2 pcsx-redux

FROM ubuntu:26.04
ENV DEBIAN_FRONTEND=noninteractive
# the -dev packages of the build stage pull in the matching runtime libraries
RUN apt-get update && apt-get install -y --no-install-recommends \
      libavcodec-dev libavformat-dev libavutil-dev libswresample-dev libcapstone-dev \
      libcurl4-openssl-dev libfreetype-dev libsdl3-dev libuv1-dev zlib1g-dev \
      python3 gdb-multiarch \
    && rm -rf /var/lib/apt/lists/*
COPY --from=build /opt/redux/pcsx-redux /opt/redux/pcsx-redux
COPY --from=openbios /nugget/openbios/openbios.bin /opt/redux/openbios.bin
ENV PCSX_REDUX=/opt/redux/pcsx-redux
WORKDIR /work
