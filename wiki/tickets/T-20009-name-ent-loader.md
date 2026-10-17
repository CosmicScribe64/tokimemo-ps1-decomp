---
id: T-20009
title: Match NAME_ENT CD-level display helper
status: Done
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[name-ent-family-results]]", "[[matching-notes]]"]
---

## Goal

Match NAME_ENT func_80149B08 (484 bytes) in src/ovl/NAME_ENT/80144AD0.c. The initially investigated func_80142E24 loader remains ASM and is tracked separately in [[tickets/T-20010-name-ent-resource-loader]]. Claim opened before implementation: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues/52. Branch codex/name-ent-loader starts at d73328b8; PR #51 and claim #50 remain separate. Neighboring functions, TT and DATE2 are outside this scope.

## Acceptance criteria

- [x] Recover the CD-level display behavior using the existing DisplayActor44 array.
- [x] Verify promoted C against actual linked bytes, all 27 binary hashes and header/global/prototype guards.
- [x] Review CODING_STANDARDS.md, annotate measured source controls and prepare the scoped PR.

## Notes

T-17900 records 44 loader diff rows, but its local table is incorrectly declared as one word despite eight stores. The older typed record substitutions also need address auditing. The display helper's archived hand port has 35 differing instructions after its second status call, around a delayed record-96 X-coordinate load. Use real aggregates and inspect default header views before any overrides or padding controls.

Full-unit controls: loader table is eight words (two rows of four), 0x10-byte row stride. Correct eight-word declaration compiles but the historical 44-row draft had only one word and an invalid frame. The helper starts 21 aligned edits away as void /22 as s32; first49 instructions agree. Return-view sweeps completed35 loader and6 helper compiles with zero untested variants, no gain. The allocator trace identifies a helper priority-1 tie: global D_80120676 is allocated before four hoisted constants, shifting V0/V1/A0/A2/A3; remaining structure is order-only. Pointer/word views, scalar snapshots, copying shared sprite fields, line grouping and return widths have not closed it. Loader controls narrow differences to opening loop plus frame; fake extra draw arguments and capacity9 are diagnostic only, not accepted source. Running bounded180-second permuter controls in ignored build/T-20009; any output still requires review and full linked proof.

## Match and source controls

The helper samples getCDlevel when the status bit is set, stores four samples, averages/divides to select a frame capped at three, and configures actors 97/98 with frames +6/+10, sharing actor 96's X coordinate. Actor 100's mode is cleared. All fields use DisplayActor44 over D_8011ECD0; initialize that local pointer before the status calls. Initializing it inside the final if or using only direct aggregate accesses leaves 20 aligned edits. The before-call view fixes the equal-priority ordering so constants occupy V0/V1/A0/A2 and the shared X load A3, reproducing the original early load and stores.

Measured retained FAKE controls: two unused words keep the original 0x40 frame (without them only the two stack adjustments differ, 0x38); joined dead average/frame initialization keeps the CD-level/average register order (eight instruction differences if removed); s32 with no return keeps implicit-int exit liveness (void differs in three instructions). These controls are commented with T-20009. The real actor pointer replaces the older draft's unused scalar home; it is not an extra copied SDK type. One s32 func_80149B08(void) declaration is added to NAME_ENT.h, with no shared-header override.

## Validation and review

Docker default ninja build passes (nine steps), including headers OK/globals OK. Independent SHA1 checks pass for all 27 binaries. sync_protos.py --check-branch: 0 new / 15 known disagreements, branch OK. Actual default linked funcdiff: MATCH, 484 bytes; this function is plain C with no ASM fallback. Logs are ignored under build/T-20009. Source-format diff check passes.

Self-review against CODING_STANDARDS.md: fixed-width C89 declarations and comments; existing actor fields and canonical shared API; one overlay declaration, no duplicate main externs; required frame/allocation/exit controls explicitly FAKE; production source location and symbol preserved; no generated ASM, raw assets, binaries, compiler artifacts, probe scripts or comparison instruction listings submitted. Ticket/kanban In Review; maintainer review remains pending. Loader work and the large T-20008 scene are not claimed as matches.

## Comments

2026-10-17, maintainer review (PR #53): no findings; the three FAKE controls name T-20009 and the reason. Verified in the integration tree: clean 27/27, headers, globals, prototypes OK, funcdiff MATCH for func_80149B08 (484 bytes). Status Done.
