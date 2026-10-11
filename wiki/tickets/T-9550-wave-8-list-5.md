---
id: T-9550
title: "Wave 8: list 5"
status: Done
assignee: w8-5
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[register-allocation-wave7]]"]
---

## Goal

Work through wave8-list-5.txt in order, keeping byte-matched C and reverting time-boxed drafts.

## Acceptance criteria

- [x] Each listed function attempted or time-boxed with evidence.
- [x] Final Docker ninja passes all 27 checksums and header/global checks.
- [x] Branch prototype check passes; inline standards review recorded.

## Notes

Follow the shared wave-8 brief. Changes stay uncommitted for the orchestrator.

Second pass: user requires sustained asm-guided iteration, 10–15 minutes per candidate and about five minutes of permutation below 20 differing lines. Prioritize nearest resolved diffs; repair aggregate/local-copy types afterward. Work for at least three hours or exhaust the rejected list. Start: 2026-10-11 11:08:23 UTC; earliest time-based finish: 14:08:23 UTC. First-pass baseline is commit df5bc9f, four matches/1,540 bytes.

## Comments

- 2026-10-11: Claimed list 5; read wiki index, workflow, coding standards, shared brief and register-allocation findings.
- 2026-10-11: Baseline main and completed overlays pass SHA1; prepared list-order drafts and explicit array forms for DATE's halfword rows. Scratch scripts and draft evidence live only in scratchpad/w8-5/.
- 2026-10-11: DATE func_801572E4 is within a moved load pair after struct-copy, narrow-index-loop and byte-chain rewrites. Prepared a common D_8011ECD0 view for its two flag updates to keep the second load after the first store; retest pending after the active batch.
- 2026-10-11: Function diff MATCH for `DATE:func_801572E4` (268 bytes).
- 2026-10-11: Function diff MATCH for `TAIIKU:func_80143D24` (172 bytes).
- 2026-10-11: Function diff MATCH for `main:cal_sprite_init` (828 bytes).
- 2026-10-11: Function diff MATCH for `main:initLight` (272 bytes).
- 2026-10-11: Docker completeness audit accounts for all 152/152 listed functions. Four matches currently retained (1,540 bytes); failed candidates and declaration views restored. Final two local-allocation retries and validation follow.
- 2026-10-11: Four-match batch Docker ninja passed 27/27 checksums, headers and globals. The final model/callback local-allocation retries differ and are restored. Added 14 T-0018 evidence rows in total.
- 2026-10-11: Inline code-review skill against HEAD (4607287db1200b1b005233958cb1d4f5e56bbf72), with the shared wave-8 brief as spec. Standards finding: incomplete DATE workspace, TAIIKU coordinate and libgs light aggregate views needed explicit FAKE documentation; comments now explain and ticket all three. No SDK definitions/prototypes added. Existing placeholder names and shared declarations retained. Spec review: all 152 entries accounted for; only owned C files, permitted declarations and wiki files changed; no raw/generated sources, tools or Git state changed. Final verification and reuse audit pending.
- 2026-10-11: Final Docker verification PASS: sync_protos --check-branch reports 0 new disagreements (17 known); migrate_globals --check passes; all four fresh resolved function diffs MATCH after comments; incremental ninja exits 0 with 27 checksum checks, followed by explicit 27/27 SHA1 verification and header/global stamps. Owned-file dupes applied 0 copies. Final progress: grand total 4738/6958 functions, 731152/2279368 bytes (32.1%); SDK 722 functions excluded.
- 2026-10-11: Inline review complete. Standards: three documentation findings resolved, zero outstanding; C89/fixed-width/layout comments, existing SDK declarations, field-based GameState/Rec24 access, explained FAKE views and overlay-only delta declaration checked. Spec: zero outstanding; all 152 entries attempted/time-boxed, four matches (1,540 bytes), fourteen appended T-0018 rows, allowed-file scope and no Git mutations confirmed. Moved to Done; edits remain uncommitted for orchestrator merge. Evidence: scratchpad/w8-5/final-validation.log and results-*.jsonl.
- 2026-10-11: Deep-pass MATCH `DATE:func_80140D38` (124 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass permutation MATCH `main:func_80086640` (60 bytes), real resolved diff and unit SHA1 pass; scheduling/expression layout marked FAKE.
- 2026-10-11: Deep-pass permutation MATCH `main:func_80067DD4` (40 bytes), real resolved diff and unit SHA1 pass; scheduling/expression layout marked FAKE.
- 2026-10-11: Deep-pass MATCH `main:func_80067DD4` (40 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `main:func_80067DFC` (40 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `DATE2:func_80138E0C` (384 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `DATE2:func_80138F8C` (532 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `DATE2:func_801391A0` (532 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `DATE2:func_801393B4` (532 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `GEKO:func_8013CC20` (64 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `RPG_BAT:func_801564F0` (620 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `GEKO:func_80139C70` (228 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `TACO:func_8013D1E0` (192 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `TACO:func_8013D2A0` (168 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `BUNKAKEN:func_80152F4C` (248 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `main:func_80086640` (60 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `DATE2:func_80138A90` (348 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `main:func_8006C700` (96 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `main:func_8006C700` (96 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Deep-pass MATCH `BUNKAKEN:func_80152F4C` (248 bytes), fresh resolved diff and unit SHA1 pass.
- 2026-10-11: Second-pass final verification: sixteen unique additional matches / 4,208 bytes; twenty batch matches / 5,748 bytes including committed df5bc9f. Repeated automatic helper entries describe cleanup gates, not additional functions. Scratch evidence: deep-final-validation.log, deep-final-taco-review.log and deep-final-dupes.log. All twenty functions pass fresh normal funcdiff --resolve; after final TACO field naming, both affected functions are rechecked and the full build plus explicit 27/27 SHA1 checks pass. Header/global guards pass; sync_protos --check-branch reports zero new disagreements (17 known), and migrate_globals --check passes. Owned-file dupes applies zero copies. Progress: grand total 4754/6958 functions, 735360/2279368 bytes (32.3%).
- 2026-10-11: Inline code-review skill, standards axis, pinned to df5bc9f18af9c08be13d52717423b8d5fade69cb: reviewed C89, SDK declarations, fixed-width types, recovered aggregate offsets, GameState member accesses, generated main API delta, placeholder names and all FAKE reasons against CODING_STANDARDS.md and CONTRIBUTING.md. Findings resolved: redundant swap/permuter statements removed; main-position unused locals removed and left shift made unsigned; required BUNKAKEN padding named/documented after removal demonstrably changed the table offset; DATE2 string-pointer/frame/conversion views documented; new TACO unknown fields renamed unk_XX. Fresh byte checks cover all resulting edits. Zero outstanding standards findings. Duplicate templates and placeholder names are required by matching conventions; no speculative helper/refactor introduced.
- 2026-10-11: Inline review, spec axis: ranked usable resolved diffs first, deferred malformed aggregate drafts, performed sustained assembly-guided experiments and full five-minute permutation runs on near misses, recovered aggregate types for eight newly matched local-copy/initializer functions, and restored failed candidates and their temporary declarations. Experiment window 11:08:23–14:14:07 UTC exceeds three hours; stopped on the duration condition, not deep-list exhaustion. Larger SDK/local-buffer types and unattempted deep drafts remain future work. Exactly sixteen INCLUDE_ASM replacements across nine owned C files; all changed paths are authorized (the final kanban card returns to its committed Done position). No tools, configs, raw/game/generated source or Git state changed. Zero outstanding spec findings for the time-bounded pass. No new T-0018 rows (fourteen remain from the first pass); historical rows get resolution prose. Known tooling mismatch: register_shapes.py does not accept the positional function argument prescribed by the brief.
- 2026-10-11: Review and final checks complete; moved T-9550 to Done. All second-pass changes remain uncommitted for orchestrator merge.
