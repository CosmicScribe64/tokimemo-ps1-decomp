---
id: T-39501
title: Rebuild the disc image with replaced files and make the xdelta patch
status: Done
assignee: claude
created: 2026-11-01
updated: 2026-11-02
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

From the user's disc image, write a patched track 1 with replaced or relocated files, valid Mode 2 EDC/ECC and an unchanged layout elsewhere, then make and apply an xdelta patch.

## Acceptance criteria

- [x] Rebuilding with no replacements gives a byte-identical track.
- [x] Replacing `SLPM_86.053` with the matching build output gives a byte-identical track.
- [x] Replaced and relocated files get correct EDC/ECC, ISO9660 records and sizes; the patched disc boots in the emulator.
- [x] xdelta3 patch creation and application round-trips; the input track is checked by SHA-1 first.

## Notes

Part of [[tickets/T-39500-english-translation-patch]]. Disc facts: [[disc-layout]].

Done (2026-11-01), branch `patch/disc`:

- `tools/patch/sector.py`: Mode 2 sector layout, EDC (CRC-32, polynomial 0x8001801B reflected) and ECC P/Q (Reed-Solomon over GF(2^8), row-wise with `bytes.translate`, about 0.6 ms a sector in CPython). Recomputing every Form 1 sector of the original track 1 gives the same bytes, except LBA 298499 (see [[disc-layout]]).
- `tools/patch/disc.py`: ISO9660 parser and `Disc` rebuild, CLI `list` and `rebuild`. API for the patch build:
  - `Disc(track1_path)` (context manager), `disc.iso.read(path)`, `disc.iso.find(path) -> Entry(lba, size, sectors, rec_lba, rec_off)`
  - `disc.replace(path, data, relocate=False) -> lba`: in place if it needs no more sectors (freed sectors become empty filler), else moved to LBA 298350 onwards; each file once; streams (Form 2, CD-DA) refused. Subheaders: the originals when the sector count is unchanged, else the first sector's for the body and the last sector's (EOR/EOF) for the end.
  - `disc.write(out_track1, cue_path=None, track2_path=None) -> sha1`: once, after every replace. Moved files push the 150-sector postgap and track 2 back; the PVD volume space size and the TR02.DA extent follow; directories never move, so path tables never change.
  - `write_cue(cue, track1, track2)`. The patch build relocates data files first, writes their LBAs into the code, then replaces `SLPM_86.053` (the BIOS finds the exe through the ISO, so it can move too).
- `tools/patch/make_patch.py`: `make ORIG PATCHED OUT.xdelta` (VCDIFF, `-S none` for Rom Patcher JS, `-B` 1 GiB so moved data is found; applies the new patch once to verify) and `apply ORIG PATCH OUT [--out-sha1]`; both check the input SHA-1 (`--source-sha1 any` skips). Exe moved to the end with one byte changed: 7,079-byte patch.
- xdelta3 3.0.11 comes from `tools/Dockerfile.patch`, a small image on top of `tokimemo-decomp-arm64` (like `Dockerfile.media`), so the toolchain image is not rebuilt; it inherits the base's `dockerfile.sha256` label, so `IMG=tokimemo-patch tools/docker.sh ...` works.
- Tests: `tools/test_patch_disc.py`, 17 tests, all pass in `tokimemo-patch` with the disc mounted (`TOKIMEMO_ORIG_DISC`, `TOKIMEMO_MATCH_EXE`, `TOKIMEMO_PATCH_TMP`), about 50 s; without the disc or xdelta3 the disc and xdelta tests skip. Covered: every Form 1 sector recomputed, no-change rebuild and the matching `build/SLPM_86.053.bin` both give SHA-1 00b59509..., in-place replacement (changed sectors exactly the record, the changed one, the new last and the freed ones; EDC/ECC valid; reads back), relocation round trip (two files, LBAs, volume size, TR02.DA, postgap headers, cue), the damaged `/CDROM` sector rewritten unchanged, xdelta round trips (synthetic and full track) and SHA-1 refusals.
- Boot check (`tools/emu/emu.py --cue`, new alias of `--iso`): A no-change rebuild, B exe header padding byte 0x7F0 set and the exe moved to LBA 298350 (326 sectors added), C the same byte changed in place. All three boot to NAME_ENT (A and C frame 3538, B 3548, the longer seek), B renders the title intro at frame 2100.
- `tools/emu/run.sh` rebuilds `tokimemo-emu` on every call: the current image has no `dockerfile.sha256` label. The boot runs used the same `docker run` line directly so the shared image was not rebuilt.

## Comments

## Completed source review

2026-11-02: Reviewed tools/patch/disc.py, sector.py and make_patch.py against CODING_STANDARDS. Directory updates retain both ISO byte orders; relocated files move the postgap and audio reference together; stream replacements preserve non-video sectors. Patch generation applies its output and checks the target hash. The real-disc Docker suite passes all 17 tests, including unchanged and matching-executable rebuilds, relocation, EDC/ECC and xdelta round trips. No unresolved finding for the supported original-disc build. No source or game bytes changed in this review. Ticket and board move to Done.
