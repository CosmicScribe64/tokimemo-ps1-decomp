---
id: T-1200
title: Fix conflicting extern declarations after batch merges
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[decompile-workflow]]"]
---

## Goal

After union-merging batch branches, a clean build fails: IDO cfe reports "redeclaration of X" where one global is declared with different types in `include/game.h` and `include/ovl/*.h`, or twice in `game.h`. Public CI on main is red. Fix every conflict without changing any matched function, and add a guard.

## Acceptance criteria

- [x] Clean build (`rm -rf asm build`, configure, full `ninja` without `-k`) passes; all 27 sha1 OK.
- [x] `ninja progress` still reports 663/6962 functions.
- [x] One type per symbol inside every header closure; exact duplicates removed.
- [x] `tools/check_headers.py` with tests, run as part of `ninja`.
- [x] CODING_STANDARDS.md notes where shared externs belong.
- [x] CI run on the pushed commit succeeds.

## Notes

Per-symbol decisions: [[matching-notes]] (section Conflicting extern declarations). Guard: `tools/check_headers.py` (closure-based, so overlays may still type their own overlay-local addresses differently), tests `tools/test_check_headers.py` (12), ninja target `headers`, CI step. Coding rule: CODING_STANDARDS.md section 8a.
Lesson: an implicit declaration is also a view; `func_80083440` and `func_80046318` needed separate main and overlay declarations (a `u8` prototype adds `andi`), found only by the overlay sha1.

## Comments

- 2026-10-09 inline code review against CODING_STANDARDS.md: C89 and `/* */` only, no function body changed except two call-site spellings (`&` / cast) with identical codegen, standards section 8a and checklist line added, wiki updated. No open findings. Clean build (`rm -rf asm build`, configure, `ninja` without `-k`): 27 of 27 sha1 OK, `ninja progress` grand total 663/6962.
