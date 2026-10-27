---
id: T-25020
title: Map recovered BUNKA_SD scheduler trace before closure tests
status: Done
assignee: codex
created: 2026-10-22
updated: 2026-10-26
links: [[[tickets/T-25016-candidate-reconstruction]], [[as1-scheduler-rule]], [[build-step-gaps]]]
---

## Goal

Verify the recovered func_80139290 trace context, map block 60 to source, and test only a specific untested scheduling input supported by that evidence.

## Acceptance criteria

- [x] Recovered variant context checked against current full translation unit.
- [x] Block 60 moved instructions mapped to source; distinguish relevant residue from unrelated filler.
- [x] At most three causal experiments, no repeated volatile or general sweeps.
- [x] Promote only on literal default C proof and affected checksum/declaration guards, or pin exact blocker.
- [ ] Code review against CODING_STANDARDS.md before Done.

## Notes

Reuse codex-t25018-kangei worktree; previous result pinned at b7fa134c1. BUNKA_SD source ownership is src/ovl/BUNKA_SD/801388D0.c. Recovered material at /private/tmp/t25016-recovered-20261022 is read-only reference. No compiler changes, host installs, push, integration, or extra agents.

## Comments

### Context and mapping

Current full-unit NON_MATCHING C baseline: seven changed instruction positions, printed as 14 +/- lines, at rows 159-161, 166-167, 171-172. Recovered directory `traces/39290-vol.txt` is scheduler-equivalent to this baseline despite its name: all 779 DAG records normalize identically after source-line offset +3358 and nonzero relocation-id offset +252; all 74 MOVETO/MOVEFROM events match exactly. This proves relevant codegen context equivalence, not the provenance of the original header file.

There are 37 MOVEFROM events total, and block 60 supplies **eight**, not nine. The eight are inst 2/3/5/8/11/14/15/16: case-0 constants 17/21/25/29, the first store-address lui, and the three D_80122EAC masks (2/4/1), moved into block 59's two divu stall windows. Source region is the later `(g->unk_1104.u - 0xC9) % 0x4E / 0x27` switch, case 0 (src lines 332 onward before this edit; recovered lines 93-97). None is the year-byte-load residue in the earlier 0x70000000 switch.

The relevant arms reproduce `drafts/T-24927/NOTES.md`: uopt emits speculative join-preamble year loads on the arm exit line, then the per-block scheduler orders the store's older-line li/lui before lbu. The global mover cannot move the lbu. T-24927's real year local fixes this but creates an allocator cascade; it was not retried.

### Two causal experiments

1. Insert `if (g->unk_03E) {}` before the store in cases 0x3B/0x49/0x66. Unlike autotweak's local/parameter-only emptytest transform, this reads the shared field without creating a local range. T-10820/T-10850 empty tests are the precedent for preserving emission/range state. Trace shows lbu now precedes the store in all three local schedules; registers stay correct. However the global mover now hoists the 0x49 and 0x66 arms' early `lui t0` into compare-chain slots, replacing the original li fillers. Whole function worsens to 16 +/- lines (vr score 10, 379/379 instructions), so this variant is rejected.
2. Isolate only case 0x3B's test. It removes all three case-0x3B changed rows with no compare-head regression: **four changed instruction positions / eight +/- lines**, only 166/167 and 171/172 remain. vr score 4, 379/379 instructions (control score 6, 14 +/- lines). Preserved as the better guarded draft with a FAKE comment. No third experiment or sweep.

Next exact problem: make the year-byte lbu precede the store-address lui in cases 0x49/0x66 while preserving the original compare-head fillers. The all-arm emptytest form solves the first and breaks the second; do not repeat the global volatile trial or real-local lever. Source notes now identify the correct local mechanism.

### Bounded next consultation

The preserved case-0x3B arm is block 41. Its head's delay slot is already the required `move a1,v1` (original 0x801394D0), so making its load earlier does not offer a free head stall to the global mover. Blocks 43 and 45 are the 0x49 and 0x66 arms. Their compare heads are bb 42 (weight 4, one stall) and bb 44 (weight 7, one stall). In the baseline bb 42 takes bb 43 inst 4, `li t2,0x60`; bb 44 takes bb 45 inst 2, `li t8,1`. With empty field tests on those arms, their early `lui t0` becomes inst 2 and is taken first instead. This explains why the identical local-order lever succeeds only for 0x3B.

A bounded allocator/scheduler consultation should solve exactly this: keep the four-row draft's register identities, arrange lbu before store-address lui in bb 43/45, and preserve bb 42's li 0x60 plus bb 44's li 1 fillers. No further tests in this ticket. Do not retry all-arm empty tests, volatile year, or the T-24927 real-local lever.

### Verification and handoff

Actual updated in-tree `funcdiff --resolve BUNKA_SD:func_80139290` explicitly compares full-unit NON_MATCHING C: four changed positions / eight +/- lines, rows 166/167 and 171/172 only. Default `funcdiff --resolve --version default` separately verifies MATCH, linked bytes equal target (1516 bytes); **that is the retained assembly fallback, not a C match**. `ninja build/ovl/BUNKA_SD.ok` SHA1 passes. Header/global migration/prototype guards pass (zero new prototype disagreements, 15 known). Wiki lint: zero broken links, zero status mismatches, one pre-existing large page. `git diff --check` clean. No full 27-unit worker recheck, push, or integration.

Root review requested; ticket remains In Review.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
