# ENDING sprite scene checkpoint

T-24047 owns only `src/ovl/ENDING/80134D20.c`, pinned at main `4496b0dc7`.
There are **zero new verified C bytes**. The production function keeps its ASM
fallback. `body.c` is the readable guarded reconstruction; both alias and width
allocation devices carry FAKE comments.

Run in the existing Docker image, from the repository root:

```
python3 drafts/T-24047/replay.py
python3 drafts/T-24047/replay.py --case12 --out build/t24047-case12-replay
```

The first command compares the entire TU with the archived body installed in a
private source copy. It writes the result and diff under `build/` and never edits
production source. Errors exit nonzero. Successful scoring is a diagnostic,
not a linked-byte match proof.

The preferred source has the original 160-byte frame and both 28-byte table
homes at `sp+0x7C`/`sp+0x60`. First two initialization loops and all case bodies
agree. It has 804 of 805 instructions, aligned cost 43, with two residual causes:

- Constants 128 and 499 occupy exchanged `a3`/`t0` registers in the final grid
  loop. Four instruction words differ.
- AS1 moves the first two default-arm script-address LUIs into switch dispatch.
  This removes one instruction overall and changes the case-exit branch offsets.
  These are the default arm's addresses, not the common continuation's globals.

The `--case12` diagnostic adds an explicit case sharing the default body. This
prevents the speculative address motion and has 805 instructions with five
differing words, but changes the bound from 12 to 13 and adds a jump-table entry.
It is not an accepted alternative or a matching claim.

`experiments.json` retains the bounded probe scores, each explicitly marked as
not a matching claim. `recovery-notes.md` records the static recovery findings;
its initial type hypotheses are superseded by measured ticket evidence. Large
native AS1 traces, objects and dumps stay ignored under `build/t24047/`. Tracing
used a scratch wrapper to add native diagnostics after the unchanged frame pass;
stock text, data, rodata and relocation sections were verified identical.

The focused continuation added 14 negative measurements to the ledger. Changing
default loads to integer-address or volatile views worsens output. Distinct
default-cell symbol names produce the same residual, so no matrix declaration
change is supported. Explicit outer range checks add another range check.
Palette casts and an early palette-store move are neutral; named constants,
including register locals, expand the frame. The preferred body is unchanged.

Eleven further trace-led probes are also negative. AS1's first block schedule
leaves the selector load stall empty; its later cross-block pass imports the
default-arm LUIs. Whole-switch/default-load line grouping does not stop it.
Stock uopt listings reach target allocation/reemission before the `wrapper_ecvt`
assertion, showing adjacent ranges 528/529 assigned colours 6/7. They do not
provide the priorities/costs needed to distinguish allocation causes. The next
diagnostic needs the instrumented uopt described in
`wiki/uopt-register-allocation.md`; `regdiff` reports that binary unavailable.
No compiler rebuild or production change was made.
