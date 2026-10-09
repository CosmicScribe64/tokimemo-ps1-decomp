---
id: T-0201
title: Analyse O.BIN (symbol-bearing developer build) and harvest names
status: In Review
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overlays]]", "[[tickets/T-0008-overlay-load-address-and-split]]"]
---

## Goal

`CDROM/EXEDIR/O.BIN` is not loaded by the game, but it carries a section table (.text/.rdata/.data/.comment, load address 0x80132000) and about 2100 plain-text symbol names (`warm_reset_init`, `system_init`, `olh_main`, `olh_init`, `menu0`..., `move_menu_title`). Work out its format, relate it to OLH.EXN and the main exe, and turn the names into `config/symbol_addrs.txt` entries where addresses can be proven.

## Acceptance criteria

- [x] File format documented in [[obin]] (ECOFF: header, section table at 0x4C, mdebug header at 0x5C0, external symbols at 0x7700).
- [x] `tools/obin_syms.py` parses O.BIN at run time; `tools/obin_map.py` maps the names onto SLPM_86.053 with stated confidence per method.
- [x] High-confidence mappings written to `config/symbol_addrs_obin.txt` and `config/obin_renames.txt` (not applied; follow-up [[tickets/T-0600-apply-obin-renames]]).
- [x] Metadata and stats in [[obin]] (no source file names; 524 high-confidence main-exe names, 0 overlay, 274 unmapped).

## Notes

Not split with splat (not code at offset 0, never loaded). Only its checksum trailer is verified (`config/overlays/` has no entry for it). Do not commit the file or name dumps beyond the verified symbol entries.

## Comments
