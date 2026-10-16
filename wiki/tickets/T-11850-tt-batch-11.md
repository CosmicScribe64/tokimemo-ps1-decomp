---
id: T-11850
title: TT batch 11: one long Opus run per file group (two at a time)
status: Done
assignee: claude (diman8 fork)
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-11840-tt-batch-10]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Eleventh TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11840 (PR #34). Batch 10 showed that long
single-target Opus sessions convert most hard functions, so this batch gives one Opus agent per file group (about 90
minutes), two running at a time in separate worktrees, never two on the same C file. Each finished branch is merged into
`tt-batch11` before the next agent starts. Groups, in order:
1. `8014AF00.c`: `func_8014C5F8` (guarded), `func_8014C84C`
2. `801478D0.c`: `func_80147FD0` (guarded), `func_80147A18`
3. `80148210.c`: `func_80148764` (guarded)
4. `80148DB0.c`: `func_80148E7C`, `func_80149704`
5. `80133970.c`: `func_80134874`, `func_80134768`, `func_80134BBC`
6. `80142D40.c`: `func_801436BC`, `func_80144A04`, `func_801437F0`
7. `8013A040.c`: `func_8013A0DC`, then `80148DB0.c`: `func_8014A2B4`

## Acceptance criteria

- [x] All seven groups run; matches committed one per commit, near misses guarded with `T-11850`
- [x] Inline review against `CODING_STANDARDS.md`
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
12 of 15 functions matched (4,520 bytes), 3 guarded:

| Group | File | Result |
|---|---|---|
| 1 | `8014AF00.c` | `func_8014C5F8`, `func_8014C84C` matched |
| 2 | `801478D0.c` | `func_80147FD0` matched; `func_80147A18` guarded (58 lines, ugen t6/t7) |
| 3 | `80148210.c` | `func_80148764` matched (pointer table, no FAKE) |
| 4 | `80148DB0.c` | `func_80148E7C`, `func_80149704` matched |
| 5 | `80133970.c` | `func_80134874`, `func_80134768`, `func_80134BBC` matched |
| 6 | `80142D40.c` | `func_80144A04`, `func_801436BC`, `func_801437F0` matched |
| 7 | `8013A040.c`, `80148DB0.c` | `func_8013A0DC` guarded (2 lines); `func_8014A2B4` guarded (28 lines) |

TT 274/303 functions, 82.7% of bytes (from 262/303, 78.6%); grand total `6208/6958 1613432/2279368 70.8%`. Notes in
[[matching-notes]] ("TT overlay, batch 11"), rows in [[data/t0018-cases]].

## Review
Inline review against `CODING_STANDARDS.md`: every fake trick carries a `FAKE ... T-11850` comment (chain pairs are
covered by one comment above the pair); one guarded header block (`TtSpr*` types); one existing declaration changed
(`D_801569E8[]`); no `main_api.h` changes; guard comments current. Clean rebuild 27/27 OK, headers/globals OK,
`sync_protos.py --check-branch` and `migrate_globals.py --check` clean. No open findings.

## Comments

### 2026-10-16 independent review and merge (PR #35, integration branch `pr35-int`)
Reviewed `git diff w2-good...pr35-int` (src, include, then wiki) against the `CODING_STANDARDS.md` checklist.

- Matches: integrator ran `funcdiff --resolve` on all 12 claimed functions (`func_8014C5F8`, `func_8014C84C`, `func_80147FD0`, `func_80148764`, `func_80134874`, `func_80134768`, `func_80134BBC`, `func_80148E7C`, `func_80149704`, `func_80144A04`, `func_801436BC`, `func_801437F0`): all MATCH. Clean rebuild 667/667 steps, 27/27 sha1 OK; `check_headers.py`, `migrate_globals.py --check` and `sync_protos.py --check-branch` (0 new disagreements) clean.
- Guards: `func_80147A18`, `func_8013A0DC` and `func_8014A2B4` stay under `#ifdef NON_MATCHING` with a `T-11850` comment; the default build does not see them.
- C89 and content: no `//`, inline asm, `.word`, `volatile` or non-ASCII in the added lines; declarations sit at block tops. No game data, asm or binaries; changes are limited to `src/ovl/TT`, `include/ovl/TT.h` and `wiki/`.
- Headers: `D_801569E8` is now `u8 *D_801569E8[]`. Its only C user is `func_80148764` (reads `D_801569E8[0]`, matches); all other users are `INCLUDE_ASM` bodies that reference the symbol address only. `TtSprPart`/`TtSprSeq`/`TtSprFrame`/`TtSprSet` carry offsets and sizes.
- Fakematches: every new trick carries a `FAKE` comment with `T-11850` and the reason.
- Finding 1 (fixed in the merge): in `func_80149704`, `q[0] = q[1] = 0;` is a trick (it uses up ugen temp t9, per [[matching-notes]]) but had no `FAKE` mark, while the same idiom is marked elsewhere in this batch. Added a `FAKE` comment on the line; no code change.
- Notes, not findings: `func_80148E7C` (split `&&` into `continue` tests, ticket says no FAKE) and `func_8014C84C` (casts of `buf` at each use) read as plausible source, so they stay unmarked.
- Bookkeeping: ticket status Done matches the card in the Done column of [[kanban]] (one card); [[index]] line updated. The ticket's grand total (6208/6958, 70.8%) was measured on the contributor's base; after this merge it is 6233/6958 functions, 1639244/2279368 bytes, 71.9%.

Result: 12 of 15 functions matched, TT 274/303 functions, 92080/111376 bytes (82.7%). Review complete, one finding fixed, none open.
