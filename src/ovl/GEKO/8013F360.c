#define MAIN_API_OVERRIDE_D_80122CD0 /* switched as u32 (main_api.h: s32), T-6030 */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 D_80122CD0;

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_80147174;

typedef struct {
    void (*f[61])();
} FnTbl61; /* size 0xF4 */

void func_8013F360(void) {
    D_80147050 = 0x801D21BC;
    D_80147054 = 0x801D21C4;
    D_80147058 = 0x801D2204;
    D_8014705C = (*(s16 *)0x801D221C);
    D_80147060 = 0x801B0000;
    D_80147064 = 0x801B2000;
    D_80147068 = 0x801B6000;
    D_8014706C = 0x801BA000;
    D_80147070 = 0x801BE000;
    D_80147074 = 0x801C2000;
    D_80147078 = 0x801C6000;
}

void func_8013F410(void) {
    D_8014707C = 0x801D2600;
    D_80147080 = 0x801D2608;
    D_80147084 = 0x801D2674;
    D_80147088 = (*(s16 *)0x801D268C);
    D_8014708C = 0x801B0000;
    D_80147090 = 0x801B2000;
    D_80147094 = 0x801B6000;
    D_80147098 = 0x801BA000;
    D_8014709C = 0x801BE000;
    D_801470A0 = 0x801C2000;
    D_801470A4 = 0x801C6000;
}

void func_8013F4C0(void) {
    switch (D_80122CD0) {
    case 1:
        func_8013F53C();
        return;
    case 2:
        func_8013F578();
        return;
    case 7:
        func_8013F5B4();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8013F53C(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013F5F0();
        return;
    }
    func_80046500();
}

void func_8013F578(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013F6A4();
        return;
    }
    func_80046500();
}

void func_8013F5B4(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013F850();
        return;
    }
    func_80046500();
}

typedef struct {
    void (*f[49])();
} FnTbl49; /* size 0xC4 */
extern FnTbl49 D_801470B0;

void func_8013F5F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl49 tbl;

    tbl = D_801470B0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013F66C(void) {
    func_80046318(0x45, 0x801B0000, 0x8DBD);
    func_8013F360();
    func_8004284C();
}

void func_8013F6A4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_80147174;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013F72C(void) {
    if ((u8) D_800E6280.unk_56C[67] >= 2U) {
        D_800CA150 = (u16) D_800CA150 + 5;
    }
    func_8004284C();
}

void func_8013F770(void) {
    func_800634FC(get_g_zyotai_h(0xA));
    if ((u8) D_800E6280.unk_56C[67] >= 2U) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013F360", func_8013F800);

extern FnTbl61 D_801471D0;

void func_8013F850(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl61 tbl;

    tbl = D_801471D0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if ((D_801470A8 != 0) && (D_800E6280.unk_110A >= 0x20U)) {
        func_8013FD3C();
    }
    if ((D_801470AC != 0) && (D_800E6280.unk_110A >= 0x20U)) {
        func_80140040();
    }
}

void func_8013F92C(void) {
    s32 temp_v0;

    D_800B3D60 = 0;
    temp_v0 = dec_bg_cd_read(0x3FCE, 0);
    if (temp_v0 == *D_800B5938) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else if (temp_v0 == (1 - *D_800B5938)) {
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

s32 func_8013F9AC(void) {
    if ((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        func_800573AC();
        func_8004284C();
        D_800E6280.unk_110A -= 3;
        return 0;
    }
    func_8005751C(0);
    func_8004284C();
}

void func_8013FA38(void) {
    dec_bg_show_switch(0);
    func_8004284C();
}

void func_8013FA60(void) {
    func_80138AF8();
    if ((u16)D_800CA154 == 2) {
        if (D_800E6280.unk_1104.w++ == 0) {
            func_80044750(0x603);
        }
    }
}

void func_8013FAB4(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_8013FADC(void) {
    func_80046318(0x45, 0x801B0000, 0x8EFE);
    func_8013F410();
    func_8004284C();
}

void func_8013FB14(void) {
    func_80044750(0x500);
    func_8004284C();
}

void func_8013FB3C(void) {
    D_800B3D60 = 0;
    func_80044890(1, 0xC65F, 0xC659, 0xD9E1, 0xD993, 0xD98E);
    func_8004284C();
}

void func_8013FB88(void) {
    func_80138AF8();
    if (D_80122CE4 != 0) {
        D_80122CE4 = 0;
        D_80122CF0 = 0;
        D_8011ED5B &= 0xFF7F;
        D_800CA2AC = 0;
        D_800CA150 = (u16) D_800CA150 + 1;
        D_800CA154 = 0;
        func_8004284C();
    }
}

void func_8013FC00(void) {
    s16 i;

    func_800634FC(get_g_zyotai_h(0xA));
    D_800B5BD4 = 0xF;
    for (i = 0; i < 0xA; i++) {
        D_8011ECD0[i * 0x44 + 0x1A4F] &= ~0x80;
    }
    func_8004284C();
}

s16 func_8013FC80(void) {
    if (D_800E6280.unk_1104.w == 0) {
        D_801206DB |= 0x80;
        D_801206F4 = 0;
        D_801206F2 = 0;
        func_80044750(0x501);
    }
    D_801206F2 += 0x80;
    D_801206F4 += 0x80;
    if (D_801206F2 >= 0x1000) {
        D_801206DB &= 0xFF7F;
        D_801470A8 = 1;
        func_8004284C();
    }
    D_800E6280.unk_1104.w += 1;
}

void func_8013FD3C(void) {
    func_8013FD88(-0xA0, -0x78, 0x140, 0xA0, 0xFFFFFF, 0xA, 0);
    dtd_on(9);
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013F360", func_8013FD88);

void func_8013FF64(void) {
    s16 i;

    D_800B5BD4 = 0xA;
    func_80065F34(0);
    func_80085234(1);
    D_80122D04 = 1;
    D_80120657 = 0x80;
    D_8012069B = 0x80;
    for (i = 0x3A; i < 0x40; i++) {
        func_80084C98(i, 0x80);
    }
    D_801470A8 = 0;
    for (i = 0; i < 0xA; i++) {
        D_8011ECD0[i * 0x44 + 0x1A4F] |= 0x80;
    }
    D_801470AC = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013F360", func_80140040);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013F360", func_8014016C);

void func_801402F8(void) {
    func_8013F360();
    load_palette(D_80147060, 0x11, 1, 2, 0);
    func_80084E90(D_80147064, D_80147068, D_8014706C, D_80147070, D_80147074, D_80147078);
    func_800850D4(D_80147054, D_80147058, D_80147050, D_8014705C);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013F360", func_801403A0);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013F360", func_801404FC);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013F360", func_801406CC);

void func_801409D8(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}
