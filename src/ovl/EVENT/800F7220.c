#include "common.h"
#include "ovl/EVENT.h"

void func_800F7220(void) {
    D_80120690 = 0x801CE0FC;
    D_80120694 = 0x801CE100;
    D_80120698 = 0x801CE120;
    D_8012069C = *(s16 *)0x801CE13C;
    D_801206A0 = 0x801B0000;
    D_801206A4 = 0x801B2000;
    D_801206A8 = 0x801B6000;
    D_801206AC = 0x801BA000;
    D_801206B0 = 0x801BE000;
    D_801206B4 = 0x801C2000;
    D_801206B8 = 0x801C6000;
}

void func_800F72D0(void) {
    D_801206BC = 0x801CE0F4;
    D_801206C0 = 0x801CE0F8;
    D_801206C4 = 0x801CE118;
    D_801206C8 = *(s16 *)0x801CE12C;
    D_801206CC = 0x801B0000;
    D_801206D0 = 0x801B2000;
    D_801206D4 = 0x801B6000;
    D_801206D8 = 0x801BA000;
    D_801206DC = 0x801BE000;
    D_801206E0 = 0x801C2000;
    D_801206E4 = 0x801C6000;
}

void func_800F7380(void) {
    D_801206E8 = 0x801D2184;
    D_801206EC = 0x801D218C;
    D_801206F0 = 0x801D21B4;
    D_801206F4 = *(s16 *)0x801D21C8;
    D_801206F8 = 0x801B0000;
    D_801206FC = 0x801B2000;
    D_80120700 = 0x801B6000;
    D_80120704 = 0x801BA000;
    D_80120708 = 0x801BE000;
    D_8012070C = 0x801C2000;
    D_80120710 = 0x801C6000;
}

void func_800F7430(void) {
    D_80120714 = 0x801CE080;
    D_80120718 = 0x801CE084;
    D_8012071C = 0x801CE094;
    D_80120720 = *(s16 *)0x801CE0A8;
    D_80120724 = 0x801B0000;
    D_80120728 = 0x801B2000;
    D_8012072C = 0x801B6000;
    D_80120730 = 0x801BA000;
    D_80120734 = 0x801BE000;
    D_80120738 = 0x801C2000;
    D_8012073C = 0x801C6000;
}

void func_800F74E0(void) {
    D_80120740 = 0x801CE0B8;
    D_80120744 = 0x801CE0BC;
    D_80120748 = 0x801CE0D4;
    D_8012074C = *(s16 *)0x801CE0E8;
    D_80120750 = 0x801B0000;
    D_80120754 = 0x801B2000;
    D_80120758 = 0x801B6000;
    D_8012075C = 0x801BA000;
    D_80120760 = 0x801BE000;
    D_80120764 = 0x801C2000;
    D_80120768 = 0x801C6000;
}

void func_800F7590(void) {
    D_8012076C = 0x801D2240;
    D_80120770 = 0x801D2248;
    D_80120774 = 0x801D2298;
    D_80120778 = *(s16 *)0x801D22B0;
    D_8012077C = 0x801B0000;
    D_80120780 = 0x801B2000;
    D_80120784 = 0x801B6000;
    D_80120788 = 0x801BA000;
    D_8012078C = 0x801BE000;
    D_80120790 = 0x801C2000;
    D_80120794 = 0x801C6000;
}

void func_800F7640(void) {
    D_80120798 = 0x801D2074;
    D_8012079C = 0x801D2078;
    D_801207A0 = 0x801D2088;
    D_801207A4 = *(s16 *)0x801D6094;
    D_801207A8 = 0x801D0000;
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F7220", func_800F7690);

void func_800F7740(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_800F77A0();
        return;
    }
    func_80015FE0();
}

typedef struct {
    void (*f[52])();
} FnTbl52; /* size 0xD0 */
extern FnTbl52 D_801207B8;

void func_800F77A0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl52 tbl;

    tbl = D_801207B8;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    D_800EAFA2 = 1;
}

