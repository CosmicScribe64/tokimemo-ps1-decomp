---
id: T-24064
title: Match three small ETC functions
status: Ready
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_80140AE4 (224 bytes, 8013FF80.c), func_8013F980 (252 bytes, 8013F980.c), and func_80141ABC (300 bytes, 80140F50.c). Primary main 1911789b2 still has all three INCLUDE_ASM; parent explicitly assigned this disjoint scope. Preserve other functions, including matched func_8013FC64.

## Acceptance criteria
- Reuse built worktree on codex/complete-etc-count-trio from frozen T-24057 acf7fa2c3; preserve ancestry.
- Read current wiki, ETC family and prior T-13103/T-17300 negatives before novel probes; prioritize siblings and evidence-backed types over broad sweeps.
- One GPT-6.1 Sol medium worker/compiler; Docker tools only, serial ninja -j1. Reserved DATE work remains frozen.
- Coordinate exact shared-header/config changes before editing. Actual wins remove INCLUDE_ASM/guard and pass explicit default linked funcdiff, ETC checksum and header/global/prototype guards.
- Pin useful source/reproducer or verified wins promptly. Parent owns independent review/integration/push; ticket stays In Review until reviewed.

## Notes
80140AE4: prior unsigned-compare constant sharing, one-higher ugen temporaries, selector/result/width/clamp controls and ordinary switches exhausted (48 baseline versus switch54/58 rows). 8013F980: pointer do-while shape recovered but constant 0x801FA808 folds versus original saved pointer; later hand draft still prologue scheduling. 80141ABC: real eight-byte unaligned album copy, byte-only destination record and word counter reached27 rows; saved-register order, copy temporary and argument scheduling remain after pointer-order/end-pointer sweeps.

## Comments
User deferred this ETC package while the sole worker focuses on DATE func_80144E20. Status is Ready, not Done; no ETC compiler probes or production edits began in this package. Current read-only notes are the existing 48-row album interaction, pointer-loop prologue lead and 27-row unaligned album-copy history linked in the ticket. Resume only after new coordination.

Parent reports main 1911789b2, 6,507 functions / 1,851,416 verified bytes (81.2%). This package claims none of those prior gains. No additional DATE compiler or trace investigation.
