---
id: T-24005
title: Match BUNKAKEN date state machine func_80143DD0
status: In Review
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[bunka-family-results]]", "[[matching-notes]]"]
---

## Goal

Match only func_80143DD0 in src/ovl/BUNKAKEN/80143DD0.c using current family types and preserved archives. Bounded 25-minute pass; do not reopen the T-24003 loader allocation experiments.

## Acceptance criteria

- Literal default linked proof, affected checksum and guards for any accepted match.
- Preserve useful unsuccessful source and precise remaining blocker.
- Independent standards review before Done.

## Notes

T-9960 reported an eight-line near miss (signed-halfword nibble cast, explicit b2 == 1 and direct selected-character accesses); later T-18500 typed archive scores 330. Search for the earlier near-miss source before rebuilding it.

## Comments

- Recovered the historical near miss from the current T-18500 archive: corrected calls/flags/signed nibble cast (82), direct selected-character accesses (66), narrow selector (8). Guarded source now preserves the result; two required self-stores carry FAKE comments. No verified C match, zero new C bytes.
- Remaining words: 0x80144824/34 mask into t5 instead of t4; 0x80144838 compares t5 instead of t4; 0x80144840 shifts t5 into itself instead of t4 into t5. All subsequent instructions agree. Actual instruction size is 3,232 bytes, queue 3,532 included tables.
- `drafts/T-24005/README.md` records provenance and all bounded failed forms. No new exhaustive/permuter pass was run. Parent requested preservation and an Astra closer handoff at this point.
- Validation: guarded full-TU funcdiff confirms exactly 8 rows / 4 words (`build/t24005/guard-proof.txt`). Explicit default-version linked proof passes for the 3,232-byte assembly fallback (`build/t24005/default-proof.txt`); this is not a C match. Default Docker ninja -j1 and BUNKAKEN SHA1 pass. Headers/globals pass; prototype branch check has 0 new / 15 known disagreements (`build/t24005/guards.txt`). Submitted In Review for independent review; closer must resolve the four C words before claiming bytes.
