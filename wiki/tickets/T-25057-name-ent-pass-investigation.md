---
id: T-25057
title: Reconstruct NAME_ENT record selector allocation
status: In Progress
assignee: Codex Astra
created: 2026-10-22
updated: 2026-10-22
links: ["[[playbook]]", "[[tickets/T-24515-name-ent]]"]
---

## Goal

Qualify and reconstruct func_801398EC, then the best remaining function in the exclusively owned src/ovl/NAME_ENT/80139740.c. Isolated tree /private/tmp/tokimemo-t24515, branch codex/t25057, base 633d95356. Prior branch codex/t24515-name-ent preserves 30b4f5716.

## Acceptance criteria

- [ ] Causal pass-level investigation with exact replayable evidence and best sources archived.
- [ ] Any promotion proves literal linked default C MATCH, 27 binary checksums and declaration guards.
- [ ] Direct standards/spec review resolves findings before Done; root integrates and pushes.

## Notes

Docker-only existing tokimemo-decomp-arm64 image, read-only disc. No shared headers/tools edits without coordinator approval. Compiler slot initially pending while TEL and ENDING run. No subagents or cross-chat messages.

## Comments

2026-10-22: Claimed before matching work. Applying local implement and code-review skills; direct review follows explicit assignment rather than spawning reviewers.

2026-10-22: Eight serial full-TU controls qualify current guard126 words/185 diff rows and indexed126/173. Named unsigned frame remainder followed by `if (tile < 0) {}` after the sprite store yields123 words/102 rows; initialize i before selector yields123/72 (36 differing words). Exact instruction shape now agrees, apart from scheduling the frame/index loads on the selector-nonzero arm. Remainder read is dominated by its assignment within the flag-zero arm; unsigned comparison is defined and always false. Diagnostic FAKE source retained in drafts/T-25057/func_801398EC.c. Trace of the ordinary remainder-test predecessor confirms unconstrained bit order: selector, i, record index, frame, remainder, record pointer. Need pointer/remainder/frame/index order to match a0/a1/a2/a3. Early address comparison adds4 words; tile initialization is neutral. No production source change or promotion. Compiler slot released to TEL during analysis.

2026-10-22: Exact-width discarded record read before flag testing fixes the pointer induction allocation: word and byte views both123 words/36 rows (18 differing words), with no new instruction. Best source replaced in drafts/T-25057/func_801398EC.c. Trace now shows index bit17->a1, frame bit26->a2, remainder bit29->a3; desired a3/a2/a1. Introducing an unreachable alternate tile assignment creates a second live range and125 words; guarding with the unsigned index instead gives122 words. An early local zero-reference adds instructions. These controls are rejected; the36-row source remains preferred. Compiler slot released to ENDING for pass inspection.

2026-10-22: Natural precomputation of tile before the flag tests is lazily sunk by uopt to the original use paths. Result123 words/30 rows (15 words), exclusively frame/remainder a1/a2 swapped; all instruction scheduling and other registers match. The source archive now preserves this best. Trace proves frame bit17 precedes remainder expression bit20; both unconstrained. Signed tile is neutral; u8 adds6 words; defining tile at loop join or using unreachable alternate definitions changes loop structure and is rejected. Reusing tile with >>= and %= is neutral. No evidence for a generic compiler change.

2026-10-22: Preserved second frontier func_801398EC-array.c (123 words/34 rows): one-element memory local plus removed dead frame initialization gives every target uopt register, and zero-store-before-remainder gives target scheduling. Remaining17 words originate in ugen operand evaluation: scalar ucode emits row-divide/multiply then tile-multiply; array emits tile-multiply first. Both captured through existing traced compiler with no production-tool modification. Signedness, commutation, association and literal identity controls normalize identically; late-zero expressions regress. Defined lexical goto diamond preserves shape but has50 rows. Best scalar123/30 and array123/34 are both archived; matching investigation continues.

2026-10-22 checkpoint verification: annotated saved sources recompiled through complete TU in Docker. Scalar108/123 identical resolved positions (15 differing,30 printed rows); array106/123 (17 differing,34 rows). Results recorded in drafts/T-25057/saved-results.json; neither is a literal linked match. Direct standards/spec review against633d95356 covers source-only diagnostic scope, C89, existing types, marked FAKE constraints, no undefined local reads, read-only disc/Docker-only tools, and no production/header/compiler changes. README is a replay guide for the next matcher and preserves unique pass evidence. Ticket remains In Progress while investigation continues.

2026-10-22 staged checkpoint review: no unresolved standards/spec findings. Docker wiki lint reports0 broken links/0 status mismatches, with only the existing large data/log pages. Staged deslop review split the long README control paragraph and removed one stock adverb; the repeated append-only log sentence and technical dominance/FAKE wording are retained. No copyrighted material, build output, production changes or shared declarations staged. This checkpoint records ongoing research, not acceptance of a match.
