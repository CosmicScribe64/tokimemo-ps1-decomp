---
type: reference
updated: 2026-10-29
sources: ["tools/assets/input.py", "tools/extract_disc.py", "wiki/disc-layout.md"]
---

# Original input inventory reader

Use `tools/assets/input.py` to read extracted directories, raw MODE2/2352 data-track images, or ZIP archives containing a raw data track. It opens sources read-only. `open_input(path)` is a context manager whose source exposes `describe()`, `entries()`, and `read(entry)`.

Each entry records a stable ISO-style relative path, declared size, kind, source location, and SHA-256 for readable files. Image locations include the ISO directory-record address, LBA, extent size, and sector extents. Archive descriptions identify the selected data-track member and list the other members without extracting them.

`read(entry)` returns bytes for ordinary files. Form2 ISO records and extents beyond the data-track image remain in the inventory as `unsupported`; this API cannot read them. Form2 records identify stream sectors, but their ISO extent size is not the stream payload byte count. The out-of-image reference is labeled as a CD-DA reference. Directory reads stay within the source directory, and symlinks and non-regular files are reported as unsupported.

Raw images must have MODE2/2352 sync and whole sectors. For ZIP input, the reader uses a single CUE selector when present. Otherwise, it requires a unique Track 1 member or a sole raw candidate. Ambiguous archives fail. This module does not implement other archive formats, cooked ISO, direct CUE/CHD/7z input, multi-session handling, or multi-extent ISO files.

## Local validation (2026-10-24)

The user's local PlayStation-the-Best ZIP selected its Track 1 member (702,072,000 bytes). Inventory produced 1,372 ISO entries: 1,355 readable files and 17 explicit unsupported entries (16 Form2 XA/STR records and one extent beginning beyond the data track, the CD-DA reference). Readable declared file sizes total 84,803,459 bytes. Inventory reads the source in place; no game data or extracted content is stored in this page. Synthetic tests cover raw image and ZIP reads, file extent metadata, directory reads, symlink rejection, and cooked-image rejection (`tools/test_asset_input.py`; Docker run passed 3 tests).

## Boundaries and caveats

The reader handles the observed single-data-track MODE2/2352 layout. It validates raw sector mode and duplicated XA subheaders for file payloads. Directory sectors have one documented exception: a malformed subheader is permitted, and directory data is read from +16. ZIP CUE selection uses the first `FILE` entry. Ambiguous tracks without a selector fail.

ISO multi-extent and interleaved entries are listed as unsupported. PVD and directory bounds are checked against the physical track. This disc declares 317,697 ISO volume sectors, but its data track contains 298,500. The reader records both values and caps traversal at the physical track. It does not decode XA audio/video, interpret CD-DA samples, or reconstruct stream sectors as files.

Directory hashes use sorted relative paths with length-framed metadata and content. Source hashes are SHA-256 of the original input file. Non-selected ZIP members contribute metadata only; they are not read as asset payloads.

Later 2026-10-24 validation passed eight input tests, within the 106-test owned media suite.
`read(entry)` can now accept a cached inventory entry before `entries()`; raw and ZIP readers
initialize their sector reader lazily from the opened track size. Entry shape, source path, size
and integer LBA are checked, and reads still use the same bounded sector/Form2 path. Callers using
cached provenance must compare the returned bytes with the expected file hash, as the export tools do.
The source-only v2 checkpoint and outstanding format gaps are recorded in [[assets-media]].

## Raw XA/STR preservation extension, 2026-10-24

`InputSource.iter_raw_sectors(entry, expected_input_sha256)` is separate from Form1 ISO
`read()`. It validates original MODE2 framing, duplicated subheaders, extent/sector bounds
and the whole-input binding; source ISO sizes count 2048-byte address units and never stand
for Form2 payload lengths. `input_stream.py` stages canonical baseline entries only beneath
`.asset-cache/input`, rejects symlink/overlap and relabeled-interleaving cases, and retains
raw-sector and payload hashes separately. Strict iteration still rejects a mismatched subheader.

The explicit opaque-preservation mode retains all original bytes and both subheader copies
without choosing fields when the copies disagree. All sixteen original XA/STR extents are
preserved at `.asset-cache/input/form2-batch-v3/aggregate.json`: 256,491 sectors / 603,266,832
raw bytes. Exactly one ASAHINA.XA sector (LBA 67338) has conflicting subheaders. Its decoded
form/payload/coding fields and complete payload hash remain unresolved; valid runs are split
around it for decoding. The other 256,490 subheaders validate. Known geometry covers 12,407
Form1 and 244,083 Form2 sectors, with 12,351 video-bit and 244,083 audio-bit sectors.
Raw preservation is complete for those extents; stream interpretation is partial for ASAHINA.
EDC verification was not performed, and original sources were never repaired or modified.

The independently CUE-bound audio track is described in [[assets-media]]; it is not assigned
the logical extent of the data-track TR02.DA reference. Source-sector and archive-member
provenance remain distinct from logical-file hashes and from decoded media outputs.
