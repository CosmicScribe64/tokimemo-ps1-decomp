---
id: T-13103
title: Reconstruct ETC memory-card state handlers
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-13100-etc-families]]"]
---

## Goal
Test consistent aggregate access and ordinary switch stores in the five remaining src/ovl/ETC/80140F50.c memory-card handlers and adjacent loader state siblings in 8013FF80.c.

## Acceptance criteria
- Full-TU scoring, default literal linked proof and ETC SHA1.
- Preserve existing C, keep rejected drafts in ignored scratch, no compiler/assembly changes.

## Comments
2026-10-15: Claimed before drafting. Prior exam family bounded tests yielded one dispatcher; array/CharFlags corrections left ranking at 40 rows, drawing at 336 rows. No random mutation planned.

2026-10-15: Source-form transfer yields full-TU zero for 80140FD8 (15 -> 0), 80141E10 (104 -> 0), and held-out 80142574 (104 -> 0), each using one GameState pointer consistently. 80140AE4 remains 48 rows and ordinary switch worsens (54/58); 8013FFAC remains 467 rows with pointer after recovering six-word local texture buffer, not promoted. Temporary shared byte declaration for this rejected draft will be removed. Extend the same bounded test to related card initialization func_80142B80 (called by 80143644).

2026-10-15 bounded negative: related 80142B80 remains 240 rows with either aggregate form; replacing false shared-goto stores worsens to 272. Rejected source/header additions and temporary main_api.h declaration removed. No main-header diff remains. Next work needs loop/buffer/type reconstruction, not repetition of these source-form probes.

2026-10-15: Accepted 80140FD8 (380), 80141E10 (924), held-out 80142574 (888): 2192 literal linked bytes with ETC SHA1 pass. Original C preserved; temporary main-header declaration and rejected album/init declarations removed. See [[etc-family-results]].

2026-10-15 review: repository code-review skill applied to aa2b3158...aa1c51ec. GPT-6.1 Sol medium reviewers: standards axis no findings; spec axis no actionable findings. Documentation observation about missing prototype result resolved here: sync_protos.py --check-branch passed, **0 new / 15 known disagreements**, branch check OK. Header/global guards pass; all eleven combined default literal proofs and ETC SHA1 pass. Existing-body preservation audit: 91/91 unchanged. No production edits after reviewed commit. Ticket complete within its bounded ETC scope; broad game-wide 80% goal remains ongoing.
