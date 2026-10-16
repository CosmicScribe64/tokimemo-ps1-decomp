#include "common.h"
#include "game.h"
#include "libapi.h"

/* The original clears GameState +0x56C through +0x66B as one byte block. */
typedef struct StateClearBytes100 {
    /* 0x00 */ u8 bytes[0x100];
} StateClearBytes100; /* size 0x100 */

void func_80041000(void) {
    func_80097D90(0x140, 0xF0, 0, 0, 0);
    func_80098380();
    func_80098490(0, 0, 0, 0xF0);
    D_800E8C70 = 8;
    D_800E8C74 = D_800E8CA0;
    D_800E8C84 = 8;
    D_800E8C88 = D_800E90A0;
    func_80098530();
    InitGeom();
    func_800985A0();
    D_8011ECA0 = func_80098370();
    func_800410AC();
}

/* Stack copies of the PsyQ DRAWENV (0x60 bytes here) and DISPENV (0x14 bytes); libgpu.h has only RECT. */
typedef struct DrawEnv60 {
    /* 0x00 */ RECT clip;
    /* 0x08 */ u8 rest[0x58];
} DrawEnv60; /* size 0x60 */

typedef struct DispEnv14 {
    /* 0x00 */ RECT disp;
    /* 0x08 */ RECT screen;
    /* 0x10 */ u8 rest[4];
} DispEnv14; /* size 0x14 */

void func_800410AC(void) {
    DrawEnv60 draw;
    DispEnv14 disp;

    func_8009CCBC(&draw);
    draw.clip.x = 0;
    draw.clip.y = 0;
    draw.clip.w = 320;
    draw.clip.h = 240;
    func_8009CC04(&draw);
    func_8009D10C(&disp);
    disp.disp.x = 1;
    disp.disp.y = 1;
    disp.disp.w = 320;
    disp.disp.h = 240;
    func_8009CCF4(&disp);
}

void func_8004111C(void) {
    RECT rect;

    rect.w = 640; rect.h = 480; /* FAKE: both on one source line, as1 then stores h before w */
    rect.x = 0;
    rect.y = 0;
    func_8009C7F8(&rect, 0, 0, 0);
    func_8009C674(0);
}

