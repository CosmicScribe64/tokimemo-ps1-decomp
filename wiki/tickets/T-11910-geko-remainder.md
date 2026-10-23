---
id: T-11910
title: "GEKO remainder: func_80133C50, func_80142C9C, func_80133024, func_80132B24 (claim #65)"
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-23
updated: 2026-10-23
links: ["[[tickets/T-11900-geko-option-closers]]", "[[playbook]]", "[[build-step-gaps]]"]
---

## Goal
Outside-contributor claim CosmicScribe64/tokimemo-ps1-decomp#65, second round after [[tickets/T-11900-geko-option-closers]]: the last four GEKO functions. The maintainer confirmed on #65 that they are off their lists and that `func_80133C50` (T-25002) is free.

1. Agent A: `func_80133C50` (`801338F0.c`, 836 B, 13 rows: ugen temporary numbering), then `func_80142C9C` (`80142760.c`, 3,656 B, checkpoint at about 40 rows in `drafts/T-11900`).
2. Agent B, after A: `func_80133024` (2 rows) and `func_80132B24` (about 28 rows), both in `801323E0.c`; `tools/trace_lab.py` first.

## Acceptance criteria

- [x] Agent A done; matches committed one per commit, near misses guarded with `T-11910` (under about 30 rows)
- [ ] Agent B done (timeboxed)
- [ ] Inline review against `CODING_STANDARDS.md` (FAKE comment on every fake trick)
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
Agent A (PR for `func_80133C50`; the maintainer said on #65 on 2026-10-23 that their agents now also work on these four, first match wins):

| Function | Result |
|---|---|
| `func_80133C50` | MATCH. The club read goes through a FAKE volatile view `D_800E6280.unk_0F4.vh` (new member of the `GsHalf` union in `include/main_api.h`), so uopt keeps it before the record load; this replaces the T-25002 `if (v) {}` |
| `func_80142C9C` | not matched; checkpoint `drafts/T-11900/GEKO/func_80142C9C/body.c` at 119 full-TU lines (from 162) |

Agent B (`func_80133024`, `func_80132B24`) is still running.
