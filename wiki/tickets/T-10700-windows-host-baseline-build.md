---
id: T-10700
title: Windows host setup and baseline build
status: Done
assignee: claude
created: 2026-10-13
updated: 2026-10-13
links: ["[[toolchain]]", "[[build-system]]", "[[versions]]", "tools/docker.sh"]
---

## Goal

Run the Docker build on a Windows 11 host (Docker Desktop, WSL2 backend, Git Bash) and reproduce the
last recorded state: 27/27 SHA1 checks, progress `5162/6958`.

## Acceptance criteria

- [x] Docker Desktop engine up; toolchain image built through `tools/docker.sh`
- [x] Best disc in `game/best/` identified and unpacked by the build
- [x] `configure.py` + `ninja`: all 27 SHA1 checks pass
- [x] Progress matches `5162/6958`
- [x] Windows quirks (if any) fixed in tooling and documented

## Notes

Host: Windows 11, Docker Desktop 29.8.2 (x86_64, 14 CPUs, 8 GB). Docker CLI is not on the Git Bash PATH
by default (`%LOCALAPPDATA%\Programs\DockerDesktop\resources\bin`).

## Comments
2026-10-13 (claude): done.
- Image `tokimemo-decomp` (amd64) built by `tools/docker.sh` in a few minutes (release binaries, no source builds).
- Fix 1, `tools/docker.sh`: under MSYS, `MSYS_NO_PATHCONV=1` and the repo passed as `pwd -W`; before,
  `-w /work` became `C:/Program Files/Git/work` and docker refused the run.
- Fix 2, `tools/cc.py` (gcc path): the old-gcc `cpp` is 32-bit and fails with EOVERFLOW
  ("Value too large for defined data type") on stat() of the WSL2 bind mount (64-bit inodes).
  It now preprocesses a copy of `include/` and the source in `/dev/shm` under the same relative
  paths; output unchanged on other hosts. Only `src/ovl/TAIIKU/801488F0.c` (gcc 2.8.1-psx) hit it.
  `tools/test_cc.py` 14/14 OK.
- Build: disc identified as Best and unpacked (1355 files); full build from clean about 2 minutes
  on 14 CPUs; 27/27 `.bin: OK` after deleting every `.ok` and re-running ninja.
- Progress: `grand total   5176/6958   961516/2279368  42.2%` (above the T-10630 log figure 5162
  because the wave-14 list-2 merge came after it).
- Review against CODING_STANDARDS.md (inline): tooling is Python 3/bash, Docker-run, no host installs,
  covered by the existing tests; no findings.
2026-10-13 (claude): review of PR CosmicScribe64/tokimemo-ps1-decomp#1 by the owner: the `cc.py` copy ran on every
host. Now a fallback: `run_cpp` runs `cpp` in place as before and retries from the `/dev/shm` copy only when it
fails with EOVERFLOW ("Value too large for defined data type"), which also covers Linux filesystems with 64-bit
inodes. Any other cpp failure exits as before; `run_stage` is back to its original form. New tests
`CppEoverflowFallback` (fake cpp: retry with the same relative paths, in place when cpp works, no retry on other
errors); `tools/test_cc.py` 17/17; 27/27 `.bin: OK` after deleting every `.ok`.
