---
id: T-27003
title: "Pilot: O.BIN names into the overlays (alias debt)"
status: Backlog
assignee:
created: 2026-10-24
updated: 2026-10-24
blocked-by: [T-27002]
links: [[[phase2]], [[obin]], [[tickets/T-0600-apply-obin-renames]], [[tickets/T-27000-phase-2-readable-codebase]]]
---

## Goal

Finish T-0600 for the overlays (audit gaps G2 and G3). The O.BIN names reach only the main executable today:
105 renamed main symbols are still used under their placeholder names in 186 overlay files (3,431 uses), and
`include/main_api.h` declares 56 of them under both names, 45 of the pairs with different types. This is the
first unit to run through `tools/rename.py` (T-27002) and proves the tool on real data.

## Acceptance criteria

- [ ] The overlay yamls load `config/symbol_addrs_obin.txt` (and the SDK names), so overlay asm and m2c output
      use the O.BIN names; the deliberate two-name address 0x80084C98 stays explained.
- [ ] The 105 names applied in the 186 overlay files through `rename.py`; no placeholder of a named main
      symbol left in `src/` or `include/` (`rename.py check`).
- [ ] The 56 dual declarations merged; each of the 45 with a type difference resolved by the build
      (`tools/sync_protos.py --fix` overrides where needed); `config/proto_known.txt` alias rows cleared.
- [ ] `cd_read_status` moved out of the shared symbol file (EVENT range, G3); `check_symbols.py` rejects
      shared-file names in EVENT's range.
- [ ] Clean build 27/27 sha1, `ninja progress` unchanged.

## Notes

Out of scope: the English comments on O.BIN names (each main module unit writes them for the names it owns)
and new names. Audit section 2.3 and risk 1 describe the alias views.

## Comments
