#ifndef OVL_RPG_BAT_H
#define OVL_RPG_BAT_H

#include "common.h"
#include "game.h"

extern s32 D_8015EDD4;
extern s32 D_8015EDD8;
extern s32 D_8015EDDC;
extern s32 D_8015EDE0;
extern s32 D_8015EDE4;

extern s32 D_8015EC28;
extern s32 D_8015EC2C;
extern s32 D_8015EC6C;
extern s32 D_8015E744;
#ifndef RPG_BAT_OVERRIDE_D_8015ED98
extern s32 D_8015ED98;
#endif
extern s32 D_8015EBDC;
extern s32 D_8015EBD8;
extern s32 D_8015EBE0;
extern s32 D_8015EDC4;
extern s32 D_8015EDF0;
extern s32 D_8015EE1C;
s32 func_8013E9A8(s32);
void func_8013EA60(s32, s32, s32, s32);
extern s32 D_8015EBAC;
extern s32 D_8015EBB0;
void func_8014EBA8(void);
void func_8014EBD0(void);
void func_8014F230(void);
extern s32 D_8015EE10;
void func_8014F258(void);
void func_8014F500(void);
void func_8014F524(void);

void func_80141C70(void);
void func_80141E38(void);
s32 func_80137280(void);
void func_8014EF8C(void);

extern s32 D_8015ED64[][4];
extern s32 D_8015ED94;

extern u8 **D_8015E740;
extern s32 D_8015E748;
void func_8013415C(void);
void func_80134570(void);
void func_80134984(void);
void func_80135B4C(void);
void func_80135D90(void);
void func_80135E80(void);
extern s32 D_8015EDA8[2]; /* defined in RPG_BAT/8014E780.c (T-9010) */
/* Counters that functions of RPG_BAT/8014E780.c reset together: one variable each, because the
 * original shares one `lui $at` between the stores (T-9010). Defined in RPG_BAT/8014E780.c. */
typedef struct RpgBatWords3 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
} RpgBatWords3; /* size 0x0C */
typedef struct RpgBatWords4 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
} RpgBatWords4; /* size 0x10 */
extern RpgBatWords3 D_8015EDB0;
extern RpgBatWords4 D_8015EE00;
extern RpgBatWords4 D_8015EE30;
extern RpgBatWords3 D_8015EE44;
extern RpgBatWords3 D_8015EE50;
void func_80139510(void);
void func_8013F82C(void);
void func_801596E8(void);
extern s32 D_8015EB98;
extern s32 D_8015EBB4;
extern s32 D_8015EBB8;
extern s32 D_8015EC14;
extern s32 D_8015EC74;
s32 func_801373A8(void);
extern s32 D_8015EB9C;
s32 func_80144C84();
s32 func_801452C0();
s32 func_801453B4();
extern s32 D_8015EBBC;
/* The two prototypes below can be hidden by a file that was matched with the implicit int view the
 * original callers had: it defines RPG_BAT_OVERRIDE_<name> before its includes and declares
 * `s32 <name>();` itself (the definitions in 8013E4D0.c are void; T-16210). */
#ifndef RPG_BAT_OVERRIDE_func_8013E7C0
void func_8013E7C0(s32 a, s32 b, s32 c, s32 d);
#endif
void func_8013E810(s32 a, s32 b, s32 c, s32 d);
#ifndef RPG_BAT_OVERRIDE_func_8013E97C
void func_8013E97C(s32 a, s32 b, s32 c);
#endif

void func_8014F080(s32 arg0);
void func_8014EF3C(void);

void func_8013F0F4(s32 a, s32 b, s32 c);
void func_801328C4(s32 a, s32 b);
s32 func_8014A3DC(void);
/* Same override rule as the pair above (the definition in 8013E4D0.c is void; T-17101). */
#ifndef RPG_BAT_OVERRIDE_func_8013F15C
void func_8013F15C(s32 a, s32 b, s32 c);
#endif
s32 func_8013E90C(s32 i);
s32 func_80156870(void);

