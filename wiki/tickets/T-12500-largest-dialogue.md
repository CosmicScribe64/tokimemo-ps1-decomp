---
id: T-12500
title: Match largest dialogue routine
status: In Review
assignee: Codex cx1
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-10870-big-functions-5-astra]]", "[[tickets/T-10900-big-functions-astra-22]]", "[[tickets/T-11590-w1-main-8007c030]]", "[[matching-notes]]"]
---

## Goal

Match dialogue_speak_part (8,672 bytes) in isolated codex/largest-dialogue from integration 2cb6a11c. Own only src/main/8007C030.c and minimal justified headers/docs. User prioritizes largest verified byte gains.

## Acceptance criteria

- [x] Fresh Docker full-file baseline and exact schedule localization.
- [x] New hypotheses informed by previous failed variants and native AS1 diagnostics.
- [ ] Any promotion passes production literal linked funcdiff and full checksums.
- [ ] Final header/global/prototype checks and independent standards review.

## Notes

Read T-10900 and T-11590: historical 16-row residual consists of two li a0,-40 scheduling rotations. Prior coordinate locals, argument layouts and 96 string lifetime/layout combinations did not improve it. Research lead: native AS1 -Wa,-R,-xbbdbg,8 scheduling diagnostics, requiring full pipeline fidelity controls before interpretation. Existing compiler and acceptance proof remain unchanged. No assembly patching.

## Comments

Worktree setup reuses ignored cx7 inputs, local skills and traced uopt. Orchestrator owns integration and publishing. Baseline verified below.

Fresh baseline configure/ninja -j2 succeeds (665 tasks, 27/27 binary checksums, headers/globals). Scratch AS1 capture wrapper preserves the existing frame/cvt/layout pipeline and adds native diagnostics only after frame transformation; stock-versus-trace fidelity will be required before reading DAG evidence. Novel route: identify actual local scheduling dependencies at the two residual render calls and combine precise source-line grouping boundaries with source expression/statement order, rather than repeating broad coordinate/string sweeps.

Native AS1 fidelity established on the complete guarded object: ordinary compiler, capture-only wrapper and `-R -xbbdbg 8` wrapper produce byte-identical objects (including all matched control functions in the TU). Both transformed binasm streams are retained. Native trace contains nonempty DAG and pick decisions (5.1 MB), so this is an exercised diagnostic, not an empty-log inference.

Baseline remains 16 rows at positions 329–332 and 410–413. In the first DAG, li a0,-40 is node 11, with no dependencies and zero downstream cycles; signed-Y sra is node 12, also zero downstream cycles, but belongs to the preceding statement. The scheduler picks the sra and two string-pointer stores before the constant. Inline Y assignment moves the constant earlier in the input DAG but introduces an unwanted move s1,a1 and changes chain priority. A 24-case grouping/coordinate/comma probe produced no improvement (16 best); scratch-only comma-in-first-argument variants have unsequenced access and are diagnostic only, never promotable. Next bounded probe removes the inline narrow assignment result to avoid that extra copy.

Further full-TU probes covered explicit signed shifts, temporary scopes, narrow/unsigned views, constant identities, reversed textual goto blocks, and live-range copies. None improved 16 rows. Wider coordinates disturb the frame and global allocation; scoped copies reach 24–32 rows but select addiu a1 instead of the original s1. Keeping inline narrow assignment adds a move; removing its result restores the old scheduling. These observations do not establish impossibility. A bounded 20-minute combined-mutation search retains measured near-miss variants and tests combinations; a separate 128-layout sweep splits increment and call tokens at source-line boundaries. All outputs are ignored scratch, and the production C remains unchanged.


## Handoff

No new matches or bytes: production C, headers and compiler remain unchanged. The inherited dialogue_speak_part guard remains at 16 rows (eight positions) across 8,672 bytes. All 128 token-line layouts and both bounded combined searches (215 and 44 full-TU evaluations; 1,205 and 365 seconds) failed to improve the baseline. Targeted expression, prototype, aggregate/bit-field, storage-class and frame probes also failed to improve it. Regressing candidates remain scratch-only; no new guards were added. This pass is In Review for its diagnostic findings, not a claim that the target is finished.

Final configure plus forced checksum/header/global rebuild passed 526 tasks and all 27 binary checksums. Headers/globals pass; branch prototypes report 0 new / 15 known. Fresh guarded funcdiff confirms the original 16 rows. Logs: build/scratch/final-{configure,clean,build,prototypes,guard}.log. Full-branch whitespace checks pass. Source promotion verification is not applicable because no candidate reached zero; independent review remains pending.

Native AS1 fidelity and the remaining scheduling blocker are recorded in [[matching-notes]]. The paired search ledgers are build/scratch/beam/ledger.jsonl and build/scratch/beam2/ledger.jsonl. Future work needs a new source or compiler-history hypothesis; these negative results do not prove impossibility.
