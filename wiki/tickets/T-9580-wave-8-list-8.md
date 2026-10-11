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

## Deep second pass

Reopened at user request after the first pass was committed. Start: 2026-10-11 09:26 UTC. Work for at least three hours or until the list is exhausted: rank resolved draft differences, iterate approximately 10–15 minutes per candidate, run five-minute permuter searches for near misses, and recover missing aggregate/local-copy types from neighboring matches. Preserve only verified matches; revert failures. Second-pass review and final checks pending.

Resolved-diff ranking from saved first-pass and retry logs: TT `func_80134178` (2 differing lines), DATE `func_801511B4` (4), EN_NICHI `func_80132A74` (20), NAME_ENT `func_80140D6C` (24), DATE `func_80150460` (28), GYOZI `func_80136124` (29), DATE `func_80150C78` and EN_NICHI `func_801344F0` (32 each). Already committed matches excluded. Ranking archive/JSON and experiment scripts are in the batch's scratchpad. TT loop/constant/layout experiments running first; aggregate/local-copy recovery follows the nearest candidates.

- Deep TT `func_80134178`: 38 loop/layout/constant/counter variants plus five ANSI/0x68-actor type variants. Best remains the single 0x40-load scheduling difference (two diff lines). A 300-second K&R search hit argument KeyErrors; an ANSI seed reproduced the same real diff and completed another 300-second search without improvement. Restored to INCLUDE_ASM after roughly 20 minutes.

- Second-pass match: func_8013F984: recovered byte buffer and implicit-int dispatcher; s32-buf-0x108. Resolved MATCH and overlay SHA1 pass; any enlarged buffer marked FAKE.

- Deep DATE `func_801511B4`: value/pointer/view/constant-chain and dead-test variants, followed by a 300-second search. Both score-30 candidates still have the same four-line resolved address-register difference. Reverted after approximately 15 minutes. No candidate retained.

- Second-pass match: func_8013DD94: recovered byte buffer and implicit-int dispatcher; s32-buf-0x108. Resolved MATCH and overlay SHA1 pass; any enlarged buffer marked FAKE.

- Second-pass match: func_8013F758: recovered byte buffer and implicit-int dispatcher; s32-buf-0x108. Resolved MATCH and overlay SHA1 pass; any enlarged buffer marked FAKE.

- Deep SHOUGATU `func_8013A2F8`: type-aware recovery, 15 flag/call-expression variants, five-minute permuter search, and approximately 30 statement-layout/dead-test variants. The shared CharFlags bit-field store plus element assignment argument reduces 13 diff lines to 2 (selector `li`/`lui` order). No verified match; restored after approximately 15 minutes.
- NAME_ENT pointer recovery: target DD94 loads a pointer at D_800B32CC before indexing -4, so its missing view is `u8 **`, not a static table. Added that pointer and the byte slot D_800E7D36 through sync_protos --add-global for the repaired retries.

- Deep-pass match: main `search_load_tpage_buf_lock` (232 bytes). Recovered byte-stepping GameState cursor (explicit FAKE view), removed redundant saved-return local, and used `(u8)var_v1 & 0x1F` at the call. Resolved MATCH and `build/SLPM_86.053.bin: OK`; no ABI override retained. Second-pass subtotal: four functions, 1,676 bytes.
- NAME_ENT `func_8013AF78`: recovered real three-word local array, tested 24 layout/loop variants, 12 return/raw-load variants and a further bounded 300-second raw/masked-value search. Best still differs in live raw/masked byte registers, indexed local-address construction and frame. Function restored; no unverified C retained.

- Deep-pass match: EN_NICHI `func_801344F0` (660 bytes), resolved MATCH and overlay SHA1. `EnObj` fixes signed halfword stores; direct `unk_1C` wrap fixes the tail; write `unk_16` before `unk_1A` in cases 0/2 to allocate remainder/subtraction temporaries before the constant. A 300-second permuter on the prior 20-line draft did not improve it; the subsequent store-order experiment matched without a trick. Second-pass subtotal: five functions, 2,336 bytes.

