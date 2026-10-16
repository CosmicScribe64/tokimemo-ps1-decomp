#include "common.h"
#include "ovl/EVENT.h"

void func_800FDC20(void) {
    D_80121800 = 0x801CE11C;
    D_80121804 = 0x801CE120;
    D_80121808 = 0x801CE140;
    D_8012180C = *(s16 *)0x801CE154;
    D_80121810 = 0x801B0000;
    D_80121814 = 0x801B2000;
    D_80121818 = 0x801B6000;
    D_8012181C = 0x801BA000;
    D_80121820 = 0x801BE000;
    D_80121824 = 0x801C2000;
    D_80121828 = 0x801C6000;
}

void func_800FDCD0(void) {
    D_8012182C = 0x801CE0DC;
    D_80121830 = 0x801CE0E0;
    D_80121834 = 0x801CE100;
    D_80121838 = *(s16 *)0x801CE114;
    D_8012183C = 0x801B0000;
    D_80121840 = 0x801B2000;
    D_80121844 = 0x801B6000;
    D_80121848 = 0x801BA000;
    D_8012184C = 0x801BE000;
    D_80121850 = 0x801C2000;
    D_80121854 = 0x801C6000;
}

void func_800FDD80(void) {
    D_80121858 = 0x801D2148;
    D_8012185C = 0x801D2150;
    D_80121860 = 0x801D2170;
    D_80121864 = *(s16 *)0x801D218C;
    D_80121868 = 0x801B0000;
    D_8012186C = 0x801B2000;
    D_80121870 = 0x801B6000;
    D_80121874 = 0x801BA000;
    D_80121878 = 0x801BE000;
    D_8012187C = 0x801C2000;
    D_80121880 = 0x801C6000;
}

void func_800FDE30(void) {
    D_80121884 = 0x801CE11C;
    D_80121888 = 0x801CE120;
    D_8012188C = 0x801CE140;
    D_80121890 = *(s16 *)0x801CE154;
    D_80121894 = 0x801B0000;
    D_80121898 = 0x801B2000;
    D_8012189C = 0x801B6000;
    D_801218A0 = 0x801BA000;
    D_801218A4 = 0x801BE000;
    D_801218A8 = 0x801C2000;
    D_801218AC = 0x801C6000;
}

void func_800FDEE0(void) {
    D_801218B0 = 0x801CE07C;
    D_801218B4 = 0x801CE080;
    D_801218B8 = 0x801CE090;
    D_801218BC = *(s16 *)0x801CE0A4;
    D_801218C0 = 0x801B0000;
    D_801218C4 = 0x801B2000;
    D_801218C8 = 0x801B6000;
    D_801218CC = 0x801BA000;
    D_801218D0 = 0x801BE000;
    D_801218D4 = 0x801C2000;
    D_801218D8 = 0x801C6000;
}

void func_800FDF90(void) {
    switch (D_800EECB0) {
    case 1:
        func_800FE030();
        return;
    case 2:
        func_800FEA38();
        return;
    case 3:
        func_800FF61C();
        return;
    case 4:
        func_800FF770();
        return;
    case 5:
        func_800FF82C();
        return;
    default:
        func_80015FE0();
        return;
    }
}

void func_800FE090(void);

void func_800FE030(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_800FE090();
        return;
    }
    func_80015FE0();
}

void func_800FE090(void) {
    s32 pad; /* FAKE: unused local above tmp; the original frame is 4 bytes larger, real source unknown. T-4080 */
    FnTbl32 tmp;

    tmp = D_801218E8;
    func_80078950("s%d\n", D_800B1AF6);
}

void func_800FE100(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7F2A);
    func_800FDC20();
    func_80011DFC();
}

void func_800FE138(void) {
    D_800B1746 = 3;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884("");
    D_800E96EF = 0;
    D_800E9733 = 0;
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    func_800F6000();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    D_800F19AB = 0;
    func_8004BC20(D_800B1746);
    func_8004CDDC();
    func_800F6000();
    D_80120678 = D_801203B4;
    D_8012067C = D_801203CC;
    D_80120680 = D_801203E4;
    func_80078970(D_80094784, "校舎裏");
    D_800B0A04[3].unk_06 += 2;
    func_8004C250();
    func_80011DFC();
}

void func_800FE270(void) {
    func_800FDC20();
    func_80012D64(D_80121810, 0x11, 1, 2, 0);
    func_8004C46C(D_80121814, D_80121818, D_8012181C, D_80121820, D_80121824, D_80121828);
    func_8004C6B0(D_80121804, D_80121808, D_80121800, (s32) D_8012180C);
    func_80011DFC();
}

