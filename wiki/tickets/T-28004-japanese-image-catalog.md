---
id: T-28004
title: Catalog every image asset containing Japanese text
status: In Progress
assignee: media-export-chat
created: 2026-10-24
updated: 2026-10-24
links: ["[[asset-export]]", "[[tickets/T-28002-media-asset-export]]", "[[tickets/T-28001-text-script-export]]"]
---

## Goal

Provide the visual localization inventory the user requested, especially the main game screens' icons. Identify Japanese text baked into icons/buttons/banners/backgrounds and distinguish it from text rendered through fonts/strings. This is preparation, not replacement art or translation.

## Acceptance criteria

- [x] Every discovered visual asset has a stable ID/source container/frame/palette/region and extraction/review status.
- [x] Japanese-text entries identify exported PNG, screen/context, confidence and future redraw/font/string work; real transcriptions/images/catalog stay ignored.
- [x] Explicit unreviewed/suspected/confirmed/no-text states; OCR is candidate evidence, not automatic proof of complete coverage.
- [ ] Main HUD/commands/calendar/menus are prioritized, then title/overlay/background signage; unsupported containers remain blocking inventory work.
- [x] Independent correctness review and deterministic/synthetic tests for catalog tooling before Done.
- [ ] Remaining source containers, unused regions/palettes and unreviewed image variants are mapped and visually reviewed; every Japanese-text asset is accounted for.

## Comments

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
