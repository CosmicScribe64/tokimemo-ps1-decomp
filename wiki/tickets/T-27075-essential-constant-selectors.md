---
id: T-27075
title: Close the remaining essential constant selector gaps
status: Done
assignee: constant-selectors
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[rename-tool]]", "[[tickets/T-27066-remaining-overlay-completion]]"]
---

## Goal

Close the six concrete rule groups in the bounded essential-constant triage, using
existing tool routes where possible. Add only the indexed-member and compound-add
selectors that are required for the remaining array masks and step delta. Preserve
the behavior of all existing selectors and never rewrite literals outside rename.py.

## Acceptance criteria

- [x] Indexed member and compound-add matching are exact and caller-bounded, with unchanged existing selector behavior.
- [x] Regression coverage includes wrong caller/index/operator, suffix/sign preservation, alias resolution, replay and refusal before writes.
- [x] Proven existing girl/club IDs are reused; thresholds, masks and step delta have truthful names and exact site/count gates.
- [x] Independent tool and semantic reviews pass; source widths, FAKE expressions and unrelated literals remain unchanged.
- [x] Actual application, 437-unit declaration proof and matching 27/27 check pass before Done.

## Notes

Ignored triage: .claude/orchestrator/state/next-integration/constant-wave/essential-triage.md.
The six groups cover ending byte masks and known heroine indices, stage-show club IDs,
liberal-arts eligibility, saved-stage step skipping and RPG guard timing. This is not
an instruction to name all 532 unresolved inventory rows. Root serializes integration;
the initial 694-site worker lease finished before the live tool changes were applied.

Independent Sol tool review approves the ignored selector patch SHA-256
87c271b47ead7939ca7a95419eb6ec8193f6969710baed72788c8ae489e4dac4.
The 137-test candidate includes main-owned field renames seen by overlay callers;
restoring a numeric literal after that rename makes check/replay refuse without
further writes. Typed-owner forms remain unsupported. This approval does not claim
live integration, semantic packet review or the final matching proof.

Root integrated the approved selector and symbol-index tools; 144 combined tests pass.
The seven-request final31 packet is independently approved at runner SHA-256
b76f8ccef564cd5e22f2080b24f5dff0479d268b77a684f4e10ce5d84ed00852,
with fresh plan1405c7d9a8ecf173963c16816a16d16e6d28a0bd6ccba127daffc0884240b5e8.
The school-trip threshold uses the ADR-compliant TRIP prefix. Its source application,
725-site combined proof and clean matching check now pass.

## Verified completion

Independent actual-source review passes; the combined725-site source proof has437 exact ABI views and no unexpected paths. Clean Docker configure/build/progress and every guard pass, independent binaries27/27,6952functions,2276920bytes; naming guard12984 rows. Evidence: ignored next-integration/constant-wave/{source-proof,clean-proof}.json and actual-source-review.md. This closes the bounded ticket, not all of phase2.
