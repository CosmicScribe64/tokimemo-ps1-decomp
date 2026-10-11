#include "ovl/MASTER.h"

void func_80133680(void) {
    D_8013C4A0 = 0x801E8660;
    D_8013C4A4 = 0x801E8740;
    D_8013C4A8 = 0x801E8AF8;
}

void func_801336B4(void) {
    D_8013C4A0 = 0x801EAA54;
    D_8013C4A4 = 0x801EAE50;
    D_8013C4A8 = 0x801ED1C4;
}

void func_801336E8(void) {
    D_8013C4A0 = 0x801E8A10;
    D_8013C4A4 = 0x801E8B7C;
    D_8013C4A8 = 0x801E93D4;
}

void func_80133724(void) {
    D_8013C4B0 = 0x801EAFC4;
    D_8013C4B4 = 0x801EB144;
    D_8013C4B8 = 0x801EAFF0;
    D_8013C4BC = 0x801EB14C;
    D_8013C4C0 = 0x801EB030;
    D_8013C4C4 = 0x801EB174;
    D_8013C4C8 = *(s16 *)0x801EB17C;
    D_8013C4CC = *(s16 *)0x801EB180;
    D_8013C4D0 = 0x801B4400;
}

void func_801337B8(void) {
    D_8013C4D4 = 0x801B00DC;
    D_8013C4D8 = 0x801B02AC;
    D_8013C4DC = 0x801B03C8;
    D_8013C4E0 = 0x801B04C4;
    D_8013C4E4 = 0x801B0668;
    D_8013C4E8 = 0x801B0808;
    D_8013C4EC = 0x801B09DC;
    D_8013C4F0 = 0x801B0BE8;
    D_8013C4F4 = 0x801B0D8C;
    D_8013C4F8 = 0x801B0F5C;
    D_8013C4FC = 0x801B1164;
    D_8013C500 = 0x801B12D8;
    D_8013C504 = 0x801B1454;
    D_8013C508 = 0x801B00F4;
    D_8013C50C = 0x801B02C8;
    D_8013C510 = 0x801B03D0;
    D_8013C514 = 0x801B04DC;
    D_8013C518 = 0x801B0680;
    D_8013C51C = 0x801B0820;
    D_8013C520 = 0x801B09F8;
    D_8013C524 = 0x801B0C00;
    D_8013C528 = 0x801B0DA4;
    D_8013C52C = 0x801B0F78;
    D_8013C530 = 0x801B1180;
    D_8013C534 = 0x801B12E0;
    D_8013C538 = 0x801B1470;
    D_8013C53C = 0x801B01A8;
    D_8013C540 = 0x801B0398;
    D_8013C544 = 0x801B03F0;
    D_8013C548 = 0x801B0590;
    D_8013C54C = 0x801B0734;
    D_8013C550 = 0x801B08D4;
    D_8013C554 = 0x801B0AE4;
    D_8013C558 = 0x801B0CB4;
    D_8013C55C = 0x801B0E58;
    D_8013C560 = 0x801B1064;
    D_8013C564 = 0x801B126C;
    D_8013C568 = 0x801B1338;
    D_8013C56C = 0x801B155C;
}

typedef struct {
    void (*f[30])();
} FnTbl30; /* size 0x78 */
extern FnTbl30 D_8013C5C8;

void func_80133A2C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl30 tbl;

    tbl = D_8013C5C8;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80133AA0);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80133F80);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801344E0);

void func_80135040(void) {
    if (((D_800E6280.unk_1BC[0].unk_10.w & 0xF) == D_800E6280.unk_03F) && (((u32)(D_800E6280.unk_1BC[0].unk_10.w << 0x17) >> 0x1B) == D_800E6280.unk_040)) {
        func_80138490();
        return;
    }
    func_80042808();
    Default_Disp();
}

