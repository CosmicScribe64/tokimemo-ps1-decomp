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

Full range trace: constant-address bit46 priority3 winsv0; quotient bit44 priority1.4 winsv1; selector bit53 priority0.333 reusesv0. Narrow-index controls are negative: s8=253/cost192, u8=248/cost211, s16=243/cost172. Reusing q/b as later call coordinates after definite assignments remains144 (142 with globaluse). Need a surviving quotient-to-use range boundary without added conversion instructions; simple local/width/argument reuse does not provide it.

### Quotient storage controls

Docker `build/T-25053/quotient-storage.py` completed: u16 quotient produces244 words/cost171; s16 produces247/cost204; reversed zero-assignment order remains243/cost144. Target243 words. None improves archived counter-use cost142. Narrow storage adds real conversion instructions, so it does not supply the missing target copy without additional changes.

### Typed counter and selector allocation improvement

New archived best `drafts/T-25053/signed-selector.c`:243/243 words, aligned cost104.
Signed b with explicit unsigned division recovers target b=a1 and q=a0 (138 alone).
Empty selector use before b initialization gives a2 (112 alone); combined104.
Counter's source is u8, preserving0..255 range. Selector FAKE is marked; no promotion.
Remaining first region still lacks quotient copy and schedules selector load early.
Empty selector use immediately before switch is neutral144. Signed zero-order136,
signed plus global empty use138. No production edits.

Follow-up: b-before-q zero reset improves archived signed-selector.c to102
aligned cost (243/243 words). Signed q and separate s32 index were neutral104;
combined narrow-index controls s16=132, u16=166, u8=177/248words are negative.

Quotient identity follow-up on102-cost source: q*1 in switch outputs is neutral102; q OR late-zero from b worsens112, and from selector worsens138. All remain243words. None restores target copy without other changes; production unchanged.

Counter alias follow-up: scalar u8 view on load worsens102→142; store-only view is neutral102; both views142 (all243words). Static-load classification is needed for current best allocation; no production edit.
