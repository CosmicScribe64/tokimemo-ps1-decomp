---
id: T-7010
title: Game-state struct audit: base vs separate symbols
status: Done
assignee: agent (worktree r4-gsaudit)
created: 2026-10-10
updated: 2026-10-10
links: ["[[game-state]]", "[[data-types]]", "[[tickets/T-5100-game-state-struct]]", "tools/aggregate_audit.py", "tools/migrate_globals.py", "config/migrate_globals.txt"]
---

## Goal

T-5100 folded 166 globals of 0x800E6280..0x800E7D10 into `GameState D_800E6280`. Wave-4 lists 3 and 7 found functions where the original seemed to reach a field as a separate symbol (its own `lui`/`%lo` at the field address) and called that the top blocker. Classify every access of the original to the block (base-relative or symbol-direct, per field, function and object), decide how the original source was organised (one struct; several aggregates; struct plus per-unit externs or aliases), implement that model with per-unit views only where the evidence needs them, teach `tools/migrate_globals.py` and `config/migrate_globals.txt` which units may use which view, keep every matched function byte-identical, and retry the functions attributed to this blocker.

## Acceptance criteria

- [x] audit of every access to the block in the original asm (tool with unit tests), per field and per object
- [x] model chosen with its evidence; game-state declarations changed only as the evidence needs
- [x] `migrate_globals.py`/`config/migrate_globals.txt` state which units may use which view
- [x] clean build 27/27 OK, `headers OK`, `globals OK`, `sync_protos.py --check-branch` OK
- [x] attributed functions retried; result recorded
- [x] [[game-state]] and [[data-types]] updated

## Notes

Result (details: [[game-state]] "Views", [[matching-notes]] "Game-state views audit (T-7010)"):
- Model: (a)/(b)/(c) all rejected; the original had one object reached through `D_800E6280` in every unit. No restructuring, no per-unit header, no `keep` line.
- Evidence: IDO 5.3 compiles a constant-offset member exactly like a scalar at that address (same words, same registers); only as1's load-over-store hoisting and uopt's reload after an indexed store differ. In the original, 0 of 407 read-modify-write pairs inside the block are hoisted (27 of 981 between other globals), and the counters of `load_palette`/`load_csr_*` and `unk_F5F` in KANGEI `func_80135438` are reloaded after record stores (one-object behaviour). Base-relative accesses (ptr, ptridx, shared `%hi`) come from 129 of 277 objects in 21 of 24 units. 13 of 15 attributed functions give identical words with the scalar view; the other two prefer the member.
- Tool: `tools/aggregate_audit.py` (14 tests). `migrate_globals.py --check` accepts a `keep` line only with a hoisted pair on that address in the file's original object (3 new tests); `configure.py` orders the check after the split. `config/migrate_globals.txt` documents that no unit has a separate view.
- Matches (9): DATE `func_8015522C`, `func_8015745C` (`FAKE`, line scheduling), SHOUGATU `func_8014147C`, GEKO `func_8013E1A0`, VALEN `func_80133B6C`, ETC `func_80147D74`, KANGEI `func_80135438`, EVENT `func_8010242C`, GYOZI `func_801399C0`. SHOUGATU `func_8013E0F0` now matches through `GameState` (bit-field), its alias `D_800E66E8` is gone. Progress 3721 -> 3730 of 6958.
- Declarations added: `D_801230F4` (`main_api.h`), `D_801506F0`/`D_801506F4` (ETC.h), `D_8013453C` (VALEN.h), `D_8014723C` (GYOZI.h); KANGEI's local `KangeiFlagBits` gained `flag2`.

## Comments

2026-10-10 inline review against CODING_STANDARDS.md (r4-gsaudit): clean build (`rm -rf asm build`, configure, ninja) 27/27 sha1 OK, `headers OK`, `globals OK`; `sync_protos.py --check-branch` OK (0 new); all `tools/test_*.py` pass. New C is plain C89 (declarations first, `/* */` comments); bit-field views are local typedefs with a comment naming the bit, as in the neighbouring files (section 8: the output needs them); one `FAKE` (DATE `func_8015745C`, line scheduling) with reason and ticket. Main-exe symbols only in `main_api.h`, overlay symbols in their headers. Tools: Python 3, Docker-run, docstrings, unit tests on synthetic asm; `migrate_globals.py` change has tests; `configure.py` edit is one order-only dependency (file owned by nobody in this round). No game data, asm or build output staged. Finding fixed during review: matching-notes claimed bit-field matches for functions that were not compiled (reworded to "same shape"). No open findings.

