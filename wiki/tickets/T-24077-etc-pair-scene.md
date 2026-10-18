---
id: T-24077
title: Recover ETC scene with actual two-word table indexing
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-17300-types-first-etc]]", "[[tickets/T-24075-etc-random-selection]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_8013CF2C (824 bytes), src/ovl/ETC/8013BEF0.c. Preserve all neighboring C and unresolved archives. Latest primary target remains ASM; no current claim/later T-240xx packet found.

## Acceptance criteria
- Read latest primary history and original assembly, acknowledging old T-17300 scores165/149. Establish a fresh faithful full-TU baseline before source-led controls.
- Correct retained whole-array arithmetic on actual s32 D_8014DBF8[13][2]: target uses eight-byte rows and two word loads. Use direct row/column access, not girl*8 scaled by the whole array. Verify exact target addresses/argument order first.
- Preserve actual one-argument matched E1DC helper, original signed counter field/unsigned comparisons and Rec24[64/65] flag semantics; do not infer siblings from same-address functions in other overlays.
- Same sole Sol medium Docker compiler/ninja -j1, built checkout from73bb13726 on codex/complete-etc-pair-scene. No extra agents/instrumentation or broad sweeps.
- Coordinate exact shared-header/config scope before changes. Actual C needs default literal824-byte proof, ETC hash/interfaces and exact neighbor audit; parent independently reviews/all27. Replay must support current matched checkout after promotion.

## Notes
New verified T-24075/F320 direct-pair call idiom supplies concrete source evidence beyond old drafts. A natural switch may remove inherited nested-default artifacts only if original control flow supports it. No match promise; preserve T-24076 as unresolved In Review with all boundary/type caveats.
