---
id: T-0201
title: Analyse O.BIN (symbol-bearing developer build) and harvest names
status: Done
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
- [x] Metadata and stats in [[obin]] (no source file names; 370 new high-confidence names plus 154 PsyQ names already known, 0 overlay, 274 unmapped).

## Notes

Not split with splat (not code at offset 0, never loaded). Only its checksum trailer is verified (`config/overlays/` has no entry for it). Do not commit the file or name dumps beyond the verified symbol entries.

## Comments

Code review (code review, Standards and Spec axes against CODING_STANDARDS.md) found: stale index status, an overstated "524" figure, dead code and an unused procedure-descriptor read in tools/obin_syms.py, unchecked read_sdk and bare open() in tools/obin_map.py, tuple-indexed data structures, an unreproducible OLH mapping, optimistic calibration, thin tests. All fixed in the follow-up commit: namedtuples, dead code removed, the OLH mapping marked manual in [[obin]], 24-function game-code spot check recorded (12 corroborated by callees, 0 refuted, 12 unverifiable), hypothesis wording in the generated files, 8 unit tests including `map_region`. Build: all 27 sha1 checks OK.
