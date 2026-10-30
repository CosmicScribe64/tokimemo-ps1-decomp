---
id: T-39210
title: English WP-B: translated image store v2 (page and BS rows)
status: In Review
assignee: Claude (phase 4 root, Lane A)
created: 2026-10-30
updated: 2026-10-30
links: ["[[phase4-design-2026-10-29]]", "[[tickets/T-39004-english-asset-completion]]"]
---

## Goal

Add the v2 image pack schema (frame, page and bs rows) with v1 back-compat, and the page_replacement / bs_replacement runtime API.

## Acceptance criteria

- [ ] v1 packs load unchanged; v2 rows are validated (kinds, regions, STP, identities).
- [ ] `run_image_store_v2_tests.gd` and the Python pack tests pass in Docker.
- [ ] Independent review against CODING_STANDARDS.md with findings resolved.

## Notes

Merged into main by the phase 4 wave 1 integration (`.asset-cache/phase4-patches/phase4-wave1-combined.v2.patch`). No presenter calls page_replacement or bs_replacement yet (root hook 8: FILESEL pages and BS signage). Agent review only; not human approval.

## Comments
