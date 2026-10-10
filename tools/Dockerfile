# PS1 decomp toolchain, built natively for the host architecture (T-3310): linux/amd64 or
# linux/arm64. Game code needs only IDO, asm-processor, the binutils mips cross tools and
# Python, all available natively on both. The old-gcc PsyQ compilers (unused by game code)
# are x86_64-only ELF binaries, so they are installed on amd64 only; on Apple Silicon use
# TOKIMEMO_PLATFORM=linux/amd64 for an image that has them. See tools/docker.sh.
# CI overrides BASE with a registry mirror: GitHub runners hit Docker Hub pull limits (HTTP 429).
ARG BASE=ubuntu:22.04
FROM ${BASE}

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
      ca-certificates curl git unzip xz-utils make ninja-build build-essential \
      python3 python3-pip python3-venv binutils-mips-linux-gnu \
      gcc-mips-linux-gnu libicu70 \
    && rm -rf /var/lib/apt/lists/*

# Pinned tool versions
ARG OLD_GCC_TAG=0.17
ARG OBJDIFF_TAG=v3.8.2
ARG MKPSXISO_TAG=v2.30
ARG IDO_TAG=v1.2
# Commit of IDO_TAG (decompals/ido-static-recomp); arm64 builds IDO from source at this commit
ARG IDO_REV=9c242adc890beef098020149d9554f48208f699d
ARG ASM_PROCESSOR_REV=f3b2f85a5bc7e8ae2030ea5a83ea8aa0bcbede03
ARG MASPSX_REV=e85ecb373828aabea96d7e6400974d4f91ce8c74
ARG PERMUTER_REV=8556c81d80d1c1af98a858c8f4dc951357f29139

# Python tooling 
RUN pip3 install --no-cache-dir "splat64[mips]==0.50.0" spimdisasm==1.42.4 rabbitizer==1.16.2 pycparser==3.11 colorama==0.4.6 \
      pyyaml==6.0.3 tqdm==4.67.1 crcmod==1.7 ninja_syntax==1.7.2 \
    && pip3 install --no-cache-dir git+https://github.com/matt-kempster/m2c.git@708d2d2cb2698f091a92492b328f73b24209f72d

# maspsx (assembler wrapper for PsyQ-style GAS output)
RUN git clone https://github.com/mkst/maspsx /opt/maspsx \
    && git -C /opt/maspsx checkout ${MASPSX_REV}

# PsyQ-era GCC builds (decompals/old-gcc). Each lives in /opt/gcc/<ver>/ (cc1, etc.)
# x86_64 binaries only: skipped on other architectures (no game code uses them).
RUN mkdir -p /opt/gcc && cd /opt/gcc && \
    if [ "$(dpkg --print-architecture)" = amd64 ]; then \
      for v in 2.6.3-psx 2.7.2-psx 2.7.2-cdk 2.8.0-psx 2.8.1-psx 2.91.66-psx 2.95.2-psx; do \
        mkdir -p $v && curl -fsSL https://github.com/decompals/old-gcc/releases/download/${OLD_GCC_TAG}/gcc-$v.tar.gz \
          | tar xz -C $v; \
      done; \
    fi

# SGI IDO 5.3 / 7.1 (decompals/ido-static-recomp), candidate compiler for the
# game code (T-0013). Each lives in /opt/ido/<ver>/ (cc, cfe, uopt, ugen, as1...).
# amd64: the upstream release binaries. Other architectures (arm64): recompiled from the
# same pinned commit with the upstream release flags (make RELEASE=1), which is what the
# upstream x86 and linux-arm releases do. Output is verified byte-identical (T-3310).
RUN mkdir -p /opt/ido && cd /opt/ido && \
    if [ "$(dpkg --print-architecture)" = amd64 ]; then \
      for v in 5.3 7.1; do \
        mkdir -p $v && curl -fsSL https://github.com/decompals/ido-static-recomp/releases/download/${IDO_TAG}/ido-$v-recomp-linux.tar.gz \
          | tar xz -C $v; \
      done; \
    else \
      git init -q /tmp/ido-src && cd /tmp/ido-src \
      && git fetch -q --depth 1 https://github.com/decompals/ido-static-recomp ${IDO_REV} \
      && git checkout -q FETCH_HEAD \
      && make -j"$(nproc)" RELEASE=1 setup \
      && for v in 5.3 7.1; do \
           make -j"$(nproc)" RELEASE=1 VERSION=$v \
           && mkdir -p /opt/ido/$v && cp -a build/$v/out/. /opt/ido/$v/; \
         done \
      && cd / && rm -rf /tmp/ido-src; \
    fi

# asm-processor: lets IDO-compiled C keep INCLUDE_ASM functions (T-0013)
RUN git clone https://github.com/simonlindholm/asm-processor /opt/asm-processor \
    && git -C /opt/asm-processor checkout ${ASM_PROCESSOR_REV}

# decomp-permuter (T-1330): random/manual C permutation against target bytes. Runtime deps only
# (pynacl and docker are for its remote/import features, which we do not use).
RUN pip3 install --no-cache-dir toml==0.10.2 Levenshtein==0.27.1 \
    && git clone https://github.com/simonlindholm/decomp-permuter /opt/decomp-permuter \
    && git -C /opt/decomp-permuter checkout ${PERMUTER_REV}

# objdiff-cli (release asset names the architecture x86_64 / aarch64)
RUN case "$(dpkg --print-architecture)" in amd64) OA=x86_64;; arm64) OA=aarch64;; esac \
    && curl -fsSL -o /usr/local/bin/objdiff-cli \
      https://github.com/encounter/objdiff/releases/download/${OBJDIFF_TAG}/objdiff-cli-linux-$OA \
    && chmod +x /usr/local/bin/objdiff-cli

# mkpsxiso / dumpsxiso: upstream ships x86_64 Linux builds only, so amd64 only. No build
# step uses them (tools/extract_disc.py replaces dumpsxiso).
RUN if [ "$(dpkg --print-architecture)" = amd64 ]; then \
      curl -fsSL -o /tmp/mk.zip https://github.com/Lameguy64/mkpsxiso/releases/download/${MKPSXISO_TAG}/mkpsxiso-${MKPSXISO_TAG#v}-Linux.zip \
      && unzip -q /tmp/mk.zip -d /tmp/mk \
      && find /tmp/mk -type f \( -name dumpsxiso -o -name mkpsxiso \) -exec install -m755 {} /usr/local/bin/ \; \
      && rm -rf /tmp/mk /tmp/mk.zip; \
    fi

ENV MASPSX_DIR=/opt/maspsx GCC_DIR=/opt/gcc IDO_DIR=/opt/ido ASM_PROCESSOR_DIR=/opt/asm-processor PERMUTER_DIR=/opt/decomp-permuter
WORKDIR /work
