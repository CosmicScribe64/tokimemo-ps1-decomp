#include "common.h"
#include "ovl/DATE.h"

void func_8014F0F0(void) {
    D_8015E690 = 0x801D21D4;
    D_8015E694 = 0x801D21DC;
    D_8015E698 = 0x801D21FC;
    D_8015E69C = *(s16 *)0x801D2210;
    D_8015E6A0 = 0x801B0000;
    D_8015E6A4 = 0x801B2000;
    D_8015E6A8 = 0x801B6000;
    D_8015E6AC = 0x801BA000;
    D_8015E6B0 = 0x801BE000;
    D_8015E6B4 = 0x801C2000;
    D_8015E6B8 = 0x801C6000;
}

void func_8014F1A0(void) {
    D_8015E6BC = 0x801D21E0;
    D_8015E6C0 = 0x801D21E8;
    D_8015E6C4 = 0x801D2228;
    D_8015E6C8 = *(s16 *)0x801D2240;
    D_8015E6CC = 0x801B0000;
    D_8015E6D0 = 0x801B2000;
    D_8015E6D4 = 0x801B6000;
    D_8015E6D8 = 0x801BA000;
    D_8015E6DC = 0x801BE000;
    D_8015E6E0 = 0x801C2000;
    D_8015E6E4 = 0x801C6000;
}

void func_8014F250(void) {
    D_8015E6E8 = 0x801D2320;
    D_8015E6EC = 0x801D2328;
    D_8015E6F0 = 0x801D2368;
    D_8015E6F4 = *(s16 *)0x801D2380;
    D_8015E6F8 = 0x801B0000;
    D_8015E6FC = 0x801B2000;
    D_8015E700 = 0x801B6000;
    D_8015E704 = 0x801BA000;
    D_8015E708 = 0x801BE000;
    D_8015E70C = 0x801C2000;
    D_8015E710 = 0x801C6000;
}

void func_8014F300(void) {
    D_8015E714 = 0x801B0000;
    D_8015E718 = 0x801B2000;
    D_8015E71C = 0x801B6000;
    D_8015E720 = 0x801BA000;
    D_8015E724 = 0x801BE000;
    D_8015E728 = 0x801C2000;
    D_8015E72C = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014F370);

void func_8014F404(void) {
    D_8015E208 = D_8015DE70;
    D_8015E20C = D_8015DFAC;
    D_8015E210 = D_8015E0E8;
    func_80043914(D_8015E6A0, 0x11, 1, 2, 0);
    func_80084E90(D_8015E6A4, D_8015E6A8, D_8015E6AC, D_8015E6B0, D_8015E6B4, D_8015E6B8);
    func_800850D4(D_8015E694, D_8015E698, D_8015E690, D_8015E69C);
    D_800E643C[0].unk_02 += 2;
    D_800E643C[0].unk_0A -= 0x1C;
    func_80084D3C();
    func_8004284C();
}

void func_8014F500(void) {
    D_8015E208 = D_8015DE74;
    D_8015E20C = D_8015DFB0;
    D_8015E210 = D_8015E0EC;
    load_palette(D_8015E6CC, 0x11, 1, 2, 0);
    func_80084E90(D_8015E6D0, D_8015E6D4, D_8015E6D8, D_8015E6DC, D_8015E6E0, D_8015E6E4);
    func_800850D4(D_8015E6C0, D_8015E6C4, D_8015E6BC, D_8015E6C8);
    D_800E643C[0].unk_02 += 3;
    D_800E643C[0].unk_06 += 2;
    D_800E643C[0].unk_0A -= 0x14;
    check_para_limit();
    func_8004284C();
}

void func_8014F610(void) {
    D_8015B68C = 2;
    D_8015E208 = D_8015DE78;
    D_8015E20C = D_8015DFB4;
    D_8015E210 = D_8015E0F0;
    D_800E643C[0].unk_02 += 1;
    D_800E643C[0].unk_06 += 1;
    D_800E643C[0].unk_0A -= 0x14;
    check_para_limit();
    D_80122D08 = 0x17;
    D_800CA21C = 2;
    D_800CA220 = 2;
    D_800CA21E = 2;
    D_800CA224 = 2;
    D_800CA228 = 2;
    D_800CA226 = 2;
    D_800CA22C = 2;
    D_800CA230 = 2;
    D_800CA22E = 2;
    D_800CA234 = 1;
    D_800CA238 = 1;
    D_800CA236 = 1;
    func_8004284C();
}

void func_8014F744(void) {
    func_8014F250();
    load_palette(D_8015E6F8, 0x11, 1, 2, 0);
    func_80084E90(D_8015E6FC, D_8015E700, D_8015E704, D_8015E708, D_8015E70C, D_8015E710);
    func_800850D4(D_8015E6EC, D_8015E6F0, D_8015E6E8, D_8015E6F4);
    D_800CA360 = 1;
    func_8004284C();
}

