---
id: T-9520
title: "Wave 8: list 2"
status: Done
assignee: w8-2
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match the 142 assigned functions (82,476 bytes) in the 35 owned C files, following the shared wave-8 brief. Time-box unresolved functions and restore their INCLUDE_ASM.

## Acceptance criteria

- [x] Every listed function attempted or explicitly time-boxed with evidence.
- [x] Retained C verified with resolved diffs or direct original-byte proof and all 27 unit checksums.
- [x] Prototype/global checks and inline coding-standards review pass.

## Notes

Shared brief and list: scratchpad/wave8-brief.md and scratchpad/wave8-list-2.txt in the orchestrator scratch directory. No commits, pushes, tooling edits, or unowned C edits.

## Comments

2026-10-11: Read wiki index, shared brief, standards, local instructions, matching workflow and register-allocation findings. Initial worktree lacked build.ninja and assembly; Docker configure succeeded and ninja is generating the initial objects.

2026-10-11 tooling note: shared brief recommends register_shapes.py <func>, but its argparse accepts only --root/--rows/--json. Per-function positional invocation exits 2; use module shape() or filter inventory instead. No tooling edit made.

Draft caution: NAME_ENT func_801341A8 loads the old D_8014CCB4 before the flag test, then compares it with the replacement. m2c instead emits a comparison against the newly stored global; restore a local snapshot before trusting that draft.

Baseline Docker ninja -j 4 completed successfully: all 27 linked units match, build/headers.ok and build/globals.ok present.

Initial 25 drafts attempted. Four main state setters additionally tried s32 return-zero, return-assignment and no-return forms; main func_80042808 had a 120-second permuter run plus six scalar-view/line-layout probes. Return-zero drafts retain a final-store/jr swap; no-return omits the original v0 zero. No C or prototype changes retained from those probes. NAME_ENT func_80134110 tried s32/locals/implicit-return forms and func_801341A8 restored the old snapshot; register choices remain different. GEKO func_80140FD0 tried CharFlags.f9 and retained different load/CSE/register choices. Recovery Docker build passed 27/27.

Added temporary overlay scalar declarations for the draft checks, inferred from original loads and stores. Headers check passes; unused additions will be removed before review. TT func_80147074 is within a swap of saved constants 8 and 0x20; started a 120-second permuter run. TACO func_80141050/80141170 drafts were cleaned to real TcActor/Tc14 scaling and show frame/local allocation differences; testing the canonical four-local loop.

Bounded cleaned-draft checks now cover indices 25-64. TT func_80147074 named u32 limit puts 8 and 0x20 in the correct saved registers but hoists li s3,8 earlier; ten variants failed resolved diffs. No C retained. Later bodies often need undeclared tables, local copy-buffer types or byte-pointer field recovery; these remain time-boxed rather than assumed matches.

- Matched NAME_ENT `func_801330E8` (532 bytes), resolved funcdiff and incremental ninja.

- Matched NAME_ENT `func_801332FC` (532 bytes), resolved funcdiff and incremental ninja.

- Matched RPG_BAT `func_801481C8` (780 bytes), resolved funcdiff and incremental ninja.

- Matched TACO `func_801361E8` (592 bytes): full ninja and TACO sha1 pass; all 592 original instruction bytes directly equal linked binary. funcdiff reports only two false HI16 string differences.

- Matched TACO `func_80136438` (592 bytes): full ninja and TACO sha1 pass; all 592 original instruction bytes directly equal linked binary. funcdiff reports only two false HI16 string differences.

- Matched TACO `func_80136688` (592 bytes): full ninja and TACO sha1 pass; all 592 original instruction bytes directly equal linked binary. funcdiff reports only two false HI16 string differences.

## Assignment audit

All 142 m2c attempts are recorded below. Initial pass: 31 unresolved drafts, 19 aggregate-view blocks, 60 compiler errors, 29 resolved diffs, 3 matches. Subsequent retries retained the three TACO string cases using unit sha1 plus direct original-byte comparison. Failed candidates were restored; full drafts, diagnostics and bounded variant/permuter output remain in scratchpad/w8-2/.