void func_800F7824(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7A26);
    func_800F7220();
    func_80011DFC();
}

void func_800F785C(void) {
    func_8004BC20(D_800B1746);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F7220", func_800F788C);

void func_800F79EC(void) {
    func_800F7220();
    func_80012D64(D_801206A0, 0x11, 1, 2, 0);
    func_8004C46C(D_801206A4, D_801206A8, D_801206AC, D_801206B0, D_801206B4, D_801206B8);
    func_8004C6B0(D_80120694, D_80120698, D_80120690, (s32) D_8012069C);
    D_800EAFB6 = 4;
    D_800EECEC = 1;
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F7220", func_800F7AA8);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F7220", func_800F7C94);

void func_800F7F44(void) {
    D_801207AC = 0;
    *(s16 *)&D_801207B0 = 3;
    D_80120678 = D_80120534;
    D_8012067C = D_8012054C;
    D_80120680 = D_80120564;
    D_800B0A04[8].unk_02 += 1;
    D_800B0A04[8].unk_06 += 1;
    D_800B0A04[8].unk_0A -= 0x14;
    func_8004C250();
    func_800F7430();
    func_80012D64(D_80120724, 0x11, 1, 2, 0);
    func_8004C46C(D_80120728, D_8012072C, D_80120730, D_80120734, D_80120738, D_8012073C);
    func_8004C6B0(D_80120718, D_8012071C, D_80120714, D_80120720);
    func_80011DFC();
}

void func_800F8070(void) {
    D_80120678 = D_80120538;
    D_8012067C = D_80120550;
    D_80120680 = D_80120568;
    D_800B0A04[8].unk_02 += 1;
    D_800B0A04[8].unk_06 += 1;
    D_800B0A04[8].unk_0A -= 0x14;
    func_8004C250();
    func_800F74E0();
    func_80012D64(D_80120750, 0x11, 1, 2, 0);
    func_8004C46C(D_80120754, D_80120758, D_8012075C, D_80120760, D_80120764, D_80120768);
    func_8004C6B0(D_80120744, D_80120748, D_80120740, D_8012074C);
    func_80011DFC();
}

void func_800F8188(void) {
    D_80120678 = D_8012053C;
    D_8012067C = D_80120554;
    D_80120680 = D_8012056C;
    func_800F7590();
    func_80012D64(D_8012077C, 0x11, 1, 2, 0);
    func_8004C46C(D_80120780, D_80120784, D_80120788, D_8012078C, D_80120790, D_80120794);
    func_8004C6B0(D_80120770, D_80120774, D_8012076C, D_80120778);
    D_800EAFA0[4] = D_800EAFA0[0x48] = 8;
    D_800B0A04[8].unk_02 += 1;
    D_800B0A04[8].unk_06 += 1;
    D_800B0A04[8].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void func_800F82B0(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void func_800F82D8(void) {
    func_800469F4(0x4028);
    func_80011DFC();
}

void func_800F8300(void) {
    func_800469F4(0x4646);
    func_80011DFC();
}

void func_800F8328(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void func_800F8350(void) {
    func_800469F4(0x4795);
    func_80011DFC();
}

void func_800F8378(void) {
    func_800469F4(0x444F);
    func_80011DFC();
}

void func_800F83A0(void) {
    func_800469F4(0x4755);
    func_80011DFC();
}

void func_800F83C8(void) {
    func_800469F4(0x42E8);
    func_80011DFC();
}

void func_800F83F0(void) {
    func_800469F4(0x4782);
    func_80011DFC();
}

void func_800F8418(void) {
    func_800469F4(0x43ED);
    func_80011DFC();
}

void func_800F8440(void) {
    func_800469F4(0x43E3);
    func_80011DFC();
}

void func_800F8468(void) {
    func_800469F4(0x47EA);
    func_80011DFC();
}

void func_800F8490(void) {
    func_800469F4(0x44E5);
    func_80011DFC();
}

void func_800F84B8(void) {
    func_800469F4(0x47C7);
    func_80011DFC();
}

typedef struct {
    void (*f[41])();
} FnTbl41; /* size 0xA4 */
extern FnTbl41 D_80120888;

void func_800F84E0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_80120888;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_800EECE0 != 0) {
        func_800F8590();
    }
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F7220", func_800F8590);

void func_800F8654(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7A63);
    func_800F72D0();
    func_80011DFC();
}

void func_800F868C(void) {
    D_80094714 = (u16) D_80094714 + D_800EECBC;
    func_80011DFC();
}

typedef struct {
    void (*f[35])();
} FnTbl35; /* size 0x8C */
extern FnTbl35 D_8012092C;

void func_800F86C4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl35 tbl;

    tbl = D_8012092C;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    func_800F8878();
    if (D_800B1AF6 == 0xE) {
        func_800F8970();
    }
}

void func_800F8784(void) {
    func_80015D28(0x45, 0x801B0000, 0x7AA0);
    func_800F7380();
    func_80011DFC();
}

void func_800F87BC(void) {
    func_8004C90C();
    D_800EAFA0[0x8B] |= 0x80;
    D_800EAFA0[0xCF] |= 0x80;
    D_800EAFA0[0x8F] = D_800EAFA0[7];
    D_800EAFA0[0xD3] = D_800EAFA0[7];
}

void func_800F8818(void) {
    func_8004C984();
    if (D_800EAFA0[7] == 0) {
        D_800EAFA0[0x8B] &= 0xFF7F;
        D_800EAFA0[0xCF] &= 0xFF7F;
    }
    D_800EAFA0[0x8F] = D_800EAFA0[7];
    D_800EAFA0[0xD3] = D_800EAFA0[7];
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F7220", func_800F8878);

void func_800F8970(void) {
    func_800F9730(-0x96, -0x78, -0x9B, -0x78, -0x50, 0x28, -0x69, 0x28, 0xFFFF00, 0xFFFF00, 9, 1);
    func_800F9730(-0xA0, -0x78, -0xA5, -0x78, -0x64, 0x28, -0x73, 0x28, 0xFFFFFF, 0xFFFFFF, 9, 1);
    func_800F9730(-0xA2, -0x78, -0xA7, -0x78, -0x6E, 0x28, -0x87, 0x28, 0xFFFF00, 0xFFFF00, 9, 1);
    func_8001A0CC(0, 0, 9, 3, 0);
    func_800F9730(-0x7D, -0x78, -0x82, -0x78, -0x28, 0x28, -0x32, 0x28, 0xFFFF00, 0xFFFF00, 0xD, 1);
    func_800F9730(-0x84, -0x78, -0x89, -0x78, -0x3C, 0x28, -0x46, 0x28, 0xFFFFFF, 0xFFFFFF, 0xD, 1);
    func_8001A0CC(0, 0, 0xD, 3, 0);
}

typedef struct {
    void (*f[47])();
} FnTbl47; /* size 0xBC */
extern FnTbl47 D_801209B8;

void func_800F8B50(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl47 tbl;

    tbl = D_801209B8;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_800B1AF6 == 0xD) {
        func_800F8C2C();
    }
}

void func_800F8BF4(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7AE5);
    func_800F7430();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F7220", func_800F8C2C);

typedef struct {
    void (*f[42])();
} FnTbl42; /* size 0xA8 */
extern FnTbl42 D_80120A94;

void func_800F93FC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_80120A94;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_800F9470(void) {
    func_800433D0(0x503);
    func_80011DFC();
}

void func_800F9498(void) {
    func_80015D28(0xD, 0x801D0000, 0x8D5B);
    func_800F74E0();
    func_80011DFC();
}

void func_800F94D0(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7B22);
    func_800F74E0();
    func_80011DFC();
}

void func_800F9508(void) {
    if (((u8)func_8002328C(D_800B1746) & 0x7F) >= 2U) {
        D_80094714 += 3;
    }
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800F7220", func_800F9558);
