---
type: reference
updated: 2026-10-25
sources: ["tools/export_assets.py", "tools/assets/images.py", "tools/assets/audio.py", "wiki/spec/h-graphics.md", "wiki/spec/h-cdio.md", "wiki/spec/m-dates-a.md"]
---

# Bounded image and audio exports

[[asset-export]] defines the scope; [[assets-input]] describes the read-only input API.
`tools/export_assets.py` inventories every supplied entry and decodes the supported subset.
No engine integration or changes to the matching build are involved. All real outputs belong in
ignored `.asset-cache/`; image/audio examples use `media/`, while sequence tools use `music/`.
Use separate output directories for different input/options and tooling checkpoints. Versioned
exports below are immutable evidence; reproduce them with their recorded tool revision or choose
a fresh output path with later tooling.
The CLI refuses to replace an existing output with different bytes and stages decoder writes
under the output root before publishing them. No copyrighted assets belong in tracked tests or docs.

Run using the existing Docker image, with the repository mounted at `/work`, for example:

```sh
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 tools/export_assets.py disc/track1.bin --inventory-only \
  --output .asset-cache/media/original-inventory
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 tools/export_assets.py disc/files --include '*.TIM' \
  --output .asset-cache/media/images-run
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 tools/export_assets.py disc/files --include '*.VH' --sample-rate 22050 \
  --output .asset-cache/media/sample-auditions
```

`--include` is a case-sensitive path glob restricting decode; inventory remains complete.
`--max-bytes` bounds individual decoding inputs (default 16 MiB), with larger entries explicitly
deferred. `--sample-rate` is an explicit raw-bank audition choice, not a claimed native rate.
Directory input cannot account for streams omitted by an earlier extractor; use the original image
or archive for the complete disc-record inventory.

## Current-source rebase and ETC SD checkpoint (2026-10-25)

The reviewed media rebase updates source citations to the completed Phase 2 layout while retaining original executable/overlay pins and the strict six-helper background body gate. The additional ETC SD reader uses the original ETC table's preceding word and first word with the executable's separate layout rows. It exports `CDROM/ETC_SD/SD_P30.NBN` and `SD_P30W.NBN` at original LBAs `0x7D78` and `0x7D81`: nine source-color frames each, with actual CLUT, indexed/STP, frame/sequence and dependency provenance. Static previews do not reproduce runtime blending or composition.

Use `media_ui_export.py --include-sd` for the extended SD profiles. Its optional repeated `--only-source` selector requires an exact whole-input-hash-matched baseline; unselected baseline records remain intact. The bounded run is `.asset-cache/media/etc-sd-published-v1/manifest.json`; its output audit matches all hashes, and 1,370 nonselected records retain their previous values. Original EXE/ETC dependency bytes remain alongside the exported sources.

The resulting static catalog `.asset-cache/media/etc-sd-visual-catalog-v1.json` has 3,480 reviewed regions from 359 exported image sources: 425 Japanese text, 350 other text/punctuation, 2,635 no text and 70 suspected. All eighteen added PNG hashes were visually reviewed: ten no-text and eight punctuation; no additional Japanese-text claim is made. The original NBN inventory now has 73 unresolved sources. Movie classifications, 235 background geometries and other corpus gaps remain open; all three catalog completeness flags are false. Older generated citations remain historical. Regenerating them with the new tools changes source-locator metadata, not the already validated pixels or their review states.

Independent Standards and correctness reviews pass after finite provenance/citation fixes. The exact 22-file tool packet composes with the current text readers: 441 canonical asset tests pass with no skips; the focused native/profile suite passes 91 tests. Original raw-disc reads, native tables, all eighteen regenerated PNGs, review hashes and 1,645 current-function instruction words pass independent checks. All 1,072 protected game-source/header/config/build files are unchanged, so no matching rebuild is required. T-28002 and T-28004 remain In Progress.

## Supported slices

- `tools/assets/images.py`: standard TIM header at offset zero, 4/8/16/24-bit pixels, all complete
  CLUT variants, source/CLUT VRAM rectangles, original palette words, zero-word alpha policy and
  separate STP masks. PNG is a preview; GPU blend mode and sprite composition remain external.
  The precise TIM extent and opaque trailing extent/hash are reported. No scanning for nested TIMs.
- `tools/assets/image_packs.py`: paired `MAINROM.NBN` plus executable, 174 source actor frames,
  497 parts and 16 animation sequences. Exports source-palette previews and five explicit HUD
  palette candidates per frame; applicability of each override is not asserted for every frame.
  Five sheets without frame-table references get separately labeled source-index diagnostics.
- `tools/assets/image_packs_calendar.py`: paired `CALROM.NBN` plus executable, 36 month tiles and
  36 month-label crops with the separate CLUTs used by `cal_sprite_init`. Full source tiles retain
  pixels outside the runtime 126/127-pixel grid crops; numeric month records and gaps are explicit.
- `tools/assets/image_packs_portrait.py`: 164 validated `CDROM/BUSTUP/*.BIN` files, each with five
  128x128 8bpp pages and one static source-frame preview. Active 256-color CLUT and STP transform
  come from `portrait_build`/`load_palette`. Dynamic rank, mouth/eye overlays and playback remain
  unresolved. The texture-cell size and 8bpp depth are separate source fields.
- `tools/assets/audio.py`: bounded Sony VAGp mono samples and paired VAB/VH+VB banks. Bank samples
  are separately named WAV auditions with explicit rate and sample extents/flags. Original bank
  header/body bytes remain available; instruments, ADSR/pitch/program behavior and SEQ rendering
  are not reproduced. Unsupported/malformed sample entries remain in the metadata.
  Decoding ends after the first ADPCM end block, inclusive; all source block flags and trailing
  source-byte size/hash are retained. Loop markers are reported without replaying loops. Limits
  are 16 MiB compressed input and two million decoded samples per waveform. The supported bank
  variant has an empty VAG(0) size entry, as observed in all local banks; nonzero VAG(0) fails clearly.
- `tools/assets/manifest.py`: schema `tokimemo.media.v1`, deterministic SHA-256 asset IDs derived
  from source path/content, original hashes and locations, output hashes, coverage statuses and
  sorted JSON. Successful modern exports retain the complete original source, including trailers
  and paired sample bodies. No byte-rebuilding claim is made.

Decoder APIs are `probe_images(data, source_path)`, `export_images(data, output_dir, asset_id)`,
`inspect_audio(data, source_path)` and `export_audio(data, output_dir, asset_id,
sample_rate=..., body=..., sample_index=...)`; all return JSON-compatible dictionaries.
The public decoder output directories are caller-provided; use the CLI for the enforced cache-root
and no-replacement publication policy. The shared manifest has no dependency on text readers;
their separate JSON API can be connected later without importing an engine.

