---
id: T-39211
title: English WP-C: build the installed English bundle
status: In Review
assignee: Claude (phase 4 root, Lane A)
created: 2026-10-30
updated: 2026-10-30
links: ["[[phase4-design-2026-10-29]]", "[[tickets/T-30002-modern-settings]]"]
---

## Goal

`tools/port/build_english_bundle.py` writes `<installed>/language/en/english_bundle.json` (ui_text, dialogue, images, optional draft images) that LanguageBundleService opens.

## Acceptance criteria

- [ ] Bundle schema and member pins as in the WP-C notes; Python tests pass.
- [ ] `run_language_bundle_built.gd` passes with ENGLISH_BUNDLE=<installed>/language/en after setup_content.sh (root hook 9).
- [ ] Independent review against CODING_STANDARDS.md with findings resolved.

## Notes

Reference build: `.asset-cache/port/english-bundle-v1`. Root hook 9 (one end-to-end setup plus verify_desktop.sh on a bundle with language/) is still open.

## Comments
