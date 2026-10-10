#include "common.h"
#include "ovl/NAME_ENT.h"

void func_80144AD0(void) {
    D_800E6280.unk_1100 += 1;
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80144A34();
        return;
    case 1:
        func_80142A80();
        return;
    }
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80144B44);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80144C8C);

void func_80144F98(void) {
    func_80044890(0, 0xC592, 0xC580, 0xD15F, 0xD110, 0xD102);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80144FDC);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80145378);

void func_80145454(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80144F98();
        break;
    case 1:
        func_80144FDC();
        break;
    case 2:
        func_80145378();
        break;
    }
    func_80064DEC();
    func_800578F4(1);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_801454CC);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_801455E4);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80145760);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80145848);

void func_80145A74(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80145760();
        return;
    case 1:
        func_80145848();
        return;
    }
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80145AC4);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80145EA4);

void func_80145F74(void) {
    func_801455E4(1);
    if (D_800E6280.unk_110A == 0) {
        func_80145AC4();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80145FB0);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_801460D8);

void func_80146288(void) {
    if (D_800E6280.unk_110A == 0) {
        func_801460D8();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_801462B4);

void func_801465F8(void) {
    if (D_800E6280.unk_110A == 0) {
        func_801462B4();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80146624);

void func_80146914(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80146624();
    }
}

void func_80146940(void) {
    D_80120651 = 0xA;
    D_80120652 = 0x40;
    D_80120688 = 0x41000000;
    D_80120653 = 0xA4;
    D_8012065C = (u8 *)0x801842C4;
    D_80120660 = (u8 *)0x801842F0;
    D_80120684 = (u8 *)0x801842BC;
    D_80120668 = 0;
    D_80120655 = 0x80;
    D_80120676 = -0xA0;
    D_8012067A = -0x78;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_801469D0);

void func_80146B3C(void) {
    D_801207E9 = 9;
    D_801207EA = 3;
    D_80120820 = 0x41000000;
    D_801207EB = 0xA4;
    D_801207F4 = 0x801842C4;
    D_801207F8 = 0x801842F0;
    D_8012081C = 0x801842BC;
    D_801207FE = 0;
    D_801207ED = 0x80;
    D_8012080E = -0xA0;
    D_80120812 = -0x78;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80146BCC);

void func_80146CB8(void) {
    D_8012071D = 9;
    D_8012071E = 0;
    D_80120754 = 0x41000000;
    D_8012071F = 0xA4;
    D_80120728 = 0x801800F8;
    D_8012072C = 0x80180114;
    D_80120750 = 0x801800F4;
    D_80120721 = 0x80;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
}

void func_80146D3C(void) {
    D_8012071D = 9;
    D_8012071E = 5;
    D_80120754 = 0x41000000;
    D_8012071F = 0xA4;
    D_80120728 = 0x801800F8;
    D_8012072C = 0x80180114;
    D_80120750 = 0x801800F4;
    D_80120721 = 0x80;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
}

void func_80146DC4(void) {
    if (func_800460EC() & 4) {
        D_80120761 = 8;
        D_80120762 = 1;
        D_80120798 = 0x41000000;
        D_80120763 = 0xA4;
        D_8012076C = 0x801800F8;
        D_80120770 = 0x80180114;
        D_80120794 = 0x801800F4;
        D_80120776 = 1;
        D_80120765 = 0x80;
        D_80120786 = -0xA0;
        D_8012078A = -0x78;
        return;
    }
    D_80120762 = 0;
}

void func_80146E88(void) {
    if (func_800460EC() & 4) {
        D_801207A5 = 8;
        D_801207A6 = 1;
        D_801207DC = 0x41000000;
        D_801207A7 = 0xA4;
        D_801207B0 = 0x801800F8;
        D_801207B4 = 0x80180114;
        D_801207D8 = 0x801800F4;
        D_801207BA = 2;
        D_801207A9 = 0x80;
        D_801207CA = -0xA0;
        D_801207CE = -0x78;
        return;
    }
    D_801207A6 = 0;
}

void func_80146F4C(void) {
    D_8012082D = 8;
    D_8012082E = 0x40;
    D_80120864 = 0x41000000;
    D_8012082F = 0xA4;
    D_80120838 = 0x801842C4;
    D_8012083C = 0x801842F0;
    D_80120860 = 0x801842BC;
    D_80120844 = 7;
    D_80120831 = 0x80;
    D_80120852 = -0xA0;
    D_80120856 = -0x78;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80146FE0);

void func_80147380(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80146FE0();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_801473AC);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80147590);

void func_801478B4(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80147590();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_801478E0);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80147950);

void func_80147A08(void) {
    func_80049A40(-0xA0, -0x78, 0xA0, 0xA0, 0xD, 0xFFFFFF, 1);
    func_80049A40(0, -0x78, 0xA0, 0xA0, 0xD, 0xFFFFFF, 1);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80147A80);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80147BEC);

