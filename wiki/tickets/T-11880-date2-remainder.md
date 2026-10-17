---
id: T-11880
title: "DATE2: match the remaining nine functions (one long Opus run per group)"
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-18800-types-first-date2]]", "[[tickets/T-11870-tt-batch-13]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Outside-contributor claim CosmicScribe64/tokimemo-ps1-decomp#49: the nine remaining DATE2 functions (14,144 bytes),
with the method that finished TT (T-11840..T-11870): one Opus agent per group with a long budget, two at a time in
separate worktrees, never two on the same C file, earlier findings passed on as tips.
1. `80137D50.c`: `func_801388C0`, `func_801385CC`, `func_801395C8`
2. `80132DE0.c` part 1: `func_80136794`, `func_801330D4`, `func_80136860`, `func_801348F0`
3. `80132DE0.c` part 2 (after group 2): `func_801350A4`, `func_80133AF0`

## Acceptance criteria

- [ ] All groups run; matches committed one per commit, near misses guarded with `T-11880`
- [ ] Inline review against `CODING_STANDARDS.md` (FAKE comment on every fake trick, including every chain store)
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
