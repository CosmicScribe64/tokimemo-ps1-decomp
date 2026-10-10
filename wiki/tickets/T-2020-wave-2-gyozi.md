---
id: T-2020
title: "Wave 2: GYOZI"
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match the remaining `INCLUDE_ASM` functions of overlay GYOZI (`src/ovl/GYOZI.c`, `include/ovl/GYOZI.h`), at least 60.

## Acceptance criteria

- [x] At least 60 functions matched: 178 (GYOZI 244 of 417 in `ninja progress`).
- [x] Clean build (`rm -rf asm build`, configure, `ninja`): 27 of 27 OK, header check passes.
- [x] T-0018 cases recorded in [[data/t0018-cases]].
- [x] Inline review against CODING_STANDARDS, findings resolved.

## Notes

## Comments
- 2026-10-09 result: 178 functions matched (33 event-call wrappers, 127 setters/table fills/small conditionals, 18 more after type fixes, string setters, record struct and compare-chain switches). Clean build (`rm -rf asm build`, configure, `ninja`): 27 of 27 OK, `headers OK`. `ninja progress`: GYOZI 244/417, grand total 1112/6962 in this worktree.
- T-0018: 6 `regorder` rows added to [[data/t0018-cases]] (table fills ending in a copy of an `s16` from another overlay).
- Inline review against CODING_STANDARDS: C89 (declarations at block top, `/* */` only, no mixed declarations; checked by script), SDK/type rules (overlay-local K&R prototypes for main-exe callees as in the other overlays; no SDK redefinition), 8a (no symbol declared twice; `tools/check_headers.py` passes), struct layout commented (`GyoziWork`/`GyoziGirl`), seven `(u8)func_8005E0E0(...)` sites carry `FAKE` comments, no dead code, no game data staged, commits small with ticket id. Findings: one (unused extern declarations left in the header by the drafting helper); fixed by removing 154 unreferenced declarations. No open findings.
- Left `INCLUDE_ASM` (173): patterns listed in [[matching-notes]], section "Wave 2: GYOZI".
