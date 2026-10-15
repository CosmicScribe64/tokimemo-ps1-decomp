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

Further archived family transfer adds func_80138A08 (1200 bytes) and func_8013A8CC (804 bytes), both still assembly at the baseline. Existing byte-store bitfields and mode-before-save ordering finish their seeds. BUNKAKEN SHA1 and literal proofs pass (build/bunka12701/four-build.log, four-proof.txt). func_80141088 remains 228 rows. Three other tested historical seeds were already C in this branch and are controls only: 3C634 and 3CA6C reproduce existing source exactly except whitespace (restored); 3C0AC's old seed remains122 rows, while current C already matches. No coverage credited to controls. Ticket total: eight new functions / 10408 literal bytes.

Targeted fresh siblings in src/ovl/BUNKAKEN/8014E440.c: func_8014E5E0 (688), func_8014E890 (608), func_8014EAF0 (868) all exact zero with the established GameState/bitfield source transform, then literal linked proof and overlay SHA1 pass (three-proof.txt, three-build.log). All three were confirmed still assembly before experiments. New gain2164 bytes; cumulative11 functions /12572 bytes. No new compiler settings or fake expressions.

All eight remaining held-out handlers in src/ovl/BUNKAKEN/8014E440.c also match the same source transformation: 8014EE54 (960), 8014F214 (1040), 8014F624 (948), 8014F9D8 (1308), 8014FEF4 (848), 80150244 (600), 80150680 (728), 80150958 (880). Eight literal proofs and BUNKAKEN SHA1 pass in eight-proof.txt/eight-build.log. This file now contains only C. Gain7312 bytes; cumulative19 functions /19884 bytes.

Adjacent-family seed func_8013EC98 initially differs40 rows, all branch relocation shifts plus two extra argument moves at func_80083440. The existing BUNKASAI K&R override convention (callee narrows argument) gives exact zero. Added explained per-TU override in src/ovl/BUNKAKEN/8013EB30.c; canonical header unchanged. Full overlay SHA1 including existing C passes and literal780 bytes verified (kr-build.log/kr-proof.txt). Cumulative20 new functions /20664 bytes.
