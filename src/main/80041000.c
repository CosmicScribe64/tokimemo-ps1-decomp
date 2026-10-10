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

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_800410AC);

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
        D_800E6280[0x10A5 + i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_8004164C);

void func_80041840(void) {
    D_800E71F4 = 1;
    D_800E71EF = 3;
    D_800E7312 = 1;
    D_80123110 = 0x80132000;
}

void func_80041878(void) {
    func_800418B0();
    func_800419FC();
    func_80041C2C();
    func_80041F48();
}

void func_800418B0(void) {
    D_800E62BD = 0;
    D_800E62BE = 0x5F;
    D_800E62BF = 4;
    D_800E62C0 = 4;
    D_800E62C1 = 1;
    D_800E62C2 = 0;
    bzero(D_800E62C4, 0x80);
    bzero(D_800E68EC, 0x30);
    bzero(D_800E71E4, 4);
    D_800E62C4[0] = 0x1E;
    D_800E62E4[0] = 5;
    D_800E62C6 = 1;
    D_800E62CD = 1;
    D_800E62D4 = 1;
    D_800E62DB = 1;
    D_800E62E1 = 2;
    D_800E62E2 = 1;
    D_800E62E8 = 1;
    D_800E6304 = 0x1F;
    D_800E6324 = 0;
    D_800E630B = 1;
    D_800E6312 = 1;
    D_800E6319 = 1;
    D_800E6320 = 1;
    D_800E6307 = 2;
    D_800E6308 = 2;
    D_800E6309 = 2;
}

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_800419FC);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041C2C);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041F48);

void func_80042058(void) {
    D_800E71DF = 0;
    D_800E71E0 = 0;
    D_800E71E1 = 0;
    D_800E71E2 = 0;
    D_800E71E3 = 0;
    D_800E71EC = 0;
    D_800E62BC = 2;
    D_800E71F0 = 0;
    D_800E71F1 = 0;
    D_800E71F3 = 0x20;
    D_800E71F5 = 0;
    D_800E71ED = 0xE;
    D_800E71EE = 0;
}
