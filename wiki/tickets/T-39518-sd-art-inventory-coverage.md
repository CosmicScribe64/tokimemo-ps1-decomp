---
id: T-39518
title: Bind and review the complete SD artwork group
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39510-no-japanese-left-check]]", "[[tickets/T-39508-english-art-on-disc]]"]
---

## Goal

Replace catalog-152's incorrect BUSTUP wildcard binding with the actual CLUB_SD, HOLI_SD and MAIN_SD files, then review their decoded graphics for untranslated text.

## Acceptance criteria

- [x] Inventory resolves every stated SD directory to concrete source hashes and does not bind to portraits.
- [x] Source-proven decoded images cover every supported file/frame; unsupported layouts remain explicit gaps.
- [x] Visual findings are repaired or covered by existing owner-approved exceptions; no unsupported no-text claims.
- [x] Inventory regressions and standards review pass.

## Evidence

The catalog JSON's selector names CLUB_SD/*, HOLI_SD/* and MAIN_SD/*, but catalog_paths ignores wildcard selectors and falls back to the section's BUSTUP/*.BIN container. Catalog-152 consequently has one unhashable portrait wildcard and no SD-file identities. Its prose claims 18 files; the source directories contain more. Existing per-file SD readers require original LBA/hash/layout provenance and can supply concrete review exports.

## Review and results

2026-11-02 In Progress -> In Review -> Done. 69 files (32 CLUB_SD, 12 HOLI_SD, 25 MAIN_SD) resolve to original hashes. All decode with the original executable's LBA/layout/palette profiles, zero unsupported layouts, and match the current disc byte-for-byte. Visually reviewed all 585 distinct frames across sheets 00..24: characters/props, comic symbols, Latin letters and numerals; no Japanese lettering. Receipt: `translations/en/graphics/reviews/retained-pixel-review-2026-11-02.json`.

37 inventory tests and regeneration check pass. Corrected stale ride-sign assertions to the pre-existing owner exception. Standards review: explicit directory selectors, preserved prior source hashes, unmatched groups retained as gaps, no guessed palettes, raw-source changes, committed pixels or host dependencies. No game binary changes. Whole-game executable/event coverage remains separate.
