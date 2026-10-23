# T-25403: GEKO func_80142C9C (926 words), not matched

Wave 11 (umbrella T-25400), worktree w11-GEKO-80142760-p1.

## Pass 1 (2026-10-23, about 115 minutes)

### Start state

No NOTES.md existed in any earlier draft folder. The best body was the diman8 checkpoint
`drafts/T-11900/GEKO/func_80142C9C/body.c` (built on the T-24503 draft). Measured here with
`funcdiff --resolve --version c` (full TU, guarded): **159 rows** (912/913 words). The source stays
`INCLUDE_ASM`; `body.c` here is that checkpoint, unchanged. `body_loop2_exact.c` is the checkpoint plus
a FAKE `if (ids.v) {}` as the first statement of the second loop body (231 rows: the second loop matches
exactly, everything after it is off by the reuse described below).

### First divergence (same place as T-11900)

Everything before the second (weighted pick) loop matches. The original keeps `&ids` in `$t4` for the
second loop only (`addiu t4,sp,0x34` in the preheader, `addu t7,t4,v1; lbu v0,0(t7)` in the body) and
rebuilds it after the loop in ugen temporaries (`addiu t6,sp,0x34; addu v0,a3,t6`, `addiu t7,sp,0x41`
for `&ids.v[13]`). The checkpoint never hoists; the FAKE variant hoists, then reuses `$t4` after the loop
(`addu v0,t4,a3`, `addiu t8,t4,13`): one word short, and the ugen ring of the switch is one slot off.

### Mechanism (traced uopt; new diagnostic, see diag/)

`diag/live_patch.py` adds two print points to the traced uopt (`R9LIVE`/`R9REACH`: the blocks of every
range after each `R9SAVE`; `R9ADJ`: every successor that `split()` tests while it grows a piece, with
shared, regsleft and numintf). With the IDO sources (uoptreg1.c, uoptreg2.c split/addadjacents,
compute_save in the recomp) this explains the whole picture:

- `&ids` is one register-allocation constant (bit 514 in the FAKE variant): uopt keys an LDA constant by
  its symbol, not by offset, and gives it a unit at every reference that needs a register: the struct
  copy (bb0), the loop 1 read (bb168), the loop 2 read, and both post-loop references (`ixa(ids, sel)` and
  the `&ids.v[13]` compare, which uopt rewrites to `equ(add(q, -13), lda ids)`). Every spelling tried
  (`ids.v[i]`, `*(ids.v + i)`, integer adds, `((u8 *)&ids)[i]`, plain `u8 ids[21]`) gives the same units.
- The range runs from bb0 to the last post-loop use and crosses loop 1, where every caller register is
  taken, so it is split. Pieces in order: {bb0..4} (stops at the first call node), {bb168} alone (loop 1:
  numintf 38 against regsleft 9, every successor rejected), then the remainder. The remainder has to pay
  reloads at bb169 and bb170 (the two successors of bb168, freq 10 each) and bb5: 21.
