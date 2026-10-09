---
id: T-0750
title: "Overlay batch B: TEL, OLH, EN_NICHI"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal

Decompile functions in `src/ovl/TEL.c`, `src/ovl/OLH.c` and `src/ovl/EN_NICHI.c` (IDO 5.3 with the frame pass), smallest first, aiming for about 50 matches. Per-overlay externs and types go in `include/ovl/<NAME>.h`.

## Acceptance criteria

- [x] Functions matched (funcdiff MATCH), all 27 sha1 checks OK. Result: 12 of the aimed ~50 (EN_NICHI 9, TEL 2, OLH 1); the rest hit the toolchain gaps listed in [[matching-notes]]
- [x] Failures that show a new pattern noted in [[matching-notes]] (section Overlay batch B)
- [x] Code-review gate run inline, findings resolved

## Notes

Matched: EN_NICHI `func_801369F4`, `func_801338EC`, `func_80132000`, `func_80132A0C`, `func_80132308`, `func_80134008`, `func_801321EC`, `func_80132E3C`, `func_80132378`; TEL `func_8013BF50`, `func_8013A76C`; OLH `func_801323AC`. Headers: `include/ovl/EN_NICHI.h`, `TEL.h`, `OLH.h`.

Renames for the orchestrator at merge (old name used in the headers and in `src/ovl/EN_NICHI.c`): `func_80043914` -> `load_palette`, `func_8006492C` -> `hizuke_disp_switch`, `func_80064E48` -> `message_disp_switch` (all in `include/ovl/EN_NICHI.h` and its callers in `src/ovl/EN_NICHI.c`). Every other main-exe name used is not in `config/obin_renames.txt`.

## Comments

- Code review (inline, CODING_STANDARDS section 13 checklist, no sub-agents): matches verified with `funcdiff.py` (12 MATCH) and `ninja` (27 of 27 sha1 OK); no `NON_MATCHING`, no fakematch, no `FAKE` needed; C89 only (grep for `//`, `inline`, `bool` clean); headers have include guards and use `s32/u8/s16`; the one struct (`Rec30`) documents offsets and size; placeholders kept for all names; no generated asm or game data staged (only `src/ovl/*.c` and `include/ovl/*.h`). Findings: none open. Spec axis: ticket goal asked for about 50 matches; 12 reached, shortfall explained by the systematic gaps in [[matching-notes]] (not by time).
