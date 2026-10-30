---
id: T-39026
title: Contact sheets and human visual review queue
status: In Review
assignee: playtest-qa agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[phase4-design-2026-10-29]]", "[[port-testing]]", "[[tickets/T-39008-synchronized-differential-captures]]", "[[tickets/T-39004-english-asset-completion]]", "[[tickets/T-30001-modern-widescreen]]", "[[tickets/T-30002-modern-settings]]"]
---

## Goal

WP-11: contact-sheet rows for checkpoint review, and a local queue whose verdicts are bound to image hashes.

## Acceptance criteria

- [x] Row cells: native, port-ja, port-en, port 16:9 full window, exact diff heatmap, rgb555 residual. Cells are 2x nearest, labelled with key, sync status, defect id and hashes, with linter findings drawn as boxes (`tools/port/contact_sheet.py`).
- [x] Queue items are bound to image sha256, with human states pending, approved, rejected, needs-fix and needs-art. A hash change invalidates the old verdict on every run, whatever the cap, and refreshes findings, defect id, kind, severity and image path (`tools/port/progress/review_queue.py`, `tools/port/schemas/visual-review.v1.schema.json`).
- [x] Agent notes are kept separate from human verdicts. An item is objective only when its row carries linter findings (`kind: objective` without findings is refused). Agents may close objective items only, and only as `resolved-objective`, never `approved`; `status` reports counts split by `closed_by`. The `verdict` command is for the human reviewer only (`--reviewer` is audit metadata, not authentication).
- [x] Each run is capped and sorted by severity (the cap limits what is surfaced and published, not invalidation); the queue is served by the unchanged `tools/port/progress/server.py` from a published media directory.
- [x] Output stays under `.asset-cache` only (enforced); unit tests use synthetic images (`tools/port/test_contact_sheet.py`, 13 tests).
- [ ] First real queue (needs WP-02 receipts and WP-04 suite output).
- [ ] Code review against CODING_STANDARDS.md.

## Notes

The rgb555 residual is diagnostic. `compare_capture_pair.py` stays the only parity gate.

## Comments

- 2026-10-30 (playtest-qa, review fixes; agent review, not human approval): fixed a stale approved verdict surviving a hash change past the per-run cap, stale findings/defect id on reopen, agent closes as `approved`, unbacked `objective` kinds, and `status` mixing agent and human closes. Queue files written before this change may hold an agent close as `approved`; none exist outside the `.asset-cache/phase4-playtest-qa/contact-demo-queue.json` demo, which has no closes.
