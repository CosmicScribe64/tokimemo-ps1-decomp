#include "common.h"
#include "game.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D10);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D14);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D18);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D1C);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D20);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D24);
u8 D_800B3D28 = 0;
u8 D_800B3D2C = 0;
s32 D_800B3D30 = 0;
s32 D_800B3D34 = 0;
s32 D_800B3D38 = 0;
s32 D_800B3D3C = 0;
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D40);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D44);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D48);
u8 D_800B3D4C = 0;
u8 D_800B3D50 = 0;
u8 D_800B3D54[12] = { 0 };

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80043510);

void draw2d3d(u8 arg0, u8 arg1) {
    D_800E6280.unk_1114 = arg0;
    D_800E6280.unk_1113 = arg1;
}

void back_clear_switch(s32 arg0) {
    if (arg0 != 0) {
        D_800E6280.unk_039 = 1;
    } else {
        D_800E6280.unk_039 = 0;
    }
}

/* Read queue counters directly and write records through g to preserve the
 * original register allocation. K&R parameters keep narrow home slots. T-9930. */
s32 load_palette(arg0, arg1, arg2, arg3, arg4)
s32 arg0;
u8 arg1;
u8 arg2;
u8 arg3;
u8 arg4;
{
    u32 index;
    GameState *g;

    g = &D_800E6280;
    index = D_800E6280.unk_1428;
    if (index < 32) {
        g->unk_1328[index].unk_00 = arg0;
        g->unk_1328[index].unk_04 = arg1;
        g->unk_1328[index].unk_05 = arg2;
        g->unk_1328[index].unk_06 = arg3;
        g->unk_1328[index].unk_07 = arg4;
        g->unk_1428++;
        return 1;
    }
    return 0;
}

s32 load_csr_ab(arg0, arg1, arg2, arg3, arg4, arg5)
s32 arg0;
u8 arg1;
u16 arg2;
u16 arg3;
u16 arg4;
u16 arg5;
{
    u32 index;
    GameState *g;

    g = &D_800E6280;
    index = D_800E6280.unk_1429;
    if (index < 32) {
        g->unk_142C[index].unk_00 = arg0;
        g->unk_142C[index].unk_04 = arg1;
        g->unk_142C[index].unk_05 = 0;
        g->unk_142C[index].unk_06 = 0;
        g->unk_142C[index].unk_07 = 0;
        g->unk_142C[index].unk_08 = arg2;
        g->unk_142C[index].unk_0A = arg3;
        g->unk_142C[index].unk_0C = arg4;
        g->unk_142C[index].unk_0E = arg5;
        g->unk_1429++;
        return 1;
    }
    return 0;
}

s32 load_csr_tp(arg0, arg1, arg2, arg3, arg4, arg5, arg6)
s32 arg0;
u8 arg1;
u8 arg2;
u16 arg3;
u16 arg4;
u16 arg5;
u16 arg6;
{
    u32 index;
    GameState *g;

    g = &D_800E6280;
    index = D_800E6280.unk_1429;
    if (index < 32) {
        g->unk_142C[index].unk_00 = arg0;
        g->unk_142C[index].unk_04 = arg1;
        g->unk_142C[index].unk_05 = arg2;
        g->unk_142C[index].unk_06 = 0;
        g->unk_142C[index].unk_07 = 0;
        g->unk_142C[index].unk_08 = arg3;
        g->unk_142C[index].unk_0A = arg4;
        g->unk_142C[index].unk_0C = arg5;
        g->unk_142C[index].unk_0E = arg6;
        g->unk_1429++;
        return 1;
    }
    return 0;
}

/* NON_MATCHING: T-10850, 26 differing listing lines; see the ticket scratch evidence. */
#ifdef NON_MATCHING
void csr_load_vram(s32 arg0) {
    GameState *base;
    s32 var_s1;
    s32 temp_v0;

    base = &D_800E6280;
    var_s1 = 0;
    if (base->unk_1429 != 0) {
        do {
            temp_v0 = base->unk_142C[var_s1].unk_04;
            if (temp_v0 == 0) {
                LoadSquare(base->unk_142C[var_s1].unk_08, base->unk_142C[var_s1].unk_0A, base->unk_142C[var_s1].unk_0C, base->unk_142C[var_s1].unk_0E, (void *) base->unk_142C[var_s1].unk_00);
            } else {
                LoadSquare((((temp_v0 & 0xF) << 6) + base->unk_142C[var_s1].unk_08), (((temp_v0 & 0x10) * 0x10) + base->unk_142C[var_s1].unk_0A), ((u16) base->unk_142C[var_s1].unk_0C >> base->unk_142C[var_s1].unk_05), base->unk_142C[var_s1].unk_0E, (void *) base->unk_142C[var_s1].unk_00);
            }
            var_s1 += 1;
        } while (var_s1 < (u8) base->unk_1429);
    }
    base->unk_1429 = 0U;
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80043510", csr_load_vram);
#endif

INCLUDE_ASM("asm/nonmatchings/main/80043510", palette_load_vram);

void LoadSquare(u16 arg0, u16 arg1, u16 arg2, u16 arg3, void *arg4) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C884(&rect, arg4);
}

