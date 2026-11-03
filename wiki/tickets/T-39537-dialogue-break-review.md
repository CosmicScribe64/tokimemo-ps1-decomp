---
id: T-39537
title: Flag suspicious dialogue breaks without changing intentional pauses
status: In Progress
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]", "[[english-text-runtime]]"]
---

## Goal

Review dialogue across routes for awkward line breaks using encoded text, source-aware reflow decisions and actual font widths. Keep intentional pauses and control boundaries intact.

## Acceptance criteria

- [x] Distinguish retained source/reviewed breaks from wrapping-generated breaks.
- [x] Flag short stranded rows, separated punctuation and suspicious phrase boundaries for review.
- [x] Preserve payload bytes; this audit must never rewrite dialogue.
- [x] Test intentional pauses, reply boundaries and automatic breaks independently.
- [ ] Run across the full translated dialogue corpus, review findings and record unresolved cases.
- [x] Review against CODING_STANDARDS.md.

## First corpus pass

The revised audit covers 30,573 dialogue records. It flags 1,368 generated boundaries: 1,335 phrase-link endings, 42 early breaks, one short single-word row and one numbered-label split (categories overlap). Another 9,570 candidates are retained source/reviewed boundaries and require context; they are not treated as defects. Name-code false positives were removed. All nine focused provenance tests pass. Reports are `build/dialogue-break-review.json` and `build/break-review-final-audit.log`; dialogue-bearing inspection output stays ignored under `build/`. No payload is changed by this audit. Candidate triage remains open.

## Reviewed corrections

Added exact-record/exact-payload wrapping overrides for the single isolated-word candidate and numbered-label split. `build/reviewed-breaks-proof.json` confirms exactly two changed records, unchanged wording, unchanged unrelated payloads, fitting rows, and no remaining generated orphan/number-label warning in those records. Both corrections are in the frozen Yumi build and have runtime receipts below. Other early-break candidates include valid balanced subtitle rows and dynamic-name reservations; no blanket reflow was applied.

## Current-build corpus refresh

Reran the full audit against `build/yumi-reviewed-release` after the two reviewed wrap corrections and 39 translation corrections. All 30,573 records were checked. Generated candidates decrease from 1,368 to 1,365: 1,334 phrase-link endings and 42 early breaks (categories overlap). There are no generated short-single-word, punctuation-only or numbered-label candidates. Retained counts stay unchanged at 9,570. Reports: `build/dialogue-break-review-current.json` and `.log`. These are candidate counts, not defect counts; phrase/early-break triage remains open; runtime verification of the two overrides is recorded below.

## Ayako wrap verified in game

The DATE_KA section 01 group 1 reply renders as two balanced rows in Ayako's actual date scene, with a full closing quote and no stranded word or overlap. Verified actual table triad `801A2A40/801A2A88/801A2CA4`, post-render line 1 and complete text at frame 900. Receipt: `translations/en/reviews/ayako-break-2026-11-02.json`. The camp numbered-label check is loading through mode 0x5F; broader candidate triage remains open.

## Camp wrap verified in game

The GASSHUKU table 4 group 23 line 6 reply fits three rows, keeps the numbered label together and includes its closing quote. Actual tables, post-render line 7 and full reveal pass at frame 1200. Receipt: `translations/en/reviews/camp-break-2026-11-02.json`. The speaker name is complete under pixel enlargement; [[tickets/T-39539-speaker-label-reuse]] records that the reduced preview was misread. Both targeted wrapping overrides now have runtime proof. Broader phrase/early-break candidate triage remains open.

## Standards review

Reviewed the audit, layout integration and targeted overrides against CODING_STANDARDS.md. The audit changes metadata only, separates replies when tracking retained boundaries and excludes dynamic name bytes from punctuation-only warnings. Nine tests cover provenance, intentional pauses, controls and numeric labels. Overrides require exact IDs and payloads; whole-corpus comparison preserves wording and unrelated records, and both changes have actual-disc runtime receipts. No default C or assembly changes and no host dependencies. No unresolved implementation finding. Contextual review of the remaining candidates keeps this ticket In Progress.

## Early-break contextual triage

Inspected all 42 generated early-break candidates with their current encoded dialogue and measured row widths. Keep 27 balanced layouts and five dynamic-name reservations. Ten candidates also split a linked phrase and remain open for closer review. No dialogue bytes or intentional source breaks changed. The ID/hash receipt is `translations/en/reviews/early-break-triage-2026-11-02.json`; dialogue-bearing context stays in ignored `build/early-break-context.json`. This text review does not replace runtime visual checks or close the broader phrase-link queue.

