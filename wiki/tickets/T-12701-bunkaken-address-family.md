---
id: T-12701
title: Match BUNKAKEN flag and address family
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ['[[tickets/T-12700-bunka-families]]']
---

## Goal

Match remaining handlers in src/ovl/BUNKAKEN/80140F60.c using existing bitfield drafts and full translation-unit comparisons.

## Acceptance criteria

- Literal linked-byte proof and affected overlay checksum for accepted C.
- Preserve unsuccessful experiments and document results.

## Notes

Own remaining BUNKAKEN sources and needed overlay-header prototypes. One compiler at a time. Parent maintains shared wiki index, kanban and log.

## Comments

2026-10-15: Read prior broad experiment; start from eight-row func_80141654 address mismatch and 22-row siblings, without repeating default sweeps.

Five existing bitfield drafts match after using a local `GameState *g = &D_800E6280` for every GameState access. Store-only replacement leaves the original mismatch. Full-TU exact scores: func_80141654 8 -> 0; func_801422C4 43 -> 0; func_80142A40 43 -> 0; func_80142FD4 -> 0; func_801438F8 -> 0. This transfers an existing documented idiom, not a new compiler rule. func_80141B80 remains 134 rows and stays assembly.

Docker `ninja -j1 build/ovl/BUNKAKEN.ok` passes SHA1. `funcdiff.py --unit BUNKAKEN --resolve` verifies literal linked bytes for func_80141654 (1324), func_801422C4 (1440), func_80142A40 (1428), func_80142FD4 (1216), func_801438F8 (1136): 6544 bytes total. Evidence in build/bunka12701/build.log and proof.txt; unsuccessful current-TU candidates retained alongside.

Sixth match: func_80141B80 (1860 bytes). The existing matched sibling handler in src/ovl/BUNKAKEN/8014A6A0.c uses BunkakenModeFlags b1=1 and mode=0 before saving GameState fields 0x720..722. Replacing the draft's combined mask with those bitfields reduces 134 -> 24 rows; moving them before the state saves gives zero. Six selector-type/return/local variants had no improvement. Literal linked-byte proof and BUNKAKEN SHA1 pass in build/bunka12701/next-proof.txt and next-build.log. Total this ticket: 8404 verified bytes.
