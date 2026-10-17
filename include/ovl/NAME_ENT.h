#ifndef OVL_NAME_ENT_H
#define OVL_NAME_ENT_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_D_800E8BEE /* matched as u8 (main_api.h: s8) */
#define MAIN_API_OVERRIDE_D_80120721 /* matched as u8 (main_api.h: s8) */

#include "common.h"
#include "psx_types.h"
#include "main_api.h"
s32 func_80142028(void);
void func_801460D8(void);
void func_801462B4(void);
void func_80146624(void);
void func_80146FE0(void);
void func_80147590(void);
void func_80147BEC(void);
void func_8014826C(void);
void func_801488DC(void);
void func_80148C20(void);
void func_80148F08(void);
void func_80149340(void);
void func_80149840(void);
void func_80149E3C(void);
void func_8014A600(void);
void func_8014A848(void);
void func_8014A98C(void);
void func_8014AB18(void);
void func_8014AD44(void);
void func_8014AE2C(void);
extern s32 D_8014D0A8;
extern u8 D_800E8BEE;
void func_8013B77C(void);
void func_80142880(void);
void func_801455E4(s32 arg0);
void func_80145AC4(void);
void func_80145760(void);
void func_80145848(void);
void func_8014068C(void);
void func_80140BA4(void);
extern u8 D_8014BEB0[];
void func_80138F60(void);
void func_80139CB4(void);
extern s32 D_8014D0D4;
void func_8013B51C(s32 arg0);
void func_80132198(void);
void func_80132320(void);
void func_80144A34(void);
void func_80142A80(void);
extern u8 D_8014B824[];
extern u8 D_8014B838[];
void func_8013BB24(void);
void func_8013BE18(void);
void func_8013BF74(void);
extern u8 D_8014B7B4[];
extern u8 D_8014B7C8[];
void func_80144FDC(void);
void func_80145378(void);
void func_80144F98(void);
extern u8 D_8014CC74[8]; /* name-entry buffers 0 and 1 (D_8014CC84 is buffer 2); sized like their definitions, T-17900 */
extern u8 D_8014CC7C[8];
extern u8 D_8014CC84[];
extern u8 D_8014BA60[];
extern u8 D_8014BA78[];
extern u8 D_8014BA88[];
extern u8 D_8014BA28[];
extern u8 D_8014BA44[];
extern u8 D_8014B998[];
extern u8 D_8014B9A4[];
void func_8013BB10(void);
void func_8013B1D8(void);
extern u8 D_8014BBD4[];
extern u8 D_8014BBEC[];
extern u8 D_8014BAA4[];
extern u8 D_8014BAC4[];
extern u8 D_8014BAE0[];
extern u8 D_8014BB8C[];
extern u8 D_8014BBA8[];
extern u8 D_8014BBC0[];
extern u8 D_8014BCB4[];
extern u8 D_8014BCCC[];
extern u8 D_8014BCE0[];
extern u8 D_8014BDE8[];
extern u8 D_8014BE00[];
extern u8 D_8014BE10[];

extern u8 D_80120721;
extern s32 D_8014CCB8;
extern s32 D_8014CCBC;
extern s32 D_8014CCC0;
extern s32 D_8014CCC4;
extern s32 D_8014CCCC;
extern s32 D_8014CCD0[];
extern u8 D_8014CE6C[];
extern u8 D_8014CEE4[];
extern u8 D_8014CF5C[];
void func_80132134(void);
void func_80132E80(void);
void func_80138488(void);
/* T-11650: called by func_80132320, func_80132E80 and func_80138488 before their definitions. */
void func_801324AC(void);
void func_80132A38(void);
void func_80134EB8(void);
void func_80135168(void);
void func_80135434(s32 arg0);
void func_801357F4(void);
void func_80135A98(void);
void func_80135BB4(void);
void func_80135E58(void);
void func_80135F74(void);
void func_8013621C(s32 arg0);
void func_801362D0(void);
void func_80137708(void);
void func_80137FC4(void);
void func_80138268(s32 arg0);
void func_80138330(void);
void func_80138338(void);
void func_801385A8(void);
void func_80138634(void);
void func_801387F4(void);
void func_80138A7C(void);
void func_80138BCC(void);
s32 func_80139684(void);

