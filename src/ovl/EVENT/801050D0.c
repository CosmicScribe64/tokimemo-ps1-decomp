#include "common.h"
#include "ovl/EVENT.h"

void func_801050D0(void) {
    D_80122F70 = 0x801D22F4;
    D_80122F74 = 0x801D22F8;
    D_80122F78 = 0x801D2318;
    D_80122F7C = *(s16 *)0x801D232C;
    D_80122F80 = 0x801B0000;
    D_80120C04 = 0x801D2000;
    D_80122F84 = 0x801B2000;
    D_80122F88 = 0x801B6000;
    D_80122F8C = 0x801BA000;
    D_80122F90 = 0x801BE000;
    D_80122F94 = 0x801C2000;
    D_80122F98 = 0x801C6000;
    D_80120C20 = 0x801CE000;
}

void func_801051A0(void) {
    D_80122F9C = 0x801CE090;
    D_80122FA0 = 0x801CE094;
    D_80122FA4 = 0x801CE0AC;
    D_80122FA8 = *(s16 *)0x801CE0C0;
    D_80122FAC = 0x801B0000;
    D_80122FB0 = 0x801B2000;
    D_80122FB4 = 0x801B6000;
    D_80122FB8 = 0x801BA000;
    D_80122FBC = 0x801BE000;
    D_80122FC0 = 0x801C2000;
    D_80122FC4 = 0x801C6000;
}

void func_80105250(void) {
    D_80122FC8 = 0x801D2240;
    D_80122FCC = 0x801D2248;
    D_80122FD0 = 0x801D2288;
    D_80122FD4 = *(s16 *)0x801D22A0;
    D_80122FD8 = 0x801B0000;
    D_80122FDC = 0x801B2000;
    D_80122FE0 = 0x801B6000;
    D_80122FE4 = 0x801BA000;
    D_80122FE8 = 0x801BE000;
    D_80122FEC = 0x801C2000;
    D_80122FF0 = 0x801C6000;
}

void func_80105300(void) {
    D_80122FF4 = 0x801CE078;
    D_80122FF8 = 0x801CE07C;
    D_80122FFC = 0x801CE08C;
    D_80123000 = *(s16 *)0x801CE098;
    D_80123004 = 0x801B0000;
    D_80123008 = 0x801B2000;
    D_8012300C = 0x801B6000;
    D_80123010 = 0x801BA000;
    D_80123014 = 0x801BE000;
    D_80123018 = 0x801C2000;
    D_8012301C = 0x801C6000;
}

void func_801053B0(void) {
    D_80123020 = 0x801CE0F4;
    D_80123024 = 0x801CE0F8;
    D_80123028 = 0x801CE118;
    D_8012302C = *(s16 *)0x801CE12C;
    D_80123030 = 0x801B0000;
    D_80123034 = 0x801B2000;
    D_80123038 = 0x801B6000;
    D_8012303C = 0x801BA000;
    D_80123040 = 0x801BE000;
    D_80123044 = 0x801C2000;
    D_80123048 = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801050D0", func_80105460);

void func_80105510(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_801055D0();
        return;
    }
    func_80015FE0();
}

void func_80105570(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_80105910();
        return;
    }
    func_80015FE0();
}

typedef struct {
    void (*f[45])();
} FnTbl45; /* size 0xB4 */
extern FnTbl45 D_8012304C;

void func_801055D0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl45 tbl;

    tbl = D_8012304C;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    func_80105DF8();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801050D0", func_8010564C);

void func_80105830(void) {
    func_80015D28(0x45, 0x801B0000, 0x86E8);
    func_801050D0();
    func_80011DFC();
}

