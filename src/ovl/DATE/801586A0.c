#define MAIN_API_OVERRIDE_D_80122CD0 /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/DATE.h"

extern u32 D_80122CD0;

typedef struct {
    void (*f[43])();
} FnTbl43; /* size 0xAC */
extern FnTbl43 D_80160668;

void func_801586A0(void) {
    D_80160440 = 0x801CE124;
    D_80160444 = 0x801CE128;
    D_80160448 = 0x801CE148;
    D_8016044C = *(s16 *)0x801CE15C;
    D_80160450 = 0x801B0000;
    D_80160454 = 0x801B2000;
    D_80160458 = 0x801B6000;
    D_8016045C = 0x801BA000;
    D_80160460 = 0x801BE000;
    D_80160464 = 0x801C2000;
    D_80160468 = 0x801C6000;
}

void func_80158750(void) {
    D_8016046C = 0x801D2240;
    D_80160470 = 0x801D2248;
    D_80160474 = 0x801D2288;
    D_80160478 = *(s16 *)0x801D22A0;
    D_8016047C = 0x801B0000;
    D_80160480 = 0x801B2000;
    D_80160484 = 0x801B6000;
    D_80160488 = 0x801BA000;
    D_8016048C = 0x801BE000;
    D_80160490 = 0x801C2000;
    D_80160494 = 0x801C6000;
}

void func_80158800(void) {
    D_80160498 = 0x801CE0F4;
    D_8016049C = 0x801CE0F8;
    D_801604A0 = 0x801CE118;
    D_801604A4 = *(s16 *)0x801CE12C;
    D_801604A8 = 0x801B0000;
    D_801604AC = 0x801B2000;
    D_801604B0 = 0x801B6000;
    D_801604B4 = 0x801BA000;
    D_801604B8 = 0x801BE000;
    D_801604BC = 0x801C2000;
    D_801604C0 = 0x801C6000;
}

void func_801588B0(void) {
    D_801604C4 = 0x801CE1A0;
    D_801604C8 = 0x801CE1A4;
    D_801604CC = 0x801CE1D4;
    D_801604D0 = *(s16 *)0x801CE1F0;
    D_801604D4 = 0x801B0000;
    D_801604D8 = 0x801B2000;
    D_801604DC = 0x801B6000;
    D_801604E0 = 0x801BA000;
    D_801604E4 = 0x801BE000;
    D_801604E8 = 0x801C2000;
    D_801604EC = 0x801C6000;
}

