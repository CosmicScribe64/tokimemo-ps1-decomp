---
id: T-0012
title: Game code file boundaries and Shift-JIS strings
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]", "[[tickets/T-0011-game-code-file-boundaries-and-compiler]]"]
---

## Goal

Split `src/game.c` into per-source-file segments and decide how to represent Shift-JIS strings. Split out of [[tickets/T-0011-game-code-file-boundaries-and-compiler]].

## Acceptance criteria

- [x] File boundaries found and one `c` segment per file added to `config/SLPM_86.053.yaml` (28 files, evidence in [[source-files]]; the inferred entry-point split was merged back as too weak). rodata/data/bss stay whole; per-file split deferred to [[tickets/T-0500-per-file-game-rodata-data-bss-split]].
- [x] Shift-JIS strings emitted readably without changing bytes (`tools/asm.py` re-encodes; see [[build-system]]).
- [x] 75 C functions still match. Evidence (final clean rebuild: `rm -rf asm build; configure; ninja`): 27 of 27 sha1 checks OK (main exe + 26 overlays), `ninja progress` total 75/834 functions, 2432/284428 bytes.
- [x] `configure.py`, `tools/progress.py`, `tools/list_leaves.py`, `tools/funcdiff.py`, `objdiff.json` work per file.

## Notes

See [[toolchain]], [[executable]], [[source-files]].

## Comments

### Review (code review, against CODING_STANDARDS.md, 2026-10-09)
Findings and resolutions:
- Spec: partial rodata split not done. Resolved by writing the concrete reasons into [[tickets/T-0500-per-file-game-rodata-data-bss-split]] (6 of 8 rodata starts coincide with a file boundary, most rodata has no code reference, game/SDK interleave, no build signal to catch a wrong cut).
- Spec: shared declarations. Checked: `src/main/*.c` contain no `extern` or `typedef`; all externs and types stay in `include/game.h`, `include/libgpu.h`, `include/libapi.h`, which each file includes as needed. No change needed.
- Spec: the 0x800420D0 boundary was only inferred. Merged back into `80041000` (28 files); decision recorded in [[source-files]].
- Docs: stale `src/game.c`/`game.o` references and the "implicit inputs" note rewritten in [[matching-notes]], [[psyq-sdk]], [[build-system]].
- Standards 7a: the `pad_text` pass in `tools/cc.py` got unit tests (`tools/test_cc.py`), a rule/evidence section in [[toolchain]] and [[matching-notes]], and was verified on all 26 overlays (27/27 sha1 OK).
- Smells: INCLUDE_ASM/definition/size scanning factored into `tools/srcscan.py`; `progress.py` uses a named `Totals` tuple and `file` instead of `seg`; unused `C_TOOLCHAIN_OVERRIDES` removed from `configure.py`.
All findings resolved.
