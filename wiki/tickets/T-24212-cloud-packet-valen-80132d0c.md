---
id: T-24212
title: "Cloud packet: VALEN func_80132D0C"
status: Done
assignee: claude (cloud session)
created: 2026-10-20
updated: 2026-10-20
links: [[[cloud-matching]], [[matching-notes]], [[register-cheatsheet]], [[tickets/T-24033-valen-selector-ports]], [[tickets/T-24138-small-overlays-wave-3-closers]], src/ovl/VALEN/80132760.c]
---

## Goal

Match VALEN `func_80132D0C` (892 B, `src/ovl/VALEN/80132760.c`) in a cloud session from a match packet ([[cloud-matching]]). Start: guarded draft at 7 resolved rows (T-24033, T-24138): the dispatch shift is in a later branch delay slot than in the original.

## Acceptance criteria

- [x] Scratch funcdiff: asm fallback MATCH, then the NON_MATCHING C judged by instructions.
- [x] MATCH -> plain C; else the guard note updated with T-24212 and the row count.
- [x] Note in [[matching-notes]]; branch `cloud/t24212-valen-80132760` pushed (C and wiki only).
- [ ] Local linked build proves the default build unchanged (local session).

## Notes

- 2026-10-20: toolchain per [[cloud-matching]]; Docker Hub answered 429 for `ubuntu:22.04`, so the base image came from `mirror.gcr.io/library/ubuntu:22.04` (tagged `ubuntu:22.04`), then the CA-injected build worked. Asm fallback equals the reference; the draft is 3 rows in an objdump row diff.
- Result: no match. The as1 trace (`-R -xbbdbg 8`) locates the difference in as1's global scheduler: the `sel == 9` block is visited first and takes the `lui v0` of the mode reload from its fallthrough instead of the dispatch `sll`. About 30 C variants plus a scan of 340 compiled switch files; details in [[matching-notes]] (T-24212). Guard comment updated (comment-only change to the C file).

## Comments

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): cloud branch changed only its C file and wiki, no asm or game data; merged with a clean 27/27 build and header/global/prototype checks. No match; guard at 3 rows with an as1 scheduling note. Done.
