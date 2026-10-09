---
id: T-1310
title: "Tooling: object-trailing padding"
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[toolchain]]", "[[matching-notes]]", "[[build-system]]", "[[tickets/T-1060-overlay-batch-j-bunkaken-bunkasai]]", "[[tickets/T-0700-overlay-batch-a]]", "[[tickets/T-0012-game-file-boundaries-and-shift-jis]]"]
---

## Goal
A function followed by trailing padding (one nop is the case asm-processor cannot express: it needs at least 2 instructions per INCLUDE_ASM block) must be portable to C by a uniform, documented mechanism (CODING_STANDARDS 7a), replacing the per-site `src/ovl/pad` stubs.

## Acceptance criteria
- [x] One rule for all objects (main + overlays), data-driven, no per-function switches: `tools/trailing_pad.py` (called by `tools/cc.py`).
- [x] Scan of all objects confirms coverage: 392 functions end in nop words after `endlabel` (30 main, 362 overlay); all expressible.
- [x] Unit tests on synthetic input: `tools/test_trailing_pad.py` (17 tests), `tools/test_cc.py`.
- [x] The 8 BUNKAKEN/BUNKASAI functions ported, plus OLH `func_801323CC`, OPTION `func_8013A4FC`, VALEN `func_80133C44`, TT `func_801320F0`.
- [x] Clean build (`rm -rf asm build; configure; ninja`), 27/27 sha1 OK.
- [x] Wiki updated: toolchain, matching-notes, build-system, CODING_STANDARDS 7a.
- [x] Inline review against CODING_STANDARDS.

## Notes
- Rule: for every function the C source defines, read the trailing nops after `endlabel` in its splat `.s` and insert them after the compiled function (ELF edit: symbols, REL offsets, section-symbol addends). 36 `INCLUDE_ASM("src/ovl/pad", ...)` stubs and their comments were deleted.
- Found on the way: `objcopy --update-section .text=` with a longer section drops all relocations of the object; `pad_text` (T-0012) now edits the ELF itself. The assembler's end-of-.text alignment fill is dropped before inserting pads.
- Still open: `func_80148764` (TT) reloads `p[5]` where uopt CSEs it.

## Comments
