---
id: T-0012
title: Game code file boundaries and Shift-JIS strings
status: In Review
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]", "[[tickets/T-0011-game-code-file-boundaries-and-compiler]]"]
---

## Goal

Split `src/game.c` into per-source-file segments and decide how to represent Shift-JIS strings. Split out of [[tickets/T-0011-game-code-file-boundaries-and-compiler]].

## Acceptance criteria

- [x] File boundaries found and one `c` segment per file added to `config/SLPM_86.053.yaml` (29 files, evidence in [[source-files]]). rodata/data/bss stay whole; per-file split deferred to [[tickets/T-0500-per-file-game-rodata-data-bss-split]].
- [x] Shift-JIS strings emitted readably without changing bytes (`tools/asm.py` re-encodes; see [[build-system]]).
- [x] 75 C functions still match, `ninja progress` 75/834, 27 sha1 checks OK after `rm -rf asm build; configure; ninja`.
- [x] `configure.py`, `tools/progress.py`, `tools/list_leaves.py`, `tools/funcdiff.py`, `objdiff.json` work per file.

## Notes

See [[toolchain]], [[executable]], [[source-files]].

## Comments
