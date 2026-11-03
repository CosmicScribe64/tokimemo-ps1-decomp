---
id: T-39541
title: Use consistent gift names
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

The DATE and DATE2 gift tables contain identical Japanese item names but use different English wording and capitalization. Reuse the established DATE2 labels for DATE so the same gift has the same name. Preserve empty entries and controls.

## Acceptance criteria

- [x] Exact-record corrections match the existing DATE2 labels for identical source text and preserve empty slots.
- [x] Production encoding, width checks and disc readback pass.
- [x] Inspect representative corrected gift labels in the game.
- [x] Review against CODING_STANDARDS, publish the verified build, and record the checks.

## Evidence

`date_gift_name_set` and `amusement_karaoke_indexed_text_select` copy the indexed gift names into `name_code_a`. The short-text census found 65 case/punctuation groups; 19 groups contain gift-name capitalization differences. Sentence fragments, on-screen headings and debug labels require separate dispositions.

## Full-table check

All 99 original DATE and DATE2 gift slots contain identical Japanese bytes. Six slots are empty. The full comparison found 82 differing English labels, including the initial 19 capitalization-only differences. Use the existing DATE2 labels for the matching DATE slots, preserving the six empty entries. This also removes unsupported additions such as "Cool" and "Summer" where the source only names the item. The longest replacement is 32 bytes plus NUL, fitting both the 33-byte table slot and the 36-byte `name_code_a` region. The maximum measured label width is 173 pixels; the runtime checks below cover the longer substitutions.

This supersedes the initial capitalization-only scope. Each replacement already ships for the exact same Japanese source in DATE2. The correction remains guarded by the original source and previous target hashes.

## Verification and standards review

The rebuilt disc reads back all 99 gift slots equal to DATE2, with 82 corrected labels and six empty slots preserved. Both script and executable audits pass. Three isolated GPU fixtures show complete long labels, including the widest and maximum-length replacements. The captured table pointers, name buffers and completed glyph counts match. All 15 browser checks pass, including archive drop and downloaded hashes. Only DATE.EXN differs from the magazine release; all other 1,370 file extents and the audio track match. Receipt: `translations/en/reviews/gift-consistency-2026-11-02.json`.

Reviewed the 82 corrections against their identical Japanese source slots and the existing DATE2 labels. Source/previous-target hashes guard every override; empty slots and control markers stay intact. The maximum encoded length is 32 bytes plus NUL, within the existing fields. The small layout-report change reads each correction's ticket instead of incorrectly attributing every correction to T-39538. All 24 writer/reviewed-target tests pass. No unresolved standards findings remain. Published target `5c761523f939755d88c720adb2360927c58fe602` in Pages commit `c9ef3a2777408db912f1901b90ff4ce88308947d`. Deployment 41593310508 succeeded; all 37 served files match the tested package. See `translations/en/reviews/published-gift-consistency-2026-11-02.json`.
