---
id: T-3310
title: Tooling: native Docker image for Apple Silicon
status: Done
assignee: agent (t2-native)
created: 2026-10-09
updated: 2026-10-09
links: ["[[toolchain]]", "[[build-system]]", "[[ci]]"]
---

## Goal

The image was forced to linux/amd64 because of the old-gcc PsyQ compilers, so every build on Apple Silicon ran under emulation. Game code only needs IDO 5.3, asm-processor, splat/spimdisasm, the binutils mips tools, objdiff-cli, m2c, decomp-permuter and maspsx. Build the image natively for the host architecture, keep CI on x86, and prove the output is byte-identical.

## Acceptance criteria

- [x] `tools/Dockerfile` builds for linux/amd64 and linux/arm64. IDO is recompiled from the pinned commit on arm64; old-gcc and mkpsxiso are amd64 only.
- [x] `tools/docker.sh` picks the native platform, `TOKIMEMO_PLATFORM` overrides it, the Dockerfile-hash label logic stays.
- [x] CI (`.github/workflows/progress.yml`) stays on linux/amd64.
- [x] Clean build on the native image: 27 of 27 OK, headers OK. Same on the old amd64 image. Wall-clock times recorded. Every built binary byte-identical between the two.
- [x] `tools/test_frame_pass.py`, `test_cc.py` and the other tool tests pass on the native image.
- [x] Documented in [[toolchain]] and [[build-system]].
- [x] Inline review against CODING_STANDARDS.md.

## Notes

Results and evidence: see "Native image (T-3310)" in [[toolchain]]. Wall-clock numbers (laptop under heavy load from parallel agents, load average 40 on 10 cores; both builds measured back to back, `rm -rf asm build` first):
- pair 1: native arm64 1m05s (64.5 s), old amd64 3m19s (199.4 s): 3.1x.
- pair 2 (later): native 1m30s (90.3 s), old amd64 4m12s (252.2 s): 2.8x. A third native run (concurrent with an image build) took 2m10s.
- Speedup about 3x on a machine that was far from idle; an idle machine should show the emulation penalty more clearly. Image build: arm64 about 10 min (IDO recompiled from source, loaded machine); amd64 image from the new Dockerfile is content-identical to the old one (hash of `/opt/ido`, `/opt/gcc`, `/usr/local/bin` equal).
- Outputs: all 27 `.bin`, `.elf` and `.ok` files byte-identical between the two images; 537 `.o` identical after masking asm-processor's random temp directory name. Tests (`tools/test_*.py`, 14 files including `test_frame_pass.py` and `test_cc.py`) pass on both images.

Finding while checking CI: the workflow built the image without the `dockerfile.sha256` label, so `tools/docker.sh` would have rebuilt it from Docker Hub on the first call (the BASE mirror existed to avoid exactly that). The workflow now sets the label.

## Comments
