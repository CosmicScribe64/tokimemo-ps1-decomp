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
