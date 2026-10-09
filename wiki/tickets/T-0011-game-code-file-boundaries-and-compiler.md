---
id: T-0011
title: Compiler confirmation on first decompiled game functions
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]"]
---

## Goal

Confirm the compiler/assembler version and flags by byte-matching 5-10 small leaf functions of the `game` segment. Record matching idioms and the decompile workflow in the wiki.

## Acceptance criteria

- [x] 5-10 small `game` functions decompiled and byte-matching; full sha1 still OK.
- [x] Compiler confirmed, or alternatives tried with evidence (result: NOT confirmed; evidence in [[matching-notes]]; follow-up [[tickets/T-0013-identify-original-compiler-pipeline]]).
- [x] [[matching-notes]] and [[decompile-workflow]] written.

## Notes

Split: file boundaries and Shift-JIS strings moved to [[tickets/T-0012-game-file-boundaries-and-shift-jis]].
See [[toolchain]] and [[executable]].

See [[toolchain]] and [[executable]].

## Comments

- 2026-10-09: matched 10 getters (`func_8004480C`, `func_8004481C`, `func_800451D0`, `func_800460CC`, `func_800460DC`, `func_800460EC`, `func_80046274`, `func_8004901C`, `func_8004ADD4`, `func_8004EC14`) with gcc 2.7.2-psx `-O2 -G0 -mcpu=3000` + maspsx 2.79; `tools/funcdiff.py` reports MATCH for each and the full build ends `build/SLPM_86.053.bin: OK` (sha1 e823bd84...). 8 other small functions (setters, `x=y`, `x+=c`, `&sym`) do not match with any gcc in /opt/gcc; reverted to INCLUDE_ASM, evidence in [[matching-notes]].
- Review (code-review, medium) findings fixed: funcdiff branch-target and internal-label handling, progress checks, criteria and comments recorded, `include/game.h` width check, commits split by topic.
