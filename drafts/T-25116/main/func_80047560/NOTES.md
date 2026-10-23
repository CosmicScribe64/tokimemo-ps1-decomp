# T-25116: main func_80047560 (352 bytes): matched

Result: default C in `src/main/80047550.c`, `funcdiff --resolve --version default` MATCH (352 bytes),
no `INCLUDE_ASM` left for the function. About 35 minutes from the first measurement to the match.

## Start state

T-24303 guard, 22 diff lines (11 words). It used `*(s8 *)&D_801255DC` in the divisor (T-25058 flagged
this: the C reads a signed byte, the code reads `lbu`) and `off = (q << 16) >> 16` as a second
narrowing of the quotient. The 11 words were the ugen temporaries of the two narrowings plus 4
instructions out of place:

```
original  sll t1,a1 / sra t2 / sll v1,a1 / addiu t3,t2,0xA0 / sra t0,v1 / sh t3,+20
guard     sll t0,a1 / sra t1 / addiu t2 / sll v1,a1 / sh t2,+20 / sra t3,v1
```

## What the target says

- The second narrowing is a uopt-coloured value: `sll v1,a1,16; sra t0,v1,16`, every later use reads t0,
  and `move v1,t0` ends up in the last delay slot (ugen writes the value to its register, as1
  propagates the copy and sinks the move). The draft makes the same shape; only the order differs.
- Ring order (ugen hands out t6..t9, t0..t5): t0 for the second narrowing, then t1..t3 for the first
  store. So ugen emits the second narrowing before the `unk_014[0]` chain, while the `unk_014[0]` store
  still comes first. In the guard, uopt forwards `off` to its first use (the `unk_014[1]` line), so
  the narrowing comes after the first chain. Moving the `off =` line up changes nothing (forwarded).
- as1 order (`as1 -R -xbbdbg 8` trace, `drafts/T-24900-as1` scripts): with the narrowing computed on
  the same line as the first store, the two chains tie at aftercycles 16 and the first chain's
  `addiu` wins on besttime (2 words left, variant v37). The original needs the narrowing on an
  earlier source line: then the tie is decided by line and gives the original's interleaving.

## Steps (whole-unit compiles, `try.py`; rows = differing words)

| variant | change | words |
| --- | --- | --- |
| v00 | T-24303 guard | 11 |
| v13 | `x` stored to `D_801255E0` before the first store | 17 (registers right, store order wrong) |
| v09 | `if (x) {}` before the first store | 10 (ring right; block split halves q's priority) |
| v37 | `+ ((u32)off < 0)` in the first store (late-folded zero) | 2 (ring right, as1 tie) |
| v48 | `off = ...; q = (s16)q; unk_014[0] = q + 0xA0;` | 6 (as1 order right; q prio 5 beats off 4: q v1, off a1) |
| v52 | `off = ...; unk_014[0] = (s16)q + 0xA0; q = 0;` | **0** |
| w01 | v52 with the plain divisor `(s32)(D_801255DC + 0x100)` | 32 |
| w03 | v52 with `(s32)(*(u8 *)&D_801255DC + 0x100)` | **0** |
| g3 | final form without `q = 0;` | 85-word function (narrowings merged) |
| c1 | `s16 off = q` (no shift form), no x/y locals | **0** |
| f2 / f4 | `for` loop, `s32 i`, `sum = 0` before / in the init | 2 / **0** (u32 i in a `for`: 101 words) |

Mechanism of the match: the dead store `q = 0;` redefines `off`'s operand, so uopt cannot forward
`off` to its uses and computes the narrowing on the `off = q;` line. That gives the ring order (t0
first) and an earlier line for the narrowing than for the first store, which as1 needs.

## Final source and FAKEs

`f4.c` is the committed body. Two FAKEs, both marked with T-25116:
- `*(u8 *)&D_801255DC` in the divisor: a view of the u8 global with its own type, so the value is the
  same as the plain read (this fixes the T-25058 semantic problem of the old s8 view). Control w01: 32 words.
- `q = 0;` after the first store: dead. Control g3: the two narrowings merge.

Negatives added: empty tests on `off` itself (`if (off)`, `off == 1`, `off > 3`, `off != 0x7FFF`), dead
stores to globals before the first store, `i = off`, `sum = off`, in-place narrowing of `off`
(`off = q << 16; off >>= 16` and others), assignment of `off` before the first store: all identical to
v00 (removed or forwarded before uopt places the value). Block splits after q (`if (q) {}`,
`if (D_801255DC) {}`): 16 words. Store-back of a promoted `D_801255E0`: narrowed on assignment and
stored back after `D_801255DC`, not the original. Multi-line layouts of the first store: no change.

## Files

- `try.py`: whole-unit scorer, two compiles at a time (`TOKIMEMO_SCRATCH=<dir> tools/docker.sh python3
  drafts/T-25116/main/func_80047560/try.py /scratch/out BODY.c ...`; writes the original's disassembly to
  the scratch dir, not to the repo).
- `lst.py`: ugen listing (`cc -S`) of a body as a snippet. `mkfull.py`: full file for `regdiff --src`.
- `v/`: the bodies in the table.
