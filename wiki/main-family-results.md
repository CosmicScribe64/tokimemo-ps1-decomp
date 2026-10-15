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
