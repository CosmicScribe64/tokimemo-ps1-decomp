#include "common.h"
#include "ovl/TAIIKU.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3B0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3B4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3B8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3D0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3D4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3D8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3DC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3E0);
TaiikuController D_8014A3E4 = { 0 };
TaiikuController D_8014A400[3] = { 0 };
TaiikuSlot1C D_8014A454[4] = { 0 };

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4D4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4D8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4E4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4E8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4F4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4F8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A528);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A52C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A53C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A54C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A56C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A570);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A5A0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A5C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A5DC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A5F4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A5FC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A608);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A614);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A620);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A634);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A648);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A658);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A668);

typedef struct {
    void (*f[8])();
} FnTbl8; /* size 0x20 */
extern FnTbl8 D_8014A54C;

typedef struct {
    s32 v[3];
} Lim3; /* size 0xC */

typedef struct TkPl {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ u8 pad4[2];
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 unk_0D;
    /* 0x0E */ u8 pad0E[6];
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ u8 pad18[4];
} TkPl; /* size 0x1C */
extern Lim3 D_8014A5FC;
extern Lim3 D_8014A608;

void func_801452D0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl8 tbl;

    tbl = D_8014A54C;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80145370);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80145640);

void func_80145A78(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        if (func_80086370() != 0) {
            func_80085ED0();
            func_8004284C();
        }
        func_80133438();
        break;
    case 1:
        if (func_80085EEC() >= 0x5B) {
            func_80044750(0x502);
            func_8004284C();
        }
        func_80133438();
        break;
    case 2:
        func_8014666C();
        func_8014684C();
        func_80146C20();
        func_80132E20(D_8014A3DE, 4);
        func_80147068();
        func_80147520();
        func_80147F10();
        if (func_80146D18() != 0) {
            func_80044750(0x504);
            func_800862D0();
            func_8004284C();
        }
        break;
    case 3:
        func_80085F0C(D_8014A3DE, 0xF, 0, 0x70, 0x280);
        func_80085F0C(D_8014A3DE % 128, 0xF, 0x70, 0x80, 0x80);
        if (func_800862F0() != 0) {
            func_80044750(0xC2);
            func_80148370();
            func_80133258();
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}

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

s32 func_8014666C(void) {
    TaiikuController *controller;
    s32 i;

    controller = &D_8014A3E4;
    controller->unk_0C++;
    controller->unk_08++;
    controller->unk_04++;
    controller->unk_02++;
    i = 0;
    /* the pointer bound keeps the original's sltu and lui order; `i < 3` gives bne (T-16553) */
    do {
        D_8014A400[i].unk_0C++;
        D_8014A400[i].unk_04++;
        i++;
    } while (&D_8014A400[i] < (TaiikuController *)D_8014A454);
    D_8014A454[0].unk_18++;
    D_8014A454[0].unk_14++;
    D_8014A454[0].unk_16++;
    D_8014A454[0].unk_0D++;
    D_8014A454[0].unk_0F++;
    D_8014A454[1].unk_18++;
    D_8014A454[1].unk_14++;
    D_8014A454[1].unk_16++;
    D_8014A454[1].unk_0D++;
    D_8014A454[1].unk_0F++;
    D_8014A454[2].unk_18++;
    D_8014A454[2].unk_14++;
    D_8014A454[2].unk_16++;
    D_8014A454[2].unk_0D++;
    D_8014A454[2].unk_0F++;
    D_8014A454[3].unk_18++;
    D_8014A454[3].unk_14++;
    D_8014A454[3].unk_16++;
    D_8014A454[3].unk_0D++;
    D_8014A454[3].unk_0F++;
}

void func_8014684C(void) {
    func_80146884();
    func_8014692C();
    func_80146994();
    func_80146A60();
}

void func_80146884(void) {
    if (D_8014A454[0].unk_0C != 0) {
        D_8014A3E4.unk_10 = -0x300;
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        D_8014A3C4 = 1;
        return;
    }
    if (D_8014A3E4.unk_08 >= 0x10) {
        if (D_8014A3C4 != 0) {
            if (D_8014A3E4.unk_14 < 0) {
                D_8014A3E4.unk_14 = 0;
            }
            D_8014A3E4.unk_10 = D_8014A3D0;
        } else {
            D_8014A3E4.unk_10 = -0x300;
        }
        D_8014A3E4.unk_08 = 0;
        D_8014A3C4 = 0;
    }
}

void func_8014692C(void) {
    if ((u32)D_8014A3E4.unk_0C >= 0xB) {
        D_8014A3E4.unk_14 += D_8014A3E4.unk_10;
        if (D_8014A3E4.unk_14 < -0x10000) {
            D_8014A3E4.unk_14 = -0x10000;
        }
        if (D_8014A3D4 < D_8014A3E4.unk_14) {
            D_8014A3E4.unk_14 = D_8014A3D4;
        }
        D_8014A3E4.unk_0C = 0;
    }
}

void func_80146994(void) {
    if ((D_80120668 != 3) && ((D_8014A454[0].unk_0C == 0) || (D_8014A3E0 != 0))) {
        if ((D_8014A3E4.unk_10 < 0) || (D_8014A3DE == 0)) {
            D_8014A3E4.position.w -= D_8014A3E0;
        } else {
            D_8014A3E4.position.w += 0x8000;
        }
    }
    if (D_8014A3E4.position.h[1] >= -0x1F) {
        D_8014A3E4.position.h[1] = -0x20;
    }
    if (D_8014A3E4.position.h[1] < -0x9F) {
        D_8014A3E4.position.h[1] = -0xA0;
    }
    D_80120676 = D_8014A3E4.position.h[1];
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80146A60);

void func_80146B8C(void) {
    if (D_8014A3E0 == 0) {
        D_8014A3E4.unk_00 = 0;
        return;
    }
    if (((u32)D_8014A3E4.unk_04 >= 6 && D_8014A3E4.unk_10 > 0) || ((u32)D_8014A3E4.unk_04 >= 0xB && D_8014A3E4.unk_14 != 0)) {
        if (D_8014A3E4.unk_00 >= 7) {
            D_8014A3E4.unk_00 = 2;
            D_8014A3E4.unk_04 = 0;
        } else {
            D_8014A3E4.unk_00++;
            D_8014A3E4.unk_04 = 0;
        }
    }
}

void func_80146C20(void) {
    D_8014A3E0 += D_8014A3E4.unk_10;
    if (D_8014A3D4 < D_8014A3E0) {
        D_8014A3E0 = D_8014A3D4;
    }
    if (D_8014A3E0 < 0 || (D_8014A3E4.unk_10 < 0 && D_8014A3E4.position.h[1] < -0x9F)) {
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
        if (D_8014A400[i].unk_01 == 0) {
            v[i] = *(s16 *)(D_8011ECD0 + i * 0x44 + 0x19EA);
        } else {
            v[i] = -0xF00;
        }
    }
    if (v[0] >= v[1] && v[0] >= v[2]) {
        D_8014A400[0].unk_01 = 1;
        return 2;
    }
    if (v[1] >= v[2]) {
        D_8014A400[1].unk_01 = 1;
        return 3;
    }
    D_8014A400[2].unk_01 = 1;
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
        if (D_8014A454[i + 1].unk_0C != 0) {
            D_8014A400[i].unk_10 = -0x4000;
        } else {
            D_8014A400[i].unk_10 = lim.v[i];
        }
    }
}

