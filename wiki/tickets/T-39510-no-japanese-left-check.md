---
id: T-39510
title: Prove no Japanese is left in the English game
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-02
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

The owner's bar for the English patch: no Japanese on screen anywhere, apart from the approved exceptions listed in [[tickets/T-39500-english-translation-patch]]. Prove it with checks that cover the whole game, not only the screens a route reaches.

## Acceptance criteria

- [ ] Text: every text container and C string in the English build is scanned for Shift-JIS glyph codes; each hit is either an approved exception, a control code, or a bug to fix.
- [ ] Runtime: a debug option of the English build logs every 2-byte glyph the text renderer draws; the emulator route and extra save-state stops (dates, festival, exams, sports, battle, endings) run with it and report zero unexpected glyphs.
- [ ] Art: every artwork item marked "no change required" or "no Japanese text" is checked against its decoded pixels, and every placed image is read back from the disc (T-39508's check).
- [ ] A screenshot sheet of the main screens, Japanese beside English, for the owner's review.

## Notes

Runs once [[tickets/T-39506-english-script-runtime]], [[tickets/T-39508-english-art-on-disc]], [[tickets/T-39509-opening-movie-pills]] and the date-scene text fit have landed.

## Comments

- 2026-11-02 Codex: generic-probe tracing also passes in the controlled club-tournament fixture (`build/emu/club-font-audit`): zero two-byte font uploads, no unknown mappings or overflow. All 21 route tests pass, including relocated hook addresses and rejecting a range table that would cross the end of RAM. Source review checks bounded trace storage, valid RAM ranges, non-mutating breakpoints and explicit coverage limits; full scene coverage is still incomplete.

- 2026-11-02 Codex: implemented optional emulator font tracing in `tools/emu/font_trace.lua`, shared by route and generic probes. It observes `vload_font` uploads (a superset of drawn two-byte text), aggregates code/caller/scene counts, and flags unmapped or overflowed records; no game-binary change. The positive-control Japanese magazine route logs 179 calls with zero unknown/overflow; the English equivalent logs zero calls. Relink-address regression added to avoid a silent zero caused by hooking an old address. Full main route and additional events still require tracing; artwork remains a separate visual audit.

- 2026-11-02 Codex: retained-record reasons total 762: 340 empty slots, 79 keyboard exceptions, 49 binary-data entries, 285 entries in EVENT/GYOZI (excluded as never loaded by `tools/english_link.py`, see [[overlays]]), and nine overlong name presets. This classifies the report, but does not replace the required scan of final containers or runtime glyph tracing.

- 2026-11-01 Codex: initial retained-record audit found all 340 `empty_target` records have empty original `raw_hex` in `.asset-cache/text/units/units.jsonl` (303 talk, 30 overlay-table, seven dictionary records). These are empty slots, not untranslated Japanese. Nine overlong romanized name presets still fall back to Japanese and require a storage/selection fix; a preference question is pending. The current build reports no unmapped character kinds and no encoded Shift-JIS kinds, but these counters exclude retained records and do not prove the runtime criterion. Catalog-142 now matches its approved exception. Broader glyph logging and off-route proof remain open.

- 2026-11-02 Codex: full fresh main route `build/emu/route-en-still-font` completes 33,649 frames in 750 s, 146 screenshots through save/load, zero original-font uploads, zero unknown mappings and overflow. It runs on `build/still-basis-proof`; final-disc comparison across all 32 art containers finds only off-route D_19_02 differs in `build/still-fallback-proof`. This proves the main route, not all events or the overlong preset-name paths. Remaining off-route coverage and retained-container audit are still required.
