---
id: T-16561
title: "Types-first pilot package pt-taco-p1"
status: In Review
assignee: claude (pt-taco-p1)
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-16560-taco-types-first]]", "[[taco-family-results]]"]
---

## Goal

Matching stage (pilot stage B) of the TACO types-first pilot: take the best redraft of each function in
`src/ovl/TACO/801335A0.c`, `80137590.c`, `80159090.c` and `80146D60.c` (9 functions, list in the orchestrator
scratchpad `pt-taco-p1-list.txt`) and match as many as possible in about 2.5 hours. Measured pilot.

## Acceptance criteria

- [x] Every matched function committed on its own ("T-16561: match UNIT:func"), unit sha1 OK (8 functions, 5,612 bytes)
- [x] Unmatched drafts under 30 differing lines kept under `#ifdef NON_MATCHING` with a T-16561 comment (`func_80133738` 8 lines, `func_801472D4` 20; `func_80147B24` at 63 lines is `INCLUDE_ASM` again)
- [x] Compiler-gap rows added to `wiki/data/t0018-cases.md`
- [x] T-16561 section in `wiki/matching-notes.md`
- [x] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch OK

## Notes

Worktree `pt-taco-p1` (branch `pt-taco-p1`, from `w2-good`). Owns the four C files above; `include/`, `config/`
and `wiki/` edits where needed. Started 2026-10-16 02:40 UTC.

## Comments

### 2026-10-16 (claude, pt-taco-p1): matching stage finished, In Review

Matched (unit sha1 OK after each, one commit each): `func_80147488` (492 bytes), `func_80159878` (1,768), `func_8013788C` (468), `func_8013760C` (640), `func_80147C98` (488), `func_80146F74` (604), `func_80147674` (644), `func_80147928` (508, found by the permuter: FAKE empty `do { } while (0);`) = 5,612 bytes. The last two and `func_80147B24` were not on the list (same files, same clone family as `func_80147488`). Guarded: `func_80133738` (8 lines), `func_801472D4` (20). `func_801471D0` keeps its T-11300 guard (4 lines). `func_80147B24` back to `INCLUDE_ASM` (63 lines, unrolled loop test).

Header edits: `func_80147488` takes `s16 *arg1`, `func_8013760C` returns `s32`, `D_8015E238` is a `TcPair8[]`. FAKE: unused `TcPos` in `func_80159878`, `(u8 *)i + (u32)ptr` operand order in `func_8013760C`; the `default: break;` and `s32`-without-return idioms are commented in place.

Review checklist for the code review: FAKE comments carry T-16561; no inline asm; guarded drafts carry the row count; the 80146D60.c comments on pointer locals are explanations, not fakematches.

### 2026-10-16 (claude, Opus closer): Closer pass, still In Review

About 35 minutes on the three guarded near misses.
- Matched `func_80133738` (312 bytes, commit "T-16561: match TACO:func_80133738"): `D_8015E238` is now `extern s32 D_8015E238[];` and the lookup reads `p = &D_8015E238[idx * 2]; func_80046290(p[0], p[1], 8);`. The flat index allocates the ugen temporary the `TcPair8` element form was missing. No FAKE: the `s32 *p` local declared before `idx` keeps the spill slot. Package total 9 of 12, 5,924 bytes.
- `func_801472D4` still 20 lines. Standalone probes show that IDO's struct copy skips one ugen register when the last store before it went through a pointer register (the loop stores), and two after a store through `$sp` or a global, after a call, or at entry. The original has two skips with no such store, so no source shape tried reaches it. The `NON_MATCHING` comment and [[register-cheatsheet]] carry the rule.
- `func_801471D0` still 4 lines. Every separate walker local is copy-propagated and its `+8` CSE'd. The in-place `dop` shape gives everything but the `move`, so the move looks like a uopt live-range split. Comment updated.
- autotweak (120 s, 2 jobs) found nothing on either. Final checks are in the commit; the ticket stays In Review for the code review.

