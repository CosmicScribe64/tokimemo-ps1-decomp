---
id: T-16920
title: "GameState +0x1093 type and kept-copy census"
status: Done
assignee: claude-opus (quizzical-swanson-a4bc82)
created: 2026-10-15
updated: 2026-10-16
links: [[[tickets/T-16910-cpw-census]], [[game-state]], [[matching-notes]], [[toolchain]], [[tickets/T-11570-w1-ovl-name_ent-80139740]], include/main_api.h, tools/cvt_pass.py]
---

## Goal

Two questions left by [[tickets/T-16910-cpw-census]]:

1. The type of GameState +0x1093/+0x1094 (`s8 unk_1093; s8 unk_1094;` in `include/main_api.h`). `menu_check` indexes
   +0x1093 by menu number, and the NAME_ENT selector-copy drafts get the original's constant-first compares only when
   the selector is read as an element. [[game-state]] keeps both bytes as members because three matched `switch`
   selectors need members. Try an array declaration, list every matched function that changes, decide which type the
   evidence supports, and write down the exceptions.
2. When does the original keep a local copy of a global aggregate element in its own register (`move`), and when does
   it fold it, as IDO's copy propagation does? Is a uniform zcopy rule supported under CODING_STANDARDS 7a?

## Acceptance criteria

- [x] Uses of +0x1093/+0x1094 in the original asm and the matched C listed (direct, indexed).
- [x] Array declaration tried: matched functions that change listed, verdict on the type with exceptions; a type change
      committed only with 27/27 sha1 and globals OK.
- [x] Census of kept against folded element copies; verdict on a zcopy rule.
- [x] Findings here and in [[matching-notes]]; log entry; ticket In Review.

## Notes

Result: **the selector-copy family is a C form, not a compiler difference.** Read through a local
`GameState *g = &D_800E6280;`, all 7 NAME_ENT functions of [[tickets/T-11570-w1-ovl-name_ent-80139740]] match with
`tools/cvt_pass.py` as built (2,332 bytes: `func_8013CEC8`, `func_8013D4CC`, `func_8013DAC4`, `func_8013E430`,
`func_8013F0DC`, `func_8013F454`, `func_8013FB8C`). +0x1093/+0x1094 stay members; an `s8[2]` changes 15 matched
`switch` functions. No zcopy rule is needed for this family, and none is supported in general: copy propagation off for every
procedure changes 737 of 5,967 matched functions.

### +0x1093/+0x1094 in the original and in the C

- Original asm: `D_800E7313` (+0x1093) is used by 61 functions (35 in C before this ticket), `D_800E7314` (+0x1094) by 69 (45 in C).
  The only indexed access is `menu_check` (`lui v0,%hi(D_800E7313); addu v0,v0,a0; lb v0,%lo(D_800E7313)(v0)`), the
  current selection of menu 0 or 1. The block at +0x1090 reads as two menus: `u8` counts at +0x1090/+0x1091 (the
  `MenuSelection5` view in `src/main/8004F870.c`), a mode byte at +0x1092, the current selections at +0x1093/+0x1094,
  and `unk_1095[]` (the confirmed selection, also indexed by menu number).
- C: 171 uses of `unk_1093`/`unk_1094` in 22 files, all member accesses except `menu_check`'s
  `(&D_800E6280.unk_1093)[arg0]`.

### Array type tried: `s8 unk_1093[2]`

Every use rewritten (`unk_1094` -> `unk_1093[1]`, `(&D_800E6280.unk_1093)[arg0]` -> `D_800E6280.unk_1093[arg0]`).
`tools/uopt_eval.py regress` on the 22 files (621 matched functions; the unmodified tree gives 621/621 identical): 602
identical, 19 changed. 15 are real changes, all a `switch` on the element whose selector moves from `$v0` (original)
to `$v1`, in ETC with an extra `move v0,v1`: main `func_8005C414`, `func_8005E2C0`, `func_8005E5C8`, `func_8005E924`,
`func_8005EB74`, `func_8005EE00`, `func_8005F09C`, `func_8005F478`, `func_8005FF68`, `func_800623E4`; ETC
`func_8014979C`, `func_80149A08`, `func_8014B8CC`; TEL `func_8013226C`, `func_8013639C`. The other 4 (`func_80061688`,
`func_80062C28`, ETC `func_8014AB88`, `func_8014C2A4`) differ only in trailing nops after the changed functions.
For IDO a constant-index element is an indirect load (not a plain variable), and the original's switches compile like
the plain member. Verdict: keep the members. `menu_check`'s `(&D_800E6280.unk_1093)[arg0]` stays as the one indexed
access (a first-member index, as before).

### The NAME_ENT compares: a GameState pointer

An element read gives the constant-first compares but not the switches, so the element is not the explanation. A read
through a local pointer is. In the T-16900 frozen targets (scratch, `measure.py`, resolved diff rows):

