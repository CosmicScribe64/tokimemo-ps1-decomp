---
id: T-39501
title: Rebuild the disc image with replaced files and make the xdelta patch
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

From the user's disc image, write a patched track 1 with replaced or relocated files, valid Mode 2 EDC/ECC and an unchanged layout elsewhere, then make and apply an xdelta patch.

## Acceptance criteria

- [ ] Rebuilding with no replacements gives a byte-identical track.
- [ ] Replacing `SLPM_86.053` with the matching build output gives a byte-identical track.
- [ ] Replaced and relocated files get correct EDC/ECC, ISO9660 records and sizes; the patched disc boots in the emulator.
- [ ] xdelta3 patch creation and application round-trips; the input track is checked by SHA-1 first.

## Notes

Part of [[tickets/T-39500-english-translation-patch]]. Disc facts: [[disc-layout]].

## Comments
