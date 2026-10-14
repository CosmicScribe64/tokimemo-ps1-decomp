#include "common.h"
#include "ovl/SHUGAKU.h"

void func_80139D00(void) {
    D_8013CAF0 = 0x801E20DC;
    D_8013CAF4 = 0x801E20F8;
    D_8013CAF8 = 0x801E2114;
    D_8013CB00 = 0x801E0000;
}

void func_80139D40(void) {
    u8 kind;
    u8 index;
    ShugakuFn68 tbl;

    tbl = D_8013CB08;
    if (D_800CA2EC != 0) {
        index = D_800E6280.unk_110A;
        if (tbl.f[index] == normal_date_two_select) {
            tbl.f[index] = normal_date_three_select;
        }
    }
    tbl.f[D_800E6280.unk_110A](0x40);
    if (tbl.f[D_800E6280.unk_110A] == func_8013864C && D_8013CB04 == 0) {
        kind = ((u8 **)D_800CA2D4)[D_800CA2DC][D_800CA2E0];
        if (kind == 8 || kind == 3) {
            func_80044750(0x207);
            D_8013CB04 = 1;
        }
    }
}

/* FAKE: byte view keeps selector base in a0; direct member access uses v0. Original source unknown. T-9710. */
void func_80139E8C(void) {
    u8 var_a0;
    RECT rect;

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
    rect.x = 0x140;
    rect.y = 0x1A0;
    rect.w = 0x140;
    rect.h = 0x58;
    func_8009C7F8(&rect, 8, 0, 0);
    rect.x = 0x2C0;
    rect.y = 0xA0;
    func_8009C7F8(&rect, 8, 0, 0);
    var_a0 = ((u8 *)&D_800E6280)[0xF5F];
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    if (var_a0 >= 0xEU) {
        var_a0 = 0xD;
    }
    D_800E6280.unk_F5F = var_a0;
    func_800847B8(var_a0);
    func_8007C740();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    D_80122CE0 = 0xFF;
    D_800CA2F4 = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80139D00", func_80139FD4);

void func_8013A1B4(void) {
    func_80042940(0x22);
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80139D00", func_8013A1D4);

void func_8013A274(void) {
    if (D_800E6280.unk_F5F == 2) {
        func_80083440(3);
    }
    if (D_800E6280.unk_F5F == 7 && D_800CA2F4 == 0) {
        func_80083440(4);
    }
    func_8004284C();
}

void func_8013A2D8(void) {
    if (D_80122CDC == 0) {
        func_8004284C();
        return;
    }
    func_801386C4();
}

void func_8013A314(void) {
    if (D_80122CDC == 0) {
        func_8007ED84(0x4225);
        func_800AE0F0(D_800CA19C, "ホテル・ロビー");
    } else {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

void func_8013A37C(void) {
    D_800CA2DC += D_800CA2CC;
    func_8004284C();
}

void func_8013A3B4(void) {
    if (D_800CA2EC == 0) {
        D_800CA2DC = (D_800CA2DC - (D_80122CDC * 2)) + 2;
    } else {
        D_800CA2DC = (D_800CA2DC - (D_80122CDC * 2)) + 4;
    }
    func_8004284C();
}

void func_8013A42C(void) {
    u8 temp;
    D_8013CB04 = 0;
    temp = D_800CA2CC;
    switch (temp) {
    case 0:
        func_8007ED84(0x4267);
        func_800AE0F0(D_800CA19C, "土産屋");
        break;
    case 1:
        func_8007ED84(0x4255);
        func_800AE0F0(D_800CA19C, "浜辺");
        break;
    default:
        func_8007ED84(0x427B);
        func_800AE0F0(D_800CA19C, "クラーク像前");
        break;
    }
    if (D_800CA2EC == 0) {
        func_80085B3C(3, D_800CA2CC + 3);
    }
    func_8004284C();
}

void func_8013A4F8(void) {
    s32 temp;
    temp = D_800CA2CC;
    switch (temp) {
    case 0:
        func_8007ED84(0x4267);
        func_800AE0F0(D_800CA19C, "土産屋");
        break;
    case 1:
        func_8007ED84(0x4255);
        func_800AE0F0(D_800CA19C, "浜辺");
        break;
    default:
        func_8007ED84(0x427B);
        func_800AE0F0(D_800CA19C, "クラーク像前");
        break;
    }
    func_8004284C();
}

void func_8013A59C(void) {
    if (D_800CA2CC == 0) {
        D_80122CE0 = 2;
    } else if (D_800CA2CC == 1) {
        D_80122CE0 = 1;
    } else {
        D_80122CE0 = 3;
    }
    func_80048EB8(0);
    func_8006BD6C(0);
    D_800E6280.unk_720 = D_800E6280.unk_1108;
    D_800E6280.unk_721 = D_800E6280.unk_1109;
    D_800E6280.unk_722 = 0x32;
    func_8006492C(0);
    func_8004E9F4(0);
    main_state_set(0x81);
}

void func_8013A648(void) {
    D_800CA2DC = (D_800CA2DC - D_800CA2CC) + (D_80122CDC == 1) + ((D_80122CDC == 2) * 3) + 2;
    D_800CA2F4 = D_80122CDC > 0;
    if (D_80122CDC == 0) {
        func_80137C3C();
    } else {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += 5;
    }
    func_80084D3C();
    if (D_800CA2EC != 0) {
        if (D_80122CDC == 2) {
            D_800E6280.unk_F5F = 0xD;
        } else if (D_80122CDC == 1) {
            D_800E6280.unk_F5F = 0xFF;
        }
    }
    func_8004284C();
}

void func_8013A74C(void) {
    if (D_800CA2CC == 1) {
        cd_read_sectors(0x3D, 0x801E0000, 0xBD04);
        func_80139D00();
        func_8004284C();
        return;
    }
    func_8004284C();
    func_8004284C();
}

void func_8013A7AC(void) {
    if (D_800CA2CC == 1) {
        load_palette(D_8013CB00, 0x11, 0, 1, 0);
        func_80048F64(0x61);
        D_80120695 = 0xC;
        D_80120696 = 1;
        D_801206CC = 0x01000000;
        D_80120697 = 0x84;
        D_801206A0 = D_8013CAF4;
        D_801206A4 = D_8013CAF8;
        D_801206C8 = D_8013CAF0;
        D_801206A8 = D_8013CAFC;
        D_801206AA = 0;
        D_801206AC = 0;
        D_8012069A = 1;
        D_801206D7 = 0x11;
        D_801206BA = 0x10;
        D_801206BE = -0x77;
        D_8012069B = 0;
        D_80120699 = 1;
        D_80120698 = 0;
        func_8004284C();
        return;
    }
    func_8004284C();
}

void func_8013A8D8(void) {
    if (D_800CA2CC == 1) {
        D_8012069B = D_800B593C;
    }
    normal_date_bg_fadein();
}

void func_8013A918(void) {
    if (D_800CA2CC == 1 && D_80122CDC != 0) {
        D_8012069B = D_800B593C;
        if (D_800B593C != 0) {
            D_80120697 |= 0x80;
        }
    }
    normal_date_bg_fadein();
}

s32 func_8013A980(void) {
    func_8007E99C();
    if (D_800CA2CC == 1) {
        D_8012069B = D_800B593C;
        if ((u8) D_800B593C < 8U) {
            D_80120697 &= 0x7F;
        }
    }
}

void func_8013A9E0(void) {
    u32 temp_t8;

    temp_t8 = (u8)func_80051A68(4) & 0x7F;
    if (D_800E6280.unk_F5F != 4 || temp_t8 >= 2U) {
        func_8004284C();
        return;
    }
    D_800E6280.unk_721 = D_800E6280.unk_1109;
    D_800E6280.unk_722 = D_800E6280.unk_110A + 2;
    D_800E6280.unk_75D = 4;
    func_80042908(0xA);
}