| function | member, as built | `cpw` (T-16900) | `g->` for the GameState fields, as built | final C |
|---|---|---|---|---|
| `func_8013DAC4` | 27 | 0 | 0 | 0 |
| `func_8013F454` | 23 | 0 | 0 | 0 |
| `func_8013FB8C` | 27 | 0 | 0 | 0 |
| `func_8013D4CC` | 33 | 6 | 6 | 0: `g->unk_1118 == idx + 1` (operand order) |
| `func_8013E430` | 28 | 4 | 4 (frame 0x30 vs 0x38) | 0 with `FAKE` `s32 pad[2]` |
| `func_8013CEC8` | 42 | 10 | 10 | 0: `D_800E7D36 == n + 1` and `FAKE` `s32 pad[2]` |
| `func_8013F0DC` | 18 | 18 | 29 | 0: `g->unk_1118 = idx + 1` and `FAKE` `s32 pad[2]` |

`g` only for `unk_1093` gives the same rows as `g` for every GameState field; the C uses `g` throughout. The three pads
are the same 8 bytes at the top of the frame in the three functions whose copy lives across a call (the spill slot,
sp+0x2C, is the same in both); `s32 pad` or `s32 pad[1]` give 0x30, `s32 pad[2]` gives 0x38. That is the open IDO
frame rule of T-11120 ("any extra local grows the frame by 8"), so the pads are marked `FAKE`. The local pointer is the
codebase's common idiom (156 `= &D_800E6280;` assignments in matched C).

Related type fix: the drafts read `(u8)(&D_800E7D15)[idx]` (first-symbol index). The original reads `D_800E7D15` with
`lbu` indexed by the selector in all 7 functions and stores it with `sb $zero` in main `func_80056284`, so it is now
`extern u8 D_800E7D15[];` (`include/main_api.h`; `func_80056284` writes `D_800E7D15[0] = 0;`, same code).

Verification: `uopt_eval regress` on `NAME_ENT/80139740` and `main/80053650` 116/116 identical (7 new functions);
`ninja` after the header change (every C object rebuilt): 27/27 sha1 OK, NAME_ENT.bin OK, globals OK.

### Kept copies in the original (census)

T-16910's dataflow census (6958 functions), used `move` copies of a global load, by class:

| copied global | matched functions | unmatched functions |
|---|---|---|
| aggregate member (GameState, record table, indexed array, near one) | 12 | 30 |
| scalar | 27 | 20 |

The matched ones get the copy from ordinary C. Of the 12 aggregate cases, 4 read through a local `GameState *g`
(main `func_8007C8D8`, TEL `func_8013BFDC`, `func_8013C4D8`, DATE `func_80134C64`), 3 copy the record-table byte
`D_8011ECD3` in read-modify-write sequences (main `func_80083A10`, GEKO `func_801351F8`, `func_8013882C`), and 5
use the global directly (main `func_800603C0`, `normal_date_bggirl_fadein`, `normal_date_bggirl_fadeout`, TAIIKU
`func_80136DA4`, BUNKA_SD `func_80137620`). Before this ticket, 7 of the 30 unmatched aggregate cases were the NAME_ENT family; the other 23
are candidates for the same pointer reading.

Copy propagation off for every procedure (scratch `cp0`, `uopt_eval regress` over all matched C after this ticket's
changes): 5,230 of 5,967 matched functions identical, 737 changed (including knock-on trailing-nop differences).
The matched code depends on IDO's propagation, so a uniform zcopy-off rule fails 7a; together with the 12 functions
of T-16900's `cps` and the NAME_ENT result, the kept copies in the original are a question of the C (pointer reads,
read-modify-write, later stores), not of the compiler.

### Follow-ups

- Try the pointer reading on the other unmatched functions that keep a copy of an aggregate member (23) and on the
  constant-first compares that still need a `FAKE` or cast ([[tickets/T-16910-cpw-census]]: ENDING `func_8013855C`,
  NAME_ENT `func_80148C20`, BUNKA_SD `func_8013261C`, EVENT `func_80117004`, RPG_BAT `func_80150CD8`).
- The 8-byte frame difference with a spilled copy (three `FAKE` pads here) belongs to the IDO frame rule (T-11120).

## Comments

- 2026-10-16 review (CODING_STANDARDS.md checklist, diff w2-good...side-int, together with [[tickets/T-16930-pointer-read-kept-copies]]):
  the 7 NAME_ENT functions are C definitions and `funcdiff.py --resolve` reports MATCH for each (2,332 bytes); the old
  `NON_MATCHING` guards and their `INCLUDE_ASM` fallbacks are gone, no inline asm. The three `s32 pad[2]` carry `FAKE`
  comments with reason and ticket. `D_800E7D15` is declared once, in `include/main_api.h` (`u8[]`, the type of its
  users), and main `func_80056284` still MATCHes with `D_800E7D15[0] = 0`. 27/27 sha1, `check_headers.py`,
  `migrate_globals.py --check` and `sync_protos.py --check-branch` OK. No finding in this ticket's scope. Done.
- Still open from the Follow-ups above: the 23 functions were taken up in T-16930 (8 matched, the rest listed there);
  the 8-byte frame rule (T-11120) is not settled, and the three pads may go the way of `rate` in RPG_BAT
  `func_801495DC` (T-16930).
