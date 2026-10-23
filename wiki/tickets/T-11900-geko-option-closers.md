---
id: T-11900
title: "GEKO and OPTION: match the remaining 14 functions (outside-contributor claim #65)"
status: Done
assignee: claude (diman8 fork)
created: 2026-10-20
updated: 2026-10-23
links: ["[[tickets/T-24140-geko-option-tel-omimai-wave-3-closers]]", "[[playbook]]", "[[build-step-gaps]]"]
---

## Goal
Outside-contributor claim CosmicScribe64/tokimemo-ps1-decomp#65: the 11 GEKO and 3 OPTION functions left at
`ff6c770b2` (14,152 bytes). Taken off [[tickets/T-24140-geko-option-tel-omimai-wave-3-closers]] for the claim; TEL and
OMIMAI stay with T-24140. Method as in [[tickets/T-11880-date2-remainder]]: one Opus agent per group with a long
budget, two at a time in separate worktrees, never two on the same C file.

1. GEKO `801323E0.c`: `func_80133024` (280), `func_80132B24` (480), `func_80133308` (656)
2. OPTION: `func_801326BC` (`80132000.c`, 516), `func_80139A30` (`801389A0.c`, 1036), `func_8013A7EC` (`8013A550.c`, 1092)
3. GEKO `80134D10.c`: `func_80136C20` (604), `func_801367EC` (832), `func_80135DC0` (2260)
4. GEKO `80142760.c`: `func_80144198` (216), `func_80142C9C` (3656)
5. GEKO `80134040.c`, `801338F0.c`, `80141AF0.c`: `func_80134808` (432), `func_80133C50` (836), `func_801420E4` (1256)

## Acceptance criteria

- [x] All groups run; matches committed one per commit, near misses guarded with `T-11900` (under about 30 rows)
- [x] Inline review against `CODING_STANDARDS.md` (FAKE comment on every fake trick)
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
4 matched (4,212 bytes) of the 8 functions still unmatched upstream when work resumed; upstream matched the other 6
claimed functions in the meantime (GEKO `func_80134808`, `func_80133308`, `func_80144198`, `func_801420E4`; OPTION
`func_80139A30`, `func_8013A7EC`; this branch kept upstream's versions, and the claim was narrowed on #65).

| Function | Result |
|---|---|
| OPTION `func_801326BC` | MATCH: palette stores through `GameState *g`, flat `((s32 *)0x801CBEF0)[k % 4 * 128 + i]` reads, FAKE empty `else return` |
| GEKO `func_80136C20` | MATCH: `D_80145094` (73-entry scene table) defined in C instead of `INCLUDE_RODATA` (T-25150 as1 rule); C unchanged from the T-24203 guard |
| GEKO `func_801367EC` | MATCH: the T-24922 draft, unchanged once the table is in C |
| GEKO `func_80135DC0` | MATCH: `GameState *gs`; FAKE call arguments through `D_800E6280.unk_*`; strcpy view `void *func_800AE0F0()` (as in RPG_BAT); FAKE frame pads |
| GEKO `func_80133024` | not matched; guard unchanged (2 rows) |
| GEKO `func_80132B24` | not matched; guard unchanged (about 28 rows) |
| GEKO `func_80142C9C` | not matched; checkpoint at about 40 rows in `drafts/T-11900/GEKO/func_80142C9C/body.c` |

`func_80133C50` was left to the maintainer (T-25002). Notes: [[matching-notes]] section "T-11900".
