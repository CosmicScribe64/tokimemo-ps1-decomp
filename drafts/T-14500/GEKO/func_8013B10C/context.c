#define MAIN_API_OVERRIDE_D_80122CD0 /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 D_80122CD0;

typedef struct {
    void (*f[41])();
} FnTbl41; /* size 0xA4 */
extern FnTbl41 D_801466D8;

void func_8013AF80(void) {
    D_80146600 = 0x801D212C;
    D_80146604 = 0x801D2130;
    D_80146608 = 0x801D2150;
    D_8014660C = (*(s16 *)0x801D216C);
    D_80146610 = 0x801B0000;
    D_80146614 = 0x801B2000;
    D_80146618 = 0x801B6000;
    D_8014661C = 0x801BA000;
    D_80146620 = 0x801BE000;
    D_80146624 = 0x801C2000;
    D_80146628 = 0x801C6000;
}

void func_8013B030(void) {
    switch (D_80122CD0) {
    case 1:
        func_8013B094();
        return;
    case 7:
        func_8013B0D0();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8013B094(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013B10C();
        return;
    }
    func_80046500();
}

void func_8013B0D0(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013B2C8();
        return;
    }
    func_80046500();
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_80146630;
extern u8 D_800E62BC;
extern u8 D_800E738A;
/* end bulk_m2c declarations */
s32 func_8013B10C(void) {
    s32 sp28[16];
    typedef struct { s32 w[42]; } M2cBlockA8; /* m2c memcpy of 168 bytes */
    s32 var_v1;

    *(M2cBlockA8 *)(&sp28[0]) = *(M2cBlockA8 *)(&D_80146630);
    (&sp28[0])[D_800E738A](0x80);
    if (D_80122D04 != 0) {
        D_80120723 = D_800B593C;
        D_8012071F |= 0x80;
        if (D_800E62BC != 0) {
            if ((u8) D_8014662C == 0) {
                D_8012071E = 3;
            }
            if ((D_80120734 == 6) && ((u8) D_8014662C == 0)) {
                D_8012071E = 0;
            }
            var_v1 = 0;
            if (func_800460EC() & 4) {
                var_v1 = getCDlevel();
            }
            if (var_v1 != 0) {
                if ((u8) D_8014662C == 0) {
                    D_8012071E = 3;
                    D_8014662C = 1;
                }
            }
        } else if ((u8) D_8014662C == 0) {
            D_8012071E = 3;
            D_80120734 = 1;
            D_8014662C = 1;
        }
    } else {
        D_8012071F &= 0xFF7F;
    }
}

void func_8013B290(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x80FF);
    func_8013AF80();
    func_8004284C();
}

void func_8013B2C8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_801466D8;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013B350(void) {
    D_80122CE0 = 6;
    func_80048EB8(0);
    func_8006BD6C(0);
    D_800E6280.unk_720 = D_800E6280.unk_1108;
    D_800E6280.unk_721 = D_800E6280.unk_1109;
    D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
    main_state_set(0x81);
}

void func_8013B3C0(void) {
    s32 pad; /* FAKE: unused scalar reproduces RECT home at +0x2C; original local unknown. T-9770 */
    RECT r;

    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_8006BD6C(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_8008585C();
    func_8004E9F4(1);
    r.x = 0x140;
    r.y = 0x100;
    r.w = 0x140;
    r.h = 0xF8;
    func_8009C7F8(&r, 8, 0, 0);
    r.x = 0x2C0;
    r.y = 0;
    func_8009C7F8(&r, 8, 0, 0);
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    /* FAKE: scalar byte store then member read preserves argument scheduling; sibling func_8013CC20 uses the same idiom. T-9770 */
    *(u8 *)&D_800E6280.unk_F5F = 2;
    func_800847B8(D_800E6280.unk_F5F);
    func_8007C740();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    func_80137990();
    D_80146274 = D_80145F10;
    D_80146278 = D_8014604C;
    D_8014627C = D_80146188;
    func_8004284C();
}

void func_8013B510(void) {
    D_800E6280.unk_F5F = 2;
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_8007C740();
    func_80137990();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    func_800847B8(D_800E6280.unk_F5F);
    func_8008585C();
    func_80137990();
    D_80146274 = D_80145EF8;
    D_80146278 = D_80146034;
    D_8014627C = D_80146170;
    func_800AE0F0(D_800CA19C, "屋上");
    D_8014662C = 0;
    D_800E6280.unk_1BC[2].unk_02 += 1;
    D_800E6280.unk_1BC[2].unk_06 += 2;
    func_80084D3C();
    D_800CA368 = 0xE;
    func_8004284C();
}

void func_8013B65C(void) {
    func_8013AF80();
    load_palette(D_80146610, 0x11, 1, 2, 0);
    func_80084E90(D_80146614, D_80146618, D_8014661C, D_80146620, D_80146624, D_80146628);
    func_800850D4(0, 0, 0, 0);
    func_80048F64(0x63);
    D_8012071D = 0xA;
    D_8012071E = 0;
    D_80120754 = 0x41000000;
    D_8012071F = 4;
    D_80120728 = D_80146604;
    D_8012072C = D_80146608;
    D_80120750 = D_80146600;
    D_80120730 = D_8014660C;
    D_80120732 = 4;
    D_80120734 = 0;
    D_80120724 = 0;
    D_80120722 = 1;
    D_8012075F = 0x11;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
    D_80120723 = 0;
    D_80120721 = 0;
    D_80120720 = 0;
    D_800E6280.unk_1228[26] = -1;
    func_8004284C();
}

void func_8013B7D0(void) {
    D_800E6280.unk_03E = 0x62;
    D_800E6280.unk_03F = 2;
    D_800E6280.unk_040 = 0x1C;
    D_800E6280.unk_041 = 5;
    D_800E6280.unk_F5F = 2;
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_8007C740();
    func_80137990();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    func_800847B8(D_800E6280.unk_F5F);
    func_8008585C();
    func_80137990();
    D_80146274 = D_80145F10;
    D_80146278 = D_8014604C;
    D_8014627C = D_80146188;
    func_800AE0F0(D_800CA19C, "近所の公園");
    D_800CA368 = 0x14;
    func_8004284C();
}

void func_8013B908(void) {
    bg_read_sub2(0x412C);
    func_8004284C();
}