void func_80105868(void) {
    func_801050D0();
    func_80012D64(D_80122F80, 0x11, 1, 2, 0);
    func_8004C46C(D_80122F84, D_80122F88, D_80122F8C, D_80122F90, D_80122F94, D_80122F98);
    func_8004C6B0(D_80122F74, D_80122F78, D_80122F70, (s32) D_80122F7C);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801050D0", func_80105910);

void func_80105998(void) {
    if (D_800B0A04[D_800B1746].unk_06 < 0x50) {
        D_80094714 += 2;
    }
    func_80011DFC();
}

void func_801059FC(void) {
    if (D_800B0A04[D_800B1746].unk_06 < 0x50) {
        D_800B1AF6 += 0xE;
    }
    func_80011DFC();
}

void func_80105A60(void) {
    if (D_800B0A04[D_800B1746].unk_06 >= 0x50) {
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
    }
    func_80011DFC();
}

void func_80105AC8(void) {
    D_80094714 = (u16) D_80094714 + D_800EECBC;
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801050D0", func_80105B00);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801050D0", func_80105BC4);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801050D0", func_80105C8C);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801050D0", func_80105DF8);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801050D0", func_80106150);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801050D0", func_80106258);

void func_80106384(void) {
    D_80120678 = D_8012057C;
    D_8012067C = D_80120598;
    D_80120680 = D_801205B4;
    D_800B0A04[9].unk_02 += 2;
    D_800B0A04[9].unk_06 += 1;
    D_800B0A04[9].unk_0A -= 0x14;
    func_8004C250();
    func_80105250();
    func_80012D64(D_80122FD8, 0x11, 1, 2, 0);
    func_8004C46C(D_80122FDC, D_80122FE0, D_80122FE4, D_80122FE8, D_80122FEC, D_80122FF0);
    func_8004C6B0(D_80122FCC, D_80122FD0, D_80122FC8, D_80122FD4);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801050D0", func_8010649C);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801050D0", func_801065B4);

void func_801066CC(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void func_801066F4(void) {
    func_800469F4(0x4622);
    func_80011DFC();
}

void func_8010671C(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void func_80106744(void) {
    func_800469F4(0x475E);
    func_80011DFC();
}

void func_8010676C(void) {
    func_800469F4(0x42A8);
    func_80011DFC();
}

void func_80106794(void) {
    func_800469F4(0x4737);
    func_80011DFC();
}

void func_801067BC(void) {
    func_800469F4(0x47F3);
    func_80011DFC();
}

void func_801067E4(void) {
    func_800469F4(0x44F6);
    func_80011DFC();
}

void func_8010680C(void) {
    func_800469F4(0x44E5);
    func_80011DFC();
}

typedef struct {
    void (*f[47])();
} FnTbl47; /* size 0xBC */
extern FnTbl47 D_80123150;

void func_80106834(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl47 tbl;

    tbl = D_80123150;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_801068D0(void) {
    func_80015D28(0x3D, 0x801B0000, 0x872D);
    func_801051A0();
    func_80011DFC();
}

typedef struct {
    void (*f[63])();
} FnTbl63; /* size 0xFC */
extern FnTbl63 D_8012320C;

void func_80106908(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl63 tbl;

    tbl = D_8012320C;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_8010698C(void) {
    func_80015D28(0x45, 0x801B0000, 0x876A);
    func_80105250();
    func_80011DFC();
}

void func_801069C4(void) {
    func_800433D0(0x501);
    func_80011DFC();
}

void func_801069EC(void) {
    func_800433D0(0x503);
    func_80011DFC();
}

void func_80106A14(void) {
    func_800433D0(0x504);
    func_80011DFC();
}

void func_80106A3C(void) {
    func_800433D0(0x505);
    func_80011DFC();
}

void func_80106A64(void) {
    func_800433D0(0x506);
    func_80011DFC();
}

typedef struct {
    void (*f[36])();
} FnTbl36; /* size 0x90 */
extern FnTbl36 D_80123308;

void func_80106A8C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl36 tbl;

    tbl = D_80123308;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_80106B10(void) {
    func_80015D28(0x3D, 0x801B0000, 0x87AF);
    func_80105300();
    func_80011DFC();
}

extern FnTbl36 D_80123398;

void func_80106B48(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl36 tbl;

    tbl = D_80123398;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_80106BDC(void) {
    func_80015D28(0x3D, 0x801B0000, 0x87EC);
    func_801053B0();
    func_80011DFC();
}
