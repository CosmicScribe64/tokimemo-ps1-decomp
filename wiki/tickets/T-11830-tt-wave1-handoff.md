---
id: T-11830
title: TT: take over the seven wave-1 TT matches (tt-wave1-handoff)
status: Done
assignee: claude (diman8 fork)
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-11820-tt-batch-9]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal
The maintainer's wave 1 had matched seven TT functions in packages p10 and p12 (tickets T-11660 and T-11680, dropped when
TT went back to its owner, issue CosmicScribe64/tokimemo-ps1-decomp#2). They ported them onto main after PRs #30-#32 as
branch `tt-wave1-handoff` and offered it to the TT owner. Take it over: verify, retag the code comments from the dropped
tickets to this one, record it in the wiki.

## Acceptance criteria

- [x] Branch verified: clean rebuild 27/27, `funcdiff --resolve` MATCH for all seven, prototype and global checks clean
- [x] Comments citing T-11660 / T-11680 retagged to T-11830
- [x] Wiki updated
- [x] Inline review against `CODING_STANDARDS.md`

## Results
TT: 245/303 -> 252/303 (60.6% -> 65.3%). Matched by wave 1 (commit b5ac7bab by the maintainer, kept as is):
`func_80135360` (872 bytes), `func_801356C8` (384), `func_80135B5C` (832), `func_8013B4A0` (552), `func_8013C0A8` (340),
`func_80138E28` (1044), `func_80139B98` (1192). Three of them replace guarded drafts from earlier TT batches
(`func_8013B4A0`, `func_80138E28`, `func_80139B98`). Five comment lines retagged (three files). The idioms are the
maintainer's; see the comments in `src/ovl/TT/80135360.c`, `801374C0.c`, `8013A3E0.c` and `8013BD10.c`.

## Review
Inline, against CODING_STANDARDS.md: the two fakematches (`pad[3]` frame locals, an empty test in `func_8013B4A0`)
carry FAKE with a ticket; no other changes besides the retag. No findings.

## Comments
