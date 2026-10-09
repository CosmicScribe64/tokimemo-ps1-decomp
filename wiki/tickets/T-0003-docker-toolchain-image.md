---
id: T-0003
title: Docker toolchain image
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]", "[[toolchain]]", "tools/Dockerfile", "tools/docker.sh"]
---

## Goal

Docker image with splat, maspsx, gcc (PsyQ-era), binutils-mips, and build tools. No host installs.

## Acceptance criteria

- [x] Image builds and runs the toolchain (`tools/docker.sh`, `tools/Dockerfile`); pinned versions listed in [[toolchain]].
- [x] Used end to end by [[tickets/T-0005-build-system-and-checksum-matching]] (splat, gcc 2.7.2-psx cc1, maspsx, binutils, ninja).

## Notes

Image `tokimemo-decomp` is linux/amd64 only. Review finding fixed: all Python deps and m2c (commit 708d2d2) are now pinned in `tools/Dockerfile`; a fresh `docker build` of the pinned file was run to verify.

## Comments

- 2026-10-09: moved to In Review. Awaiting code-review against CODING_STANDARDS.md.
- 2026-10-09: code review (vs CODING_STANDARDS.md, range 060900b..HEAD) run. Findings: Dockerfile pins (all Python deps and m2c commit pinned; fresh docker build verified). Fixed. Build re-verified from a clean tree after fixes: `build/SLPM_86.053.bin: OK`. Moved to Done.
