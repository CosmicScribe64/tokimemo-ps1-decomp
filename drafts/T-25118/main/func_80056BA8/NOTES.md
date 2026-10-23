# T-25118: main func_80056BA8 (movie player)

Result: matched, default C, 1296 bytes (325 words). `funcdiff --resolve --version default` prints MATCH and
`src/main/800563F0.c` has no INCLUDE_ASM line for it. Commit `T-25118: match main:func_80056BA8`.

## Start state

INCLUDE_ASM. Best known body: `drafts/T-17014/func_80056BA8/body.c` (T-17014, "about 250 rows"; its
DISPENV/DRAWENV/CdlFILE types were in a local `declarations.h`). With the types in headers and the
`s32 (s32, s32, s32)` prototype it measured 219 diff lines (sbs rows) and a 240-byte frame against 264.

## Changes, in order, with the rows after each (funcdiff --resolve rows)

1. Types: DISPENV/DRAWENV in `include/psx_types.h` (next to DR_ENV), CdlFILE in a new `include/libcd.h`,
   `void strCallback(void);` and `s32 func_80056BA8(s32, s32, s32);` in `main_api.h`.
2. Volume calls: the original narrows both volume arguments (`sll 16; sra 16; move`) on every call, so
   `func_8008B750(s16, s16)` and `func_8008BFB0(s32, s16, s16)` as file overrides (the second as in
   80079B10.c, T-12902; the first got a `MAIN_API_OVERRIDE_` guard). 219 to 213.
3. `id = D_80125C58->dec.rectid ? 0 : 1;` (the sample's ternary). The `id = 1; if (rectid) id = 0;`
   form let uopt move `id = 1` to the loop entry and latch, which made `id` live across calls in s2.
   With the ternary `id` stays in v0 and `frames` takes s2. 213 to 206.
4. `func_8009AD00((u8 *)&D_800E8C70 + D_8011ECA0 * 0x14);` (GsOT byte offset, as in 80043510.c). The
   `s32 *` + `* 5` form numbers the multiply chain one ugen register higher (`sll t6,t5,2`), which shifts
   every later ring temporary by one. 206 to 100.
5. `start = D_800E6280.unk_10F4;` before the limit: the original loads the global into t7 and arg1 into
   t8 (ugen order start, then limit). 100 to 94.
6. `for (vol = 0x7F; vol > 0; vol -= 10)` instead of the do/while: same code, one more uopt basic block
   (see below). 94 to 59.
7. The empty test `if (D_800B58F0) {}` after `func_8009C5E0(1)` (FAKE): two more blocks inside the loop.
   59 to 58; all remaining rows were stack offsets.
8. `limit` as an expression, `D_800B58F0 < arg1 - 3 && frames < arg1 - 0xF`: uopt hoists `arg1 - 3` (the
   first operand is evaluated on every iteration; the second is not, so `arg1 - 15` stays in the loop)
   and spills the temporary to sp+0x48, the original's slot. 58 to 50 (declaring the four scalars
   first gave 54 on the way).
9. Frame: declared order `frames, vol, disp, draw, i, id, pad[5], file, pad2, start` (pads FAKE). 50 to 0.

## The allocation mechanism (why steps 6 and 7 matter)

regdiff's R9 trace (`build/scratch/regdiff/func_80056BA8/draft.log`, SAVE/COST lines) gives the savings
(priority times the D column) and the colour cost of each range. A new callee-saved register costs
`0.25 * number_of_basic_blocks`, plus `F/10` per use that has another precolour (argument register).

- With 75 blocks (cost 18.75): `&draw` saves 21 against 18.75 + 2.2 = 20.95 and takes s3; `&disp` (22
  against 21.05) takes s4; the arg0 piece (lr 148, priority 2, colour cost 1.1) then shares s3. The
  original has only `&disp` in s3 and reloads arg0 from its home before each CdSearchFile.
- With 76 blocks (for-loop vol): `&draw` costs 21.2 and fails, `&disp` takes s3, but the arg0 piece is
  coloured after `&disp` (equal priority 2.0, the piece has a later lr) and shares s3: 9 rows left
  (measured as N0 below, with the other changes in place).
- With 78 blocks (plus the empty test inside the loop): `&disp`'s range spans one more D step (priority
  22/12 = 1.83), so the arg0 pieces are coloured first, need a new register (cost 20.6 against savings 10
  or 11) and fail; `&disp` then takes s3 at 21.8 < 22. This is the original's allocation.
- The window is narrow: `&disp` must still pay for a new register (22 > 19 + 0.25k + 2.3), so the
  function can have at most 78 blocks.

## Measured negatives (do not repeat)

- Without the empty test (N0), or with the exit test spelled as `||`, as two separate ifs, as an if/else
  with an empty then-arm, as `goto end;` with a label, or as an `if (...) { ... } else break;` body
  (G1): 9 rows each, all 76 blocks (arg0 in s3). Nested if with `continue` (N2): 54 rows, code changes.
- do/while for the main loop (`do { ... } while (!(unk_F88 & 0x860))`): one block fewer (74).
- `while (1)` for `for (;;)`: no change. `if (i++ < 0x10000) continue; return 0;` in the retry loop:
  one more block, but outside `&disp`'s range, so it acts like the vol for-loop.
- Two empty tests (E2, measured before steps 8 and 9): 189 rows (allocation flips again).
- `limit` as a declared local: home inside the declared area (0x50/0x60), not at sp+0x48.

## FAKEs left (all name T-25118)

- `s32 pad[5]` and `s32 pad2`: measured frame homes. The original declares about six more words of
  locals than this body (two above `disp`, seven between `draw` and `file` with `i`/`id`, one between
  `file` and `start`); their names and uses are unknown. Promoted scalars (`frames`, `vol`, `i`, `id`)
  still own a slot each, so their position among those slots is not observable.
- `if (D_800B58F0) {}`: the two blocks. A real source probably had more control flow here whose code
  was removed (dead stores to the unused locals would do it), but no candidate was found.

## Next hypothesis for removing the FAKEs

Try a dead store under a condition inside the loop (for example a status local set on the pad-abort or
end-frame path and never read), which would give empty blocks and one of the unused frame slots at the
same time; check the block count with the COST line of `frames` (cost = 0.25 * blocks).
