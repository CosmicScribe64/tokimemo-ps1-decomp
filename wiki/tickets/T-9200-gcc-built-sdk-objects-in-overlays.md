---
id: T-9200
title: gcc-built SDK objects inside overlays
status: Done
assignee: r6-gcc
created: 2026-10-10
updated: 2026-10-10
links: ["[[toolchain]]", "[[build-system]]", "[[matching-notes]]"]
---

## Goal
Tooling round 6. Some overlay objects are PsyQ gcc output (TACO `func_8015D270`, `func_801420DC`, `func_801421AC`; DATE `func_8015A980`; TT `func_8014D260`), not IDO. Find them all with a general fingerprint, let `configure.py` pick the compiler per overlay object, make the old gcc available in the arm64 image, and match what is plain C.

## Acceptance criteria
- [x] Fingerprint tool with tests; list of gcc-built overlay functions in the wiki.
- [x] `configure.py` chooses the toolchain per overlay object from a config file.
- [x] gcc 2.7.2-psx runs in the native arm64 image, with output identical to the amd64 binary.
- [x] Plain-C gcc functions matched.
- [x] Clean build 27/27 OK, headers OK, progress not lower.

## Notes
Result summary (details in [[gcc-objects]] and [[toolchain]]):
- Fingerprint `tools/gcc_fingerprint.py` (`addu rd,rs,$zero` moves, `j .L` local jumps; 0 hits in 4521 matched IDO functions): 8 functions in the overlays. Six are hand-written libgte-style rotation helpers (not gcc output: `$t7`-first registers, `multu`; gcc 2.6.3 to 2.91.66 give `mult`, `$v0`-first), stay `INCLUDE_ASM`. TAIIKU `func_801488F0` is gcc 2.8.1-psx C and matches. TT `func_8014D260` is gcc 2.7-class C, 4 bytes off because of ASPSX's divide layout (open).
- TACO `func_801420DC` and `func_801421AC` were not gcc: plain IDO C matches (+2 functions).
- `config/toolchains.txt` + `configure.py` pick the toolchain per object (main and overlays); tests `tools/test_configure_toolchains.py`.
- arm64 image builds gcc 2.7.2-psx and 2.8.1-psx from the GNU tarballs (cpp, cc1 only); 110 of 110 test sources compile to byte-identical assembly against the amd64 release binaries. Licence: GCC is GPL; old-gcc has no licence file and only its patches are used (pinned commit); nothing is redistributed, no proprietary SDK binary.
- Functions gained: 3 (progress 4521 -> 4524). Clean build 27/27 OK, headers OK, globals OK.

## Review (inline, CODING_STANDARDS.md)
- 7a: the only build-wide behaviour added is `--expand-div` for the gcc path (uniform; ASPSX expands the three-operand divide; evidence TT `func_8014D260`, four divisions). The per-object table is a compiler choice with evidence, not a flag override. No per-function switches.
- 2/9/11: C89, `/* */` comments only, tools have tests, no game data or asm committed.
- Finding fixed: first draft declared `s16 v;` unused in `func_801421AC`; removed.
- No open findings.

## Comments
