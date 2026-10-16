---
id: T-11860
title: TT batch 12: the TT remainder, one long Opus run per file group (two at a time)
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-11850-tt-batch-11]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Twelfth TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11850 (PR #35): every remaining TT
function except `func_8014D260` (gcc, needs maspsx; see [[gcc-objects]]). Same method as T-11850: one Opus agent per
file group (about 90 minutes), two at a time in separate worktrees, never two on the same C file; each finished branch
is merged into `tt-batch12` before the next agent starts, and each new agent gets the earlier agents' findings as tips.
Groups, in order:
1. Small singles: `func_8013BE9C` (`8013BD10.c`), `func_8013D2E0` (`8013C6D0.c`), `func_801487D0` (`801487D0.c`)
2. `80142570.c`: `func_80142570`, `func_80142AF0`, `func_801427A8`
3. `80136830.c`: `func_80136AD8`, `func_80136830`
4. Singles: `func_8013E498` (`8013E290.c`), `func_8013A3E0` (`8013A3E0.c`), `func_80142110` (`80142110.c`)
5. `80145E60.c`: `func_80146000`, `func_80146924`
6. `8013F040.c`: `func_80140D84`, `func_8013F040`
7. `801374C0.c` part 1: `func_801374C0`, `func_80137A78`, `func_80137654`
8. Singles: `func_80146E10` (`80146BE0.c`), `func_801410A0` (`801410A0.c`)
9. `80148DB0.c`: `func_801494D8`, `func_801498C8`, then `func_8014A2B4`
10. `801374C0.c` part 2: `func_80138808`, `func_801392A0`
11. Near misses: `func_8013A0DC` (`8013A040.c`), `func_80147A18` (`801478D0.c`), `func_801455C8` (`80144D70.c`)

## Acceptance criteria

- [ ] All eleven groups run; matches committed one per commit, near misses guarded with `T-11860`
- [ ] Inline review against `CODING_STANDARDS.md`
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
