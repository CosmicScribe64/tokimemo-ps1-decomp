---
id: T-16553
title: "Types-first pilot package pt-taiiku-p3"
status: Done
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16550-taiiku-types-first]], [[taiiku-family-results]], [[matching-notes]], [[data/t0018-cases]]
---

## Goal

Match the TAIIKU functions of `src/ovl/TAIIKU/801452D0.c` (game C) from the T-16550 types-first redrafts, using and extending the types in `include/ovl/TAIIKU.h`. Work list (best first): `func_8014666C`, `func_80145640`, `func_80145C28`, `func_8014607C`, `func_80147318`, `func_80147928`, `func_80146A60`, `func_80147F58`, `func_80145370`. Measured pilot (stage B): start 2026-10-16 02:25 UTC.

## Acceptance criteria

- [x] Each matched function is plain C in `src/ovl/TAIIKU/801452D0.c`, TAIIKU sha1 OK, committed as `T-16553: match TAIIKU:func`. (10 functions, `func_80145640` in the closer pass)
- [x] Unmatched drafts under about 30 differing lines stay under `#ifdef NON_MATCHING` with a `NON_MATCHING: T-16553` comment (`func_8014607C` 20, `func_80145370` 22, `func_80146D18` 31); `func_80147B14` stays `INCLUDE_ASM`.
- [x] Compiler-gap rows in [[data/t0018-cases]] (four new rows and an update line for `func_80146A60`).
- [x] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.
- [x] Short T-16553 section in [[matching-notes]]; family page [[taiiku-family-results]] updated.
- [x] Code review against `CODING_STANDARDS.md` (required before Done). Done 2026-10-16, see Comments.

## Notes

Worktree `tokimemo-wt/pt-taiiku-p3`, branch `pt-taiiku-p3` from `w2-good`. Owned C file: `src/ovl/TAIIKU/801452D0.c` (plus `include/ovl/TAIIKU.h`). `tools/siblings.py` found no matched sibling above 0.70 for any function of the package, so the starting point was the redraft plus the asm.

## Results

Matched (10, 6,324 bytes): `func_8014666C` (480), `func_80145C28` (716), `func_80147318` (520), `func_80147928` (324), `func_80146A60` (300), `func_80147F58` (572) from the list; `func_80148370` (596), `func_80146338` (820), `func_80147594` (916) from the rest of the file; `func_80145640` (1,080) in the closer pass.

Guarded: `func_8014607C` (700 B, 20 lines: a load that IDO splits into a global range and a copy), `func_80145370` (720 B, 22 lines: constant and pointer swapped in two loops), `func_80146D18` (648 B, 31 lines: a loop store scheduled before the pointer advance, the tail index in the wrong register). `func_80147B14` (1,020 B) was not started.

Header changes (`include/ovl/TAIIKU.h`): tables `D_8014A4F8[12]`, `D_8014A528[9]`, `D_8014A4D4[8]`, `D_8014A4E8[6]`, `D_8014A56C`, `D_8014A570`, `D_8014A5A0`, `D_8014A668` typed, `D_8014A4F4` is now `s32` (word access), `TaiWords9` and `TaiWords12`, prototypes of the game-C helpers called before their definition. `src` declares `D_8014A4A8` (a splat name inside `D_8014A454[4]`, used as the loop end of `func_80147594`) and `D_8014A614`.

What carried over from the pilot: the typed records made the m2c byte offsets readable (record number plus field), which is what let the loops be rewritten as index loops; the two record-init functions and `func_80147594` then needed only `order_search`, an alias symbol and the choice of index variables. Details in [[matching-notes]], "T-16553".

## Comments

### Closer pass (Opus, 2026-10-16 04:12-05:00 UTC)

- `func_80145640` matched in plain C (1,080 bytes, commit "T-16553: match TAIIKU:func_80145640"). The call result goes into a declared local, `slot = search_load_tpage_buf_lock(D_8014A4F8[i], 1);`, declared before `i`, and the chain stores through it. With the call nested in the chain, uopt allocates three spill temporaries below the locals and the counter's home sits at sp+0x3C. With `slot` taking the top local slot, the counter's home is sp+0x38 and the pointer temporary stays at sp+0x30, as in the original. The matcher found that "a second local grows the frame" because they tried unused locals. A local that replaces a nested call result does not grow it.
- `func_8014607C` stays guarded at 20 lines. Both local copies in the tail are propagated by IDO into their uses after the `||`: `var_v0_2 = D_800E638E` into `>> 1` (hence the separate `$a1` range) and `temp_v1 = D_800E6375 & 0xF` into the two bit tests (hence `$t5` and not `$v1`). Evidence: with a then-arm that does not read `var_v0_2`, the `lh` goes straight into `$v0`. A volatile access between the definitions and the `if` stops the propagation (the mask moves to `$v1`) but makes ugen materialize the address. These did not help: `order_search` over the six tail statements (720 orders, best 20), `s16`/`u8`/`u16`/`u32` locals, a `u16` view with an `s16` local, labels and `goto` before the `if`, an `if`/`else` with the global read in both arms or through a `raw` local (uopt sinks the load into each arm), the assignment inside the condition, record-view and absolute-address stores, a bit-field view, dead tests, and `autotweak` (120 s, 753 variants, no gain).
- `autotweak` (120 s) on `func_80145640` before the match found nothing either. Ticket stays In Review until the code review.

### Code review (Opus, 2026-10-16)

Reviewed `git diff w2-good...pt-taiiku-p3` (`include/ovl/TAIIKU.h`, `src/ovl/TAIIKU/801452D0.c`, wiki pages) against the checklist in `CODING_STANDARDS.md`. Scope checks: only the owned C file changed, no `drafts/` or `asm/` change, no inline asm or `.word`, no `//` comments, no non-ASCII, no game data. The 10 matched functions are plain C and the default build is 27/27 OK (clean build in the merge). The three guarded drafts (`func_80145370`, `func_8014607C`, `func_80146D18`) sit under `#ifdef NON_MATCHING` with a `NON_MATCHING: T-16553` comment naming the lines left; `func_80147B14` stays `INCLUDE_ASM`. `check_headers.py`, `migrate_globals.py --check` and `sync_protos.py --check-branch` pass. No fakematch was added: the pointer-bound loops, the declaration order of locals and the one-line `k = 0; do {` are matching idioms with an explanation in `wiki/matching-notes.md` (T-16553).

Findings and fixes:
- Compiler warning 681 (`&` before an array ignored) on `unk_34 = (u8 *)&D_8014A528;` in `func_80148370`: dropped the `&` (identical code, build still OK).
- The next-table-symbol loop bounds of `func_80145640` and `func_80147594` had no comment (the other two functions did): added a trailing comment on the first loop of each (no line added, so no `.loc` shift).
- Stale counts after the closer pass in `wiki/index.md` (9 matched, 4 guarded), `wiki/matching-notes.md` (T-16553 header paragraph, the `func_80145640` bullets) and `wiki/taiiku-family-results.md`: corrected to 10 matched (6,324 bytes) and 3 guarded.

All findings resolved. Status Done.