- Remainder priority = (uses - 21) / n. Checkpoint: 10 (loop 2) + 1 + 1 (post-loop) - 21 < 0, so the
  constant is never coloured (that is the checkpoint's code). FAKE: +10, priority 0.02 > 0, split again.
- `split()` seeds at the first unit with references in graph-list order (a DFS that takes the
  fall-through successor first), which is the loop 2 body. The piece grows along successors through the
  break block and the latch exit into the post-loop blocks: at the post-loop head, shared 1, regsleft 10,
  numintf 13, so the heuristic `2*regsleft >= numintf + shared` passes easily. The reload at the loop
  header is free (`canmoverlod`), so the piece gets priority 3.67 and `$t4`, post-loop included.
- Growth stops only at a node that is deadout for the piece, ends in a call (constrained path) or in
  `ijp`, or at a successor that is not in the remainder. None applies at the loop 2 exits.

So with this CFG IDO 5.3 cannot give the original's split ({loop 2} coloured, post-loop not): the
post-loop references would have to be outside the remainder when the loop 2 piece grows.

### Measured (rows = funcdiff --resolve --version c, full TU)

| variant | rows | note |
|---|---:|---|
| checkpoint (body.c) | 159 | start, best |
| FAKE `if (ids.v) {}` as first loop-2 body statement | 251 | hoist, but `addu` lands after `lbu` of F5F |
| FAKE `if (ids.v) {}` before `if (flags[i] != 0)` (body_loop2_exact.c) | 231 | loop 2 exact, post-loop reuses `$t4` |
| `if (flags[i] == 0) continue;` | 159 | identical code |
| continue + loop-2 FAKE (top or body) | 303 / 329 | post-loop now first in DFS: post piece seeded first, priority (2-1)/3 = 0.33, coloured `$v1`; the extra blocks also drop `r` (11/4 -> 11/5) below the `lui` constant, so loop 1's constants shift |
| `q = ids.v;` before loop 2 (also `register`, back-edge reassignments, for-init/increment forms, `q = &ids.v[i]` in the header) | 159 | copy propagation substitutes the address every time |
| dead `u8 *` parameter reused as the base | 1414 | still propagated, plus `sw a0` home store |
| integer no-ops on the base (`| 0`, `^ 0`, `& -1`, `* 1`, casts) | 159 | folded by cfe |
| alias through another table (`banned.v - 0x18`) in loop 2 or post-loop | 159 / 165 | separate constant, but keyed by `banned`: the -0x18 stays in the load (`addiu t6,sp,0x4C ... lbu -24(v0)`) |
| compare spellings `ids.v + 13`, `(u8 *)&ids + 13`, `(u8 *)&ids.v[13]` | = | same unit at the compare |
| double reference in one FAKE test (`ids.v == ids.v`, `ids.v + (s32)ids.v`, ...) | 165-276 | folded or emits code |
| global copy propagation off (T-25301 FAKE `register s32 zc = D_800E71DF;` first) | 166 | loop 1 still matches; first divergence at the loop-2 preheader: `sel` in `$t4`, the constant 19 gets `$a3` (`multu`) because `p` stays a variable |
| zcopy off + separate `register u8 *` base | 555 | the register local takes a frame home (+8) |
| zcopy off + `D_800E71DF * 0x13` in loop 2 | 281 | now `&weights` is the hoisted `$t4` constant (same mechanism, wrong table) |
| zcopy off + `q = ids.v;` before loop 2 as the base, post-loop without `q` | 443 | `q` is a variable now, but its range (1 def + 1 use at freq 10 over 4) has priority 2.75: coloured 8th (`$a3`), ahead of the constants; the original needs it last (below 1.417, into the `$t4` that `year` frees) |

### Negatives added

Everything in the table except the first line. In particular: no spelling of the loop-2 read, the
post-loop pointer or the compare separates the `&ids` constant; a pointer copy of `&ids` is always
propagated (zcopy on); `continue` cannot be combined with more loop-2 blocks (r's priority).

### Next hypothesis

The post-loop references must leave the remainder before the loop 2 piece grows, or the piece must stop
at the loop exits. Two concrete leads:

1. Global copy propagation off (the T-25301 `register` FAKE) with the loop-2 base as a real variable:
   with zcopy off, loop 1 still matches (166 rows). Reuse a declared pointer that already has a frame
   slot (`q` or `p`) as the loop-2 base so the frame does not grow, give the post-loop pointer its own
   expression, and fix the constant-19 `multu` (the weights row pointer is a variable under zcopy off;
   try indexing `weights` directly in loop 2, or reading F5F through a different view there).
2. A source shape whose graph order visits the post-loop before the loop 2 body without adding blocks
   to loop 2, together with a reason for the post-loop piece to stay uncoloured (for example a later
   reference after a call, so the piece holds a call node and a caller register costs more than its
   priority). This needs two more loop-2 references without new blocks; not found yet.

Tools for both: `diag/live_patch.py` (build recipe in its docstring), `diag/units.py` (pieces of the
`&ids` constant), `diag/graph.py` (flow graph from `draft.o.trace`).
