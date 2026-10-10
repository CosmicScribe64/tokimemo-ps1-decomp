#include "common.h"
#include "ovl/EVENT.h"

void func_800F9680(void) {
    func_800433D0(0x200);
    func_80011DFC();
}

void func_800F96A8(void) {
    func_80015D28(0x45, 0x801B0000, 0x7B5F);
    func_800F7590();
    func_80011DFC();
}

void func_800F96E0(void) {
    func_800F7074();
    if ((u16) D_80094718 == 4) {
        func_8004CA18();
        D_800B1AF6 -= 1;
    }
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800F9730);

void func_800F9900(void) {
    D_80120BF0 = 0x801D638C;
    D_80120BF4 = 0x801D6394;
    D_80120BF8 = 0x801D63B4;
    D_80120BFC = *(s16 *)0x801D63C8;
    D_80120C00 = 0x801B0000;
    D_80120C04 = 0x801D6000;
    D_80120C08 = 0x801B2000;
    D_80120C0C = 0x801B6000;
    D_80120C10 = 0x801BA000;
    D_80120C14 = 0x801BE000;
    D_80120C18 = 0x801C2000;
    D_80120C1C = 0x801C6000;
    D_80120C20 = 0x801D2000;
}

void func_800F99CC(void) {
    D_80120C24 = 0x801D21D4;
    D_80120C28 = 0x801D21DC;
    D_80120C2C = 0x801D21FC;
    D_80120C30 = *(s16 *)0x801D2210;
    D_80120C34 = 0x801B0000;
    D_80120C38 = 0x801B2000;
    D_80120C3C = 0x801B6000;
    D_80120C40 = 0x801BA000;
    D_80120C44 = 0x801BE000;
    D_80120C48 = 0x801C2000;
    D_80120C4C = 0x801C6000;
}

void func_800F9A7C(void) {
    D_80120C50 = 0x801D21D0;
    D_80120C54 = 0x801D21D8;
    D_80120C58 = 0x801D2218;
    D_80120C5C = *(s16 *)0x801D2230;
    D_80120C60 = 0x801B0000;
    D_80120C64 = 0x801B2000;
    D_80120C68 = 0x801B6000;
    D_80120C6C = 0x801BA000;
    D_80120C70 = 0x801BE000;
    D_80120C74 = 0x801C2000;
    D_80120C78 = 0x801C6000;
}

void func_800F9B2C(void) {
    D_80120C7C = 0x801D232C;
    D_80120C80 = 0x801D2334;
    D_80120C84 = 0x801D2374;
    D_80120C88 = *(s16 *)0x801D238C;
    D_80120C8C = 0x801B0000;
    D_80120C90 = 0x801B2000;
    D_80120C94 = 0x801B6000;
    D_80120C98 = 0x801BA000;
    D_80120C9C = 0x801BE000;
    D_80120CA0 = 0x801C2000;
    D_80120CA4 = 0x801C6000;
}

void func_800F9BDC(void) {
    D_80120CA8 = 0x801B0000;
    D_80120CAC = 0x801B2000;
    D_80120CB0 = 0x801B6000;
    D_80120CB4 = 0x801BA000;
    D_80120CB8 = 0x801BE000;
    D_80120CBC = 0x801C2000;
    D_80120CC0 = 0x801C6000;
}

void func_800F9C4C(void) {
    D_80120CC4 = 0x801D22D0;
    D_80120CC8 = 0x801D22D8;
    D_80120CCC = 0x801D233C;
    D_80120CD0 = *(s16 *)0x801D2358;
    D_80120CD4 = 0x801B0000;
    D_80120CD8 = 0x801B2000;
    D_80120CDC = 0x801B6000;
    D_80120CE0 = 0x801BA000;
    D_80120CE4 = 0x801BE000;
    D_80120CE8 = 0x801C2000;
    D_80120CEC = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800F9CFC);

void func_800F9DBC(void) {
    if (D_800B1AF5 == 0) {
        func_800F9DF8();
        return;
    }
    func_80015FE0();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800F9DF8);

void func_800F9E74(void) {
    func_800433D0(0x505);
    func_800433D0(0x506);
    func_80011DFC();
}

void func_800F9EA4(void) {
    func_800137AC(0x140, 0, 0x40, 0x80, D_80120C20);
    D_800B1C1C = 1;
    func_80011DFC();
}

void func_800F9EEC(void) {
    func_80015D28(0x4D, 0x801B0000, 0x7BA4);
    func_800F9900();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800F9F24);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FA0D0);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FA194);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FA25C);

void func_800FA584(void) {
    if (D_800B1AF5 == 0) {
        func_800FA5C0();
        return;
    }
    func_80015FE0();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FA5C0);