void func_80153F3C(s32, s32, s32, s32, s32, s32);
extern s32 D_8015EC38;
extern s32 D_8015EC3C;
extern s32 D_8015EC40;
extern s32 D_8015EC44;
extern s32 D_8015ED28;
extern s32 D_8015ED2C;
extern s32 D_8015ED30;
extern s32 D_8015ED34;
extern s32 D_8015EC48;
extern s32 D_8015EC4C;
void func_80155868(s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* defined in C in one object, called from another (T-0500) */
s32 func_8013EA00(s32 arg0, s32 arg1);

extern s16 D_8015E5A0[][12];
extern s32 D_8015EE5C;
extern s32 D_8015EE60;
extern s32 D_8015EE68;
extern s32 D_8015EE74;
extern s32 D_8015EE80;
extern s32 D_8015EE64;
void func_8013E8B8(s32 a0, s32 a1, s32 a2);
void func_8013E864(s32 arg0, s32 arg1, s32 arg2);
void func_8014EB58(s32 arg0);
extern s32 D_8015EE6C;
extern s32 D_8015EE70;
extern s32 D_8015EE78;
extern s32 D_8015EE7C;
extern s32 D_8015EE84;
extern s32 D_8015EE88;
/* T-5000: six words read and written as one object (the original keeps loads after the stores). */
typedef struct RpgRec18 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
} RpgRec18; /* size 0x18 */
extern RpgRec18 D_8015EC58;
void func_80150DE0(void);
extern s32 D_8015EEE8;
extern s32 D_8015EC20;
extern s32 D_8015EC24;
extern s32 D_8015ED9C;
extern s32 D_8015EDC8;
extern s32 D_8015EDE8;
extern s32 D_8015EDF4;
extern s32 D_8015EE40;
s32 func_8013C1CC(void);
void func_8013C2A0(void);
void func_8013C32C(void);
void func_8013BEA8(void);
s32 func_8013B910(void);
void func_8013C890(void);
void func_8013CC90(void);
void func_801439E0();
void func_8014B294(void);
void func_8014B79C(void);
void func_8014C4FC(void);
void func_8014C804(void);
void func_8014CB0C(void);
void func_8014CE14(void);
/* Pad-sequence matcher state of func_8013BBF8, one aggregate. The original keeps the store of unk_00 in front
 * of the load of unk_04; four separate scalars let the build sink that store into the branch delay slot (T-17107). */
typedef struct RpgPadSeq {
    /* 0x00 */ s32 unk_00; /* pad word read this frame (D_8015EBEC) */
    /* 0x04 */ s32 unk_04; /* pad word of the previous change (D_8015EBF0) */
    /* 0x08 */ s32 unk_08; /* position in the D_8015E578 sequence (D_8015EBF4) */
    /* 0x0C */ s32 unk_0C; /* position in the D_8015E550 sequence (D_8015EBF8) */
} RpgPadSeq; /* size 0x10 */
extern RpgPadSeq D_8015EBEC;
void func_8013BBF8();
/* Battle counters; one struct because as1 then keeps the access order of the original (T-4030, func_801452C0). */
typedef struct RpgStat {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C; /* T-17100: the words between are the scalars D_8015EC08..D_8015EC30 */
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
    /* 0x38 */ s32 unk_38;
} RpgStat; /* size 0x3C */
extern RpgStat D_8015EBFC;

extern s32 D_8015EE0C;

extern s32 D_8015ED3C[];

extern s32 D_8015EBE4;

extern s32 D_8015ED8C[2]; /* [1] is D_8015ED90; one base symbol keeps loads behind stores (T-4070) */

