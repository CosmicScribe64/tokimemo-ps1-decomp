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

2026-10-24: v4 adds original SD, sports and encounter source readers. Validated combined
cache has 306 image sources / 3,339 previews; 70 SD files yield 825 frames, four sports
files yield five viewports, and five encounter files yield 45 page/crop views. All 875
new previews have manual hash-bound classifications. Independent review resolved page
table bounds, duplicate crop identities, source-path LBA binding and unused EXE pairing.
[[assets-media]] records evidence and limitations. Remains In Progress for older unreviewed
previews and remaining containers; root integrates shared log and source-only checkpoint.

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

2026-10-24 source freeze: `.asset-cache/staging/media-reviewed-20261024-v1/` contains 28
source/test/doc files with per-file SHA-256, no extracted game content, and a read-only source tree.
All 64 isolated Docker tests passed. The additional eight tests cover the independently reviewed
BS v1/v2/v3 library; hardware pixel parity remains unverified. Background orchestration and TITLE
work are excluded pending corrections/review. [[assets-media]] records exact checkpoint hashes.
Status remains In Progress; integration owns the shared log and any commit.

2026-10-24 next milestone: reviewed TITLE, background association/export, coverage and MAINROM
context modules now have 106 passing Docker tests. All 313 BS coefficient streams parse;
55 backgrounds have sufficient geometry proof. TITLE adds 18 bounded previews. Combined local
artifacts contain 223 exported sources and 2,194 variants; exact paths, remaining gaps and review
results are in [[assets-media]]. The association bug is resolved conservatively, including computed
replacement reads. Source-only freeze: `.asset-cache/staging/media-reviewed-20261024-v2/`.
No shared CLI, Git, game source or raw input changes by this lane. Keep In Progress.

2026-10-24 v3: independent original FILESEL, OPTION, EXAMINE and NAME_ENT readers add 270
previews with authentic source palettes and STP masks. A standalone `media_ui_export.py` command
publishes these profiles plus TITLE while retaining a baseline census and checking exact input,
file and paired executable hashes. [[assets-media]] records source/table proof, review findings
and remaining runtime/extent gaps. Combined validated artifacts are under
`.asset-cache/media/ui-reviewed-v3/`; the source-only freeze is
`.asset-cache/staging/media-reviewed-20261024-v3/`. No fan-patch implementation, translation
or replacement artwork is reused. Keep In Progress; root owns shared integration.