void func_800FE318(void) {
    s16 i;

    for (i = 0; i < 0xC; i++) {
        D_801218DC[i] = func_800789A0() % 4;
    }
    D_80120678 = D_801203B8;
    D_8012067C = D_801203D0;
    D_80120680 = D_801203E8;
    func_800FDCD0();
    func_80012D64(D_8012183C, 0x11, 1, 2, 0);
    func_8004C46C(D_80121840, D_80121844, D_80121848, D_8012184C, D_80121850, D_80121854);
    func_8004C6B0(D_80121830, D_80121834, D_8012182C, (s32) D_80121838);
    D_800EECEC = 2;
    D_800EAFB8 = 3;
    D_800B0A04[3].unk_02 += 2;
    D_800B0A04[3].unk_06 += 1;
    D_800B0A04[3].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void func_800FE494(void) {
    D_80120678 = D_801203BC;
    D_8012067C = D_801203D4;
    D_80120680 = D_801203EC;
    func_800FDD80();
    func_80012D64(D_80121868, 0x11, 1, 2, 0);
    func_8004C46C(D_8012186C, D_80121870, D_80121874, D_80121878, D_8012187C, D_80121880);
    func_8004C6B0(D_8012185C, D_80121860, D_80121858, D_80121864);
    func_800189C4(0x62);
    D_800EB029 = 9;
    D_800EB02A = 1;
    D_800EB060 = 0x41000000;
    D_800EB02B = 4;
    D_800EB034 = D_8012185C;
    D_800EB038 = D_80121860;
    D_800EB05C = D_80121858;
    D_800EB03C = D_80121864;
    D_800EB03E = 4;
    D_800EB040 = 0;
    D_800EB02E = 1;
    D_800EB06B = 0x11;
    D_800EB04E = -0xA0;
    D_800EB052 = -0x78;
    D_800EB02F = 0;
    D_800EB02D = 0;
    D_800EB02C = 0;
    D_800B0A04[3].unk_02 -= 1;
    D_800B0A04[3].unk_06 -= 1;
    D_800B0A04[3].unk_0A += 0x14;
    func_8004C250();
    D_80124394 = 3;
    D_80124396 = 3;
    D_80124398 = 3;
    D_801243A4 = 2;
    D_801243A6 = 2;
    D_801243A8 = 2;
    func_80011DFC();
}

void func_800FE6C0(void) {
    D_80120678 = D_801203C0;
    D_8012067C = D_801203D8;
    D_80120680 = D_801203F0;
    func_800FDE30();
    func_80012D64(D_80121894, 0x11, 1, 2, 0);
    func_8004C46C(D_80121898, D_8012189C, D_801218A0, D_801218A4, D_801218A8, D_801218AC);
    func_8004C6B0(D_80121888, D_8012188C, D_80121884, D_80121890);
    D_800B0A04[3].unk_02 += 2;
    D_800B0A04[3].unk_06 += 1;
    D_800B0A04[3].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void func_800FE7D4(void) {
    D_80120678 = D_801203C4;
    D_8012067C = D_801203DC;
    D_80120680 = D_801203F4;
    func_800FDEE0();
    func_80012D64(D_801218C0, 0x11, 1, 2, 0);
    func_8004C46C(D_801218C4, D_801218C8, D_801218CC, D_801218D0, D_801218D4, D_801218D8);
    func_8004C6B0(D_801218B4, D_801218B8, D_801218B0, (s32) D_801218BC);
    func_80011DFC();
}

void func_800FE8A8(void) {
    func_800469F4(0x4015);
    func_80011DFC();
}

void func_800FE8D0(void) {
    func_800469F4(0x462C);
    func_80011DFC();
}

void func_800FE8F8(void) {
    func_800469F4(0x4862);
    func_80011DFC();
}

void func_800FE920(void) {
    func_800469F4(0x458C);
    func_80011DFC();
}

void func_800FE948(void) {
    func_800469F4(0x480C);
    func_80011DFC();
}

void func_800FE970(void) {
    func_800469F4(0x451C);
    func_80011DFC();
}

void func_800FE998(void) {
    func_800469F4(0x479D);
    func_80011DFC();
}

void func_800FE9C0(void) {
    func_800469F4(0x446A);
    func_80011DFC();
}

void func_800FE9E8(void) {
    func_800469F4(0x4847);
    func_80011DFC();
}

void func_800FEA10(void) {
    func_800469F4(0x455F);
    func_80011DFC();
}

void func_800FEA38(void) {
    s32 idx; /* FAKE: unused local declared before the table, the original frame has the table 4 bytes lower (T-3330) */
    FnTbl49 tbl;

    tbl = D_80121968;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6]();
    if (D_800B1AF6 == 0x17 || D_800B1AF6 == 0x18) {
        func_800FEF74();
    }
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FEAE8);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FEDC8);

