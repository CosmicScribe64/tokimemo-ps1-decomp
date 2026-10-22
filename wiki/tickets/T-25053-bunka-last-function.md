---
id: T-25053
title: Match final BUNKA_SD function
status: In Progress
assignee: orchestrator
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-24516-overlay-family]]"]
---

## Goal

Match func_80134D94 in src/ovl/BUNKA_SD/80134540.c. Start from repaired T-24516 semantics, not the old guard's wrong polarity, coordinates and argument order.

## Acceptance criteria

- [ ] Reproduce current whole-TU baseline and causal remaining differences.
- [ ] Literal default-C match, full binary gate, declaration checks and independent review.

## Notes

Root owns this separate source. Read T-24137/T-24516 and function index before experiments. Existing guard remains untouched until verified replacement.

Fresh Docker replay on current main reproduces243/243 diagnostic words,144 aligned cost/368 printed rows from repaired T-24516 source. This is not a literal match; original function scope is960 bytes and diagnostic span includes padding. No new source control yet; preserve corrected semantics when investigating counter promotion and switch-copy allocation.

Targeted source controls: declaring first counter b as u8 instead of u32 and adding an empty quotient-lifetime test each preserve243words/cost144. First divergence begins at counter/quotient/selector register allocation; target quotient is copied from a0 to v0 before switch, draft keeps one range. Next investigate separate quotient-use lifetime rather than byte-width changes. No source edit retained.

Separate signed/unsigned index locals assigned from q fold back to baseline243/cost144; u16 index worsenscost200. Late-zero addition plus an explicit later empty q use still gives144 for signed/unsigned index. These source identities do not preserve the target quotient copy, so next inspect optimizer ranges before more controls. Private scripts and outputs remain build/T-25053; production untouched.

Optimizer trace: local b bit39 priority0.6 allocated before global counter/address bit41 priority0.5. Extra global use changes b to targeta1 but address to a0; cost142, still243words. Signed quotient has no further effect. Preserved diagnostic counter-use.c and concise causal/negative ledger in drafts/T-25053. Next trace quotient/selector interference; do not repeat exhausted width/index spelling controls.
