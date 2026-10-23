---
type: data
updated: 2026-10-20
---

# Remaining functions (2026-10-20)

Generated from the integration tree at 6,825/6,952 (92.2%). Classes are defined in [[plan-remaining]]. `rows` comes from the NON_MATCHING note; `score` is the best draft's funcdiff score (lower is closer; drafts under `drafts/`).

## A - near miss, guarded, 10 rows or fewer (8 functions, 5232 bytes)

| function | file | bytes | rows | draft score | note |
|---|---|---|---|---|---|
| KANGEI:func_8013852C | `src/ovl/KANGEI/80137B90.c` | 180 | 2 | 36 | T-24202, T-24900, 2 rows (rows 35..36): as1 schedules lui $at, %hi(D_80139ADC) before addu $t6, $t4, $v1 instead of after (a local, same-block choice: the T-249 |
| KANGEI:func_80134724 | `src/ovl/KANGEI/80134120.c` | 716 | 2 | 150 | T-24201, 2 rows (1 pair); frame, registers ($v0=v, $v1=s, $a0=unk_66C, * $a1=unk_0F4>>12, $a2=c2) and all switch cases match (T-24122 draft). Only the IDO 5.3 a |
| VALEN:func_80132D0C | `src/ovl/VALEN/80132760.c` | 936 | 3 | 218 | T-24900 (was T-24212), 3 rows (7 resolved rows in funcdiff); only the dispatch `sll t3,a1,2` is in the wrong delay slot. as1's global scheduler visits the `== 9 |
| GEKO:func_80136C20 | `src/ovl/GEKO/80134D10.c` | 632 | 4 | 4 | T-24203, T-24900, 4 rows. as1 trace (wiki/as1-scheduler-rule.md): the D_80145088 test block (bb 8, weight 1) cannot take the selector lui because the switch hea |
| TACO:func_8013B460 | `src/ovl/TACO/8013B170.c` | 1328 | 4 | 632 | T-24200, 4 differing lines (2 rows, one per quad): the p->y3 halfword store is scheduled before the eight * UV byte stores; the original computes y3 first but s |
| TACO:func_801511C0 | `src/ovl/TACO/80149F90.c` | 164 | 5 | 10 | T-24205, 5 rows (rows 9-13), one instruction: only the sw a1,0x3C(sp) home store is late. The original stores arg1 after sw ra, before addu s2, or s0, or s1, li |
| GYOZI:func_80140E0C | `src/ovl/GYOZI/80140C90.c` | 388 | 5 | 135 | T-24301, 5 rows; the original keeps 0 in $a1 from entry (`or $a1,$zero,$zero; sb $a1,D_800F62CF; * or $a0,$a1,$zero`) and has a 0x28 frame (0x30 here). Zero loc |
| GYOZI:func_8013907C | `src/ovl/GYOZI/80138B50.c` | 888 | 7 | 207 | 7 resolved rows (same jump-table dispatch shift in branch delay slot as VALEN func_80132D0C, T-24033). T-24116. |

## B - guarded, more than 10 rows (18 functions, 23176 bytes)

| function | file | bytes | rows | draft score | note |
|---|---|---|---|---|---|
| main:func_8006C988 | `src/main/800674B0.c` | 276 | 11 | 131 | T-24301, 11 rows; the record pointer and the unk_00 value swap $t0/$t1, and the last-value * copy is `andi v1,t1,0xff` in both arms where the original has one ` |
| ENDING:func_8013A2E8 | `src/ovl/ENDING/801396E0.c` | 4772 | 12 | 1667 | T-23002, 12 differing lines (6 rows): the original fills the load-delay slot after `lbu a0` of the first flag test with the `lui a1` of the first string (arm 1  |
| main:func_80047560 | `src/main/80047550.c` | 352 | 22 | 105 | T-24303, 22 rows (4 instructions out of place, ugen numbers the first (s16)q chain t0-t2 before the off narrowing, the original t1-t3 after it; putting the unk_ |
| TACO:func_80138B2C | `src/ovl/TACO/80138890.c` | 476 | 30 | 103 | T-19015 (T-16564), 30 differing instruction rows (60 funcdiff lines), registers only: the original colours the u8 count D_8015EDCC $a0, the second local $a1 and |
| GEKO:func_80132B24 | `src/ovl/GEKO/801323E0.c` | 480 | 30 | 160 | T-24302, about 30 rows (61 funcdiff lines); frame and both * table copies match (scalars declared before the tables, only date, girl and * month as scalars). Le |
| NAME_ENT:func_8013AF78 | `src/ovl/NAME_ENT/80139740.c` | 296 | 99 | 111 | T-24132, ~99 resolved rows. Frame is exact (0x30, pad above the 3-word sel) and the * exit is s32 with a bare fallthrough (void loses five instructions); arg0 = |
| NAME_ENT:func_80138C1C | `src/ovl/NAME_ENT/80132000.c` | 740 | 200 | 248 | T-24132, ~200 resolved rows. Structure follows the asm: two clear loops over the * GameState text buffers, three name-copy loops that skip space pairs and zero  |
| NAME_ENT:func_801398EC | `src/ovl/NAME_ENT/80139740.c` | 492 | 260 | 253 | T-24132, ~260 resolved rows, all register allocation. Shape follows the asm and the * matched sibling func_80139F30: tmp is D_8014D0D4 % 3, the 13 NameRecord fl |
| GEKO:func_80133024 | `src/ovl/GEKO/801323E0.c` | 280 |  | 46 | T-24503, two rows: the day-address base is a1 rather than a2. * FAKE: empty day/month tests assign the day and table value to a2/v1; * the table-element test in |
| BUNKA_SD:func_80139290 | `src/ovl/BUNKA_SD/801388D0.c` | 1604 |  | 133 | C draft for T-24137. |
| BUNKA_SD:func_80134D94 | `src/ovl/BUNKA_SD/80134540.c` | 972 |  | 137 | C draft for T-24137. |
| SHOUGATU:func_8013E50C | `src/ovl/SHOUGATU/8013E330.c` | 472 |  | 167 | C draft for T-24137 (twin of GYOZI func_80140E0C). |
| main:func_80054E6C | `src/main/80053650.c` | 340 |  | 179 | T-24123, score=84 (CSE spill of &D_800E7D10+slot and &D_800E7D10+slot*8 at sp+0x34 and sp+0x30). |
| main:func_80075C84 | `src/main/80075320.c` | 796 |  | 270 |  |
| GYOZI:func_80138C9C | `src/ovl/GYOZI/80138B50.c` | 992 |  | 302 | CharFlags b1/b8 and s16 r before sp4C[13] match the 0x68 frame, sp+0x4C offset, and bit-field accesses; remaining diffs are $s3/$s4 swap (sp4C vs 1) and uopt zc |
| TACO:func_8014AB50 | `src/ovl/TACO/80149F90.c` | 1876 |  | 374 | T-24502, seven differing instruction words (28 bytes), all * constant-1 register choices (v0 here, v1 in the original). Regdiff: hoisted * constant bit 161 has  |
| ENDING:func_80135F4C | `src/ovl/ENDING/80134D20.c` | 3268 |  | 813 | T-24047, default-arm address * speculation; 804/805 instructions, original 160-byte frame. |
| main:func_8006D7EC | `src/main/8006CB30.c` | 4692 |  | 1912 | T-24204, 1 row; the first idx++ (right after idx = 0) is folded by uopt to li v1,1 where the original keeps addiu v1,v1,1 (draft T-23003; T-24001, T-24204: no s |

## C - not guarded, good draft (score 150 or less) (11 functions, 8372 bytes)

| function | file | bytes | rows | draft score | note |
|---|---|---|---|---|---|
| GEKO:func_801367EC | `src/ovl/GEKO/80134D10.c` | 872 |  | 44 |  |
| main:sndisp | `src/main/8004F870.c` | 388 |  | 98 |  |
| main:func_8006BA40 | `src/main/800674B0.c` | 488 |  | 98 |  |
| RPG_BAT:func_80154DD8 | `src/ovl/RPG_BAT/80154430.c` | 1204 |  | 113 |  |
| ENDING:func_801330E4 | `src/ovl/ENDING/80133030.c` | 796 |  | 129 |  |
| ETC:func_8014790C | `src/ovl/ETC/80146400.c` | 1128 |  | 129 |  |
| main:func_8005B43C | `src/main/8005A0B0.c` | 920 |  | 131 |  |
| OPTION:func_801326BC | `src/ovl/OPTION/80132000.c` | 516 |  | 134 |  |
| ETC:func_8013CCE4 | `src/ovl/ETC/8013BEF0.c` | 584 |  | 137 |  |
| main:strCallback | `src/main/800563F0.c` | 640 |  | 139 |  |
| GEKO:func_80133C50 | `src/ovl/GEKO/801338F0.c` | 836 |  | 149 |  |

## D - large or poor draft (reconstruction) (90 functions, 144040 bytes)

| function | file | bytes | rows | draft score | note |
|---|---|---|---|---|---|
| ENDING:func_801387B0 | `src/ovl/ENDING/80137340.c` | 748 |  | 156 |  |
| main:func_80065964 | `src/main/80062CD0.c` | 424 |  | 160 |  |
| main:func_80043510 | `src/main/80043510.c` | 972 |  | 168 |  |
| main:parameter_up_down | `src/main/800737A0.c` | 1228 |  | 177 |  |
| ETC:func_80146D4C | `src/ovl/ETC/80146400.c` | 1076 |  | 180 |  |
| ETC:func_80144224 | `src/ovl/ETC/80143240.c` | 796 |  | 200 |  |
| main:col2sepia | `src/main/8004AD60.c` | 580 |  | 202 |  |
| main:func_8006A044 | `src/main/800674B0.c` | 700 |  | 207 |  |
| main:srn_set | `src/main/8004C3F0.c` | 596 |  | 210 |  |
| main:func_8004443C | `src/main/80043510.c` | 708 |  | 212 |  |
| main:mod_set_gpu | `src/main/8004C3F0.c` | 828 |  | 214 |  |
| main:func_8004A1A0 | `src/main/80049FF0.c` | 628 |  | 226 |  |
| main:func_8005938C | `src/main/80058D20.c` | 764 |  | 226 |  |
| RPG_BAT:func_8014BFE4 | `src/ovl/RPG_BAT/8014ADD0.c` | 892 |  | 229 |  |
| main:func_80054FC0 | `src/main/80053650.c` | 796 |  | 235 |  |
| TAIIKU:func_80139AB0 | `src/ovl/TAIIKU/80138950.c` | 812 |  | 237 |  |
| ETC:func_8013F054 | `src/ovl/ETC/8013BEF0.c` | 716 |  | 238 |  |
| ETC:func_80132C3C | `src/ovl/ETC/80132880.c` | 884 |  | 241 |  |
| main:mod_trans_vram | `src/main/8004C3F0.c` | 776 |  | 243 |  |
| TAIIKU:func_80137C44 | `src/ovl/TAIIKU/80133C80.c` | 612 |  | 245 |  |
| main:func_80056BA8 | `src/main/800563F0.c` | 1296 |  | 252 |  |
| main:mod_set | `src/main/8004C3F0.c` | 804 |  | 254 |  |
| main:func_80062948 | `src/main/80061710.c` | 736 |  | 255 |  |
| main:func_80061A3C | `src/main/80061710.c` | 1216 |  | 264 |  |
| KANGEI:func_801327DC | `src/ovl/KANGEI/80132090.c` | 1164 |  | 266 |  |
| VALEN:func_80133088 | `src/ovl/VALEN/80132760.c` | 1108 |  | 292 |  |
| main:func_80067870 | `src/main/800674B0.c` | 952 |  | 304 |  |
| ETC:func_801328A4 | `src/ovl/ETC/80132880.c` | 920 |  | 313 |  |
| main:func_80065B0C | `src/main/80062CD0.c` | 1064 |  | 313 |  |
| main:sndi | `src/main/8004F870.c` | 1052 |  | 314 |  |
| GYOZI:func_801412A4 | `src/ovl/GYOZI/80140C90.c` | 684 |  | 317 |  |
| SHOUGATU:func_8013E6E4 | `src/ovl/SHOUGATU/8013E330.c` | 948 |  | 317 |  |
| main:func_8005215C | `src/main/8004F870.c` | 1260 |  | 318 |  |
| SHOUGATU:func_8013EA98 | `src/ovl/SHOUGATU/8013E330.c` | 920 |  | 334 |  |
| main:func_8006D138 | `src/main/8006CB30.c` | 920 |  | 338 |  |
| main:func_80076FDC | `src/main/80075320.c` | 940 |  | 351 |  |
| RPG_BAT:func_80144C84 | `src/ovl/RPG_BAT/801447E0.c` | 1276 |  | 360 |  |
| TAIIKU:func_8013E894 | `src/ovl/TAIIKU/80138950.c` | 972 |  | 366 |  |
| main:func_80073CB0 | `src/main/800737A0.c` | 1440 |  | 380 |  |
| main:func_8006509C | `src/main/80062CD0.c` | 1076 |  | 382 |  |
| TACO:func_8013FBF0 | `src/ovl/TACO/8013F250.c` | 1040 |  | 394 |  |
| main:func_8004A414 | `src/main/80049FF0.c` | 1024 |  | 396 |  |
| MASTER:func_801326B4 | `src/ovl/MASTER/80132140.c` | 3376 |  | 397 |  |
| main:_sprite_set_light_effect1 | `src/main/800490C0.c` | 1124 |  | 403 |  |
| TAIIKU:func_8013DDB8 | `src/ovl/TAIIKU/80138950.c` | 1120 |  | 412 |  |
| TACO:func_8013DEE4 | `src/ovl/TACO/8013DB80.c` | 1416 |  | 412 |  |
| TAIIKU:func_80137784 | `src/ovl/TAIIKU/80133C80.c` | 1216 |  | 448 |  |
| ENDING:func_80134270 | `src/ovl/ENDING/80133C10.c` | 1900 |  | 468 |  |
| MASTER:func_801344E0 | `src/ovl/MASTER/80133680.c` | 2980 |  | 475 |  |
| MASTER:func_801363F4 | `src/ovl/MASTER/80133680.c` | 2656 |  | 479 |  |
| RPG_BAT:func_8015528C | `src/ovl/RPG_BAT/80154430.c` | 1500 |  | 498 |  |
| EN_NICHI:func_80136DEC | `src/ovl/EN_NICHI/80136B10.c` | 1412 |  | 502 |  |
| GEKO:func_80135DC0 | `src/ovl/GEKO/80134D10.c` | 2304 |  | 508 |  |
| VALEN:func_80132834 | `src/ovl/VALEN/80132760.c` | 1240 |  | 509 |  |
| ETC:func_8013FFAC | `src/ovl/ETC/8013FF80.c` | 2176 |  | 514 |  |
| TAIIKU:func_8013F3A4 | `src/ovl/TAIIKU/80138950.c` | 1472 |  | 519 |  |
| main:func_8005557C | `src/main/80053650.c` | 1212 |  | 523 |  |
| TACO:func_8013C6D4 | `src/ovl/TACO/8013B990.c` | 1484 |  | 535 |  |
| TAIIKU:func_80135F3C | `src/ovl/TAIIKU/80133C80.c` | 1676 |  | 535 |  |
| main:func_80055AFC | `src/main/80053650.c` | 1396 |  | 555 |  |
| TACO:func_80139FB8 | `src/ovl/TACO/80139170.c` | 1480 |  | 558 |  |
| main:func_800455C4 | `src/main/800451D0.c` | 2992 |  | 579 |  |
| EN_NICHI:func_8013735C | `src/ovl/EN_NICHI/80136B10.c` | 2368 |  | 584 |  |
| main:func_80049A40 | `src/main/800490C0.c` | 1484 |  | 589 |  |
| main:func_80052F68 | `src/main/8004F870.c` | 1200 |  | 604 |  |
| TACO:func_80139424 | `src/ovl/TACO/80139170.c` | 1676 |  | 606 |  |
| TACO:func_8013EAA4 | `src/ovl/TACO/8013DB80.c` | 1964 |  | 626 |  |
| main:make_color_bar | `src/main/8004AD60.c` | 1072 |  | 665 |  |
| GYOZI:func_801368B8 | `src/ovl/GYOZI/80136220.c` | 2296 |  | 701 |  |
| TACO:func_80145650 | `src/ovl/TACO/801447D0.c` | 1908 |  | 709 |  |
| main:get_h_tokimeki | `src/main/8004F870.c` | 1624 |  | 742 |  |
| TAIIKU:func_8013E1FC | `src/ovl/TAIIKU/80138950.c` | 1688 |  | 746 |  |
| main:func_8004FC10 | `src/main/8004F870.c` | 1812 |  | 752 |  |
| TAIIKU:func_8013EC60 | `src/ovl/TAIIKU/80138950.c` | 1788 |  | 784 |  |
| ETC:func_8013D570 | `src/ovl/ETC/8013BEF0.c` | 2352 |  | 792 |  |
| TACO:func_8013BAC4 | `src/ovl/TACO/8013B990.c` | 1840 |  | 813 |  |
| SHOUGATU:func_801350F8 | `src/ovl/SHOUGATU/80134890.c` | 2348 |  | 995 |  |
| main:func_80067F04 | `src/main/800674B0.c` | 2452 |  | 1019 |  |
| TEL:func_8013D88C | `src/ovl/TEL/8013CCD0.c` | 3044 |  | 1045 |  |
| main:func_80046AC8 | `src/main/80046500.c` | 2696 |  | 1048 |  |
| main:func_8006B0C8 | `src/main/800674B0.c` | 1268 |  | 1056 |  |
| NAME_ENT:func_80143580 | `src/ovl/NAME_ENT/80142A80.c` | 4388 |  | 1067 |  |
| GEKO:func_80142C9C | `src/ovl/GEKO/80142760.c` | 3700 |  | 1115 |  |
| main:func_800476C0 | `src/main/80047550.c` | 3396 |  | 1132 |  |
| TEL:func_80136724 | `src/ovl/TEL/80132000.c` | 7044 |  | 1162 |  |
| ETC:func_801480D0 | `src/ovl/ETC/80146400.c` | 2808 |  | 1200 |  |
| main:srn_tpage_show | `src/main/8004C3F0.c` | 3224 |  | 1311 |  |
| main:palette_load_vram | `src/main/80043510.c` | 2044 |  | 1384 |  |
| main:get_near_menu | `src/main/8004F870.c` | 1784 |  | 1432 |  |
| main:func_8007132C | `src/main/8006CB30.c` | 4292 |  | 1440 |  |