/* bulk_m2c declarations (T-11190) */
void func_800FF320();
/* end bulk_m2c declarations */
s32 func_800FEF74(void) {
    s16 temp_s0;
    s16 var_s0;

    if (((u32) D_800B1AE4 % 51U) == 0) {
        var_s0 = 0;
        do {
            temp_s0 = var_s0 + 1;
            D_801218DC[var_s0] = (s8) (func_800789A0() % 4);
            var_s0 = temp_s0;
        } while (temp_s0 < 0xC);
    }
    func_800FF320((s32) (s16) ((u32) (D_800B1AE4 * 0x10) % 1000U), 0, 9);
    func_800FF320((s16) ((u32) (D_800B1AE4 * 0x10) % 1000U) - 0xA, 1, 9);
    func_800FF320((s16) ((u32) (D_800B1AE4 * 0x10) % 1000U) - 0x14, 2, 9);
    func_800FF320((s32) (s16) ((u32) ((D_800B1AE4 * 0x10) + 0x1F4) % 1000U), 6, 9);
    func_800FF320((s16) ((u32) ((D_800B1AE4 * 0x10) + 0x1F4) % 1000U) - 0xA, 7, 9);
    func_800FF320((s16) ((u32) ((D_800B1AE4 * 0x10) + 0x1F4) % 1000U) - 0x14, 8, 9);
    func_8001A0CC(0, 0, 9, 3, 0);
    func_800FF320((s32) (s16) (0x320 - ((u32) (D_800B1AE4 * 0x10) % 800U)), 3, 0xD);
    func_800FF320((s16) (0x320 - ((u32) (D_800B1AE4 * 0x10) % 800U)) + 0xA, 4, 0xD);
    func_800FF320((s16) (0x320 - ((u32) (D_800B1AE4 * 0x10) % 800U)) + 0x14, 5, 0xD);
    func_800FF320((s32) (s16) (0x320 - ((u32) ((D_800B1AE4 * 0x10) + 0x190) % 800U)), 9, 0xD);
    func_800FF320((s16) (0x320 - ((u32) ((D_800B1AE4 * 0x10) + 0x190) % 800U)) + 0xA, 0xA, 0xD);
    func_800FF320((s16) (0x320 - ((u32) ((D_800B1AE4 * 0x10) + 0x190) % 800U)) + 0x14, 0xB, 0xD);
    func_8001A0CC(0, 0, 0xD, 3, 0);
}

void func_800FF320(arg0, arg1, arg2)
    s16 arg0;
    s16 arg1;
    s16 arg2;
{
    /* Port of the DATE clone func_80152684 (T-17400). FAKE: 8 unused bytes above tbl give the original's frame and tbl offset, as in the clone (T-9310). */
    s32 pad[2];
    EvTbl tbl;

    tbl = D_80121AD8;
    if (arg0 < 0x140) {
        func_800FFA4C(0x140, -0x118, 0xA0 - arg0, 0x28, (0xA0 - arg0) + (s16)(arg0 / 32 + 0xA), 0x28, tbl.e[*(u8 *)&D_801218DC[arg1]].a, tbl.e[*(u8 *)&D_801218DC[arg1]].b, arg2, 1);
        return;
    }
    if (arg0 < 0x154) {
        func_800FFA4C(0x140, -0x118, -0xA0, 0x28 - (s16)((arg0 * 7 - 0x8C0) / 8), -0xA0, 0x28, tbl.e[*(u8 *)&D_801218DC[arg1]].a, tbl.e[*(u8 *)&D_801218DC[arg1]].b, arg2, 1);
        func_800FFA4C(0x140, -0x118, -0xA0, 0x28, (s16)(arg0 / 32 + 0xA) - arg0 + 0xA0, 0x28, tbl.e[*(u8 *)&D_801218DC[arg1]].a, tbl.e[*(u8 *)&D_801218DC[arg1]].b, arg2, 1);
        return;
    }
    func_800FFA4C(0x140, -0x118, -0xA0, 0x28 - (s16)((arg0 * 7 - 0x8C0) / 8), -0xA0, (0x28 - (s16)((arg0 * 7 - 0x8C0) / 8)) + (s16)(0xF - (arg0 - 0x140) / 32), tbl.e[*(u8 *)&D_801218DC[arg1]].a, tbl.e[*(u8 *)&D_801218DC[arg1]].b, arg2, 1);
}

void func_800FF5E4(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7F67);
    func_800FDCD0();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FF61C);

void func_800FF710(void) {
    func_80015D28(0x45, 0x801B0000, 0x7FA4);
    func_800FDD80();
    func_80011DFC();
}

void func_800FF748(void) {
    D_800EB02A = 5;
    func_80011DFC();
}

void func_800FF770(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_80121BB8;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_800FF7F4(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7FE9);
    func_800FDE30();
    func_80011DFC();
}

void func_800FF82C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl45 tbl;

    tbl = D_80121C60;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_800FF8C0(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8026);
    func_800FDEE0();
    func_80011DFC();
}

void func_800FF8F8(void) {
    if (D_800EECBC == 1) {
        D_80094714 += 2;
        D_80094718 = 0;
        D_800B0A04[3].unk_02 += 2;
        D_800B0A04[3].unk_06 += 2;
        D_800B0A04[3].unk_0A -= 0x14;
        func_8004C250();
        func_80011DFC();
    } else {
        D_800B0A04[3].unk_02 += 3;
        D_800B0A04[3].unk_06 += 1;
        D_800B0A04[3].unk_0A -= 0x14;
        func_8004C250();
    }
    func_80011DFC();
}

void func_800FF9D4(void) {
    if (D_800EECBC == 0) {
        func_80011DFC();
        func_80011DFC();
    }
    func_80011DFC();
}

void func_800FFA10(void) {
    if (D_800EECBC == 0) {
        func_8004A734();
        return;
    }
    func_8004A764();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FFA4C);
