# T-24903 VALEN func_80132D0C — matched

`tools/docker.sh python3 tools/funcdiff.py --resolve VALEN:func_80132D0C` ->
`MATCH (linked function bytes equal target (892 bytes))`, default build (no guard).

## What it was

The last misplacement was as1's global delay-slot filler (wiki/as1-scheduler-rule.md): the
block `sel == 9` (bb 8, weight 4) fills its stall with the head `lui v0` of the dominated
`mode == 1` block (bb 9, weight 7, number 9) because (weight, number) tries bb 9 before the
dispatch bb 20 (weight 7, number 20). The original had that `lui` pinned, so the next
candidate — the switch dispatch `sll t3,a1,2` from bb 20 — filled the slot, and the third
`if` head kept its `nop`.

## The form that matched

```c
if (state->unk_F5F == 9) {
    mode = D_80134544;          /* volatile read, inside the ==9 arm */
    if (mode == 1) {
        D_80134498 = 3;
    }
}
mode = D_80134544;              /* second read, shared by the next two ifs */
if ((state->unk_F5F == 0) && (mode == 1) && (state->unk_1BC[0].unk_02 >= 0x46)) { ... }
if ((state->unk_F5F == 0) && (mode == 1) && (state->unk_1BC[0].unk_02 < 0x46)) { ... }
```

plus `extern volatile u8 D_80134544;` in `include/ovl/VALEN.h` (FAKE, T-24903).

Two things had to be true together:

1. The first read must be *textually inside* the `sel == 9` arm. The old `mode = D_80134544;`
   before the first `if` was sunk into bb 9 by uopt; with `volatile` it no longer sinks, so
   the load stayed above the `bne` (that is the 190-row "a volatile read in the first `if`
   only changes the allocation" result).
2. There must be a *second* source-level read for the following two `if`s. With a single
   read, `volatile` makes uopt keep one load in one register (13 rows), where the original
   has two loads (bb 9 and the second-if head) and shares the second one with the third
   `if`. Two separate statements give two accesses; each keeps `sched=1/1`, so neither
   `lui`/`lbu` pair is movable and the dispatch `sll` wins the slot.

## Variant scores (full-unit compile, `drafts/T-24900-as1/vr.py`)

| form | rows |
| --- | --- |
| base (single read, no volatile) | 7 (sll in third-if slot, `lui v0` in the ==9 slot) |
| A: volatile, `D_80134544 == 1` in the first `if`, second `mode =` read | 190 (right layout, mode+1 are ugen ring temps: `lbu t0`, `li at,1`) |
| B: volatile, first read direct, second via `*(u8 *)&D_80134544` | 192 |
| C: volatile, first read via plain `*(u8 *)&` view, second direct | 11 (first load hoisted above the `bne`) |
| D: no volatile, two reads | 7 |
| E: volatile, nested `if` + second read (chosen) | **0** |
| F: volatile, `((mode = D_80134544) == 1)` in the condition + second read | **0** |

A gives the right instruction layout, so the schedule fix is independent of the register
problem; the extra rows in A/B are uopt treating the compared byte as a temporary instead
of a coloured value. E and F differ only in how the first read is spelled; both match.

## Checks run

- `funcdiff --resolve` MATCH, default build (892 bytes, no `INCLUDE_ASM`).
- The header change touches two other readers of `D_80134544`; both still match:
  `VALEN:func_80133568` (same file) and `VALEN:func_80133B6C` (`src/ovl/VALEN/801339D0.c`).
- `tools/docker.sh python3 tools/check_headers.py` -> `headers OK`.
- `tools/migrate_globals.py --check`: only the pre-existing `src/ovl/ETC/80146400.c:565`
  `D_800E7384` use (not this change).
- Not run here (orchestrator): clean `rm -rf build && configure.py && ninja` 27/27.

## Left

Nothing on the function. The `volatile` is a FAKE (real declaration unknown); if the real
declaration is found, keep the split reads and re-check the delay slot.
