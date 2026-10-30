---
id: T-39025
title: English legibility and asset-coverage linter
status: In Review
assignee: playtest-qa agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[phase4-design-2026-10-29]]", "[[port-testing]]", "[[tickets/T-39004-english-asset-completion]]", "[[tickets/T-39016-generic-text-layer]]", "[[tickets/T-30001-modern-widescreen]]", "[[tickets/T-30002-modern-settings]]"]
---

## Goal

WP-09: turn "playable in English" into measured numbers per checkpoint from E-mode draw receipts.

## Acceptance criteria

- [x] Flags untranslated kana/kanji by translation-unit id (except declared fallbacks), runs past the source box or the window, line and page overflow after reflow, and missing or substituted glyphs.
- [x] Art state per drawn image with text: approved-used, approved-unused, approved-unverified, draft or none. A draft never counts as approved. Only `--approvals` (the `tools/translation_assets.py` audit) can make art approved-used: without it a claimed approval is approved-unverified with an `art_approval_unverified` finding, and with it a hash without an exact approve event is a draft. An approved replacement needs 64-hex `output_sha256` and `drawn_sha256`; `has_text` is a required boolean.
- [x] % English text, % declared-fallback text (reported apart, never counted as English) and % English art per checkpoint.
- [x] Synthetic fixtures only; the unit tests cover every rule (`tools/port/test_english_lint.py`, 11 tests).
- [ ] Producer: the WP-02 observer receipt v2 must embed a `tokimemo.english-draw-receipt.v1` block (format in the `tools/port/english_lint.py` docstring).
- [ ] Code review against CODING_STANDARDS.md.

## Notes

Files: `tools/port/english_lint.py`, `tools/port/test_english_lint.py`, `tools/port/tests/fixtures/english_lint/`.

## Comments

- 2026-10-30 (playtest-qa, review fixes; agent review, not human approval): a receipt can no longer self-report English art. An approved replacement without hashes, or an image without `has_text`, is a LintError; unverified approvals no longer count as approved-used; declared Japanese fallbacks no longer count toward % English text.
