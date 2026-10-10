#ifndef OVL_TAIIKU_H
#define OVL_TAIIKU_H

#include "common.h"
#include "game.h"

/* Overlay-local externs (T-1300). */
void func_80136A28(void);
void func_80136AB0(void);
void func_80136B18(void);
void func_80136BB4(void);
void func_8013CD34(void);
void func_8013CF88(void);
void func_8013D114(void);
void func_8013D2B4(void);
void func_80146884(void);
void func_8014692C(void);
void func_80146994(void);
void func_80146A60(void);
void func_801470A0(void);
void func_80147120(void);
void func_80147210(void);
void func_80147318(void);
void func_801331D0(void);
void func_801330FC(void);
void func_80147928(void);
void func_80147594(void);
void func_80143C84(void);
void func_80143CC0(void);
void func_8014756C(void);

extern s32 D_8014A128;
extern s32 D_8014A144;
extern s32 D_8014A158;
extern s32 D_80148F00;
extern s32 D_80148F04;
extern s32 D_80148F08;
extern s32 D_801491B0;
extern s32 D_801491B4;
extern s32 D_801491B8;
extern s32 D_8014A0A0;
extern s32 D_8014A0A4;
extern s32 D_8014A0A8;
extern s32 D_8014A0D0;
extern s32 D_8014A0D4;
extern s32 D_8014A0D8;
extern u8 D_801499D8;
extern u8 D_8014A11C;

extern s32 D_80148EF0;
extern s32 D_80148EF4;
extern s32 D_80148EF8;
extern s32 D_80148EFC;
extern s32 D_801491C0;
extern s32 D_801491C4;
extern s32 D_801491C8;
extern s32 D_801491CC;
extern s32 D_80149FA0;
extern s32 D_80149FA4;
extern s32 D_80149FA8;
extern s32 D_80149FAC;
extern s32 D_8014A0B0;
extern s32 D_8014A0B4;
extern s32 D_8014A0B8;
extern s32 D_8014A0BC;
extern s32 D_8014A12C;
extern s32 D_8014A148;
extern u32 D_8014A14C;
extern s32 D_801491E8;
extern s32 D_80149218;
extern s32 D_80149220;
extern u32 D_80149224;
extern s32 D_8014A3D4;
extern u32 D_8014A3F0;
extern s32 D_8014A3F4;
extern s32 D_8014A3F8;
extern s32 D_8014A370;
extern s32 D_8014A374;
extern s32 D_8014A378;
extern s32 D_8014A37C;
extern s32 D_8014A380;
extern s32 D_8014A384;
extern s32 D_8014A388;

extern s32 D_8014A0E0;
extern s32 D_8014A0E4;
extern s32 D_8014A0E8;
extern s16 D_8014A0EC;
extern s32 D_8014A0F0;
extern s32 D_8014A0F4;
extern s32 D_8014A0F8;
extern s32 D_8014A0FC;
extern s32 D_8014A100;
extern s32 D_8014A104;
extern s32 D_8014A108;
extern s32 D_8014A390;
extern s32 D_8014A394;
extern s32 D_8014A398;
extern s16 D_8014A39C;
extern s32 D_8014A3A0;
extern s32 D_8014A3A4;
extern s32 D_8014A3A8;
extern s32 D_8014A3AC;
extern s32 D_8014A3B0;
extern s32 D_8014A3B4;
extern s32 D_8014A3B8;

extern u8 D_8014A454[];
extern s16 D_80149984;
extern s16 D_80149986;
extern s32 D_80149940;
typedef struct TaiikuPair {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 val;
} TaiikuPair; /* size 0x4 */
extern TaiikuPair D_801499B0[];

extern s32 D_80149FB0;
extern s32 D_80149FC0;
extern s32 D_80149FD0;
extern s32 D_80149FE8;
extern s32 D_80149FF8;
extern s32 D_8014A008;
extern s16 D_8014A020;
extern s16 D_8014A030;
extern s16 D_8014A040;
void func_80136EC0(void);
void func_8013703C(void);
void func_80137090(void);
void func_80137228(void);
void func_8013732C(void);
extern u16 D_801499A2;
void func_8013F3A4(s32);
void func_8013F948(void);
void func_80147F58(s32);
void func_801482BC(s32);
void func_80147A6C(s32);
void func_80147B14(s32);
s32 func_80141CDC();
void func_80132E20(s32, s32);
typedef struct TaiikuBig {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 unk4[0x1C];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 unk24[0x2C];
} TaiikuBig; /* size 0x50 */
extern TaiikuBig D_801227A0[];
s32 func_80140A90();
extern s8 D_8014A110;
void func_80143D24(void);
void func_80143DD0(void);
void func_80143F50(void);
extern u16 D_8014A154;
typedef struct TaiikuRec {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 unk8;
    /* 0xC */ s32 unkC;
} TaiikuRec; /* size 0x10 */
extern s32 D_80149944;

void func_801339C4(void);
extern u8 D_801492C4;
void func_8013DDB8(void);
s8 func_8013FE44();
void func_80144834(void);
void func_801448D4(void);
void func_801449A0(void);
void func_80144B40(void);
void func_80144C70(void);
extern u8 D_8014A4F4;
extern u8 D_8014A1FC;
void func_80146B8C(void);
void func_801488F0(s16 *);
void func_8013D9B4(s32, u8);
extern s16 D_80149204;
extern u32 D_8014921C;
extern s32 D_801491E4;
extern u8 D_801491E0;
extern u8 D_801491E1;
extern u8 D_801491E2;
extern u8 D_801491E3;
extern s32 D_8014994C;
extern s32 D_80149950;
extern s32 D_80149974;
extern s32 D_80149988;
extern s32 D_8014999C;
void func_8013B104(void);
void func_8013C780(void);
void func_8013C978(void);
extern s32 D_80149990;
extern TaiikuPair D_801499B4[];

#endif