| File | Function | Outcome | Evidence / initial stopping point |
|---|---|---|---|
| DATE/8014A1B0 | `func_8014AD9C` | Time-boxed | m2c unresolved local/table/argument type |
| main/80042540 | `func_80042808` | Time-boxed | no declaration and offset 0x1104 is ambiguous (GsWord .unk_1104, s32 .unk_1104.w, u32 .unk_1104.u, u16[] .unk_1104.h, u16 .unk_1104.h[0], u8[] .unk... |
| main/80042540 | `func_80042908` | Time-boxed | no declaration and offset 0x1104 is ambiguous (GsWord .unk_1104, s32 .unk_1104.w, u32 .unk_1104.u, u16[] .unk_1104.h, u16 .unk_1104.h[0], u8[] .unk... |
| NAME_ENT/80132000 | `func_80134110` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| NAME_ENT/80132000 | `func_801341A8` | Time-boxed | 'D_8014CCB4' undefined; reoccurrences will not be reported. |
| RPG_BAT/8013E4D0 | `func_8013F350` | Time-boxed | 'D_8015ED90' undefined; reoccurrences will not be reported. |
| KANGEI/801355F0 | `func_80135C78` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| main/80042540 | `func_8004284C` | Time-boxed | no declaration and offset 0x1104 is ambiguous (GsWord .unk_1104, s32 .unk_1104.w, u32 .unk_1104.u, u16[] .unk_1104.h, u16 .unk_1104.h[0], u8[] .unk... |
| main/800451D0 | `func_8004636C` | Time-boxed | Selector requires struct/union pointer as left hand side |
| GEKO/80140A00 | `func_80140FD0` | Time-boxed | no declaration and offset 0x430 is ambiguous (Rec38Flags .unk_1BC[11].unk_0C, s32 .unk_1BC[11].unk_0C.w, u32 .unk_1BC[11].unk_0C.u, u16[] .unk_1BC[... |
| TT/8013F040 | `func_8013F878` | Time-boxed | Selector requires struct/union pointer as left hand side |
| main/800451D0 | `func_800463E8` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| SHOUGATU/80134120 | `func_801341E4` | Time-boxed | no declaration and offset 0x2AA is ambiguous (u16 .unk_1BC[4].unk_0C.h[1], u8 .unk_1BC[4].unk_0C.b[2]): declare the old view in include/main_api.h ... |
| ETC/8013BEF0 | `func_8013E094` | Time-boxed | Selector requires struct/union pointer as left hand side |
| main/800451D0 | `func_80045318` | Time-boxed | Selector requires struct/union pointer as left hand side |
| RPG_BAT/801525B0 | `func_80153A60` | Time-boxed | Selector requires struct/union pointer as left hand side |
| TACO/8015ABF0 | `func_8015B1B8` | Time-boxed | m2c unresolved local/table/argument type |
| main/80042540 | `func_80042940` | Time-boxed | no declaration and offset 0x1104 is ambiguous (GsWord .unk_1104, s32 .unk_1104.w, u32 .unk_1104.u, u16[] .unk_1104.h, u16 .unk_1104.h[0], u8[] .unk... |
| SHOUGATU/80134120 | `func_80134730` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| TACO/80135720 | `func_801357B0` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| DATE/8015A640 | `func_8015A640` | Time-boxed | Hand-written rotation helper; retained INCLUDE_ASM |
| SHOUGATU/80134120 | `func_801347E0` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| NAME_ENT/80132000 | `func_80133D94` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| TACO/8015ABF0 | `func_8015C0A0` | Time-boxed | Selector requires struct/union pointer as left hand side |
| RPG_BAT/8013E4D0 | `func_8013E4D0` | Time-boxed | m2c unresolved local/table/argument type |
| TACO/80140DF0 | `func_80141050` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| TACO/80140DF0 | `func_80141170` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| TACO/80140DF0 | `func_80141290` | Time-boxed | member of structure or union required |
| NAME_ENT/80132000 | `func_80135A98` | Time-boxed | 'D_8014CD04' undefined; reoccurrences will not be reported. |
| TACO/8015ABF0 | `func_8015BF70` | Time-boxed | 'D_80161E60' undefined; reoccurrences will not be reported. |
| TT/80142D40 | `func_801436BC` | Time-boxed | Selector requires struct/union pointer as left hand side |
| TACO/8015ABF0 | `func_8015CCC8` | Time-boxed | member of structure or union required |
| SHOUGATU/80141780 | `func_801419D0` | Time-boxed | m2c unresolved local/table/argument type |
| GYOZI/8013A140 | `func_8013A4E8` | Time-boxed | 'D_801474B4' undefined; reoccurrences will not be reported. |
| NAME_ENT/80132000 | `func_80135650` | Time-boxed | 'D_80120CF7' undefined; reoccurrences will not be reported. |
| TT/80146BE0 | `func_80147074` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| ETC/8013BEF0 | `func_8013F77C` | Time-boxed | m2c unresolved local/table/argument type |
| TACO/80134930 | `func_8013506C` | Time-boxed | no s32 field at offset 0x10F8 (there: u32 .unk_10F8) |
| ETC/8013BEF0 | `func_8013EEFC` | Time-boxed | 'D_80150070' undefined; reoccurrences will not be reported. |
| RPG_BAT/8013E4D0 | `func_8013F3F0` | Time-boxed | m2c unresolved local/table/argument type |
| SHOUGATU/80141780 | `func_801430A0` | Time-boxed | m2c unresolved local/table/argument type |
| TT/80146BE0 | `func_80146E10` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| RPG_BAT/8013E4D0 | `func_8013E54C` | Time-boxed | m2c unresolved local/table/argument type |
| TT/80142D40 | `func_801437F0` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| TT/8013F040 | `func_8013FF14` | Time-boxed | m2c unresolved local/table/argument type |
| ETC/8013BEF0 | `func_8013EB90` | Time-boxed | 'D_8014F828' undefined; reoccurrences will not be reported. |
| TACO/80134930 | `func_8013546C` | Time-boxed | no u32 field at offset 0x1100 (there: s32 .unk_1100) |
| TT/80146BE0 | `func_80147150` | Time-boxed | Selector requires struct/union pointer as left hand side |
| DATE/8014A1B0 | `func_8014AF88` | Time-boxed | m2c unresolved local/table/argument type |
| TACO/8015ABF0 | `func_8015B3B0` | Time-boxed | m2c unresolved local/table/argument type |
| EVENT/801024B0 | `func_80103774` | Time-boxed | m2c unresolved local/table/argument type |
| TT/8013F040 | `func_8013F9E0` | Time-boxed | 'D_80155E48' undefined; reoccurrences will not be reported. |
| TT/80135360 | `func_801356C8` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| KANGEI/801355F0 | `func_80135678` | Time-boxed | m2c unresolved local/table/argument type |
| NAME_ENT/80132000 | `func_80132A38` | Time-boxed | no declaration and offset 0x1228 is ambiguous (s32[] .unk_1228, s32 .unk_1228[0]): declare the old view in include/main_api.h or write the field |
| RPG_BAT/801525B0 | `func_801528DC` | Time-boxed | no u8 field at offset 0x110C (there: s8 .unk_110C) |
| TACO/8013B170 | `func_8013B2A0` | Time-boxed | m2c unresolved local/table/argument type |
| NAME_ENT/80132000 | `func_8013635C` | Time-boxed | 'D_8012091E' undefined; reoccurrences will not be reported. |
| NAME_ENT/80132000 | `func_80132B78` | Time-boxed | redeclaration of 'func_8013664C'; previous declaration at line 157 in file 'build/ovl/NAME_ENT/src/ovl/NAME_ENT/80132000.o.sjis.c' |
| TT/80135360 | `func_80135870` | Time-boxed | m2c unresolved local/table/argument type |
| GYOZI/80140C90 | `func_80140E0C` | Time-boxed | 'D_800F5746' undefined; reoccurrences will not be reported. |
| ETC/8013BEF0 | `func_8013F498` | Time-boxed | 'D_801217DC' undefined; reoccurrences will not be reported. |
| TACO/8015ABF0 | `func_8015BD30` | Time-boxed | Selector requires struct/union pointer as left hand side |
| TT/80135360 | `func_80136244` | Time-boxed | 'D_80155B8A' undefined; reoccurrences will not be reported. |
| ETC/8013BEF0 | `func_8013E524` | Time-boxed | Selector requires struct/union pointer as left hand side |
| NAME_ENT/80132000 | `func_80132198` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| TT/80142D40 | `func_80144854` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| EVENT/801024B0 | `func_801035BC` | Time-boxed | m2c unresolved local/table/argument type |
| NAME_ENT/80132000 | `func_80138C1C` | Time-boxed | m2c unresolved local/table/argument type |
| TT/8013F040 | `func_8013F3B4` | Time-boxed | m2c unresolved local/table/argument type |
| ETC/8013BEF0 | `func_8013DEA0` | Time-boxed | no declaration and offset 0x14 is ambiguous (s16[] .unk_014, s16 .unk_014[0]): declare the old view in include/main_api.h or write the field |
| TT/80135360 | `func_801365C4` | Time-boxed | Selector requires struct/union pointer as left hand side |
| ETC/8013BEF0 | `func_8013E89C` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| RPG_BAT/801525B0 | `func_80153F3C` | Time-boxed | 'D_80120E93' undefined; reoccurrences will not be reported. |
| main/800451D0 | `func_800460FC` | Time-boxed | 'D_801255D0' undefined; reoccurrences will not be reported. |
| ETC/8013BEF0 | `func_8013CCE4` | Time-boxed | Selector requires struct/union pointer as left hand side |
| NAME_ENT/80132000 | `func_80132320` | Time-boxed | Selector requires struct/union pointer as left hand side |
| SHOUGATU/8013AF10 | `func_8013AFEC` | Time-boxed | no declaration and offset 0xD4 is ambiguous (u8[] .unk_0D4, u8 .unk_0D4[0]): declare the old view in include/main_api.h or write the field |
| TACO/8013B170 | `func_8013B170` | Time-boxed | no s32 field at offset 0x10F8 (there: u32 .unk_10F8) |
| VALEN/80133C70 | `func_80134030` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| NAME_ENT/80132000 | `func_801324AC` | Time-boxed | 'D_80120C84' undefined; reoccurrences will not be reported. |
| TT/8013F040 | `func_80140920` | Time-boxed | m2c unresolved local/table/argument type |
| TT/80135360 | `func_80135FA8` | Time-boxed | Selector requires struct/union as left hand side |
| SHUGAKU/801344B0 | `func_80134658` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| ETC/8013BEF0 | `func_8013D32C` | Time-boxed | Selector requires struct/union pointer as left hand side |
| NAME_ENT/80132000 | `func_80136808` | Time-boxed | 'D_80120888' undefined; reoccurrences will not be reported. |
| TT/80142D40 | `func_80142DEC` | Time-boxed | 'NULL' undefined; reoccurrences will not be reported. |
| TT/8013F040 | `func_8013FBD0` | Time-boxed | Selector requires struct/union pointer as left hand side |
| TT/80142D40 | `func_80143A44` | Time-boxed | Selector requires struct/union pointer as left hand side |
| OLH/80135AC0 | `func_80135B8C` | Time-boxed | 'D_8013CEBC' undefined; reoccurrences will not be reported. |
| EVENT/801024B0 | `func_80102E54` | Time-boxed | m2c unresolved local/table/argument type |
| GYOZI/80140C90 | `func_801412A4` | Time-boxed | 'D_8014875C' undefined; reoccurrences will not be reported. |
| TACO/80148FE0 | `func_80149538` | Time-boxed | m2c unresolved local/table/argument type |
| ETC/8013BEF0 | `func_8013F054` | Time-boxed | redeclaration of 'func_8013F320'; previous declaration at line 273 in file 'src/ovl/ETC/8013BEF0.c' |
| RPG_BAT/801525B0 | `func_80153784` | Time-boxed | m2c unresolved local/table/argument type |
| NAME_ENT/80132000 | `func_801330E8` | Matched | Resolved diff + ninja |
| NAME_ENT/80132000 | `func_801332FC` | Matched | Resolved diff + ninja |
| SHOUGATU/80141780 | `func_801421D4` | Time-boxed | no s32 field at offset 0x10F8 (there: u32 .unk_10F8) |
| TT/8013F040 | `func_80140624` | Time-boxed | m2c unresolved local/table/argument type |
| main/800451D0 | `func_80045414` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| TACO/80140DF0 | `func_801413B8` | Time-boxed | m2c unresolved local/table/argument type |
| TACO/80135720 | `func_80136988` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| TT/80142D40 | `func_801430D4` | Time-boxed | 'D_80156788' undefined; reoccurrences will not be reported. |
| RPG_BAT/80145CC0 | `func_80146578` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| TACO/80135720 | `func_801361E8` | Matched | TACO sha1 + 592/592 original instruction bytes; funcdiff HI16 string false positive |
| TACO/80135720 | `func_80136438` | Matched | TACO sha1 + 592/592 original instruction bytes; funcdiff HI16 string false positive |
| TACO/80135720 | `func_80136688` | Matched | TACO sha1 + 592/592 original instruction bytes; funcdiff HI16 string false positive |
| GEKO/80140A00 | `func_80141374` | Time-boxed | no declaration and offset 0xD4 is ambiguous (u8[] .unk_0D4, u8 .unk_0D4[0]): declare the old view in include/main_api.h or write the field |
| TT/80135360 | `func_80135360` | Time-boxed | 'D_80155B30' undefined; reoccurrences will not be reported. |
| TACO/80134930 | `func_80134A18` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| TT/8013F040 | `func_8013F040` | Time-boxed | 'D_80155B30' undefined; reoccurrences will not be reported. |
| ETC/8013BEF0 | `func_8013C8EC` | Time-boxed | 'D_8014E0A4' undefined; reoccurrences will not be reported. |
| KANGEI/801355F0 | `func_80135838` | Time-boxed | no s32 field at offset 0x10F8 (there: u32 .unk_10F8) |
| TACO/80148FE0 | `func_801497EC` | Time-boxed | m2c unresolved local/table/argument type |
| TACO/8013B170 | `func_8013B460` | Time-boxed | Selector requires struct/union pointer as left hand side |
| NAME_ENT/80132000 | `func_80134F6C` | Time-boxed | Syntax Error |
| TACO/80140DF0 | `func_801416E8` | Time-boxed | m2c unresolved local/table/argument type |
| RPG_BAT/801525B0 | `func_801525B0` | Time-boxed | no declaration and offset 0xD4 is ambiguous (u8[] .unk_0D4, u8 .unk_0D4[0]): declare the old view in include/main_api.h or write the field |
| NAME_ENT/80132000 | `func_80139440` | Time-boxed | m2c unresolved local/table/argument type |
| SHOUGATU/80141780 | `func_80141D20` | Time-boxed | Syntax Error |
| NAME_ENT/80132000 | `func_801372A0` | Time-boxed | 'D_800B328C' undefined; reoccurrences will not be reported. |
| TACO/8015ABF0 | `func_8015ACCC` | Time-boxed | m2c unresolved local/table/argument type |
| RPG_BAT/801480F0 | `func_801481C8` | Matched | Resolved diff + ninja |
| SHOUGATU/80141780 | `func_80142DA0` | Time-boxed | no declaration and offset 0x10A5 is ambiguous (u8[] .unk_10A5, u8 .unk_10A5[0]): declare the old view in include/main_api.h or write the field |
| ETC/8013BEF0 | `func_8013BFFC` | Time-boxed | redeclaration of 'func_8013D264'; previous declaration at line 98 in file 'src/ovl/ETC/8013BEF0.c' |
| GYOZI/80140C90 | `func_80140F90` | Time-boxed | 'D_8014875C' undefined; reoccurrences will not be reported. |
| TT/80135360 | `func_80135B5C` | Time-boxed | Selector requires struct/union pointer as left hand side |
| NAME_ENT/80132000 | `func_80134B94` | Time-boxed | 'D_800B3644' undefined; reoccurrences will not be reported. |
| TT/80142D40 | `func_801441BC` | Time-boxed | Selector requires struct/union pointer as left hand side |
| RPG_BAT/801496F0 | `func_801496F0` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| TACO/8015ABF0 | `func_8015C2CC` | Time-boxed | 'D_80160858' undefined; reoccurrences will not be reported. |
| TT/80142D40 | `func_80144A04` | Time-boxed | Selector requires struct/union pointer as left hand side |
| BUNKAKEN/80145E40 | `func_80145EBC` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| RPG_BAT/80135AE0 | `func_80135E80` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| TACO/80135720 | `func_80135A04` | Time-boxed | 'sp58' undefined; reoccurrences will not be reported. |
| NAME_ENT/80132000 | `func_80133510` | Time-boxed | m2c unresolved local/table/argument type |
| RPG_BAT/80156CC0 | `func_80157640` | Time-boxed | no declaration and offset 0xD4 is ambiguous (u8[] .unk_0D4, u8 .unk_0D4[0]): declare the old view in include/main_api.h or write the field |
| RPG_BAT/8013E4D0 | `func_8013EA60` | Time-boxed | 'D_80121643' undefined; reoccurrences will not be reported. |
| RPG_BAT/80135AE0 | `func_80135B4C` | Time-boxed | Resolved REAL DIFFERENCE; no candidate retained |
| ETC/8013BEF0 | `func_8013D570` | Time-boxed | Selector requires struct/union pointer as left hand side |
| TACO/8015ABF0 | `func_8015C718` | Time-boxed | m2c unresolved local/table/argument type |
| NAME_ENT/80132000 | `func_801338DC` | Time-boxed | m2c unresolved local/table/argument type |

## Review and final verification

2026-10-11: Moved to In Review. Inline standards/spec review of the working diff against CODING_STANDARDS.md and the shared brief: C89, fixed-width types, SDK interfaces, existing placeholder names, header ownership and scope pass; no new FAKE or NON_MATCHING code, raw data/tooling/config edits, or unverified source retained. Final default-build verification is pending. sync_protos --check-branch: 0 new disagreements, 17 known; migrate_globals --check: globals OK. All unused trial declarations removed; only NAME_ENT string-address words and the three TACO unsigned selector bytes remain. No commits or staging.

2026-10-11 final verification and review: default Docker ninja -j 4 exits 0, 27/27 SHA1 checks, headers OK and globals OK. After declaration cleanup, NAME_ENT func_801330E8/func_801332FC and RPG_BAT func_801481C8 again pass resolved funcdiff; all three TACO functions again equal all 592 original instruction bytes in the linked binary. git diff --check passes; scoped dupes applies 0. Inline code review against every applicable CODING_STANDARDS.md checklist item and the shared brief is complete with zero unresolved findings. Six functions / 3,620 bytes retained; all other 136 listed functions time-boxed. Fourteen T-0018 rows added. Ticket moved to Done; edits remain uncommitted for the orchestrator.

Final `ninja progress`: `grand total   4740/6958   733232/2279368  32.2%`. Baseline was 4734 functions / 729612 bytes, so measured gain is 6 functions / 3620 bytes. The work list gives RPG_BAT func_801481C8 a 792-byte extent, while its splat nonmatching header is 0x30C (780 bytes); previous aggregate byte claims used the work-list extent and were corrected here.
