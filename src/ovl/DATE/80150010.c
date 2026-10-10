#include "common.h"
#include "ovl/DATE.h"

typedef struct {
    void (*f[40])();
} FnTbl40; /* size 0xA0 */

void func_80150010(void) {
    D_8015EA40 = 0x801B0000;
    D_8015EA44 = 0x801B2000;
    D_8015EA48 = 0x801B6000;
    D_8015EA4C = 0x801BA000;
    D_8015EA50 = 0x801BE000;
    D_8015EA54 = 0x801C2000;
    D_8015EA58 = 0x801C6000;
}

void func_80150080(void) {
    D_8015EA5C = (u8 *)0x801CE1A4;
    D_8015EA60 = (u8 *)0x801CE1A8;
    D_8015EA64 = (u8 *)0x801CE1E0;
    D_8015EA68 = *(s16 *)0x801CE1FC;
    D_8015EA6C = (u8 *)0x801B0000;
    D_8015EA70 = (u8 *)0x801B2000;
    D_8015EA74 = (u8 *)0x801B6000;
    D_8015EA78 = (u8 *)0x801BA000;
    D_8015EA7C = (u8 *)0x801BE000;
    D_8015EA80 = (u8 *)0x801C2000;
    D_8015EA84 = (u8 *)0x801C6000;
}

void func_80150130(void) {
    D_8015EA88 = (u8 *)0x801D2370;
    D_8015EA8C = (u8 *)0x801D2374;
    D_8015EA90 = (u8 *)0x801D23A4;
    D_8015EA94 = *(s16 *)0x801D23C0;
    D_8015EA98 = (u8 *)0x801B0000;
    D_8015EA9C = (u8 *)0x801D2000;
    D_8015EAA0 = (u8 *)0x801B2000;
    D_8015EAA4 = (u8 *)0x801B6000;
    D_8015EAA8 = (u8 *)0x801BA000;
    D_8015EAAC = (u8 *)0x801BE000;
    D_8015EAB0 = (u8 *)0x801C2000;
    D_8015EAB4 = (u8 *)0x801C6000;
    D_8015EAB8 = (u8 *)0x801CE000;
}

void func_80150200(void) {
    D_8015EABC = (u8 *)0x801D2174;
    D_8015EAC0 = (u8 *)0x801D217C;
    D_8015EAC4 = (u8 *)0x801D21A8;
    D_8015EAC8 = *(s16 *)0x801D21C4;
    D_8015EACC = (u8 *)0x801B0000;
    D_8015EAD0 = (u8 *)0x801B2000;
    D_8015EAD4 = (u8 *)0x801B6000;
    D_8015EAD8 = (u8 *)0x801BA000;
    D_8015EADC = (u8 *)0x801BE000;
    D_8015EAE0 = (u8 *)0x801C2000;
    D_8015EAE4 = (u8 *)0x801C6000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_801502B0);

void func_80150344(void) {
    D_8015E208 = D_8015DE8C;
    D_8015E20C = D_8015DFC8;
    D_8015E210 = D_8015E104;
    func_80150010();
    func_8004E9F4(0);
    func_8006612C("園内");
    func_8004E9F4(1);
    func_80043914(D_8015EA40, 0x11, 1, 2, 0);
    func_80084E90(D_8015EA44, D_8015EA48, D_8015EA4C, D_8015EA50, D_8015EA54, D_8015EA58);
    func_800850D4(0, 0, 0, 0);
    D_800E6280.unk_1BC[2].unk_02 += 3;
    D_800E6280.unk_1BC[2].unk_06 += 2;
    D_800E6280.unk_1BC[2].unk_0A -= 0x14;
    func_80084D3C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150460);

void func_801505B4(void) {
    D_8015E208 = D_8015DE94;
    D_8015E20C = D_8015DFD0;
    D_8015E210 = D_8015E10C;
    func_80150130();
    func_80043914((s32) D_8015EA98, 0x11, 1, 2, 0);
    func_80084E90((s32) D_8015EAA0, (s32) D_8015EAA4, (s32) D_8015EAA8, (s32) D_8015EAAC, (s32) D_8015EAB0, (s32) D_8015EAB4);
    func_800850D4((s32) D_8015EA8C, (s32) D_8015EA90, (s32) D_8015EA88, (s32) D_8015EA94);
    func_80048F64(0x62);
    D_801206D9 = 9;
    D_801206DA = 1;
    D_80120710 = 0x41000000;
    D_801206DB = 4;
    D_801206E4 = (s32) D_8015EA8C;
    D_801206E8 = (s32) D_8015EA90;
    D_8012070C = (s32) D_8015EA88;
    D_801206EC = D_8015EA94;
    D_801206EE = 4;
    D_801206F0 = 0;
    D_801206DE = 1;
    D_8012071B = 0x11;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
    D_801206DF = 0;
    D_801206DD = 0;
    D_801206DC = 0;
    func_80043914((s32) D_8015EA9C, 0x12, 1, 0, 0);
    D_800E6280.unk_1228[26] = 1;
    func_8004435C(0x280, 0x100, 0x40, 0x80, D_8015EAB8);
    D_80121FD4 = 0x81000000;
    D_80121FDC = 0x55;
    D_80121FDE = 0x38;
    D_80121FE2 = 0;
    D_80121FE3 = 0;
    D_80121FE4 = 0;
    D_80121FE0 = 0x1A;
    D_80121FE6 = 0x1F2;
    D_80121FE8 = 0;
    D_80121FE9 = 0;
    D_80121FEA = 0;
    D_80121FEC = 0;
    D_80121FEE = 0;
    D_80121FF0 = 0x1000;
    D_80121FF2 = 0x1000;
    D_80121FF4 = 0;
    D_80121FD8 = 5;
    D_80121FDA = -0x1E;
    D_800E6280.unk_10A5[57] = 0xC;
    D_800E6280.unk_1BC[2].unk_06 += 2;
    D_800E6280.unk_1BC[2].unk_0A -= 0xA;
    func_80084D3C();
    func_8004284C();
}

