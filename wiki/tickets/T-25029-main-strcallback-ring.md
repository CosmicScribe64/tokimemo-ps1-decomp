---
id: T-25029
title: MAIN src/main/800563F0.c strCallback copy-ring diagnosis
status: In Progress
assignee: Codex
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25001-class-c-main-sndisp-strcallback]]", "[[tickets/T-25010-class-cd-main-80065964-8004a1a0-strcallback]]"]
---

## Goal

Diagnose the extra ugen temporary consumed at strCallback's RECT copy using faithful whole-TU pre/post-pass evidence. At most three new trace-supported source tests. Preserve callback ABI/behavior; no repeated casts/width/declaration/ternary grids or build-rule changes. Exclusive file src/main/800563F0.c; src/main/8004F870.c released. Root owns integration.

## Acceptance criteria

- [ ] Literal default-C linked proof before a match claim.
- [ ] Root reviews and executable/declaration integration gates before Done.

## Notes

Starting pin624541ad3, one Sol worker, no consultation or additional workers. T-25001/10 report target second copy wordt1 and else-address LUI t2, build one ring slot high.

## Mechanism checkpoint

Whole-TU guarded baseline160target/160built words, aligned148/printed150;75 instructions differ only in registers. Recording wrapper and logging-only evaluator are faithful to normal compilation; same-input entire objects match SHA256071fba1b4e008445993d119f60634a71859ce3a11eddb2a8fc2a99fd111d6c93.

Exact optimized input is8-byte Umov, destination alignment32/source16, source pointer+28. Normal shim/raw ugen callback binasm agree. Offset-zero, source alignment8/32 and copy-length4 private input controls retain the same scratch-ring reservation. Existing T-24067 logging diagnostic confirms this callback uses a reused eval_mov frame slot+232 value10000; non-NULL destination skips its assignment, then chooses double dummy allocations t0/t1 before second copy wordt2. Target is consistent with the single-burn counterpart (second wordt1, following LUI t2). Named-helper last candidate writer is f_loadstore:L4278FC saved gp10000; slot does not change during address evaluation.

No faithful new source hypothesis follows; zero source controls executed. No cast/type/return or build-rule change, source sweep, extra worker or Astra. Audit, limitations and exact blocker recorded in drafts/T-25029/README.md. Raw compiler reference/ucode/binasm/logs remain ignored under build/T-25029/. Ticket remains In Progress pending root review; no match claimed.

## Consultation closure

Prior exact callback Astra result only covered sentinel bypass; read and reused, not confused with this mechanism. Root-authorized read-only Astra follow-up reviewed the exact reused-slot packet while Sol idled. Required predecessor branches precede the call/scalar operations; next required branch follows Umov. Reference f_jump predicate-before-emitter ordering supplies no justified pre-copy small-value branch-helper route preserving the existing CFG. Zero new source probes recommended or run. Findings and record ordering preserved in the draft README. Selective candidate-writer caveat retained; no universal impossibility claim. Production unchanged, ticket remains In Progress for root review/reassignment.
