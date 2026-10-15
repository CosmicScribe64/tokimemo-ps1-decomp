#include "common.h"
#include "ovl/BUNKA_SD.h"

s32 func_80137B40(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80044890(1, 0xBEFB, 0xBEE2, 0xC9DC, 0xC997, 0xC988);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 1) {
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        if (func_80044F94(1) == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 3) {
        cd_read_sectors(0xD2, 0x80180000, 0x4CC8);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 4) {
        if (cd_read_sync_status() & 1) {
            D_8013D98C = D_8013CEF0;
            D_8013D990 = D_8013CFC8;
            D_800E6280.unk_F5F = 0xE;
            func_8004284C();
        }
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80137B40", func_80137CAC);

void func_80137D6C(void) {
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80134260();
        func_80044750(0x7F);
        break;
    case 1:
        func_80137B40();
        break;
    case 2:
        func_80044750(0x202);
        func_80137F8C();
        break;
    case 3:
        func_80134418();
        break;
    case 4:
        func_80138898(0x78);
        break;
    case 5:
        func_8013838C();
        break;
    case 6:
        func_80138898(0x78);
        break;
    case 7:
        func_801344A8();
        break;
    case 8:
        func_801383CC();
        break;
    case 9:
        func_80134418();
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        func_8013842C();
        break;
    case 15:
        func_80044750(0x200);
        func_8004284C();
        break;
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
        func_8013842C();
        break;
    case 33:
        func_80044750(0x201);
        func_8004284C();
        break;
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
        func_8013842C();
        break;
    case 40:
        func_80044750(0x200);
        func_8004284C();
        break;
    case 41:
    case 42:
    case 43:
        func_8013842C();
        break;
    case 44:
        func_80044750(0xC1);
        func_80044750(0x500);
        func_8004E9F4(1);
        func_8004284C();
        break;
    case 45:
        if ((u32) D_800E6280.unk_1104.u >= 0x1FU) {
            func_8004284C();
        }
        break;
    case 46:
        func_801344A8();
        break;
    case 47:
        func_80134314();
        break;
    default:
        func_80046500();
        break;
    }
    D_800E6280.unk_1104.u += 1;
}

void func_80137F8C(void) {
    func_80137FEC();
    k_speed_set(5);
    D_8013D988 = 0;
    D_8013D994 = 1;
    D_8013D998 = 0;
    D_8013D99C = 0;
    D_8013D9A0 = 0;
    D_8013D9A4 = 0;
    func_8004284C();
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_8013D9AC;
extern s32 D_8013D9B0;
extern s32 D_8013D9B4;
extern void *D_8013D9A8;
/* end bulk_m2c declarations */
void func_80137FEC(void) {
    s32 var_s1;
    s32 var_s1_2;
    u8 *var_s0;

    var_s1 = 0;
    do {
        load_palette(D_8013D9B4, (var_s1 + 0x10) & 0xFF, 1, 1, var_s1);
        var_s1 += 1;
    } while (var_s1 < 0x10);
    var_s0 = D_8011ECD0;
    var_s1_2 = 0;
    do {
        func_80048F64(var_s1_2 + 0x60);
        *(s8 *)((unsigned char *)var_s0 + (6529)) = 4;
        *(s32 *)((unsigned char *)var_s0 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)var_s0 + (6531)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6530)) = 0x40;
        *(s32 *)((unsigned char *)var_s0 + (6540)) = (s32) D_8013D9AC;
        *(s32 *)((unsigned char *)var_s0 + (6544)) = (s32) D_8013D9B0;
        var_s1_2 += 1;
        var_s0 += 0x44;
        *(s16 *)((unsigned char *)var_s0 + (6484)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6466)) = 1;
        *(s8 *)((unsigned char *)var_s0 + (6467)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (6498)) = -0xA0;
        *(s16 *)((unsigned char *)var_s0 + (6502)) = -0x78;
        *(s8 *)((unsigned char *)var_s0 + (6465)) = 0x80;
        *(s8 *)((unsigned char *)var_s0 + (6527)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6464)) = 0;
        *(void **)((unsigned char *)var_s0 + (6512)) = (void *) D_8013D9A8;
    } while (var_s1_2 != 0x20);
    load_tpage_buf_lock(5, (*(s32 *)D_8013D9A8), 1);
    load_tpage_buf_lock(6, *(s32 *)((unsigned char *)D_8013D9A8 + (4)), 1);
    load_tpage_buf_lock(7, *(s32 *)((unsigned char *)D_8013D9A8 + (8)), 1);
    load_tpage_buf_lock(8, *(s32 *)((unsigned char *)D_8013D9A8 + (12)), 1);
    load_tpage_buf_lock(9, *(s32 *)((unsigned char *)D_8013D9A8 + (16)), 1);
    load_tpage_buf_lock(0xA, *(s32 *)((unsigned char *)D_8013D9A8 + (20)), 1);
    load_tpage_buf_lock(0xB, *(s32 *)((unsigned char *)D_8013D9A8 + (24)), 1);
    load_tpage_buf_lock(0xC, *(s32 *)((unsigned char *)D_8013D9A8 + (28)), 1);
    load_tpage_buf_lock(0xD, *(s32 *)((unsigned char *)D_8013D9A8 + (32)), 1);
    load_tpage_buf_lock(0xE, *(s32 *)((unsigned char *)D_8013D9A8 + (36)), 1);
    load_tpage_buf_lock(0xF, *(s32 *)((unsigned char *)D_8013D9A8 + (40)), 1);
    load_tpage_buf_lock(0x25, *(s32 *)((unsigned char *)D_8013D9A8 + (44)), 1);
    load_tpage_buf_lock(0x26, *(s32 *)((unsigned char *)D_8013D9A8 + (48)), 1);
    load_tpage_buf_lock(0x27, *(s32 *)((unsigned char *)D_8013D9A8 + (52)), 1);
    load_tpage_buf_lock(0x28, *(s32 *)((unsigned char *)D_8013D9A8 + (56)), 1);
    load_tpage_buf_lock(0x29, *(s32 *)((unsigned char *)D_8013D9A8 + (60)), 1);
    load_tpage_buf_lock(0x2A, *(s32 *)((unsigned char *)D_8013D9A8 + (64)), 1);
    load_tpage_buf_lock(0x2B, *(s32 *)((unsigned char *)D_8013D9A8 + (68)), 1);
    load_tpage_buf_lock(0x2C, *(s32 *)((unsigned char *)D_8013D9A8 + (72)), 1);
    load_tpage_buf_lock(0x2D, *(s32 *)((unsigned char *)D_8013D9A8 + (76)), 1);
    load_tpage_buf_lock(0x2E, *(s32 *)((unsigned char *)D_8013D9A8 + (80)), 1);
    load_tpage_buf_lock(0x2F, *(s32 *)((unsigned char *)D_8013D9A8 + (84)), 1);
    load_tpage_buf_lock(0x15, *(s32 *)((unsigned char *)D_8013D9A8 + (88)), 1);
    load_tpage_buf_lock(0x16, *(s32 *)((unsigned char *)D_8013D9A8 + (92)), 1);
    load_tpage_buf_lock(0x17, *(s32 *)((unsigned char *)D_8013D9A8 + (96)), 1);
    func_80138810(0, 0x5B, 1);
    func_80138810(0x1F, 0x56, 1);
}