void func_80134A70();
void func_8013E300();
void func_80136530();
void func_8013C5D0();
void func_80147F10();
void func_80143830();
void func_80141C30();
void func_8013A480();
void func_80132000();
void func_801454F0();
void func_801496F0();
void func_8014B738();
void func_8014ED80();
void func_8014EE98();
void func_80150F84();
void func_80151F38(void);
extern u8 D_8015E814;
extern s32 D_8015EC18;
void func_8013F1C4();
extern u8 D_8015E904[];
extern u8 D_8015E9CC[];
extern s32 D_8015EC50;
extern s32 D_8015EC54;

extern s32 D_8015EDEC;
void func_8014EF6C(void);
void func_8014F1D4(s32 arg0);
void func_8014F278();
void func_8014F350();
void func_8013F250();
void func_8013F220();

extern u8 D_8015E74C[];
extern s32 D_8015EC78;
extern s32 D_8015EDB8;
/* A file that reads the first word as a scalar (func_80147930, T-17109) defines RPG_BAT_OVERRIDE_D_8015EBCC and declares `extern s32 D_8015EBCC;` itself. */
#ifndef RPG_BAT_OVERRIDE_D_8015EBCC
extern s32 D_8015EBCC[3];
#endif
extern s32 D_8015EEE4;
extern u8 D_8015E774[];
extern u8 D_8015E79C[];
extern u8 D_8015E850[];
extern u8 D_8015E878[];
extern u8 D_8015E8A0[];
extern u8 D_8015EA08[];
extern u8 D_8015EA30[];
extern u8 D_8015EA58[];
extern u8 D_8015EA80[];
extern u8 D_8015EAA8[];
extern s32 D_8015EDA0;
extern s32 D_8015EDA4;
extern u32 D_8015EE08;
void func_8014F38C();
extern s32 D_8015EDC0;
extern s32 D_8015EDCC;
extern u32 D_8015EE04;
void func_8015A11C(void);
extern s32 D_8015EBD0;
extern s32 D_8015EBD4;
extern u32 D_8015EBA0;
extern s32 D_8015EBA8;
void func_801321D0(void);
void func_80134D30(void);
void func_801368B0(void);
void func_8013A650(void);
void func_801421E0(void);
void func_801484E0(void);
void func_801498C0(void);
void func_8014A660(void);
void func_801505B8(void);
void func_80154430(void);
void func_80155CC0(void);
void func_8015A1F0(void);
extern s32 D_8015EE38;
void func_8014EDFC();
void func_801340F0();
void func_80135AE0();
void func_80136374();
void func_80136E2C();
void func_8013D030();
void func_8013D0D4(void);
void func_801420EC();
s32 func_801446F8();
s32 func_801447B8(void);
void func_80150AA4(void);
void func_80151108(void);
void func_80151380(s32 arg0);
s32 func_801513C8(void);
s32 func_801515F0(void);
s32 func_80147930();
void func_801481C8();
void func_8014DF60();
void func_8014F88C();
void func_8014FDBC();
void func_80151C10();
s32 func_80155A70();
s32 func_8013B2F0(void);
void func_8013B534(void);
void func_8015675C();
void func_801568A4();
void func_80159E28();
/* T-11190: declarations added by tools/bulk_m2c.py. */
extern s32 D_8015EE48;

/* T-11190: declarations added by tools/bulk_m2c.py. */
extern s32 D_8015EDF8;