void func_80041168(s32 arg0) {
    s32 i;
    s32 j;
    s32 k;
    RECT rect;

    if (arg0 == 0xFF) {
        rect.x = 0;
        rect.y = 0;
        rect.w = 0x40;
        rect.h = 0x80;
        func_8009C7F8(&rect, 0, 0, 0);
        i = 1;
        do {
            func_80041168(i);
            i++;
        } while (i != 0x40);
    } else {
        j = 0;
        if (arg0 == 0xFE) {
            do {
                func_80041168(0xF - j);
                func_80041168(0x2F - j);
                func_80041168(j + 0x15);
                func_80041168(j + 0x35);
                j++;
            } while (j != 5);
        } else if (arg0 == 0) {
            k = 0;
            do {
                if (((k & 0xF) >= 5) && ((k & 0xF) < 0xF) && (k < 0x16)) {
                    func_80041168(k);
                    func_80041168(k + 0x20);
                }
                k++;
            } while (k != 0x20);
        } else {
            rect.x = (arg0 & 0xF) << 6;
            rect.y = ((arg0 & 0x10) << 4) + ((arg0 & 0x20) << 2);
            rect.w = 0x40;
            rect.h = 0x80;
        }
    }
    func_8009C7F8(&rect, 0, 0, 0);
    func_8009C674(0);
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E6282;
extern s16 D_800E6284;
extern s16 D_800E6294;
extern s16 D_800E6296;
extern s16 D_800E6298;
extern s16 D_800E629A;
extern s32 D_800E6344;
extern s32 D_800E6354;
extern s32 D_800E635C;
extern s32 D_800E6364;
extern s32 D_800E7200;
extern s32 D_800E7204;
extern s32 D_800E7208;
extern s32 D_800E7318;
extern s32 D_800E7368;
extern s32 D_800E7370;
extern s32 D_800E7378;
extern s32 D_800E737C;
extern s32 D_800E7380;
extern s32 D_800E7384;
extern s8 D_800E62B4;
extern s8 D_800E62B6;
extern s8 D_800E62B7;
extern s8 D_800E62B8;
extern s8 D_800E62B9;
extern s8 D_800E7388;
extern s8 D_800E7389;
extern s8 D_800E738A;
extern s8 D_800E738F;
extern s8 D_800E7390;
extern s8 D_800E7393;
extern s8 D_800E7394;
extern u8 D_800E6378;
void func_80041840(void);
/* end bulk_m2c declarations */
void func_800412E0(s32 arg0) {
    func_8009C210(0);
    if (arg0 != 0) {
        func_80041168(0xFF);
        bzero(&D_800E6280, 0x1A8C);
    }
    D_800E6280.unk_000 = 0x140;
    D_800E6282 = 0xF0;
    D_800E6284 = 0;
    D_800E62B6 = 0;
    D_800E62B7 = 0;
    D_800E62B8 = 0;
    D_800E6294 = 0xA0;
    D_800E6296 = 0xA0;
    D_800E6298 = 0x168;
    D_800E629A = 0x78;
    D_800E7378 = 0;
    D_800E737C = 0;
    D_800E7380 = 0;
    D_800E7384 = 0;
    D_800E7388 = 0x10;
    D_800E7389 = 0;
    D_800E738A = 0;
    D_800E7208 = 0;
    D_800E7204 = 0;
    D_800E7200 = 0;
    D_800E7390 = 0;
    D_800E738F = 0;
    D_800E7370 = 0x106;
    D_800E7318 = 0x9F;
    func_8004902C();
    D_800E62B9 = 0;
    D_800E7393 = 0;
    D_800E7394 = 0;
    D_800E62B4 = 1;
    tpage_buf_clear();
    search_tpage(0x7FFF, 0, 0);
    D_800E7368 = 1;
    func_80041584();
    func_80048EB8(0);
    dec_bg_reset();
    func_8004482C();
    menu_bar_color(0x10, 0, 0, 8);
    func_80041840();
    strncpy((s32) &D_800E6344, (s32) "ときめき", 0x10);
    strncpy((s32) &D_800E6354, (s32) "アルバ", 8);
    strncpy((s32) &D_800E635C, (s32) "ム作成", 8);
    strncpy((s32) &D_800E6364, (s32) "後未セーブ", 0x10);
    D_800E6378 = (D_800E6378 & 0xFFF0) | 1;
    D_800E6378 = (s16) (((u16) D_800E6378 & 0xFE0F) | 0x10);
    func_8006BC28(0);
    func_8006BD6C(0);
    func_8006C848(1);
    func_8006C848(0);
    func_80041878();
    func_80054C4C();
    bzero(D_80122DD0, 0x100);
    bzero(&D_80122CD0, 0x100);
    bzero(&D_801230D0, 0x40);
    func_80042058();
}

void func_80041584(void) {
    func_800415B4(0, 0x40);
    func_8004164C(0, 2);
}

/* One 36-byte sprite entry of the table at D_801217D0 (main_api.h declares only its first word).
 * Indexing the table as an array of this struct keeps IDO from unrolling the loop below, as in the
 * original (T-5020, wiki/matching-notes.md "Loop unrolling"). */
typedef struct SprEnt {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ u8 unk_0E;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ s16 unk_12;
    /* 0x14 */ u8 unk_14;
    /* 0x15 */ u8 unk_15;
    /* 0x16 */ u8 unk_16;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s32 unk_20;
} SprEnt; /* size 0x24 */

void func_800415B4(s32 start, s32 end) {
    s32 i;

    for (i = start; i < end; i++) {
        ((SprEnt *)&D_801217D0)[i].unk_00 = 0x80000000;
        ((SprEnt *)&D_801217D0)[i].unk_04 = 0;
        ((SprEnt *)&D_801217D0)[i].unk_06 = 0;
        ((SprEnt *)&D_801217D0)[i].unk_08 = 0;
        ((SprEnt *)&D_801217D0)[i].unk_0A = 0;
        ((SprEnt *)&D_801217D0)[i].unk_0C = 0;
        ((SprEnt *)&D_801217D0)[i].unk_0E = 0;
        ((SprEnt *)&D_801217D0)[i].unk_0F = 0;
        ((SprEnt *)&D_801217D0)[i].unk_10 = 0;
        ((SprEnt *)&D_801217D0)[i].unk_12 = 0;
        ((SprEnt *)&D_801217D0)[i].unk_14 = 0;
        ((SprEnt *)&D_801217D0)[i].unk_15 = 0;
        ((SprEnt *)&D_801217D0)[i].unk_16 = 0;
        ((SprEnt *)&D_801217D0)[i].unk_18 = 0;
        ((SprEnt *)&D_801217D0)[i].unk_1A = 0;
        ((SprEnt *)&D_801217D0)[i].unk_1C = 0;
        ((SprEnt *)&D_801217D0)[i].unk_1E = 0;
        ((SprEnt *)&D_801217D0)[i].unk_20 = 0;
        D_800E6280.unk_10A5[i] = 0;
    }
}

void func_8004164C(s32 arg0, s32 arg1) {
    s32 i;
    GameState *g;

    g = &D_800E6280;
    for (i = arg0; i < arg1; i++) {
        ((SpriteSlot24 *)&D_801220D0)[i].unk_00 = 0x80000000;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_04 = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_06 = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_08 = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_0A = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_0C = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_0E = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_10 = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_11 = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_12 = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_14 = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_18 = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_1A = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_1C = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_1E = 0;
        ((SpriteSlot24 *)&D_801220D0)[i].unk_20 = 0;
        g->unk_10EC[i] = 0x80;
    }
}

void func_80041840(void) {
    D_800E6280.unk_F74 = 1;
    D_800E6280.unk_F6F = 3;
    D_800E6280.unk_1092 = 1;
    D_80123110 = 0x80132000;
}

void func_80041878(void) {
    func_800418B0();
    func_800419FC();
    func_80041C2C();
    func_80041F48();
}

void func_800418B0(void) {
    D_800E6280.unk_03D = 0;
    D_800E6280.unk_03E = 0x5F;
    D_800E6280.unk_03F = 4;
    D_800E6280.unk_040 = 4;
    D_800E6280.unk_041 = 1;
    D_800E6280.unk_042 = 0;
    bzero(D_800E6280.unk_044[0].unk_00, 0x80);
    bzero(D_800E6280.unk_66C, 0x30);
    bzero(D_800E6280.unk_F64, 4);
    D_800E6280.unk_044[0].unk_00[0] = 0x1E;
    D_800E6280.unk_044[0].unk_20[0] = 5;
    D_800E6280.unk_044[0].unk_00[2] = 1;
    D_800E6280.unk_044[0].unk_00[9] = 1;
    D_800E6280.unk_044[0].unk_00[16] = 1;
    D_800E6280.unk_044[0].unk_00[23] = 1;
    D_800E6280.unk_044[0].unk_00[29] = 2;
    D_800E6280.unk_044[0].unk_00[30] = 1;
    D_800E6280.unk_044[0].unk_20[4] = 1;
    D_800E6280.unk_044[1].unk_00[0] = 0x1F;
    D_800E6280.unk_044[1].unk_20[0] = 0;
    D_800E6280.unk_044[1].unk_00[7] = 1;
    D_800E6280.unk_044[1].unk_00[14] = 1;
    D_800E6280.unk_044[1].unk_00[21] = 1;
    D_800E6280.unk_044[1].unk_00[28] = 1;
    D_800E6280.unk_044[1].unk_00[3] = 2;
    D_800E6280.unk_044[1].unk_00[4] = 2;
    D_800E6280.unk_044[1].unk_00[5] = 2;
}

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_800419FC);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041C2C);