void func_8013838C(void) {
    func_80138810(0, 0x5B, 1);
    func_80138810(0x1F, 0x57, 1);
    func_8004284C();
}

void func_801383CC(void) {
    func_80138810(0, 0x5B, 1);
    func_80138810(0x1F, 0x58, 1);
    func_80138810(5, 0, 1);
    func_80138810(4, 8, 1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80137B40", func_8013842C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AE70);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AE80);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AE98);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AEB0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AEC8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AEDC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AEEC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AF08);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AF28);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AF38);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AF54);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AF70);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AF80);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AF94);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AFA4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AFB8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AFC8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AFE0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013AFF8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B010);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B028);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B044);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B068);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B08C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B0A0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B0AC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B0C0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B0D4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B0EC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B108);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B114);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B12C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B144);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B160);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B17C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B194);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B1AC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B1C0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B1DC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B1EC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B1F8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B208);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B220);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B23C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B258);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B274);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B284);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B294);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B2B0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B2D0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B2E8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B300);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B314);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B320);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B338);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B34C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B360);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B368);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B380);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B388);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B398);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B3A0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B3B0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B3C0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B3D0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B3E0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B3F0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B3F8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B408);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B41C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B424);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B434);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B43C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B444);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B458);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B46C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B47C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B48C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B49C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B4AC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B4C0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B4D8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B4F0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B500);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata", D_8013B510);

void func_8013867C(void) {
    s32 index;
    s32 *coordinates;

    if (D_800E6280.unk_1100 == 1) {
        D_800E6280.unk_1100++;
        set_kanji_string(-86, 64, 0, (s32)"「", 0);
        index = D_8013D988;
        if (D_8013D988 < 100) {
            do {
                set_kanji_string(-72, (s16)((index - D_8013D988) * 16 + 64), 0, (s32)(((u8 **)D_8013D98C)[index] + 1), 0);
                if (((u8 **)D_8013D98C)[index][0] == 103) {
                    if (!(D_800E6280.unk_F80 & 32)) {
                        coordinates = (s32 *)D_8013D990;
                        coordinates += *(u8 *)&D_800E6280.unk_110C;
                        func_80046290(coordinates[0], coordinates[1], D_800E6280.unk_F5F);
                        func_80044750(0x300);
                    }
                    (*(u8 *)&D_800E6280.unk_110C) += 2;
                    D_8013D988 = index + 1;
                    index = 100;
                }
                index++;
            } while (index < 100);
        }
    }
    if (func_800460EC() == 2 && check_end_k() != 0) {
        D_800E6280.unk_1100 = 0;
    }
    check_k_scroll();
    k_disp_inc();
}

void func_80138810(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;

    if (arg2 == 1) {
        p = D_8011ECD0 + arg0 * 0x44;
        p[0x1983] = 0x80;
    } else {
        p = D_8011ECD0 + arg0 * 0x44;
        p[0x1983] = 0;
    }
    *(s16 *)(p + 0x1998) = arg1;
}

void func_80138860(s32 arg0) {
    k_disp_start(set_kanji_string(-0x80, 0x30, 0, arg0, 0));
}

void func_80138898(u32 arg0) {
    if (arg0 < (u32)D_800E6280.unk_1104.u) {
        func_8004284C();
    }
}