- Deep-pass match: NAME_ENT `func_80140D6C` (172 bytes), resolved MATCH and overlay SHA1. A 300-second permuter produced a candidate that checked at two differing lines in the real source; joining pointer initialization with `do` fixed the remaining load order. The biased cursor, alternate end-address expression and line layout are marked FAKE. This closes the previously guarded T-8050 draft. Second-pass subtotal: six functions, 2,508 bytes.

- Deep-pass match: KANGEI `func_801335F0` (324 bytes), resolved MATCH and overlay SHA1. Recovered the real packed bits 25–26 at GameState offset 0x66C, changed the 15-trip counter to a for-loop, and tried 28 packed-index/view variants plus a 300-second permuter. Inline field index plus the wave-7 dead-byte-test idiom fixed the remaining record-address register. Dead test explicitly marked FAKE. Second-pass subtotal: seven functions, 2,832 bytes.

- Deep aggregate repairs: BUNKAKEN `func_80145550` recovered three aligned 54/66/66-byte union tables and a GameState member view; 27 declaration/counter/table variants still miss frame, fallback byte and live-index scheduling (best 129 lines). SHUGAKU `func_801357AC` recovered the 68-byte copy and signed record view; roughly 40 control/layout/type variants plus a 300-second permuter remain 20 lines away. DATE `func_80150C78` fixed color expression order, spill frame and counter forms; 38 valid variants plus a 300-second search remain 12 lines away. All three failed drafts and unused header additions restored.

- OMIMAI `func_80132670`: wide byte temporary and scalar-array view reduce the draft to six lines; the 300-second permuter's real candidate is four lines away. Sixteen argument/store/type variants and 19 dead-read/line-layout variants leave only the two-instruction address-register choice. All failed C/header additions restored after approximately 25 minutes. Header failures caused by a concurrent temporary main return-type experiment were excluded and the main draft was restored immediately.

- Deep-pass match: ETC `func_801467C4` (312 bytes), resolved MATCH and ETC SHA1. Recovered the packed flag-table byte read; the 14-trip for-loop removes two-counter copies. A 300-second search on the resulting two-line miss did not repair the real remaining issue: signed versus unsigned `unk_F6D`. Its unsigned byte view matches. Second-pass subtotal: eight functions, 3,144 bytes. Final array-bound/type review pending.

- ETC match type review: replaced the temporary cross-table `unk_66C[index + 10]` byte view with the actual `GsRec69C unk_69C[index - 2].unk_00` table. The target's 0x6914 base is two records before the typed 0x69C table. The corrected member expression still passes the resolved diff and ETC SHA1, avoiding the cross-table array access.

- SHOUGATU `func_8013F494`: missing overlay byte and 68-byte aggregate types repaired, parent-buffer copies used for destinations. Thirty-two buffer declaration/order/counter/combined-array variants plus a 300-second permuter remain 16 lines away: stack table addresses differ by eight bytes, destination address scheduling and two tail stores still differ. Failed function restored; trying the other functions that share its overlay next.

- Deep-pass match: SHOUGATU `func_80140AD8` (256 bytes), resolved MATCH and SHOUGATU SHA1. The original word-load/sign-bit test reads `CharFlags.b1`, and the byte update is the same bitfield's assignment; using the real bitfield on both sides plus s32 return type recovers all instructions. No new overlay declarations or FAKE idioms needed. Second-pass subtotal: nine functions, 3,400 bytes.

- TT `func_8013923C`: real actor fields (phase +0x52, counter +0x3AA, speed +0x3AC) and a held typed pointer reduce 39 to 14 lines. Thirty-six type/order/return/assignment-chain variants and a 300-second permuter still swap the narrow counter and speed registers and fold the original zero mask; restored failed C/type. GYOZI `func_80136124`: 12 unsigned-index/byte-pointer/integer-address/dead-read variants and type-recovery inspection preserve the repeated-multiply gap (best 29); restored. TACO `func_80154230`: fixed word-pointer scaling and u16 parameter promotion, then 28 parameter/modified-pointer/constant-local/layout variants; best 25 lines in early pointer promotion, constant operands and mask temporaries. Restored failed C.

