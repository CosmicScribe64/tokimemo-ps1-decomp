---
id: T-11840
title: TT batch 10: one long Opus run per large function (two at a time), bulk_m2c with a long autotweak budget
status: Done
assignee: claude (diman8 fork)
created: 2026-10-15
updated: 2026-10-16
links: ["[[tickets/T-11830-tt-wave1-handoff]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Tenth TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11830 (PR #33). TT is ours including the
former wave-1 files. Every remaining TT function has failed at least once; the twelve large ones (1.1-2.2 KB) hold about
half the remaining bytes.
- Track 1: one Opus agent per large function, about 90 minutes each, two running at a time in separate worktrees (never
  two on the same C file); each finished branch is merged into `tt-batch10` before the next agent starts. Order:
  `func_80146348`, `func_8014A2B4`, `func_80143A44`, `func_80144D70`, `func_801441BC`, `func_80141450`, `func_801400B0`,
  `func_80137C20`, `func_8013230C`, `func_8013D988`, `func_80136C64`, `func_801455C8`.
- Track 3: `tools/bulk_m2c.py --unit TT --exclude ""` in its own worktree with `--time 900` and `--max-size 1100` (the
  mid-size plain-asm functions; the large ones are track 1's).

## Acceptance criteria

- [x] Track 1 run over the list; matches committed one per commit, near misses guarded with `T-11840`
- [x] Track 3 run; verified matches committed (none found)
- [x] Inline review against `CODING_STANDARDS.md`
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
Track 1: 10 of 12 large functions matched, 15,564 bytes; 2 guarded.

| function | bytes | result |
|---|---|---|
| `func_80146348` | 1332 | matched (`TtRow6` row view, separate local for a call argument) |
| `func_8014A2B4` | 1324 | guarded, 28 lines (case 7 ugen emission order) |
| `func_80143A44` | 1444 | matched (store-chain phantom range found by a 900 s permuter run) |
| `func_80144D70` | 1536 | matched (u32 selector local, implicit-int `func_800AE080` override) |
| `func_801441BC` | 1688 | matched (`u16` store joins the constant-4 range) |
| `func_801400B0` | 1396 | matched (`u16` store of 7, `base + table[idx]`) |
| `func_80137C20` | 1368 | matched (2D-array casts for the loop-test variable) |
| `func_80141450` | 1484 | matched (pointer parameter, two permuter runs) |
| `func_8013D988` | 1160 | matched (reused loop counter, inline hoisted expression) |
| `func_8013230C` | 1284 | matched (`u16` stores join switch constants, separate zero stores) |
| `func_80136C64` | 2140 | matched (chained zero stores as phantom ranges) |
| `func_801455C8` | 2188 | guarded, 24 lines (550/550 instructions; registers only) |

Track 3 (bulk_m2c, 18 mid-size functions, `--time 900`): no match.

Per agent about 15-75 minutes and 120-255k tokens. Two weekly/session usage limits interrupted agents; they were resumed
from their saved state. Upstream `main` (849 commits ahead, including PR #33) was merged into the branch at the end:
conflicts only in `include/main_api.h` (our `func_800AE080` override guard next to upstream's new `func_800AE090` guard,
both kept) and the wiki kanban/log. After the merge: TT 262/303 (78.6% of TT bytes); grand total
`6123/6958   1566432/2279368  68.7%`.

Header changes: guarded blocks in `include/ovl/TT.h` (`TtRow6`, `TtSprXform`, `TtCore36`, externs, prototypes for
functions defined after their callers); one `MAIN_API_OVERRIDE_func_800AE080` guard in `include/main_api.h`.

## Review
Inline, against CODING_STANDARDS.md: every agent branch verified in its worktree before merging (full rebuild 27/27,
`funcdiff --resolve` MATCH); after the upstream merge a clean rebuild (asm/ and build/ removed) gives 27/27, headers and
globals OK, all ten functions MATCH, `sync_protos.py --check-branch` and `migrate_globals.py --check` clean. Every
fakematch (frame pads, empty-if priority levers, store chains, one-line groups, the parameter-vs-local base) carries
FAKE with T-11840 on its line; the override follows the `MAIN_API_OVERRIDE_` convention with its reason. One finding
fixed during the batch: `TT.h` declarations had been appended inside an earlier block; each agent now adds its own
guarded block, and the merge conflicts that causes were resolved by keeping both blocks. No open findings.

## Comments

### 2026-10-16 - integration review (PR #34, diman8)

Reviewed `git diff w2-good...pr34-int` (src, include, then wiki) against the CODING_STANDARDS.md checklist. No findings.

- Matches: the integrator's clean rebuild (asm/ and build/ removed) gave 666/666 with 27 `: OK`; all ten functions (`func_80146348`, `func_80143A44`, `func_80144D70`, `func_801441BC`, `func_801400B0`, `func_80137C20`, `func_80141450`, `func_8013D988`, `func_8013230C`, `func_80136C64`) verified. `check_headers.py`, `migrate_globals.py --check` and `sync_protos.py --check-branch` clean.
- Plain C: the diff has no inline asm, `.word`, `//` comments, `register`/`inline`/`volatile` or non-ASCII text. The only `goto` sits inside the guarded `func_801455C8` draft.
- Guards: the two near misses (`func_801455C8`, `func_8014A2B4`) are under `#ifdef NON_MATCHING` with `INCLUDE_ASM` in the `#else`, each with `T-11840` and the diff size in the comment.
- FAKE: every new FAKE comment gives the reason and `T-11840` (the multi-line one in `func_80141450` carries the ticket on its second line).
- Headers: `MAIN_API_OVERRIDE_func_800AE080` follows section 8a. `src/ovl/TT/80144D70.c` defines it on line 1, before the includes, with the reason (implicit-int view; the `void` prototype moves the reload of `ctl` to v0), and `include/main_api.h` wraps the prototype in `#ifndef`. The TT.h additions are `OVL_TT_T11840_*` guarded blocks with fields marked `unk_XX`.
- No game data: only `.c`, `.h` and `.md` files changed. The ticket and kanban column (Done) agree, and the card appears once.
- Integration: merge of `pr34` onto `w2-good` (commit 7825c84f); the only conflict was `wiki/kanban.md` (both sides kept). The splat "Unable to determine a segment" messages during the overlay splits name main-exe symbols and are not caused by this PR.
- After the merge: 6205/6958 functions, 1616160/2279368 bytes, 70.9% (overlays 5533/6128, 1468928/1994940, 73.6%; TT 262/303, 87564/111376, 78.6%).
