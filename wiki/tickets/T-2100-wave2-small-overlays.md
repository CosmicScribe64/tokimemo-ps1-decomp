---
id: T-2100
title: "Wave 2: small overlays"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]", "[[data/t0018-cases]]"]
---

## Goal

Decompile functions in `src/ovl/ENDING.c`, `EN_NICHI.c`, `NAME_ENT.c`, `SHUGAKU.c`, `OPTION.c`, `KANGEI.c`, `DATE2.c`, `BUNKA_SD.c`, `OMIMAI.c`, `VALEN.c`, `MASTER.c` (IDO 5.3 with the frame pass, `-Wo,-nokpicopt`), smallest unblocked first, at least 60 matches. Headers: `include/ovl/<NAME>.h`.

## Acceptance criteria

- [x] At least 60 functions matched, all 27 sha1 checks OK after a clean rebuild. Result: 94 (ENDING 17, SHUGAKU 21, NAME_ENT 18, EN_NICHI 16, KANGEI 11, BUNKA_SD 3, DATE2 3, OMIMAI 2, VALEN 2, OPTION 1, MASTER 0); `ninja progress` grand total 901 -> 995 of 6962
- [x] T-0018 cases recorded in [[data/t0018-cases]] (3 rows); new patterns noted in [[matching-notes]] (section Wave 2 small overlays)
- [x] Code-review gate run inline, findings resolved

## Notes

Wave 2 batch agent `w2-small`, branch `w2-small`. Not merged.

## Comments

- Code review (inline, CODING_STANDARDS section 13 checklist, no sub-agents): every match verified by a full `ninja` (27 of 27 sha1 OK after `rm -rf asm build` and a clean configure) and, for the bulk-converted functions, by comparing each function's linked bytes with the original overlay file; `tools/check_headers.py` passes. C89 only (no `//`, `inline`, `bool`, mixed declarations); fixed-width types; new externs declared once per overlay header with the access width of the asm (`lbu` u8, `lh` s16, `lw` s32), added only for functions that stayed in C (unused declarations pruned); callee names are the applied names from `build/main_names.ld`. One fakematch, marked `FAKE` in ENDING `func_80136C5C` (second global written as `(&D_8011F4F2)[0x22]` to stop a load hoist). `*(s16 *)0x801E6638`-style reads follow the existing rule for addresses above 0x80162000. Cosmetic leftovers: m2c-style redundant casts and `U` suffixes in some compare expressions (`(u8) D >= 0x61U`), kept because they compile to the original bytes. No `NON_MATCHING` blocks, no generated asm or game data staged. Findings: none open.
- Tooling issues seen (not fixed, `tools/` is out of scope here): `tools/m2c.py` takes the alphabetically first overlay when a function address exists in several overlays; `funcdiff.py` compares stale objects when the overlay does not compile, and from the host python it fails with `NameError: SRC_GLOB` (line 68). Details in [[matching-notes]].
