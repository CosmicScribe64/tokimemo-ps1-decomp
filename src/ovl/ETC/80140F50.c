#include "common.h"
#include "ovl/ETC.h"

u8 *func_80140F50(void) {
    if (D_800E6280.unk_F74 == 0) {
        if (D_800E6280.unk_F75 != 0) {
            return D_80150128;
        }
        return D_8015012C;
    }
    return D_80150108;
}

u8 *func_80140F94(void) {
    if (D_800E6280.unk_F74 == 0) {
        if (D_800E6280.unk_F75 != 0) {
            return D_8015012C;
        }
        return D_80150128;
    }
    return D_80150124;
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80140FD8);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80141154);

void func_801413FC(void) {
    func_800AE0B0("データセーブ");
    if (D_800E6280.unk_1124 != 0) {
        func_8004482C();
        func_80042878(0x12);
        return;
    }
    func_8004E58C();
    func_80057390(0);
    D_800E6280.unk_03A = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80141468);

void func_801415A4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EA98();
        func_8004E788(-0x78, 0x30, 0, "今回のアルバムモードのデータを", 0);
        func_8004E788(-0x78, 0x3E, 0, "　ときめきメモリアル・システムファイル", 0);
        func_8004E788(-0x78, 0x4C, 0, "にセーブしますか。", 0);
        func_8004E788(-0x88, 0x5A, 0, D_801500F0, 0);
        func_8004E788(-0x34, 0x5A, 1, func_80140F50(), 0);
        func_8004E788(8, 0x5A, 0, D_8015010C, 0);
        func_8004E788(0x5C, 0x5A, 2, func_80140F94(), 0);
        func_8004EAD4(7);
        func_800AE0B0("データセーブメッセージ");
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if ((u32)D_800E6280.unk_1104.w >= 0x41U) {
            func_8004284C();
        }
        break;
    }
    func_800578F4(2);
}

void func_80141700(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        func_8004284C();
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8004482C();
        func_80042878(0xC4);
    }
    func_800578F4(2);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_8014175C);

void func_80141980(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x78, 0x30, 0, "ただいま、セーブ中です。", 0);
        func_8004EAD4(2);
        D_800E6280.unk_110D += 1;
        func_80053CE0();
        break;
    case 1:
        if (func_80054284() == 3) {
            func_80053D10();
            func_800AE0B0("LOAD SYSTEM OK");
            D_800E6280.unk_110D += 1;
        } else if ((u32)D_800E6280.unk_1120 >= 0x201U || D_800E6280.unk_1115 == 0xF0) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 2:
        if (func_800552DC(0) == -1) {
            func_80042940(0xFD);
        } else {
            func_8004284C();
        }
        break;
    }
    func_800578F4(2);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80141ABC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80141BE8);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80141E10);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_801421AC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80142390);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80142574);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_801428EC);

void func_80142974(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80143644();
        break;
    case 1:
        func_80143FE0();
        break;
    }
    if (func_800460EC() & 4) {
        if (D_800E6280.unk_F88 & 0x200000) {
            if (D_800E6280.unk_1124 != 0) {
                func_80042878(0x11);
            } else {
                func_80042878(0xC4);
            }
        } else if (D_800E6280.unk_F88 & 0x400000) {
            if (D_800E6280.unk_1124 != 0) {
                func_80042878(0x11);
            } else {
                func_80042878(0x91);
            }
        }
    }
}
void func_80142A4C(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        D_801217D0[i].unk_04 = i * 0x80 - 0x60;
        D_801217D0[i].unk_00 = 0x01000000;
        D_801217D0[i].unk_06 = -0x40;
        D_801217D0[i].unk_08 = 0x80;
        /* s16 view of the u16 member: the constant is then shared with unk_08 */
        *(s16 *)&D_801217D0[i].unk_0A = 0x80;
        D_801217D0[i].unk_0C = i + 0x15;
        D_801217D0[i].unk_0E = 0;
        D_801217D0[i].unk_0F = 0;
        D_801217D0[i].unk_10 = 0;
        D_801217D0[i].unk_12 = 0x1FF;
        D_801217D0[i].unk_14 = 0;
        D_801217D0[i].unk_15 = 0;
        D_801217D0[i].unk_16 = 0;
        D_801217D0[i].unk_18 = 0;
        D_801217D0[i].unk_1A = 0;
        D_801217D0[i].unk_1C = 0x1000;
        D_801217D0[i].unk_1E = 0x1000;
        D_801217D0[i].unk_20 = 0;
        D_800E6280.unk_10A5[i] = 0;
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80142AE8);
