---
id: T-24089
title: Match DATE and EVENT movie sibling pairs (func_8014718C/func_8011D5C4, func_801462B0/func_8011C750)
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

In `src/ovl/DATE/801461B0.c` and `src/ovl/EVENT/8011C650.c`, two pairs of movie handler functions have high-similarity matched `DATE2` siblings (`DATE2:func_801348F0` and `DATE2:func_80133AF0` in `src/ovl/DATE2/80132DE0.c`) and near-miss drafts (`drafts/T-24028/` and `drafts/T-24018/`):

- Pair 1 (4,032 B total, ratio 0.929):
  - `DATE:func_8014718C` (1,996 B in `src/ovl/DATE/801461B0.c`, guarded at 11 differing instructions — only the tail timer narrowing `raw/masked` range split differs)
  - `EVENT:func_8011D5C4` (2,036 B in `src/ovl/EVENT/8011C650.c`, 7 differing instructions in `drafts/T-24028/` — same tail timer narrowing range split)
- Pair 2 (7,504 B total, ratio 0.950; matched sibling `DATE2:func_80133AF0` at ratio 0.967 / 0.943):
  - `DATE:func_801462B0` (3,804 B in `src/ovl/DATE/801461B0.c`)
  - `EVENT:func_8011C750` (3,700 B in `src/ovl/EVENT/8011C650.c`, draft in `drafts/T-24018/`)

## Acceptance criteria

- [ ] Close `DATE:func_8014718C` (1,996 B) and `EVENT:func_8011D5C4` (2,036 B) to `score=0`, and investigate/close `DATE:func_801462B0` and `EVENT:func_8011C750` in `build/scratch/t24089/`
- [ ] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- Subagent scratch workspace: `build/scratch/t24089/`
