---
id: T-0004
title: splat config & split
status: In Review
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]", "[[executable]]", "config/SLPM_86.053.yaml"]
---

## Goal

Create the splat YAML for the boot executable and run the initial split.

## Acceptance criteria

- [x] `config/SLPM_86.053.yaml`: psx platform, PSYQ compiler, header, game code (c), SDK libs and libapi stubs (asm), rodata, data, bss (size 0x47D38).
- [x] Split output reassembles to the original (sha1 OK), see [[tickets/T-0005-build-system-and-checksum-matching]].

## Notes

Segment decisions and bss derivation: [[executable]]. Lib/game code boundary is a heuristic (see [[tickets/T-0010-sdk-lib-object-boundaries]]). Config and empty `config/symbol_addrs.txt`, `config/reloc_addrs.txt` are committed; `asm/` is generated and gitignored.

## Comments

- 2026-10-09: moved to In Review. Awaiting code-review against CODING_STANDARDS.md.