void func_801350A8(void) {
    s32 index;
    MasterWords11 text_ids;
    u8 flags;

    text_ids = D_8013C64C;
    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        if ((D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF)) && (D_800E6280.unk_040 == ((u32) (D_800E6280.unk_0F8 << 0x17) >> 0x1B)) && !((flags = D_800E6280.unk_71E) & 4)) {
            func_80044750(0xC1);
            flags = D_800E6280.unk_71E;
            flags |= 4;
            D_800E6280.unk_71E = flags;
            func_8004284C();
        } else {
            func_80042940(0xB);
        }
        break;
    case 1:
        D_800E6280.unk_71C += 1;
        if (func_8008667C(func_8008647C()) < 2) {
            D_800E6280.unk_F5F = func_8008647C();
            func_80085B3C(0x15, 6);
            func_80044750(0xBF);
            func_80044890(1, 0xC043, 0xC01D, D_800B3688[D_800E6280.unk_F5F], D_800B36C8[D_800E6280.unk_F5F], D_800B3708[D_800E6280.unk_F5F]);
            func_8004284C();
        } else {
            func_80042940(0x10);
        }
        break;
    case 2:
        if (func_80044E8C() == 1) {
            index = D_800E6280.unk_F5F;
            func_80062CD0(text_ids.v[index]);
            func_8004284C();
        }
        break;
    case 3:
        if ((flags = D_800E6280.unk_71E) & 1) {
            func_8004284C();
        } else {
            func_8007EAAC();
        }
        break;
    case 4:
        if (func_800460CC() & 1) {
            func_8007C740();
            func_80083474();
            func_80046318(3U, 0x801B0000, 0xAF43);
        }
        break;
    case 5:
        if (func_800460CC() & 1) {
            func_801337B8();
            func_800847B8(D_800E6280.unk_F5F);
            D_800CA148 = 0;
            D_800CA14C = 0;
            switch (D_800E6280.unk_F5F) {
            case 0:
                D_800CA160 = D_8013C4D8;
                D_800CA164 = D_8013C50C;
                D_800CA168 = D_8013C540;
                break;
            case 1:
                D_800CA160 = D_8013C4EC;
                D_800CA164 = D_8013C520;
                D_800CA168 = D_8013C554;
                break;
            case 2:
                D_800CA160 = D_8013C4E0;
                D_800CA164 = D_8013C514;
                D_800CA168 = D_8013C548;
                break;
            case 3:
                D_800CA160 = D_8013C4E4;
                D_800CA164 = D_8013C518;
                D_800CA168 = D_8013C54C;
                break;
            case 4:
                D_800CA160 = D_8013C4FC;
                D_800CA164 = D_8013C530;
                D_800CA168 = D_8013C564;
                break;
            case 5:
                D_800CA160 = D_8013C4F0;
                D_800CA164 = D_8013C524;
                D_800CA168 = D_8013C558;
                break;
            case 6:
                D_800CA160 = D_8013C4F4;
                D_800CA164 = D_8013C528;
                D_800CA168 = D_8013C55C;
                break;
            case 7:
                D_800CA160 = D_8013C4E8;
                D_800CA164 = D_8013C51C;
                D_800CA168 = D_8013C550;
                break;
            case 8:
                D_800CA160 = D_8013C4D4;
                D_800CA164 = D_8013C508;
                D_800CA168 = D_8013C53C;
                break;
            case 9:
                D_800CA160 = D_8013C4F8;
                D_800CA164 = D_8013C52C;
                D_800CA168 = D_8013C560;
                break;
            case 10:
                D_800CA160 = D_8013C504;
                D_800CA164 = D_8013C538;
                D_800CA168 = D_8013C56C;
                break;
            }
            func_80044750(0x200);
            func_8004284C();
        }
        break;
    case 6:
        func_80085A60();
        break;
    case 7:
        func_8007E390();
        break;
    case 8:
        func_80085A60();
        break;
    case 9:
        func_8007E5A0();
        if (D_800E6280.unk_110A == 0xA) {
            func_80044750(0xC1);
        }
        break;
    case 10:
        func_80085A60();
        break;
    case 11:
        if (((flags = D_800E6280.unk_71E) & 1) || ((flags = D_800E6280.unk_71E) & 4)) {
            func_80044750(0xBF);
            func_80044890(1, 0xC043, 0xC01D, 0xC982, 0xC935, 0xC91B);
            func_8004284C();
        } else {
            func_80042808();
        }
        break;
    case 12:
        if (func_80044E8C() == 1) {
            func_80046318(0x1BU, 0x80197000, 0xB03E);
            func_8004284C();
        }
        break;
    case 13:
        func_8007EAAC();
        break;
    case 14:
        if (func_800460CC() & 1) {
            func_80138F10();
            func_80043914(D_8013C730, 0x10, 1, 1, 0);
            func_80046318(2U, 0x801E8000, 0xA9BA);
            func_8004284C();
        }
        break;
    case 15:
        if (func_800460CC() & 1) {
            func_80133680();
            D_800E6280.unk_F5F = 0;
            func_800847B8(0U);
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_8013C4A0;
            D_800CA164 = (s32) D_8013C4A4;
            D_800CA168 = (s32) D_8013C4A8;
            func_80042808();
        }
        break;
    case 16:
        if ((flags = D_800E6280.unk_71E) & 1) {
            func_80046318(2U, 0x801E8000, 0xA9BA);
            func_8004284C();
        } else {
            D_800CA148 = 0x1B;
            func_80042940(0x12);
        }
        break;
    case 17:
        if (func_800460CC() & 1) {
            func_80133680();
            D_800E6280.unk_F5F = 0;
            func_800847B8(0U);
            D_800CA148 = 0x1B;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_8013C4A0;
            D_800CA164 = (s32) D_8013C4A4;
            D_800CA168 = (s32) D_8013C4A8;
            func_8004284C();
        }
        break;
    case 18:
        func_80085A60();
        break;
    case 19:
        func_80042940(0xB);
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801358C8);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801359F0);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80135CF4);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80135FB8);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801363F4);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80136E24);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80137044);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801375E8);

