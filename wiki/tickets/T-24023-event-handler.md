---
id: T-24023
title: Recover EVENT large handler
status: In Review
assignee: Astra hard-function lane
created: 2026-10-17
updated: 2026-10-18
links: ["[[tickets/T-24020-astra-hard-functions]]"]
---

## Goal

Match `func_801103A0` (6,848 bytes) in exclusively reserved `src/ovl/EVENT/8010FE10.c`, after TEL.

## Acceptance criteria

- [x] Read T-15200 and T-17400 evidence, test a new hypothesis.
- [x] Promote only verified C; 6,848 literal bytes and full EVENT SHA1 pass.
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

### Full-match continuation (2026-10-18)

Dialogue is verified. Reopen this target under the user's full-match requirement. Test whether zero-masked early definitions can split repeated month Boolean ranges without the extra long-lived CSE value. Preserve both nested tail exits and measure whole-function changes, including instruction bytes at equal aligned scores.

### Boolean reconstruction evidence (2026-10-18)

Zero-mask constants alone retain the previous 214/1000 scores. Reused month locals introduce extra copies or narrowing. A new distinct-expression probe is more informative despite a worse aggregate score: `month * 1`, `month & 255`, and `month << 0` in the three logical tests reproduce all three separate Boolean materializations and original register choices at the date block. The middle mask adds an unwanted `andi` and shifts later ugen temporary numbers. Testing distinct instruction-free operand wrappers next; do not discard structural progress merely because aligned score worsens. All outputs remain ignored under build/t24023-zero, build/t24023-lifetime and build/t24023-compare.

### Structurally exact checkpoint

Distinct month operands `* 1`, `^ 0`, `| 0` preserve all three required Boolean materializations. Replacing the negative date exit plus terminal jump with an equivalent positive date condition around the success block removes both explicit gotos while preserving every failure path. The common-tail month load and branch delay slots then match. Full-TU output now has exactly 1,712 instructions, aligned cost 58 / 58 printed rows: all 29 mismatching instructions only exchange the operands of equality branches. Saved the source-only checkpoint in drafts/T-24023/closure/body.c; not promoted.

Audit rejected month-assignment probe variants 11 and 14 despite their apparent 74 score: a missing pair of parentheses changed `month >= 3` to `month != 0` through `|` precedence. They are not evidence of progress and remain ignored. The structured positive condition was inspected directly and retains the original date failure behavior.

### Fourteen operand pairs remain

Assigning comparison constants through the existing word temporary fixes 15 of the 29 reversed branches, including four direct-global comparisons. Preferred experimental source is build/t24023-firstconst/four_globals/body.c (cost 28, 1,712 instructions). Captured unmodified cvt_pass pre/post ucode shows its `_constant_first` postpass exchanges loads for constant-only registers; constants 2, 7, 8 and 12 remain affected. Word-local copies, identity wrappers, pointer views and redundant call-argument diagnostics have not closed this. No diagnostic call-argument changes are accepted for production. Investigating signed-load/unsigned-value views without changing the compiler.

### Nine selector pairs remain

A `u32` selector copy at the start of each girl-dispatch arm preserves the exact 1,712 instructions while fixing additional branch operands. Use the existing `date_ok` byte for the four direct-global comparison constants so the selector copy is never clobbered. This source is build/t24023-armmutate/four_globals_byte/body.c, cost 18 / 18 printed rows (nine swapped instructions). The remaining constants vary by region: a register holding 12 earlier holds 3 in the girl-5 arm. The trace maps exact ucode comparisons, avoiding assumptions from register names. No production promotion yet.

### Verified default-C closure (2026-10-18)

Promoted `func_801103A0` into src/ovl/EVENT/8010FE10.c. Default `funcdiff.py EVENT:func_801103A0 --resolve` reports MATCH for all 6,848 linked bytes (1,712 instructions). Normal `ninja -j1 build/ovl/EVENT.ok` passes SHA1 ff7184d7e92b7aff372450ac3b8c2bdd42e55a5e. Header/global guards pass; prototype branch check reports 0 new / 15 known disagreements. Parent coordinated the canonical declaration `s32 func_801103A0(void)` in include/ovl/EVENT.h. The sole C caller, func_80119A60 in the same source file, ignores its result; the complete overlay checksum covers it and every rebuilt EVENT unit.

The final source keeps a u32 selector copy per dispatch arm and a positive date guard around the success block. Three distinct identity operands prevent unwanted month-equality commoning. The existing byte-gap local captures four comparison constants. The selector view `((u8 *)&D_801243A4)[0xC]` reaches the same byte as D_801243B0 while avoiding the conversion pass's global classification that reversed the last nine branch pairs. This neighboring-base view is explicitly FAKE under standards section 8: the containing aggregate is unknown, so no invented aggregate declaration is claimed. Literal linking and the full checksum verify the address/load width. These source workarounds are marked and documented; no toolchain changes or diagnostic extra arguments were accepted.

Retained final body and full-TU replay/negative controls in drafts/T-24023/closure/. Earlier body.c remains the immutable research checkpoint. Self-review checks C89, initialized locals, preserved date failure exits, alias explanation, coordinated prototype and source scope. Formal review and main integration remain with the orchestrator; ticket is In Review, not Done.

### Return-view rationale and replay controls

The s32 view preserves the target's return-register behavior. Early exits explicitly return zero. On the final path the original calls func_80011DFC and immediately restores the frame/returns, leaving v0 as the callee left it. There is deliberately no invented `return 0` after that call. The sole C caller discards the result, so no caller relies on a C fallthrough return value. Parent preview requested this clarification; the complete EVENT checksum verifies this tail and the caller.

Retained replay results: matched 0/0 with literal6848; canonical selector symbol18/18; shared month equality967 aligned/969 printed,1709 instructions; literal comparison constants8/8; historical checkpoint214/826. Except the shared-month control, all have1712 instructions. These are measured negative controls, not unverified match claims.
