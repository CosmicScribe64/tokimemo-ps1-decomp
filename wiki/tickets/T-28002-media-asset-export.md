---
id: T-28002
title: Export images and audio with format metadata
status: In Progress
assignee: asset-coordinator
created: 2026-10-24
updated: 2026-10-25
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

2026-10-24 v5 bounded formats: source/native callback proof adds 23 backgrounds; ETC photo
bindings add 28 sources / 100 actual-palette pages. Combined static artifacts now cover 357
image sources / 3,462 previews, all manually classified. CUE-bound track 2 exports to WAV
with exact source-payload parity, while all 16 XA/STR extents are preserved separately.
Pinned Docker FFmpeg and an original-SDK-proved temporary BS v1 compatibility tag adapter
support stream export without altering original sectors or guessing movie timing. Full-stream
batch validation is ongoing; [[assets-media]] will record its outcome. Format and source
completeness remain false; no engine integration or game-build changes. Keep In Progress.

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

2026-10-24 v5 stream batch completed: all 16 preserved XA/STR sources have audited exports,
14,910 WAV groups and 997 movie frames. Independent final audit checks every output hash/
header and exact source sector coverage; no source failures or modified raw/manifest hashes.
One ASAHINA sector remains opaque/quarantined and 56 type-0 data sectors are explicitly
outside media decoding. Original timing, runtime clip mapping and hardware pixel parity
are not claimed. [[assets-media]] records cache paths and full counts. Static image corpus
now contains 3,462 manually classified previews; ticket stays In Progress for remaining formats.

2026-10-25 root integration: applied the reviewed v4 current-source media packet (22 tool/test files). Original table-backed ETC SD profiles export SD_P30 and SD_P30W into eighteen native-color frames with raw/indexed/STP, CLUT, sequence and EXE/ETC dependency provenance. The selector requires an exact matching baseline and preserves all 1,370 unselected records. Independent Standards/correctness reviews resolved witness tree/build-digest labeling, a stale HUD citation and an inherited test-count scalar. The focused suite passes 91 tests; the coherent current-text/media suite passes 441 tests, zero skips. All output hashes and regenerated pixels match, and game source/build files remain unchanged. Current static census: 359 image sources / 3,480 regions; 73 NBN sources, 235 background geometries, movie review and other corpus gaps remain open. Keep In Progress. [[assets-media]], [[asset-export]].

2026-10-25 review of isolated four-BS candidate: the original C_01_27–C_01_30 files and all sixteen active/padded/mask PNGs reproduce correctly, but production verification needs changes before integration. Independent correctness checks show that a consistently rehashed fabricated source witness is accepted and that C_01_31 can replace row 3 without enforcing the original row's LBA. Require independently trusted source/selector/read/decode evidence, exact file/hash/index/LBA/table-offset binding, and negative regressions. A stale school-day continuation citation also needs correction. The media owner prepares a new isolated packet and full localization-catalog records; no candidate code is applied. Preserve declared-file decode bounds, native overread provenance and latest ETC SD baseline records. Existing background/corpus gaps stay open.

2026-10-25: [[tickets/T-28006-sequence-music-export]] tracks a disjoint sequence-music lane. Initial scope is source-backed original SEQ inventory, lossless event/JSON/MIDI reading and bank/program dependencies. WAV rendering remains open until native instrument/ordering/loop semantics are verified; instrument samples or MIDI are not claimed as completed music export. Root retains live integration ownership.

2026-10-25 four-BS integration: the repaired v4 packet is applied after independent correctness/standards review. Trusted C/assembly/EXE/table/file pins reject fabricated witnesses and wrong-row file substitutions. The live export reproduces all sixteen PNGs, preserves 1,368 nonselected records and records active/padded geometry, conditional VRAM placement and declared-file overreads without feeding those overreads to the decoder. The coherent suite passes 466 tests without skips; protected game/build files are unchanged. Static image sources increase to 363 and BS geometry backlog falls to 231; 73 NBN sources remain unresolved. The merged catalog/gallery and exact packet/replay hashes are in [[assets-media]]. Keep In Progress.