void func_80140D6C(void);
void func_80140E18(void);

void func_801454CC(void);
extern s32 D_8014D184;

void prologue_line_start(void);
void prologue_line_run(void);
extern s8 D_8014D180;
void func_80142E24();
void func_80143580();
s32 func_80140CCC(void);
extern s32 D_8014D170;

extern s32 D_8014D0E0;
extern s32 D_8014D0E4;
extern s32 D_8014D0E8;
extern s32 D_8014D0EC;
extern s32 D_8014D0F0;
void func_8013B59C(void);
void func_8013664C(s32 arg0);

extern u8 D_8014D014[]; /* u8 (u, v) pairs, one per decimal digit */
extern u8 D_8014D070[];
extern u8 D_8014D078[];
extern u8 D_8014D080[];
extern s8 D_8014D08C;
extern s8 D_8014D090;
extern s8 D_8014D094;
extern s8 D_8014D098;
extern s8 D_8014D09C;
extern s8 D_8014D0A0;
extern s8 D_8014D0A4;

extern s32 D_8014CD28;
extern s32 D_8014CD2C;
extern s32 D_8014CD30;
extern s32 D_8014CD34;
extern s32 D_8014CD38;
extern s32 D_8014CD3C;
extern s32 D_8014CD40;
extern s32 D_8014CD48;
extern u8 D_8014D028[];
/* T-9520: globals used by the matched wave-8 functions. */
extern s32 D_8014CD44;
extern s32 D_8014CD4C;
extern s32 D_8014CD50;
extern s32 D_8014CD54;
extern s32 D_8014CD58;
extern s32 D_8014CD5C;
extern s32 D_8014CD60;
extern s32 D_8014CD64;
extern s32 D_8014CD68;
extern s32 D_8014CD6C;
extern s32 D_8014CD70;
extern s32 D_8014CD74;
extern s32 D_8014CD78;
extern s32 D_8014CD7C;
extern s32 D_8014CD80;
extern s32 D_8014CD84;
extern s32 D_8014CD88;
extern s32 D_8014CD8C;
extern s32 D_8014CD90;
extern s32 D_8014CD94;
extern s32 D_8014CD98;
extern s32 D_8014CD9C;
extern s32 D_8014CDA0;
extern s32 D_8014CDA4;
extern s32 D_8014CDA8;
extern s32 D_8014CDAC;

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_80137AA0(void);

#endif /* OVL_NAME_ENT_H */
extern u8 D_8014CC70;
void func_801377E8();
extern s32 D_8014CC58;
extern s32 D_8014CC5C;
extern s32 D_8014CC50;
extern s32 D_8014CC54;
void func_8013635C();
void func_801372A0();
extern s32 D_8014CCC8;
void func_80134F6C();
void func_80137B84(s32 arg0, s32 arg1, s32 arg2);
void func_80137CA0();



#ifndef OVL_NAME_ENT_T9770_SELECTION_GLOBALS
#define OVL_NAME_ENT_T9770_SELECTION_GLOBALS
extern s32 D_8014CCAC; /* Word reset in func_801341A8. T-9770 */
extern s32 D_8014CCB0; /* strlen of the help text in func_80133510 */
extern u8 *D_8014D010; /* help title shown by func_801338DC: D_8014CCE4, D_8014CD04 or D_8014CD10 */
extern s32 D_8014CCB4; /* Word selection and old-value snapshot. T-9770 */
#endif