void func_80150890(void) {
    D_8015E208 = D_8015DE98;
    D_8015E20C = D_8015DFD4;
    D_8015E210 = D_8015E110;
    func_80150200();
    func_80043914((s32) D_8015EACC, 0x11, 1, 2, 0);
    func_80084E90((s32) D_8015EAD0, (s32) D_8015EAD4, (s32) D_8015EAD8, (s32) D_8015EADC, (s32) D_8015EAE0, (s32) D_8015EAE4);
    func_800850D4((s32) D_8015EAC0, (s32) D_8015EAC4, (s32) D_8015EABC, (s32) D_8015EAC8);
    D_800CA360 = 1;
    func_80048F64(0x62);
    D_801206D9 = 9;
    D_801206DA = 1;
    D_80120710 = 0x41000000;
    D_801206DB = 4;
    D_801206E4 = (s32) D_8015EAC0;
    D_801206E8 = (s32) D_8015EAC4;
    D_8012070C = (s32) D_8015EABC;
    D_801206EC = D_8015EAC8;
    D_801206EE = 4;
    D_801206F0 = 0;
    D_801206DE = 1;
    D_8012071B = 0x11;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
    D_801206DF = 0;
    D_801206DD = 0;
    D_801206DC = 0;
    D_800E6280.unk_1BC[2].unk_02 += 3;
    D_800E6280.unk_1BC[2].unk_06 += 2;
    D_800E6280.unk_1BC[2].unk_0A -= 0x14;
    func_80084D3C();
    func_8004284C();
}

void func_80150A84(void) {
    bg_read_sub2(0x4587);
    func_8004284C();
}

void func_80150AAC(void) {
    bg_read_sub2(0x457D);
    func_8004284C();
}

void func_80150AD4(void) {
    bg_read_sub2(0x4925);
    func_8004284C();
}

void func_80150AFC(void) {
    bg_read_sub2(0x4631);
    func_8004284C();
}

void func_80150B24(void) {
    bg_read_sub2(0x4845);
    func_8004284C();
}

void func_80150B4C(void) {
    bg_read_sub2(0x44D6);
    func_8004284C();
}

void func_80150B74(void) {
    bg_read_sub2(0x453B);
    func_8004284C();
}

void func_80150B9C(void) {
    func_80044750(0x203);
    bg_read_sub2(0x4520);
    func_8004284C();
}

extern FnTbl40 D_8015EAE8;

void func_80150BCC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = D_8015EAE8;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
    if (D_80122D04 != 0) {
        func_80150C78();
        D_800B5BD4 = 9;
        return;
    }
    D_800B5BD4 = 0xF;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150C78);

void func_80150E9C(void) {
    func_80046318(0x35, 0x801B0000, 0x8144);
    func_80150010();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80150ED4);

void func_80151064(void) {
    func_80046318(0x3D, 0x801B0000, 0x8179);
    func_80150080();
    func_8004284C();
}

void func_8015109C(void) {
    func_800853FC();
    D_801206DF = D_800B593C;
    if ((u8) D_800B593C < 8U) {
        D_801206DB &= 0xFF7F;
    }
}

void func_801510E8(void) {
    D_8011ECD0[0x1A0F] = 0x80;
    D_8011ECD0[0x1A0B] |= 0x80;
    *(s16 *)&D_8011ECD0[0x1A10] = 0;
    *(s16 *)&D_8011ECD0[0x1A1E] = 4;
    *(s16 *)&D_8011ECD0[0x1A20] = 0;
    D_8011ECD0[0x1A0A] = 1;
    func_8004284C();
}

void func_8015114C(void) {
    func_80044750(0x500);
}

void func_8015116C(void) {
    func_80044750(0x503);
}

void func_8015118C(void) {
    func_80044750(0x504);
    func_8004284C();
}

extern FnTbl40 D_8015EC24;

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_801511B4);

void func_80151300(void) {
    func_80044750(0x502);
    func_8004284C();
}

void func_80151328(void) {
    func_80046318(0x45, 0x801B0000, 0x81B6);
    func_80150130();
    func_8004284C();
}

extern FnTbl40 D_8015ECC4;

void func_80151360(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = D_8015ECC4;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_80122D04 != 0) {
        D_801206DB |= 0x80;
        D_801206DF = D_800B593C;
        return;
    }
    D_801206DB &= 0xFF7F;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80150010", func_80151424);
