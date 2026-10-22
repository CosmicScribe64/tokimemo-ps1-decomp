---
id: T-25059
title: Match ETC ranking generator
status: Done
assignee: Astra ETC worker
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25054-etc-results-scene]]", "[[tickets/T-24049-etc-exam-ranking]]", "[[tickets/T-24513-etc-matching]]", "[[playbook]]"]
---

## Goal

Match func_801480D0 in src/ovl/ETC/80146400.c. Continue exclusive ownership of this source after the T-25054 match checkpoint. Preserve prior pins and private evidence; root handles integration. No shared-header or compiler changes without coordination.

## Starting evidence

The corrected T-24049 reconstruction uses the current three ranking arrays, signed random divisors, the actual player-score field, and eleven final bonus records. T-24513 reproduced 702 target / 709 candidate words, aligned cost859 and a 216-byte frame versus the target208. Explicit generation pointer walks, return-int and debug-int controls did not improve it. Reproduce this complete-TU baseline before interpreting older metrics. The remaining gap is structural, not merely temporary registers.

## Acceptance criteria

- Verified literal default C, affected ETC checksum and declaration guards.
- Independent standards/spec review before integration; annotate artificial matching forms.
- No inline assembly, instruction patches, per-function compiler exceptions or semantic shortcuts.
- Sustain causal source/phase experiments until a verified match or concrete external blocker, without arbitrary time or probe limits.

## Activity

2026-10-22: Parent assigned the ranking generator after T-25054. Read wiki index, current status/playbook, T-24049 and T-24513 history and corrected reconstruction. Ticket created before experiments on branch codex/t25059-etc-ranking, preserving pin0d07f7dac59abef3669c20655860e13d1b2d21a7. Production remains INCLUDE_ASM.

2026-10-22: Reproduced current full-TU baseline, 702 target / 709 candidate words, aligned cost859. The newly matched caller requires a coherent scratch void forward declaration; no production declaration changed. Existing uopt tracer reproduces all709 candidate words byte-for-byte. Late-zero next-index controls remove too much unrolling and do not recover the first phase. Target register continuity instead shows that the generation counter becomes the inner counter of both tie loops and the selection loop. Swapping only these source variable identities preserves behavior and improves the structure to705 words/cost621. Giving the first noise result a separate scalar (instead of reusing the later adjustment amount) restores the target208-byte frame and four saved registers, at705 words/cost595. A one-element-array control reaches cost571 but is not yet selected; scalar evidence is clearer. Next controls address the observed switch layout and expression evaluation order.

2026-10-22: Reversed the generation selector's conditional layout to match the target's case9-before-case1 bodies, commuted the two multiplication operands to recover the observed evaluation order, and reused the same random-result local across both samples. Stack homes were derived directly from frame208 and declared offsets: first noise at188, angle at164, random offset at172, values array at104. Placing twelve scalars before the array restores these homes without padding. Candidate now704 words/cost526; the first random generation block matches. The next difference is the second product in a ring register instead of target v1, so controls use existing local identities rather than a new declaration grid.

2026-10-22: Matched all 702 instructions / 2,808 bytes. Existing `a` carries the second product, `offset` carries the rank curve and top-score shift, and `j` carries the final player correction. Explicit lower-bound delta and direct player-rank accesses restore the remaining arithmetic and reload. Renaming the unused former `last` word to `pad` preserves the match; removing it regresses stack homes (cost 42). Scalar order and the pad are marked FAKE. Promoted actual C with a same-TU void forward declaration. See drafts/T-25059/README.md and [[matching-notes]] for controls.

2026-10-22: Default linked C matches all 2,808 bytes; rebuilt ETC checksum is 681c93b6e57c885f86e074c885a8bf6c3cca2319. Header/global checks pass and prototypes report zero new / 15 known disagreements. Root independently reviewed the final source against CODING_STANDARDS.md and scope with no findings. Ticket and board moved to In Review pending root's independent full 27-binary integration gate. No fallback remains anywhere in this source file.

Root acceptance: independent default-C linked2808-byte MATCH, fresh27/27 hashes, header/global checks and0new/15known prototype disagreements pass. Final source directly reviewed against standards/spec with no findings. Done; entire80146400.c is now C. Worker continues ETC80144224 in80143240.c underT-25061.
