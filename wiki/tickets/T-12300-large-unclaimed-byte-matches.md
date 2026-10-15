---
id: T-12300
title: Match substantial unclaimed functions
status: In Progress
assignee: Codex large-byte-matches
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]"]
---

## Goal

Prioritize verified matched bytes in substantial functions outside all existing object claims.

## Acceptance criteria

- [ ] Establish exclusive object ownership before source edits.
- [ ] Verify retained matches with literal linked bytes and unit SHA1.
- [ ] Full 27-binary and prototype checks at handoff; ticket In Review and clean commits.

## Notes

Baseline 7a65b3d8. Main and all other worktrees are read-only. Exclude cx1–cx10 headers, w1/bm/TT, existing worktree changes and acceleration T-12230 manifest. Ownership census in build/large-bytes; no C files claimed yet. Docker limited to one or two jobs.

## Comments

2026-10-15: Read index, standards, local instructions, workflow/register notes and external matching research. Acceleration has expanded to a 101-function broad sweep; its full source objects must be excluded, including unattempted siblings.

Owned source objects for this pass: `src/ovl/RPG_BAT/80145CC0.c`, `src/ovl/RPG_BAT/8014ADD0.c`, `src/ovl/RPG_BAT/8014A660.c`, `src/ovl/BUNKAKEN/8013BFE0.c`. Excluded RPG_BAT/80137280 despite census eligibility because T-9960 explicitly retains its claim. Also exclude BUNKASAI/801354B0 from acceleration's earlier cohort. Main remains untouched. No shared header changes planned.

Baseline passed all 27 SHA1 checks. First default-m2c pass attempted eight targets / 10,676 list bytes; no matches. Exact differing-row scores: BUNKAKEN 8013C634 124, 8013CA6C 119, 8013C0AC 130; RPG_BAT 8014BB80 214, 8014B294 501, 8014ADD0 284, 8014A660 103, 80145CC0 195. Drafts outside checkout at /private/tmp/tokimemo-large-results. Scores include unnormalized old global names, so normalize aggregates before interpreting them as structural blockers.

User requested subagents. Delegated BUNKAKEN/8013BFE0 and RPG_BAT/80145CC0 to one compiler worker each; third worker reasons about RPG_BAT/8014ADD0 and 8014A660 while waiting for a compiler slot. Parent serializes headers, validation, commits and wiki. Maximum two compiler jobs across workers.

Verified BUNKAKEN gain: **3,604 literal linked bytes / 3 functions** (8013C0AC 1,416; 8013C634 1,080; 8013CA6C 1,108). Consistent initialized GameState pointer access plus existing CharFlags bit2 reads/stores solved this family. No FAKE needed. Full-file funcdiff literal proof and BUNKAKEN SHA1 passed. Handoff build/large-bytes/bunkaken/handoff.md. Two canonical overlay declarations added; no shared main API changes. RPG workers continue.
