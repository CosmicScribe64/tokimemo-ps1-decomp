---
id: T-39028
title: Original-free port CI job
status: In Review
assignee: playtest-qa agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[phase4-design-2026-10-29]]", "[[ci]]", "[[port-testing]]", "[[tickets/T-30001-modern-widescreen]]", "[[tickets/T-30002-modern-settings]]", "[[tickets/T-39004-english-asset-completion]]"]
---

## Goal

WP-13: a T0 job, labelled "original-free; not fidelity evidence", that runs the Python tooling tests, schema validation and the translation-asset lints, with the same set runnable locally in Docker.

## Acceptance criteria

- [x] `.github/workflows/port-offline.yml` added. It was not pushed and not enabled remotely.
- [x] `tools/port/run_offline_suite.sh` runs the same set in `tokimemo-recovery`: 310 unittests, schema validation (pairs not yet in the tree print UNAVAILABLE) and the lint. It passed on 2026-10-30, and a negative copy containing an orphan image fails with exit 1.
- [x] `tools/port/lint_translation_assets.py` with `tools/port/test_lint_translation_assets.py` (13 tests). The committed `translations/en` passes.
- [x] [[ci]] updated.
- [ ] Godot headless parse and the GDScript suites in CI: waits for user approval.
- [ ] Code review against CODING_STANDARDS.md.

## Notes

`tools/port/schema_check.py` validates with jsonschema when it is installed (CI). Locally, no permitted image has jsonschema, so it falls back to a strict subset validator that fails on any keyword it does not implement.

## Comments

- 2026-10-30 (playtest-qa, review fixes; agent review, not human approval): the lint now (1) fails a checkpoint ledger entry with status `approved` or `human_approved: true` unless the latest style event for each listed image's hash is `approve`; (2) compares protected markers for every non-empty translation, so a translation that drops every marker fails; (3) walks every PNG under the pack root, not only `batches/` and `checkpoints/`. Each of the three review mutations on a copy of `translations/en` (ledger flip, all markers dropped from `talk:CDROM/BUNKA/AS_S6.BIN:main:g0000:l0000`, `graphics/artwork/extra/unreviewed.png`) now exits 1; the committed tree still passes. Offline suite: 310 tests, PASS.
