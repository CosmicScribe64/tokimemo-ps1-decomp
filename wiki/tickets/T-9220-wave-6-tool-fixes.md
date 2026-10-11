---
id: T-9220
title: Wave-6 tool fixes
status: Done
assignee: r6-fixes
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[tickets/T-9030-wave-5-tool-fixes]]", "[[build-system]]"]
---

## Goal
Fix the tool bugs agents hit in wave 6: stale asm breaking funcdiff, data_island.py emitting a line for an interior label, sync_protos silent skips and no way to add a new main-exe global, queue.py listing a function with an unbuildable jump table, funcdiff noise on jump tables and GameState offsets.

## Acceptance criteria
- [x] Each fix has a unit test; all tools/test_*.py pass
- [x] Clean build 27/27 OK, headers OK, progress 4521/6958 or higher (unchanged by this ticket)

## Notes
Worktree r6-fixes (branch r6-fixes), not merged. No `configure.py` change was needed.

## Comments

### Result (2026-10-10)
1. Stale asm: `funcdiff.asm_functions` keeps one copy per function, the folder that matches the C file's state (matchings if the C file defines it, else nonmatchings; file age if the C file is unreadable) and funcdiff prints a note naming the ignored copies. `configure.py` untouched.
2. `data_island.py`: a label that code never names (only a data pointer) inside an array is "interior": no `INCLUDE_RODATA` line, a comment in the block and a message (TACO `D_8015F718`). Label alone does not prove a variable start, so the message says to add the line when it is one.
3. `sync_protos.py --write/--fix`: `SKIPPED name: ... Fix: move the typedef ...` for symbols whose type only the declaring header defines (was silent), `WARNING` for a global inside a `migrate_globals.txt` aggregate.
4. `sync_protos.py --add-global NAME TYPE` (`'s16[4]'`, `u8`); `D_800B66E0` and `D_800B66F4` moved from `src/main/800674B0.c` into `main_api.h`. `src/main/800737A0.c` only forward-declares its own functions (`week_day_main/exit`), nothing to move.
5. `queue.py` flag `O` (jump table in an orphan chunk per `config/objects/<UNIT>.txt`): blocks, P(match) 0. 22 functions flagged, incl. BUNKASAI `func_80158EB0`.
6. funcdiff: jump-table relocations (`jtbl_<addr>` against IDO's `.rodata`+off table, found by its `R_MIPS_32` relocs) compare as `jump-table`; a text DIFF that is resolved-equal and reads a table is reported `MATCH (relocations resolved automatically ...)`, the unit sha1 decides the contents.
7. funcdiff: every text DIFF is followed by `resolved:` either naming-only or `REAL DIFFERENCE` with the rows after relocation; relocation types other than HI/LO/26 keep their addend; %hi pairs with the %lo of the same symbol and addend.

Proof: all tools/test_*.py pass; clean build 27/27 OK, headers OK, globals OK, progress 4521/6958 (unchanged), `--check-branch` OK.

### Review (inline, CODING_STANDARDS.md)
Tool code only; no game data or asm committed; no per-function switches (rules apply uniformly); tests synthetic. No open findings.
