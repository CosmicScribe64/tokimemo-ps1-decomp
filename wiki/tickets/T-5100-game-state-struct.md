---
id: T-5100
title: Recover the main game-state struct
status: Done
assignee: agent (worktree o-gamestate)
created: 2026-10-10
updated: 2026-10-10
links: ["[[game-state]]", "[[data-types]]", "[[tickets/T-5000-type-recovery-arrays-structs]]", "tools/migrate_globals.py", "config/migrate_globals.txt"]
---

## Goal

T-5000 found that the main-exe bss block around 0x800E6248..0x800E7400 behaves as one object that almost every overlay touches. Recover it as one struct: field offsets, types, sub-arrays and records from the original's access patterns, declared once in `include/main_api.h` with the base symbol the original used. Write `tools/migrate_globals.py` to rewrite every C use of an absorbed `D_` global into a field access (re-runnable for later waves) and a ninja check that rejects the old names. Apply it to the whole tree byte-identically, retry the load-hoist and unit-private functions that touch the block, and document the struct in [[game-state]].

## Acceptance criteria

- [x] `GameState` declared in `include/main_api.h`, base symbol from the original's relocations, offsets checked by the tool
- [x] `tools/migrate_globals.py` (apply, check, layout) with unit tests; ninja step `build/globals.ok`
- [x] all C uses migrated; clean build 27/27 sha1 OK, headers OK, `sync_protos.py --check-branch` OK
- [x] every use kept on an old view listed with its reason (`config/migrate_globals.txt`)
- [x] load-hoist and unit-private functions touching the block retried; result recorded
- [x] [[game-state]] page; [[data-types]] and [[decompile-workflow]] updated

## Notes

Result (details: [[game-state]]):
- Base and size: `GameState D_800E6280`, 0x800E6280..0x800E7D10 (0x1A90 bytes). The base is the address the original's strength-reduced loops keep in the base register; 0x800E6248..0x800E6280 is SDK bss and an artifact, and another object starts at 0x800E7D10. O.BIN `sys` (0x1A70) fits as a name (hypothesis, comment only).
- Layout: 161 top-level fields, records `GsRec01C`, `GsRec044`, `GsRec0FC`, `Rec38` (now with `GsWord` +0x0C/+0x10), `GsRec66C`, `GsRec69C`, `GsRec75E`, `GsRecF90`, `GsRec1328`, `GsRec142C`, unions `GsWord`/`GsHalf`; 88.8% of the bytes typed, 759 bytes unknown (1% of the accesses).
- Migration: 166 globals absorbed, 1396 uses plus 63 old byte views of `D_800E6280` rewritten, 185 declarations removed; byte-identical. Kept old view: SHOUGATU `func_8013E0F0` (`D_800E66E8`). Needed changes of the first layout: +0x1093/+0x1094 and the nine records at +0xFC as members, not arrays; byte views rewritten as field accesses.
- 7 `FAKE` first-symbol tricks removed. 47 load-order functions matched (191 tried, none matches with separate symbols); 1 U0-flagged function (main `func_80074D28`) matched with plain C, independent of the struct.
- Side fix: `sync_protos.py --write` deleted the type definitions of `main_api.h`; it keeps them now.
- Progress 3491 -> 3539 of 6958.

## Comments

2026-10-10 inline review against CODING_STANDARDS.md (o-gamestate): clean build (`rm -rf asm build`, configure, ninja) 27/27 sha1 OK, `headers OK`, `globals OK`, `sync_protos.py --check-branch` OK, all `tools/test_*.py` pass. No NON_MATCHING; new C is plain C89 from m2c (no `//`, declarations first), placeholder names only (`unk_XXX`, O.BIN `sys` only in a comment, CODING_STANDARDS 4). Struct offsets carry `/* 0xNN */` comments that `migrate_globals.py` checks against the computed layout. Main-exe declarations only in `include/main_api.h` (new globals of the matched functions added there; EVENT's own data at D_801206DC/D_801206EC kept through two explained overrides), overlay symbols in their headers. No new FAKE; 7 removed; the remaining casts (`*(u8 *)&D_800E6280.unk_163C[34]`, `*(s32 *)&D_800E6280.unk_69C[i]`) are the views the matched code had before, with their comment. The kept old symbol is listed with its reason. Tool: Python 3, Docker-run, docstring, 9 unit tests on synthetic input; sync_protos fix with a test. No game data, asm or build output staged. Findings fixed during review: blank-line leftovers from removed declarations (ENDING.h), comments still naming old symbols (DATE, OMIMAI, 80062CD0), overlay externs pasted from m2c moved to the headers. No open findings.
