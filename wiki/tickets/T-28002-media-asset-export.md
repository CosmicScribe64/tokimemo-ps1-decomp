---
id: T-28002
title: Export images and audio with format metadata
status: In Progress
assignee: asset-coordinator
created: 2026-10-24
updated: 2026-10-24
links: ["[[asset-export]]", "[[disc-layout]]", "[[tickets/T-0002-disc-extraction-and-exe-identification]]"]
---

## Goal

Inventory and decode supported background/sprite/texture and audio assets into standard formats, retaining palettes, frames, loops, banks and provenance. Distinguish sample/stream decoding from rendering sequenced music.

Required extension (2026-10-24): build a local visual-localization catalog for assets containing
Japanese text. Prioritize main-screen HUD/command/calendar/menu icons, then buttons, banners,
background signage, title and overlay graphics. Preserve original container/frame/palette/region,
stable IDs, PNG paths, screen context, review state/confidence and later redraw/font/string-rendering
needs. No translation, replacement art or engine integration. Catalogs, images and transcriptions
remain under ignored `.asset-cache/`. OCR suggestions alone cannot establish completed review.

## Acceptance criteria

- [x] Formats and control semantics backed by source/primary references; unknown cases explicit.
- [x] Working bounded exporter/reader with deterministic outputs, source hashes/locations and coverage manifest.
- [x] Synthetic tests and local real-input validation run in Docker; no copyrighted assets or game text committed.
- [x] Independent standards/correctness review resolved before Done.
- [x] Preserve current decomp/build behavior; engine-specific code and full translation remain out of scope.
- [x] Visual-text catalog schema/tooling and local catalog preserve source/frame/palette/region/context provenance.
- [ ] Main-screen HUD/command/calendar/menu image corpus decoded, mapped and visually reviewed.
- [ ] Remaining buttons/banners/signage/title/overlay image corpus inventoried and review coverage explicit.
- [x] Distinct unreviewed, suspected, confirmed Japanese-text and confirmed no-text states; OCR-only confirmation rejected.
- [x] Independent review of visual-text catalog extension resolved; keep In Progress while required corpus is incomplete.
- [x] Color browsing layer separates established-CLUT art, stored/override candidates, source pages, STP debug masks and palette-unresolved diagnostics.

## Comments

2026-10-24: User authorized agents for asset extraction/readers, especially translation-ready text and reusable modern exports. No direct Godot work. Root owns shared tracker/index/log and commits.

2026-10-24: Dispatched parallel chat 01a16f34-9559-7a53-80cf-6152fc4b9ca6. Engine-independent tools only; default real outputs under ignored .asset-cache; root integrates source/tests/docs.

2026-10-24: Implemented `tools/export_assets.py`, shared manifest helpers, TIM PNG/STP export,
and VAGp/VH+VB sample WAV audition export. [[assets-media]] records APIs, commands, source evidence,
coverage and exact deferred formats. Final Docker owned suite: 31 tests passed. Local publication:
32 PNGs from one TIM, 1,716 WAVs from 121 bank pairs, 120 complete banks and one partial bank
(sample 2 of `CDROM/VAB_SEQ/SYUGAKU.VH`, invalid ADPCM predictor/shift at block 522). Partial
status returns nonzero and retains all original bytes and error metadata. All real outputs remain
ignored under `.asset-cache/media/`; no original/build/source game edits or engine files.

Independent code-review skill checks: image agent reviewed standards and input/manifest/CLI/audio
safety, input agent reviewed spec/correctness. Findings resolved: VAG header offsets; VAB table
indexing; WAV staging statuses; unsafe output IDs; end/trailing-source semantics; memory bounds;
ISO/CUE unsupported cases. The TIM palette-index concern was withdrawn as structurally impossible
under the parser's validated full palettes. Review pass recorded; root owns final status/card/log
transition and integration. No commit made in this chat.

2026-10-24: User requires the visual-localization catalog extension above. Main-screen graphic
source reads map to `CDROM/FUKUTEST/MAINROM.NBN` (LBA 0x9D85), calendar to `CALROM.NBN`
(0x9B94), title to `TITLE.NBN` (0xA0CC). Main HUD frame/sequence/palette tables are embedded
in the executable, requiring paired provenance rather than a generic TIM-only scan. Catalog and
main-screen decoder work started; status remains In Progress for corpus review.

2026-10-24 color/catalog continuation: implemented paired MAINROM, CALROM and BUSTUP readers,
the visual-text catalog and a local color-first gallery. Published 167 validated source files,
2,121 previews / 4,237 PNGs including STP masks. All 164 BUSTUP packs have actual-CLUT pages
and a static composite. Gallery has 411 default color cards, separate candidate/source-page/debug
controls and explicit MDEC/title/other-container gaps. Final owned Docker suite passed 56 tests;
independent standards/spec reviews passed. [[assets-media]] records paths, commands, review counts,
palette policies and browser QA limitation. No guessed colors, AI colorization, new art or engine work.
