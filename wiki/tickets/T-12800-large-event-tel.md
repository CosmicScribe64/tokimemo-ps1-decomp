---
id: T-12800
title: Large EVENT TEL and main matching
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]"]
---

## Goal

Recover byte-identical C in the assigned files; prioritize largest functions using full-TU evidence.

## Acceptance criteria

- Resolved full-TU byte proof and affected binary SHA1 for each win.
- Review against CODING_STANDARDS.md; final 27 checks.

## Notes

Isolated branch codex/t12800-large-event-tel, origin/main 95c3fc9f. No changes to original checkout or other workers. Parent T-12800; scope main/8006CB30.c, EVENT/8010FE10.c, TEL/80132000.c. At most two concurrent compilers.

## Comments

## Handoff

Branch codex/t12800-large-event-tel at /Users/lila/Documents/Projects/tokimemo-wt/t12800. Commit c0cc3335 adds 644 verified bytes in date_sasoi_judge. Parent/children now In Review for final build and evidence review. EVENT and TEL source/header restored exactly; child tickets retain negative findings and local scratch references. Main large dispatcher generated and aggregate-migrated scratch only; unresolved pointer-stride/type recovery is not a matching claim. Larger global 80% objective is not reached by this batch.

Final checkpoint: 27/27 binary SHA1 hashes pass; prototype branch check has zero new disagreements; headers and global migration checks pass. Standards and separate spec review found no outstanding issues. Investigation batch complete; only date_sasoi_judge gained matching bytes (644). Evidence is local and gitignored under build/t12800-evidence/. No claim of global 80% completion.
