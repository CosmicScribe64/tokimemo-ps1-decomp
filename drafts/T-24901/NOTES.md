# T-24901 KANGEI func_8013852C - notes

Target: `KANGEI:func_8013852C` (180 B), owned file `src/ovl/KANGEI/80137B90.c`. Class A, guarded, **2 rows**
(one swapped pair), no match.

## Current diff (unchanged at the end)

```
  -@row 35 01837021  ; addu t6,t4,v1
  +@row 35 3c018014  ; lui at,0x0
  -@row 36 3c018014  ; lui at,0x0
  +@row 36 01837021  ; addu t6,t4,v1
```

Target has `addu; lui at,%hi(D_80139ADC); sh`, the build has `lui at; addu; sh`.

## What the as1 scheduler actually does (new evidence this run)

Captured the unit's as1 input and `as1 -R -xbbdbg 8` trace with the T-24900 scripts
(`drafts/T-24900-as1/` copied to `build/as1tr/`, `AS1CAP=build/capKANGEI`, `build/as1tr/s.py` for a
post-ugen `cc -S` listing). The trace prints as1's per-block dependency graph and every pick.

The block (line 3307/3308) is nodes 21..0; the two nodes in question:

```
Node   5: inst 01837021 (addu)   before 1, aftercycles 4
Node   4: inst 3c010000 (lui at, reloc 282 = D_80139ADC)  before 2, aftercycles 4, afternodes: 2/1 3/1
...
  node 4 (INST 21), time = 16, aftercycles = 4, latency = 1, besttime = 15
  node 5 (INST 20), time = 16, aftercycles = 4, latency = 1, besttime = 16
Picking node 4 (INST 21), at 9dc, best_addr = 99c
Picking node 5 (INST 20), at 9e0, best_addr = 99c
```

So it is as1's local (per-block) list scheduler, not the cross-block mover: it takes the ready node with the
smallest `besttime` (earliest ready time). The `lui $at` node is ready at 15; the `addu` is ready at 16 because
its only in-block predecessor `subu t4,t2,v0` is scheduled at 14 (with its latency). The `$at` node's two
dependents are the `sh` and the later `lui $at,%hi(D_8013A2AC)` (resource chain on `$at`) - it has to be emitted
before both, but its readiness is one cycle earlier.

Consequence: no statement order, grouping, or store-order change can move it, because the instruction stream is
already identical. Only a change to the *DAG* (an op as1 deletes still contributes a node, T-24202) or to how
soon `addu` becomes ready can flip it.

Also: as1 enforces no source-line order between the ALU op and the store expansion - `tmp = sum; store tmp;`
(uopt promotes `tmp` away, the two statements keep separate `.loc`s) still hoists the `lui` above the `addu`.
That kills the "attach the address materialization to a later line" idea empirically.

## Variants tried this run (all via `drafts/T-24900-as1/vr.py`, full-unit NON_MATCHING compile)

Score = funcdiff score; rows = diff lines.

| variant | score / rows |
| --- | --- |
| baseline | 4 / 2 rows |
| sum into `u16 tmp` on its own line, then store | 4 / 2 (same pair) |
| sum into `s32 tmp`, store `(u16)tmp` | 6 / 4 (same pair + `subu` t3/t4) |
| store through `*(s16 *)&D_80139ADC` (address view) | 4 / 2 (same pair) |
| `D_8013A2AC = (u8)year` (extra `andi` as1 deletes) | 4 / 2 (same pair) |
| `(u16)(s16)year` -> `(u16)(s16)(u8)year` | 8 / 6 |
| `(u16)(u8)((F<<2)-F)` (deleted mask on the product) | 22 / 11, len 46 |
| `(u16)((u8)(F<<2)-F)` | 26 / 13, len 46 |
| `(u16)((F<<2)-F & 0xFFFF)` | 12 / 8 |
| `(u16)((F<<2)-F+year)` (one mask dropped) | 12 / 8 |
| sum operands reversed (`year` first) | 12 / 8 |
| `D_800E6280.unk_F5F * 3` instead of `<<2` - `F` | 32 / 34 |
| `D_8013A2AC = year` before the sum store | 24 / 28 |
| `D_8013A2AC = year` after the call | 26 / 17, len 46 |
| `year` as `s16` | 96 / 50 |
| `f = D_800E6280.unk_F5F` local | 64 / 34 |

Nothing improves; the two-row pair is invariant under every d02-to-dag change the C can express cheaply.

## Next concrete hypothesis

The readiness gap is one cycle, produced by `subu` -> `addu` at time 14/16. To make `addu` ready at 15 the sum's
in-block chain must be one node shorter *without* changing the emitted instructions, i.e. an as1-deleted op must
sit **between `sll` and `subu`** (T-24202: "the deleted ops still shape the schedule"). Candidates not yet tried:

1. The product written as a `u16` expression that keeps a deletable `andi` *before* the `subu`
   (`(u16)((F<<2) & 0xFFFF) - F`-shaped; the `& 0xFFFF` after the `sll` is deleted, the `subu` moves one node
   later in the numbering).
2. A `move`-shaped deleted op (as1 folds `or rd,rs,zero`): `u32 g = F; ... g` with `g` promoted, so ugen emits a
   copy that as1 deletes between `sll` and `subu`.
3. Check the same block's trace for a *deleted* node in a matched sibling that has this store shape
   (`func_801385E0` is not a match candidate - `INCLUDE_ASM` - but `func_80138728`/`func_8013885C` write the same
   global; a matched function whose trace shows the `lui` after the ALU op would give the DAG shape directly).

If none of those move it, this is a build-step question (as1's readiness heuristic), not a source question; the
guard should stay at 2 rows.

## Repro

```
cp drafts/T-24900-as1/*.py build/as1tr/
AS1CAP=/work/build/capKANGEI tools/docker.sh python3 build/as1tr/cap.py src/ovl/KANGEI/80137B90.c /work/build/capKANGEI
tools/docker.sh python3 build/as1tr/s.py /work/build/capKANGEI/unit.c /work/build/sK   # post-ugen listing
# trace is in build/capKANGEI/bbdbg.log (grep -n "lineno 3307")
```
