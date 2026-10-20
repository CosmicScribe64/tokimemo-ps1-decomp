---
id: T-24074
title: Recover ETC record scene with proven helper calls
status: Done
assignee: Codex
created: 2026-10-18
updated: 2026-10-20
links: ["[[etc-family-results]]", "[[tickets/T-24073-etc-record-init]]", "[[tickets/T-17300-types-first-etc]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_80144224 (796 bytes), src/ovl/ETC/80143240.c. Preserve verified4407C/441DC and all other functions. Parent exclusively reserves this target to the same single Sol medium worker.

## Acceptance criteria
- Read current wiki/prior negatives and original assembly; establish fresh valid full-TU baseline with current aggregates and actual helper arguments before ordinary target-only C controls.
- Correct old missing4407C argument: original44344 passes frame_counter/701, needed for proven arg14 positioning. Replace phantom4-arg441DC call with actual matched2-arg view; use real D5014C16-word order table and Rec24 layout. Verify every correction against assembly/current definitions.
- Same sole Docker compiler/ninja -j1, existing built tree froma84f8462e on codex/complete-etc-record-scene; no extra agents or instrumentation/broad sweeps.
- No helper/header/config changes without exact evidence and coordination. Actual C needs default unguarded literal796-byte proof, ETC hash/interfaces and exact neighbor audit; parent independently reviews/all27.

## Notes
New verified4407C exposes a semantic omission in the retained T-17300 draft, justifying reconstruction beyond old scores. No opcode sibling above0.7 or match promise. Parent published final T-24073 archive7837b7e4e, measured6515 functions/81.9%; preserve source and prior unresolved archives. Ticket uniqueness/current ASM checked before edits.

Fresh valid current full-TU baseline209, frame58 versus original38. Corrected proven helper args/table/Rec24 plus post-call frame-counter reloads: original reloads after helper groups, while old retained draft incorrectly cached stale initial frame. No production/interface edit. First source cleanup joins disjoint fade/spill aliases and phase temporaries before allocation work.

Bounded controls:209/frame58 baseline; joined203/48; directlookup186/40; unsignedframe187/40; quotient-first187; consistentpointer226 rejected; inlinephase166/38 best; signedquotient166 neutral (max6126914). Inlinephase removes copied remainder web and restores initiala3/orderindexv1. Remaining quotient a0/sp34 vs scratcht6/sp30, framecopya2, table-base bias/redundant-store and later fade/register gaps. Source-only forms/replay under drafts/T-24074; no instrumentation/production/helper/header/config changes, no actual C proof. In Progress pending focused coordination.

Parent-approved bounded post-review cursor control uses actual order pointer and
post-increment loads only in the interior table arm. Valid default-resolved
full-TU diagnostic241/frame40 is worse than166/frame38; orderindexv1 becomesa1,
quotient spillsp34 becomessp3C. Rejected and retained as source-only replay.
No other source choices, production or interfaces changed; ownership remains
active In Progress pending a concrete next lead, without further pointer sweep.

Final bounded result: UNRESOLVED, In Review, never Done or matched. Best166
retains semantic corrections and exact frame38; post-increment control241 is
rejected. Production remains assembly. No supported next faithful source form
is isolated, and this is not a compiler-impossibility claim. Stop further
probes/instrumentation; parent reviews the source-only handoff.

## Comments

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
