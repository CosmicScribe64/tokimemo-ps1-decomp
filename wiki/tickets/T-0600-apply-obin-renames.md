---
id: T-0600
title: Apply the O.BIN rename list after the game.c split
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[obin]]", "[[tickets/T-0201-obin-format-and-symbols]]"]
---

## Goal

Apply `config/obin_renames.txt` (370 pairs, old func_/D_ name to O.BIN name, high confidence only) through the splat symbol files and sources after the `src/game.c` split merged. T-0201 deliberately did not touch `src/`, `include/` or the main symbol files.

## Acceptance criteria

- [ ] Renames done via `config/symbol_addrs*.txt` (CODING_STANDARDS section 4), `src/` updated to match.
- [ ] If addresses moved, `config/symbol_addrs_obin.txt` regenerated with `tools/obin_map.py --write`.
- [ ] All 27 sha1 checks OK.

## Notes

Name collisions with existing symbols are already filtered by the generator. Method and confidence: [[obin]].

## Comments