void func_80147120(void) {
    s32 i;
    Lim3 lim;

    lim = D_8014A608;
    for (i = 0; i < 3; i++) {
        if (D_8014A400[i].unk_01 == 1 && D_80120896 + 0x140 < *(s16 *)(D_8011ECD0 + i * 0x44 + 0x19EA)) {
            D_8014A400[i].unk_14 = 0;
        } else if (D_8014A400[i].unk_0C >= 0xB) {
            D_8014A400[i].unk_14 += D_8014A400[i].unk_10;
            if (D_8014A400[i].unk_14 < 0) {
                D_8014A400[i].unk_14 = 0;
            }
            if (lim.v[i] < D_8014A400[i].unk_14) {
                D_8014A400[i].unk_14 = lim.v[i];
            }
            D_8014A400[i].unk_0C = 0;
        }
    }
}


void func_80147210(void) {
    s32 i;
    s32 k;
    TaiikuController *v;
    u8 *a;
    s32 d;
    s32 x;

    for (i = 0, v = D_8014A400, a = D_8011ECD0, d = D_8014A3DE; i < 3; i++, v++, a += 0x44) {
        v->position.w += v->unk_14;
        x = v->position.h[1] - d;
        *(s16 *)(a + 0x19EA) = x - 0xA0;
        if (D_80120896 + (i + 1) * 8 < *(s16 *)(a + 0x19EA) + 0x40 && v->unk_01 == 0) {
            D_801208CC = 0x4F;
            k = 0;
            if (D_8014A3D8[0] != 0) {
                do {
                    k += 1;
                } while (D_8014A3D8[k] != 0);
            }
            D_8014A3D8[k] = i + 2;
            v->unk_01 = 1;
            x = v->position.h[1] - d;
        }
        if (x >= 0x1E1 || x < -0xA0) {
            a[0x19C7] = 0;
        } else {
            a[0x19C7] = 0xA4;
        }
    }
}

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