void func_80137B5C(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80044750(0xBF);
        func_800674B0();
        func_8004284C();
    } else if (func_80044E8C() == 1) {
        func_80048F64(0x60);
        D_800E6280.unk_71E |= 0x10;
        func_8008585C();
        set_dec_bri(0);
        func_8006D6E0();
        func_80072B5C(1);
    }
    Default_Disp();
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80137BF4);

void func_80137F94(void) {
    func_80138EC0();
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120657 = 0;
    D_80120655 = 0x80;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
    D_80120693 = 0x10;
    D_80120676 = 0;
    D_8012067A = -0x20;
    D_8012065C = (u8 *)D_8013C714;
    D_80120660 = (u8 *)D_8013C718;
    D_80120684 = (u8 *)D_8013C710;
    D_80120664 = D_8013C71C;
    D_80120652 = 1;
    D_80120653 = 4;
    D_80120668 = 0;
}

void func_80138088(void) {
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_00 = D_800E6280.unk_F5F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_02 = 0x15;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = 6;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_04 = D_800E6280.unk_03E;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_05 = D_800E6280.unk_03F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_06 = D_800E6280.unk_040;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = D_800E6280.unk_041;
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 1:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 2:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 3:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 4:
        if ((D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 6) {
            D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 5;
            D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = 0xFF;
        } else {
            D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        }
        break;
    case 5:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 6:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 0;
        break;
    case 7:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 0;
        break;
    case 8:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 9:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 5;
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = 0xFF;
        break;
    case 10:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 11:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 12:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    }
    if ((D_800E6280.unk_F5E = D_800E6280.unk_F5E + 1) >= 0xFF) {
        D_800E6280.unk_F5E = 0xFE;
    }
}

void func_80138374(void) {
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_00 = D_800E6280.unk_F5F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_02 = 0x15;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = 6;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 5;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_04 = D_800E6280.unk_03E;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_05 = D_800E6280.unk_03F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_06 = D_800E6280.unk_040;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = D_800E6280.unk_041;
    D_800E6280.unk_F5E += 1;
    if (D_800E6280.unk_F5E >= 0xFFU) {
        D_800E6280.unk_F5E = 0xFE;
    }
}
