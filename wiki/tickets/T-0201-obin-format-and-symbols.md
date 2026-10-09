---
id: T-0201
title: Analyse O.BIN (symbol-bearing developer build) and harvest names
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overlays]]", "[[tickets/T-0008-overlay-load-address-and-split]]"]
---

## Goal

`CDROM/EXEDIR/O.BIN` is not loaded by the game, but it carries a section table (.text/.rdata/.data/.comment, load address 0x80132000) and about 2100 plain-text symbol names (`warm_reset_init`, `system_init`, `olh_main`, `olh_init`, `menu0`..., `move_menu_title`). Work out its format, relate it to OLH.EXN and the main exe, and turn the names into `config/symbol_addrs.txt` entries where addresses can be proven.

## Acceptance criteria

- [ ] File format documented in [[overlays]] (header at 0x0, section table at 0x4C, symbol table around 0x700-0x7700).
- [ ] Names that map to main-exe or OLH functions verified by code comparison, then added through the splat symbol files.

## Notes

Not split with splat (not code at offset 0, never loaded). Only its checksum trailer is verified (`config/overlays/` has no entry for it). Do not commit the file or name dumps beyond the verified symbol entries.

## Comments
