---
id: T-24008
title: Recover TAIIKU runner family
status: In Review
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: []
---

## Goal

Bounded types/sibling pass on src/ovl/TAIIKU/80133C80.c: func_80137544, func_80135480, func_80137C44, func_80137EA8, func_801365C8. Necessary include/ovl/TAIIKU.h changes only. Base 8f88afff607eb20c4e143239e28b94cc7f1c7392; preserve all earlier work.

## Acceptance criteria

Fresh default linked literal proof and TAIIKU SHA1 for wins. Docker only, serial compiler/ninja -j1. Source-only useful hypotheses, review against CODING_STANDARDS.md before Done.

## Notes

Read [[taiiku-family-results]], T-16550 and recent family history before probes.

## Comments

## Checkpoint: func_80137EA8 matched

692 verified bytes, no new FAKE or header edits. Matched sibling port from func_80146D18 with canonical runner/display fields, local C result array definition, explicit final else, and element-zero-first assignment. Fresh Docker default linked literal proof and TAIIKU SHA1 306d1013b9be465819525d1fb4dc0e86b422a846 pass. See [[taiiku-family-results]]. Other four targets remain open within bounded pass.

2026-10-17 independent root integration review: func_80137EA8: reviewed C89 source, shared types, four-byte array and sibling-derived control flow. No new FAKE or header change. Independent default literal MATCH 692 bytes; all 27 hashes and guards pass; published f647bee35. No unresolved findings in this match; remaining family work is separate.
## Final results

| Target | Result |
| --- | --- |
| func_80137EA8 | Matched, 692 literal linked bytes, commit d2c844fb3df7a3f57eaa74942517fdac07a0b4c5 |
| func_801365C8 | New guarded typed draft: 188 / 188 instructions, frame/spill exact, nine differing register/operand instructions (aligned cost 18) |
| func_80135480 | Existing T-19004 guard retained; new hypotheses no gain |
| func_80137544 | Existing T-19004 guard retained; geometry and sentinel hypotheses no gain |
| func_80137C44 | INCLUDE_ASM retained, canonical typed hypothesis archived (187 cost, 154 / 153 instructions) |

Source-only drafts and complete stable probe costs: drafts/T-24008/. No new shared headers, toolchain changes or host dependencies. The updater keeps a documented FAKE four-runner view; the finish helper has no new FAKE. See [[taiiku-family-results]] for exact recipes and rejected variants.

### Final verification and review

Docker ninja -j1 object and TAIIKU checksum pass after final guard. SHA1 306d1013b9be465819525d1fb4dc0e86b422a846. Fresh default --resolve --no-build linked proof passes all five targets, with only func_80137EA8 counting as new C. check_headers and migrate_globals --check pass; sync_protos --check-branch passes with 0 new disagreements and 15 known.

Self-review: C89 and SDK/game types; guarded draft ticketed; FAKE view documented; no raw data/generated assembly/assets; no previous archives modified; focused commits and wiki/board consistent. In Review for parent independent review; no Done claim.

Full NON_MATCHING translation-unit funcdiff also compiles and reproduces exactly the same nine differing updater instructions; this confirms the retained scratch diagnosis with the other guards active. Default proof remains literal MATCH through ASM.
