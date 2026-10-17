---
id: T-20930
title: "Targeted type pass: ENDING"
status: Done
assignee: agent (ty-ending)
created: 2026-10-17
updated: 2026-10-17
links: ["[[data-types]]", "[[build-step-gaps]]", "[[speedup-research-2026-10-16]]", "[[ending-family-results]]", "[[tickets/T-18300-types-first-ending]]"]
---

## Goal

A second, targeted type pass for ENDING, one of the least-matched overlays. The first types-first pass
(T-18300) left 13 unmatched functions far from a match. Find the shared structures the remaining functions
use (work structs, menu/option records, callback tables, stack tables, bit-fields) from the asm access
patterns and the matched siblings, declare them, move the matched code onto them (byte-identical), and
redraft the unmatched functions with `tools/redraft.py`, keeping the drafts in `drafts/T-20930/`.

Scope: the C files under `src/ovl/ENDING/` and `include/ovl/ENDING.h` (additions only in `include/game.h`).
Not `src/ovl/TT/*` (issue #2) and not main `dialogue_render` in `src/main/8007C030.c` (issue #45).

## Acceptance criteria

- [x] New ENDING types declared with offset and size comments, matched code uses them, 27/27 sha1 OK.
- [x] The unmatched ENDING functions redrafted, drafts and `result.json` (scope, score, census_score) in `drafts/T-20930/`.
- [x] Paired score changes (census or T-18300 draft vs new draft) reported here and in [[ending-family-results]].
- [x] Types documented in [[data-types]] and [[ending-family-results]].
- [x] Clean rebuild 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Start 2026-10-17 07:02 UTC. Worktree `ty-ending` (branch `ty-ending`, from `w2-good`).

Types (all in `include/ovl/ENDING.h`, details in [[ending-family-results]] "Targeted type pass" and [[data-types]]):
message scripts `D_8013C280[3][17]` and `D_8013CAC0[3][15]`, `EdScriptSet D_8013C34C`, `EdScene30`
(`D_8013C5F0`, `D_8013CBB0`), `EdScene44 D_8013C620[15]`, `EdPortraitTbl D_8013C4C0`, `D_8013C5A0[7]`, the
callback tables `EdFnTblN` and stack tables `EdWords4`/`EdShorts13` (moved from five C files), `u8 D_8013C364`,
`s32 func_80132B04`, `s32 func_80132420();` (K&R), ENDING's `set_kanji_string` override removed. 427 scalar
declarations gone, 449 matched uses rewritten (`drafts/T-20930/fold_ending.py`, scoped to ENDING).

Matches (27/27 sha1 after each): `func_80139B48` (476 B, `POLY_G4` port of main `_sprite_set_box_shade_tarao`,
keeps the sibling's FAKE note), `func_80132B04` (660 B), `func_80132420` (1,764 B, FAKE frame layout). 2,900
bytes.

Paired scores (census measure; before = `tools/redraft.py` on 172ab7ce, after = on b035184a):

| function | census | T-18300 | before | after | final |
| --- | --- | --- | --- | --- | --- |
| func_80132420 | 364 | 336 | 336 | 336 | matched |
| func_80132B04 | 82 | 82 | 82 | - | matched |
| func_80139B48 | 194 | 194 | 182 | - | matched |
| func_801330E4 | 129 | 129 | 129 | 129 | 114 (hand) |
| func_80134270 | 468 | 468 | 487 | 468 | 468 |
| func_80134B18 (guarded) | 24 | 24 | 24 | 24 | 24 |
| func_80134C2C | 56 | 52 | 46 | 46 | 40 (hand) |
| func_80135F4C | 815 | 813 | 795 | 795 | 795 |
| func_8013759C | 99 | 94 | 99 | 99 | 66 (hand) |
| func_80137BB4 | 100 | 58 | 62 | 62 | 58 (T-18300 hand) |
| func_80137E40 | 67 | 64 | 66 | 66 | 64 (T-18300 hand) |
| func_801387B0 | 209 | 156 | 174 | 174 | 156 (T-18300 hand) |
| func_8013A2E8 | 1667 | 1667 | none | none | 1667 |

Headers alone changed no score (the one 487 to 468 change is a `draft_fixes` search-order effect: the archived
T-18300 body names the folded scalars and no longer compiles). Hand drafts written around the types gave the
three matches and the three hand gains.

Not done: `tools/redraft.py`'s archived variant cannot compile old bodies that use the folded scalar names
(`D_8013C4FC` and so on); a symbol-alias map like `TYPE_ALIASES` would fix that (tools/, out of scope here).

## Comments

- 2026-10-17: In Progress to In Review. Three matches (2,900 bytes), types and drafts committed on `ty-ending`; clean
  rebuild 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` OK. The code review
  against `CODING_STANDARDS.md` is open (two FAKE notes to check: `func_80139B48`, the sibling's masks;
  `func_80132420`, the frame layout).
- 2026-10-17: Code review against `CODING_STANDARDS.md`, In Review to Done. No findings. Checked: no edits under `src/ovl/TT/` and none to main `dialogue_render` (it is only called; `src/main/8007C030.c` and `include/main_api.h` untouched); every header change keeps one declaration per symbol (`check_headers` OK; the ENDING `set_kanji_string` override is gone and the main_api.h view is used); matched code changed only to the new aggregates and typed views (the one other change is dropping `(u8)` on `D_8013C364`, now `u8`); the three new matches are plain C (K&R definition of `func_80132420`, `POLY_G4` port, nested ifs); both new FAKE comments (`func_80132420` frame layout, `func_80139B48` colour masks) name T-20930 and the reason; types documented in [[data-types]] and [[ending-family-results]]; C89 only (no `//`, declarations at block top), comments ASCII; no game data or binaries (drafts hold source and scores, register names only inside NON_MATCHING comments, same layout as `drafts/T-18300/`); commits start with the ticket id and carry the trailer. `w2-good` merged into the branch first: only `wiki/index.md`, `wiki/kanban.md` and `wiki/log.md` conflicted (index and log keep both sides; the kanban board is `w2-good`'s, with this card re-added under Done). `check_headers`, `migrate_globals --check` and `sync_protos --check-branch` OK; the full clean rebuild (27/27 sha1) runs in the merge step.
