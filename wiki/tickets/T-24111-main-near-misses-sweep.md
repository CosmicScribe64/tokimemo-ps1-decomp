---
id: T-24111
title: Close low-score near misses in main (80043510, 800451D0, 8004F870, 80053650, 80058D20, 8005A0B0, 80061710, 80079B10, 80085E30)
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24099
  - T-24104
---

## Context

From the live re-scoring sweep (`build/scratch/t24104/ranked_drafts.txt`), the following functions in `src/main/` have low instruction-diff scores (`score=15..57`) under current `HEAD` (`tools/ugen_pass.py` + `tools/cvt_pass.py` + `tools/trailing_pad.py`):

- `main:func_8005CC40` (`score=15` in `src/main/8005A0B0.c`, `drafts/T-20900/main/func_8005CC40/body.c`)
- `main:func_800590CC` (`score=16` in `src/main/80058D20.c`, `src_guarded`)
- `main:func_80062210` (`score=16` in `src/main/80061710.c`, `src_guarded`)
- `main:func_800460FC` (`score=17` in `src/main/800451D0.c`, `src_guarded`)
- `main:menu_bar_color` (`score=17` in `src/main/8004F870.c`, `src_guarded`)
- `main:func_8007B144` (`score=21` in `src/main/80079B10.c`, `src_guarded`)
- `main:func_8008647C` (`score=22` in `src/main/80085E30.c`, `src_guarded`)
- `main:csr_load_vram` (`score=24` in `src/main/80043510.c`, `src_guarded`)
- `main:x_taku_menu_set` (`score=24` in `src/main/8004F870.c`, `src_guarded`)
- `main:menu_check_1` (`score=27` in `src/main/8004F870.c`, `src_guarded`)
- `main:func_800591D8` (`score=32` in `src/main/80058D20.c`, `src_guarded`)
- `main:func_80079C70` (`score=37` in `src/main/80079B10.c`, `drafts/T-24062/func_80079C70.c`)
- `main:func_8005A410` (`score=41` in `src/main/8005A0B0.c`, `drafts/T-24015/main/func_8005A410/body.c`)
- `main:func_80054140` (`score=42` in `src/main/80053650.c`, `drafts/T-20800/main/func_80054140/body.c`)
- `main:func_8007B64C` (`score=51` in `src/main/80079B10.c`, `src_guarded`)
- `main:func_8005CA94` (`score=57` in `src/main/8005A0B0.c`, `drafts/T-24015/main/func_8005CA94/body.c`)

## Acceptance criteria

- [ ] Attack and close as many of these `main` near misses as possible to `score=0` in `build/scratch/t24111/`
- [ ] Promote all `score=0` matches to unguarded default C and verify `tools/funcdiff.py --resolve`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md`
