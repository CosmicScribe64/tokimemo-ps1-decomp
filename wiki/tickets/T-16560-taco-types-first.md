---
id: T-16560
title: "Types-first pilot: TACO"
status: In Review
assignee: claude (pt-taco)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[taco-family-results]]", "[[data-types]]"]
---

## Goal

Recover the TACO overlay's shared data types (structs, arrays, stride tables, bit-field views, callback tables) in
`include/ovl/TACO.h` before function matching, then redraft every unmatched TACO function with the new headers and
measure how the drafts change. Pilot of the types-first approach from [[speedup-research-2026-10-16]] (in
BUNKAKEN/BUNKASAI fresh m2c drafts matched 67% of the time once shared types were in headers, against 1.6% elsewhere).

## Acceptance criteria

- [x] Survey: type_recovery, siblings, census drafts, wiki notes for TACO
- [x] Types declared in `include/ovl/TACO.h`, globals given aggregate types; every matched function still matches
- [x] Fresh draft for every unmatched TACO function under `drafts/T-16560/TACO/<func>/`, scored like the census
- [x] Four file-disjoint work packages (T-16561..T-16564) for the matching stage
- [x] Family page section "Types-first pass (T-16560)"; clean build 27/27, header, globals and prototype checks OK

## Notes

Worktree `pt-taco` (branch `pt-taco`, from `w2-good`). Owns `src/ovl/TACO/` and `include/ovl/TACO.h`; additions only
in `include/game.h` and `include/main_api.h`. New drafts only under `drafts/T-16560/`.

## Comments

### 2026-10-16 (claude, pt-taco): pass finished, In Review

Types (`include/ovl/TACO.h`, `include/main_api.h` additions only): `TcActor` +0x04 `Tc10 unk4` (GsDOBJ2
layout) and +0x14 `TcObj50 unk14` (GsCOORDINATE2 layout, `TcObj50` now complete), first bytes `unk0[2]`;
`Tc14` holes filled; `D_8015EC70` is `Tc14[16]`; `extern MATRIX D_80129C40;` in `main_api.h`, which now
includes `libgte.h`; prototypes of `func_8015AB18`/`func_8015AB80`. Matched code updated (`pad0[` to
`unk0[`, one `pad4`, one `padE[3]`, `&D_8015EC70`, `o->pad0[4]`), TACO sha1 OK after each family.

Redraft (`drafts/T-16560/`, driver `redraft.py`): 126 functions, plain (headers only) and typed (rewrites
pointer-step, actor-offset, matrix-copy, tcpos-args, field-symbols), then `draft_fixes`. Over the 116
unguarded functions: 103 compile (census 116, 13 of them manual repairs), 48 better than the census, 53
same, 2 worse; median 166 to 150 rows, sum 21,752 to 19,983. Headers alone changed 7 of 98 scores. No
draft scored 0.

Matches (5 functions, 2,032 bytes; each a small hand edit of the new draft): `func_8013822C`,
`func_8013555C`, `func_80135638`, `func_8015A948`, `func_80159260` (1,048 bytes, tcpos-args draft plus a
FAKE unused `TcPos` for the frame).

Packages for the matching stage (lists in the orchestrator scratchpad, `pt-taco-p1-list.txt`..`p4`):
T-16561 `pt-taco-p1` (801335A0.c, 80137590.c, 80159090.c, 80146D60.c; 9 functions), T-16562 `pt-taco-p2`
(80154230.c, 801430B0.c, 80133870.c; 10), T-16563 `pt-taco-p3` (80151C30.c, 8015ABF0.c; 10), T-16564
`pt-taco-p4` (80154960.c, 80137A60.c, 80138890.c; 8). About 170 minutes each.

Review notes: `TcObj50`/`Tc10` are the SDK's GsCOORDINATE2/GsDOBJ2 under TACO names (existing types,
completed here); a shared `libgs.h` with the SDK names would be the CODING_STANDARDS 3 fix, outside this
ticket's files. Follow-up worth a ticket: m2c's byte-step pointer arithmetic (`pointer-step`) as a
`tools/draft_fixes.py` detector for every unit.
