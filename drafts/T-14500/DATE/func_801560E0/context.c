#define MAIN_API_OVERRIDE_D_80122CD0 /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/DATE.h"

extern u32 D_80122CD0;

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

void func_80155530(void) {
    switch (D_80122CD0) {
    case 3:
        func_80155D80();
        return;
    case 4:
        func_80155E40();
        return;
    case 5:
        func_801563A8();
        return;
    case 6:
        func_80156494();
        return;
    default:
        func_80046500();
        return;
    }
}

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
    D_800CA224[0] = 2;
    D_800CA224[1] = 2;
    D_800CA224[2] = 2;
    D_800CA234[0] = 1;
    D_800CA234[1] = 1;
    D_800CA234[2] = 1;
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

typedef struct {
    void (*f[56])();
} FnTbl56; /* size 0xE0 */
extern FnTbl56 D_8015FBFC;

typedef struct {
    void (*f[40])();
} FnTbl40; /* size 0xA0 */
extern FnTbl40 D_8015FB5C;

void func_80155D80(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl38 tbl;

    tbl = D_8015F9D0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80155E08(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8816);
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
    cd_read_sectors(0x3D, 0x801B0000, 0x8853);
    func_80155360();
    func_8004284C();
}

void func_801560B8(void) {
    func_80044750(0x502);
    func_8004284C();
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015F9A0;
extern s32 D_8015F9A8;
extern s32 D_8015FB1C;
extern s32 D_8015FB2C;
extern s32 D_8015FB3C;
extern s32 sp;
/* end bulk_m2c declarations */
void func_801560E0(void) {
    s32 sp98[4];
    s32 sp88[4];
    s32 sp68[8];
    s16 var_s2;
    s32 temp_t6;
    u8 *temp_s1;
    u8 *temp_s1_2;
    u8 *temp_s3;
    u8 temp_v0;
    u8 var_v0;
    u8 *temp_s0;

    (*(s32 *)&sp98) = (s32) D_8015FB1C;
    *(s32 *)((unsigned char *)(&sp98) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB1C) + (4));
    *(s32 *)((unsigned char *)(&sp98) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB1C) + (12));
    *(s32 *)((unsigned char *)(&sp98) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB1C) + (8));
    (*(s32 *)&sp88) = (s32) D_8015FB2C;
    *(s32 *)((unsigned char *)(&sp88) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB2C) + (4));
    *(s32 *)((unsigned char *)(&sp88) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB2C) + (12));
    *(s32 *)((unsigned char *)(&sp88) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB2C) + (8));
    (*(s32 *)&sp68) = (s32) D_8015FB3C;
    *(s32 *)((unsigned char *)(&sp68) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (4));
    *(s32 *)((unsigned char *)(&sp68) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (12));
    *(s32 *)((unsigned char *)(&sp68) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (8));
    *(s32 *)((unsigned char *)(&sp68) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (16));
    *(s32 *)((unsigned char *)(&sp68) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (20));
    var_s2 = 0;
    *(s32 *)((unsigned char *)(&sp68) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (28));
    *(s32 *)((unsigned char *)(&sp68) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (24));
    do {
        temp_s3 = var_s2 + &D_8015F9A0;
        temp_v0 = *temp_s3;
        temp_s0 = D_8011ECD0 + (var_s2 * 0x44);
        *(u8 *)((unsigned char *)temp_s0 + (6671)) = (u8) D_80120657;
        if (temp_v0 == 0) {
            temp_s1 = &D_8015F9A8 + var_s2;
            *temp_s1 += 1;
        }
        temp_s1_2 = &D_8015F9A8 + var_s2;
        if ((temp_v0 == 0) && ((u8) *temp_s1_2 >= 0x79U) && ((u32) (0xB4 - *temp_s1_2) < (u32) (func_800AE0D0() % 60))) {
            *(s8 *)((unsigned char *)temp_s0 + (6666)) = 5;
            *(u8 *)((unsigned char *)temp_s0 + (6667)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (6667)) | 0x80);
            *temp_s3 = 1;
        }
        var_v0 = *temp_s3;
        if (var_s2 == 0) {
            if ((var_v0 == 1) && (*(s16 *)((unsigned char *)temp_s0 + (6688)) == 7) && (*(s16 *)((unsigned char *)temp_s0 + (6672)) == 0)) {
                *(s32 *)((unsigned char *)temp_s0 + (6712)) = 0;
                *temp_s3 = 2;
                goto block_16;
            }
        } else if ((var_v0 == 1) && (*(s16 *)((unsigned char *)temp_s0 + (6688)) == 3) && (*(s16 *)((unsigned char *)temp_s0 + (6672)) == 7)) {
            *(s32 *)((unsigned char *)temp_s0 + (6712)) = 0;
            *temp_s3 = 2;
block_16:
            var_v0 = 2 & 0xFF;
        }
        if (var_v0 == 2) {
            temp_t6 = *(s32 *)((unsigned char *)temp_s0 + (6712)) + 0x4000;
            *(s32 *)((unsigned char *)temp_s0 + (6712)) = temp_t6;
            *(s32 *)((unsigned char *)temp_s0 + (6704)) = (s32) (*(s32 *)((unsigned char *)temp_s0 + (6704)) + temp_t6);
            if ((*(s16 *)((unsigned char *)(sp + (var_s2 * 4)) + (106)) - 0x78) < *(s16 *)((unsigned char *)temp_s0 + (6706))) {
                *(s8 *)((unsigned char *)temp_s0 + (6666)) = 0;
                *(u8 *)((unsigned char *)temp_s0 + (6667)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (6667)) & 0x7F);
                *(s16 *)((unsigned char *)temp_s0 + (6706)) = (s16) (*(s16 *)((unsigned char *)(sp + (var_s2 * 2)) + (136)) - 0x78);
                *(s32 *)((unsigned char *)temp_s0 + (6704)) = 0;
                *(s16 *)((unsigned char *)temp_s0 + (6688)) = 0;
                *(s16 *)((unsigned char *)temp_s0 + (6672)) = 0;
                *temp_s3 = 0;
                *(&D_8015F9A8 + var_s2) = 0;
            }
        }
        var_s2 += 1;
    } while (var_s2 < 8);
}

