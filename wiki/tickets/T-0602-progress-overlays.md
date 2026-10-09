---
id: T-0602
title: Extend progress reporting to the 26 overlays
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[build-system]]", "[[overlays]]", "[[tickets/T-0009-progress-report-script]]"]
---

## Goal

`tools/progress.py` and `ninja progress` only counted the main exe. Add per-overlay rows (`src/ovl/*.c` against the overlay asm), an overlays subtotal, the main-game subtotal and a grand total, and report the SDK library bytes as a separate asm-only line that is not counted in any denominator.

## Acceptance criteria

- [x] Overlay configs keep asm for decompiled functions (`disassemble_all`) so the denominator is complete; all 27 sha1 checks still OK.
- [x] `ninja progress` prints main rows, main subtotal, 26 overlay rows, overlays subtotal, grand total, and the SDK line marked as not counted.
- [x] Unit tests for the new aggregation (synthetic input); wiki and README progress docs updated.

## Notes

Main-game progress stays 75/834 functions.

## Comments

Result: `ninja progress` after a clean rebuild (27 of 27 sha1 OK): main game 75/834 functions, 2432/284428 bytes; overlays 8/6128, 64/1994940; grand total 83/6962, 2496/2279368; SDK libs 722 functions, 165212 bytes (asm only, not counted). `disassemble_all: True` added to `tools/gen_overlay_configs.py` and the 26 `config/overlays/*.yaml` (patched in place, same text as the generator emits). New `tools/test_progress.py` (4 tests, synthetic input) passes. Several overlay C bodies sit under `NON_MATCHING` guards, so they count as asm there (the INCLUDE_ASM in the `#else` branch is the build).

## Review (code-review, inline against CODING_STANDARDS.md)
Walked the checklist: scripts are Python 3 run through `tools/docker.sh`, take `--root`, print short output, exit non-zero on error, have docstrings; no generated asm or game data staged; tests use synthetic input; no C changes; wiki ([[build-system]]), README, kanban, index and log updated. Finding: the progress denominator for overlays needed `disassemble_all` or C-defined functions would have no size; fixed. No open findings.