void MoveSquare(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4, u16 arg5) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C93C(&rect, arg4, arg5);
}

void StoreSquare(u16 arg0, u16 arg1, u16 arg2, u16 arg3, void *arg4) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C8E0(&rect, arg4);
}

void func_80044434(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_8004443C);

void func_80044700(s32 arg0, s32 arg1, s32 arg2) {
    RECT rect;
    s32 x;

    x = arg1;
    rect.x = x << 4;
    rect.y = arg0 + 0x1E0;
    rect.w = 0x10;
    rect.h = 1;
    func_8009C93C(&rect, 0x100, arg2 + 0x1E0);
}

s32 func_80044750(s32 arg0) {
    func_80079B10(arg0 & 0xFFFF);
    return 1;
}


s32 func_80044774(s32 arg0) {
    s32 *p;

    if (arg0 != 0) {
        p = (s32 *)0x8002E800;
    } else {
        p = (s32 *)0x8001C000;
    }
    if ((*p & 0xFFFF) != 0x40) {
        return -1;
    }
    if (arg0 != 0) {
        func_8004500C(D_800B3D44, D_800B3D48, arg0);
    } else {
        func_80044750(0x1200);
    }
    D_800B3D40 = arg0;
    return arg0 & 0xFF;
}

u8 func_8004480C(void) {
    return D_800B3D40;
}

u8 func_8004481C(void) {
    return D_800B3D44;
}


void func_8004482C(void) {
    D_800B3D54[0] = 0;
    D_800B3D54[1] = 0;
    D_800B3D44 = 0xFF;
    D_800B3D30 = 0;
    D_800B3D34 = 0;
    D_800B3D38 = 0;
    D_800B3D3C = 0;
    D_800B3D28 = 0;
    D_800B3D2C = 0;
    D_800B3D4C = 0;
    D_800B3D50 = 0;
    D_800B3D48 = 0;
}

void func_80044890(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
    D_800B3D44 = 0xFF;
    D_800B3D28 = 0;
    D_800B3D2C = 0;
    D_800B3D24 = (u8) a;
    D_800B3D4C = 0;
    D_800B3D50 = 1;
    D_800B3D20 = func_800460DC();
    if (a == 0) {
        func_800451E0(0x801B8000);
        if (D_800B3D30 == b) {
            D_800B3D20 -= 2;
            D_800B3D28 = 1;
        } else {
            if (b != 0) {
                func_800462C8(3U, 0x80010000, b);
                D_800B3D30 = b;
            } else {
                D_800B3D20 -= 1;
                D_800B3D28 = 1;
            }
            if (c != 0) {
                func_800462C8(0x20U, 0x801B8000, c);
            } else {
                D_800B3D20 -= 1;
                D_800B3D28 = 1;
            }
        }
        if (D_800B3D34 == d) {
            D_800B3D20 -= 3;
            D_800B3D2C = 1;
            return;
        }
        if (f != 0) {
            func_800462C8(0x25U, 0x8001C000, f);
            D_800B3D34 = d;
        } else {
            D_800B3D20 -= 1;
            D_800B3D2C = 1;
        }
        if (e != 0) {
            func_800462C8(0x50U, 0x801C8000, e);
        } else {
            D_800B3D20 -= 1;
            D_800B3D2C = 1;
        }
        if (d != 0) {
            func_800462C8(6U, 0x80012000, d);
            D_800B3D34 = d;
            return;
        }
        D_800B3D20 -= 1;
        D_800B3D2C = 1;
        return;
    }
    func_800451E0(0x801B8000);
    if (D_800B3D38 == b) {
        D_800B3D20 -= 2;
        D_800B3D28 = 1;
    } else {
        if (b != 0) {
            func_800462C8(3U, 0x8001A000, b);
            D_800B3D38 = b;
        } else {
            D_800B3D20 -= 1;
            D_800B3D28 = 1;
        }
        if (c != 0) {
            func_800462C8(0x35U, 0x801E0000, c);
        } else {
            D_800B3D20 -= 1;
            D_800B3D28 = 1;
        }
    }
    if (D_800B3D3C == d) {
        D_800B3D20 -= 3;
        D_800B3D2C = 1;
        return;
    }
    if (f != 0) {
        func_800462C8(0x25U, 0x8002E800, f);
        D_800B3D3C = d;
    } else {
        D_800B3D20 -= 1;
        D_800B3D2C = 1;
    }
    if (e != 0) {
        func_800462C8(0x50U, 0x801B8000, e);
    } else {
        D_800B3D20 -= 1;
        D_800B3D2C = 1;
    }
    if (d != 0) {
        func_800462C8(6U, 0x80016000, d);
        D_800B3D3C = d;
        return;
    }
    D_800B3D20 -= 1;
    D_800B3D2C = 1;
}

