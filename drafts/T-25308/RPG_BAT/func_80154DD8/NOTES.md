# T-25308: RPG_BAT func_80154DD8 notes (second pass)

Status: **matched**, default-build C, 1204 bytes, commit 553f40b11 (`T-25308: match RPG_BAT:func_80154DD8`).
`tools/funcdiff.py func_80154DD8 --unit RPG_BAT --resolve --version default`: MATCH. About 20 minutes.

## Start state

T-25114 guard in `src/ovl/RPG_BAT/80154430.c`: 34 funcdiff rows (14 with `drafts/T-25114/score.py`),
all aligned. Left: frame 0x60 against 0x58, the case 0 `D_8015EBAC` load (first in the original) and
the case 3 counter store (before the test loads in the original). Notes:
`drafts/T-25114/RPG_BAT/func_80154DD8/NOTES.md`.

## What was measured

Frame and homes, read with the fast scorer and a home extractor on variants of the guard body:

| body | frame | case 1 homes | case 3 homes |
| --- | --- | --- | --- |
| T-25114 guard (views, x y nx ny declared) | 0x60 | 0x48/0x44 | 0x48/0x44 |
| plain reads (x folded) | 0x50 | 0x44/0x40 | 0x44/0x40 |
| plain reads + `pad` first | 0x58 | 0x48/0x44 | 0x48/0x44 |
| views on case 1 only | 0x60 | 0x48/0x44 | 0x54/0x50 |
| views, no nx/ny, `-x, -y` in both calls | **0x58** | **0x48/0x44** | **0x48/0x44** |

- Declared scalars take slots top-down in declaration order (x top-4, y top-8, nx top-0xC, ny top-0x10);
  four more words sit below them in every variant.
- When x is folded (plain reads), the spilled nx/ny use their declared slots. When the view keeps x,
  the spilled values get new uopt homes below the declared locals (one extra word, then nx', ny'), and the
  declared nx/ny slots stay unused: 4 declared + 3 + 4 = 11 words, frame 0x60.
- So the original declares only two scalars. With the views, `-x`/`-y` written in both calls are kept in
  uopt temporaries across the first call (stored once at 0x48/0x44, reloaded for the second call):
  2 + 3 + 4 = 9 words, frame 0x58. T-25114's negative "uopt spills x and negates again" holds only for
  plain reads.

Order (as1 `-R -xbbdbg 8` trace, T-24900 capture shim): in case 0 ugen already emits the selector load
first (INST 2/3), but the `lui at`/`sw` chain of the five stores has aftercycles 10 against 4 for
`lui v1`, so as1 schedules the stores first. as1 does not move a load above a store through the same
symbol (wiki/game-state.md, wiki/data-types.md). In the original the counter and the selector are one int
array (`D_8015EC9C` is `D_8015EBAC[60]`, as 80134D30.c writes it; `D_8015EC74..80` are elements 50-53,
as func_8015528C's `(&D_8015EBAC)[arg6]` shows). The same rule explains case 3: the test loads cannot
pass the counter store when they share its symbol.

## What closed

1. Frame: drop `nx`/`ny`, write `-x, -y` in both calls (4 of the 14 rows gone).
2. Case 0: `(&D_8015EBAC)[60] = 1;` for the counter store (FAKE). Writing all five stores through
   `(&D_8015EBAC)[50..53, 60]` matches too.
3. Case 3: `(&D_8015EC9C)[-16]` for `D_8015EC5C` in the test (FAKE). The same read through `D_8015EC9C`
   for both test loads swaps the `addu` operands (indirect read first); reading `D_8015EC64` through
   its struct is enough, as1 then keeps both loads behind the store.

## Negatives and controls (fast scorer rows, final body unless stated)

- `switch (D_8015EBAC)` without the view: 20 rows (constant 1 in `$v1`). The T-17114 view stays.
- Plain reads in case 1: 281 rows; in case 3: 297 rows. The web-keeping views stay.
- `x = -x; y = -y;` with two declared locals: folded again (342 rows), frame 0x48.
- A `sel` local (plain or `register`) read before the stores: the load does not move and the homes
  shift by one word. `*(volatile s32 *)&D_8015EBAC` as selector or local: 283 to 305 rows.
- Counter store through the struct, `(&D_8015EC58.unk_00)[17]++`: 32 rows. All case 3 accesses
  through `(&D_8015EBAC)[n]`: 180 rows (the counter is no longer one uopt variable).
- A dead `register s32 zc = D_8015E814;` (zcopy off, T-25301) on the plain body: still folded, so the
  fold is the in-block tree build, not global copy propagation.

## Follow-up

The FAKE views stand in for the real aggregate: an int array at `D_8015EBAC` that holds at least
elements 43-60 (`D_8015EC58` as elements 43-48, `D_8015EC74..80`, `D_8015EC9C`). Declaring it in
`include/ovl/RPG_BAT.h` (CODING_STANDARDS section 8) needs a pass over every RPG_BAT user and
`tools/migrate_globals.py`; it is not part of this ticket.
