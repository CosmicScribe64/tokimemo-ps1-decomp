---
type: research
updated: 2026-10-18
ticket: T-24047
---

# Reusing the ENDING allocator tracer

[[tickets/T-24047-ending-sprite-scene]] rebuilt the existing diagnostic from
[[uopt-register-allocation]]. All generated source and binaries are external:

- Host scratch: `/private/tmp/t24047-uopt` (62 MB after build).
- Instrumented executable: `/private/tmp/t24047-uopt/variants/trace`.
- Patched baseline: `/private/tmp/t24047-uopt/recomp/build/5.3/out/uopt`.
- Docker image: `tokimemo-decomp-arm64`; mount scratch at `/scratch`.
- Reconstruction revision: `9c242adc890beef098020149d9554f48208f699d`.
- Pristine generated `build/5.3/uopt.c` SHA256:
  `b0058f1559441c1a194d649271eb43b8637ec255682cfdd629031340b915b13f`.

The production `/opt/ido/5.3/uopt` is unchanged. The existing
`tools/ido-ecvt.patch` repairs diagnostic floating-point formatting;
`tools/uopt_patch.py trace` adds allocator logging without changing decisions.
No host packages were installed. Serial setup/build finished in about three
minutes, within the authorized 15-minute setup bound.

## Use the existing binary

Run from an isolated worktree's root. Substitute its absolute host path for
the example worktree mount when reusing this from another lane:

```sh
docker run --rm \
  -v /private/tmp/t24047-uopt:/scratch \
  -v /private/tmp/tokimemo-ending-large:/work -w /work \
  tokimemo-decomp-arm64 \
  python3 tools/regdiff.py ENDING:func_80135F4C \
    --src build/t24047-replay/candidate.c --uopt /scratch/variants/trace -v
```

Generate the preferred input and verify fidelity before interpreting traces:

```sh
docker run --rm -v /private/tmp/tokimemo-ending-large:/work -w /work \
  tokimemo-decomp-arm64 python3 drafts/T-24047/replay.py
docker run --rm \
  -v /private/tmp/t24047-uopt:/scratch \
  -v /private/tmp/tokimemo-ending-large:/work -w /work \
  tokimemo-decomp-arm64 python3 drafts/T-24047/trace_fidelity.py
```

The second command compiles the complete preferred TU three times, with stock,
patched-baseline and traced uopt. It compares section inventories and exact
`.text`, `.rel.text`, `.rodata`, `.rel.rodata`, `.data` and `.rel.data` bytes,
including absence of optional sections. It also compares the traced target's
resolved words to the preferred reference. All agree; target length is804
instructions. This proves diagnostic fidelity, not a match to the game.
Logs and objects stay under ignored `build/t24047/tracer-fidelity/`.

## Rebuild only if scratch is gone

The exact build sequence used is below. Run inside the same Docker image with
external scratch mounted at `/scratch` and the repository at `/work` read-only.
Use one compiler, and check the source hash before compiling it.

```sh
cd /scratch
git init -q recomp
cd recomp
git remote add origin https://github.com/decompals/ido-static-recomp.git
git fetch --depth 1 origin 9c242adc890beef098020149d9554f48208f699d
git checkout --detach FETCH_HEAD
git submodule update --init --depth 1
patch --fuzz=0 -p1 < /work/tools/ido-ecvt.patch
make -j1 RELEASE=1 setup
make -j1 RELEASE=1 VERSION=5.3 IDO_TC=uopt build/5.3/uopt.c
sha256sum build/5.3/uopt.c
make -j1 RELEASE=1 VERSION=5.3 IDO_TC=uopt build/5.3/out/uopt
mkdir -p /scratch/variants
python3 /work/tools/uopt_patch.py trace build/5.3/uopt.c /scratch/variants/trace.c
gcc -std=c11 -Os -fno-strict-aliasing -I. /scratch/variants/trace.c \
  build/5.3/libc_impl.o build/5.3/version_info.o -lm -o /scratch/variants/trace
```

Setup logs are `/private/tmp/t24047-uopt/setup.log`, `generate.log`,
`build-uopt.log` and `build-trace.log`. Generated compiler C and binaries must
remain external; do not commit them.

## Recovered allocation tie

In the43-cost baseline, ranges528 (499) and529 (128) each have priority5 and cheapest colour cost0.
They are constrained; ascending bit order gives499 colour6/a3 before128
colour7/t0. The original uses the opposite pair. This is a measured first-use
tie, rather than a priority imbalance. See [[ending-family-results]] for the
separate default-arm AS1 cross-block motion; no compiler decision changes are
enabled in the normal build. The current35-cost source fixes that tie by moving
UV before palette, spelling the later quotient directly, and placing the zero
store after UV. All loops/case bodies match; only default-arm motion remains.

## Original-version controls

T-24047 reused the cached `tokimemo-decomp-ido52` image (`linux/amd64`) and
the private-use approval in [[tickets/T-3110-test-ido-52-and-41]]. The archive
SHA256 is the existing
`870cd946d31d6abee24d47926328d15909771520240a0a4f5cb7d27aea90e1ad`.
Archive/extraction stay at `/private/tmp/t24047-uopt/ido5.2.tar.xz` and
`/private/tmp/t24047-uopt/ido5.2` (37 MB extracted). No OS image or host package
was installed; no production settings changed.

Five target controls are in `drafts/T-24047/backend-evidence.json`: substitute
only52 as1, only52 ugen, only52 cfe, only52 uopt, then all four52 stages together.
Every result is identical to stock53:804 instructions/cost35. A stock amd64/arm64
target equality control passes. All runs retain the existing conversion/frame
passes; no per-function or instruction patch is used. A simple3.18/3.19 release
difference does not explain this source's remaining motion.

As1/ugen inputs and symbol sidecars were copied by scratch wrappers, then passed
unchanged to the stock stages to verify capture fidelity. Captures, replay
scripts and output objects remain in ignored `build/t24047/` or external scratch.
The approved runtime invokes a pass as:

```sh
/scratch/ido5.2/usr/bin/qemu-irix -silent -L /scratch/ido5.2 \
  /scratch/ido5.2/usr/lib/as1 -elf -G 0 -p0 -EL -g0 -O2 \
  /work/build/t24047/switch-metadata/default.G \
  -o /scratch/as1-52.o -t /scratch/as1-52.T
```

Copy `build/t24047/switch-metadata/default.T` to the output sidecar before that
replay. Run it in the cached amd64 image with scratch writable and the project
read-only. [[ido-52-evaluation]] documents the other pass interfaces and archive
provenance. Compiler data and reconstructed C remain private, never committed.