## Evidence and coverage boundaries

The standard headers and audio layout are checked against the
[PSX-SPX format reference](https://psx-spx.consoledev.net/cdromfileformats/)
and [Sony Psy-Q file-format reference](https://psx.arthus.net/sdk/Psy-Q/DOCS/Devrefs/Filefrmt.pdf).
The game-side callers are described in [[spec/h-graphics]], [[spec/h-cdio]] and
[[spec/m-dates-a]]: backgrounds go through MDEC, actor textures use uploaded blocks and runtime
tables, sound banks have separate headers/bodies, songs use sequence commands and voices stream XA.

Initial deferred inventory (historical; use the current checkpoint above): 313 `.BS` backgrounds (MDEC), 178 other `.NBN` files including `TITLE.NBN`, 308 other
`.BIN` files (not all proven graphical), other sprite frame/palette relocation tables, 49 `.TMD`
models, 63 `.SEQ` files (instrument rendering), XA streams, STR movies and CDDA track/reference
mapping. Counts are from the local extracted
1,355-file tree, not from generic extension inference that proves each format's contents.
The 16 Form2 streams and one beyond-data-track CDDA reference are retained in the original-input
inventory rather than called exported.

## Validation

Docker synthetic tests use invented TIM, VAG, VAB and ISO records only. The CLI integration tests
check repeated manifest bytes, original retention, paired sample publication, path filters,
malformed recognized input and no replacement/path escapes.
Final owned suite passed 31 tests in Docker:

```sh
docker run --rm --cpus=2 -v "$PWD:/work" -w /work -e PYTHONPATH=tools \
  tokimemo-decomp-arm64 python3 -m unittest assets.manifest_tests \
  tools.test_asset_input tools.test_asset_images tools.test_asset_audio
```

Real TIM validation: one 4-bit 256x256 image, 16 palettes, 32 PNGs including STP masks.
Its 34,816-byte original contains a 33,312-byte TIM extent and 1,504 uninterpreted trailing bytes.
The original stays local and is preserved whole. The final CLI outputs are local at
`.asset-cache/media/validated-images-final/` (32 PNGs plus original/manifest metadata),
`.asset-cache/media/validated-audio-final/` (1,716 WAV auditions at explicit 22,050 Hz plus 121
complete original header/body pairs), and `.asset-cache/media/original-inventory-final/`
(all 1,372 original ISO records, including 17 unsupported stream/reference records).

Audio publication produced 120 fully exported banks and one partial bank: sample index 2 in
`CDROM/VAB_SEQ/SYUGAKU.VH` has an invalid predictor/shift at block 522. It is reported with its
source location and retained original bytes; its waveform is not fabricated. The CLI returns
nonzero for partial/invalid results while saving the complete coverage manifest. Unsupported
formats and unselected supported files are separately inventoried. Independent standards/spec
reviews resolved the VAG header, VAB indexing, staging/publication, ISO/CUE handling, end-block
metadata and bounded-memory findings. Original game input hashes/locations are in local manifests,
and no game content is included in tracked source or synthetic tests.

## Color browsing and visual localization checkpoint

The current local gallery is `.asset-cache/media/gallery/index.html`, generated by
`tools/assets/media_gallery.py` from `.asset-cache/media/color-corpus/manifest.json` and
`.asset-cache/media/color_visual_text_manifest.json`. It has 411 default color cards: main UI
base frames, calendar graphics, static portrait composites and TIM palette 0. Filters expose
885 additional stored/override palette candidates, 820 portrait source pages and five source-index
diagnostics separately. STP masks are separate debug links, hidden by default. All image colors in
finished previews come from stored, source-established CLUTs; no colors or replacement art are invented.

Grayscale has three distinct causes: an established source palette can itself use gray colors;
`.stp.png` is intentionally a single-channel bit-15 mask; and `source-index-sheet-*` is an opaque
numeric index visualization whose runtime palette, transparency and placement are unresolved.
The latter is labeled **palette unresolved**, never offered as finished grayscale artwork.
The gallery uses a checkerboard behind transparent art and nearest-neighbor enlargement for small
sprites. It runs without external scripts, fonts, network resources or an engine.

```sh
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 tools/export_assets.py disc/files --include 'CDROM/FUKUTEST/MAINROM.NBN' \
  --include 'CDROM/FUKUTEST/CALROM.NBN' --include 'CDROM/BUSTUP/*.BIN' --include '*.TIM' \
  --output .asset-cache/media/color-corpus
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 tools/assets/visual_text.py --media-manifest .asset-cache/media/color-corpus/manifest.json \
  --reviews .asset-cache/media/color_visual_text_reviews.json \
  --output .asset-cache/media/color_visual_text_manifest.json
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 tools/assets/media_gallery.py --media-manifest .asset-cache/media/color-corpus/manifest.json \
  --visual-catalog .asset-cache/media/color_visual_text_manifest.json \
  --output .asset-cache/media/gallery/index.html
```

The review file is local human/agent visual evidence, not committed reference data. Omit `--reviews`
to start unreviewed; use a fresh output filename for each changed review revision because publication
refuses different existing bytes. The catalog API is `build_catalog(media_manifest, reviews=None)`;
the gallery API is `build_gallery(media_manifest, catalog=None, output_path=...)`.

Schema `tokimemo.visual-text.v1` binds each review to a stable region ID and PNG SHA-256. It preserves
file/container/source extent, paired executable, frame/part/crop, palette words/CLUT, alpha/STP,
source placement, screen-context evidence, confidence and later redraw/font/string-rendering tasks.
States distinguish unreviewed, suspected, confirmed Japanese text, confirmed no text and confirmed
other text (Latin letters/numerals/punctuation). OCR can suggest a candidate but cannot confirm.
CLI generation verifies current PNG/STP files under the cache; pure API generation is labeled
metadata-only. Real catalogs, reviews, transcriptions and images remain local and ignored.

Current publication: 167 decoded source files, 2,121 image variants and 4,237 PNGs including masks.
All 164 portrait packs validate; current portrait exports are 984 images including 164 static
composites. The catalog contains 40 confirmed Japanese-text regions, 118 no-text, 94 other-text,
one suspected and 1,868 unreviewed regions. It also retains 1,188 media sources without decoded
image regions (875 with probable graphic-bearing suffixes); those source counts are not image counts.
The command-icon animation frames 3-59 are pictorial. Japanese image text is present in date,
parameter, weekday and magazine graphics, plus the MAINROM source-index sheet at offset 0x2000.
No extracted Japanese text is transcribed here.

All 174 base actor frames, all 72 calendar previews, five diagnostics, one TIM palette and one
portrait composite have hash-bound visual review. Other palettes and portraits remain unreviewed.
Five MAINROM sheets (offsets 0x2000, 0x6000, 0xC000, 0x10000, 0x14000) lack frame-table references;
their diagnostics do not resolve runtime CLUT/placement. Unused texel regions, other palette rows,
opaque gaps/trailers, MDEC backgrounds, title and overlay graphics still block corpus completion.
All catalog completeness flags stay false; [[tickets/T-28004-japanese-image-catalog]] stays In Progress.

Final owned Docker suite: 56 tests passed, including input, TIM, MAINROM/CALROM/portrait, audio,
manifest/CLI, catalog and gallery tests. Independent standards and spec reviews passed with no
unresolved findings. Gallery publication verified 8,807 linked file references; deterministic HTML
regeneration matched exactly and has zero remote resources. PNGs/contact sheets were visually
inspected. In-app browser rendering was not verified because browser policy rejected `file://`;
no alternate route was attempted. Existing Docker images lack ffmpeg, and no MDEC decoder or new
dependency installation was added. No matching build, source game code, original data or engine files changed.

## Reviewed source checkpoint, 2026-10-24

The later source-only freeze is `.asset-cache/staging/media-reviewed-20261024-v1/`.
Its `SHA256.json` enumerates 28 source, test and documentation files; `source/` is read-only.
The complete isolated Docker suite passed 64 tests with only that snapshot mounted. The manifest
SHA-256 is `df1615d5737b3322b9223a11e0f47cf1c9e4f515d31fbc6f80a64ca93d27a58f`;
the sorted source-hash set digest is
`396629e6e1bc7b2b7779a7cc6767ee7f298117ffc65e46b95ef78c9b8fe6b932`.
The frozen documentation captures the earlier color-gallery milestone; this continuation note
describes the subsequent checkpoint. Integration owns the shared CLI alignment and log entry.

This freeze adds the independently reviewed `tools/assets/image_mdec.py` library and eight
synthetic tests. It parses bounded BS v1/v2/v3 streams and renders only with explicit, evidenced
geometry. All 313 original BS headers/coefficient streams parsed (311 v1, two v3); this is a
format census, not complete image export or hardware pixel-parity validation. MDEC CLI dispatch,
background context/export orchestration, coverage/UI-context reports and the new TITLE reader
are excluded from this freeze. A background-context association bug still requires correction:
an earlier decoder rectangle in a function was incorrectly assigned to a later raw read. The
decoder rejected the incompatible macroblock count, so no image was published for that case.
Tickets and all corpus-completeness flags remain In Progress/false.

## Background and TITLE continuation, 2026-10-24

The next reviewed source checkpoint is `.asset-cache/staging/media-reviewed-20261024-v2/`.
It includes the bounded TITLE reader, background-context scanner/exporter, source coverage census,
MAINROM context API, and their synthetic tests. All 106 owned media tests pass in Docker.
The shared CLI remains the earlier media dispatcher: TITLE is a reusable library API and BS uses
the separate `tools/assets/media_mdec_export.py` CLI. Integration owns shared CLI/text alignment.

The corrected scanner associates only decoders after a read and before the next replacement read.
Computed wrapper calls, computed raw LBAs/counts, and unknown destinations stop the association;
known different literal raw buffers do not replace one another. This remains lexical evidence,
not a full control-flow proof. Wrapper sector counts are read from the actual reader body.
The final report `.asset-cache/media/background_contexts_final.json` has 480 literal calls,
96 source streams, 36 single-shape streams and 19 single-shape streams with unresolved other uses;
223 BS sources have no mapped literal read. The previously incorrect later `D_11_11.BS` binding
is now unresolved. All 313 coefficient streams parse; only 55 have enough geometry evidence for
image publication. Their manifest binds the canonical context-report hash and source-code hashes.
Hardware IDCT/color rounding parity remains unverified.

TITLE publishes six original-CLUT background pages and 12 actor frames, using actual CLUT rows
0x1FE/0x1FF for actors and row 0x1EF for backgrounds. All 36 PNGs match the earlier visually inspected
slice byte for byte. Five internal frame-alignment gaps totaling 10 bytes are hashed. Thirteen model
texture pages, model palette bindings, unused actor texels and the background map's 120-versus-128
row sampling remain unresolved. Source bodies distinguish logo frame 0, menu states 1-4 and actor
animation frames 5-11. Current C evidence paths follow `config/renames/main-files.txt`.

Combined artifacts live under `.asset-cache/media/milestone-v2/`: `manifest.json`,
`visual_text_reviews.json`, `visual_text_manifest.json`, `coverage.json`, `main_ui_context.json`
and `gallery/index.html`. They retain 1,372 source records, 223 exported sources, 2,194 image
variants and 4,383 PNGs including masks. The 73 new TITLE/background previews were manually
reviewed using six contact sheets with adjacent slot/source/PNG-hash maps. With the 37 earlier
source-scoped MAINROM palette reviews merged, the catalog has 73 confirmed Japanese regions,
164 confirmed no-text, 118 confirmed other-text, eight suspected and 1,831 unreviewed. Suspected
small signs/boards are not treated as confirmed Japanese or no-text. No transcriptions are stored here.
The coverage census has zero orphan catalog regions and keeps source, image and review counts separate.

The gallery has 478 default cards and 2,194 total variants. All 9,156 linked artifact references
were hash-verified; regeneration produced identical HTML, SHA-256
`0e97510978c697e4cab288766f9fa918e25a494e6c1a192681c56801e96a6aea`.
The previous browser-policy limitation still applies. Source/context APIs preserve unresolved
MAINROM texels and CLUT applicability; their local todo report initializes reviews independently
and does not overwrite catalog classifications. Input readers now support validated cached raw/ZIP
entries before inventory initialization through the same bounded sector path. Independent reviews
resolved stale oversized-export claims, unsafe baseline IDs and read-replacement boundaries.
All three asset tickets remain In Progress; root owns shared log/board updates and commits.

## Source-verified UI expansion, 2026-10-24

The v3 source-only integration checkpoint is `.asset-cache/staging/media-reviewed-20261024-v3/`.
`tools/assets/media_ui_export.py` adds a separate reusable CLI for TITLE, FILESEL, OPTION, EXAMINE
and NAME_ENT profiles. It accepts `input --baseline-media manifest.json --output cache-path`.
Exact whole-input SHA-256 equality allows cached inventory reuse, while each decoded file and
paired executable is still read and hash-checked. Cached reuse covers the supplied baseline's
records; it does not rediscover sources omitted from a filtered baseline. Unselected exports,
unsupported source records and prior MDEC context provenance are retained. All output stays
beneath `.asset-cache`; changed bytes at an existing output path are refused.

Four additional original sources now have independent readers and synthetic tests:

- FILESEL: 73 previews (12 background pages, 45 base frames and 16 selected states). Four actor
  page pointers are 0x1A000, 0x3C000, 0x1E000 and 0x40550; actual palettes are source offsets 0,
  0x22000 and 0x22200. Depth is 8bpp while actor cells are 16 pixels, selected by independent
  flags; a nonzero-UV, second-page test protects this distinction. Selected palette scope is
  proved only for frames 4-17 and 30-31. External card icons, dynamic text and unused texels remain open.
- OPTION: 130 frames / 811 parts, including native page pointers 0x200, 0xC200, 0x13CF0 and
  0x8200. Observed part CLUT rows 0x1F0/0x1F1 resolve to source 0x134F0/0x136F0. Mode 1 preserves
  source STP words, and each preview has a separate mask. Frame padding is exactly two bytes
  before the texture-pointer table, not the parsed table itself. The remaining source-byte
  complement and additional uploaded page/palettes are explicit; runtime palette cycling is not composed.
- EXAMINE: 32 frames / 61 parts from the three original 4bpp pages. Paired executable default
  CLUTs and boot STP edits are required. Runtime names and score sprites remain separate from
  these static frames; no palette is guessed when the executable is absent.
- NAME_ENT: six 8bpp background pages plus 29 4bpp actor frames / 186 parts. The original actor
  page starts at 0x200 and frame 1 has 65 parts. Actor override 0x10 selects row 0x1F0 while
  preserving each 16-color column; the background uses row 0x1F8. No unproved actor override
  0x18 variants are emitted. External candidate-name data, font/glyph rendering and MDEC background remain separate.

The fan repository was used only as an inventory lead, pinned at commit
`50dc2b9bbac66a2bf7c888c80aefad0c09753da8`: source
`https://github.com/gothamor/tokimeki-memorial-english-patch/blob/50dc2b9bbac66a2bf7c888c80aefad0c09753da8/tools/redraw_labels.py`.
No patch implementation, translated wording or replacement art is used by the readers or tests.
An inert AST projection retained 59 numeric label boxes from seven sheet descriptions, plus
five sports resource locator sets. All 59 boxes fit original source extents and all their texels
intersect independently decoded original frames/pages. This is a geometry cross-check, not
automatic Japanese-text confirmation or whole-patch coverage. Evidence is
`.asset-cache/media/milestone-v3/external-rectangle-crosscheck.json`. MAINROM offsets 0x4000,
0xE000 and 0x16000 already have source-frame coverage; these differ from the five unreferenced
sheet diagnostics. Sports map/header/page facts are partly verified, but final viewport/CLUT
association is still unresolved; no guessed-color sports image is published.

Validated combined artifacts are `.asset-cache/media/ui-reviewed-v3/manifest.json`,
`visual_text_reviews.json`, `visual_text_manifest.json`, `coverage.json` and `gallery/index.html`.
They retain 1,372 source records, 227 exported image sources, 2,464 variants and 4,923 PNGs including
masks. All 270 new previews were individually reviewed on 24 hash-mapped contact sheets. New
classifications are 142 Japanese, 67 other-text, 58 no-text and three suspected; OPTION frames
119-121 remain suspected after individual inspection because of tiny pictogram details. Total
catalog states are 215 confirmed Japanese, 185 other-text, 222 no-text, 11 suspected and 1,831
unreviewed. Assembly keys include source, frame and variant; PNG hash alone cannot identify
aliased or identical frames. The catalog rejected duplicate review identities before publication.

The deterministic gallery has 730 default cards and 10,243 hash-verified linked references;
HTML SHA-256 is `d0a1264d1b29670b60bf6408175185404da3901fcd96a80aaeedbae59152e284`.
Coverage preserves per-image and per-part CLUT words/addresses, source geometry, palette selection
evidence and opaque extents. It has zero orphan catalog regions. All corpus-completeness flags
remain false, and the existing browser-render/hardware-pixel limitations still apply. Root owns
Git and shared wiki/log integration; these are progress checkpoints, not completed asset tickets.

## SD, sports and encounter previews, 2026-10-24

The v4 extension adds 70 original SD files bound by executable LBA/u16 layout tables:
25 weekly, 13 holiday and 32 club entries plus else index 2 form 71 caller rows, with
one identical weekly/holiday source/layout alias. The readers export 825 static frames
and preserve 1,532 source parts, the validated sequence-pointer prefix, source CLUT
words, STP masks and hashed opaque/partially represented extents. School-day actor
setup proves 8bpp with 16-pixel cells and explicit row 0x1F0 override. Other runtime
uses, unused texels and complete animation composition remain unresolved.

Four sports files provide five initial 320x240 instruction viewports. Original MOD/SRN
code reconstructs the indexed map; `Scroll` fixes the viewport and CLUT row 0x1F0.
Source mode 1 preserves the original palette/STP words. Exact texture-pointer lists
are checked, including rejection of an in-range redirected page. The former sports
viewport/CLUT uncertainty is resolved for this initial view only.

Five BATTLE encounter files provide one native 128x128 page and eight numeric locator
crops each (45 previews). C establishes 8bpp, 8-pixel cells, row 0x1F0 and palette mode 4.
The five-entry texture-pointer bound rejects index 5. Stable page/crop identities avoid
catalog collisions. Locator crop bytes are identical across these five profiles; that
does not establish whole-pack identity or coverage of the four other locator leads.

`media_ui_export.py --include-sd` resolves dynamic SD profiles from the paired original
executable. Path-bound sports/BATTLE profiles require their original LBA and do not
attach an unused executable. Coverage and visual-text profiles retain decoder metadata;
SD context requires the bound caller/path/LBA/executable metadata and never infers text.

Validated artifacts are `.asset-cache/media/ui-reviewed-v4/`. The census retains 1,372
sources, 306 exported image sources, 3,339 previews and 6,673 PNGs including masks.
All 875 new previews received manual exported-preview-only adjudication through 602
distinct hash-mapped PNGs. New classifications: 35 Japanese, 29 other-text, 778 no-text
and 33 suspected. Total catalog: 250 Japanese, 214 other-text, 1,000 no-text, 44 suspected
and 1,831 unreviewed; zero orphan regions. The 1,600 default gallery cards have 13,892
hash-checked links; HTML SHA-256 is
`e218e1534f86fd651b08d450bdfad9ecfb967b20ad086d543ac5dba8229f3218`.

`visual_text_equivalence.py` reports source/hash/geometry groups and optional exact PNG
aliases without changing review states. Geometry alone does not prove visible text under
different palette contrast. Exact byte-identical PNGs may share a manual preview
adjudication, with each member's identity/hash and runtime palette caveats retained.
The v3-corpus report `.asset-cache/media/equivalence-exact-v2.json` validates all 2,464
PNG hashes and finds 139 exact-preview groups / 359 members; one conflicting prior
classification remains blocked. Runtime equivalence and corpus-completeness flags stay
false. Independent reviews closed SD, sports, BATTLE, publication, coverage/profile and
equivalence findings; full remaining containers and older unreviewed previews are active work.

## Published-preview review census, 2026-10-24

`.asset-cache/media/corpus-reviewed-v5/` adjudicates the remaining 1,831 older previews
through 1,679 distinct PNGs on 140 hash-mapped contact sheets. Every candidate palette
PNG was inspected separately; only exact-byte PNG aliases shared a preview judgment.
This adds 170 Japanese, 100 other-text, 1,538 no-text and 23 suspected classifications.
Manual reinspection also corrected three earlier FILESEL Japanese-text classifications:
their six-member exact-PNG group is a fully transparent 16x16 image with zero nonzero-alpha
pixels. The earlier catalog remains immutable; `review-corrections.json` records old reviews
and the three changes. No review was promoted by OCR, source names or palette geometry.

All 3,339 currently exported previews now have a review state: 417 Japanese, 314 other-text,
2,541 no-text, 67 suspected and zero unreviewed. Source/palette/runtime caveats remain
independent of this preview-only adjudication. The catalog has zero orphan regions and
the gallery has 1,600 default cards / 13,892 hash-checked links; gallery SHA-256 is
`abea9f8c09f065b15df5e14f4efe45b33f3c10aa54560caf31e94c9471d7ddf8`.
The final exact-alias report verifies all 3,339 PNG hashes, finds 351 groups / 844 members
and zero conflicting review states. Its SHA-256 is
`90c1fa6356694e2a781b9e2affeee02fe85d12e6fdf5391b4808bffa2a22dddb`.
The corpus still has unsupported containers, unresolved source geometry and suspected marks;
all whole-game graphics/text-completeness flags and asset tickets remain open.

## Reviewed v5 formats and source proof, 2026-10-24

`media_background_steps.py` adds an opt-in native callback proof to the existing background
context census. It requires exact helper bodies/signatures, a strict unconditional table-copy
dispatcher, one-array callback types and configured original overlay hashes/addresses. The
native sequence must contain the literal reader followed by wait, decode and show callbacks.
Header/call macros, pointer aliases, mutations and ambiguous layouts fail closed. Both old and
current names for the two low-level background routines resolve to their original VMAs.
The current report has 25 proved native uses, 117 combined streams, 46 explicit rejections
and no unmapped/ambiguous reads. Existing unresolved/conflicting uses are retained. This adds
23 backgrounds: 78 of 313 BS sources now have previews, with 235 geometry cases unresolved.
The 23 additions were manually reviewed (five Japanese, two other-text, thirteen no-text,
three suspected); hardware IDCT/color parity remains unverified.

`image_packs_photos.py` binds 28 sources to original ETC data and literal staff-roll reads.
Thirteen native per-girl LBAs use 0x21 = 33 sectors; ALL_C uses 0x31 = 49 sectors. Their
four/six 8bpp pages use source palettes at 0x10000/0x18000 and the caller's mode-3 STP
transformation at CLUT row 0x1FF. Fourteen STAFF2 files use 0xD = 13 sectors and three
4bpp pages each; the independently hashed STAF2COL source at LBA 0xB2CC supplies the
actual row-0x1F1 palette. Only the default staff-roll variant is established. Runtime stitching,
row-120/upload-128 behavior, placement and fades remain separate. All 100 raw pages and
their STP masks are exported; all 100 were individually reviewed (three Japanese, 26
other-text, 71 no-text). `media_ui_export.py --include-photos` records the original overlay
and palette dependencies, preserves cache copies and clears stale claims if dependencies
cannot be revalidated. The reader uses immutable native ETC facts without runtime source-file reads.

The combined static-image artifacts are `.asset-cache/media/static-reviewed-v5/`: 1,372
source records, 357 exported image sources, 3,462 previews and 6,919 PNGs including masks.
Catalog states are 425 Japanese, 342 other-text, 2,625 no-text, 70 suspected and zero
unreviewed; zero orphan regions. The gallery has 1,623 default cards and 14,435 hash-checked
links. HTML SHA-256 is `06fa2dab5991a41db660fd4f673f6bf17ff4ca0a5f035957c98d904e88e203f4`.
This static catalog does not classify the separately exported movie frames.

`media_cdda.py` reads an explicitly selected CUE AUDIO track from a CUE path, directory or
ZIP. FILE BINARY samples remain little-endian, as specified by the
[GNU CUE FILE documentation](https://www.gnu.org/software/ccd2cue/manual/html_node/FILE-_0028CUE-Command_0029.html).
The original selected file is first preserved/hash-bound in cache; WAV and pregap are derived
from that copy. Unsupported flags, source/output overlap, symlink escapes and out-of-bounds
indices are rejected. Original track 2 contains 19,197 sectors; INDEX 01 starts at 150, leaving
19,047 program sectors / 11,199,636 stereo frames at 44,100 Hz (253.96 seconds). Its WAV
payload is byte-identical to the source program slice, SHA-256
`1f6ded47de4b3e4f4f54bf23e7935125ec3018401573690e0712e804c42559c0`.
The 352,800-byte pregap and full original track remain separate under
`.asset-cache/media/cdda-track2/`. No TR02.DA logical-size or runtime-use assumption is made.

The optional `tools/Dockerfile.media` reuses the existing decomp image and pins official Ubuntu
FFmpeg `7:4.4.2-0ubuntu0.22.04.1`. Its documented stdin build uses an empty context. No host
dependency is installed. The stream wrapper checks raw hashes, sector metadata, file/channel/
coding groups and exact output dimensions/sample counts. Software movie decoding is a PNG
frame sequence; original timing is not inferred from FFmpeg's demuxer time base. The original
SDK's BS v1/v2 paths converge before absolute DC/shared AC decoding; a bounded three-frame
gate proved identical coefficient arrays when only a temporary decoder-input version tag was
adapted from v1 to v2. This addresses the pinned FFmpeg decoder's v1 handling. Exact offsets,
source/derived hashes and checksum limits are recorded; staged original sectors are unchanged.
Known audio packet counts cap FFmpeg before its PSX demuxer's normal-EOF EIO path, while
strict decode errors and exact PCM frame-count checks remain enabled. SEQ rendering, complete
runtime clip mapping, hardware pixel parity and whole-game media coverage remain open.

The completed stream batch is `.asset-cache/media/streams-reviewed-v5/batch-summary.json`.
All sixteen original sources completed with no failed source, and all staged raw/manifest
hashes remained unchanged. Outputs are 14,910 XA WAV groups, 923,051,808 audio sample
frames and 997 movie PNG frames in two video groups. Independent `final-audit.json`
checked every output SHA/size and WAV/PNG header, exact source sector runs and raw aggregate
provenance: 244,083 audio sectors account for 1,968,285,312 PCM bytes; 12,351 video sectors
account for the 997 frames. Fifty-six type-0 data sectors remain explicitly unrepresented
as media, and the single ambiguous ASAHINA sector stays quarantined. ASAHINA's valid runs
are separate and its status is partial; all other sources export normally. These are native
file/channel/coding groups, not a claim that every WAV is one runtime dialogue clip. The
movie frames have not been visually classified in the static-image text catalog.

## Bounded OPENXA Japanese-text frame review (2026-10-25)

The ignored `.asset-cache/media/openxa-frame-review-392-407-v1/review-packet.json` records sixteen zero-based decoded output indices, 392–407, from `OPENXA.STR`. Each 288×192 color PNG visibly contains Japanese character overlays; an independent reviewer inspected every frame and verified its PNG hash, source frame-header fields, sector indices and original raw-track extent. The packet SHA256 is `790bdd9742e2c6c1534ae46bf521544e9289e5b390329cb217cb2b20bc4a56bd`. No text is transcribed in tracked documentation.

This is full-frame presence classification for one window within OPENXA's 829 decoded frames. It is separate from the 3,480-region static catalog and does not classify the complete 997-frame movie corpus. Software MDEC decoding has no hardware pixel-parity claim; no CLUT applies to these decoded movie images. Decoded indices and original source-frame numbers remain separate fields, and presentation timing is unproved.

Future localization needs edited frame images or synchronized replacement compositing because the Japanese text is present in the decoded pixels. The packet leaves the replacement method undecided; this is an explicit follow-up, not a completed redraw plan. The immutable review packet retains original frame and raw-stream provenance; remaining movie/whole-corpus review stays open under T-28004.

## Four school-day backgrounds, reviewed live export (2026-10-25)

The accepted six-file BS packet has manifest SHA256 `2bbe64d472e55f61bae6c928c265ea7a5097c4d8a6a3885e960166d1bdcdc414` and patch SHA256 `9300b886dc456a64ddfc9bb56d051471b73c20f06632e3d404518da5f4d4ce69`. Independent correctness and standards reviews pass after resolving self-rehashed witness acceptance, missing table-row/file binding, a stale school-day citation and incomplete placement metadata. The committed profile verifies five exact C/assembly files, original EXE/track hashes, the four original EXE table words at offset `0x75F60`, and each source file/hash/size/ISO record/index/LBA association. A fabricated self-consistent witness or a real adjacent file substituted into the wrong table row fails closed. The exporter needs no ignored report to establish these facts.

Original `C_01_27.BS` through `C_01_30.BS` produce four active 320×88 color PNGs, four 320×96 padded diagnostics and eight STP masks. Native reads request ten sectors each; the last two files have one and two sectors beyond the declared file extent. Those sector payloads are recorded as overreads and excluded from decoder input. The original mode-0 caller uploads `[704,160,320,88]` or `[320,416,320,88]`, selected by the decoder return. Two display pieces are relative to a runtime-unknown draw origin: offset `[-160,-48]`, size 192×88; offset `[32,-48]`, size 128×88. Software IDCT/RGB555 conversion remains approximate; hardware pixel parity is unproved. No CLUT applies to these YCbCr-coded images.

Reproduce the bounded export in Docker:

```sh
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 tools/assets/media_mdec_export.py disc/track1.bin --bs-caller-cohort \
  --baseline-media .asset-cache/media/etc-sd-published-v1/manifest.json \
  --repository-root . --output .asset-cache/media/bs-cohort-current-v4
```

Root's live replay verifies all sixteen PNG hashes against the reviewed packet and preserves all 1,368 nonselected manifest records, including both ETC SD sources. The standard merged catalog is `.asset-cache/media/bs-cohort-current-v4/visual-catalog.json`, SHA256 `3dd34001c326ab39887528c723a1dcfbeb3b836df78ea1793ec64863a7135f37`. Its 3,484 active regions comprise 427 Japanese, 350 other-text, 2,635 no-text, 72 suspected and zero unreviewed regions; all 6,963 referenced PNG/mask files verify. Padded images and masks are diagnostics, excluded from art counts. The four new manual reviews confirm Japanese on the chalkboard/sign in C_01_28/C_01_30 and retain small book/notebook marks in C_01_27/C_01_29 as suspected. Hash-bound subregion bounds, context, confidence and later redraw requirements remain in the accepted packet's `visual-catalog.json`; no text is transcribed. The live gallery is `.asset-cache/media/bs-cohort-current-v4/gallery/index.html`.

The coherent suite passes 466 Docker tests without skips; all 1,072 protected game/header/config/build files remain unchanged. There are now 82 exported BS sources of 313, leaving 231 unresolved geometries; 73 NBN sources and remaining movie/corpus classification stay open. All asset-completeness flags remain false.

## Seasonal school-day backgrounds (2026-10-25)

The three-file seasonal extension is independently reviewed and applied. Its candidate manifest SHA256 is `eb3e91727d7cb15b5369850961e964d8b79f06138a4a8820f7c1af3d2cc78a6c`, patch SHA256 `92b765192127e3444f3c27bf70285847f23b99f57b729b8f1a8f13b7f4e30eed`. Original selector/caller/helper instructions and the EXE table at offset `0x75150` bind four seasonal indices to LBAs `0x3FE9`, `0x3FF2`, `0x3FD7`, `0x3FE0`. Months 3–5, 6–8, 9–11 and 1–2/12 select indices 0, 1, 2 and 3; the default is 0. These are `C_02_02H.BS`, `C_02_02N.BS`, `C_02_02A.BS` and `C_02_02F.BS`, respectively. Independent checks verify 753 original instruction words, exact ISO records/file hashes/native reads and sixteen rejecting negative cases.

All four files occupy nine declared sectors; native reads request ten. Each extra sector is provenance only and excluded from decoding. Active images are 320×88; the 320×96 padded images and both sizes of STP masks remain separate diagnostics. Conditional VRAM rectangles and runtime-unknown draw origins retain the earlier caller's limits. Colors derive from the original YCbCr stream; no CLUT applies, and software IDCT/RGB555 conversion has no hardware pixel-parity claim.

```sh
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 tools/assets/media_mdec_export.py disc/track1.bin --bs-seasonal-cohort \
  --baseline-media .asset-cache/media/bs-cohort-current-v4/manifest.json \
  --repository-root . --output .asset-cache/media/bs-seasonal-current-v1
```

Root's actual replay preserves all 1,368 nonselected records and matches every one of the sixteen reviewed PNG hashes. The merged standard catalog at `.asset-cache/media/bs-seasonal-current-v1/visual-catalog.json` has SHA256 `83fda6f91b20f28ec5ca8f132c0a675dc30e2f55be4772dcb05b7b4e44d4cdf1`; all 6,971 PNG/mask references verify. Its 3,488 active regions comprise 427 Japanese, 350 other text, 2,635 no text, 76 suspected and zero unreviewed. Each of the four new active images has hash-bound manual subregion/context/confidence/localization records in the candidate's `visual-catalog.json`; all four remain suspected, with confidence 0.55. No transcription is claimed. Twelve new diagnostic outputs are excluded from art totals. The updated gallery is `.asset-cache/media/bs-seasonal-current-v1/gallery/index.html`.

The coherent current-source suite passes 483 Docker tests without skips. The three-file application preserved all 34,950 other tracked files. The ignored candidate's legacy `base_build_source_digest` field hashes only three captured base tool/test files; production code does not use it as a trust input, and it is distinct from the frozen game-source witness/build digest. There are now 86 exported BS sources, leaving 227 unresolved geometries; NBN remains 73. Movie and whole-corpus coverage stay open.

## Lossless sequence packages and provisional MIDI (2026-10-25)

This is the historical v5 checkpoint at `ee27829ae`; use the later v9 section for current commands.

The reviewed sequence reader/exporter/validator and invented-data tests are applied under [[tickets/T-28006-sequence-music-export]]. Accepted candidate v5 has manifest SHA256 `b3bb094465b65701064199980c4c21f25c0a61fe073dbbc73d1acd9ba330a29e` and patch SHA256 `0426e9aa142356a432924ab0fb0ee6240320c6aa336214fd4e01c02ea86e4dc4`. Earlier packets remain immutable. Independent review closed three validator bypasses, delta/EOT truncation and duplicate/extra manifest-row acceptance. The parser always consumes the bounded delta before status/EOT; failed validation cannot be relabeled as an unsupported source event. Native-entry headers, stable identities, exact source sets and aggregate counts are checked against a fresh original-input parse.

All 63 original SEQ files (1,277,012 bytes) export to JSON, preserving 177 physical entries and 1,008 runtime slot aliases. Package identities use source-relative path and full source hash; entry identities also include their physical offset. Offset/reverb tables, headers, gaps, complete bounded score/tail bytes and original source bytes are retained. Structured reconstruction assembles those fields and checks byte equality; retaining a raw copy alone is not the proof. Independent source/native checks verify 69 pack-index instructions, 166 `GetSeqData` instructions and 44 `ReadDeltaValue` instructions against the original EXE.

The supported event subset has 138 complete entries and 260,108 decoded events. Thirty-eight FF51 meta-event streams and one CC1 stream retain full raw bytes but produce no MIDI in this version. The default export is raw/structured JSON. MIDI requires the explicit tempo-assumption option below; it is a provisional audition artifact. Native CC99 loop/NRPN messages remain preserved controls, without General MIDI playback equivalence. Rhythm bytes remain raw; bank selection, instrument/envelope/loop rendering and native timing/hardware parity remain unproved.

```sh
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 -m tools.assets.sequence_music_export disc/files/CDROM/VAB_SEQ \
  .asset-cache/music/current-v5-json
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 -m tools.assets.sequence_music_export disc/files/CDROM/VAB_SEQ \
  .asset-cache/music/current-v5-preview --midi-preview-assuming-u24-is-mpqn
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 -m tools.assets.sequence_music_validate disc/files/CDROM/VAB_SEQ \
  .asset-cache/music/current-v5-preview
```

Root's installed CLI replay validates both modes and identical repeated writes. JSON-only output has 63 package JSON files plus its manifest (SHA256 `14ca55f2a2ccffa52f4f9573b8384fdff82ba58cfa52dc300b304d255424b4c2`). Preview output has 63 package JSON files, 138 MIDI files and its manifest (SHA256 `0a976c19df629cfddad4b8a13692355ae9d5c1c69b1c3aa87261285a1dc73ef4`). All original inputs remain unchanged. Raw/header/event/identity data match the reviewed exports, and every MIDI is byte-identical. Preview JSON embeds output paths, so relocating the output directory changes only those paths and dependent package checksums/sizes; the root audit verifies that exact transformation. This is distinct from a source-data change.

The coherent suite passes 493 Docker tests without skips, including ten music test methods and additional independent malformed/forgery/collision probes. The four new files preserve all 34,953 existing tracked files at application. No music WAV, bank closure or whole-music completion is claimed. T-28006 and the parent asset tickets remain In Progress.

## Complete current SEQ event coverage (2026-10-25)

Reviewed v9 replaces the bounded v5 reader. Its manifest SHA256 is `993013f8c3f0460e071b117f419fb7dd310c67f530c89c16d3d76c2319b1bd96`, patch SHA256 `76e7cbba45cbc181c8a78cc207dd73b183a4abc0431b706f0bde26fc84fdba26`. Independent original-instruction checks prove native FF51's fixed three-byte big-endian tempo payload, CC1 modulation dispatch and cached-FF implicit meta events. Standard MIDI uses `FF 51 03` plus the payload; the native stream has no separate length byte. Explicit FF is retained as the native cached status, rather than clearing it or retaining an earlier channel status.

All 177 entries across the 63 original files now decode: 337,797 events, including 235 tempo events (two implicit) and 42 CC1 controls. The previously rejected YUUEN entry has 1,760 events through tick 44,519 and 22 tempo events. Original package bytes, 1,008 aliases, source IDs and the earlier 138 supported channel-event lists remain unchanged. Declared-bound tests cover explicit/implicit EOT, physical padding/EOF and adjacent entry headers. The parser never counts an out-of-bound zero as framing, and metadata labels a reserved zero only when one is present within the entry. Unknown event types remain explicit failures outside this verified corpus.

The complete native PPQN/tempo/scheduler/delta/tick-rate chain establishes MPQN units; 1,212 original instruction words and controller-table entries match. The game selects 60 Hz scheduling. Header initialization rounds BPM, while tempo events truncate it, and native delta/scheduler quantization differs from raw-MPQN MIDI. MIDI is therefore an alternate audition, without exact native timing, General MIDI instrument equivalence or playback-parity claims. Bank selection, instrument/envelope/loop rendering and WAV music remain open. The old tempo-assumption CLI name remains an alias; `--midi-audition` is the current option.

```sh
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 -m tools.assets.sequence_music_export disc/files/CDROM/VAB_SEQ \
  .asset-cache/music/current-v9-json
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 -m tools.assets.sequence_music_export disc/files/CDROM/VAB_SEQ \
  .asset-cache/music/current-v9-preview --midi-audition
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 -m tools.assets.sequence_music_validate disc/files/CDROM/VAB_SEQ \
  .asset-cache/music/current-v9-preview
```

The installed CLIs validate both modes and repeat with identical bytes. Default JSON output has 63 package files plus its manifest, SHA256 `01ead6204dbbf07e1e9182abe74bea927e66f6c126f9d5c33e01b61b0a159c72`. Audition output has 63 package files, 177 MIDI files and its manifest, SHA256 `ac57d1e8fee63aeb6e3bd4d0c91e56905bdcd3bf2dd951e5f2c5d92a45464c56`. All 63 original input hashes remain unchanged. Raw/native/event/identity data and MIDI payloads match the reviewed packet; only embedded output paths and their dependent JSON checksums/sizes change when relocating outputs. No source or semantic difference is hidden by that comparison.

The composed suite passes 498 Docker tests without skips, including fifteen music test methods; independent native/boundary/forgery probes also pass. All four applied files match their captured after-images, and the application preserves all 34,953 other tracked files. Current-corpus event decoding is complete; this does not close the broader media/music ticket or prove unused formats, runtime bank binding or rendered sound.

## Conditional sequence-bank provenance (2026-10-25)

The reviewed conditional bank provenance exporter is applied. The exporter verifies all 63 original ISO SEQs by canonical path, size and SHA-256. It records 53 load requests across 45 paths; 18 have no candidate. Evidence covers 133 source calls (114 literal, 19 dynamic), 91 source pins, 24,373 native words, 839 SDK words, six tables, and 159 joins to 139 ISO records. The 177 physical entries retain 1,008 distinct runtime-slot aliases. Playback assignments are null pending successful read/cache, VAB construction/slot and epoch, pack, and sequence selection. Channel initial program is its channel-indexed VAB program; this does not imply General MIDI identity. UNDOKAI2 co-loading does not prove KARI/NININ playback. No WAV/SF2, hardware parity, or playback claim.

Run against the validated sequence catalog; outputs stay ignored:

```sh
tools/docker.sh python3 -m tools.assets.sequence_bank_export \
  --sequence-manifest .asset-cache/music/current-v9-json/manifest.json \
  --output-dir .asset-cache/music/bank-provenance/current-v3
tools/docker.sh python3 -m tools.assets.sequence_bank_validate \
  --sequence-manifest .asset-cache/music/current-v9-json/manifest.json \
  --output-dir .asset-cache/music/bank-provenance/current-v3
```

The bundled profile is source evidence: every current source/native/ISO fact is rechecked, without importing ignored investigation or reviewer files. All 437 unchanged C files yield the same call identities and graph when their filesystem creation order is reversed. A fresh otherwise-valid export of either a changed context-bearing or unresolved SEQ rejects against original ISO identity. Independent native correctness and Standards reviews pass; the coherent installed suite passes 528 tests without skips. Keep T-28006 In Progress.

Root installed CLI export/validation and deterministic repeats pass against both the reviewed and current v9 catalogs. The reviewed-input graph reproduces SHA256 `19d33755ac398bbd5a20b7fe01d0a0802d3d1af469d3eeb85c0b065365c9a5ea` exactly. The current-catalog graph has SHA256 `05cc38d342b32503ebf1165972fc9874205a0550120e0d7e852878b2e54e18b6` and manifest `1ac1a849d009cfefb6e6c231ae6ca002753185f1e7b2939bda46ec61c22378d6`; only the selected catalog hash differs between graphs. All 63 original SEQ inputs are unchanged. Numeric evidence is ignored `T-28006/bank-v3-live-cli-proof.json`.

## Prologue NBN native cells (2026-10-25)

The reviewed reader exports six native UV-mapped cells from each of CUT_05, CUT_07, CUT_08 and CUT_09. Canonical `native-cell-N` variants retain cell indices and give the generic catalog 24 distinct IDs. The source/native proof checks four original 49-sector files and 1,112 original instruction words. Lower cells sample upper-page rows 120–127 followed by lower-page rows 0–111; unused source pixels and trailers remain retained in the original copies. No final scene composition, transition timing, runtime grayscale/STP selection or hardware parity is claimed.

Both independent reviews, the corrected global-test isolation and direct-catalog regression pass. The installed coherent suite passes 560 tests without skips. Actual CLI/pixel/catalog replay preserves all 1,368 nonselected media rows and all 3,488 previous visual review/context records. All 24 color and 24 raw CLUT-bit-15 PNGs reconstruct from the original track, and four retained native files match original bytes. The mask images are diagnostics, separate from art counts. The 24 manual records retain source/cell/palette/spans/context/confidence/evidence and future redraw requirements; each confirms no text only within its reviewed 128×120 preview. Unused source pixels and composed scenes are not classified by that result.

Current accepted static coverage is 371 sources / 3,512 active regions: 427 Japanese, 350 other-text, 2,659 no-text, 76 suspected and zero unreviewed. Four NBN sources now have this bounded reader, leaving 69 generic NBN sources and 227 BS geometries open. Whole-corpus completeness remains false.

Actual ignored output: `.asset-cache/media/nbn-prologue-published-v4-verified/`. Its `gallery.html` contains all 3,512 cards, including exactly 24 selected cells; all 17,531 local gallery references and 48 diagnostic-page references pass containment and hash checks. `visual-reviews.json` preserves all previous review rows and rebuilds the complete catalog exactly. The separate `raw-clut-bit15-diagnostics.html` states its unproved runtime interpretation.

Manifest SHA256 `26d865d627e3ab103514d256b1b36aad6fd5eed5108877d82b863c22a3e5efde`; catalog `d155a2958e7e8694a9b5444412ef71843a0229b15503e713e911a9c1b5f8b4d6`; review sidecar `994e3ec664b20909a0510e9fc8eec53680532e86cb382fe87ed474aecb443207`; gallery `c3d8b97d509636998a7fc2dbb650d45c2d67441b32e07e64bc6251b5423c021e`. Exact source/installed/replay evidence is ignored `T-28002/nbn-v4-{live-application,current-replay}-proof.json`. The first replay caught a Python integer-key versus persisted JSON string-key comparison; every one of the 3,488 inherited region records is equal after normal JSON serialization, without dropping fields or normalizing values. Its failed output remains preserved.
