---
id: T-16910
title: "Census the original for the T-16900 cpw rule"
status: In Review
assignee: claude-opus (quizzical-swanson-a4bc82)
created: 2026-10-15
updated: 2026-10-15
links: [[[tickets/T-16900-compiler-experiments]], [[uopt-register-allocation]], [[toolchain]], [[matching-notes]], [[tickets/T-11570-w1-ovl-name_ent-80139740]], tools/cvt_pass.py, tools/test_cvt_pass.py]
---

## Goal

Decide whether the T-16900 `cpw` rule (zcopy off plus constant first in `==`/`!=`, only in a procedure that copies a
narrow signed global into a local and compares it with a constant) can be a uniform step of `tools/cvt_pass.py` under
CODING_STANDARDS 7a. 7a needs the rule derived from a broad sample of the original code, with its exceptions written
down; T-16900 showed it fires in no matched function, so the matched build is no evidence for it.

## Acceptance criteria

- [x] Census of the original asm (`asm/**`): functions that load a narrow signed global (`lb`/`lh`), compare it `==`/`!=`
      with a constant held in a register, and copy it with `move`/`or` into another register the code then uses;
      constant-first and variable-first counts; the opposite forms (no copy, global folded) in functions that copy a
      narrow signed global. Exceptions listed.
- [x] If the census supports the rule: `cpw` in `tools/cvt_pass.py` (docstring, synthetic tests, fails loudly), clean
      rebuild 27/27 sha1 and globals OK, NAME_ENT `func_8013DAC4`, `func_8013F454`, `func_8013FB8C` matched as plain C
      (NAME_ENT.bin sha1 OK). Otherwise a negative result. **Negative result; no pass change.**
- [x] Numbers in this ticket and in [[toolchain]] or [[matching-notes]]; log entry; ticket In Review.

## Notes

Verdict: **`cpw` cannot be a uniform step of `tools/cvt_pass.py`.** The coupled shape exists in the original only in
the 7 NAME_ENT functions it was built for, all on one byte. Constant-first compares on narrow signed globals are common
without any copy, and what decides the order is whether the global is a member of an aggregate. IDO already gives that
order when the C reads the value as an element. So the constant-first half of `cpw` is a question for the data types,
not for the compiler. The copy half (copy propagation off) is a separate open question. `tools/cvt_pass.py` is unchanged.

T-16900 (`wiki/tickets/T-16900-compiler-experiments.md`) is on branch `claude/blissful-fermi-43d29a`, not merged into
this branch.

### Method

Scratch script (outside the repository, see "Scratch" below): a forward dataflow over every game function in the split
asm (`asm/{matchings,nonmatchings}`, main and the 26 overlays, SDK excluded): 6958 functions, 5960 of them in
`matchings`. Register facts: a scalar global loaded with `%lo(D)(r)` where `r` holds `%hi(D)` (array loads through an
index register are not counted), its `move`/`or`/`addu` copies, and constants (`addiu`/`ori` from `$zero`, `lui` +
`ori`). Facts merge by intersection at labels; `jlabel` targets and the function entry start empty; a call clobbers
the caller-saved registers after its delay slot. A compare is a `beq`/`bne` with a global on one side and a
constant on the other; constant-first means the constant is the first operand. A copy is *used* if its destination is
read by any instruction before it is overwritten; copies read only by a call (argument) or by `jr $ra` (return) are
not counted. Constants in `$at` are ugen's, always second, and say nothing about uopt's order (lb 101, lh 288 compares,
all variable-first); they are left out below.

Aggregate class of a global (address-based): GameState (`D_800E6280`..`0x800E7D10`), the 0x44-byte record table
(`D_8011ECD0`..`0x80121750`, 160 records), an indexed array (the symbol is the base of an `addu` with an index register
somewhere in the original; 610 symbols), within 0x40 bytes above such a base (heuristic), or scalar.

Sanity check against T-7000 ("K&R promotion rules"): this scanner counts more compares than T-7000's but the same
direction: `lbu` 488 constant-first against 136, `lh` 50 against 8, `lw` 30 against 97 (T-7000: 310:76, 40:8, 24:83).

