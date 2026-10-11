# PS1 decomp toolchain, built natively for the host architecture (T-3310): linux/amd64 or
# linux/arm64. Game code needs only IDO, asm-processor, the binutils mips cross tools and
# Python, all available natively on both. The old-gcc PsyQ compilers
# are x86_64-only release binaries, so on arm64 gcc 2.7.2-psx is built from source (T-9200);
# the other old-gcc versions are amd64 only (TOKIMEMO_PLATFORM=linux/amd64). See tools/docker.sh.
# CI overrides BASE with a registry mirror: GitHub runners hit Docker Hub pull limits (HTTP 429).
ARG BASE=ubuntu:22.04
FROM ${BASE}

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
      ca-certificates curl git unzip xz-utils make ninja-build build-essential \
      python3 python3-pip python3-venv binutils-mips-linux-gnu \
      gcc-mips-linux-gnu libicu70 patch \
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

# PsyQ-era GCC builds (decompals/old-gcc). Each lives in /opt/gcc/<ver>/ (cpp, cc1, ...).
# amd64: the upstream release binaries (x86_64 only). arm64 (T-9200): 2.7.2-psx and 2.8.1-psx,
# the versions the build uses, are built from the GNU tarballs with the decompals/old-gcc
# patches (pinned commit, patches only; gcc is GPL and nothing built here is redistributed).
# Differences from upstream's i386 `-m32` Dockerfiles: there is no 32-bit host on arm64, so the
# host is LP64 (old-gcc's xm-darwin.h, a generic LP64 description, replaces xm-i386.h); the
# pointer-returning functions that gcc calls without a declaration get prototypes (else rtx
# pointers are cut to 32 bits and reorg.c crashes); only cpp and cc1 are built (the driver
# needs sys_nerr, gone from glibc, and tools/cc.py runs cpp and cc1 directly). Output was
# checked byte-identical to the amd64 release binaries (wiki/toolchain.md, T-9200).
ARG OLD_GCC_REV=b74211c9d959e9724802f3177c8229cd67202c87
ARG GCC272_SHA256=7cd8bce5c3aeec59a72ecc2d3d5123864a817b14cdbd0680b1a969c3bccc5da5
ARG GCC281_SHA256=3b30fbfdf93e628373d90d174243f3267b0eec9ebe792bb64fd15b8828c2ea4c
RUN mkdir -p /opt/gcc && cd /opt/gcc && \
    if [ "$(dpkg --print-architecture)" = amd64 ]; then \
      for v in 2.6.3-psx 2.7.2-psx 2.7.2-cdk 2.8.0-psx 2.8.1-psx 2.91.66-psx 2.95.2-psx; do \
        mkdir -p $v && curl -fsSL https://github.com/decompals/old-gcc/releases/download/${OLD_GCC_TAG}/gcc-$v.tar.gz \
          | tar xz -C $v; \
      done; \
    else \
      git init -q /tmp/old-gcc && git -C /tmp/old-gcc fetch -q --depth 1 https://github.com/decompals/old-gcc ${OLD_GCC_REV} \
      && git -C /tmp/old-gcc checkout -q FETCH_HEAD \
      && printf '%s\n' 'struct rtx_def; union tree_node;' \
           'extern struct rtx_def *alter_subreg (), *emit (), *emit_line_note (), *expand_expr (), *label_rtx (),' \
           '  *negate_rtx (), *remove_death (), *replace_regs ();' \
           'extern union tree_node *reorder_blocks (), *truthvalue_conversion ();' 'extern char *bc_xstrdup ();' \
           > /tmp/gcc-host-protos.h \
      && P=/tmp/old-gcc/patches \
      && CF="-std=gnu89 -w -fcommon -fno-strict-aliasing -include stdlib.h -include string.h -include strings.h -include alloca.h -include /tmp/gcc-host-protos.h" \
      && for v in 2.7.2 2.8.1; do \
           case $v in \
             2.7.2) url=https://ftp.gnu.org/old-gnu/gcc/gcc-2.7.2.tar.gz; sha=${GCC272_SHA256};; \
             2.8.1) url=https://ftp.gnu.org/gnu/gcc/gcc-2.8.1.tar.gz; sha=${GCC281_SHA256};; \
           esac \
           && curl -fsSL -o /tmp/gcc-$v.tar.gz $url && echo "$sha  /tmp/gcc-$v.tar.gz" | sha256sum -c - \
           && tar xzf /tmp/gcc-$v.tar.gz -C /tmp && cd /tmp/gcc-$v \
           && sed -i -- 's/include <varargs.h>/include <stdarg.h>/g' *.c \
           && if [ $v = 2.7.2 ]; then \
                patch -u -p1 obstack.h -i $P/obstack-2.7.2.h.patch \
                && patch -u -p1 configure -i $P/configure.patch \
                && patch -u -p1 config.sub -i $P/config.sub.patch \
                && patch -u -p1 config/mips/mips.h -i $P/mipsel-2.7.patch \
                && patch -su -p1 < $P/psx-2.5.7.patch \
                && sed -i '0,/^#include "config.h"/s//&\nstruct hashnode; extern struct hashnode *lookup ();/' cexp.c; \
              else \
                patch -u -p1 obstack.h -i $P/obstack-2.8.1.h.patch \
                && patch -u -p1 config/mips/mips.h -i $P/mips.patch \
                && patch -su -p1 < $P/psx.patch; \
              fi \
           && ./configure --target=mips-sony-psx --prefix=/opt/cross --with-endian-little --with-gnu-as \
                --disable-gprof --disable-gdb --disable-werror --host=i386-pc-linux --build=i386-pc-linux \
           && cp $P/xm-darwin.h config/i386/xm-i386.h && touch insn-config.h cexp.c \
           && make -j"$(nproc)" cpp cc1 CFLAGS="$CF" \
           && ./cc1 -quiet -O2 /tmp/old-gcc/tests/little_endian.c -o /tmp/le.s && grep -E 'lbu\s\$2,0\(\$4\)' /tmp/le.s \
           && ./cc1 -quiet -O2 /tmp/old-gcc/tests/host_wide_int.c -o /tmp/hw.s && grep -E '# 0x80000000$' /tmp/hw.s \
           && mkdir -p /opt/gcc/$v-psx && cp cpp cc1 /opt/gcc/$v-psx/ \
           && cd /opt/gcc && rm -rf /tmp/gcc-$v /tmp/gcc-$v.tar.gz /tmp/le.s /tmp/hw.s; \
         done \
      && rm -rf /tmp/old-gcc /tmp/gcc-host-protos.h; \
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

# Archive readers for tools/identify_version.py (T-3300): 7z (p7zip-full) and CHD (chdman)
RUN apt-get update && apt-get install -y --no-install-recommends \
      p7zip-full=16.02+dfsg-8 mame-tools=0.242+dfsg.1-1 \
    && rm -rf /var/lib/apt/lists/*

ENV MASPSX_DIR=/opt/maspsx GCC_DIR=/opt/gcc IDO_DIR=/opt/ido ASM_PROCESSOR_DIR=/opt/asm-processor PERMUTER_DIR=/opt/decomp-permuter
WORKDIR /work
