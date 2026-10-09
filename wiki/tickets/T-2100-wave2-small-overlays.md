---
id: T-2100
title: "Wave 2: small overlays"
status: In Progress
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]", "[[data/t0018-cases]]"]
---

## Goal

Decompile functions in `src/ovl/ENDING.c`, `EN_NICHI.c`, `NAME_ENT.c`, `SHUGAKU.c`, `OPTION.c`, `KANGEI.c`, `DATE2.c`, `BUNKA_SD.c`, `OMIMAI.c`, `VALEN.c`, `MASTER.c` (IDO 5.3 with the frame pass, `-Wo,-nokpicopt`), smallest unblocked first, at least 60 matches. Headers: `include/ovl/<NAME>.h`.

## Acceptance criteria

- [ ] At least 60 functions matched (funcdiff MATCH), all 27 sha1 checks OK after a clean rebuild
- [ ] T-0018 cases recorded in [[data/t0018-cases]]; new patterns noted in [[matching-notes]]
- [ ] Code-review gate run inline, findings resolved

## Notes

Wave 2 batch agent `w2-small`, branch `w2-small`. Not merged.

## Comments
