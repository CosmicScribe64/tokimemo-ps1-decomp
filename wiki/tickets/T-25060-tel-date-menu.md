---
id: T-25060
title: Match final TEL date menu builder
status: Done
assignee: Codex
created: 2026-10-22
updated: 2026-10-26
links: ["[[tickets/T-24511-tel]]", "[[tickets/T-25055-tel-pass-mechanisms]]"]
---

## Goal

Match TEL func_8013D88C (2,972 progress bytes, 2,980 linked bytes including eight trailing padding bytes) in src/ovl/TEL/8013CCD0.c. Root explicitly transferred exclusive ownership after T-25055; func_80134FE4 is already matched and the previous owned translation unit is complete. Continue on codex/t25060 from clean596c2c269 without resetting or rebasing integration work.

## Acceptance criteria

- [ ] Default C literal linked MATCH and coordinated27-binary checksums.
- [ ] Header/global/prototype guards and direct standards/spec review.
- [ ] Preserve causal controls and exact metrics; no unsupported coverage claim.

## Prior evidence

T-24039 repairs halfword/flag pointer strides, text-table indexing and dispatch extents. T-24511 additionally recovers page-two exclusive branching, arg2 case14 bypass and packed-word load placement. Current archived candidate drafts/T-24511/menu-reconstruction.c has755 words against745 target, frame232 against224. Target saves the x-array pointer while the draft saves the flag pointer; y/step pointers spill in different homes. Avoid prior declaration, month, generic source-shape and volatile-pointer grids. Capture current full-TU allocation and frame evidence first. Docker-only, coordinated compiler slot, at most j2, no shared headers or compiler changes.

## Comments

2026-10-22: Claim created before experiments. Parent confirmed exclusive transfer to the separate TEL8013CCD0 TU; former source80132000.c has no remaining INCLUDE_ASM or NON_MATCHING. Compiler slot currently yielded; preparing read-only history and replay.

2026-10-22: Root integrated T-25055 as Done and pushed mainc18c8dcb8 after independent gates. TEL is64/65 functions,47,228/50,200 progress bytes. The remaining2,972 bytes exclude the final two NOP words; literal matching still covers2,980 bytes. Isolated branch retains its exact prior pin while this qualification proceeds.

2026-10-22: Current full-TU baseline reproduces 755 words against 745, with 689 changed positions and 798 aligned cost. Its final default object and native traced comparison both allocate 240 stack bytes, against target 224. The prior 232-byte statement describes earlier controls and must not be applied to this preserved packed-word body. The current flag pointer already gets v1; the x coordinate and packed-word local share an extra saved s2 range (priority 14), shifting later saved allocations. Reusing dead arg0 for the three packed-word captures reduces the body to 751 words, cost 786; narrowing the now separate coordinate x to s16 is byte-neutral. Both remain unpromoted diagnostics. Native allocation capture will distinguish removed range interference from unrelated frame placement.

2026-10-22: Independent lifetime controls: arg0 capture removes the extra saved range but flags then takes fp; xs false-read gives 750 words/cost 851, index false-read 747/cost 804. Negative literal multiplication preserves the target negative quotient plus negative-multiply macro: 759/cost 792 on baseline, 755/cost 770 with arg0 capture. These are mechanism probes, not preferred semantics or proof. Current source uses signed word aliases in the first three page-one packed-bit tests and unsigned aliases in later tests; only those first three emit separate absolute addresses where the target shares a GameState base. Preparing an unsigned-load identity control, also removing signed-left-shift undefined behavior in those expressions. Compiler slot released at completed controls for ENDING/NAME_ENT.

2026-10-22: All three unsigned-load controls are byte-neutral: signedness did not cause the address splitting. Naming a separate page-one GameState pointer recovers base-relative loads but adds pressure (776 words/cost 797), so it is not adopted. Combined xs/index reads with the negative-literal multiply give756/cost863; capturing allocation to identify the interaction before another source change. Precisely, the baseline pre-framepass binasm frame is224 and final240; target final224.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