/* T-11190: declarations added by tools/bulk_m2c.py. */
extern s32 D_8015EDB4;
extern s32 D_8015EE4C;

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_801359FC(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_8013673C(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_80137204(void);
void func_80137148(s32 arg0, s32 arg1);

#endif
extern u8 D_8015E9E0[];
extern u8 D_8015EB70[];
extern u8 D_8015E828[];
extern u8 D_8015E8F0[];
extern s32 D_8015EBC0;
extern s32 D_8015EBC4;
extern s32 D_8015EBC8;
#ifndef RPG_BAT_OVERRIDE_D_8015EDAC
/* The six words D_8015EBC0..D_8015EBD4 are one aggregate; func_8013EA60 only matches when it reads them through
 * one struct (the original loads the counter above the flag-byte store, scalars give the other order; T-17107). */
typedef struct RpgGauge {
    /* 0x00 */ s32 cur[3];  /* D_8015EBC0, D_8015EBC4, D_8015EBC8 */
    /* 0x0C */ s32 flag[3]; /* D_8015EBCC, D_8015EBD0, D_8015EBD4 */
} RpgGauge; /* size 0x18 */
/* FAKE: interim view. The scalar names above stay for the files that still use them (801504E0.c, 8014DF60.c,
 * 80154430.c, func_8013C1CC), so RPG_GAUGE reaches the aggregate through the first symbol until those are
 * migrated to a real `RpgGauge D_8015EBC0`. T-17107 */
#define RPG_GAUGE ((RpgGauge *)&D_8015EBC0)
extern u32 D_8015EDAC;
#endif
void func_80136A9C(void);
void func_80136BD8(void);
void func_80136CCC(void);
extern u32 D_8015EC34;
void func_8013EA30();
void func_8014F3C8();
void func_80136070();
void func_801361AC();
extern u32 D_8015ED38;

#ifndef OVL_RPG_OBJECT_BLOCK_T9520
#define OVL_RPG_OBJECT_BLOCK_T9520
typedef struct RpgObjectRecord44 {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 pad3[3];
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 pad7[0x13];
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ u8 pad1E[8];
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ u8 pad28[2];
    /* 0x2A */ s16 unk_2A;
    /* 0x2C */ u8 pad2C[0xC];
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ u8 unk_40;
    /* 0x41 */ u8 pad41[3];
} RpgObjectRecord44; /* size 0x44; confirmed main object-table view, T-9520 */
typedef struct RpgObjectBlock {
    /* 0x0000 */ RpgObjectRecord44 prefix[96];
    /* 0x1980 */ RpgObjectRecord44 group[64];
} RpgObjectBlock; /* size 0x2A80 */
#endif

void func_8014E1A4();
void func_8014E28C();

extern s32 D_8015EC1C;
extern s32 D_8015EE28;
extern s32 D_8015EE20;
typedef struct RpgClearWords50 {
    /* 0x00 */ s32 values[50];
} RpgClearWords50; /* size 0xC8 */

/* Three flag bytes at a 0x44-byte stride are accessed through the shared record table. */
typedef struct RpgFlagRecord44 { /* size 0x44 */
    /* 0x00 */ u8 unk_00[3];
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u8 unk_04[0x40];
} RpgFlagRecord44;

void func_80148654(void);
void func_80148B08(void);

/* T-9780: second dialogue text buffer. */
s32 func_80142544(void);
void func_801426B0(void);
void func_80142CB8(void);

#ifndef OVL_RPG_HEALTH_RECORDS_T9780
#define OVL_RPG_HEALTH_RECORDS_T9780
/* Additional flag/health view of the 0x44-byte main object records. */
typedef struct RpgHealth44 {
    /* 0x00 */ u8 pad0[3];
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u8 pad4[0x22];
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ u8 pad28[0x1C];
} RpgHealth44; /* size 0x44 */
typedef struct RpgHealthBlock {
    /* 0x0000 */ RpgHealth44 record[160];
} RpgHealthBlock; /* size 0x2A80; same extent as RpgObjectBlock */
extern s32 D_8015EDBC;
extern s32 D_8015EE14;
extern s32 D_8015ED14;
#endif

#ifndef OVL_RPG_BAT_TRANSITION_T9740
#define OVL_RPG_BAT_TRANSITION_T9740

/* Packed shade used by the scene transition renderer. T-9740. */
typedef union RpgColorWord {
    /* 0x00 */ u32 word;
    /* 0x00 */ struct {
        /* 0x00 */ u8 r;
        /* 0x01 */ u8 g;
        /* 0x02 */ u8 b;
        /* 0x03 */ u8 code;
    } rgb;
} RpgColorWord; /* size 0x04 */

/* Three words rooted at D_8015EE08; the last advances by two per frame. */
typedef struct RpgTransition0C {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ s32 unk_08;
} RpgTransition0C; /* size 0x0C */

void func_80150120(void);

#endif


#ifndef OVL_RPG_ASSET_WORDS_T9720
#define OVL_RPG_ASSET_WORDS_T9720
/* Word values copied into 0x44-byte draw records by func_8013E54C. */
#ifndef RPG_BAT_OVERRIDE_D_8015EE8C
extern s32 D_8015EE8C;
#endif
extern s32 D_8015EE90;
extern s32 D_8015EE9C;
extern s32 D_8015EEA0;
extern s32 D_8015EEA4;
extern s32 D_8015EEA8;
extern s32 D_8015EEB4;
extern s32 D_8015EEB8;
extern s32 D_8015EEBC;
extern s32 D_8015EEC0;
extern s32 D_8015EECC;
extern s32 D_8015EED0;
#endif


#ifndef OVL_RPG_BAT_W9_FF28_H
#define OVL_RPG_BAT_W9_FF28_H
/* Dispatcher has no result read by its callers. T-9750. */
void func_8013FF28(void);
#endif /* OVL_RPG_BAT_W9_FF28_H */


#ifndef W9_RPG_BAT_func_8013A98C
#define W9_RPG_BAT_func_8013A98C
void func_8013A98C(void);
#endif

#ifndef W9_RPG_BAT_func_8013ACA4
#define W9_RPG_BAT_func_8013ACA4
void func_8013ACA4(void);
#endif


#ifndef W9_RPG_BAT_D_8015EBA4
#define W9_RPG_BAT_D_8015EBA4
extern s32 D_8015EBA4;
#endif

#ifndef W9_RPG_BAT_D_8015EED4
#define W9_RPG_BAT_D_8015EED4
extern s32 D_8015EED4;
#endif

#ifndef W9_RPG_BAT_D_8015EED8
#define W9_RPG_BAT_D_8015EED8
extern s32 D_8015EED8;
#endif

#ifndef W9_RPG_BAT_D_8015EEDC
#define W9_RPG_BAT_D_8015EEDC
extern s32 D_8015EEDC;
#endif

#ifndef W9_RPG_BAT_D_8015EEE0
#define W9_RPG_BAT_D_8015EEE0
extern s32 D_8015EEE0;
#endif

#ifndef W9_RPG_BAT_D_8015EEEC
#define W9_RPG_BAT_D_8015EEEC
extern s32 D_8015EEEC;
#endif

#ifndef W9_RPG_BAT_func_80155E80
#define W9_RPG_BAT_func_80155E80
void func_80155E80(void);
#endif

#ifndef W9_RPG_TEXT_E92C
#define W9_RPG_TEXT_E92C
extern u8 D_8015E92C[]; /* text destination; size unknown, func_801563A0. T-9710 */
#endif

#ifndef RPG_BAT_MOTION_T9930
#define RPG_BAT_MOTION_T9930
void func_80142DF4(void);
#endif

#ifndef OVL_RPG_BAT_TOGGLE_T10030
#define OVL_RPG_BAT_TOGGLE_T10030
extern s32 D_8015EC70;
void func_8013D3E4(void);
void func_8015461C(void);
extern s32 D_8015EC68;
#endif
void func_8014E300(void);
void func_8014E5B4(s32 arg0);

#ifndef OVL_RPG_BAT_PTRS_T10720
#define OVL_RPG_BAT_PTRS_T10720
extern s32 D_8015EE94;
extern s32 D_8015EE98;
extern s32 D_8015EEAC;
extern s32 D_8015EEB0;
extern s32 D_8015EEC4;
extern s32 D_8015EEC8;
#endif

#ifndef OVL_RPG_BAT_T10850_TYPES
#define OVL_RPG_BAT_T10850_TYPES
/* T-10850: initializer is called before its definition in its original object. */
s32 func_8014E994(void);
#endif


#ifndef OVL_RPG_BAT_T12040
#define OVL_RPG_BAT_T12040
void func_80132348(void);
void func_80132A94(s32 arg0, s32 arg1);

/* T-12040: displacement and the six rectangle words share EC50..EC6F. */
typedef struct RpgMotion20 {
    /* 0x00 */ s32 dx;
    /* 0x04 */ s32 dy;
    /* 0x08 */ RpgRec18 rect;
} RpgMotion20; /* size 0x20 */
void func_8015A6D4(void);
void func_8015A744(void);
void func_8015A8B4(void);
void func_80132CE4(void);
void func_80132DE8(void);
void func_80132EE0(void);

/* T-12040: position and displacement share the EC28..EC57 work range. */
typedef struct RpgMotion30 {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ u8 unk_08[0x20];
    /* 0x28 */ s32 dx;
    /* 0x2C */ s32 dy;
} RpgMotion30; /* size 0x30 */

#endif

#ifndef OVL_RPG_BAT_T12300_DECLS
#define OVL_RPG_BAT_T12300_DECLS
/* T-12300: word flag defined in 8014E780.c and read by 80145CC0.c. */
extern s32 D_8015EE18;
/* T-12300: word counters cleared by func_8014A660 (sw accesses). */
extern s32 D_8015EC7C;
extern s32 D_8015EC80;
extern s32 D_8015ECEC;
extern s32 D_8015ECF0;
extern s32 D_8015ECF4;
extern s32 D_8015ECF8;
/* Existing guarded definitions later in 8014A660.c. */
void func_8014AB84(s32 arg0, s32 arg1, s32 arg2);
void func_8014AC74(void);

#ifndef OVL_RPG_BAT_T17100_TABLES
#define OVL_RPG_BAT_T17100_TABLES
/* T-17100: constant tables that unmatched functions copy whole to the stack
 * (an lw/sw run, written in C as one struct assignment, like StrTbl41). */
typedef struct RpgDirTable {
    /* 0x00 */ s32 v[16]; /* eight (x, y) unit steps at 45 degrees, 0x100 = 1.0 */
} RpgDirTable; /* size 0x40 */
typedef struct RpgStrTbl4 {
    /* 0x00 */ u8 *s[4];
} RpgStrTbl4; /* size 0x10 */
typedef struct RpgWordTbl4 {
    /* 0x00 */ s32 v[4];
} RpgWordTbl4; /* size 0x10 */
typedef struct RpgWordTbl10 {
    /* 0x00 */ s32 v[10];
} RpgWordTbl10; /* size 0x28 */
typedef struct RpgHalfTbl12 {
    /* 0x00 */ s16 v[12]; /* six (x, y) offsets */
} RpgHalfTbl12; /* size 0x18 */
extern RpgDirTable D_8015E4F0;  /* func_801328C4 */
extern RpgStrTbl4 D_8015E530;   /* func_80139E88 */
extern RpgWordTbl4 D_8015E540;  /* func_80139E88 */
extern RpgWordTbl10 D_8015E550; /* func_8013BBF8 */
extern RpgWordTbl10 D_8015E578; /* func_8013BBF8 */
extern RpgHalfTbl12 D_8015E720; /* func_8014BB80 */
extern RpgStrTbl4 D_8015EFA0;   /* func_80152384 */
extern RpgStrTbl4 D_8015EFB0;   /* func_80152384 */
extern RpgStrTbl4 D_8015EFC0;   /* func_80152384 */
extern RpgWordTbl4 D_8015EFD0;  /* func_80152384 */
extern RpgStrTbl4 D_8015EFE0;   /* func_80159B40 */
extern RpgStrTbl4 D_8015EFF0;   /* func_80159B40 */
extern RpgWordTbl4 D_8015F000;  /* func_80159B40 (the first 16 bytes of a zero block) */
/* T-17100: called by func_801498C0 before their definitions. */
void func_80149AA8(void);
void func_8014A16C(void);
#endif

#endif
