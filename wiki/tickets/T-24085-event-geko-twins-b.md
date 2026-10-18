---
id: T-24085
title: Match EVENT and GEKO twin cluster B (func_80105DF8/func_8013E950, func_80109404/func_801406CC)
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following the completion of [[tickets/T-24081-event-geko-twins-a]] (4 functions, 2,632 B matched across `EVENT` and `GEKO`), target the next two high-similarity twin pairs between `EVENT` and `GEKO`:

- Pair 1 (1,712 B total, ratio 1.000; matched sibling `EVENT:func_800FA25C` / `GEKO:func_80139E1C` at ratio 0.798):
  - `EVENT:func_80105DF8` (856 B in `src/ovl/EVENT/801050D0.c`)
  - `GEKO:func_8013E950` (856 B in `src/ovl/GEKO/8013DF70.c`)
- Pair 2 (1,548 B total, ratio 0.992):
  - `EVENT:func_80109404` (768 B in `src/ovl/EVENT/80107FC0.c`)
  - `GEKO:func_801406CC` (780 B in `src/ovl/GEKO/8013F360.c`)

## Acceptance criteria

- [x] Match `EVENT:func_80105DF8` (856 B) and `GEKO:func_8013E950` (856 B) to `score=0`
- [x] Diagnose `EVENT:func_80109404` and `GEKO:func_801406CC` (`T-19001`/`T-22014` `ugen` block-copy state `F` vs `B` gap)
- [x] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- Matched 2 of 4 functions (1,712 B total):
  - `GEKO:func_8013E950` (856 B in `src/ovl/GEKO/8013DF70.c`) and `EVENT:func_80105DF8` (856 B in `src/ovl/EVENT/801050D0.c`): signed byte/halfword views on `unk_0E`/`unk_0A` share constants `8` (`$s3`) and `0x18` (`$s5`) with `unk_08`, and same-line store grouping (`FAKE`) places `mfhi` after the zero stores (`score=0`).
- `GEKO:func_801406CC` (780 B) and `EVENT:func_80109404` (768 B) match byte-for-byte up to the `((DisplayActor44 *)D_80120650)[2] = ((DisplayActor44 *)D_80120650)[0];` struct copy, where IDO 5.3 resets `ugen`'s circular FIFO queue to State B (popping 5 registers) instead of the original compiler's State F (popping 4 registers, `T-19001`/`T-20710`). Kept as `INCLUDE_ASM`; clean drafts in `build/scratch/t24085/`.
