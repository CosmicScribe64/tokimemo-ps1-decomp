---
id: T-25100
title: Wave 8 conveyor - one deep Opus pass per remaining function
status: Done
assignee: orchestrator
created: 2026-10-23
updated: 2026-10-23
links: `[status`, `plan-remaining`, [[playbook]]]
---

## Goal

User request "Finish the game decomp" (2026-10-23, ultracode). Give each of the 92 remaining game functions outside the issue #65 claim (GEKO, OPTION) one deep, history-first Opus matching pass, with a fresh-eyes Opus closer for guards of 25 rows or fewer, then a Sonnet review and the locked merge (`merge3.sh`) with literal default-C checks per match.

## Acceptance criteria

- [ ] Every package below has run and its ticket is Done with results.
- [ ] Every claimed match passed: no INCLUDE_ASM for the function, `funcdiff --resolve --version default` MATCH, 27/27 sha1, header/global/prototype checks.
- [ ] [[status]] and `plan-remaining` updated with the new counts and the blockers left.

## Notes

Packages are file-disjoint (one worktree per C file, functions of a file in sequence), longest chain first, six workers. Usage gate: wait while the 5-hour window is at 80% or more, stop new work at 90% weekly. Script: local `.claude/orchestrator/state/conveyor-w8.js`, data built from the INCLUDE_ASM census at 552926721 (100 game functions left; GEKO 7 and OPTION 1 reserved for diman8, issue #65).

- [[tickets/T-25101-w8-main-8004F870|T-25101]] src/main/8004F870.c: sndi, func_80052F68, func_8005215C, get_h_tokimeki, get_near_menu, func_8004FC10
- [[tickets/T-25102-w8-TAIIKU-80138950|T-25102]] src/ovl/TAIIKU/80138950.c: func_80139AB0, func_8013E894, func_8013DDB8, func_8013F3A4, func_8013E1FC, func_8013EC60
- [[tickets/T-25103-w8-main-800674B0|T-25103]] src/main/800674B0.c: func_8006C988, func_8006A044, func_80067870, func_8006B0C8, func_80067F04
- [[tickets/T-25104-w8-main-8004C3F0|T-25104]] src/main/8004C3F0.c: mod_trans_vram, mod_set, mod_set_gpu, srn_tpage_show
- [[tickets/T-25105-w8-main-80053650|T-25105]] src/main/80053650.c: func_80054E6C, func_80054FC0, func_8005557C, func_80055AFC
- [[tickets/T-25106-w8-TAIIKU-80133C80|T-25106]] src/ovl/TAIIKU/80133C80.c: func_80137C44, func_80137784, func_80135F3C
- [[tickets/T-25107-w8-MASTER-80133680|T-25107]] src/ovl/MASTER/80133680.c: func_801363F4, func_801344E0
- [[tickets/T-25108-w8-main-80062CD0|T-25108]] src/main/80062CD0.c: func_80065964, func_80065B0C, func_8006509C
- [[tickets/T-25109-w8-EN_NICHI-80136B10|T-25109]] src/ovl/EN_NICHI/80136B10.c: func_80136DEC, func_8013735C
- [[tickets/T-25110-w8-TACO-8013DB80|T-25110]] src/ovl/TACO/8013DB80.c: func_8013DEE4, func_8013EAA4
- [[tickets/T-25111-w8-TACO-8013B990|T-25111]] src/ovl/TACO/8013B990.c: func_8013C6D4, func_8013BAC4
- [[tickets/T-25112-w8-TACO-80139170|T-25112]] src/ovl/TACO/80139170.c: func_80139FB8, func_80139424
- [[tickets/T-25113-w8-ETC-8013BEF0|T-25113]] src/ovl/ETC/8013BEF0.c: func_8013F054, func_8013D570
- [[tickets/T-25114-w8-RPG_BAT-80154430|T-25114]] src/ovl/RPG_BAT/80154430.c: func_80154DD8, func_8015528C
- [[tickets/T-25115-w8-main-800490C0|T-25115]] src/main/800490C0.c: _sprite_set_light_effect1, func_80049A40
- [[tickets/T-25116-w8-main-80047550|T-25116]] src/main/80047550.c: func_80047560, func_800476C0
- [[tickets/T-25117-w8-VALEN-80132760|T-25117]] src/ovl/VALEN/80132760.c: func_80133088, func_80132834
- [[tickets/T-25118-w8-main-800563F0|T-25118]] src/main/800563F0.c: strCallback, func_80056BA8
- [[tickets/T-25119-w8-main-80061710|T-25119]] src/main/80061710.c: func_80062948, func_80061A3C
- [[tickets/T-25120-w8-GYOZI-80138B50|T-25120]] src/ovl/GYOZI/80138B50.c: func_8013907C, func_80138C9C
- [[tickets/T-25121-w8-ETC-80132880|T-25121]] src/ovl/ETC/80132880.c: func_80132C3C, func_801328A4
- [[tickets/T-25122-w8-main-80049FF0|T-25122]] src/main/80049FF0.c: func_8004A1A0, func_8004A414
- [[tickets/T-25123-w8-main-800451D0|T-25123]] src/main/800451D0.c: func_800455C4
- [[tickets/T-25124-w8-main-80046500|T-25124]] src/main/80046500.c: func_80046AC8
- [[tickets/T-25125-w8-main-8006CB30|T-25125]] src/main/8006CB30.c: func_8007132C
- [[tickets/T-25126-w8-ENDING-80134D20|T-25126]] src/ovl/ENDING/80134D20.c: func_80135F4C
- [[tickets/T-25127-w8-ENDING-801396E0|T-25127]] src/ovl/ENDING/801396E0.c: func_8013A2E8
- [[tickets/T-25128-w8-ETC-8013FF80|T-25128]] src/ovl/ETC/8013FF80.c: func_8013FFAC
- [[tickets/T-25129-w8-GYOZI-80136220|T-25129]] src/ovl/GYOZI/80136220.c: func_801368B8
- [[tickets/T-25130-w8-MASTER-80132140|T-25130]] src/ovl/MASTER/80132140.c: func_801326B4
- [[tickets/T-25131-w8-NAME_ENT-80142A80|T-25131]] src/ovl/NAME_ENT/80142A80.c: func_80143580
- [[tickets/T-25132-w8-SHOUGATU-80134890|T-25132]] src/ovl/SHOUGATU/80134890.c: func_801350F8
- [[tickets/T-25133-w8-TEL-8013CCD0|T-25133]] src/ovl/TEL/8013CCD0.c: func_8013D88C
- [[tickets/T-25134-w8-main-80043510|T-25134]] src/main/80043510.c: palette_load_vram
- [[tickets/T-25135-w8-ENDING-80133C10|T-25135]] src/ovl/ENDING/80133C10.c: func_80134270
- [[tickets/T-25136-w8-TACO-801447D0|T-25136]] src/ovl/TACO/801447D0.c: func_80145650
- [[tickets/T-25137-w8-TACO-8013B170|T-25137]] src/ovl/TACO/8013B170.c: func_8013B460
- [[tickets/T-25138-w8-RPG_BAT-801447E0|T-25138]] src/ovl/RPG_BAT/801447E0.c: func_80144C84
- [[tickets/T-25139-w8-KANGEI-80132090|T-25139]] src/ovl/KANGEI/80132090.c: func_801327DC
- [[tickets/T-25140-w8-main-8004AD60|T-25140]] src/main/8004AD60.c: make_color_bar
- [[tickets/T-25141-w8-TACO-8013F250|T-25141]] src/ovl/TACO/8013F250.c: func_8013FBF0
- [[tickets/T-25142-w8-BUNKA_SD-80134540|T-25142]] src/ovl/BUNKA_SD/80134540.c: func_80134D94
- [[tickets/T-25143-w8-SHOUGATU-8013E330|T-25143]] src/ovl/SHOUGATU/8013E330.c: func_8013EA98
- [[tickets/T-25144-w8-RPG_BAT-8014ADD0|T-25144]] src/ovl/RPG_BAT/8014ADD0.c: func_8014BFE4
- [[tickets/T-25145-w8-ENDING-80133030|T-25145]] src/ovl/ENDING/80133030.c: func_801330E4
- [[tickets/T-25146-w8-ETC-80143240|T-25146]] src/ovl/ETC/80143240.c: func_80144224
- [[tickets/T-25147-w8-main-80058D20|T-25147]] src/main/80058D20.c: func_8005938C
- [[tickets/T-25148-w8-ENDING-80137340|T-25148]] src/ovl/ENDING/80137340.c: func_801387B0
- [[tickets/T-25149-w8-NAME_ENT-80132000|T-25149]] src/ovl/NAME_ENT/80132000.c: func_80138C1C
- [[tickets/T-25150-w8-KANGEI-80134120|T-25150]] src/ovl/KANGEI/80134120.c: func_80134724
- [[tickets/T-25151-w8-GYOZI-80140C90|T-25151]] src/ovl/GYOZI/80140C90.c: func_801412A4
- [[tickets/T-25152-w8-TACO-80138890|T-25152]] src/ovl/TACO/80138890.c: func_80138B2C
- [[tickets/T-25153-w8-NAME_ENT-80139740|T-25153]] src/ovl/NAME_ENT/80139740.c: func_8013AF78

## Comments

- 2026-10-23 (orchestrator): wave 8 finished (run wf_f91e4ce7-198, 291 agents). 34 packages merged by wave 8 with 57 verified matches, every merge with 27/27 sha1, header/global/prototype checks and literal default-C checks; no merge failed. The other 19 packages went to the wave 9 neighbor ([[tickets/T-25200-wave-9-neighbor]]). Main 6,932/6,952 (98.6%) after the last merge (SHOUGATU 80134890). Script weakness found: agents report `UNIT:func` names and the script compared bare names, so the closer stage never ran here; wave 9 ran those closers. Functions still unmatched move to the second pass, [[tickets/T-25300-wave-10-second-pass]].
