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
- Correct the typed archive whole-array arithmetic on actual s32 D_8014DBF8[13][2] (plain archive already has correct row selection): target uses eight-byte rows and two word loads. Use direct row/column access, not girl*8 scaled by the whole array. Verify exact target addresses/argument order first.
- Preserve actual one-argument matched E1DC helper, original signed counter field/unsigned comparisons and Rec24[64/65] flag semantics; do not infer siblings from same-address functions in other overlays.
- Same sole Sol medium Docker compiler/ninja -j1, built checkout from73bb13726 on codex/complete-etc-pair-scene. No extra agents/instrumentation or broad sweeps.
- Coordinate exact shared-header/config scope before changes. Actual C needs default literal824-byte proof, ETC hash/interfaces and exact neighbor audit; parent independently reviews/all27. Replay must support current matched checkout after promotion.

## Notes
New verified T-24075/F320 direct-pair call idiom supplies concrete source evidence beyond old drafts. A natural switch may remove inherited nested-default artifacts only if original control flow supports it. No match promise; preserve T-24076 as unresolved In Review with all boundary/type caveats.

Read correction before baseline: retained PLAIN165 already selects the actual
D_8014DBF8[girl] row correctly; only typed archive has malformed whole-array
arithmetic. No untried overall pair-indexing fix is claimed. Original full VRAM
8013D118..8013D12C confirms eight-byte row/two words; B118 was ROM-offset shorthand.
Fresh baseline uses actual fields, correct row pointer/columns and real Rec24
flags64/65. Scratch-only actual one-word void E1DC/F320 forwards; no shared edit.

Fresh current full-TU row-pointer baseline132/frame28. It exposes a necessary
state-byte view: shared GameState.unk_110C is s8, so direct increments use LB,
while original target reads all state values with LBU. Next exact u8 address
view preserves original byte reads/writes without changing the shared header.

Unsigned state view reaches122; direct-pair call and natural default are neutral
122 and whole-object identical. Target-only s32 fallthrough126 rejected, no
helper/caller overrides; current ETC caller407F0 ignores result. Matched BFFC
word-join alone122, complete consistent-GameState plus word-join recipe102;
explicit FAKE graph purpose. Original206 words/frame28 versus best206/28.
Remaining girl/case2 statusv0-v1, later scratchrotation and early terminal
constant scheduling gaps; source-only packet/replay drafts/T-24077. Production/
headers/config untouched, active In Progress, no match/impossibility claim.
