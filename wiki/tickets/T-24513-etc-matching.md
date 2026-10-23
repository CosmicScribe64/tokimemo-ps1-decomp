---
id: T-24513
title: Match remaining ETC functions
status: In Review
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[playbook]]", "[[notes/functions]]"]
---

## Goal
Recover literal default C for remaining ETC functions, largest tractable first, in isolated branch codex/t24513-etc from 23e1a20fb.

## Acceptance criteria
- Verified linked function bytes, full 27 binary checksums and interface guards.
- Source-only bounded causal experiments; preserve unresolved evidence.
- Standards/spec review before Done.

## Notes
Own src/ovl/ETC and include/ovl/ETC.h. Existing T-24078 renderer experiments are negative history, not candidates for wholesale repetition. Docker only; root disc mounted read-only.

## Comments
- 2026-10-20: Claimed isolated worktree /private/tmp/tokimemo-t24513. Initial target func_801480D0 (2808 bytes); history audit precedes experiments.

### Bounded experiments
- Ranking func_801480D0 current T-24049 baseline reproduces 702 target /709 candidate words, 885 printed rows, 859 aligned cost. Explicit generation pointers yield 708/1030/994; return-int 710/888/862; debug-int 709/885/859. Rejected, no source promotion.
- Scene initializer func_8013FFAC has a real six-entry address buffer: original 0x80198600..0x801B0600 step0x4000, pointer byte step4. Old T-16100 buffer16 and pointer element step4 are wrong. Current reconstruction uses six elements, element step1, real current GameState/table views and original second six-entry table. The target calls later same-TU void func_80140AE4, so private forward declaration is required for a coherent compile.
- Scene valid full-TU baseline: 529 target /533 candidate words, 486 printed rows, 484 aligned cost. Consistent GameState pointer: 533/476/476; coherent int return view:536/489/487. Removing two copied table/byte locals gives533/492/456 and restores original frame0x70 (buffer remains4 bytes high). Printed rows and aligned cost differ; lower printed rows alone do not establish improvement.
- Grade func_80146D4C T-24303 baseline reproduced264 words/104 rows/102 cost. Repeating direct grade-array expressions and adding a byte field view both give264/138/136. Rejected, no new shared-header override.

- Scene source follow-up: removing copied aliases yields original frame0x70 but buffer0x58 vs original0x54. Explicit signed address bound is neutral. Natural second-table loop from0 gives538 candidate words/455 rows/453 cost; retained as semantic reconstruction. First constant-address loop is expanded by compiler versus retained original loop. No matching claim or impossibility proof. Archive/replay retained under drafts/T-24513; all other scratch variants remain ignored or are discarded.

### Self-review and handoff
- Standards review against CODING_STANDARDS.md and local code-review skill: no findings. Drafts are source-only C89 with fixed-width types, real current arrays/fields, no inline asm, no new FAKE shaping or invented padding, no production guard/header change, and no generated assembly/game data. Replay uses Docker tools and actual scratch C, preserves production, and distinguishes diagnostics from linked proof.
- Scope review: no function matched. Coordinator approved a focused negative-results archive after the bounded controls. This ticket preserves reconstruction/handoff evidence, not completed ETC decompilation. No byte-gain or impossibility claim. In Review for coordinator review.
- Exact unchanged audit: `git diff 23e1a20fb -- src include config tools` is empty. All target production INCLUDE_ASM paths remain unchanged. No shared-root/integration/contributor changes or push.
- Fresh Docker `ninja -j2`: all 27 binary checksum targets pass. Early aggregate guard ran before TACO splitting and reported missing TACO proof assembly; final guard is rerun after the completed full build.
- Final verification: all eight replay cases reproduce expected metrics through actual C and explicit-object DIFF. Headers OK; globals OK; prototype branch check OK (0 new, 15 known). Independent hashes of all 27 built binary paths match configured digests. Direct sha1sum -c overlay invocation initially used the wrong working directory and was replaced by explicit built-path hashing; no build failure. Replay's initial helper rejected expected DIFF exit1 and is corrected to require a real verdict while accepting that status. Re-reviewed fixes, no findings.
