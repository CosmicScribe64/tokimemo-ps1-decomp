# T-25051 TACO func_8013B460

Research checkpoint requested by the user on 2026-10-22. The production
function in src/ovl/TACO/8013B170.c is unchanged and remains guarded.
No candidate matched, no literal-C proof was claimed, and this is not an
external-blocker finding. Ticket T-25051 is In Review.

The fresh baseline has 332 words (1328 bytes) and resolved aligned cost 4:
one y3 halfword store per quad appears before the UV byte stores instead
of between the following mode-address lui and lw. All other instructions
match. Read the ticket for the complete measured ledger and prior-ticket
exclusions before running more controls.

## Findings

- A separate s16 y3 local delays the store past the UV writes, but the
  coordinate core takes v0; the FF/F0 constants and a saved division result
  change registers. It remains 332 words, cost 44. The store is also still
  one instruction before the required position.
- Actual full-TU uopt tracing assigns the local priority 2, ahead of the
  constants at priority 1. Actual stock as1 tracing gives y3 and UV stores
  equal aftercycles 6, each preceding the following mode load.
- Reading the reconstructed stock as1 local-selection routine func_433ed0
  shows its ordinary tie path prefers earliest readiness, larger path
  length/latency, then earlier original instruction position (node +48),
  then source line (node +12). This explains why more formatting controls
  are not a promising next step. No assembler input was rewritten.
- Empty tests change the control-flow graph and disrupt cached expressions
  and saved registers. Branch-free zero reads collapse to the baseline.
- Narrowing/mask boundaries spend rotating temporaries but leave the
  direct store early. Applied to a named temporary, they retain its v0
  core allocation. Reversible XOR/shift encodings retain extra code.
- Local arrays add stack traffic. Const input views do not remove the
  relevant alias ordering. A volatile view of y3 or u0 alone leaves cost 4;
  marking all UV writes volatile changes their order and gives cost 32.

## Reproduction

These are source-only generators. They read the existing guarded source
and private expected object, preprocess the whole translation unit, alter
only func_8013B460, compile with the normal IDO 5.3 pipeline, and write
ignored scratch C/objects/diffs under build/t25051. Nothing edits production
source, headers, flags, or compiler passes. Run from the repository root:

```sh
tools/docker.sh python3 drafts/T-25051/baseline.py
tools/docker.sh python3 drafts/T-25051/temp16.py
```

The remaining scripts preserve rejected controls by mechanism; they are
not recommendations to repeat them. Printed columns are variant name,
resolved word count, and aligned diff cost, not positional row counts or
literal-link proof. The s64 local attempt was a CFE declaration error and
is documented but disabled in life.py.

Captured private traces lived under build/t25051/cap-base and cap-temp16,
using the copying capture scripts from T-25037 and the existing T-24047
uopt tracer. These traces, generated translation units, expected objects,
game inputs, and compiler reconstruction binaries are not committed.
