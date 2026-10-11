#include "common.h"
#include "ovl/TEL.h"

void func_80132000(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80132060();
        return;
    case 1:
        func_80134BE0();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80132060(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80132138();
        break;
    case 1:
        func_8013226C();
        break;
    case 2:
        func_80132FC4();
        break;
    case 3:
        func_80133D54();
        break;
    case 4:
        func_801345C4();
        break;
    }
    func_80066C08(1);
    func_800646CC();
    func_80064F48();
    func_80064DEC();
    func_80067870();
    func_80066334();
    func_8006BA40();
}

void func_80132138(void) {
    if (*(u8 *)&D_800E6280.unk_110D == 0) {
        func_80051010("Ｔｒｒｒｒ…", 0, 0x2F);
        func_80044750(0x508);
        func_8004EAD4(0);
        D_801230D0 = (func_800AE0D0() & 3) + 1;
        *(u8 *)&D_800E6280.unk_110D = 1;
    } else if (*(u8 *)&D_800E6280.unk_110D == 1) {
        if ((D_800E6280.unk_1104.u & 0x7F) == 1) {
            func_8004EAAC();
            func_80051010("Ｔｒｒｒｒ…", 0, 0x2F);
            func_8004EAD4(0);
            if (D_801230D0 != 0) {
                D_801230D0 -= 1;
            }
            if (D_801230D0 > 0) {
                func_80044750(0x508);
            }
        }
        if (D_801230D0 <= 0 || (D_800E6280.unk_F88 & 0x40)) {
            func_80044750(0x1508);
            func_80044750(0x50B);
            func_8004284C();
        }
    }
    func_80050B54(0);
    func_80069128();
    func_8004EC24();
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_8013226C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_80132FC4);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_80133D54);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_801345C4);

void func_80134BE0(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0x0:
        func_80134DA0();
        break;
    case 0x1:
        func_80134FE4();
        break;
    case 0x2:
        func_80135FFC();
        break;
    case 0x3:
        func_801352F0();
        break;
    case 0x4:
        func_801360E4();
        break;
    case 0x8:
        func_8013586C();
        break;
    case 0x9:
        func_80135A44();
        break;
    case 0xA:
        func_80135C4C();
        break;
    case 0x5:
        func_801361CC();
        break;
    case 0x7:
        func_80135E28();
        break;
    case 0x6:
        func_801362B4();
        break;
    case 0x10:
        func_801396EC();
        break;
    case 0x30:
        func_80136724();
        break;
    case 0x20:
        func_8013639C();
        break;
    case 0x40:
        func_801380D8();
        break;
    case 0x41:
        func_801384F0();
        break;
    case 0x42:
        func_8013883C();
        break;
    case 0x43:
        func_80138D20();
        break;
    case 0x44:
        func_80139064();
        break;
    case 0x45:
        func_801393A8();
        break;
    }
    func_80066C08(1);
    func_800646CC();
    func_80064F48();
    func_80064DEC();
    func_80067870();
    func_8006BA40();
}

void func_80134DA0(void) {
    if (*(u8 *)&D_800E6280.unk_110D == 0) {
        func_80051010("Ｔｒｒｒｒ…", 0, 0x2F);
        func_80044750(0x508);
        func_8004EAD4(0);
        D_801230D0 = (func_800AE0D0() & 3) + 1;
        *(u8 *)&D_800E6280.unk_110D = 1;
        if (*(u8 *)&D_800E6280.unk_F5F == 0xB) {
            D_801405E4 = 0xC;
        } else {
            D_801405E4 = *(u8 *)&D_800E6280.unk_F5F;
        }
    } else if (*(u8 *)&D_800E6280.unk_110D == 1) {
        if ((D_800E6280.unk_1104.u & 0x7F) == 1) {
            func_8004EAAC();
            func_80051010("Ｔｒｒｒｒ…", 0, 0x2F);
            func_8004EAD4(0);
            if (D_801230D0 > 0) {
                func_80044750(0x508);
            }
            if (D_801230D0 != 0) {
                D_801230D0 -= 1;
            }
        }
        if (D_801230D0 <= 0 || (D_800E6280.unk_F88 & 0x40)) {
            func_80044750(0x1508);
            func_80044750(0x50B);
            if (func_8006AEC4(*(u8 *)&D_800E6280.unk_03E, *(u8 *)&D_800E6280.unk_03F, *(u8 *)&D_800E6280.unk_040) != 0 &&
                *(u8 *)&D_800E6280.unk_F5F == (u32)(D_800E6280.unk_54C[0].w << 3) >> 0x1C) {
                func_80042940(0x10);
            } else if (*(u8 *)&D_800E6280.unk_03E == 0x62 && *(u8 *)&D_800E6280.unk_03F == 2 && *(u8 *)&D_800E6280.unk_040 == 0x16) {
                func_80042940(0x10);
            } else if (func_8013655C() == 0) {
                func_80042940(0x10);
            } else if (D_801405E4 == 0xC) {
                func_80042940(0x40);
            } else {
                func_8004284C();
            }
        }
    }
    func_80050B54(0);
    func_80069128();
    func_8004EC24();
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_80134FE4);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_801352F0);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_8013586C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_80135A44);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_80135C4C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_80135E28);

void func_80135FFC(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(D_800B35F4[D_801405E4]);
        func_80050E8C(D_80140AD8[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140B04, D_80140B5C, D_801405E4, D_801405E4);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    func_80050B54(0);
    func_80069128();
}

void func_801360E4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(D_800B35F4[D_801405E4]);
        func_80050E8C(D_80140AD8[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140B04, D_80140B5C, D_801405E4, D_801405E4);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    func_80050B54(0);
    func_80069128();
}

void func_801361CC(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(D_800B35F4[D_801405E4]);
        func_80050E8C(D_80140D18[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140D44, D_80140D9C, D_801405E4, D_801405E4);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

void func_801362B4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(D_800B35F4[D_801405E4]);
        func_80050E8C(D_80140F0C[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140F38, D_80140F90, D_801405E4, D_801405E4);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_8013639C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_8013655C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_80136724);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_801380D8);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_801384F0);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_8013883C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_80138D20);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_80139064);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000", func_801393A8);

void func_801396EC(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80044750(0x50C);
        func_80050DFC(D_800E6280.unk_0D4);
        func_80051010("あ〜あ、留守みたいだな。", 0, 0x1F);
        func_80051010("今日はあきらめるか……。）", 1, 0x1F);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80051DBC();
        break;
    case 2:
        func_8004E58C();
        func_80048E78();
        func_80042878(0x46);
        break;
    }
    func_80050B54(0);
    func_80069128();
}