#ifndef OVL_NAME_ENT_T10010_INPUT_GLOBALS
#define OVL_NAME_ENT_T10010_INPUT_GLOBALS
extern u8 D_8014CD04[]; /* Name-entry character buffer. T-10010. */
extern s32 D_8014CC40; /* Word arguments to func_80046290. */
extern s32 D_8014CC44;
extern s32 D_8014CC48;
extern s32 D_8014CC4C;
#endif

#ifndef OVL_NAME_ENT_T10860_H
#define OVL_NAME_ENT_T10860_H
/* Four blood-type text pointers; stride four, T-10860. */
extern u8 *D_8014D1A0[4];
#endif

#ifndef OVL_NAME_ENT_PROFILE_FLAGS_H
#define OVL_NAME_ENT_PROFILE_FLAGS_H
/* GameState +0x1CC: month/day/blood-type fields read by 80149E3C. T-10860. */
typedef struct NameProfileFlags {
    /* 0x00 */ u32 month : 4;
    /* 0x00 */ u32 day : 5;
    /* 0x00 */ u32 blood_type : 2;
    /* 0x00 */ u32 unk_0B : 21;
} NameProfileFlags; /* size 0x04 */
#endif

#ifndef OVL_NAME_ENT_T10860_FORWARD_H
#define OVL_NAME_ENT_T10860_FORWARD_H
void func_80145EA4(void);
void func_801478E0(void);
#endif

#ifndef OVL_NAME_ENT_T10860_ACTOR_HELPER_H
#define OVL_NAME_ENT_T10860_ACTOR_HELPER_H
/* Side-effect-only actor setup; no returned value (801473AC assembly). */
void func_801473AC(void);
#endif

#ifndef OVL_NAME_ENT_T10860_FADE_HELPER_H
#define OVL_NAME_ENT_T10860_FADE_HELPER_H
/* Original K&R fallthrough signature, required by 80147BEC's live ranges. */
s32 func_80147A08(void);
#endif

#ifndef OVL_NAME_ENT_DIALOGUE_TABLES_H
#define OVL_NAME_ENT_DIALOGUE_TABLES_H
/* Paired command words, indexed at stride 8 by the dialogue byte (T-10860). */
typedef struct NameCommandPair8 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} NameCommandPair8; /* size 0x08 */
extern NameCommandPair8 D_8014DA30[43];
/* At least 43 dialogue pairs; this splat label also covers trailing untyped data. */
extern NameCommandPair8 D_8014DB88[];
extern u8 D_8014DA04[44];
extern s32 D_8014D188;
void func_80146940(void);
void func_80146CB8(void);
#endif

#ifndef OVL_NAME_ENT_T10860_LEVEL_HISTORY
#define OVL_NAME_ENT_T10860_LEVEL_HISTORY
/* T-10860: last getCDlevel sample and four signed samples, indexed by frame & 3. */
extern s32 D_8014D1B0;
extern s32 D_8014D1B4[4];
#endif

#ifndef OVL_NAME_ENT_T10860_DIALOGUE_TEXT
#define OVL_NAME_ENT_T10860_DIALOGUE_TEXT
/* T-10860: 43 text pointers, immediately before the 44-byte speaker table. */
extern u8 *D_8014D958[43];
#endif

#ifndef OVL_NAME_ENT_T10860_SECOND_BOB_HELPER
#define OVL_NAME_ENT_T10860_SECOND_BOB_HELPER
/* T-10860: integer fallthrough view keeps caller 80149840's state in v1. */
s32 func_8014977C(void);
#endif

/* The album buffer is also read by main/800674B0.c. T-10880. */
#include "game.h"

#ifndef OVL_NAME_ENT_T10880_FUNCTIONS
#define OVL_NAME_ENT_T10880_FUNCTIONS
void func_8013ACA4(void);
void func_80140044(void);
/* Original implicit-int display dispatcher falls through. */
s32 func_801425C0(void);
extern s32 D_8014D0D0;
extern s32 D_8014D0D8;
extern s32 D_8014D0DC;
#endif

