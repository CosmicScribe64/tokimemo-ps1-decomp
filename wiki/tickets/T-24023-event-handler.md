---
id: T-24023
title: Recover EVENT large handler
status: In Review
assignee: Astra hard-function lane
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-24020-astra-hard-functions]]"]
---

## Goal

Investigate `func_801103A0` (6,848 bytes) in exclusively reserved `src/ovl/EVENT/8010FE10.c`, after TEL.

## Acceptance criteria

- [x] Read T-15200 and T-17400 evidence, test a new hypothesis.
- [x] Preserve source provenance; no win claimed, unchanged unit SHA verified.
- [ ] Standards review before Done.

## Notes

Read T-15200 and T-17400. Use T-15200's 1930-row typed draft (the T-17400 redraft regresses to 3174). New hypothesis: structured outer girl dispatch changes uopt range splitting while preserving per-girl tests and the common tail. Compare a switch and explicit else-if chain, then inspect structure and allocation. One compiler maximum across this lane.

## Comments

### 2026-10-17 intermediate evidence

EVENT baseline SHA1 passes. T-15200 source remeasures at aligned cost 1618. An equivalent outer else-if chain improves to 1461; switch 2243, explicit local selector 1929. Adding a byte Boolean between the two saved result bytes reproduces the missing stack-byte gap (cost 1457 with assignment-expression spelling). Explicit repeated Boolean tests expose extra narrowing; three distinct Boolean locals lower score to 1362 but expand the frame from 48 to 56 bytes, so that is not a preferred reconstruction. Scalar alias views do not beat 1457. The allocator trace from an existing ARM64 diagnostic uopt binary identifies the girl range at priority 2.5, precoloured a0 rather than original v1; month range and selector-address range also compete in the wrong order. Testing argument conversions next. No compiler patches or production source edits.

### Correction during review

The intermediate else-if transformation incorrectly used a global substring replacement for terminal `goto block_448`, deleting nested jumps too. Every result derived from that candidate is invalidated, including the apparent 469 score. No production source was changed or progress claimed. Corrected the transformation to remove only the final jump of each arm, with per-arm and whole-function jump-count assertions. Rerunning dependent probes; only corrected results may be checkpointed. Old raw metrics are retained locally as `build/t24023/invalid-rewrite-results.jsonl` for audit, not as candidate evidence.

### Corrected checkpoint

Corrected replay: baseline 1618, else-if 1544, recovered byte gap 1540; signed call arguments plus the ordinary seasonal predicate 1062; combining that predicate with conditional-expression date Booleans 934; explicit selector byte views **214**. Both sides have 1712 instructions, the 48-byte frame and sp+0x2B/sp+0x29 saved bytes agree. Printed rows are 826, not 214. Source-only draft, not a match or guard candidate. Tail aggregate stores preserve 214; removing the unused var_at declaration also preserves it. The saved body marks both allocation-only byte views and the redundant gap local FAKE.

Preserved `drafts/T-24023/body.c`, corrected replay scripts, compact results and independent measurement. The remaining structural gap starts at original instruction 1280 (date Boolean materialization vs direct inequality/zero paths), plus one tail-load placement; several selector comparisons have exchanged operands. Detailed negative hypotheses and next-step criterion are in `drafts/T-24023/README.md`. No production C/header/tool/config changes. Review pending.

### Resume

User requests continued large-function work. Preserve checkpoint b312d1e24. New bounded hypothesis: register-qualified Boolean locals change front-end narrowing and value-number lifetime without changing conditional exits. If exhausted, proceed to the newly reserved MAIN dispatcher and GEKO hallway picker. Header edits require root coordination because another worker is extending EVENT.h.

### Register lifetime follow-up (2026-10-17)

Six new register-qualified Boolean forms did not beat 214. Explicit narrow/word Boolean blocks scored 1782/1008; logical and reused ternary Boolean forms 1000/1164. `register u8` alone stayed 214. Saved the script and results in `drafts/T-24023/resume/`. Preferred body remains byte-for-byte unchanged from b312d1e24. Returning to In Review and continuing MAIN under [[tickets/T-24024-main-dispatcher]].
