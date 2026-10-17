#ifndef OVL_ENDING_H
#define OVL_ENDING_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_set_kanji_string /* matched with s32(s16,s32,s32,void*,s32): the result is passed on (main_api.h: void) */
#define MAIN_API_OVERRIDE_dialogue_render /* T-18300: func_8013A2E8 stores the result in D_80122CD0 (main_api.h: void), as main k_disp_start does */

#include "common.h"
#include "libgpu.h"
#include "main_api.h"

extern u8 D_8013C3E0;
s32 set_kanji_string(s16 arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4);
s32 dialogue_render(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, void *arg5, void *arg6, s32 arg7);
extern u32 D_8013C360; /* compares take their own constant register: unsigned (T-9020) */
extern s8 D_8013C364;
void func_80132000(void);
void func_80133C10(void);
void func_80134D20(void);
void func_8013B6A0(void);
extern u8 D_8013BFB8[];
extern u8 D_8013BFC0[];

extern s16 D_8013C358;
extern s16 D_8013C35C; /* read with lh/sh only (T-8080, T-9020) */
s32 func_80132B04(s32 arg0, s32 arg1, s32 arg2);
extern u8 D_8013CA38;







void func_801396A4(s16 arg0, u8 arg1);

void func_801341D8();


void func_80139B48();

void func_80139AA0();

void func_80136C5C(s16 arg0);
void func_80136CD0(s16 arg0);
extern s16 D_8013CA1C[];

extern u8 D_8013CC48[];

void func_80134140();
void func_80137BB4();
void func_80137E40();
void func_80137F64();
void func_801380B0();
void func_80138278();
void func_8013855C();
void func_801387B0();
void func_80138A9C();
void func_80138C70();
void func_80138DC8();
void func_80138FC8();
void func_801391E4();
void func_80139498();


void func_80136D60(void);
s32 func_80137574(void);

extern s16 D_8013CBE0;
void func_8013BDD8(s16 arg0);
void func_80132D98();
void func_80132F18();
s32 func_80137730();
void func_80137AA8();

extern u8 D_8013CAA0;


/* T-9580: symbols recovered from list-8 assembly. */
/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_80132400(void);

/* T-18300: tables that unmatched functions copy whole to the stack (lw/sw runs); the element type comes
 * from the reads of the copy (lh with a halfword index). */
typedef struct EdShorts14 {
    s16 v[14];
} EdShorts14; /* size 0x1C */
typedef struct EdShorts18 {
    s16 v[18];
} EdShorts18; /* size 0x24 */
extern EdShorts18 D_8013C3B0; /* func_801330E4: chance per level 0..0x11, compared with rand() % 256 */
extern EdShorts14 D_8013CA60; /* func_80135F4C: x positions of the 14 sprites */
extern EdShorts14 D_8013CA7C; /* func_80135F4C: y positions of the 14 sprites */

/* T-20930: ENDING's data tables, recovered from the matched initialisers (one store per element, in
 * address order) and the readers. All are .data with the same initial values the initialisers write. */

/* Message scripts: one row per message table, one entry per scene; a scene's three entries are copied
 * together into D_800CA160/164/168 (main) or into D_8013C34C. Entries are message block addresses
 * (D_8013CAC0[0][12] is read as a block whose first word is a message pointer table, func_8013A2E8). */
extern s32 D_8013C280[3][17]; /* size 0xCC */
extern s32 D_8013CAC0[3][15]; /* size 0xB4 */

/* The message tables of the running scene, copied as a whole to D_800CA13C/140/144 (func_80132D98). */
typedef struct EdScriptSet {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
} EdScriptSet; /* size 0x0C */
extern EdScriptSet D_8013C34C;

/* Scene image records: three TIM addresses and a halfword read from the TIM data (the order of
 * func_80137160's and func_800850D4's arguments: +0x04, +0x08, +0x00, +0x0C go to the display record's
 * unk_0C, unk_10, unk_34 and unk_14), then the addresses of the scene's texture pages. */