#ifndef OVL_NAME_ENT_T10890_BIRTHDAY
#define OVL_NAME_ENT_T10890_BIRTHDAY
/* T-10890: byte selections at four-byte intervals; read with lbu, 255 means unset. */
extern u8 D_8014CC90;
extern u8 D_8014CC94;
extern u8 D_8014CC98;
extern u8 D_8014CC9C;
extern u8 D_8014CCA0;
extern u8 D_8014CCA4;
void func_80136808(s32 arg0);
#endif

#ifndef OVL_NAME_ENT_T10890_CHARACTER_TABLES
#define OVL_NAME_ENT_T10890_CHARACTER_TABLES
/* T-10890: 17 pointers per kana table (CD28..CD6C and CD6C..CDB0).
 * Existing scalar declarations retain the first-pointer view for older callers. */
typedef struct NameKanaPages44 {
    /* 0x00 */ u8 *text[17];
} NameKanaPages44; /* size 0x44; index = row * 3 + column / 6 */
extern u8 *D_8014CDB0[45];
extern u16 *D_8014CE64;
extern u8 **D_8014CE68;
void func_80134248(s32 arg0);
void func_80138F00(void);
#endif

#ifndef OVL_NAME_ENT_T11010_H
#define OVL_NAME_ENT_T11010_H
extern s32 D_8014CC60; /* Word arguments to func_80046290. */
extern s32 D_8014CC64;
#endif

#ifndef OVL_NAME_ENT_T11010_OPTIONS_H
#define OVL_NAME_ENT_T11010_OPTIONS_H
void func_8013BB1C(void);
void func_8013B994(void);
/* Option values read back by func_8013B77C (one word each; only the low byte is read). */
extern s32 D_8014D0AC;
extern u8 D_8014D0B0;
extern u8 D_8014D0B4;
extern u8 D_8014D0B8;
extern u8 D_8014D0BC;
extern u8 D_8014D0C0;
extern u8 D_8014D0C4;
extern u8 D_8014D0C8;
extern u8 D_8014D0CC;
#endif

#ifndef OVL_NAME_ENT_T11010_B_H
#define OVL_NAME_ENT_T11010_B_H
extern u8 D_8014CCE4[]; /* Argument block passed to func_801330E8 and friends. */
#endif

#ifndef OVL_NAME_ENT_T11010_C_H
#define OVL_NAME_ENT_T11010_C_H
void func_80142560(void);
extern u8 D_8014CD10[];
#endif

#ifndef OVL_NAME_ENT_T11010_D_H
#define OVL_NAME_ENT_T11010_D_H
extern s32 D_8014CC38; /* Word arguments to func_80046290. */
extern s32 D_8014CC3C;
#endif

#ifndef OVL_NAME_ENT_T11010_E_H
#define OVL_NAME_ENT_T11010_E_H
extern u8 D_8014CC8E; /* loop bound and terminator: labels inside D_8014CC84, the original addresses them by name */
extern u8 D_8014CC8F;
#endif

/* T-11080 forward declarations. */
void func_8013A9C8(void);

/* T-11120 forward declarations. */
void func_80142A3C(void);
s32 func_801429BC(void);

#ifndef OVL_NAME_ENT_T17900_TABLES
#define OVL_NAME_ENT_T17900_TABLES
/* T-17900: sixteen flag counters, cleared and counted four at a time (end address D_8014D070) and
 * copied into Work800E7D10.flags (D_800E7D10) by func_80140F90 and func_80141820. */
extern u8 D_8014D060[16];
/* T-17900: second sixteen-byte flag table of func_80140F90 (walked four at a time up to D_8014D168). */
extern u8 D_8014D158[16];
/* T-17900: sprite (u, v) of each decimal digit, indexed by value % 10 in func_80139740; the label
 * covers 0x64 bytes, the rest is zero. */
extern u8 D_8014D0F4[10][2];
#endif
