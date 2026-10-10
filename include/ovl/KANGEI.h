#ifndef OVL_KANGEI_H
#define OVL_KANGEI_H

#include "common.h"
#include "game.h"

/* KANGEI overlay (load address 0x80132000): externs and types. */

extern s8 D_80139AE4;
extern s8 D_80139AC0;
extern s8 D_8013A2A8;
extern s16 D_80139ADC;
void func_80133C84(void);
void bg_read_sub2(s32 arg0);

extern s8 D_80139AC4;
extern s32 D_80139DFC;
extern s16 D_801D4094;
extern s16 D_800E6442;
extern s32 D_80139A50;
extern s32 D_80139A54;
extern s32 D_80139A58;
extern s8 D_80139DF0;
typedef struct KSub {
    /* 0x00 */ u8 unk_00[8];
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
} KSub; /* size unknown, only 0x00-0x0F used */

typedef struct KObj {
    /* 0x00 */ u8 unk_00[0x34];
    /* 0x34 */ KSub *unk_34;
} KObj; /* size unknown, only 0x00-0x37 used */

extern KObj *D_80139AD0;
extern s32 D_80139AD4;
extern s32 D_80139AD8;
extern s32 D_8013A210;
extern s32 D_8013A244;
extern s32 D_8013A278;
void func_80085B3C(s32 arg0, s32 arg1);
void normal_date_two_select(void);

extern s32 D_801398B0;
extern s32 D_801398E0;
void func_800AE0F0(void *arg0, void *arg1);
void func_80132090(void);
void func_8013260C(void);
void func_80132E74(void);
void func_80135F84(void);
void func_80137B90(void);
void func_80137E04(void);

extern s16 D_80139AE0;
extern u8 D_8013A2B0;
extern s32 D_80139DB0;
extern s32 D_80139DB4;
extern s32 D_80139DB8;
extern s32 D_80139DBC;
extern s32 D_80139DC0;
extern s32 D_80139DC4;
extern s32 D_80139DC8;
extern s8 D_80139AC8;
void load_palette(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_80084E90(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_800850D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void don_wait(void);
u32 get_g_zyotai_h(u8 arg0);
void func_80133A54(void);
void func_80133EF8(void);
void func_801349D4(void);
void func_801392D4(void);

extern s32 D_80139D10;
extern s32 D_80139D14;
extern s32 D_80139D18;
extern s16 D_80139D1C;
extern s32 D_80139D20;
extern s32 D_80139D24;
extern s32 D_80139D28;
extern s16 D_80139D2C;
extern s16 D_80120658;
extern s32 D_8013980C;
void func_80083474(void);
void func_80135438(void);

extern u8 D_8011F513;
extern u8 D_80120657;
extern s32 D_801398E8;
extern s32 D_801398EC;
extern s32 D_801398F0;
extern s32 D_801398F4;
void normal_date_girl_in(void);
void func_80081190(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, void *arg5, void *arg6, s32 arg7);
void func_80062CD0(s32 arg0);
void func_80139540(void);
void func_80046318(s32 arg0, s32 arg1, s32 arg2); /* overlay view; main_only.h has the u8 view */
void func_80083440(s32 arg0); /* overlay view; main_only.h has the u8 view */
extern s32 D_80139DCC;
extern s32 D_80139DD0;
extern s32 D_80139DD4;
extern s32 D_80139DD8;
extern s32 D_80139DDC;
extern s32 D_80139DE0;
extern s16 D_80139DE4;
extern s16 D_80139DE8;
extern s32 D_80139DEC;
void check_k_scroll();
void func_80083808();
void func_80083A10();
void func_80132214();
void k_disp_inc2();

extern s32 D_800E6378;
void func_8006509C(void);

void func_80135634();

void func_801355F0();

extern s32 D_80139E10;
void place_init();

extern s32 D_800B36C8;
extern s32 D_800B3708;
extern s32 D_800B3688;

extern s32 D_800E7368;
void func_8008585C();
void addr_init_bustup();
void func_80084E4C();
void func_801325D0();

#endif /* OVL_KANGEI_H */
