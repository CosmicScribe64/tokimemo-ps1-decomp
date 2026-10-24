---
type: reference
updated: 2026-10-24
sources: ["tools/export_assets.py", "tools/assets/images.py", "tools/assets/audio.py", "wiki/spec/h-graphics.md", "wiki/spec/h-cdio.md", "wiki/spec/m-dates-a.md"]
---

# Bounded image and audio exports

[[asset-export]] defines the scope; [[assets-input]] describes the read-only input API.
`tools/export_assets.py` inventories every supplied entry and decodes the supported subset.
No engine integration or changes to the matching build are involved. All real outputs belong in
ignored `.asset-cache/media/`; use separate output directories for different input/options.
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

Deferred: 313 `.BS` backgrounds (MDEC), 178 other `.NBN` files including `TITLE.NBN`, 308 other
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