### Census: narrow signed globals (`lb`/`lh`) compared `==`/`!=` with a register constant

72 compares in 23 functions (19 globals constant-first, 2 variable-first):

| form | functions | compares |
|---|---|---|
| coupled shape: used copy of G and constant-first compare of G | 7 | 14 |
| constant-first, no copy of G | 15 (10 matched) | 50 |
| variable-first, no copy | 6 (4 matched) | 8 |
| variable-first with a used copy | 0 | 0 |

- Coupled shape: NAME_ENT `func_8013CEC8`, `func_8013D4CC`, `func_8013DAC4`, `func_8013E430`, `func_8013F0DC`,
  `func_8013F454`, `func_8013FB8C`: all in `src/ovl/NAME_ENT/80139740.c`, all on `D_800E7313` (GameState +0x1093),
  constant -1 in `$a0`/`$a1`. Exactly the T-11570 family; nothing else in the original has the shape.
- Constant-first without a copy: DATE `func_80139DD0`, `func_80139F3C` (`D_800CA22C`/`E`, an indexed `s16` array);
  ENDING `func_80137F64`, `func_8013855C`, `func_80138A9C`, `func_80138DC8`, `func_801387B0` (`D_8011F4D0`/`DE`/`E0`,
  table fields); NAME_ENT `func_80148C20` (`D_80120658`/`68`, table); BUNKA_SD `func_8013261C` (`D_801206F0`, table);
  TAIIKU `func_8013F948`, `func_80137EA8`, `func_80146D18` (`D_80120888`/`8CC`/`AA8`, table); RPG_BAT `func_80150CD8`
  (`D_801213B0`, table); EVENT `func_80117004` (`D_8012439C`, indexed `s16` array); EN_NICHI `func_8013735C`
  (`D_80139C4C`..`54`, an indexed `s16[5]`; the values stay in `$a0`..`$t0`, so register promotion does not decide the
  order).
- Variable-first (the exceptions to constant-first): ENDING `func_80137F64`, `func_8013855C`, `func_80138A9C`,
  `func_80138DC8`, `func_801387B0` on `D_8013C35C`, SHOUGATU `func_80139334` on `D_80144E0C`. Both are `s16` scalars:
  their address is taken (stored as a pointer), but no access indexes them.
- Used copies of a narrow signed global without a register-constant compare of it: 10 functions (main
  `func_800603C0` (matched), `func_80065B0C`; OPTION `func_80137C1C`, `func_801383C4`; RPG_BAT `func_801495DC`,
  `func_801447E0`; ENDING `func_80137730`; EVENT `func_800FEDC8`; DATE `func_8014A8FC`; TACO `func_80134A18`). 17
  functions copy a narrow signed global in all; the copy alone does not come with constant-first.
- The folded form (C copies the global into a local, the original has no `move`) cannot be seen in asm. From the
  matched C (T-16900): 12 matched functions copy a narrow signed global into a local and need IDO's propagation (zcopy
  off changes them). Their globals are aggregate members too (OPTION `func_80134804`, `func_80134C00`, `func_80134E64`,
  `func_801368FC`, `func_80137E38`: `sel = D_800E6280.unk_1094`, the byte after the NAME_ENT selector; NAME_ENT
  `func_80133D94`, `func_80133E54`, `func_80149E3C`: table fields). Their original source may not have had the local,
  so they show only that the copy half does not follow from "copies a narrow signed global".

### What decides the order: aggregate members

| load | class | constant-first | variable-first |
|---|---|---|---|
| `lb` | GameState | 14 (7 fn) | 0 |
| `lh` | record table | 18 (11 fn) | 0 |
| `lh` | indexed array | 17 (4 fn) | 0 |
| `lh` | within 0x40 above an indexed base | 15 (3 fn) | 0 |
| `lh` | scalar | 0 | 8 (6 fn) |
| `lbu` | GameState | 332 (67 fn) | 10 (8 fn) |
| `lw` | GameState | 5 (2 fn) | 0 |
| `lw` | scalar | 12 (6 fn) | 82 (21 fn) |