- SHOUGATU `func_80140314`: recovered the 48-byte coordinate copy and 36-byte index copy; normalizing the year-minus-95 index and testing narrow year/index/pointer/declaration forms. Fourteen further variants compile only when the year is narrowed before multiplying; best 202 lines. Ordinary/normalized pointer forms hit `frame_pass.py: negative $sp offset` (-1052, -1056, -1080, -1048), despite the target's legitimate negative table-load displacement. Failed function and unused copy types/declarations restored; verified SHOUGATU flag match retained.

- Main `func_80047560`: corrected signed-halfword narrowing of rsin, tested 16 generated-prototype-safe return/wide-byte/narrow-result/explicit-conversion variants. Best remains 86 lines, with different promoted globals, duplicate conversion elimination and coordinate-store temporaries. Restored failed C and its original generated prototype; prior four void-only variants also failed.

- EN_NICHI `func_80132EE8`: recovered word flags for ten 0x30-byte actors and the missing viewport halfwords; eight compiling signed/unsigned/end-pointer variants, then 12 pointer-reuse/counter-member/atomic-increment variants. Atomic increment reduces the valid best from 139 to 83 lines, but broad pointer/counter register differences remain. Missing-declaration batches excluded. Failed function restored; its shared overlay initializer is the next candidate.

- NAME_ENT `func_8013BB24`: nine 40-byte option-record views repair the aggregate/local-copy draft. Reordered reads/stores and explicit word/byte views reduce the resolved difference to 14 lines, all seven shared-address-store scheduling pairs. More than 50 layout/counter/view variants, a 300-second permuter, and six assignment-in-call variants did not resolve it. Restored the failed function and its unused snapshot declarations.
- TT `func_80133AD0`: real packed phase flags and unsigned index view reduce the draft to 33 lines; 24 valid return/parameter/declaration variants preserve different pointer/counter allocation and flag-test scheduling. Restored the failed function; its unused view will be removed after the remaining TT experiment.
- EN_NICHI `func_8013275C`: recovered the ten-byte flag aggregate and word-clear pointer, then 12 valid call/field/chain/layout variants. Best remains 65 lines: the current one-argument overlay declaration loses the observed a1 setup, and the dynamic byte span unrolls differently. Explicit cast to the observed two-argument call generated an indirect call and was rejected. Failed function and unused EN declarations restored. No alternate alias or ABI override retained.
- Review cleanup: removed the stale T-8050 NON_MATCHING comment from the resolved NAME_ENT match, unused loop declarations in KANGEI/ETC, and trailing whitespace. Testing an equivalent if/else in the SHOUGATU s32 dispatcher to avoid its valueless return; all nine retained functions are being reverified before accepting the cleanup.

Review cleanup accepted: all nine resolved functions remain MATCH and all six affected unit SHA1 checks pass (`verify-deep.log`). SHOUGATU now uses an if/else with s32 fallthrough, preserving the implicit-int dispatcher shape without a valueless return. Main API generator cleanup leaves only two required additions: `u8 **D_800B32CC` and `u8 D_800E7D36`. No rejected aggregate types remain except the active TT experiment; final full-build and review gate pending.

- TT `func_80134630`: 30 promoted-parameter/declaration variants plus 64 struct/raw-pointer, constant-cast, return-type and mask-placement variants. Genuine 0x30-byte actor prefix recovered and tested; signed local initialization order improves 53 to 45 resolved lines, but saved argument registers, constant masking and tail-constant scheduling remain different. Reverted after approximately 11 minutes; unused TT aggregate and flag views removed. No failed TT C retained.

Final declaration guards pass: headers OK, globals OK, branch prototype check OK (zero new disagreements, 17 known baseline). Guard checks ran after removing unused declarations from failed drafts. Inline review must still examine the final restored source and any duplicate-tool copies before completion.

