---
id: T-24200
title: "Cloud packet pilot: TACO func_8013B460"
status: In Review
assignee: claude (cloud session)
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[cloud-matching]], [[register-cheatsheet]], src/ovl/TACO/8013B170.c]
---

## Goal

Pilot of a "match packet": the local session sends the original disassembly of one C file
(`src/ovl/TACO/8013B170.c`) to a cloud session, which recreates it under the gitignored `asm/`,
compiles that one file with IDO 5.3 and funcdiffs `func_8013B460` (1,328 bytes, two `POLY_FT4`
quads for a wipe; 41 rows at T-19030). Second goal: learn how to run the toolchain in a cloud
container and document it in [[cloud-matching]].

## Acceptance criteria

- [x] Toolchain reproduces the original object of the unchanged file (sha1 `40a801c5...`) and MATCH for the four matched functions
- [x] `func_8013B460` matched, or guarded with a row count, or back to `INCLUDE_ASM`
- [x] [[cloud-matching]] written, linked from [[index]]
- [x] T-24200 section in [[matching-notes]]
- [ ] Local linked proof (full build) by the local session

## Notes

No packet content (asm) is committed; branch `cloud/t24200-taco-8013b460` holds C and wiki only.

## Comments

### 2026-10-20 result (cloud session)

- Toolchain: worked. Docker daemon started by hand, image built with the proxy CA injected (about 8 min); unchanged file reproduces sha1 `40a801c5a66bec258366d8069fe8011b70b2302c`; the four matched functions compare equal (scratch objdump diff; funcdiff's MATCH/diff paths need the disc). Recipe in [[cloud-matching]].
- `func_8013B460`: not matched. 41 -> 4 differing lines (2 rows, one per quad: the `sh` of `p->y3` sits before the UV byte stores instead of after the `lui` of the `AddPrim` argument). Guarded `NON_MATCHING` in `src/ovl/TACO/8013B170.c`; the default build is unchanged (same sha1). Details in [[matching-notes]] (T-24200).
- Still due: the local linked proof (full `ninja`, funcdiff on the default build, SHA1) and the code review against `CODING_STANDARDS.md`.

