#include "common.h"
#include "ovl/TAIIKU.h"

typedef struct {
    s32 v[3];
} Lim3; /* size 0xC */
extern Lim3 D_8014A5FC;
extern u8 D_8014A400[];
extern u8 D_8014A400[]; /* func_80146FA0: stride 0x1C */
extern u8 D_8014A401;
extern u8 D_8014A41D;
extern u8 D_8014A439;

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_801452D0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80145370);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80145640);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80145A78);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80145C28);

void func_80145EF4(void) {
    s32 i;

    func_8004E44C(0, D_80148EF8, D_80148EF0);
    srn_init(0, D_80148EFC, 0);
    srn_vram_set(0, 5, 0x580, 0xF0);
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 5] = 8;
    }
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 0x25] = 8;
    }
}

void func_80145F8C(void) {
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120652 = 1;
    D_80120653 = 0xA4;
    D_80120657 = 0x80;
    D_8012065C = (u8 *) D_80149FD0;
    D_80120660 = (u8 *) D_8014A008;
    D_80120684 = &D_8014A4F4;
    D_80120664 = D_8014A040;
    D_80120654 = 0;
    D_80120655 = 0x80;
    D_80120666 = 0;
    D_80120676 = 0x20;
    D_8012067A = 0x10;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_8014607C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80146338);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_8014666C);

void func_8014684C(void) {
    func_80146884();
    func_8014692C();
    func_80146994();
    func_80146A60();
}

void func_80146884(void) {
    if (D_8014A460 != 0) {
        D_8014A3F4 = -0x300;
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        D_8014A3C4 = 1;
        return;
    }
    if (D_8014A3EC >= 0x10) {
        if (D_8014A3C4 != 0) {
            if (D_8014A3F8 < 0) {
                D_8014A3F8 = 0;
            }
            D_8014A3F4 = D_8014A3D0;
        } else {
            D_8014A3F4 = -0x300;
        }
        D_8014A3EC = 0;
        D_8014A3C4 = 0;
    }
}

void func_8014692C(void) {
    if ((u32)D_8014A3F0 >= 0xB) {
        D_8014A3F8 += D_8014A3F4;
        if (D_8014A3F8 < -0x10000) {
            D_8014A3F8 = -0x10000;
        }
        if (D_8014A3D4 < D_8014A3F8) {
            D_8014A3F8 = D_8014A3D4;
        }
        D_8014A3F0 = 0;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80146994);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80146A60);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80146B8C);

void func_80146C20(void) {
    D_8014A3E0 += D_8014A3F4;
    if (D_8014A3D4 < D_8014A3E0) {
        D_8014A3E0 = D_8014A3D4;
    }
    if (D_8014A3E0 < 0 || (D_8014A3F4 < 0 && D_8014A3FE < -0x9F)) {
        D_8014A3E0 = 0;
    }
    D_8014A3DC += D_8014A3E0; /* the fixed-point high half (+2) is read as an s16 below */
    func_80085F0C(((s16 *)&D_8014A3DC)[1] / 2 % 640, 0xF, 0, 0x70, 0x280);
    func_80085F0C(((s16 *)&D_8014A3DC)[1] % 128, 0xF, 0x70, 0x80, 0x80);
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80146D18);

/* FAKE: v[6] as in func_8013815C. T-0016 */
s32 func_80146FA0(void) {
    s16 v[6];
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_8014A400[i * 0x1C + 1] == 0) {
            v[i] = *(s16 *)(D_8011ECD0 + i * 0x44 + 0x19EA);
        } else {
            v[i] = -0xF00;
        }
    }
    if (v[0] >= v[1] && v[0] >= v[2]) {
        D_8014A401 = 1;
        return 2;
    }
    if (v[1] >= v[2]) {
        D_8014A41D = 1;
        return 3;
    }
    D_8014A439 = 1;
    return 4;
}

void func_80147068(void) {
    func_801470A0();
    func_80147120();
    func_80147210();
    func_80147318();
}

void func_801470A0(void) {
    s32 i;
    Lim3 lim;

    lim = D_8014A5FC;
    for (i = 0; i < 3; i++) {
        if (D_8014A454[i * 0x1C + 0x28] != 0) {
            *(s32 *)(D_8014A400 + i * 0x1C + 0x10) = -0x4000;
        } else {
            *(s32 *)(D_8014A400 + i * 0x1C + 0x10) = lim.v[i];
        }
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80147120);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80147210);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80147318);

void func_80147520(void) {
    s32 var_s0;

    func_8014756C();
    var_s0 = 0;
    do {
        func_80147A6C(var_s0);
        func_80147B14(var_s0);
        var_s0 += 1;
    } while (var_s0 != 4);
}

void func_8014756C(void) {
    func_80147594();
    func_80147928();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80147594);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80147928);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80147A6C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80147B14);

void func_80147F10(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        func_80147F58(var_s0);
        func_801482BC(var_s0);
        var_s0 += 1;
    } while (var_s0 != 4);
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80147F58);

void func_80148194(s32 arg0) {
    u8 *temp_v0;
    u8 *temp_v1;

    temp_v1 = (arg0 * 0x1C) + D_8014A454;
    temp_v0 = (arg0 * 0x44) + D_8011ECD0;
    *(u8 *)(temp_v1 + 0xC) = 1;
    *(u8 *)(temp_v1 + 0xD) = 0;
    *(s16 *)(temp_v0 + 0x1998) = (s16) ((arg0 * 2) + 0x51);
    *(s16 *)(temp_v0 + 0x1AB6) = (s16) (*(s16 *)(temp_v0 + 0x19A6) + 0x74);
    *(s16 *)(temp_v0 + 0x1ABA) = (s16) (*(s16 *)(temp_v0 + 0x19AA) + 0x32);
    *(s16 *)(temp_v1 + 0x2) = 0x6A;
    *(s16 *)(temp_v1 + 0x6) = 0x32;
    if (arg0 == 0) {
        func_80044750(0x503);
    }
}

void func_80148228(s32 arg0) {
    u8 *temp_v0;
    u8 *temp_v1;

    temp_v1 = (arg0 * 0x1C) + D_8014A454;
    temp_v0 = (arg0 * 0x44) + D_8011ECD0;
    *(u8 *)(temp_v1 + 0xC) = 2;
    *(u8 *)(temp_v1 + 0xD) = 0;
    *(s16 *)(temp_v0 + 0x1998) = (s16) ((arg0 * 2) + 0x52);
    *(s16 *)(temp_v0 + 0x1AB6) = (s16) (*(s16 *)(temp_v0 + 0x19A6) - 0x17);
    *(s16 *)(temp_v0 + 0x1ABA) = (s16) (*(s16 *)(temp_v0 + 0x19AA) + 0x32);
    *(s16 *)(temp_v1 + 0x2) = -0x17;
    *(s16 *)(temp_v1 + 0x6) = 0x32;
    if (arg0 == 0) {
        func_80044750(0x503);
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_801482BC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80148370);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_801485C4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80148678);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_801487B4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80148820);
