---
type: reference
updated: 2026-10-24
sources: ["tools/assets/input.py", "tools/extract_disc.py", "wiki/disc-layout.md"]
---

# Original input inventory reader

`tools/assets/input.py` provides read-only source access for extracted directories, raw MODE2/2352 data-track images, and ZIP archives containing a raw data track. `open_input(path)` is a context manager. The returned source has `describe()`, `entries()`, and `read(entry)`. Each entry has a stable ISO-style relative path, declared size, kind, source location, and SHA-256 for readable files. Image locations include the ISO directory-record address, LBA, extent size, and sector extents. Archive descriptions identify the selected data-track member and list the other archive members without extracting them.

Ordinary file entries can be read as bytes. Form2 ISO records and extents beyond the data-track image are retained as `unsupported` inventory records and cannot be read through this API. Form2 records identify the stream sectors; ISO's extent size is not treated as the actual stream payload byte count. The out-of-image reference is labeled as a CD-DA reference. Directory symlinks and non-regular files are reported as unsupported; reads are confined to the source directory. Raw image input must have MODE2/2352 sync and whole sectors. ZIP selection prefers a member named as Track 1, otherwise the first sorted `.bin`/`.img`; other archive formats, cooked ISO, CUE/CHD/7z, multi-session handling, and multi-extent ISO files are not implemented by this module.

## Local validation (2026-10-24)

The user's local PlayStation-the-Best ZIP selected its Track 1 member (702,072,000 bytes). Inventory produced 1,372 ISO entries: 1,355 readable files and 17 explicit unsupported entries (16 Form2 XA/STR records and one extent beginning beyond the data track, the CD-DA reference). Readable declared file sizes total 84,803,459 bytes. Inventory reads the source in place; no game data or extracted content is stored in this page. Synthetic tests cover raw image and ZIP reads, file extent metadata, directory reads, symlink rejection, and cooked-image rejection (`tools/test_asset_input.py`; Docker run passed 3 tests).

## Boundaries and caveats

The current reader handles the observed single-data-track MODE2/2352 layout. It validates raw sector mode and duplicated XA subheaders for file payloads; for directory sectors only, it preserves the documented disc quirk that permits a malformed subheader and reads directory data from +16. ZIP CUE selection uses its first `FILE` entry; archives with ambiguous tracks and no selector fail explicitly. ISO multi-extent and interleaved entries are listed as unsupported, and PVD/directory bounds are checked against the physical track. This disc declares 317,697 ISO volume sectors while its data track contains 298,500; the reader records both values and caps traversal at the physical track. It does not decode XA audio/video, interpret CD-DA samples, or reconstruct stream sectors as files. Directory hashes use sorted relative paths with length-framed metadata and content. Source hashes are SHA-256 of the original input file; non-selected ZIP members are metadata only and are not read as asset payloads.
