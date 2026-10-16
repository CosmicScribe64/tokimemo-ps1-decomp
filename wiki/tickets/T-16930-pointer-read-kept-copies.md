---
id: T-16930
title: "Read aggregate fields through a local pointer: kept copies and constant-first tricks"
status: In Progress
assignee: claude-opus (pensive-montalcini-7151d4)
created: 2026-10-15
updated: 2026-10-16
links: [[[tickets/T-16920-gamestate-1093-type]], [[tickets/T-16910-cpw-census]], [[register-cheatsheet]], [[matching-notes]], [[game-state]], [[data-types]]]
---

## Goal

[[tickets/T-16920-gamestate-1093-type]] matched the NAME_ENT selector-copy family by reading GameState through a local
`GameState *g = &D_800E6280;`: an indirect load is not a plain variable for uopt, so it keeps the original's
constant-first `==`/`!=` and the `move` copy of the field. Apply the same reading to:

1. The other 23 unmatched functions of the T-16910 census whose original keeps a used `move` copy of a load of an
   aggregate member (GameState 0x800E6280..0x800E7D10, the `D_8011ECD0` record table 0x8011ECD0..0x80121750, indexed
   arrays). Read the field through `GameState *g`, or a record pointer for the table.
2. The five matched constant-first compares that still need a trick: ENDING `func_8013855C` (`FAKE` `s16 one`),
   NAME_ENT `func_80148C20` (`FAKE` byte constant), BUNKA_SD `func_8013261C` (`FAKE` `^ 0`), EVENT `func_80117004`
   (`FAKE` `(u32)`), RPG_BAT `func_80150CD8` (`(u32)` cast). Replace a trick only if the function stays byte-identical.

## Acceptance criteria

- [ ] The 23 functions listed with the census scripts; each tried with a pointer read; matches committed as plain C
      (no new `FAKE` unless marked and ticketed).
- [ ] The five tricks tried; each replaced only when the function stays byte-identical.
- [ ] Unit sha1 and 27/27 sha1, headers and globals OK.
- [ ] Results and misses here and in [[matching-notes]]; log entry; ticket In Review.

## Notes

### Part 2: the five constant-first tricks

Four of five tricks go away when the field is read through the aggregate; each function stays byte-identical
(`funcdiff --resolve` MATCH, then 27/27 sha1, globals and headers OK). Commit `8ba34ea3`.

| function | trick before | C now |
|---|---|---|
| EVENT `func_80117004` | `FAKE` `(u32)` casts on `D_8012439C` | `s16 *res = &D_8012439C;` and `*res == 4` etc. |
| BUNKA_SD `func_8013261C` | `FAKE` `0x14 == (D_801206F0 ^ 0)` | `s16 *state = &D_801206F0;` and `*state == 0x14`, `*state == 5` |
| NAME_ENT `func_80148C20` | `FAKE` `u8 one = 1` | `NameRecord44 *rec = &((NameRecord44 *)D_8011ECD0)[96];` and `rec->unk_18 == 1 && rec->unk_08 == 1` (+0x08 named `unk_08` in `include/ovl/NAME_ENT.h`) |
| ENDING `func_8013855C` | `FAKE` `s16 one = 1` | the file's table view (T-9020): `*(s16 *)&D_8011ECD0[0x810] == 1 && *(s16 *)&D_8011ECD0[0x800] == 1` |
| RPG_BAT `func_80150CD8` | `(u32) D_801213B0 == 1` | unchanged: the cast is a type, not an order trick |

RPG_BAT: the original shares the constant 1 in `$v0` with the `case 1` of the `u32` switch, so the compare must be
unsigned. Through the table view (`*(s16 *)&D_8011ECD0[0x26E0] == 1`) the order is right but the constant moves to
`$at`; with an `s32` switch the selector moves from `$v1` to `$v0` (38 rows). The `(u32)` stays.

ENDING `func_8013855C` also carries a second `FAKE` (a dead `if (D_800E6280.unk_10F8) {}` before `% 240`, T-9410).
Reading `unk_10F8` through `GameState *g` instead does not replace it (register rows remain); it stays.

### Part 1: the census functions

The census scripts of T-16910 (scratch, see "Scratch") with an aggregate classifier (`agg7.py`) give 22 unmatched and
19 matched functions on this branch whose original keeps a used `move` copy of a load of an aggregate member
(T-16920 counted 23 unmatched; one function fewer reproduces now). All 22 were still `INCLUDE_ASM` on `main` too.