void func_80147A6C(s32 index) {
    TaiikuSlot1C *slot;

    slot = &D_8014A454[index];
    /* FAKE: this empty test preserves the slot pointer's register range. T-10020. */
    if (!slot) {
    }
    if (slot->unk_0C == 0 && (index != 0 || D_8014A3E4.unk_10 > 0 || D_8014A3E0 != 0) && slot->unk_16 >= 6) {
        if (slot->unk_08 > 0) {
            slot->unk_02++;
            slot->unk_16 = 0;
        }
        if (slot->unk_08 < 0) {
            slot->unk_16 = 0;
            slot->unk_02--;
        }
    }
}

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
    TaiikuSlot1C *temp_v1;

    temp_v1 = &D_8014A454[arg0];
    temp_v0 = (arg0 * 0x44) + D_8011ECD0;
    temp_v1->unk_0C = 1;
    temp_v1->unk_0D = 0;
    *(s16 *)(temp_v0 + 0x1998) = (s16) ((arg0 * 2) + 0x51);
    *(s16 *)(temp_v0 + 0x1AB6) = (s16) (*(s16 *)(temp_v0 + 0x19A6) + 0x74);
    *(s16 *)(temp_v0 + 0x1ABA) = (s16) (*(s16 *)(temp_v0 + 0x19AA) + 0x32);
    temp_v1->unk_02 = 0x6A;
    temp_v1->unk_06 = 0x32;
    if (arg0 == 0) {
        func_80044750(0x503);
    }
}

void func_80148228(s32 arg0) {
    u8 *temp_v0;
    TaiikuSlot1C *temp_v1;

    temp_v1 = &D_8014A454[arg0];
    temp_v0 = (arg0 * 0x44) + D_8011ECD0;
    temp_v1->unk_0C = 2;
    temp_v1->unk_0D = 0;
    *(s16 *)(temp_v0 + 0x1998) = (s16) ((arg0 * 2) + 0x52);
    *(s16 *)(temp_v0 + 0x1AB6) = (s16) (*(s16 *)(temp_v0 + 0x19A6) - 0x17);
    *(s16 *)(temp_v0 + 0x1ABA) = (s16) (*(s16 *)(temp_v0 + 0x19AA) + 0x32);
    temp_v1->unk_02 = -0x17;
    temp_v1->unk_06 = 0x32;
    if (arg0 == 0) {
        func_80044750(0x503);
    }
}

void func_801482BC(s32 index) {
    TaiikuController *controller;
    TaiikuSlot1C *slot;

    slot = &D_8014A454[index];
    if (slot->unk_0C != 0 && slot->unk_0D >= 0x65U) {
        slot->unk_0C = 0;
        if (index == 0) {
            D_8014A3E4.unk_00 = 0;
            D_8014A3E4.unk_08 = 0;
            D_8014A3E4.unk_0C = 0;
            D_8014A3E4.unk_04 = 0;
            D_8014A3E4.unk_14 = 0;
        } else {
            controller = &D_8014A400[index - 1];
            controller->unk_00 = 0;
            controller->unk_08 = 0;
            controller->unk_0C = 0;
            controller->unk_04 = 0;
            controller->unk_14 = 0;
        }
        slot->unk_02 = 0x37;
        slot->unk_06 = 0x19;
        slot->unk_08 = 0;
        slot->unk_14 = 0;
        slot->unk_16 = 0;
        slot->unk_0D = 0;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80148370);

s32 func_801485C4(void) {
    s32 index;
    s32 chosen;
    u8 *p;

    p = D_8014A3D8;
    index = 0;
    do {
        if (*p == 1) {
            chosen = index;
        }
        index++;
        p++;
    } while (index < 4);
    if (chosen * 48 - 64 < ((TaiikuScene44 *)D_8011ECD0)[104].position.h[1]) {
        ((TaiikuScene44 *)D_8011ECD0)[104].position.w += 0xFFFE8000;
        return 0;
    }
    ((TaiikuScene44 *)D_8011ECD0)[104].unk_02 = 64;
    ((TaiikuScene44 *)D_8011ECD0)[104].unk_18 = 61;
    func_80085ED0();
    return 1;
}

s32 func_80148678(void) {
    s32 i;
    s32 n;

    if (func_80085EEC() >= 0x2E) {
        *(s16 *)&D_8011ECD0[0x1BB8] = 0x3C;
        *(s16 *)&D_8011ECD0[0x1BCA] = -8;
        for (i = 0; i < 4; i++) {
            if (D_8014A3D8[i] == 1) {
                n = i;
            }
        }
        for (i = 0; i < 4; i++) {
            if (n < i) {
                *(s16 *)&D_8011ECD0[0x19A6 + i * 0x44] += 0x30;
                *(s16 *)&D_8011ECD0[0x19A6 + 0x110 + i * 0x44] += 0x30;
            }
        }
        return 1;
    }
    return 0;
}

void func_801487B4(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        *(s16 *)&D_8011ECD0[0x1AA8 + i * 0x44] -= 4 - i;
    }
    func_80085ED0();
}

void func_80148820(void) {
    s32 i;
    s32 n;

    for (i = 0; i < 4; i++) {
        if (D_8014A3D8[i] == 1) {
            n = i;
        }
    }
    if (func_80085EEC() >= 0x33) {
        for (i = 0; i < 4; i++) {
            *(s16 *)&D_8011ECD0[0x1998 + i * 0x44] += i + 1;
        }
        *(s16 *)&D_8011ECD0[0x1BB8] = *(s16 *)&D_8011ECD0[0x1BB8] + n + 2;
        func_8004284C();
    }
}
