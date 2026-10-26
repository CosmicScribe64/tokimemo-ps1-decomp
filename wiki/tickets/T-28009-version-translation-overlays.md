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

2026-10-25: completed full translation quality review and remediation (`F-001` through `F-015`) across `translations/en/` (`.claude/orchestrator/state/T-28009/translation-quality-review-v1/report.md` and `.claude/orchestrator/state/T-28009/translation-remediation-v1/report.md`). Fixed 14 `[[T3:...]]` choices with empty or unbracketed choice blocks, fixed `[[T1]]` missing-prefix/nameplate alignment across 162 rows, standardized 294 `printf` format specifiers (`%2d`, `%2s`, `%s`), resolved 7 untranslated CJK leaks and 30 romaji leaks (`Onee-chan`, `Okaa-san`, `Furisode`, `Osechi`, `Sembei`, `Takoyaki`), standardized Yoshio's `"The truth is..."` catchphrase (`実はな……`), Mio's polite register, Rei's aristocratic register, Mira's `"Darling"` address, `Club Master` / `Graduated with Honors` / `Graduation Exam` terminology, `%1` subject-verb agreement, `Date Spot` / `Kirameki Chart` UI terms, and double-punctuation artifacts. Rebuilt `translations/en/returned-v3.jsonl` (`sha256: b37988c0dfdeba1ac83db32c1e6e15e6f2b3058d3e3f4e373bbba89e0ba6c397`) and `translations/en/supplemental.jsonl` (`sha256: 04e67b8d55fbdf4f81eb8a5d8e5f9574e478887f8eb90d295c9f3f87e710018a`), passing `translation_tree.py validate`, `translation_tree.py build`, `test_translation_tree.py` (10/10), and `verify_remediation.py` (15/15 findings resolved) in Docker.
