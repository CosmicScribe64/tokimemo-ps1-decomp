---
id: T-24073
title: Recover ETC typed record initializer
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-17300-types-first-etc]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_8014407C (352 bytes), src/ovl/ETC/80143240.c. Parent reserves it to this single-target lane; no other active ownership found. Preserve all neighboring functions and unresolved archives.

## Acceptance criteria
- Read current wiki/T-17300/T-10030 negatives, original assembly and matched MAIN80084E90 read-only reference. Establish fresh valid full-TU baseline before source-led controls.
- Fix known stale `&D_801217D0 + 0x48` array-object scaling: actual start is Rec24[2], stride0x24 through records2..4, paired with GameState10A5[2..4]. Preserve signed RNG%112 and exact update/store sequencing.
- Same sole Sol medium worker/compiler, built checkout from55ff6f88e on codex/complete-etc-record-init. Docker only/ninja -j1, no new agents or blind sweeps.
- No assumed header/config changes; coordinate exact shared scope first. Actual C requires default literal352-byte proof, ETC checksum/interfaces and unchanged-neighbor audit; parent review/all27 before integration.

## Notes
Matched MAIN80084E90 has the real Rec24 initialization fields/constants80/1F1/1000; it is type/semantic evidence, not an exact opcode clone. Retained old draft still has the wrong array-object scaling under current Rec24[] declaration. Fresh correction is authorized; do not claim a match from compilation alone. Parent published final T-24072 fe932f320 as unresolved In Review; no further4354C diagnostics.

Original incoming a0 is spilled atsp48 and compared to0xE. Existing ETC.h void(void) conflicts with actual s32 argument; only current matched caller43334 intentionally supplies no argument expression. Parent approves narrowly changing the header to voidfunc8014407C() with evidence comment, definitionvoid(s32arg0), preserving caller unchanged. Private scratch-only header view establishes fresh valid baseline129, not historical ERROR. Baseline has s16 local narrowing shifts/extra saved register/frame50 versus original48; production/header unchanged pending source improvement.

Measured source-led sequence:129 baseline, wordx124, wordnext_id115, matched unsigned SDK width84/exactframe48, directID64, early required y-store22, postincrementID20 best. Moving IDstore before pointeradvance30 rejected. Best20 has original instruction count/frame/allotherwords; only i++ lies after9-instruction signedDIV guard rather than before it. Source/controls/private-header replay retained under drafts/T-24073. Production/header unchanged; no actual C proof/verified bytes. In Progress pending one focused scheduling hypothesis.