- Final EN_NICHI `func_80134F2C` retry: 20 declaration/signature/return variants (void-prototype conflicts excluded), then 12 ANSI storage-class/raw-pointer variants, plus two 300-second searches. Best remains four resolved lines: frame allocate/release is 0x38 versus original 0x30; all other instructions and +0x28 pointer spill match. ANSI avoids K&R argument KeyErrors, but no score-zero candidate. Restored after approximately 16 minutes. No unused padding or register declarations retained.
- Final ETC bounds review found that `unk_69C[index - 2]` can form a negative subarray index. Added an overlay-local prefix view at the actual GameState +0x694 base, spanning 34 four-byte records through +0x71C. The direct index preserves the original address and avoids crossing a declared array boundary. Resolved funcdiff still MATCH; final whole-unit verification pending. This supersedes the earlier type-review claim that the subtract-two expression was sufficient.

Inline review in progress against CODING_STANDARDS.md sections 4, 7, 7a and 8a plus its checklist. Source scope, C89, fixed-width types, typed GameState/EnObj fields, shared extern ownership, FAKE annotations and restoration of failed drafts checked. ETC subarray-boundary finding resolved with the new prefix view and a fresh resolved MATCH. Two now-unused EN_NICHI draft temporaries (`temp_t4`, `var_v0`) require removal if the matching output permits it. Exact-path duplicate pass applied zero copies; near-duplicate scan and full-build gate remain pending.

## Deep-pass final result

Active matching: 09:26–12:30 UTC (3 hours 4 minutes), followed by final verification and review. Nine additional matches, 3,400 bytes; combined with the committed first pass, 14 listed functions and 8,760 bytes.

| Unit | Function | Bytes |
|---|---|---:|
| NAME_ENT | `func_8013F984` | 388 |
| NAME_ENT | `func_8013DD94` | 500 |
| NAME_ENT | `func_8013F758` | 556 |
| NAME_ENT | `func_80140D6C` | 172 |
| main | `search_load_tpage_buf_lock` | 232 |
| EN_NICHI | `func_801344F0` | 660 |
| KANGEI | `func_801335F0` | 324 |
| ETC | `func_801467C4` | 312 |
| SHOUGATU | `func_80140AD8` | 256 |
| Total | | 3,400 |

Inline review findings resolved: ETC uses the complete 34-record +0x694 view with its own repeated-inclusion guard; both unused EN_NICHI temporaries were removed and the function remains a resolved MATCH. No failed drafts, unused recovered types, new ABI overrides, or unmarked allocation tricks remain. Original committed matches are preserved. Six retained C files, two append-only overlay type/declaration headers and generator-only main declarations are within the ticket's scope; no tools/config/raw/asm edits.

Reviewed against every CODING_STANDARDS.md checklist item: C89, fixed-width types and documented layouts, SDK headers unchanged, shared declarations centralized, original binary checks, guarded nonmatches, explicit FAKE comments, no debug/dead code, no data staging, Docker-only tools and wiki consistency. Code findings: zero unresolved. Shared-brief/spec findings: zero unresolved. No reviewer agent used. Changes remain uncommitted as requested.

Final sequential build after review cleanup: 27/27 SHA1 checks OK; exact/near duplicate scans applied zero copies. Branch prototype check passes with zero new disagreements (17 known baseline). Three T-0018 resolution rows appended for NAME_ENT/T-8050, SHOUGATU/T-9020 and the search half of T-9110. Main non-T-0018 blocker: EN_NICHI frame-only 0x38 versus 0x30 and larger copy/table layouts. Tool issues observed: K&R permuter argument KeyErrors (ANSI retries used), and frame-pass negative-SP rejection of legitimate biased year/table addressing. No tooling changes.

`ninja progress`: `grand total   4748/6958   738372/2279368  32.4%`.

Final direct guard checks after all review changes: headers OK and globals OK. Both guard marker files exist; `git diff --check` passes. All review findings and acceptance criteria resolved. Ticket ready for Done; no commits or staging performed.