void func_80147F4C(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80147BEC();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80147F78);

void func_801480E0(void) {
    D_801206D9 = 9;
    D_801206DA = 3;
    D_80120710 = 0x41000000;
    D_801206DB = 0xAC;
    D_801206E4 = 0x80184288;
    D_801206E8 = 0x801842AC;
    D_8012070C = 0x80184280;
    D_801206EE = 0;
    D_801206DD = 0x80;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
}

void func_80148170(void) {
    D_8012071D = 0xC;
    D_8012071E = 0;
    D_80120754 = 0x41000000;
    D_8012071F = 0xA4;
    D_80120728 = 0x80180110;
    D_8012072C = 0x80180130;
    D_80120750 = 0x8018010C;
    D_80120732 = 0;
    D_80120721 = 0x80;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
    D_80120761 = 0xC;
    D_80120762 = 0;
    D_80120798 = 0x41000000;
    D_80120763 = 0xA4;
    D_8012076C = 0x80180110;
    D_80120770 = 0x80180130;
    D_80120794 = 0x8018010C;
    D_80120776 = 1;
    D_80120765 = 0x80;
    D_80120786 = -0xA0;
    D_8012078A = -0x78;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_8014826C);

void func_801486CC(void) {
    if (D_800E6280.unk_110A == 0) {
        func_8014826C();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_801486F8);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_801488DC);

void func_80148BF4(void) {
    if (D_800E6280.unk_110A == 0) {
        func_801488DC();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80148C20);

void func_80148EDC(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80148C20();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80148F08);

void func_80149118(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80148F08();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80149144);

void func_801492B0(void) {
    D_801206D9 = 9;
    D_801206DA = 0x11;
    D_80120710 = 0x41000000;
    D_801206DB = 0xAC;
    D_801206E4 = 0x801800F4;
    D_801206E8 = 0x80180114;
    D_8012070C = 0x801800F0;
    D_801206EE = 0;
    D_801206DD = 0x80;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80149340);

void func_8014952C(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80149340();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80149558);

void func_801496C0(void) {
    if (!(D_8012071E & 1)) {
        func_80048F64(0x62);
        D_801206D9 = 8;
        D_801206DA = 3;
        D_80120710 = 0x41000000;
        D_801206DB = 0xAC;
        D_801206E4 = 0x80180294;
        D_801206E8 = 0x801802C8;
        D_8012070C = 0x80180284;
        D_801206EE = 0;
        D_801206DD = 0x80;
        D_801206FE = -0xA0;
        D_80120702 = -0x78;
    }
}

void func_8014977C(void) {
    func_80048F64(0x60);
    func_80048F64(0x61);
    func_80048F64(0x62);
    func_80048F64(0x63);
    D_8012071D = 8;
    D_8012071E = 5;
    D_80120754 = 0x41000000;
    D_8012071F = 0xA4;
    D_80120728 = 0x80180294;
    D_8012072C = 0x801802C8;
    D_80120750 = 0x80180284;
    D_80120732 = 2;
    D_80120721 = 0x80;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80149840);

void func_80149ADC(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80149840();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80149B08);

void func_80149CEC(void) {
    D_8012071D = 9;
    D_8012071E = 5;
    D_80120754 = 0x41000000;
    D_8012071F = 0xA4;
    D_80120728 = 0x80180258;
    D_8012072C = 0x80180298;
    D_80120750 = 0x8018024C;
    D_80120721 = 0x80;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
}

void func_80149D74(void) {
    D_80120761 = 0xA;
    D_80120762 = 0;
    D_80120798 = 0x41000000;
    D_80120763 = 0xA4;
    D_8012076C = 0x80180258;
    D_80120770 = 0x80180298;
    D_80120794 = 0x8018024C;
    D_80120776 = 3;
    D_80120765 = 0x80;
    D_80120786 = -0xA0;
    D_8012078A = -0x38;
    D_800B5BD4 = 9;
}

void func_80149E10(void) {
    if (!(D_80120762 & 1)) {
        D_80120762 = 1;
    }
    D_800B5BD4 = 0xF;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_80149E3C);

void func_8014A5D4(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80149E3C();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_8014A600);

void func_8014A81C(void) {
    if (D_800E6280.unk_110A == 0) {
        func_8014A600();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_8014A848);

void func_8014A960(void) {
    if (D_800E6280.unk_110A == 0) {
        func_8014A848();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_8014A98C);

void func_8014AAEC(void) {
    if (D_800E6280.unk_110A == 0) {
        func_8014A98C();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_8014AB18);

void func_8014AD18(void) {
    if (D_800E6280.unk_110A == 0) {
        func_8014AB18();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_8014AD44);

void func_8014AE00(void) {
    if (D_800E6280.unk_110A == 0) {
        func_8014AD44();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_8014AE2C);

void func_8014AEFC(void) {
    if (D_800E6280.unk_110A == 0) {
        func_8014AE2C();
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80144AD0", func_8014AF28);
