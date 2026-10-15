---
type: research
updated: 2026-10-15
---

# MAIN source family recovery

Scope: [[tickets/T-14200-main-source-families]], branch codex/main-14200 from 0f117230. MAIN only; 8004F870.c and 8007C030.c reserved. Original checkout remains read only. Existing Docker image, serial compiler probes.

## Initializer checkpoint

func_800412E0 adds **676 literal linked bytes**. Full-TU default funcdiff reports MATCH; MAIN SHA1 passes (e823bd844a8f8fa4d05483b59c66bc54b8393b26). Evidence: build/main14200/412-proof.log and 412-build.log.

Canonical GameState fields and the actual byte/halfword views of unk_0F8 reproduce the original initialization. The word store at +0x1098 overlaps the menu byte array and uses its canonical address. Preserve mask 0xFFF0: shortening it to 0xF0 is behaviorally redundant for the byte but differs in one instruction. Direct fields and a local GameState pointer both score two rows before restoring the original mask. No new FAKE. Added only the shared void declaration of the existing func_80041840 definition, avoiding implicit-int conflict when the initializer becomes C.

## Rejected focused probes

All drafts below remain ignored in build/main14200; no production/header changes retained for them.

- Appointment func_80076FDC: two eleven-row, two-byte tables and direct switch stores compile, but produce 252 rows; shared value-local and original goto forms give 264/341. Not a match. Table declarations removed.
- Telephone func_80077900: bitfield tests at bits 1/2/18 and month bit 29 with canonical GameState fields give 161 rows, consistent aggregate pointer 152. Helper-result narrowing and allocation remain. Temporary views removed.
- Parameter parameter_up_down: signed nine-byte activity/club rows compile after canonical forward declaration, but give 177 rows; input-parameter reuse 167, broader exclusive-branch local reuse 220. No promotion or header changes retained.
- Setup func_8007437C: direct canonical day-array fields compile at 85 rows, aggregate pointer 99; widening result locals and return produces 240/234. The target uses word spill homes, unsigned byte reads and a word return. Temporary prototype correction removed pending a complete match.

These are full-TU diagnostic row counts, not literal matches. Historical evidence read first: [[matching-breakthroughs]], [[bunka-family-results]], [[etc-family-results]], [[broad-coverage-results]] and referenced MAIN tickets. Do not repeat the same source forms without new evidence.

Initializer guards pass: headers OK, globals OK, prototypes 0 new / 15 known, branch check OK (build/main14200/412-guards.log). Review remains pending while further families are investigated.

## Character initializer checkpoint

func_80041C2C adds **796 literal linked bytes**. Using one local GameState pointer for every access with existing CharFlags b1/b3/b6/b7/f9 stores changes the full-TU score from 366 rows to zero. A new month/day bitfield view also scores zero with the pointer but is unnecessary and was removed. No header layout change or new FAKE is needed. The explicit month/day byte/halfword masks retain the original stores. Evidence: build/main14200/41c-proof.log, 41c-build.log and 41c-guards.log. Both new functions pass default literal verification together; MAIN SHA1, headers and globals pass. Cumulative accepted gain: **2 functions / 1472 bytes**.

## Review correction to initializers

The first standards/spec reviews found the new prototype outside the include guard, unexplained pointer-to-s32 strncpy casts, and redundant literal mask spellings whose original-source plausibility was not established. These have been removed. The final initializer source uses GsDateBits month/day fields for the birthday word and Rec38 +0x10, and CharFlags.f20 for the initialized nibble. Existing word/byte/halfword views and record sizes remain unchanged. The prototype is inside MAIN_API_H; strncpy receives pointers directly. Ordinary complemented-mask probes failed (store scheduling and a widened AND), so they were rejected. Both final bitfield functions retain default literal MATCH and MAIN SHA1 OK (build/main14200/bitfield-proof.log and bitfield-build.log). No FAKE is needed for the initializer pair. Re-review and final guards follow.