void func_800FA664(void) {
    if (D_800B1AF5 == 0) {
        func_800FA6A0();
        return;
    }
    func_80015FE0();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FA6A0);

void func_800FA784(void) {
    func_80015D28(0x45, 0x801B0000, 0x7D4A);
    func_800F9C4C();
    func_80011DFC();
}

void func_800FA7BC(void) {
    func_800143DC(1, 0xB176, 0xB166, 0xBAF6, 0xBAA6, 0xBAA1);
    func_80011DFC();
}

void func_800FA800(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void func_800FA828(void) {
    func_800335E0(0x5714);
    func_80011DFC();
}

void func_800FA850(void) {
    if ((u8) D_800B1AF6 >= 0x1BU) {
        D_800EB02F = D_800EAFA7;
    }
    if (((u8) D_800B1AF6 >= 0x1BU) && (((u32) D_800B1AE4 % 300U) == 0x14)) {
        D_800EB02A |= 3;
    }
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FA8B8);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FAA04);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FAC00);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FAD28);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FAE6C);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FAF7C);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FB078);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FB188);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FB2A0);

void func_800FB39C(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void func_800FB3C4(void) {
    func_800469F4(0x461A);
    func_80011DFC();
}

void func_800FB3EC(void) {
    func_800469F4(0x40E8);
    func_80011DFC();
}

void func_800FB414(void) {
    func_800469F4(0x4700);
    func_80011DFC();
}

void func_800FB43C(void) {
    func_800469F4(0x4713);
    func_80011DFC();
}

void func_800FB464(void) {
    func_800469F4(0x4284);
    func_80011DFC();
}

void func_800FB48C(void) {
    func_800469F4(0x42CD);
    func_80011DFC();
}

void func_800FB4B4(void) {
    func_800469F4(0x4746);
    func_80011DFC();
}

void func_800FB4DC(void) {
    func_800469F4(0x42A8);
    func_80011DFC();
}

void func_800FB504(void) {
    func_800469F4(0x4831);
    func_80011DFC();
}

void func_800FB52C(void) {
    func_800469F4(0x4538);
    func_80011DFC();
}

void func_800FB554(void) {
    func_800469F4(0x410B);
    func_80011DFC();
}

void func_800FB57C(void) {
    func_800469F4(0x4691);
    func_80011DFC();
}

void func_800FB5A4(void) {
    func_800469F4(0x40F1);
    func_80011DFC();
}

void func_800FB5CC(void) {
    D_800EECBC = 0;
    D_8012438C = 1;
    D_8012438E = 1;
    D_80124390 = 1;
    D_801243A4 = 0;
    D_801243A6 = 0;
    D_801243A8 = 0;
    D_80124394 = 1;
    D_80124396 = 1;
    D_80124398 = 1;
    D_8012439C = 0;
    D_8012439E = 0;
    D_801243A0 = 0;
    func_80011DFC();
    D_80094714 = 0;
    D_80094718 = 0;
    func_800F6000();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FB67C);

void func_800FB6F0(void) {
    func_80015D28(0x45, 0x801B0000, 0x7BF1);
    func_800F99CC();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FB728);

void func_800FB7B0(void) {
    func_80015D28(0x45, 0x801B0000, 0x7C36);
    func_800F9A7C();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FB7E8);

void func_800FB864(void) {
    func_800F7074();
    if ((u16) D_80094718 == 2) {
        func_8004CA48();
        D_800B1AF6 -= 1;
    }
}

void func_800FB8B4(void) {
    func_80015D28(0x45, 0x801B0000, 0x7C7B);
    func_800F9B2C();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FB8EC);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FB920);

void func_800FB994(void) {
    func_800433D0(0x502);
    func_80011DFC();
}

void func_800FB9BC(void) {
    if ((u16) D_80094718 == 5) {
        func_8004A8EC(1);
        func_80033E88();
    } else if ((u16) D_80094718 == 9) {
        func_8004A8EC(2);
        func_80033E88();
    } else if ((u16) D_80094718 == 0xB) {
        func_8004A8EC(func_8002336C(D_800B1746));
        func_80033E88();
    }
    func_800F7074();
}

void func_800FBA50(void) {
    func_80015D28(0x35, 0x801B0000, 0x7CC0);
    func_800F9BDC();
    func_80011DFC();
}

void func_800FBA88(void) {
    func_80011E28(0x52);
}

void func_800FBAA8(void) {
    func_800469F4(0x416E);
    func_80011DFC();
}

void func_800FBAD0(void) {
    func_800469F4(0x4164);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F9680", func_800FBAF8);
