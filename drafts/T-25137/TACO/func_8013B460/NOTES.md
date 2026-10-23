# T-25137: TACO func_8013B460 (1328 bytes), matched

Result: default-C match (`funcdiff --resolve --version default`: MATCH, 1328 linked bytes equal), no
INCLUDE_ASM left for the function. Commit `T-25137: match TACO:func_8013B460`.

## Start state

`src/ovl/TACO/8013B170.c` held the T-24200 guard (kept here as `start_guard.c`): 332 words, aligned
cost 4, 18 positional rows. In each quad the `sh` of `p->y3` sat directly after the y3 chain, before
the eight UV `sb`; the original has it after the `sb` and after `lui` of the AddPrim argument
(`lui t4; sh t3,0x22(s0); lw t4`). T-24200, T-24705, T-24905, T-25011 and T-25051 had ruled out
statement order, comma grouping, same-line forms, temporaries, casts, volatile and empty tests.

## What was measured

Stock `as1 -R -xbbdbg 8` on the captured binasm (T-24900 capture shim, `trace.py` here) prints the
DAG of the block and every pick. Mapping the printer and the selection routine in the recompiled as1
(`func_433ed0`, around L434340 of as1-list.c in the T-24047 scratchpad):

- node fields: +0 instruction, +4 relocation, +12 **lineno**, +20 INST (emission index), +42 before,
  +44 aftercycles, +48 hazard address (set only for users of a load).
- a pick takes the smallest start time, then the larger aftercycles, then the larger latency, then
  the smaller +48, then the **smaller lineno**; on a full tie the first node of the ready list wins.
- released nodes go to the front of the ready list. That is why the eight UV stores (one statement,
  released together by the last coordinate `lh`) come out reversed, v3 first.

In the baseline the y3 store (line N), the UV stores (N+1) and the AddPrim `lui` (N+2) all tie on
time 224, aftercycles 6, latency 1 and +48. The y3 store wins on its line. With the y3 store on the UV
line it still wins, by list order (it is released last, after `addiu t3`). A y3 temporary stored after
the UV statement (T-24905 v1) loses to the `sb` on line but beats the `lui`.

So the original's y3 store must carry a later line than the AddPrim call, while being emitted before
the call's `lw` (the memory edge keeps it in front of the load). Monotone source lines cannot give
that: statements take the line of their first token (a statement split over three lines, a call with
arguments on later lines and an assignment inside a call argument all kept the first line), and a
block moved to a `goto` site drops its own `.loc` and takes the jump's line (`goto` probes: 332 words,
y3 and UV on the goto line, cost 38 from a saved-register rotation). A one-trip `for` with the UV
and AddPrim in the increment keeps the loop (448).

`#line` probes on the preprocessed unit: y3 statement on line AddPrim+1, numbering restored after it.
First quad only: cost 2. Both quads: 332 words, cost 0. In the real file the same `#line` pairs give
the default-build MATCH.

## What changed

The guard is gone. Each quad has `#line <AddPrim line + 1>` before the y3 statement and
`#line <true next line>` after it, marked FAKE with this ticket. The pair is self-consistent if lines
above move (the y3 line stays above the AddPrim line); only the reported numbers of the rest of the
file shift.

Tool caveat: `funcdiff --version c` and `tools/funcdiff_verify.preprocess_draft` run `gcc -E -P`,
which drops the markers, so a NON_MATCHING-style compile of this file shows the old 4-cost baseline.
The default build (asm-processor, then IDO's own cpp) honours `#line`.

## Negatives added

- y3 statement split over lines, AddPrim arguments split over lines: lineno is the statement's
  first line (cost 4).
- y3 and UV inside the AddPrim argument (`AddPrim(ot, (p->y3 = ..., UV, p))`): one line, cost 8.
- `goto` to a y3 block placed after the function's last statement, with or without the UV statement
  in it: merged code takes the goto line, cost 38.
- `for (pad = 0; pad < 1; pad++, UV, AddPrim(...)) { p->y3 = ...; }`: loop kept, cost 448.

## Next hypothesis (if a reviewer rejects `#line`)

The original source layout is unknown. A front end or source layout that gives the y3 store a later
`.loc` than the AddPrim call is the only lever found; a loop-free C form that does this under IDO 5.3
cfe/uopt was not found.
