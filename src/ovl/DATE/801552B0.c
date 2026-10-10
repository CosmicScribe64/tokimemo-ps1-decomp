#include "common.h"
#include "ovl/DATE.h"

void func_801552B0(void) {
    D_8015F900 = 0x801CE080;
    D_8015F904 = 0x801CE084;
    D_8015F908 = 0x801CE094;
    D_8015F90C = *(s16 *)0x801CE0A8;
    D_8015F910 = 0x801B0000;
    D_8015F914 = 0x801B2000;
    D_8015F918 = 0x801B6000;
    D_8015F91C = 0x801BA000;
    D_8015F920 = 0x801BE000;
    D_8015F924 = 0x801C2000;
    D_8015F928 = 0x801C6000;
}

void func_80155360(void) {
    D_8015F92C = 0x801CE31C;
    D_8015F930 = 0x801CE320;
    D_8015F934 = 0x801CE39C;
    D_8015F938 = *(s16 *)0x801CE3BC;
    D_8015F93C = 0x801B0000;
    D_8015F940 = 0x801B2000;
    D_8015F944 = 0x801B6000;
    D_8015F948 = 0x801BA000;
    D_8015F94C = 0x801BE000;
    D_8015F950 = 0x801C2000;
    D_8015F954 = 0x801C6000;
}

void func_80155410(void) {
    D_8015F958 = 0x801CE124;
    D_8015F95C = 0x801CE128;
    D_8015F960 = 0x801CE148;
    D_8015F964 = *(s16 *)0x801CE15C;
    D_8015F968 = 0x801B0000;
    D_8015F96C = 0x801B2000;
    D_8015F970 = 0x801B6000;
    D_8015F974 = 0x801BA000;
    D_8015F978 = 0x801BE000;
    D_8015F97C = 0x801C2000;
    D_8015F980 = 0x801C6000;
}

void func_801554C0(void) {
    D_8015F984 = 0x801B0000;
    D_8015F988 = 0x801B2000;
    D_8015F98C = 0x801B6000;
    D_8015F990 = 0x801BA000;
    D_8015F994 = 0x801BE000;
    D_8015F998 = 0x801C2000;
    D_8015F99C = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801552B0", func_80155530);

void func_801555C4(void) {
    D_8015E208 = D_8015DF00;
    D_8015E20C = D_8015E03C;
    D_8015E210 = D_8015E178;
    func_801552B0();
    func_80043914(D_8015F910, 0x11, 1, 2, 0);
    func_80084E90(D_8015F914, D_8015F918, D_8015F91C, D_8015F920, D_8015F924, D_8015F928);
    func_800850D4(D_8015F904, D_8015F908, D_8015F900, (s32) D_8015F90C);
    D_800E6280.unk_1BC[5].unk_02 += 2;
    D_800E6280.unk_1BC[5].unk_06 += 1;
    D_800E6280.unk_1BC[5].unk_0A -= 0xA;
    func_80084D3C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801552B0", func_801556D8);

void func_801559C0(void) {
    D_8015E208 = D_8015DF08;
    D_8015E20C = D_8015E044;
    D_8015E210 = D_8015E180;
    func_80155410();
    func_80043914(D_8015F968, 0x11, 1, 2, 0);
    func_80084E90(D_8015F96C, D_8015F970, D_8015F974, D_8015F978, D_8015F97C, D_8015F980);
    func_800850D4(D_8015F95C, D_8015F960, D_8015F958, (s32) D_8015F964);
    D_800E6280.unk_1BC[5].unk_02 += 3;
    D_800E6280.unk_1BC[5].unk_06 += 2;
    D_800E6280.unk_1BC[5].unk_0A -= 0x14;
    func_80084D3C();
    func_8004284C();
}

void func_80155AD4(void) {
    D_8015E208 = D_8015DF0C;
    D_8015E20C = D_8015E048;
    D_8015E210 = D_8015E184;
    func_801554C0();
    func_80043914(D_8015F984, 0x11, 1, 2, 0);
    func_80084E90(D_8015F988, D_8015F98C, D_8015F990, D_8015F994, D_8015F998, D_8015F99C);
    func_800850D4(0, 0, 0, 0);
    D_800E6280.unk_1BC[5].unk_02 += 1;
    D_800E6280.unk_1BC[5].unk_06 += 1;
    D_800E6280.unk_1BC[5].unk_0A -= 0x14;
    func_80084D3C();
    D_800CA224 = 2;
    D_800CA226 = 2;
    D_800CA228 = 2;
    D_800CA234 = 1;
    D_800CA236 = 1;
    D_800CA238 = 1;
    func_8004284C();
}

void func_80155C18(void) {
    bg_read_sub2(0x48EE);
    func_8004284C();
}

void func_80155C40(void) {
    bg_read_sub2(0x45D3);
    func_8004284C();
}

void func_80155C68(void) {
    bg_read_sub2(0x48B5);
    func_8004284C();
}

void func_80155C90(void) {
    bg_read_sub2(0x4591);
    func_8004284C();
}

void func_80155CB8(void) {
    bg_read_sub2(0x47E0);
    func_8004284C();
}

void func_80155CE0(void) {
    bg_read_sub2(0x42F4);
    func_8004284C();
}

void func_80155D08(void) {
    bg_read_sub2(0x488A);
    func_8004284C();
}

void func_80155D30(void) {
    bg_read_sub2(0x4562);
    func_8004284C();
}

void func_80155D58(void) {
    bg_read_sub2(0x4558);
    func_8004284C();
}

typedef struct {
    void (*f[38])();
} FnTbl38; /* size 0x98 */
extern FnTbl38 D_8015F9D0;

void func_80155D80(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl38 tbl;

    tbl = D_8015F9D0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80155E08(void) {
    func_80046318(0x3D, 0x801B0000, 0x8816);
    func_801552B0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801552B0", func_80155E40);

void func_80156024(void) {
    func_8014C5C8();
    if (((u16) D_800CA154 == 2) && (D_800E6280.unk_1104.w == 1)) {
        func_80083440(1);
    }
    D_800E6280.unk_1104.w += 1;
}

void func_80156080(void) {
    func_80046318(0x3D, 0x801B0000, 0x8853);
    func_80155360();
    func_8004284C();
}

void func_801560B8(void) {
    func_80044750(0x502);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801552B0", func_801560E0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801552B0", func_801563A8);

void func_8015645C(void) {
    func_80046318(0x3D, 0x801B0000, 0x8890);
    func_80155410();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801552B0", func_80156494);

void func_80156538(void) {
    func_80046318(0x35, 0x801B0000, 0x88CD);
    func_801554C0();
    func_8004284C();
}

/* Bit 14 of the word is tested as a bit-field: the original has sll 17; bgez, not andi (T-4050) */
typedef struct {
    u32 pad : 14;
    u32 b14 : 1;
    u32 rest : 17;
} Flags14; /* size 4 */

void func_80156570(void) {
    if (((Flags14 *)&D_800E6280.unk_1BC[5].unk_0C.w)->b14) {
        D_800CA150 = (u16)D_800CA150 + 1;
    }
    func_8004284C();
}

void func_801565B4(void) {
    if (!((Flags14 *)&D_800E6280.unk_1BC[5].unk_0C.w)->b14) {
        D_800CA150 = (u16)D_800CA150 + 1;
    }
    func_8004284C();
}
