---
id: T-39400
title: Finish the missing English art
status: In Progress
assignee: worker agent
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39004-english-asset-completion]]", "[[assets-media]]"]
---

## Goal

Make English drafts for the 21 art items the inventory lists as missing, so the English patch can replace every Japanese sign and label the owner wants translated.

## Acceptance criteria

- [ ] The nine MAINROM genre tags (frames 0x134, 0x135, 0x138, 0x139, 0x141-0x145) are registered in `tools/translation_inventory.py` from the existing port-made checkpoint, and the inventory counts them as drafts.
- [ ] The twelve BS sign stills have corrected file bindings in the image text catalog, a size proven from the decoded stream or the calling code, and an English draft drawn in each sign's style.
- [ ] The drafts are recorded in a checkpoint ledger with source and output hashes, and the inventory and translation tests pass.
- [ ] No original game image is committed; only the English drafts, as with the existing artwork.

## Comments
