#include "common.h"
#include "ovl/ETC.h"

void func_8013BEF0(void) {
    D_80150030 = (u8 *)0x8019DCFC;
    D_80150034 = (u8 *)0x8019DD00;
    D_80150038 = (u8 *)0x8019DDC0;
    D_8015003C = *(s16 *)0x8019DDF8;
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013BF34);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013BFFC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013C330);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013C700);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013C8EC);

void func_8013CC74(void) {
    if ((D_80150054 == 0 && D_80150058 == 0 && D_8015005C == 0) || D_80150064 == 0xFF) {
        func_80042808();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013CCE4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013CF2C);

void func_8013D264(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = D_8011ECA0 * 0xF0;
    rect.w = 0x140;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x140, 0x100);
    func_8009C674(0);
}

void func_8013D2C0(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = (1 - D_8011ECA0) * 0xF0;
    rect.w = 0x140;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0, D_8011ECA0 * 0xF0);
    func_8009C674(0);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013D32C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013D570);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013DEA0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013E094);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013E1DC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013E364);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013E524);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013E6AC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013E89C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013EAC0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013EB90);

void func_8013ECDC(void) {
    func_801408F0();
    if (D_8014F990 == 1) {
        func_8013ED8C();
    } else if (D_8015007C == 1) {
        func_80136448();
    }
}

void func_8013ED38(void) {
    D_80150084 = (u8 *)0x80180084;
    D_80150088 = (u8 *)0x80180098;
    D_8015008C = (u8 *)0x8018009C;
    D_80150090 = *(s16 *)0x801960AC;
    D_80150094 = (u8 *)0x801800AC;
}

void func_8013ED8C(void) {
    func_8013ED38();
    func_800415B4(0, 0x40);
    func_80062DBC(D_80150094, D_80150084, D_80150088, D_8015008C, D_80150048);
    func_8013E094(D_80150088);
}

void func_8013EDF8(void) {
    if (D_8014F990 == 1) {
        func_80063668();
    }
}

void func_8013EE28(void) {
    if (D_8014F990 == 1) {
        func_8013EEAC();
    } else if (D_8015007C == 1) {
        func_8013EE7C();
    }
}

void func_8013EE7C(void) {
    D_8015007C = 0;
    D_801220D0 = 0x80000000;
    D_80120653 = 0;
    D_80120697 = 0;
    D_801206DB = 0;
}

void func_8013EEAC(void) {
    D_8011F50F = 0;
    D_8011F4CB = 0;
    D_80120653 = 0;
    D_801217D0[0].unk_00 = 0x80000000;
    D_801217D0[1].unk_00 = 0x80000000;
    D_801217D0[2].unk_00 = 0x80000000;
    D_801217D0[3].unk_00 = 0x80000000;
    D_801217D0[4].unk_00 = 0x80000000;
    D_801217D0[5].unk_00 = 0x80000000;
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013EEFC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013F054);

void func_8013F320(s32 arg0) {
    switch (D_8015006C) {
    case 0:
        func_80046290(D_801500C4[arg0 * 2], D_801500C4[arg0 * 2 + 1], D_800E6280.unk_F5F);
        func_80044750(0x300);
        D_8015006C += 1;
        break;
    case 1:
        if (func_800460EC() & 4) {
            D_8015006C += 1;
        }
        break;
    case 2:
        if (func_800460EC() & 2) {
            D_8015006C += 1;
        }
        break;
    }
}

s32 func_8013F3F8(void) {
    if (D_80150040 == 0x15 &&
        (D_80150044 == 6 || D_80150044 == 7 || D_80150044 == 8 || D_80150044 == 9 || D_80150044 == 10)) {
        if ((D_8015004C == 0 || D_8015004C == 1 || D_8015004C == 4 || D_8015004C == 6 || D_8015004C == 9 ||
             D_8015004C == 10) &&
            D_80150048 == 5) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013F498);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013F77C);
