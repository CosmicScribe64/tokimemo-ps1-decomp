---
id: T-28004
title: Catalog every image asset containing Japanese text
status: Done
assignee: media-export-chat
created: 2026-10-24
updated: 2026-10-26
links: ["[[asset-export]]", "[[tickets/T-28002-media-asset-export]]", "[[tickets/T-28001-text-script-export]]"]
---

## Goal

Provide the visual localization inventory the user requested, especially the main game screens' icons. Identify Japanese text baked into icons/buttons/banners/backgrounds and distinguish it from text rendered through fonts/strings. This is preparation, not replacement art or translation.

2026-10-25 user direction: Gemini's translation pipeline also supplies image-text translations and suggestions that fit the original image dimensions, including icons. Retain asset identity and dimension/context metadata so those suggestions can guide Phase 5 replacement images. Actual translated-image creation and integration are explicitly deferred; do not edit Gemini's translations or pipeline.

## Acceptance criteria

- [x] Every discovered visual asset has a stable ID/source container/frame/palette/region and extraction/review status.
- [x] Japanese-text entries identify exported PNG, screen/context, confidence and future redraw/font/string work; real transcriptions/images/catalog stay ignored.
- [x] Explicit unreviewed/suspected/confirmed/no-text states; OCR is candidate evidence, not automatic proof of complete coverage.
- [ ] Main HUD/commands/calendar/menus are prioritized, then title/overlay/background signage; unsupported containers remain blocking inventory work.
- [x] Independent correctness review and deterministic/synthetic tests for catalog tooling before Done.
- [ ] Remaining source containers, unused regions/palettes and unreviewed image variants are mapped and visually reviewed; every Japanese-text asset is accounted for.

## Comments

2026-10-24 v5 static extension: `.asset-cache/media/static-reviewed-v5/` covers 3,462 static
previews from 357 sources. Every static preview has manual evidence: 425 Japanese, 342
other-text, 2,625 no-text, 70 suspected, zero unreviewed and zero orphan regions. The new
23 backgrounds and 100 photo pages were individually reviewed. Movie frame exports remain
separate and are not visually classified by this static catalog. Unknown runtime palettes,
unsupported containers and whole-game text/graphics completeness remain open.

2026-10-24: `.asset-cache/media/corpus-reviewed-v5/` manually classifies every currently
published preview: 3,339 regions, 417 Japanese, 314 other-text, 2,541 no-text, 67 suspected,
zero unreviewed/orphans. All 1,831 older pending previews were inspected via 1,679 exact-PNG
representatives; source/runtime palette caveats remain separate. A correction audit records
three old FILESEL false positives on fully transparent PNGs; the new exact-alias audit has
zero state conflicts and checks all 3,339 PNG hashes. Full source coverage remains incomplete.

2026-10-24: v4 manually adjudicates all 875 new SD/sports/encounter previews through 602
distinct PNGs. Catalog totals: 250 Japanese, 214 other-text, 1,000 no-text, 44 suspected,
1,831 unreviewed; zero orphan regions. Exact-PNG preview alias reports preserve independent
source/CLUT/runtime evidence and never propagate states automatically. One conflicting
older alias group is blocked. Corpus-completeness flags remain false; details in [[assets-media]].

2026-10-24: Assigned to existing media/input chat01a16f34-9559-7a53-80cf-6152fc4b9ca6. No direct Godot work or replacement images. User explicitly requires asset-by-asset Japanese text coverage for a comprehensive future translation.

2026-10-24 checkpoint: Added `tools/assets/visual_text.py` (`tokimemo.visual-text.v1`),
source-backed review priorities in `visual_text_profiles.py`, and `test_asset_visual_text.py`.
The catalog binds review state to region ID plus current PNG SHA-256; confirmed states require
visual evidence and reject OCR-only confirmation. Added `confirmed_other_text` for visible Latin
letters/digits so `confirmed_no_text` does not falsely describe all non-Japanese text as absent.

