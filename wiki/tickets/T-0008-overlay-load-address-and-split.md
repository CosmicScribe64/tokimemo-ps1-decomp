---
id: T-0008
title: Determine overlay load address and split overlays
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]"]
---

## Goal

Umbrella ticket: find where the 26 `CDROM/EXEDIR/*.EXN` overlays are loaded and split them with splat. See [[overlays]].

## Acceptance criteria

- [x] Load address and slot size confirmed from the main exe loader. The exe has no file names; it reads by sector (`func_80078C48`, `func_8007959C`) into 0x80132000, 0x30000 bytes, and enters through `D_80123110`. 24 overlays confirmed at 0x80132000 by loader and `jal` evidence; EVENT (0x800F6000) and GYOZI (0x80134000) by `jal` evidence only, loader not found: [[tickets/T-0200-event-gyozi-loader-and-address]]. Details in [[overlays]].
- [x] Per-overlay splat configs and build rules; each overlay rebuilds byte-identical (26 of 26, sha1 per overlay in `config/overlays/*.sha1`, checked by `ninja`, clean rebuild from scratch verified).
- [x] Symbols shared with the main exe through `config/symbol_addrs.txt`/`reloc_addrs.txt` and per-overlay `*_undefined_funcs_auto.txt`. The 131 main-exe undefined symbols lie in the shared slot, which holds different code per overlay, so they stay auto-defined (see [[overlays]]).
- [x] O.BIN identified: a developer build with an embedded symbol table, never loaded; follow-up [[tickets/T-0201-obin-format-and-symbols]].

## Notes

Result: load 0x80132000 (EVENT and GYOZI inferred elsewhere), overlays split into IDO C (`src/ovl/<NAME>.c`, all `INCLUDE_ASM`) plus a rodata asm blob; no overlay needed a child ticket for matching. Also fixed a main-build trap: splat rewrote `include/include_asm.h` on every run (`generate_asm_macros_files: False` now in all configs). Tools: `tools/gen_overlay_configs.py`.

## Comments

2026-10-09 code-review (Standards + Spec, since 159adc9). Resolved: generator import moved to top; index status fixed; wiki now warns that shared symbol_addrs apply by address across overlays. Accepted: the generator has fixed paths (it is a one-shot project script); commit 6a84c1c bundles the generator, configs and the one-line main-config fix (documented in its body); the loader/entry decoding scripts were throwaway and are described in [[overlays]], not committed; EVENT and GYOZI entries unknown and O.BIN format deferred to T-0200 and T-0201; overlays not yet in objdiff.json. Commit trailer follows the task brief (Opus 5.5).
