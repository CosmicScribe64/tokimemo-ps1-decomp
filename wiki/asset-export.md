---
type: plan
updated: 2026-10-25
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
  alone is not a complete dialogue parser; translation itself is deferred. The initial 44 unsupported SERIFU
  files were a blocking backlog. Current profiles classify all 45 SERIFU files; this closes the container-profile
  gap, while 1,889 semantically unknown executable/overlay extents and other reader/context gaps still block
  full translation coverage. Cover dialogue, choices, menus, labels,
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

## Current-source evidence continuation (2026-10-25)

Phase 2 is complete and pushed at `d7fb33f960395d4c32ea238aabda82f211689973`. Its stable C/header/path layout permits the deferred asset-evidence rebase. A new ignored witness at `.asset-cache/shared/source-snapshot-phase2-687b5ce7` captures tracked `src/`, `include/` and `config/` from that commit, plus the final clean generated `asm/`. The capture verifies unchanged live build-source digest, both copies of every assembly-file hash and all 437 C files. It contains 14,697 files; manifest SHA256 `3fede6401e79c19a0982a596899744433658f749c7702b7c6c28d55dd487c0eb`, source-tree SHA256 `75eafb5958b44eef4c1ead490eea7bc06372d094e2cf939854a3ab7b67b638cd`. The historical f9c witness remains immutable. This is an evidence-capture checkpoint, not proof that existing profiles accept current source or that text/media coverage is complete.

The reviewed text rebase, bounded RPG initial-buffer reader and native shooter display-input proof are applied. Current-source proofs cover 38,701 units (all 38,696 previous rows plus five flagged initial-only RPG views), 8,837 physical extents and 2,330 verified C identities. The 31,346 ready rows are unchanged; initial contents do not prove runtime values or display lifetime. One previously unresolved global locator follows its renamed file; twelve existing shooter extents gain original-instruction-backed contexts, leaving 1,877 semantically unknown extents. Glyph modeling and comprehensive translation coverage remain unproved. Reviewed media work adds eighteen native-color ETC SD frames and eight source-proved school-day backgrounds, including four seasonal variants. The static corpus now has 367 image sources / 3,488 regions, while 73 NBN sources and 227 background geometries remain unresolved. The coherent asset suite passes 493 canonical tests without skips. Movie image-text review continues separately. Sequence-music tooling now preserves all 63 original packages as structured JSON and supports 138 opt-in provisional MIDI entries; 39 event streams, bank dependencies and native WAV rendering remain open under [[tickets/T-28006-sequence-music-export]]. Agents prepare ignored packets; root reviews and serializes live application, tests, shared wiki and commits. [[assets-text]], [[assets-media]], [[tickets/T-28001-text-script-export]], [[tickets/T-28002-media-asset-export]], [[tickets/T-28004-japanese-image-catalog]].
