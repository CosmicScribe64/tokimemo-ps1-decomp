# T-25311: ENDING func_80134270 (second pass)

Result: matched, default C build, 1848 bytes (476 words). Source: `src/ovl/ENDING/80133C10.c`.

## Start state

The first pass (T-25135) was stopped by the end of wave 9 and left no notes and no commit, only an
uncommitted `NON_MATCHING` body in the worktree. That body used the absorbed global `D_800E71DF`
(now `D_800E6280.unk_F5F`); after that one fix it built and measured 5 rows, all at the switch
join before the last call:

    expected                      built
    or   v0,zero,zero
    sb   v0,%lo(D_800E62B7)       sb zero,0x36
    sb   v0,%lo(D_800E62B6)       sb zero,0x37
    sb   zero,%lo(D_800E62B8)     sb zero,0x38   (jal delay slot)

## What closed it

1. Store order. `unk_036 = unk_037 = 0;` puts 037 before 036. The element chain
   `(&D_800E6280.unk_036)[0] = (&D_800E6280.unk_036)[1] = (&D_800E6280.unk_036)[2] = 0;`
   (the TACO func_8013B2A0 idiom, T-19030) gives the full order 037, 036, then 038 at the end
   of the block, and the original's sharing: 037 and 036 share the chain's hidden temporary,
   038 gets its own constant. Still 5 rows, all `sb zero`.
2. The zero in `$v0`. Traced the stages with scratch shims (copies of tools/ under
   TOKIMEMO_SCRATCH, no tool change): uopt's output already keeps the chain in registers, and
   ugen emits `move v1,zero; andi v0,v1,0xff; sb v0; sb v0; sb v1`. as1 folds all three
   stores to `sb zero` and deletes both definitions. Patching the as1 input showed that as1
   also propagates a plain `move v0,zero`, but not `li v0,0`. With the function `s32` and no
   `return` (cheatsheet row "a move/andi/li into v0 near the end", T-11590), `$v0` is live at
   the exit, as1 keeps `or v0,zero,zero` and the two stores of `v0`: MATCH. The `jal` between
   the stores and the exit does not stop it.

Both levers are needed: the s32 type with a struct-field chain stays at 5 rows, and the element
chain in a `void` function stays at 5 rows. Both are marked FAKE.

Cleanups after the match: the local `ED_A` macro was expanded into the file's own
`((DisplayActor44 *)D_80120650)[k]` form, and `D_80120693 = 0x11` became the record field
`[0].unk_43` (same bytes). The `*(u16 *)&D_801217D0[i + 4].unk_08` view is required (425 rows
without it) and is marked FAKE.

## Negatives (measured, no change to the 5 rows)

- Plain chains in any order, the three-target plain chain (stores 038, 037, 036), a read-back
  (`unk_037 = 0; unk_036 = unk_037;`), `i = 0` reused as the value, `(s16)0`, s8 views of the
  chain elements, `*= 0` folds.
- Global copy propagation off for the procedure (`OPTN 405 0`, as tools/cvt_pass.py emits for
  unsigned-global switch temporaries): identical output.
- The object's function table `D_8013C5BC` (which names func_80134270) defined in C in the
  same unit (the T-25150 jr back edge): identical output. This object does not need its data
  island for this function.
