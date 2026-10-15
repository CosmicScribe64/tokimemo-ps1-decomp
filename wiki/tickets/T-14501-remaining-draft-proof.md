---
id: T-14501
title: Verify and integrate remaining-function drafts
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[matching-notes]]", "[[tickets/T-14500-exhaustive-drafts]]"]
---

## Goal

Promote ordinary C from the exhaustive T-14500 draft census only after default linked-byte proof and the enclosing binary checksum pass. Preserve existing C.

## Acceptance criteria

- [x] BUNKAKEN: 12 functions / 8,384 bytes verified using Docker `tools/funcdiff.py --resolve --no-build`, without `NON_MATCHING`.
- [x] BUNKAKEN overlay SHA1 passes.
- [x] Header/global checks and prototype branch check pass.
- [x] Existing C preserved; source diff replaces only 12 INCLUDE_ASM lines.
- [ ] Required code-review skill axes completed before Done.

## Notes

BUNKAKEN functions: func_80137804 (564), func_80139184 (480), func_80139364 (488), func_80146700 (732), func_801469DC (688), func_80148094 (788), func_80148594 (584), func_80148A28 (1,088), func_801513E8 (848), func_80151738 (832), func_80151EC0 (500), func_80152300 (792).

Local evidence: build/t14500/rows.json, build/t14500/proof.txt, /private/tmp/t14500-proof-build.log, /private/tmp/t14500-protos.log. Generated drafts remain in the separate T-14500 catalog. No asm or game data added.

## Comments

2026-10-15: Claimed proof lane under parent T-14500. Focused CODING_STANDARDS.md inspection: C89, fixed-width local types, ordinary state-machine C; no forced registers, dummy operations, oversized locals, or FAKE patterns. No new declarations or overrides. Removed empty generated declaration-marker comments. Ticket remains In Review for the full code-review gate. Prototype check: zero new disagreements, 15 known.

2026-10-15: BUNKASAI batch: 6 functions / 4,248 default linked bytes verified after GameState normalization; overlay SHA1, header/global checks and prototype branch guard pass. Focused standards inspection: ordinary C89 state handlers; no FAKE artifacts; original C preserved. Functions: func_8013ABA0 (616), func_8013AE08 (792), func_80141F70 (808), func_801464A8 (596), func_8013A95C (580), func_80143C38 (856). Evidence: /private/tmp/t14500-BUNKASAI-proof.log and local build/t14500/BUNKASAI-proof.txt.

2026-10-15: BUNKA_SD batch: 1 functions / 908 default linked bytes verified after GameState normalization; overlay SHA1, header/global checks and prototype branch guard pass. Focused standards inspection: ordinary C89 state handlers; no FAKE artifacts; original C preserved. Functions: func_80136578 (908). Evidence: /private/tmp/t14500-BUNKA_SD-proof.log and local build/t14500/BUNKA_SD-proof.txt.

2026-10-15: DATE batch: 2 functions / 1,088 default linked bytes verified after GameState normalization; overlay SHA1, header/global checks and prototype branch guard pass. Focused standards inspection: ordinary C89 state handlers; no FAKE artifacts; original C preserved. Functions: func_8013E7B0 (464), func_80145EC4 (624). Evidence: /private/tmp/t14500-DATE-proof.log and local build/t14500/DATE-proof.txt.
