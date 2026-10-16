---
id: T-13101
title: Reconstruct ETC career state handlers
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-13100-etc-families]]"]
---

## Goal
Test adjacent remaining state handlers in src/ovl/ETC/801492C0.c with bounded source-form variants.

## Acceptance criteria
- Exact full-TU scores, literal default linked bytes, ETC SHA1.
- Preserve pre-existing C; retain rejected variants in ignored build/etc13100 only.

## Comments
2026-10-15: Claimed before drafting; first three adjacent candidates then bounded held-out siblings.

2026-10-15 checkpoint: four literal MATCH functions: 801492C0 (212), 80149AC4 (252), 8014A534 (416), 8014A8A8 (252), total 1132 bytes. ETC SHA1 OK; proof build/etc13100/career-proof.log and career-build.log. Explicit void declarations recover forward calls to existing void definitions. Real pointer arrays recover scaled table loads.
Rejected: 8014A360 remains four listing rows (string/store HI16 scheduling) after ordinary local initialization and delayed else assignment fix its spill. No FAKE variant promoted. 80149F48 structured reload/load-order hypothesis improves 176 to 140 rows but remains nonmatching. Both remain original assembly; scratch only.

2026-10-15: Final combined proof retains all four accepted functions / 1132 bytes. Existing C body audit passes. No rejected career variants promoted. See [[etc-family-results]].

2026-10-15 review: repository code-review skill applied to aa2b3158...aa1c51ec. GPT-6.1 Sol medium reviewers: standards axis no findings; spec axis no actionable findings. Documentation observation about missing prototype result resolved here: sync_protos.py --check-branch passed, **0 new / 15 known disagreements**, branch check OK. Header/global guards pass; all eleven combined default literal proofs and ETC SHA1 pass. Existing-body preservation audit: 91/91 unchanged. No production edits after reviewed commit. Ticket complete within its bounded ETC scope; broad game-wide 80% goal remains ongoing.
