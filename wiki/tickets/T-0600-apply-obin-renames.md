---
id: T-0600
title: Apply the O.BIN rename list after the game.c split
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[obin]]", "[[tickets/T-0201-obin-format-and-symbols]]"]
---

## Goal

Apply `config/obin_renames.txt` (370 pairs, old func_/D_ name to O.BIN name, high confidence only) through the splat symbol files and sources after the `src/game.c` split merged. T-0201 deliberately did not touch `src/`, `include/` or the main symbol files.

## Acceptance criteria

- [x] Renames done via `config/symbol_addrs*.txt` (CODING_STANDARDS section 4), `src/` updated to match.
- [x] If addresses moved, `config/symbol_addrs_obin.txt` regenerated with `tools/obin_map.py --write`.
- [x] All 27 sha1 checks OK.

## Notes

Name collisions with existing symbols are already filtered by the generator. Method and confidence: [[obin]].

## Comments

All 370 pairs applied, none skipped. Collision check (script, before applying): no duplicate new names, no C keywords, no invalid identifiers, no clash with `config/symbol_addrs_sdk.txt`, `include/`, `src/` identifiers or libc names.
Method: `config/symbol_addrs_obin.txt` added to `symbol_addrs_path` in `config/SLPM_86.053.yaml` and to the `split` inputs in `configure.py`; a word-boundary script rewrote `src/main/*.c`, `include/game.h` and old-name mentions in wiki pages (not `wiki/log.md` or `wiki/obin.md`, which keep the old names); the inverse mapping reproduces the previous sources exactly. `size:` was dropped from code lines of the symbol file because it added 12 bytes (trailing nops) to two functions' progress sizes. Convention for provisional names added to CODING_STANDARDS section 4; hypothesis status (12 of 24 spot checks corroborated, 0 refuted) in [[obin]].
Verification: `rm -rf asm build; configure; ninja` gives 27 of 27 sha1 OK; `ninja progress` 75/834 functions, 2432/284428 bytes (unchanged).

## Review (code-review, inline against CODING_STANDARDS.md)
Checklist walked: sha1 verified and recorded; no NON_MATCHING change; no C changes beyond identifiers (round-trip proven); renames via splat symbol file; asm not hand-edited, INCLUDE_ASM names follow splat's file names; no data staged; Python only in Docker; wiki, kanban, log updated. Findings: (1) generator header said "Not applied" and "do not edit by hand" - fixed; (2) `--write` would now emit an empty list - documented in [[obin]]. No open findings.