typedef struct EdScene30 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ u8 pad_0E[2];
    /* 0x10 */ s32 unk_10[8];
} EdScene30; /* size 0x30 */
typedef struct EdScene44 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ u8 pad_0E[2];
    /* 0x10 */ s32 unk_10[13];
} EdScene44; /* size 0x44 */
extern EdScene30 D_8013C5F0; /* func_80134D20 */
extern EdScene44 D_8013C620[15]; /* func_80134DE0 .. func_80135CC0, one initialiser per record */
extern EdScene30 D_8013CBB0; /* func_8013B6A0 */

/* The same four values per portrait as columns of 14 (func_80133C10, func_80134270 case k reads entry
 * k + 1): unk_00 goes to the display record's unk_34, unk_38 to unk_0C, unk_70 to unk_10, unk_A8 to
 * unk_14. unk_A8 is never written (zero data) and is read with lh at a 4-byte stride. */
typedef struct EdHalf4 {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ u8 pad_02[2];
} EdHalf4; /* size 0x04 */
typedef struct EdPortraitTbl {
    /* 0x00 */ s32 unk_00[14];
    /* 0x38 */ s32 unk_38[14];
    /* 0x70 */ s32 unk_70[14];
    /* 0xA8 */ EdHalf4 unk_A8[14];
} EdPortraitTbl; /* size 0xE0 */
extern EdPortraitTbl D_8013C4C0;
extern s32 D_8013C5A0[7]; /* texture page addresses of func_80134270 (func_80133C10) */

/* Callback tables (T-20930, collected from the matched files): a step function copies the table to the
 * stack whole and calls entry D_800E6280.unk_110A (or unk_1109) with 0x80 (T-3330). */
typedef struct EdFnTbl6 { void (*f[6])(); } EdFnTbl6; /* size 0x18 */
typedef struct EdFnTbl9 { void (*f[9])(); } EdFnTbl9; /* size 0x24 */
typedef struct EdFnTbl13 { void (*f[13])(); } EdFnTbl13; /* size 0x34 */
typedef struct EdFnTbl14 { void (*f[14])(); } EdFnTbl14; /* size 0x38 */
typedef struct EdFnTbl16 { void (*f[16])(); } EdFnTbl16; /* size 0x40 */
typedef struct EdFnTbl25 { void (*f[25])(); } EdFnTbl25; /* size 0x64 */
typedef struct EdFnTbl43 { void (*f[43])(); } EdFnTbl43; /* size 0xAC */
extern EdFnTbl16 D_8013C370; /* func_80133030 */
extern EdFnTbl43 D_8013C3E4; /* func_80133540 */
extern EdFnTbl9 D_8013C490; /* 80133540.c */
extern EdFnTbl13 D_8013C5BC; /* func_80133F1C */
extern EdFnTbl9 D_8013CA3C; /* func_80135DD0 */
extern EdFnTbl6 D_8013CAA4; /* func_80137340 */
extern EdFnTbl14 D_8013CB74; /* func_801399B4 */
extern EdFnTbl25 D_8013CBE4; /* func_8013B760 */

/* Small tables copied to the stack whole (8013B6A0.c). */
typedef struct EdWords4 {
    s32 w[4];
} EdWords4; /* size 0x10 */
typedef struct EdShorts13 {
    s16 h[13];
} EdShorts13; /* size 0x1A */
extern EdWords4 D_8013CD90; /* func_8013BB9C: two pairs of func_80046290 arguments */
extern EdShorts13 D_8013CDA0; /* func_8013BC58: thresholds compared with D_8013CBE0 */
#endif /* OVL_ENDING_H */

#ifndef OVL_ENDING_TIMER_T9780
#define OVL_ENDING_TIMER_T9780
s32 func_8013B510(void);
#endif
