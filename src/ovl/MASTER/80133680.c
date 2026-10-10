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

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801350A8);

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
