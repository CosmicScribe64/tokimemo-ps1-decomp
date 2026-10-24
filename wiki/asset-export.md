---
type: plan
updated: 2026-10-24
---

# Reusable asset extraction and exports

User scope (2026-10-24): put agents on asset extraction/readers, especially text for future translation,
and general modern exports useful to the decomp. No direct Godot work. Parent [[tickets/T-28000-asset-export-pipeline]].

## Pipeline

Read original user inputs without changing them; identify release and enumerate exact source extents.
Keep lossless originals/provenance and produce deterministic modern exports with format metadata.
Outputs live under ignored `.asset-cache/`, outside build/ so matching clean builds cannot delete them.
Commit tooling, synthetic fixtures, format descriptions and manifests of schema/coverage only, never
extracted game text, assets, screenshots, executable bytes or save files. Do not quote dialogue in docs.

- [[tickets/T-28001-text-script-export]]: comprehensive structured text, original-byte/control-code fidelity, stable IDs,
  source locations, name substitutions, choices and voice links where proved. Ordinary string scanning
  alone is not a complete dialogue parser; translation itself is deferred. The reported44unsupported SERIFU
  files are a blocking backlog, not an accepted final export. Cover dialogue, choices, menus, labels,
  names/place tables and executable/overlay text, with source-backed decoding and context.
- [[tickets/T-28002-media-asset-export]]: supported images to PNG with palettes/frame/placement metadata;
  supported audio to WAV with loops/banks/rates, keeping sequenced-music rendering distinct from sample decoding.
  Keep faithful palette-applied color previews/galleries and verified variants for browsing. Clearly label
  raw-index/STP masks as diagnostic; an unresolved palette is not a finished monochrome art export.
  Never invent colors; preserve actual source CLUT provenance and raw representation.
- [[tickets/T-28004-japanese-image-catalog]]: asset-by-asset Japanese text inventory, prioritizing main
  HUD/command/calendar/menu icons, then buttons, banners, title/overlay graphics and background signage.
  Preserve source/frame/palette/region/context and explicit unreviewed/suspected/confirmed/no-text states;
  list later redraw/font/string requirements without making replacement art now.
- [[tickets/T-28003-original-input-assets]]: read-only original-input handling and complete coverage inventory;
  existing disc extraction deliberately skips XA/STR streams and a CDDA reference ([[disc-layout]]), so these
  cannot be silently reported as exported.

All tools/dependencies run in Docker; reuse existing images. Output schema is engine-independent. The
matching build keeps its proven original data path while export/rebuild round-trip support is validated.
A PNG/WAV export does not by itself prove original-byte reconstruction; retain source encodings and metadata.

## Reference practice

Primary upstream examples checked on 2026-10-24:

- [SM64 extractor](https://github.com/n64decomp/sm64/blob/master/extract_assets.py): original-ROM extraction,
  a revisioned local asset list, image and audio outputs.
- [OoT image pipeline](https://github.com/zeldaret/oot/blob/main/docs/assets/images.md): extract images to PNG,
  convert them back to console data at build time, preserve palette relationships and separate original exports
  from replacement assets. These are examples to learn from, not a requirement to copy an N64 format pipeline.

The existing [[tickets/T-0002-disc-extraction-and-exe-identification]] and tools/prepare_disc.py cover build
input extraction, not a general modern-format exporter. New implementation tickets above extend that work.