void func_801563A8(void) {
    s32 pad; /* FAKE: unused slot above tbl, the original frame has it (T-3330 idiom, real source unknown). T-8010 */
    FnTbl40 tbl;

    tbl = D_8015FB5C;
    if (D_800E6280.unk_110A < 0xB) {
        func_801407D0();
    }
    if (D_800E6280.unk_110A == 0xB) {
        func_80140788();
    }
    tbl.f[D_800E6280.unk_110A](0x80);
}

void func_8015645C(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8890);
    func_80155410();
    func_8004284C();
}

void func_80156494(void) {
    s32 idx; /* FAKE: never read; declared first so tbl lands at the original frame offset (T-3330) */
    FnTbl56 tbl;

    tbl = D_8015FBFC;
    if (D_800E6280.unk_110A >= 0x1E && D_800E6280.unk_110A < 0x2C) {
        func_8006B900();
    }
    tbl.f[D_800E6280.unk_110A](0x80);
}

void func_80156538(void) {
    cd_read_sectors(0x35, 0x801B0000, 0x88CD);
    func_801554C0();
    func_8004284C();
}

void func_80156570(void) {
    if (D_800E6280.unk_1BC[5].unk_0C.f.b14) {
        D_800CA150 = (u16)D_800CA150 + 1;
    }
    func_8004284C();
}

void func_801565B4(void) {
    if (!D_800E6280.unk_1BC[5].unk_0C.f.b14) {
        D_800CA150 = (u16)D_800CA150 + 1;
    }
    func_8004284C();
}
