---
id: T-9580
title: "Wave 8: list 8"
status: Done
assignee: wave-8-agent-8
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[decompile-workflow]]"]
---

## Goal

Match the 158 functions (84,124 bytes) in wave8-list-8.txt, limited to its 36 owned C files, using the shared wave-8 brief.

## Acceptance criteria

- [x] Every listed function matched or time-boxed with failed drafts reverted.
- [x] Individual matches verified in Docker; final ninja passes 27 units and header/global guards.
- [x] Prototype branch check and inline coding-standards/spec review completed.
- [x] Results recorded, wiki and board synchronized; changes left uncommitted.

## Notes

Shared brief and list read; Docker-only tooling, append-only overlay declarations, and no tool/config/build-system changes.

## Comments

- 2026-10-11: Claimed list 8. Starting from HEAD with a clean worktree.

- DATE/func_80149F48 (176 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- EN_NICHI/func_80134F2C (280 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- EN_NICHI `func_80134F2C` typed retry: `EnObj` plus read-modify-write on `unk_1C`, calculation before constant store, and leading scalar yields exact instructions and spill at +0x28, but frame remains 0x38 versus original 0x30. K&R unchanged; 35-second permuter best score 10. Reverted. Permuter emitted `KeyError: arg1` on the K&R definition.

- TT/func_8013923C (100 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- EN_NICHI/func_80132A74 (104 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- EN_NICHI/func_80135A48 (132 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_80133AD0 (108 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- ETC/func_80146AC0 (128 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_80140B20 (132 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_80134ADC (224 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TT/func_80134874 (176 bytes): time-boxed. Missing draft declarations need manual recovery.

- main/_sys_default_tpage_set (300 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- main/load_tpage_buf_lock (168 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- EN_NICHI/func_80136A2C (228 bytes): time-boxed. Missing draft declarations need manual recovery.

- DATE/func_801551D0 (92 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- DATE/func_80149268 (168 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- TT/func_80134D0C (276 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- GYOZI/func_80136124 (132 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- ETC/func_801464D4 (292 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- RPG_BAT/func_801453B4 (316 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- NAME_ENT/func_8013AF78 (296 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TACO/func_8015435C (224 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- TACO/func_80154230 (236 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- DATE `func_801551D0` (92 bytes) repaired and verified by `funcdiff --resolve`: `s32` return, early `return 0`, local `GameState *g`, unsigned `++g->unk_1104.u`; local pointer gives the original `$v1` counter. Full build pending batch checkpoint.
- Main `load_tpage_buf_lock`: migrated the table access to `GameState.unk_1228` and used K&R narrow arg2; real register/scheduling differences remain. Reverted after 30-second permuter; same K&R KeyError issue.

- TT/func_8013A610 (256 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TT/func_8013A710 (256 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TACO/func_801546FC (292 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TT/func_80138B90 (384 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- EN_NICHI/func_801354E0 (140 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- main/search_load_tpage_buf_lock (232 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TACO/func_80154820 (320 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- DATE/func_801511B4 (332 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_8013DAC4 (268 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_80134768 (268 bytes): time-boxed. Missing draft declarations need manual recovery.

- main/func_80047560 (352 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- DATE/func_80155004 (368 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_801349B0 (300 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- NAME_ENT/func_801398EC (492 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- main/func_80048DD0 (168 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- EN_NICHI/func_801330D0 (736 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_80133C1C (248 bytes): time-boxed. Missing draft declarations need manual recovery.

- SHUGAKU/func_80135480 (340 bytes): time-boxed. Missing draft declarations need manual recovery.

- EN_NICHI/func_80132A74 (104 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TT/func_80133AD0 (108 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TT/func_80134ADC (224 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TT/func_80134874 (176 bytes): time-boxed. Missing draft declarations need manual recovery.

- main/load_tpage_buf_lock (168 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- DATE/func_80149268 (168 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- TT/func_80134D0C (276 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- TT/func_80138B90 (384 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- DATE/func_801511B4 (332 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- DATE/func_80155004 (368 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_801349B0 (300 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- main/func_80048DD0 (168 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- TT/func_80133C1C (248 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_80140D6C (172 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- ETC/func_80146400 (212 bytes): time-boxed. Missing draft declarations need manual recovery.

- ENDING/func_80132000 (820 bytes): matched. funcdiff --resolve MATCH; unit ninja SHA1 check passes.

- main/func_80042CB0 (864 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_80134630 (312 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- main/func_80042AC8 (268 bytes): time-boxed. Missing draft declarations need manual recovery.

- EN_NICHI/func_801337E0 (268 bytes): time-boxed. Missing draft declarations need manual recovery.

- BUNKAKEN/func_80145550 (756 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TT/func_80134E20 (808 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- NAME_ENT/func_8013F0DC (300 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_8013FB8C (304 bytes): time-boxed. Missing draft declarations need manual recovery.

- EN_NICHI/func_8013275C (216 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TT/func_801374C0 (404 bytes): time-boxed. Missing draft declarations need manual recovery.

- SHOUGATU/func_80140AD8 (256 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- DATE/func_801560E0 (712 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TT/func_80134BBC (336 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- SHOUGATU/func_8013A2F8 (260 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TT/func_80134178 (340 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- main/func_800422C8 (268 bytes): time-boxed. Missing draft declarations need manual recovery.

- main/search_tpage (616 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- main/search_tpage8 (616 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- main/func_80057710 (484 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TT/func_80133D14 (284 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- SHOUGATU/func_80140314 (508 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- NAME_ENT/func_8013D6C0 (308 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- main/func_80046754 (884 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- KANGEI/func_80133A54 (444 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TT/func_8013B4A0 (552 bytes): time-boxed. Missing draft declarations need manual recovery.

- GEKO/func_8013B3C0 (336 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- NAME_ENT/func_8013A9C8 (732 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- NAME_ENT/func_8013BE18 (348 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- NAME_ENT/func_8013F454 (348 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_8013D4CC (352 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_8013B1C4 (600 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_8013E430 (364 bytes): time-boxed. Missing draft declarations need manual recovery.

- SHUGAKU/func_80135630 (380 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TT/func_80133970 (240 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_8013CEC8 (396 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_801425C0 (704 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_80137654 (548 bytes): time-boxed. Missing draft declarations need manual recovery.

- DATE `func_801511B4`: existing FnTbl40 copy and leading scalar reproduce frame/table exactly. Only the `D_800B593C` address register differs (`lui v1; lbu v0` versus `lui v0; lbu v0`). Literal-byte view worsens other temporary allocation; local byte pointer gives the same one-register miss. Reverted after the 15-second permuter and two targeted variants.

- EN_NICHI/func_80135ACC (1160 bytes): time-boxed. Missing draft declarations need manual recovery.

- EN_NICHI/func_80134D74 (440 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TT/func_801342CC (420 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- EN_NICHI/func_80135274 (620 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- EN_NICHI/func_80132834 (472 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_8013BB28 (488 bytes): time-boxed. Missing draft declarations need manual recovery.

- RPG_BAT/func_801447E0 (924 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TT/func_80137878 (512 bytes): time-boxed. Missing draft declarations need manual recovery.

- DATE/func_80150C78 (548 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- SHUGAKU/func_801357AC (396 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- MASTER/func_8013B448 (956 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- EN_NICHI/func_80135600 (976 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TT/func_8013B6C8 (608 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- ENDING `func_80132000` (820 bytes) matched from the assignment-chain draft; missing overlay-local s32 declarations appended to `include/ovl/ENDING.h`. Resolved function diff and ENDING unit SHA1 both pass.

- EN_NICHI/func_80134800 (1052 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- NAME_ENT/func_80140574 (280 bytes): time-boxed. Missing draft declarations need manual recovery.

- OMIMAI/func_80132670 (280 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- EN_NICHI/func_80132EE8 (488 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TT/func_80137A78 (424 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_8013B928 (424 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- NAME_ENT/func_80139740 (428 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_8013B994 (380 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- ENDING/func_80132B04 (660 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- EN_NICHI/func_801344F0 (660 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- main/func_8004F6B0 (448 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_80134470 (448 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- TT/func_80133FB0 (456 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- NAME_ENT/func_80142880 (316 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_80140430 (324 bytes): time-boxed. Missing draft declarations need manual recovery.

- SHUGAKU/func_801360B0 (720 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_8013ACA4 (724 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_8013FE40 (344 bytes): time-boxed. Missing draft declarations need manual recovery.

- SHUGAKU/func_80135EA4 (524 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- NAME_ENT/func_8013F984 (388 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TT/func_8013AE3C (676 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- TT/func_8013A3E0 (560 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_801392A0 (1060 bytes): time-boxed. Missing draft declarations need manual recovery.

- GYOZI/func_8013ED84 (3700 bytes): matched. funcdiff --resolve MATCH; unit ninja SHA1 check passes.

- EN_NICHI/func_80134258 (664 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- TT/func_80138808 (904 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- ETC/func_801467C4 (312 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- EN_NICHI/func_8013358C (596 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- KANGEI/func_801335F0 (324 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- GYOZI `func_8013ED84` (3,700 bytes) matched from the scene-data initializer draft. Missing overlay-local declarations appended to `include/ovl/GYOZI.h`. Resolved function diff and GYOZI unit SHA1 pass.
- DATE `func_801511B4`: ordinary assignment-chain retry also retains the address-register miss; final draft reverted.

- TACO/func_8015443C (704 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- NAME_ENT/func_8013DD94 (500 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- EVENT/func_800F6E84 (376 bytes): time-boxed. Missing draft declarations need manual recovery.

- ETC/func_80148BC8 (720 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TT/func_80133E30 (384 bytes): time-boxed. Missing draft declarations need manual recovery.

- DATE/func_80150460 (340 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- NAME_ENT/func_8013C9F0 (340 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- EN_NICHI/func_80135FBC (744 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_8013B77C (536 bytes): time-boxed. Missing draft declarations need manual recovery.

- EN_NICHI/func_801339D4 (724 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TT/func_80137C20 (1368 bytes): time-boxed. Missing draft declarations need manual recovery.

- TT/func_80138E28 (1044 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_8013F758 (556 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- EVENT/func_801002C8 (384 bytes): time-boxed. Missing draft declarations need manual recovery.

- ETC/func_80148F7C (836 bytes): time-boxed. Missing draft declarations need manual recovery.

- main/func_80042134 (404 bytes): time-boxed. Missing draft declarations need manual recovery.

- RPG_BAT/func_80144C84 (1276 bytes): time-boxed. Missing draft declarations need manual recovery.

- EN_NICHI/func_80133CA8 (600 bytes): time-boxed. Missing draft declarations need manual recovery.

- RPG_BAT/func_80148714 (1012 bytes): time-boxed. Missing draft declarations need manual recovery.

- NAME_ENT/func_8013BB24 (756 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TT/func_80139770 (1064 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- NAME_ENT/func_8013D2AC (544 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- SHOUGATU/func_8013FB28 (1152 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TT/func_80138300 (1288 bytes): time-boxed. Missing draft declarations need manual recovery.

- SHOUGATU/func_8013F494 (1328 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- NAME_ENT/func_8013BF74 (1332 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- RPG_BAT/func_801484E0 (372 bytes): time-boxed. Missing draft declarations need manual recovery.

- RENSYU/func_80133990 (808 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- NAME_ENT/func_8014231C (580 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- EN_NICHI/func_80132450 (780 bytes): time-boxed. Missing draft declarations need manual recovery.

- RENSYU/func_80133628 (872 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- main/func_80079070 (1204 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- TEL/func_8013CCD0 (876 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- RPG_BAT/func_80148B08 (1128 bytes): time-boxed. Missing draft declarations need manual recovery.

- DATE/func_801496C4 (1020 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- ETC/func_8014790C (1128 bytes): time-boxed. Draft requires unresolved types/aggregate mapping.

- EN_NICHI/func_80134D74 (440 bytes): matched. funcdiff --resolve MATCH; unit ninja SHA1 check passes.

- NAME_ENT/func_8013D6C0 (308 bytes): matched. funcdiff --resolve MATCH; unit ninja SHA1 check passes.

- TT/func_80134630 (312 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TT/func_80134178 (340 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

- TT/func_801342CC (420 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- TT/func_80134470 (448 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- TT/func_80133FB0 (456 bytes): time-boxed. Draft reverted; compile/type/frame failure or timeout.

- TT/func_8013AE3C (676 bytes): time-boxed. Draft reverted; instruction/register/control-flow difference.

## Final results and review

Five retained matches, 5,360 bytes:

| Unit / object | Function | Bytes |
|---|---|---:|
| DATE/80154260 | `func_801551D0` | 92 |
| ENDING/80132000 | `func_80132000` | 820 |
| GYOZI/8013EB10 | `func_8013ED84` | 3,700 |
| NAME_ENT/80139740 | `func_8013D6C0` | 308 |
| EN_NICHI/80132000 | `func_80134D74` | 440 |

All 158 listed functions covered; 153 time-boxed and restored. Selected retries corrected K&R narrow definitions, recovered existing FnTbl types, byte-buffer field accesses, and NULL constants. No unverified C or new NON_MATCHING drafts retained. Exact-path dupes pass applied zero copies.

Six T-0018 rows appended. Main non-T-0018 near misses: EN_NICHI `func_80134F2C` matches instructions but frame is 0x38 versus original 0x30; TT `func_80134178` has one 0x40-load scheduling difference after NULL repair, unchanged by six bounded source-order variants. Other remaining drafts need pointer/aggregate/type recovery. Permuter K&R mutation produced argument `KeyError` failures (`arg1`, `arg2`); no tooling changes.

Inline review against CODING_STANDARDS.md and the shared brief (HEAD working diff): C89, fixed-width types, existing EnObj/GameState member access, original address order, one shared declaration per symbol, append-only overlay header additions, no SDK redefinitions, no raw/generated assets or tool/config edits. `type_recovery.py` found no aggregate proposals for initializer bases `D_8014826C` and `D_8013C280`. Retained source has no new fakematches. Standards findings: zero. Spec findings: zero. Final build verification passed.

Final five resolved function checks all MATCH. The concurrent full build encountered a truncated EN_NICHI object while funcdiff was rebuilding it; rerunning ninja sequentially after funcdiff completed. Future final validation must avoid simultaneous writers to the same object.

Sequential incremental ninja completed successfully; all 27 unit checksum markers are current, with `build/headers.ok` and `build/globals.ok` present. `sync_protos.py --check-branch`, `migrate_globals.py --check`, and `git diff --check` passed. `ninja progress`: `grand total   4739/6958   734972/2279368  32.2%`. Ticket, index, and board synchronized to Done. Changes left uncommitted for the orchestrator.
