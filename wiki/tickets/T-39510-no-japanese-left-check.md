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

- 2026-11-02 Codex: exploratory actual-disc executable reference census finds 91 Shift-JIS-looking candidates: 79 approved NAME_ENT keyboard entries and 12 numeric/code false positives identified from symbols and reference provenance. A separate exact-payload presence check covers 1,643 C literal records across the main executable and 23 loaded overlays; 1,642 are present, while SHOUGATU's obsolete `-san%s` target is superseded by the English bounded family-name copy plus `-san` append in `src/ovl/new_year_christmas/yumi_scene_handlers.c`. Ignored reports: `build/executable-text-census.json`, `build/literal-presence-census.json`. These complementary checks do not prove all call-site bindings, computed fixed-table elements, or source-only records; executable acceptance remains open.

- 2026-11-02 Codex: generic-probe tracing also passes in the controlled club-tournament fixture (`build/emu/club-font-audit`): zero two-byte font uploads, no unknown mappings or overflow. All 21 route tests pass, including relocated hook addresses and rejecting a range table that would cross the end of RAM. Source review checks bounded trace storage, valid RAM ranges, non-mutating breakpoints and explicit coverage limits; full scene coverage is still incomplete.

- 2026-11-02 Codex: implemented optional emulator font tracing in `tools/emu/font_trace.lua`, shared by route and generic probes. It observes `vload_font` uploads (a superset of drawn two-byte text), aggregates code/caller/scene counts, and flags unmapped or overflowed records; no game-binary change. The positive-control Japanese magazine route logs 179 calls with zero unknown/overflow; the English equivalent logs zero calls. Relink-address regression added to avoid a silent zero caused by hooking an old address. Full main route and additional events still require tracing; artwork remains a separate visual audit.

- 2026-11-02 Codex: retained-record reasons total 762: 340 empty slots, 79 keyboard exceptions, 49 binary-data entries, 285 entries in EVENT/GYOZI (excluded as never loaded by `tools/english_link.py`, see [[overlays]]), and nine overlong name presets. This classifies the report, but does not replace the required scan of final containers or runtime glyph tracing.

- 2026-11-01 Codex: initial retained-record audit found all 340 `empty_target` records have empty original `raw_hex` in `.asset-cache/text/units/units.jsonl` (303 talk, 30 overlay-table, seven dictionary records). These are empty slots, not untranslated Japanese. Nine overlong romanized name presets still fall back to Japanese and require a storage/selection fix; a preference question is pending. The current build reports no unmapped character kinds and no encoded Shift-JIS kinds, but these counters exclude retained records and do not prove the runtime criterion. Catalog-142 now matches its approved exception. Broader glyph logging and off-route proof remain open.

- 2026-11-02 Codex: full fresh main route `build/emu/route-en-still-font` completes 33,649 frames in 750 s, 146 screenshots through save/load, zero original-font uploads, zero unknown mappings and overflow. It runs on `build/still-basis-proof`; final-disc comparison across all 32 art containers finds only off-route D_19_02 differs in `build/still-fallback-proof`. This proves the main route, not all events or the overlong preset-name paths. Remaining off-route coverage and retained-container audit are still required.

2026-11-02: added actual-disc script audit, including pointer bounds, NUL termination, exact target equality, original-font bytes and complete modeled-slot coverage. Diagnostic census checks 31,334 strings/pointers across 104 talk/supplemental containers, finding three original-font fragments in festival records AS_S6W g0078/g0079 l0000 and HI_S6 g0176 l0010. All arise from stray original tab bytes and retained opaque tokens. Repair pending; profile table, executable text and art still need their separate audits. Earlier encoder counters do not cover these opaque bytes.

2026-11-02: four guarded anomaly repairs are now built. Final actual-disc audit of `build/anomaly-repair-proof` passes 105 text-data containers, 31,471 strings and 31,334 pointers, including relocated JOHO labels: exact target equality and zero original-font bytes. Executable literals/tables remain outside this proof. Controlled ambush/battle replay also reports zero original-font calls/unknown/overflow. Wider festival, exams, endings and artwork coverage remain open.

2026-11-02: source-only literal follow-up checks all 321 such records in loaded units against the actual disc; every encoded payload is present. The other 28 belong to unloaded EVENT/GYOZI. Combined with the prior 1,643-record census, all 1,964 compiled-literal records are accounted for: 1,963 exact payloads present and the one reviewed superseded SHOUGATU formatter. Presence does not prove call-site binding or computed-table coverage. Report: `build/source-only-presence-census.json`.

2026-11-02: controlled BUNKAKEN festival bootstrap (3,000 frames), AS_S6W g0078 and g0079 exact-line replays (600 frames each) all report zero original-font uploads, unknown mappings or overflow. Both repaired lines are visually clean in `build/emu/festival-repair-g0078/shot_600.png` and `build/emu/festival-repair-g0079/shot_600.png`. These probes use the actual loaded tables/renderer and log forced selections; they are not normal-career traversal evidence.

2026-11-02: BUNKASAI art-exhibition bootstrap (3,500 frames) and corrected HI_S6 g0176 l0010 replay (600 frames) also pass with zero original-font uploads, unknown mappings or overflow. `build/emu/festival-yuina-clear-g0176/shot_600.png` is visually clean. These add controlled festival coverage; exams, endings, broader art and complete executable table verification remain open.
