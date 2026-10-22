---
id: T-25052
title: Match MAIN remaining schedule dispatcher
status: In Progress
assignee: Astra counter worker
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25040-main-flag-dispatch]]", "[[tickets/T-24024-main-dispatcher]]"]
---

## Goal

Match only `func_8007132C` in `src/main/8006CB30.c` (3992 bytes), preserving
the accepted T-25036 and T-25040 functions in the same translation unit.
Continue in the existing isolated checkout; coordinator owns integration.

## Acceptance criteria

- [ ] Literal guarded/default C match and complete affected MAIN checksum.
- [ ] Header/global/prototype checks and independent standards/spec review.
- [ ] Defined source-only controls, marked FAKE where needed; no extra runtime operations.

## Notes

Parent assigned this function exclusively after accepting T-25040 with all27
hashes and pushing4933de332. Latest reported T-24024 evidence is984 candidate
versus998 target words, aligned cost892, with broad reload differences.
Read and reproduce its exact source before choosing a new compiler boundary;
do not repeat old source grids or broaden to other functions.

## Comments

- 2026-10-22: claimed before source edits. Existing matched functions and their evidence are committed; production7132C remains assembly.

### First verified prefix and causal negatives

Whole-TU reproduction matches T-24024's measurements. Calendar byte views plus
reusing `op` for the first count and `t` for the later count recover the72-byte
frame, spill0x30 and first101 instructions (except exit distances);984/998words,
cost860. Index byte views lower the aggregate score but introduce wrong pointer
induction; explicit GameState record pointers introduce wrong reloads. Neither
is adopted. Evidence and named negatives are retained in `drafts/T-25052/README.md`.

### Verified compiler options

Actual `trace/draft.B` records show zcopy on for the prefix checkpoint and for
the accepted D138 sibling. Default-only and one-case empty entry switches do
not create a VREG selector temporary. Two cases do; a C89 `register s32 t`
assigned an unsigned global before the first call also triggers the existing
uniform `OPTN 405 0` rule, with the dead assignment emitting no runtime work.
That control measures 985 words / cost 975 and does not yet improve the target.
`options.py` inspects the actual records; the earlier zcopy-off inference is
corrected in both draft READMEs. Historical logs remain append-only.
