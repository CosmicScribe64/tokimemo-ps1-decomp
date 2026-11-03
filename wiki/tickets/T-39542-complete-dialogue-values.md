---
id: T-39542
title: Preserve complete long dialogue values
status: In Review
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

The O/W substitution values use a character-name-sized backing buffer. English fortune labels can exceed its 23-character payload, losing their ending even though the disc contains the full label. Preserve complete values while keeping the original state fields bounded.

## Acceptance criteria

- [x] Reproduce truncation with the production setter and protect adjacent fields in regression tests.
- [x] Give dialogue values sufficient independent capacity; leave character-name storage unchanged.
- [x] Verify the longest fortune labels in the emulator and check placement, RAM limits and default matching.
- [ ] Review the fix and publish a build that passes the disc and browser checks.

## Reproduction

The native C regression executes the production setter and lookup helper. On the published code it fails the complete-value equality check after a string longer than 23 bytes is copied (`build/dialogue-values-before.log`). The fix gives only O/W backing arrays a 64-byte capacity and copies according to each array's size. The original 8-byte and 12-byte state fields still receive bounded, terminated prefixes; the 24-byte character-name buffers are unchanged.

## Verification and review

The production helper passes the complete-value, bounded-prefix, adjacent-field, stale-prefix and multibyte checks. All 16 focused regression tests pass. The rebuilt disc passes both text audits, all 27 original binary hashes still match, and the main image ends at 0x801314B0 below its 0x80132000 limit. All 15 browser checks pass against target `81d34fdf65e956898051a76a72f8adeeeb7c804b`.

Controlled emulator fixtures execute the real fortune setter and dialogue renderer for Future Blessing (Suekichi) and Middle Blessing (Chukichi). Both show complete labels and closing punctuation. The initial fixture retained the previous scene's text and was rejected; the isolated captures clear that text before selecting the line. The fixture does not establish a natural shrine playthrough. Evidence is recorded in `translations/en/reviews/dialogue-values-2026-11-02.json`.

Reviewed the C89 change and regression against CODING_STANDARDS.md. Only the English O/W backing arrays grow, by 80 bytes total. Legacy state fields and character-name capacities remain unchanged. Copies use their destination capacity and retain the existing termination and multibyte rules. No unresolved review findings remain. Publication is pending.
