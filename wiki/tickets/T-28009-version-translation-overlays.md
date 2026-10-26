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

2026-10-25: per explicit user direction (`"I think in this case it is okay to put the japanese text beside it"`, `"add a 'context' section to the tables"`, `"I also dont see a simple page in the translation breaking down what the different [[]] tags are/do"`, `"I don't get why ready for review columns exist"`), refactored all 184 Markdown tables in `translations/en/` to a 5-column layout (`| ID | Context | Japanese | Translation | Notes |`), restored `Original Japanese` in `translations/en/graphics/image_text_catalog.md` and `image_text_catalog.jsonl`, removed the `Status` and `Ready for review` columns from tables and indexes while preserving `review_status` in `returned-v3.jsonl` and `supplemental.jsonl`, and added `translations/en/TAGS.md` documenting all 28 `[[T...:...]]` control token families. Verified 10/10 unit tests in `tools/test_translation_tree.py` and exact byte-for-byte round-trip SHA-256 hashes for `returned-v3.jsonl` and `supplemental.jsonl` in Docker.
