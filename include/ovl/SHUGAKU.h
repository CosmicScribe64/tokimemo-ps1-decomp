#ifndef OVL_SHUGAKU_H
#define OVL_SHUGAKU_H

#include "common.h"
#include "game.h"

/* SHUGAKU overlay (load address 0x80132000): externs and types. */

extern s16 D_800CA2DC;
void bg_read_sub2(s32 arg0);

extern s8 D_800B5BD4;
extern s8 D_8013C2E4;
extern u8 D_8013C2EC;
void func_800847B8(u8 arg0);
void func_80085B3C(s32 arg0, s32 arg1);

extern s16 D_800E6382;
extern u8 D_800CA2CC;
extern u8 D_8013C1C0;
extern s32 D_8013AEF0;
void func_800AE0F0(void *arg0, void *arg1);
void func_80132000(void);
void func_80133430(void);
void func_80133D60(void);
void func_801344B0(void);
void func_80134880(void);
void func_80135430(void);
void func_80135CC8(void);
void func_80136380(void);
void func_80139018(void);
void func_801386C4(void);

void normal_date_girl_out(void);
void normal_date_bg_fadein(void);
void func_80134970(void);
void func_801343B0(void);
void func_80134B94(void);

extern s8 D_800CA2F0;
extern s16 D_8013B084;
extern s32 D_8013C870;
extern s32 D_8013C874;
extern s32 D_8013C878;
extern s32 D_8013C880;
extern u8 D_8013C974;
extern s32 D_8013CAF0;
extern s32 D_8013CAF4;
extern s32 D_8013CAF8;
extern s32 D_8013CB00;
extern u8 D_8012069B;
extern s8 D_801206DA;
extern s16 D_801206F0;
extern s32 D_800CA2D0;
extern s32 D_800CA2D4;
extern s32 D_800CA2D8;
extern s32 D_8013C450;
extern s32 D_8013C484;
extern s32 D_8013C4B8;
extern s16 D_800CA2E4;
extern u8 D_80120657;
extern u8 D_801206DF;
extern u8 D_80120723;
extern s16 D_801206EE;
extern u8 D_800CA2EC;
extern s32 D_8013C2D0;
extern s32 D_8013C2D4;
extern s32 D_8013C2D8;
extern s16 D_8013C2DC;
extern s32 D_8013C2E0;
extern s16 D_801C26A0;
extern u8 D_80120697;
extern s32 D_8013AEE0;
void func_800853FC(void);
void func_80062CD0(s32 arg0);
void func_8013ABAC(u8 arg0);

extern s16 D_800CA2E0;
extern s32 D_80122CFC;
extern s16 D_800CA2E8;
extern s32 D_8013B06C;
extern s32 D_8013B070;
extern s32 D_8013B074;
extern s32 D_8013B078;
extern s32 D_8013B07C;
extern s32 D_8013B080;
extern s16 D_8013B08C;
extern u8 D_800CA2F4;
extern s32 D_800CA188;
extern s32 D_8013AD70;
void func_80067DD4(void);
void normal_date_bggirl_fadeout(void);
void func_80132E84(void);
void func_80139D00(void);
void func_80046318(s32 arg0, s32 arg1, s32 arg2); /* overlay view: main defines it with u8 arg0 */
extern s16 D_8013B088;
extern s32 D_8013BFFC;
extern s32 D_8013C000;
extern s32 D_8013C004;

#endif /* OVL_SHUGAKU_H */
