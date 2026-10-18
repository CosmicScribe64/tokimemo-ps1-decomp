---
id: T-24093
title: Reclassify hand-written LIBGTE RotMatrixX/Y/Z overlay objects to asm subsegments
status: Done
priority: high
type: config
created: 2026-10-18
updated: 2026-10-18
---

## Summary

Reclassify the six trailing hand-written Sony `LIBGTE.LIB` `RotMatrixX`/`RotMatrixY`/`RotMatrixZ` assembly objects in `DATE` (`0x8015A640`, `0x8015A7E0`, `0x8015A980`) and `TACO` (`0x8015CF30`, `0x8015D0D0`, `0x8015D270`) from `c` subsegments to `asm` subsegments (`libgte_8015A640`, `libgte_8015A7E0`, `libgte_8015A980`, `libgte_8015CF30`, `libgte_8015D0D0`, `libgte_8015D270`), matching how `config/SLPM_86.053.yaml` classifies the SDK `libgte` assembly objects.

- Update `configure.py` (`overlay_data_files`) to assemble `asm` subsegments in overlays (`asm/ovl/<NAME>/<sname>.s`).
- Update `tools/progress.py` and `tools/test_progress.py` to include `asm/ovl/*/*.s` in `SDK libs` totals rather than the C decompilation denominator.
- Remove the 6 placeholder `INCLUDE_ASM`-only `.c` files (`src/ovl/DATE/8015A640.c`, `8015A7E0.c`, `8015A980.c`, `src/ovl/TACO/8015CF30.c`, `8015D0D0.c`, `8015D270.c`).
- Update `[[gcc-objects]]` and close `[[tickets/T-24032-date-taco-rotation-clones]]`.
