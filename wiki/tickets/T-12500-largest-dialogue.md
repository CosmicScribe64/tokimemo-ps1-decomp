---
id: T-12500
title: Match largest dialogue routine
status: In Progress
assignee: Codex cx1
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-10900-big-functions-astra-22]]", "[[tickets/T-11590-w1-main-8007c030]]", "[[matching-notes]]"]
---

## Goal

Match dialogue_speak_part (8,672 bytes) in isolated codex/largest-dialogue from integration 2cb6a11c. Own only src/main/8007C030.c and minimal justified headers/docs. User prioritizes largest verified byte gains.

## Acceptance criteria

- [ ] Fresh Docker full-file baseline and exact schedule localization.
- [ ] New hypotheses informed by previous failed variants and native AS1 diagnostics.
- [ ] Any promotion passes production literal linked funcdiff and full checksums.
- [ ] Final header/global/prototype checks and independent standards review.

## Notes

Read T-10900 and T-11590: historical 16-row residual consists of two li a0,-40 scheduling rotations. Prior coordinate locals, argument layouts and 96 string lifetime/layout combinations did not improve it. Research lead: native AS1 -Wa,-R,-xbbdbg,8 scheduling diagnostics, requiring full pipeline fidelity controls before interpretation. Existing compiler and acceptance proof remain unchanged. No assembly patching.

## Comments

Worktree setup reuses ignored cx7 inputs, local skills and traced uopt. Orchestrator owns integration and publishing. Baseline pending.

Fresh baseline configure/ninja -j2 succeeds (665 tasks, 27/27 binary checksums, headers/globals). Scratch AS1 capture wrapper preserves the existing frame/cvt/layout pipeline and adds native diagnostics only after frame transformation; stock-versus-trace fidelity will be required before reading DAG evidence. Novel route: identify actual local scheduling dependencies at the two residual render calls and combine precise source-line grouping boundaries with source expression/statement order, rather than repeating broad coordinate/string sweeps.