Paired MAINROM/executable decoding now exposes 174 actor frames, 497 part records and 16
sequences. Six explicitly labeled palette candidates produce 1,044 previews; together with 16 TIM
palettes the first catalog checkpoint has 1,060 regions. Local files are under
`.asset-cache/media/visual-localization-corpus/`; catalog/reviews under `.asset-cache/media/`.
All 174 base actor frames were visually reviewed, with 39 Japanese-text frames; one font palette
was reviewed as non-Japanese text. Current checkpoint has 39 confirmed Japanese-text regions,
113 no-text regions, 22 other-text regions, one suspected region and 885 unreviewed variants.
No transcriptions or replacement graphics were added to tracked files. Confirmed results apply
only to inspected PNG hashes, not all palette candidates.

Main icon sequences (frames 3-59) are pictorial; baked Japanese labels are present in the date,
parameter, weekday and magazine graphics. Further raw-sheet inspection found additional Japanese
image text outside the actor-frame rectangles in MAINROM offset 0x2000; runtime palette/usage
remain unresolved. Five unreferenced sheets, unused texel regions, CALROM month tiles, title,
overlays and MDEC backgrounds are blocking corpus completion. CALROM decoder work is in progress.
All catalog completeness flags remain false and this ticket remains In Progress.

2026-10-24 updated checkpoint: added CALROM month graphics, 164 BUSTUP portrait packs and a color
gallery (`.asset-cache/media/gallery/index.html`). Current source manifest is
`.asset-cache/media/color-corpus/manifest.json`; current catalog/review list are
`.asset-cache/media/color_visual_text_manifest.json` and `color_visual_text_reviews.json`.
There are 2,121 image variants: 40 confirmed Japanese text, 118 no text, 94 other text, one
suspected and 1,868 unreviewed. Catalog generation hash-verified 4,237 image/mask files. Unknown
source queues remain explicit (1,188 media sources without decoded image regions, including audio
and 875 probable graphic-bearing sources; not a count of missing images).

All 72 calendar previews were visually reviewed as non-Japanese text; the 164 portrait composites
and their source pages are available in color, with only one portrait composite reviewed so far.
MAINROM's Japanese-bearing index sheet at offset 0x2000 is cataloged with unresolved runtime CLUT
and placement, not presented as finished grayscale art. The gallery opens on established-CLUT color
previews; STP masks, unresolved index diagnostics, source pages and unconfirmed palette applicability
are separated and labeled. No image replacements or transcriptions are committed.

Final independent reviews: standards axis (input agent) passed decoder/manifest/CLI/profile files;
spec axis (image agent) passed catalog/gallery/CLI files; audio agent independently reviewed new
decoders and gallery. A portrait-depth concern was withdrawn after source confirmation that
quad-attribute depth and cell-size flags are independent. All findings resolved; 56 owned Docker
tests passed. Browser policy blocked local-file preview, so gallery QA is static/hash/test plus
direct PNG/contact-sheet inspection. Corpus completion remains blocked by unsupported containers,
unmapped texels/CLUTs and 1,868 unreviewed variants. Keep this ticket In Progress.

2026-10-24 source-only checkpoint: reviewed catalog/gallery/profile sources and synthetic tests
are frozen with the media readers at `.asset-cache/staging/media-reviewed-20261024-v1/`.
All 64 isolated Docker tests passed; [[assets-media]] records the exact hashes. The 37 additional
source-scoped palette reviews remain in an ignored review file and have not yet been merged into
the catalog totals above. No completeness claim or status change follows from this checkpoint.

2026-10-24 next review milestone: merged the 37 source-scoped MAINROM palette reviews and manually
reviewed all 55 exported backgrounds plus 18 TITLE previews. The 2,194-region catalog now has
73 confirmed Japanese, 164 no-text, 118 other-text, eight suspected and 1,831 unreviewed regions.
Small uncertain signs/boards remain suspected. Six contact sheets and adjacent source/slot/PNG-hash
maps document the new review; exact artifact paths are in [[assets-media]]. Gallery has 478 default
cards and 9,156 verified local references. MAINROM context evidence is separate from review states;
unused texels, unresolved palettes and 258 BS sources without exported geometry remain open.
All completeness flags remain false. Source-only v2 freeze and 106 passing Docker tests are
documented in [[assets-media]]; keep this ticket In Progress.

