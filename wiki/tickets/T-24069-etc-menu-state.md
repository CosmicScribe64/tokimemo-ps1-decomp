---
id: T-24069
title: Match ETC menu state machine
status: Done
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-17300-types-first-etc]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_8013BFFC (820 instruction bytes) in src/ovl/ETC/8013BEF0.c. Preserve matched same-file func_8013C330 and every other body. T-24067/24068 remain unresolved archives with no further probes.

## Acceptance criteria
- Current primary015c3496f retains target INCLUDE_ASM and actual matched same-file sibling func_8013C330 (976 bytes, sibling ratio0.766). Read current wiki and prior target negatives first; confirm ownership remains unclaimed.
- Reuse clean built worktree from af25ebdc7 on codex/complete-etc-menu-state, the same single Sol medium worker/compiler, Docker only and ninja -j1. No extra agents or broad sweeps.
- Reconstruct target-specific cases from original assembly using matched sibling structure and verified GameState fields; do not copy incorrect old field folds.
- Coordinate exact shared-header/config scope before changes. Actual C requires default unguarded literal proof, ETC checksum/interface guards and unchanged-body audit including sibling.
- Parent owns independent review/integration; remain In Review until reviewed.

## Notes
T-17300 typed draft had12 repair failures and no score; apparent case1/2 GameState110D updates were misfolded as unk_000. T-9520 records a redeclaration failure. Matched sibling supplies concrete menu-step/load/fade-tail structure, sequential load-state if statements and an existing s32 fallthrough-return view. Verify target semantics and return usage rather than assuming every sibling detail transfers. No live ownership found for this TU; old T-24055 scope is a different TU, and contributor NAME_ENT/TT/DATE2 scopes remain excluded.

## Comments
Target selected read-only under parent's next-unclaimed-ETC direction, with target/rationale sent before edits. Checkpoint af25ebdc7 and prior unresolved packets preserved. No target edit or compile before ticket setup.

Fresh corrected full-TU baseline compiles:110 resolved +/- rows, unlike old repair failures. It uses real GameState110D and target case3's actual else-if (the matched sibling's sequential-if structure does not transfer there). A single case1 control retains the two original branch-specific D80150070 clears, improving103 rows. Provisional s32 fallthrough reflects the existing earlier implicit-int caller; a private void view with scratch forward is neutral103, so no return type is inferred from sibling alone. One consistent GameState pointer improves67 rows. Production and all neighbor bodies remain untouched; private diagnostics are not actual C proof.

Historical clarification: deeper lookup found T-14500/repairs/automatic/mechanical/ETC/func_8013BFFC already scored110 using correct scalar-symbol semantics. Fresh110 reproduces that context after current aggregate normalization, not a novel semantic recovery. T-17300's separate bad folds/compile failures remain as recorded.

Verified actual default C: a shared word temporary captures required step/status loads, reproducing original v1 allocation (FAKE annotated because original locals unknown). Initial switch capture gives5 rows/extra move; direct switch with later captures gives zero. Existing s32 implicit-int view also gives zero, avoiding a header change. Production replaces only assigned INCLUDE_ASM with unguarded C.

Proof: tools/funcdiff.py --no-build --resolve --version default --unit ETC func_8013BFFC reports MATCH820 linked bytes after ninja -j1 build/ovl/ETC.ok passes SHA1. Header/global guards pass; prototype branch0 new/15 known. Exact other-source text preserved;40 neighbor object bodies byte-identical to fresh baseline, including8013C330. Logs: build/etc24069/final-proof.log, final-build.log, header-guard.log, global-guard.log, proto-guard.log, body-audit.log. In Review pending parent's independent review/all27/integration, not Done.

Parent independently reviewed complete92-line change against standards: PASS/no findings, including FAKE capture, existing s32 view and real fields. Actual-C gate/literal820, all27 hashes, interfaces and prototypes0 new/15 known pass. Published primary fd700bf3f; merge log .claude/orchestrator/state/merge-logs/e3750f110.log. Done after review. Source-only drafts/T-24069 preserves measured controls/replay; zero replay independently reproduces0 rows without production edits. Read-only Docker progress on primary fd700bf3f:6514/6958 functions,1865416/2279368 bytes(81.8%);413952 bytes remain.
