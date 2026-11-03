---
id: T-39545
title: Keep secret name codes usable in English
status: In Progress
assignee: codex
created: 2026-11-03
updated: 2026-11-03
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

Restore the original four secret-name effects through English inputs that fit the name-entry fields. The translated comparison strings exceed seven letters, and the original word and arrow variants both translate to the same three strings. The zero-stat code consequently also triggers the maximum-stat branch.

## Acceptance criteria

- [x] All English codes fit seven-letter fields and the zero/max combinations remain distinct.
- [x] Execute the actual generated comparison function to check all four effects and a nonmatching name.
- [ ] Rebuild, audit and publish the corrected patch; the default binaries remain unchanged.
- [x] Review against coding standards and document the usable English codes.

## Notes

Found while tracing the remaining menu-growth candidates. These strings are comparison keys, not display labels. Hash-guarded target corrections reject changes to the source or prior translation.

## Verification

The final inputs are uppercase so the persistent name-pair encoder leaves the comparison keys intact. The integration test uses that production encoder and executes the actual generated comparison function. It checks all four effects, the friendship exclusions and a mixed nonmatching code. All 16 focused tests pass. The revised test rejects the stale intermediate lowercase strings.

Target `c3c205c005712aa57072053c2e3faa511c1820fa` passes script, executable and generated-source audits. All 27 original binary hashes still match. Of 992 compared disc files, 991 are byte-identical to the previous published build; only NAME_ENT.EXN changes. The normal artwork readback also passes. The earlier dialogue and artwork evidence remains applicable; this is not a new emulator playthrough.

Reviewed the correction guards, generated comparisons, name encoding, test assertions and documentation against CODING_STANDARDS.md. No unresolved finding. The English codes are documented in `translations/en/README.md`. Receipt: `translations/en/reviews/name-codes-2026-11-03.json`. All 15 browser checks pass; publication is pending.
