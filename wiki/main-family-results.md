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

## Background sprite checkpoint

Three related renderers add **5152 default literal linked bytes**: func_800578F4 (1064), func_80057D28 (1648), func_80058398 (2440). MAIN SHA1 passes. Proof: build/main14200/bg-proof.log and bg-build2.log.

The decisive source repair is one reused SPRT pointer across successive packets, rather than m2c's separate short-lived packet values. Shared table-pointer comparisons in the draft were actually comparisons of the incoming mode with 1/2; those now use the argument directly. The repository already defines SPRT in include/psx_types.h; it is reused, with a PsyQ-compatible setWH comma-expression helper. Paired size assignment and x/y-before-UV store order close the remaining scheduling differences.

**Two functions are FAKE:** 57D28 and 58398 each need one unused s32 word before the table-pointer local to reproduce the target's +0x3C spill home and 0x50-byte frame. Without it all non-frame instructions match, but 24/32 frame/home rows differ. Their source comments explicitly label this T-14200 workaround; the real extra local remains unknown. The 1064-byte 578F4 function uses no FAKE. Cumulative gain: **5 functions / 6624 literal bytes**, comprising **2536 bytes without new FAKE and 4088 bytes in two labeled frame-workaround functions**.

An initial duplicate SPRT declaration in libgpu.h was rejected by the complete MAIN build; it was removed in favor of the existing shared type. The subsequent build and literal proofs pass. A source audit confirms all 22 pre-existing C bodies in the two touched files remain byte-for-byte identical as source (build/main14200/body-audit.log).

## Additional bounded negative evidence

- Neighboring initializer 419FC improves 36 -> 22 -> 14 rows by correcting its byte clear, one-based index and loop increment in the body. Return-type and consistent-pointer probes do not close the remaining aggregate-base register and clear/store scheduling gap. No promotion.
- Scheduler 6D138: existing CharFlags casts 347 rows, consistent GameState pointer 262, aggregate flag-copy forms 329/394. Entry speculation/control-flow remains unresolved. No header change retained.
- Input 42CB0: packet bytes through existing controller arrays and canonical GameState fields 330 rows, pointer 324. Still broad allocation/control-flow differences.
- Numeric 4AE54: abs argument/texture byte-pair repairs identified, but initial scratch compile failed due an uncorrected void-pointer texture local. No match claim. The prior spill-order evidence remains applicable.

## Final review and validation

Both code-review skill axes (GPT-6.1 Sol medium) pass after removing redundant signed RGB casts. The final source passes all five full-TU default literal proofs (build/main14200/final-proof.log) and **27/27 freshly rerun binary SHA1 checks** (final-confirm-build.log). The complete shared-header build also passes (final-build.log). Headers/globals pass on final source; prototypes report 0 new / 15 known disagreements and branch check OK. All 22 old C bodies remain verbatim. No reserved-file, overlay-source, generated-ASM, compiler or coverage changes; no push/PR.

Final isolated-branch census: 5887/6958 game functions, 1414152/2279368 game bytes (62.0%). Accepted gain is 6624 literal bytes: 2536 without new FAKE and 4088 with the two labeled frame-home locals. This bounded checkpoint is complete; the broader 80% goal and remaining MAIN work are ongoing. Integrate the entire T-14200 commit chain, including the final review corrections, before relying on the source-plausibility result.
