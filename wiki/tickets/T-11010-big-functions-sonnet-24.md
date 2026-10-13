---
id: T-11010
title: Big functions (Sonnet) 24
status: Done
assignee: sonnet
created: 2026-10-13
updated: 2026-10-13
links: []
---

## Goal

Match big functions in TAIIKU/80138950, NAME_ENT/80139740, main 800674B0 and NAME_ENT/80132000 (list astra24, 60 functions).

## Acceptance criteria

- [x] ninja 27/27 OK, headers and globals OK
- [x] inline review against CODING_STANDARDS.md

## Notes

Result: 7 functions matched (4,680 bytes), 8 guarded near misses. Details and levers: [[matching-notes]] section "Big functions (Sonnet) 24".

Matched: NAME_ENT `func_80139CB4`, `func_801387F4`, `func_80137FC4`, `func_80142028`, `func_8013D054`, `func_8013B77C`, `func_801372A0`.
Near misses (`#ifdef NON_MATCHING`, INCLUDE_ASM fallback, T-11010 in the comment): `func_8013F208`, `func_80135BB4`, `func_8014231C`, `func_80135168`, `func_801357F4`, `func_80135F74`, `func_8013BB24`, `func_801377E8`.
Commit `bacef2d` is mislabelled ("match TAIIKU func_8013C0E4"): it only holds the ticket; that function stays INCLUDE_ASM (first draft failed at the switch/global-promotion registers, not kept).

Inline review (CODING_STANDARDS): C89 declarations at block top; `/* */` comments only; every NON_MATCHING block carries a ticket id and keeps the INCLUDE_ASM fallback; the only oversized local (`sp30[0x108]`, `func_80139CB4`, `func_8013F208`) is marked FAKE; no dead code; new overlay declarations appended to `include/ovl/NAME_ENT.h` under T11010 guards; main-exe globals added through `sync_protos.py --add-global` and the unused ones removed again; `sync_protos --check-branch`, `migrate_globals --check`, `check_headers` and the 27 sha1 checks pass. No findings.

## Comments
