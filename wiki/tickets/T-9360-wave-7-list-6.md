---
id: T-9360
title: Wave 7: list 6
status: Done
assignee: w7-6
created: 2026-10-10
updated: 2026-10-10
links: []
---

## Goal
Match functions from wave-7 list 6 (35 files) per the wave-7 brief.

## Acceptance criteria

- [x] Matched functions build byte-identical; ninja 27/27 OK
- [x] Inline review against CODING_STANDARDS.md

## Notes
Matched (3): TAIIKU `func_80146B8C` (data island + `TkCnt`), main `icon_disp_switch` (local `GameState *g`), GEKO `func_8013A6D0` (`""` argument). 26 T-0018 rows added ([[data/t0018-cases]]). Patterns: [[matching-notes]], "Wave 7, list 6 (T-9360)".

## Comments

### Review (inline, against CODING_STANDARDS.md)
- No game data, asm or build output committed; `config/overlays/TAIIKU.yaml` changed only by `tools/data_island.py` (data subsegment cut).
- Headers: `TkCnt` appended to `include/ovl/TAIIKU.h`; no main-exe declaration added by hand (`sync_protos --check-branch` OK, `migrate_globals --check` OK).
- Comments are ASCII English; the one trick (`GameState *g`) has a comment; no FAKE locals were left in matched code.
- Findings: none open.
