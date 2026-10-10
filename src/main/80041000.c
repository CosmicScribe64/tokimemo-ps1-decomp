#include "common.h"
#include "game.h"
#include "libapi.h"

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

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, store order: the original stores h, w before x, y. */
void func_8004111C(void) {
    RECT rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = 640;
    rect.h = 480;
    func_8009C7F8(&rect, 0, 0, 0);
    func_8009C674(0);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80041000", func_8004111C);
#endif

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041168);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_800412E0);

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

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_8004164C);

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

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041F48);

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
