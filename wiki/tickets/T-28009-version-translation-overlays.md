---
id: T-28009
title: Version English translation overlays for repository backup
status: Done
assignee: root
created: 2026-10-25
updated: 2026-10-25
links: ["[[assets-text]]", "[[tickets/T-28007-complete-text-and-translator-handoff]]"]
---

## Goal

Preserve the user-supplied English work in Git without extracted Japanese text, original assets or changes to Gemini's working files.

## Acceptance criteria

- [x] Copy the already validated main target-only return with stable IDs, control markers, notes and trusted source digest intact.
- [x] Preserve supplemental target text and locator/context metadata separately from unverified original-byte coverage claims; omit Japanese/copy-template fields.
- [x] Record provenance, exact counts/hashes and remaining image-catalog backup limitations.
- [x] Independently review the tracked payload and documentation before commit/push.

Independent publication review PASS: `.claude/orchestrator/state/T-28009/publication-review.md` and reproducible Docker proof. Four files in `translations/en/` preserve the exact 55,403-row return and 508 supplemental projections. Manifest records hashes and explicit coverage limits. Gemini's working files are unchanged. Image catalog remains local; runtime integration and language quality are separate.

2026-10-25: extended `translations/en/` with 184 browsable per-archive Markdown tables, `CONTRIBUTING.md`, `INDEX.md`, target-only `graphics/image_text_catalog.md` and `image_text_catalog.jsonl`, and `tools/translation_tree.py` (`tools/test_translation_tree.py`). All 192 files pass zero-Japanese validation and rebuild the pinned `returned-v3.jsonl` and `supplemental.jsonl` hashes byte-for-byte.