void func_8014F7F4(void) {
    D_8015E208 = D_8015DE7C;
    D_8015E20C = D_8015DFB8;
    D_8015E210 = D_8015E0F4;
    load_palette(D_8015E714, 0x11, 1, 2, 0);
    func_80084E90(D_8015E718, D_8015E71C, D_8015E720, D_8015E724, D_8015E728, D_8015E72C);
    func_800850D4(0, 0, 0, 0);
    D_800E643C[0].unk_02 += 1;
    D_800E643C[0].unk_06 += 2;
    D_800E643C[0].unk_0A -= 0x14;
    check_para_limit();
    func_8004284C();
}

void func_8014F8F0(void) {
    bg_read_sub2(0x47BA);
    func_8004284C();
}

void func_8014F918(void) {
    bg_read_sub2(0x47CD);
    func_8004284C();
}

void func_8014F940(void) {
    bg_read_sub2(0x42EB);
    func_8004284C();
}

void func_8014F968(void) {
    bg_read_sub2(0x4350);
    func_8004284C();
}

void func_8014F990(void) {
    bg_read_sub2(0x4800);
    func_8004284C();
}

void func_8014F9B8(void) {
    bg_read_sub2(0x432B);
    func_8004284C();
}

void func_8014F9E0(void) {
    bg_read_sub2(0x48EA);
    func_8004284C();
}

void func_8014FA08(void) {
    bg_read_sub2(0x45BF);
    func_8004284C();
}

void func_8014FA30(void) {
    D_80122CDC = 0;
    D_800CA21C = 1;
    D_800CA21E = 1;
    D_800CA220 = 1;
    D_800CA234 = 0;
    D_800CA236 = 0;
    D_800CA238 = 0;
    D_800CA224 = 1;
    D_800CA226 = 1;
    D_800CA228 = 1;
    D_800CA22C = 0;
    D_800CA22E = 0;
    D_800CA230 = 0;
    func_8004284C();
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_8014B1F0();
}

typedef struct {
    void (*f[41])();
} FnTbl41; /* size 0xA4 */
extern FnTbl41 D_8015E730;

void func_8014FAE0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_8015E730;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

void func_8014FB68(void) {
    func_80046318(0x45, 0x801B0000, 0x7F61);
    func_8014F0F0();
    func_8004284C();
}

typedef struct {
    void (*f[43])();
} FnTbl43; /* size 0xAC */
extern FnTbl43 D_8015E7D4;

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014FBA0);

void func_8014FC48(void) {
    func_80046318(0x45, 0x801B0000, 0x7FA6);
    func_8014F1A0();
    func_8004284C();
}

typedef struct {
    void (*f[65])();
} FnTbl65; /* size 0x104 */
extern FnTbl65 D_8015E880;

void func_8014FC80(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl65 tbl;

    tbl = D_8015E880;
    idx = D_800E738A;
    tbl.f[idx]();
}

void func_8014FD08(void) {
    func_80044750(0x200);
    func_8004284C();
}

s32 func_8014FD30(void) {
    if (D_800E7384 == 0) {
        func_8007BFA8();
        func_80044750(0xB1);
        func_80044750(0x501);
        func_80047550();
    }
    if (D_800E7384++ == 0x80) {
        func_8004284C();
    }
}

s32 func_8014FDA0(void) {
    s32 unused; /* FAKE: extra local moves the spill slot of sel to sp+0x28; real source unknown. T-4090 */
    s32 sel;

    func_8014C5C8();
    if ((u16) D_800CA154 == 2) {
        sel = D_800E7384;
        func_800854C8();
        D_800E738A -= 1;
        D_800E7384 = sel + 1;
        if (sel == 1) {
            func_80044750(0x201);
            func_80044750(0x502);
        }
    }
}

void func_8014FE24(void) {
    func_80046318(0x45, 0x801B0000, 0x7FEB);
    func_8014F250();
    func_8004284C();
}

void func_8014FE5C(void) {
    func_800AE0F0(D_800CA1DC, "観覧車");
    func_8004284C();
}

typedef struct {
    void (*f[45])();
} FnTbl45; /* size 0xB4 */
extern FnTbl45 D_8015E984;

void func_8014FE90(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl45 tbl;

    tbl = D_8015E984;
    if (D_80122D04 == 0 && D_800B593C == 0x80) {
        func_8006B900();
    }
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

void func_8014FF2C(void) {
    func_80044750(0x502);
    func_8004284C();
}

void func_8014FF54(void) {
    if ((u16) D_800CA154 == 5) {
        func_80083440(1);
    } else if ((u16) D_800CA154 == 9) {
        func_80083440(2);
    } else if ((u16) D_800CA154 == 0xB) {
        func_80083440(get_g_zyotai_h(D_800E71DF));
    }
    func_8014C5C8();
}

void func_8014FFD0(void) {
    func_80046318(0x35, 0x801B0000, 0x8030);
    func_8014F300();
    func_8004284C();
}