For narrow signed globals the split is complete: 64 of 64 constant-first compares are on aggregate members and 8 of 8
variable-first ones on scalars. Words follow the same direction. (Unsigned narrow scalars are mixed, 98:75, because
of their widening `CVT`, T-7000.) uopt swaps `==`/`!=` when the left operand is not a plain variable; in the original,
a member of a global aggregate was not one.

The matched code already says the same. The 10 matched constant-first functions above get the order from IDO in two
ways: 5 read the value through the aggregate (DATE's `D_800CA22C[]`; ENDING's table view `*(s16 *)&D_8011ECD0[0x80E]`,
T-9020, in the same file whose scalar `D_8013C35C` compares variable-first). The other 5 declare a scalar and need a
trick: `FAKE` `s16 one` (ENDING `func_8013855C`), `FAKE` byte constant (NAME_ENT `func_80148C20`), `FAKE` `^ 0`
(BUNKA_SD `func_8013261C`), `FAKE` `(u32)` casts (EVENT `func_80117004`), a `(u32)` cast (RPG_BAT `func_80150CD8`).

### Experiment: the NAME_ENT drafts with an element access

`menu_check` indexes GameState +0x1093 by menu number ([[game-state]]), so +0x1093 is an array in the original. The
T-16900 frozen targets (scratch copy of `src/ovl/NAME_ENT/80139740.c`, not committed) were compiled with every
`D_800E6280.unk_1093` written as an element: `(&D_800E6280.unk_1093)[0]` (table below), or `D_800E7313[0]` with
`extern s8 D_800E7313[];` (same rows, except `func_8013F454` as built: 23). Resolved diff rows (T-16900 `measure.py`):

| function | member, pass as built | `cpw` | element, pass as built | element, zcopy off for the file (`cp0`) |
|---|---|---|---|---|
| `func_8013DAC4` | 27 | 0 | 27 | **0** |
| `func_8013F454` | 23 | 0 | 14 | **0** |
| `func_8013FB8C` | 27 | 0 | 27 | **0** |
| `func_8013D4CC` | 33 | 6 | 33 | 6 |
| `func_8013E430` | 28 | 4 | 28 | 4 |
| `func_8013F0DC` | 18 | 18 | 32 | 8 |
| `func_8013CEC8` | 42 | 10 | 40 | 10 |

With the element, IDO's uopt already puts the constant first; only the copy remains. (The `cps` rule does not fire on
the element form, which is an indirect load, so its column equals "as built".) `cp0` is a scratch switch for this
file, not a rule.

### Consequences

- No `cpw` in `tools/cvt_pass.py`. Its constant-first half would mark one family; the rule in the original is
  aggregate membership, and section 8 of CODING_STANDARDS ("prefer the real aggregate") already covers it in C.
- Open, for follow-up tickets: (1) the type of GameState +0x1093/+0x1094. An element access gives the NAME_ENT compares,
  while [[game-state]] keeps them as members because three matched `switch` selectors (main `func_8005C414`,
  `func_8005E924`, ETC `func_8014979C`) need members. (2) The copy half: the original keeps `idx = sel` in its own
  register in the 7 NAME_ENT functions; IDO folds it. A rule needs its own census (copies of aggregate elements kept
  against folded), and the folded side is only visible through matched C. (3) The 5 `FAKE`s/casts above could become
  aggregate reads (table view or arrays), as T-9020 did in ENDING.
- The `cvt_pass.py` docstring's "lh 40 against 8" (T-7000) stays true; the 8 are the two scalars above.

### Scratch

Census script, aggregation scripts and the element-access targets: shared notes folder of the session group
(`.../side-session-notes/local_0bca76a4-.../t16910/`, README there). The census reads only `asm/` text; it writes no
game data into the repository.

## Comments

- 2026-10-15, T-16920: the open questions are settled. Read through a local `GameState *g`, all 7 NAME_ENT
  functions match with the pass as built (constant-first compare and kept copy both follow from the indirect load).
  An `s8[2]` type for +0x1093 changes 15 matched switches, so the members stay. [[tickets/T-16920-gamestate-1093-type]].
