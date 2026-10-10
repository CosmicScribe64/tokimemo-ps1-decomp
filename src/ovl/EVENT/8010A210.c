#include "common.h"
#include "ovl/EVENT.h"

void func_8010A210(void) {
    D_80123E90 = 0x801B0000;
    D_80123E94 = 0x801B2000;
    D_80123E98 = 0x801B6000;
    D_80123E9C = 0x801BA000;
    D_80123EA0 = 0x801BE000;
    D_80123EA4 = 0x801C2000;
    D_80123EA8 = 0x801C6000;
}

void func_8010A280(void) {
    D_80123EAC = 0x801CE150;
    D_80123EB0 = 0x801CE154;
    D_80123EB4 = 0x801CE1A4;
    D_80123EB8 = *(s16 *)0x801CE1BC;
    D_80123EBC = 0x801B0000;
    D_80123EC0 = 0x801B2000;
    D_80123EC4 = 0x801B6000;
    D_80123EC8 = 0x801BA000;
    D_80123ECC = 0x801BE000;
    D_80123ED0 = 0x801C2000;
    D_80123ED4 = 0x801C6000;
}

void func_8010A330(void) {
    switch (D_800EECB0) {
    case 1:
        func_8010A3D0();
        break;
    case 2:
        func_8010A430();
        break;
    case 3:
        func_8010AD70();
        break;
    case 4:
        func_8010A490();
        break;
    case 5:
        func_8010B29C();
        break;
    default:
        func_80015FE0();
        break;
    }
}

void func_8010A3D0(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_8010A4F0();
    } else {
        func_80015FE0();
    }
}


void func_8010A430(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_8010A5AC();
    } else {
        func_80015FE0();
    }
}


void func_8010A490(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_8010AE7C();
    } else {
        func_80015FE0();
    }
}


typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 D_80123ED8;

void func_8010A4F0(void) {
    s32 idx; /* unused: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_80123ED8;
    func_80078950("s%d %d\n", D_800B1AF6, D_80094714);
    tbl.f[D_800B1AF6]();
}


void func_8010A584(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_80123F30;

void func_8010A5AC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_80123F30;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_8010A628(void) {
    func_80015D28(0, 0x801B0000, 0);
    func_80011DFC();
}

void func_8010A658(void) {
    func_80012D64(D_80123E90, 0x11, 1, 2, 0);
    func_8004C46C(D_80123E94, D_80123E98, D_80123E9C, D_80123EA0, D_80123EA4, D_80123EA8);
    func_8004C6B0(0, 0, 0, 0);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A6E4);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A790);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A7E0);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A91C);

void func_8010AAAC(void) {
    D_80120678 = D_8012062C;
    D_8012067C = D_8012064C;
    D_80120680 = D_8012066C;
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010AAF8);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010AC40);

void func_8010ACF8(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void func_8010AD20(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void func_8010AD48(void) {
    func_800469F4(0x4678);
    func_80011DFC();
}

typedef struct {
    void (*f[14])();
} FnTbl14; /* size 0x38 */
extern FnTbl14 D_80123F94;

void func_8010AD70(void) {
    s32 idx; /* unused: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_80123F94;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6](0x80);
}

void func_8010AE04(void) {
    func_800469F4(0x4284);
    func_80011DFC();
}

void func_8010AE2C(void) {
    func_800469F4(0x4296);
    func_80011DFC();
}

void func_8010AE54(void) {
    func_800469F4(0x473D);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010AE7C);

void func_8010AF40(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8BD8);
    func_8010A280();
    func_80011DFC();
}

void func_8010AF78(void) {
    func_800143DC(1, 0xB19B, 0xB178, 0xBE66, 0xBE24, 0xBE21);
    func_80011DFC();
}

void func_8010AFBC(void) {
    func_800335E0(0x692F);
    func_80011DFC();
}

void func_8010AFE4(void) {
    func_8001A1EC(0x120, -0xF8, 0xA0, 0x3C, -0xA0, 0x3C, 9, 0x808080, 0);
    func_8001A1EC(0x120, -0xF8, 0x40, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0);
    func_8001A1EC(0x120, -0xF8, 0x10, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x40);
    func_8001A1EC(0x120, -0xF8, -0x1C, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x63);
    func_8001A1EC(0x120, -0xF8, -0x50, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x24);
    func_8001A1EC(0x120, -0xF8, -0x8C, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x73);
    func_8001A1EC(0x120, -0xF8, -0xF0, 0x3C, 0, 0xB4, 9, 0xC0C0C0, 0x11);
    func_8001A1EC(0x120, -0xF8, -0x140, 0x3C, 0, 0xB4, 9, 0xC0C0C0, 0x50);
    func_8001A1EC(0x120, -0xF8, -0x30, 0x3C, 0, 0xB4, 9, 0x808080, 0);
    func_8001A0CC(0, 0, 9, 1, 0);
}

void func_8010B228(void) {
    func_800F7074();
    D_80094714 = (u16) D_80094714 + 1;
    D_80094718 = 0;
    func_80011DFC();
}

void func_8010B268(void) {
    func_800201FC(1);
    D_800EECC0 = 0;
    D_800EECCC = 0;
    func_80011DFC();
}

extern FnTbl14 D_801240AC;

void func_8010B29C(void) {
    s32 idx; /* unused: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_801240AC;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6](0x80);
}

void func_8010B330(void) {
    func_800335E0(0x692F);
    func_80011DFC();
}
