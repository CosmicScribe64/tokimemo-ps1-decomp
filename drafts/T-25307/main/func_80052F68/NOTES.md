# T-25307 main func_80052F68: matched (1200 bytes, default C)

Second pass after T-25101. Result: `funcdiff --resolve --version default` MATCH (linked function bytes equal
target, 1200 bytes); the C is in `src/main/8004F870.c`, no guard, no `INCLUDE_ASM`. About 70 minutes.

## Start state

The T-25101 body (`drafts/T-25101/main/func_80052F68/body.c`): 69 rows (compared NON_MATCHING C), 299/300
words. Its first divergence: the constant 10 in `v1` where the target has `a0`, and the loop-2 counters in
`a0`/`v1` where the target has `v1`/`v0`. Its next hypothesis was a traced uopt run.

## What was measured

- The traced uopt exists in the session scratchpad (`$TOKIMEMO_SCRATCH/r9-uopt/variants/trace`); regdiff runs
  with it. A scratch copy of the generated trace C with one more field printed `interf=` (the range's
  interference count, `lr + 36`) and `class=` (`0x1001e600`, the int register-class size). `isconstrained`
  in the recomp: a leaf range is constrained when `interf >= class`; class is 22 here.
- First pass body: constrained ranges m (interf 22, priority 136.8, gets v0), the constant 10 (27, 26.25, v1),
  arg1 (38). The loop-2 outer counter i is unconstrained (20), so it is coloured after 10 and gets a0.
- The target order needs i constrained with a priority above 10 (m v0, i v1, 10 a0), then the loop-3 sum in
  v0 and the loop-3 inner counter in v1.
- The `li a0,10` in loop 1's preheader is as1's global filler (the ugen listing has it after the clamp), as
  the first pass said.

## What changed and why (rows)

| Change | Rows |
| --- | ---: |
| T-25101 body | 69 |
| loop-3 inner counter is i (was j): i's range spans loop 3, interf 26, constrained, priority 90 | 28 |
| stress in m (was sum), and one variable v for both arms (the H arm takes t1 like the L arm) | 4 |
| loop-1 counter is m too, stress back in sum, no `pv` FAKE | 0 |

The last 4 rows were the order of two increments in loop 1 (`addiu a2,a2,8` before `addiu v1,v1,4` in the
target). as1 keeps ugen's order (checked by swapping the two lines in the listing and reassembling), and uopt
emits the increments and colours unconstrained ranges both in bit order. So the target's val pointer has the
lower bit but is kept off v1 and a0 by interference. That happens when loop 1 and loop 3 index `val` with the
same counter: `ixa(val, m * 2)` is then one expression, the strength-reduced pointer is one range over loops 1,
3 and 4, it interferes with i and 10 and gets a2 everywhere (the target's val pointer is a2 in every loop).

## Negatives added

- A separate loop-1 counter `k` shared with loop 3 outer (`k5`): loop 1 exact, but uopt then converts the
  loop-2 inner test to `bne v0,s0` (EQ_INEQ at the inner latch). The inner test stays `slti` only while m is
  also the loop-3 outer counter. Test spellings (`<= 6`, `7 > m`, `++m`, `m += 1`) change nothing.
- m as the loop-1 counter and also the stress variable: loop 1 stays rolled (275 rows).
- Counter grid (stress, loop-3 outer, loop-3 inner, loop-4, pv; 80 bodies): nothing below 4 rows without the
  shared loop-1 counter.
- `like`/`dislike` locals for the masks: same interference counts, frame moves.
- Removing one of the unused scalars shrinks the frame to 0x78 (4 rows), two move `val` (36 rows).

## Left

Nothing. FAKE: the unused locals `j`, `k`, `x` (stack homes for the 0x80 frame). The `m == 7` arm is dead
code that the original also has. The table bases `D_800B4348`, `D_800B4354` and `D_800B4360` are in
`config/symbol_addrs_main.txt`; the C file includes the three new data pieces after `D_800B41B0`.
`--version c` prints DIFF with zero instruction rows (the whole-unit NON_MATCHING compile does not split the
data pieces), the known tool artefact; the default build and the sha1 are the proof.