2026-10-25 seasonal integration: applied the independently reviewed three-file extension for four original seasonal school-day backgrounds. Original selector/native table/file/read proofs, sixteen hard negatives and all sixteen PNG hashes pass. Root's coherent current-source suite passes 483 tests without skips; the application preserves all 34,950 other tracked files. Live replay retains all 1,368 unselected records and the prior ETC SD/weekly additions. Static sources are now 367; BS geometry backlog is 227, NBN remains 73. Nine-sector file bounds exclude each native ten-sector read's extra sector from decoding; active/padded/mask and placement limits remain explicit. Exact artifacts and source-provenance scope are in [[assets-media]]. Keep In Progress.

2026-10-25 sequence milestone: [[tickets/T-28006-sequence-music-export]] adds reviewed lossless JSON export/reconstruction for all 63 original SEQ packages, plus 138 explicit opt-in provisional MIDI entries. The coherent suite passes 493 tests without skips and both installed CLI modes validate/repeat on original inputs. Thirty-nine event streams, native timing/bank dependencies and music WAV rendering remain open. This is separate from existing sample/XA/CDDA WAV exports and does not close media coverage. [[assets-media]].

2026-10-25 next NBN review is held, with no live application. The isolated four-source prologue candidate passes 498 composed tests and reproduces its 48 PNGs, but independent original-word/pixel checks find two P2 defects: unchecked selected baseline IDs permit escaped PNG writes, and bottom-cell cropping ignores the original upload-at-128 / sample-at-120 mapping. Ten of twelve bottom color previews differ from the native sampling path. Per-cell confidence/context/evidence/localization fields also need completion. The source bit-15 masks are raw palette diagnostics, without a runtime STP claim. A new isolated revision must repair writes and pixels, then refresh affected manual reviews; tests/output reproduction alone do not establish acceptance. Exact reviews are ignored `T-28002/nbn-prologue-{correctness,standards}-review` records. Current published media counts and the 73-source NBN backlog stay unchanged.

2026-10-25 sequence coverage: reviewed v9 now decodes all 177 event streams in the 63 original SEQ files, with 337,797 events and 177 opt-in MIDI auditions. Original bytes/aliases/IDs persist and installed reconstruction/source/SMF checks pass; the coherent suite passes 498 tests without skips. Native tempo units are proved, but raw-MPQN MIDI is not exact native scheduling or bank/instrument rendering. The former 39 raw-only streams are closed in this reader version; music WAV, bank/loop dependencies and broader media coverage remain open under [[tickets/T-28006-sequence-music-export]]. Static/image counts and the held NBN work are unchanged. [[assets-media]].

2026-10-25 NBN v2 integration hold: independent native/pixel review confirms four source identities, 1,112 original instruction words, corrected lower-cell sampling, 48 PNG/mask outputs and safe writes. Root composed 536-test replay found one test-isolation defect: the new test module changes shared manifest repository/cache globals at import, redirecting an existing CDDA test in an isolated checkout. No NBN tooling is applied. The owner is preparing immutable v3 with scoped test patches/cleanup; source/pixel proofs can be reused only when production hashes remain identical. Exact failure and independent findings are ignored `T-28002/nbn-v2-with-bank-current-integration/tests.log` and `nbn-prologue-v2-correctness-review.md`. Keep In Progress.

2026-10-25 NBN v3 catalog hold: the import/global isolation finding is resolved (22 focused tests, 537 isolated coherent tests, then 559 installed tests alongside SPEAKTST). Three new tooling files are applied but uncommitted. Actual four-source CLI export passes, but direct generic visual catalog construction rejects duplicate region identities: cell descriptors lack a canonical frame/variant. The owner is preparing separate v4 with explicit native cell variants and direct catalog regression coverage; no private derived-manifest workaround will replace that fix. Native/PNG/source evidence remains valid, all v3 outputs are preserved, and no NBN corpus count is published before complete catalog/gallery verification. Review: ignored T-28002/nbn-v3-catalog-identity-review.md. Keep In Progress.
