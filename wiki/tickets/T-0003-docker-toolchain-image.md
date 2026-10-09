---
id: T-0003
title: Docker toolchain image
status: In Review
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

Image `tokimemo-decomp` is linux/amd64 only; the m2c dependency is unpinned (git HEAD), noted in [[toolchain]].

## Comments

- 2026-10-09: moved to In Review. Awaiting code-review against CODING_STANDARDS.md.