## White Day phrase correction

The White Day runtime batch exposed an awkward generated break inside a short exclamation in table-3 group 1 line 1. Added an exact-ID/exact-payload override that restores the translation's sentence boundary while preserving its opening and closing rows. The corpus comparison confirms that only the three registered overrides differ when enabled, with unchanged wording, fitting rows and unrelated payloads unchanged. The rebuilt-disc verification is recorded below; this correction is not yet published. The other two overrides retain their existing runtime receipts.

## White Day correction verified

Built `fd2f74c0a3bb0cc853ed7849ec6bffdf9dc84c5f` and reloaded WHITE.BIN through the scene's CD loader. The corrected exclamation stays on one row, with the opening, scrolling rows and closing quote intact. Receipt: `translations/en/reviews/white-day-wrap-verified-2026-11-02.json`.

The full-disc comparison finds only WHITE.BIN changed; 1,370 other file extents and track 2 are identical to the published build. Script and executable audits pass. Focused tests pass 55 cases with one optional skip. Reviewed the exact-record override against CODING_STANDARDS.md: wording and unrelated records are preserved, no default binary or raw-source changes, and runtime evidence matches the new build. No unresolved implementation finding for this correction. Broader phrase triage remains open, and the new build is not yet published.

## White Day build published

Published `fd2f74c0a3bb0cc853ed7849ec6bffdf9dc84c5f` after all 15 browser checks passed. Pages deployment 41581825760 succeeded, and all 36 served files match the tested package. `translations/en/reviews/published-white-wrap-2026-11-02.json` records the release hashes and verification. The visual review queue remains at 37 pending records.

## Early phrase review

Reviewed the ten unresolved early-break candidates against their encoded rows and widths. Seven have readable two-row phrases without stranded words or punctuation; retain them. The other three share one festival reaction whose ellipsis may provide a better boundary. They still need a runtime comparison. `translations/en/reviews/early-phrase-triage-2026-11-02.json` records the decisions without changing dialogue or claiming additional visual coverage. Broader phrase-link triage remains open.

## Event and festival boundaries

The current-disc event batch shows Shiori's cake invitation splitting its question after the auxiliary verb. Prepared an exact-record override to retain the translation's sentence boundary, without changing the hesitant opening or trailing invitation. Prepared the same guarded treatment for the three copies of the festival reaction, breaking at the ellipsis. Corpus comparison and rebuilt-disc visual checks are pending; none of these four corrections is approved or published yet.

## Event and festival corrections verified

Built `fbb1eb25a60fbcc51ab308c15c9d7e19777a48e4`. The invitation keeps its question together, and the festival reaction breaks at its ellipsis. Receipts are `translations/en/reviews/event-queue-retested-long-pages-2026-11-02.json` and `translations/en/reviews/festival-ellipsis-verified-2026-11-02.json`. The festival receipt verifies the actual Yuina science-club renderer and proves the other two copies have identical payloads and player-speech commands; it does not claim separate scene runs for those copies.

The corpus comparison changes exactly the seven registered overrides, including the three previously verified ones. Wording and unrelated payloads are unchanged. All 55 focused tests pass, with one optional skip. Script and executable audits pass; only the four intended script containers differ from the published disc. The other 1,367 file extents and the audio track match. Reviewed the new exact-ID/exact-payload overrides against CODING_STANDARDS.md: no default binary or raw-source changes, no new dependencies, fitting rows and runtime evidence for both corrected phrases. No unresolved implementation finding. Broader phrase triage remains open, and this build is not yet published.

## Event and festival build published

Published `fbb1eb25a60fbcc51ab308c15c9d7e19777a48e4` after all 15 browser checks passed. Pages deployment 41584066787 succeeded, and all 36 public files match the tested package. `translations/en/reviews/published-event-festival-2026-11-02.json` records the release hashes and verification. The long-passage queue has 27 pending records.

## Published-build audit refresh

Reran the diagnostic across all 30,573 dialogue records using the published build's font metrics. Generated early-break candidates decrease to 39 and phrase-link candidates to 1,331 after the festival corrections. There are no generated short-single-word, punctuation-only or numbered-label candidates. Retained boundaries remain separate. `translations/en/reviews/published-break-audit-2026-11-02.json` records the counts and report hash; contextual phrase review remains open.