void func_80158960(void) {
    switch (D_80122CD0) {
    case 2:
        func_8015908C();
        return;
    case 3:
        func_801591BC();
        return;
    case 4:
        func_801592A0();
        return;
    case 5:
        func_80159354();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_801589F4(void) {
    D_8015E208 = D_8015DF48;
    D_8015E20C = D_8015E084;
    D_8015E210 = D_8015E1C0;
    func_801586A0();
    func_80043914(D_80160450, 0x11, 1, 2, 0);
    func_80084E90(D_80160454, D_80160458, D_8016045C, D_80160460, D_80160464, D_80160468);
    func_800850D4(D_80160444, D_80160448, D_80160440, (s32) D_8016044C);
    D_800E6280.unk_1BC[4].unk_02 += 3;
    D_800E6280.unk_1BC[4].unk_06 += 2;
    D_800E6280.unk_1BC[4].unk_0A -= 0x14;
    D_800E6280.unk_0FC.unk_02 += 0xA;
    func_80084D3C();
    func_8004284C();
}

void func_80158B1C(void) {
    D_8015E208 = D_8015DF4C;
    D_8015E20C = D_8015E088;
    D_8015E210 = D_8015E1C4;
    func_80158750();
    func_80043914(D_8016047C, 0x11, 1, 2, 0);
    func_80084E90(D_80160480, D_80160484, D_80160488, D_8016048C, D_80160490, D_80160494);
    func_800850D4(D_80160470, D_80160474, D_8016046C, (s32) D_80160478);
    D_800CA360 = 1;
    D_80122D0C = 1;
    D_800E6280.unk_1BC[4].unk_02 += 3;
    D_800E6280.unk_1BC[4].unk_06 += 2;
    D_800E6280.unk_1BC[4].unk_0A -= 0x14;
    func_80084D3C();
    D_800CA224[0] = 3;
    D_800CA224[1] = 3;
    D_800CA224[2] = 3;
    D_800CA234[0] = 2;
    D_800CA234[1] = 2;
    D_800CA234[2] = 2;
    func_8004284C();
}

void func_80158C8C(void) {
    D_8015E208 = D_8015DF50;
    D_8015E20C = D_8015E08C;
    D_8015E210 = D_8015E1C8;
    func_80158800();
    func_80043914(D_801604A8, 0x11, 1, 2, 0);
    func_80084E90(D_801604AC, D_801604B0, D_801604B4, D_801604B8, D_801604BC, D_801604C0);
    func_800850D4(D_8016049C, D_801604A0, D_80160498, (s32) D_801604A4);
    D_800CA360 = 1;
    D_800E6280.unk_1BC[4].unk_06 += 2;
    D_800E6280.unk_1BC[4].unk_0A -= 0x14;
    func_80084D3C();
    func_8004284C();
}

void func_80158D98(void) {
    D_8015E208 = D_8015DF54;
    D_8015E20C = D_8015E090;
    D_8015E210 = D_8015E1CC;
    func_801588B0();
    func_80043914(D_801604D4, 0x11, 1, 2, 0);
    func_80084E90(D_801604D8, D_801604DC, D_801604E0, D_801604E4, D_801604E8, D_801604EC);
    func_800850D4(D_801604C8, D_801604CC, D_801604C4, (s32) D_801604D0);
    D_800CA360 = 1;
    D_800E6280.unk_1BC[4].unk_02 += 1;
    D_800E6280.unk_1BC[4].unk_06 += 1;
    D_800E6280.unk_1BC[4].unk_0A -= 0x14;
    func_80084D3C();
    D_800CA21C[0] = 2;
    D_800CA21C[2] = 2;
    D_800CA21C[1] = 2;
    D_800CA224[0] = 2;
    D_800CA224[2] = 2;
    D_800CA224[1] = 2;
    D_800CA22C[0] = 2;
    D_800CA22C[2] = 2;
    D_800CA22C[1] = 2;
    D_800CA234[0] = 1;
    D_800CA234[2] = 1;
    D_800CA234[1] = 1;
    func_8004284C();
}

void func_80158F44(void) {
    bg_read_sub2(0x47D6);
    func_8004284C();
}

void func_80158F6C(void) {
    bg_read_sub2(0x42EB);
    func_8004284C();
}

void func_80158F94(void) {
    bg_read_sub2(0x48E0);
    func_8004284C();
}

void func_80158FBC(void) {
    bg_read_sub2(0x45BF);
    func_8004284C();
}

void func_80158FE4(void) {
    bg_read_sub2(0x490A);
    func_8004284C();
}

void func_8015900C(void) {
    bg_read_sub2(0x45F8);
    func_8004284C();
}

void func_80159034(void) {
    bg_read_sub2(0x4868);
    func_8004284C();
}

void func_8015905C(void) {
    func_80044750(0x203);
    bg_read_sub2(0x4520);
    func_8004284C();
}

typedef struct {
    void (*f[38])();
} FnTbl38; /* size 0x98 */
extern FnTbl38 D_801604F0;

void func_8015908C(void) {
    s32 idx; /* FAKE: never read; declared first so tbl lands at the original frame offset (T-3330) */
    FnTbl38 tbl;

    tbl = D_801604F0;
    if (D_800E6280.unk_110A < 8U && D_800B593C == 0x80) {
        func_8006B900();
    }
    tbl.f[D_800E6280.unk_110A](0x80);
}

void func_8015913C(void) {
    func_80046318(0x3D, 0x801B0000, 0x8C4B);
    func_801586A0();
    func_8004284C();
}

void func_80159174(void) {
    if (D_800E6280.unk_56C[27] != 0) {
        D_800CA150 = (u16) D_800CA150 + 1;
        D_800CA154 = 0;
    }
    func_8004284C();
}

typedef struct {
    void (*f[56])();
} FnTbl56; /* size 0xE0 */
extern FnTbl56 D_80160588;

void func_801591BC(void) {
    s32 idx; /* FAKE: never read; declared first so tbl lands at the original frame offset (T-3330) */
    FnTbl56 tbl;

    tbl = D_80160588;
    if (D_80122D04 == 0 && D_800B593C == 0x80) {
        func_8006B900();
    }
    tbl.f[D_800E6280.unk_110A](0x80);
}

void func_80159268(void) {
    func_80046318(0x45, 0x801B0000, 0x8C88);
    func_80158750();
    func_8004284C();
}

void func_801592A0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl43 tbl;

    tbl = D_80160668;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8015931C(void) {
    func_80046318(0x3D, 0x801B0000, 0x8CCD);
    func_80158800();
    func_8004284C();
}

typedef struct {
    void (*f[51])();
} FnTbl51; /* size 0xCC */
extern FnTbl51 D_80160714;

void func_80159354(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl51 tbl;

    tbl = D_80160714;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_801593C8(void) {
    D_800E6280.unk_F5F = 4;
    func_8014C5C8();
}

void func_801593F0(void) {
    D_800E6280.unk_F5F = 0xE;
    func_8014C5C8();
}

void func_80159418(void) {
    func_80046318(0x3D, 0x801B0000, 0x8D0A);
    func_801588B0();
    func_8004284C();
}

void func_80159450(void) {
    D_800E6280.unk_F5F = 0xE;
    D_80120666 = 4;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801586A0", func_80159484);