| function | bytes | copied load | result |
|---|---|---|---|
| main `func_80042AC8` | 268 | GameState +0xF74 (`lbu`) | **match** through `GameState *g`; the same C with plain member reads leaves 2 rows (`bne v1,a1` instead of the original's constant-first `bne a1,v1`); the inner `if/else` gives the duplicated `b`/`li a1,0xFF` tail |
| GEKO `func_80134040` | 352 | `D_8011ECD3` (`lbu`, record table) | **match** as plain C: the read-modify-write of its matched siblings `func_801351F8`/`func_8013882C` |
| RPG_BAT `func_801495DC` | 276 | GameState +0x10E (`lh`) | **match** as plain C: `rate = D_800E6280.unk_10C.unk_02; lv = (rate > 300) ? 300 : rate;` gives the copy and the original's 0x38 frame (without `rate` the frame is 0x30, 14 rows) |
| main `func_8007B64C` | 148 | `D_80125CC0` (`lhu`, queue count) | miss, 52 rows: the copy is the loop-exit reload of the count; the count sits in `$v1` instead of `$v0` with or without a pointer |
| TACO `func_801484E4` | 392 | GameState +0x1104 (`lw`, frame counter) | miss, 65 rows: a plain local copied across calls; the load lands in `$a1` (original `$v0`) and the frame is 0x30 (original 0x38); `GameState *g` gives 70 rows |

### State at hand-over (2026-10-15)

Stopped on the user's request; the main thread merges and finishes the ticket.

- This branch (`claude/pensive-montalcini-7151d4`): part 2 (`8ba34ea3`), main `func_80042AC8`, GEKO `func_80134040`,
  RPG_BAT `func_801495DC` (`0e75a632`), ENDING `func_80137730` (`0af95fcc`, agent B: `D_800B5A60` written as the promoted
  global, `GameState *g`, plain-read control 46 rows; `D_8013C358` typed `s16`, `func_80137730` returns `s32`). Each
  commit checked 27/27 sha1, globals and headers OK.
- Branch `t16930-a` (agent A, worktree `.claude/worktrees/agent-a1dc1c3dff694bc54`, based on `0e75a632`, not merged):
  main `func_80078C48`, OPTION `func_80137C1C`, OPTION `func_801383C4`, SHUGAKU `func_80137490`. Its report was not
  received (stopped); the agent ran `check.sh` before each commit, not re-verified here.
- Misses with drafts in the scratch folder `drafts/`: EVENT `func_800FEDC8` (44 rows, two register swaps: imax/base and
  sum/q), DATE `func_8013FAD0` (~123 rows; table view `((DateParticleView44 *)D_8011ECD0)[31].unk_07` cuts 194 to 130),
  VALEN `func_80133088` (193), RPG_BAT `func_801447E0` (174, frame 0x48 vs 0x40), main `func_80065B0C` (353, agent A).
- Not attempted: SHOUGATU `func_8013FB28`, TACO `func_8014616C`, `func_80146754`, OPTION `func_8013A7EC`, KANGEI
  `func_80132FF8`, NAME_ENT `func_80134248`, ENDING `func_8013A2E8`.
- Findings for [[matching-notes]] (not yet written there): reading through `g` decided main `func_80042AC8` (control 2
  rows) and ENDING `func_80137730` (control 46 rows); a global the original keeps in one register and stores from several
  branches is the variable itself (uopt promotes it); a natural extra local declared first closed an 8-byte frame
  (RPG_BAT `func_801495DC`), which may also replace T-16920's three `FAKE` `s32 pad[2]`.

Scratch: `/Users/lila/Library/Application Support/Claude/side-session-notes/local_e21231c2-d615-49ae-b3fd-761d32d316e4/t16930/`
(census scripts with `agg7.py`, `tools/try.py`, `fn.py`, `check.sh`, drafts).

## Comments

### Integration (2026-10-16, side-int)

Branches `claude/pensive-montalcini-7151d4`, `t16930-a` and `claude/blissful-fermi-43d29a` merged onto `w2-good` in worktree
`side-int`. Agent A's four functions were re-verified there with `funcdiff.py --resolve` and all MATCH: main `func_80078C48`
(920 bytes), OPTION `func_80137C1C` (540), OPTION `func_801383C4` (556), SHUGAKU `func_80137490` (624). With the pensive-montalcini
matches (main `func_80042AC8`, GEKO `func_80134040`, RPG_BAT `func_801495DC`, ENDING `func_80137730`) that is 8 new
matches in this ticket, and the four rewritten constant-first functions (EVENT `func_80117004`, BUNKA_SD `func_8013261C`,
NAME_ENT `func_80148C20`, ENDING `func_8013855C`) still MATCH. One header conflict: `D_800B5A68` is `u8[][3]` on `w2-good` (main
`func_80067C28`); ENDING `func_80137730` now indexes it as `[g->unk_F5F][i]` instead of the flat `[g->unk_F5F * 3 + i]` and still
matches. Agent A wrote no wiki notes; its commit messages carry the findings (selector read through `GameState *g`; plain
member reads leave 21 / 94 / 109 rows; `ovl_load_lba` assigned in every case is promoted, so `func_80078C48` returns `s32`;
declaration order `e, sel, i, j` sets the OPTION spill slot). Status stays In Progress (open misses listed above).