/* NON_MATCHING: T-17019, 3 differing lines; register of the D_800B3D4C load */
#ifdef NON_MATCHING
s32 func_80044C98(void) {
    if ((u32) (((func_800460DC() - D_800B3D20) + 0x100) & 0xFF) >= 5U) {
        if (D_800B3D4C == 0) {
            func_80044D54();
            D_800B3D4C += 1;
        } else {
            D_800B3D4C += 1;
            if ((D_800B3D4C > 0xC0U) || (func_80044F94((s32) D_800B3D24) != 0)) {
                D_800B3D54[D_800B3D24] = 0;
                return 1;
            }
        }
    }
    return -1;
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80044C98);
#endif

s32 func_80044D54(void) {
    if (D_800B3D24 == 0) {
        if (D_800B3D28 == 0 && D_800B3D2C == 0) {
            func_800451E0(0x801B8000);
            func_80044750(0x11);
            return 1;
        }
        if (D_800B3D28 == 0) {
            func_800451E0(0x801B8000);
            func_80044750(0x13);
            return 1;
        }
        if (D_800B3D2C == 0) {
            func_800451E0(0x801C8000);
            func_80044750(0x14);
            return 1;
        }
        return 0;
    }
    if (D_800B3D28 == 0 && D_800B3D2C == 0) {
        func_800451E0(0x801B8000);
        func_80044750(0x12);
        return 1;
    }
    if (D_800B3D28 == 0) {
        func_800451E0(0x801E0000);
        func_80044750(0x16);
        return 1;
    }
    if (D_800B3D2C == 0) {
        func_800451E0(0x801B8000);
        func_80044750(0x15);
        return 1;
    }
    return 0;
}

/* NON_MATCHING: T-17019, 8 differing lines; same prologue as func_80044C98: the D_800B3D4C load is in $v0 (original $v1) and the copy of the increment is missing (T-0018) */
#ifdef NON_MATCHING
s32 func_80044E8C(void) {
    if ((u32) (((func_800460DC() - D_800B3D20) + 0x100) & 0xFF) >= 5U) {
        if (D_800B3D4C == 0) {
            func_80044D54();
            D_800B3D4C += 1;
        } else {
            D_800B3D4C += 1;
            if ((D_800B3D4C > 0xC0U) || (func_80044F94((s32) D_800B3D24) != 0)) {
                if (D_800B3D24 == 0) {
                    func_80044750(0x22);
                    func_80044750(0x21);
                    func_80044750(0x2F);
                } else {
                    func_80044750(0x23);
                    func_80044750(0x24);
                    func_80044750(0x2E);
                }
                D_800B3D54[D_800B3D24] = 0;
                return 1;
            }
        }
    }
    return -1;
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80044E8C);
#endif

s32 func_80044F94(s32 arg0) {
    if (arg0 == 0) {
        if (func_80079E00(0) != 0 && func_80079E00(1) != 0) {
            return 1;
        }
        return 0;
    }
    if (func_80079E00(2) != 0 && func_80079E00(3) != 0) {
        return 1;
    }
    return 0;
}

s32 func_8004500C(s32 arg0, s32 arg1) {
    s32 *var_v0_2;

    if (arg0 != 0) {
        var_v0_2 = (s32 *)0x8002E800;
    } else {
        var_v0_2 = (s32 *)0x8001C000;
    }
    if ((*var_v0_2 & 0xFFFF) != 0x40) {
        return -1;
    }
    if (arg0 == 0) {
        func_80044750(0x22);
        func_80044750(0x21);
        func_80044750(0x2F);
        func_80044750((arg1 | 0x200) & 0xFFFF);
    } else {
        func_80044750(0x23);
        func_80044750(0x24);
        func_80044750(0x2E);
        func_80044750((arg1 | 0x200) & 0xFFFF);
    }
    D_800B3D44 = (u8) arg0;
    D_800B3D48 = (u8) arg1;
    D_800B3D40 = 1;
    /* no return here: the original returns the v0 of the last func_80044750 call (implicit int) */
}

s32 func_800450F4(s32 arg0, s32 arg1) {
    s32 *var_v0;

    if (arg0 != 0) {
        var_v0 = (s32 *)0x8002E800;
    } else {
        var_v0 = (s32 *)0x8001C000;
    }
    if ((*var_v0 & 0xFFFF) != 0x40) {
        return -1;
    }
    if (arg0 == 0) {
        func_80044750(0x22);
        func_80044750(0x2F);
        func_80044750((arg1 | 0x200) & 0xFFFF);
    } else {
        func_80044750(0x23);
        func_80044750(0x2E);
        func_80044750((arg1 | 0x200) & 0xFFFF);
    }
    D_800B3D44 = (u8) arg0;
    D_800B3D48 = (u8) arg1;
    D_800B3D40 = 1;
    /* no return here: the original returns the v0 of the last func_80044750 call (implicit int) */
}
