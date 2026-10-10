---
id: T-4100
title: Wave 3: list 10
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Wave 3 batch agent 10: match as many of the 131 listed functions (26188 bytes) as possible in the 27 owned C files (main 8004F870, 80062CD0, 800737A0, 80075320; overlays BUNKASAI, BUNKA_SD, DATE, DATE2, ENDING, ETC, EVENT, GEKO, GYOZI, KANGEI, RPG_BAT, TACO, TAIIKU). Work list: scratchpad `wave3-list-10.txt`.

## Acceptance criteria

- [x] Work list processed in order, time-boxed per function (54 of 131 listed functions matched; 61 more from the same files).
- [x] Clean build (`rm -rf asm build; configure.py; ninja`): 27 of 27 OK, `build/headers.ok`.
- [x] T-0018 cases appended to [[data/t0018-cases]] (39 rows); new patterns in [[matching-notes]] ("Wave 3 list 10").
- [x] Inline code review against CODING_STANDARDS recorded below.

## Notes

Result: 115 functions (21472 bytes) turned into C in the 27 owned files; `ninja progress` grand total 2806 -> 2920 of 6958 (326492 of 2279368 bytes), clean build 27 of 27 OK. Method and patterns: [[matching-notes]], section "Wave 3 list 10 (T-4100)". Fakematches: 17 comments, all marked `FAKE` (single-symbol read-modify-write accesses and one empty `if` in main `func_80066ACC`).

## Comments

Inline review against CODING_STANDARDS (2026-10-10):
- Matches verified: every kept function was compared with `expected/` objects (scratch driver and `funcdiff.py` after `ninja <object>`), and the full `ninja` ends with all 27 sha1 checks OK; an earlier stale `funcdiff.py --built` MATCH (two main functions) was caught by the sha1 and fixed.
- No `NON_MATCHING` blocks added; unmatched functions stay `INCLUDE_ASM` (section 6).
- C89: variables at block top, `/* */` comments only, no `//`, no mixed declarations; local `typedef` tables as in the T-3330 proof.
- Fakematches: 17, each with a `FAKE` comment (reason, ticket); no unmarked tricks. `s32 idx;` before a table is the T-3330 idiom, not a fake.
- Types: fixed-width; the new function-pointer tables carry a size comment.
- Declarations (8a): new main-exe symbols went to `include/main_api.h` (no overrides added); overlay symbols and prototypes were appended to `include/ovl/<NAME>.h`; declarations whose function stayed `INCLUDE_ASM` were removed again; `check_headers.py` passes in every build.
- Strings are literals (Shift-JIS through `tools/cc.py`), no extern string symbols.
- Commits: small, ticket id, Co-Authored-By; no `asm/`, `build/`, `expected/`, `disc/`.
- Findings: none open. Tooling bug reported in [[matching-notes]]: `funcdiff.py --built` does not rebuild; `queue.py` over many files dies in Docker.
