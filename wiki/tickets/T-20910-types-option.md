---
id: T-20910
title: "Targeted type pass: OPTION"
status: Done
assignee: claude-opus (ty-option)
created: 2026-10-17
updated: 2026-10-17
links: ["[[data-types]]", "[[build-step-gaps]]", "[[speedup-research-2026-10-16]]", "[[option-family-results]]"]
---

## Goal

A second, targeted type pass for OPTION, one of the least-matched overlays (32 functions unmatched after the
T-17800 types-first pass, 2 of them guarded). Find the shared structures the remaining functions use (work
structs, menu and option records, callback tables, stack tables, bit-fields) from the asm access patterns and
the matched siblings, declare them in `include/ovl/OPTION.h` (`include/game.h`: additions only), convert matched
OPTION code to them byte-identically, and redraft the unmatched functions with `tools/redraft.py`, saving the
drafts under `drafts/T-20910/` with `result.json` (`scope`, `score`, `census_score`). Report the paired score
changes. Scope: `src/ovl/OPTION/*.c`, `include/ovl/OPTION.h`, additions to `include/game.h`.

## Acceptance criteria

- [x] New OPTION types declared with offsets and sizes; matched code converted where it stays byte-identical.
- [x] Every matched function byte-identical (clean build 27/27); `check_headers`, `migrate_globals --check`,
      `sync_protos --check-branch` pass.
- [x] Unmatched OPTION functions redrafted into `drafts/T-20910/`; paired score changes recorded.
- [x] [[data-types]] and [[option-family-results]] document the types.
- [x] Code review against `CODING_STANDARDS.md` (before Done): no blocking findings, see Comments.

## Notes

- Not touched: `src/ovl/TT/*` (issue #2), main `dialogue_render` in `src/main/8007C030.c` (issue #45).
- Types (`include/ovl/OPTION.h`): `SpriteCell8`, `SpriteMap10`, `SpriteIndex80` (PsyQ GsCELL / GsMAP layouts) with
  `D_80122120[2]`, `D_80122140[2][64]`, `D_80122540[2]`, the tables of main's two BG slots at `D_801220D0`;
  `void func_80134210(void);`. `include/game.h` and `main_api.h` unchanged. Draft-side `SpriteSlot24` view of
  `D_801220D0` in `drafts/T-20910/redraft.py`.
- Measured and left: `D_8013D3B8` stays `volatile u16` (without it `func_80139A30` 225 to 219, but the guarded
  `func_80139E3C` 60 to 63 and its `typed` draft 225 to 370).
- Redraft (`drafts/T-20910/`): paired with `tools/redraft.py` on the tree before the pass, sum 5,184 to 3,504
  diff rows, median 156 to 97, 12 better, 20 same, 0 worse. Header types alone moved two variant scores; the
  gains are hand drafts in the menu-page template of the matched siblings plus `gs-local`.
- Matched: `func_80135E48` (948 bytes, commit e8a452e2). One FAKE: `s32 pad;` after `s32 i;` reproduces the
  0x88 frame and the row offsets (the same family as the T-4070 pad of the matched four-array pages; real source
  unknown). To revisit when the compiler gap behind the frame offsets is understood.
- Details: [[option-family-results]] ("Second type pass (T-20910)"), [[data-types]].

## Comments

- 2026-10-17 (claude-opus, ty-option): types, redraft and one match committed on `ty-option`; clean build
  27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass. In Review: the code
  review against `CODING_STANDARDS.md` is open. About two hours of agent time.
- 2026-10-17 (claude-sonnet, review): Code review against `CODING_STANDARDS.md`, In Review to Done. No blocking
  findings. Checked: no edits under `src/ovl/TT/` and none to main `dialogue_render` (`src/main/8007C030.c`,
  `include/main_api.h` and `include/game.h` untouched); the new `SpriteCell8`/`SpriteMap10`/`SpriteIndex80` and the
  three externs are declared once, in `include/ovl/OPTION.h` (section 8a: main-bss symbols only OPTION's C reads,
  through types of that header), and `void func_80134210(void)` agrees with its definition; matched code changed
  only by the one new match, `func_80135E48` replacing its `INCLUDE_ASM` (the matched users of `D_801220E0..` keep the
  `main_api.h` scalars, so no conversion was needed); the match is plain C89 (declarations at block top, `/* */`
  comments, `s32` as the header already says) and its single FAKE (`s32 pad;`) names the reason and T-20910, and
  was missing from the ticket notes, which is now added; offsets and sizes are documented in the struct comments;
  the types are in [[data-types]] and [[option-family-results]]; the 75 draft `.c`, 32 `.h`, 32 `.json` and 2
  `.jsonl` files under `drafts/T-20910/` hold no disc data or asm; ticket status and kanban card agree. One
  small fix: a stray blank line at the end of `wiki/data-types.md`. `w2-good` (T-20930 and the ticket cleanup)
  was merged into the branch first (`wiki/index.md`, `wiki/kanban.md`, `wiki/log.md` conflicted; index and log
  keep both sides, kanban is `w2-good`'s board plus this card). Clean rebuild 27/27 sha1 OK after the merge;
  `check_headers`, `migrate_globals --check` and `sync_protos --check-branch` results in the log.