2026-10-24 v3: manually reviewed all 270 new FILESEL/OPTION/EXAMINE/NAME_ENT previews. Total
catalog: 2,464 regions; 215 confirmed Japanese, 185 other-text, 222 no-text, 11 suspected and
1,831 unreviewed. The three new suspected regions are tiny OPTION pictograms; no OCR or
filename-based confirmation was used. A facts-only fan-patch geometry cross-check found all
59 curated numeric label boxes inside source extents and fully represented by original exported
texels; this does not assert their language or full corpus coverage. [[assets-media]] records
the pinned reference, original-source proof and exact artifact paths. Tickets remain In Progress.

2026-10-24 final v5 static catalog: 3,462 previews / 357 image sources, with 425 Japanese,
342 other-text, 2,625 no-text, 70 suspected and zero unreviewed/orphan regions. All current
static images have per-preview manual evidence and source/palette scope. The separately
exported 997 movie frames remain visually unclassified; whole-game completeness stays
false. All three earlier transparent FILESEL false positives are corrected in the new
catalog with retained correction history. No translation or transcription was added.

2026-10-25: added hash-bound manual classifications for eighteen original ETC SD color frames (ten no-text, eight other punctuation). The combined static catalog now contains 3,480 regions: 425 Japanese, 350 other text, 2,635 no text, 70 suspected and zero unreviewed. No Japanese confirmations were added by these two assets. Independent correctness review inspected all eighteen actual frames and verified PNG/review identities. Original-source palette/runtime caveats and all corpus-completeness flags remain open. Movie frame review is a separate next tranche. Keep In Progress; [[assets-media]].

2026-10-25 bounded movie review: sixteen OPENXA decoded frames 392–407 are independently confirmed to contain Japanese text. All PNG hashes/dimensions, original source frame headers, sector indices and raw-track extent bindings pass. The ignored review packet is recorded in [[assets-media]]. This presence-only window is separate from static-image counts; it does not establish complete movie coverage, timing, hardware pixel parity or a chosen replacement method. Edited frames or synchronized replacement compositing will be needed for later localization; method remains open. Keep In Progress.

2026-10-25 four-BS review: all four actual active backgrounds have independent manual review, exact source/PNG hash bindings and subregion/context/confidence/redraw records. C_01_28 chalkboard and C_01_30 sign confirm Japanese; C_01_27 book labels and C_01_29 notebook marks remain suspected. The merged static catalog has 3,484 regions: 427 Japanese, 350 other text, 2,635 no text, 72 suspected, zero unreviewed. Twelve padded/mask diagnostics are excluded from art totals. All 6,963 referenced catalog files verify, and the accepted live gallery retains the earlier ETC SD frames. Exact paths and hardware/placement/corpus limits are in [[assets-media]]. Movie counts remain separate; keep In Progress.

2026-10-25 seasonal review: four additional active backgrounds have independent manual pixel/hash review and source/subregion/context/confidence/localization records. All four retain suspected status at confidence 0.55; no characters are transcribed or confirmed by OCR. The merged catalog contains 3,488 active regions: 427 Japanese, 350 other text, 2,635 no text, 76 suspected, zero unreviewed. All 6,971 PNG/mask references verify. Twelve additional padded/mask diagnostics are excluded from art totals. The updated gallery and exact replay/profile hashes are in [[assets-media]]; movie and whole-game coverage remain open.

2026-10-25: all 24 prologue cells have manual source/cell/palette/region/context/confidence/evidence/redraw records bound to canonical variants and exact PNG hashes. All are no-text within these specific previews; no full-page/composed-scene absence claim follows. The inherited 3,488 region records remain unchanged, yielding 3,512 total: 427 Japanese, 350 other-text, 2,659 no-text, 76 suspected, zero unreviewed. Raw bit-15 masks are diagnostic-only. Whole-game image-text coverage remains open. [[assets-media]].

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