void func_80041F48(void) {
    s32 i;

    bzero(D_800E6280.unk_54C, 0x20);
    bzero(D_800E6280.unk_66C, 0x30);
    for (i = 0; i < 32; i++) {
        D_800E6280.unk_71C = 0;
        ((GsWord *)&D_800E6280.unk_69C[i])->w = 0;
    }
    for (i = 0; i < 256; i++) {
        ((StateClearBytes100 *)D_800E6280.unk_56C)->bytes[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        ((GsWord *)&D_800E6280.unk_724)->b[i] = 0;
    }
    D_800E6280.unk_75D = 0;
    for (i = 0; i < 4; i++) {
        D_800E6280.unk_728[i] = 0;
    }
    i = 0;
    while (i < 16) {
        D_800E6280.unk_72C[i] = 0;
        i++;
    }
    D_800E6280.unk_75C = 0;
    D_800E6280.unk_F5E = 0;
    bzero(D_800E6280.unk_75E, 0x800);
}

void func_80042058(void) {
    D_800E6280.unk_F5F = 0;
    D_800E6280.unk_F60 = 0;
    D_800E6280.unk_F61 = 0;
    D_800E6280.unk_F62 = 0;
    D_800E6280.unk_F63 = 0;
    D_800E6280.unk_F6C = 0;
    D_800E6280.unk_03C = 2;
    D_800E6280.unk_F70 = 0;
    D_800E6280.unk_F71 = 0;
    D_800E6280.unk_F73 = 0x20;
    D_800E6280.unk_F75 = 0;
    D_800E6280.unk_F6D = 0xE;
    D_800E6280.unk_F6E = 0;
}
