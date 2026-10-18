---
id: T-24005
title: Match BUNKAKEN date state machine func_80143DD0
status: Done
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

- Astra bounded closer started: test new selector lifetime hypotheses against the four-word residue; retain fallback unless literal C proof passes.

- Astra lifetime probe: narrow selector plus an empty test immediately before the switch scores 0 in full-unit resolved comparison. Word-selector empty tests score 72, versus their 66 baseline. Installed the narrow form with a FAKE comment; literal default verification pending.

- Closure validation: unguarded default C reports `MATCH (linked function bytes equal target (3232 bytes))` with `python3 tools/funcdiff.py --unit BUNKAKEN --version default --resolve func_80143DD0`. `ninja -j1 build/ovl/BUNKAKEN.ok` passes SHA1. `check_headers.py include src`, `migrate_globals.py --check`, and `sync_protos.py --check-branch` pass (0 new / 15 known prototype disagreements). Gain is 3,232 verified C bytes.
- Closer stopped after 14 sequential full-unit probes at the first exact candidate. Three marked FAKE devices remain: two call-delay self-stores and one empty selector test. No build flags, passes, or generated assembly changed. Self-check against CODING_STANDARDS.md found no new issue; independent parent review is pending, so status is In Review.

## Independent integration review

2026-10-17 root: reviewed the full added C against CODING_STANDARDS.md. C89, fixed-width temporaries, existing shared aggregate accesses, explicit K&R view, and all three FAKE statements explained. No generated assembly, compiler exceptions or raw data changes. Integrated default literal proof matches all 3,232 bytes; all 27 binary hashes and header/global/prototype guards pass (0 new prototype disagreements). Published c0662e257. No unresolved findings; Done.
