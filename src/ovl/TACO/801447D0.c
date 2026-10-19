#include "common.h"
#include "ovl/TACO.h"

void func_801447D0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014488C();
        break;
    case 1:
        func_80145DC4();
        break;
    case 2:
        func_8014616C();
        break;
    case 3:
        func_80146754();
        break;
    case 4:
        func_80144998();
        break;
    }
    if (D_800E6280.unk_110A != 5) {
        if (D_800E6280.unk_F88 & 0x860) {
            func_80042940(4);
        }
    }
}

void func_8014488C(void) {
    (&D_800E6280.unk_036)[0] = (&D_800E6280.unk_036)[1] = 8;
    D_800E6280.unk_038 = 0x10;
    draw2d3d(1, 1);
    func_80058D20();
    func_80146E3C();
    func_80146D60(-0xFA0);
    func_801471D0(0x8019C800);
    func_80144CC0((u8 *)D_800E6280.unk_163C);
    func_80144CC0(D_800E6280.unk_183C);
    func_8009C674(0);
    func_80144D90(7);
    func_80147488(1, D_8015F82C[0], 0);
    D_80127090.unk0 = 0;
    D_80128880[1].x = -0x5A;
    D_80128880[1].z = 0;
    D_80128880[1].y = 0x5A;
    func_80147488(2, D_8015F82C[1], 0);
    D_801270A0.unk0 = 0;
    D_80128880[2].x = -0x5A;
    D_80128880[2].z = 0;
    D_80128880[2].y = -0x5A;
    func_8004284C();
}

void func_80144998(void) {
    D_80127090.unk0 = 0x80000000;
    D_801270A0.unk0 = 0x80000000;
    draw2d3d(1, 0);
    func_80042908(2);
}

void func_801449D8(s32 *arg0, s32 arg1, s32 arg2) {
    /* FAKE: reserves the 0x130 bytes of frame the original has above its packet table; real locals unknown. T-19002 */
    u8 pad[0x130];
    POLY_F4 *pkt[25];
    s32 i;
    s32 x;
    s32 y;
    s32 fade;

    for (i = 0; i != 25; i++) {
        pkt[i] = (POLY_F4 *)GetWorkBase(0x18, D_8011ECA0);
        func_8009F02C(pkt[i]);
        SetSemiTrans(pkt[i], 1);
        SetShadeTex(pkt[i], 0);
        arg0[i * 2] += arg1;
        arg0[i * 2 + 1] += arg2;
        x = arg0[i * 2];
        y = arg0[i * 2 + 1] + D_800E6280.unk_018[D_8011ECA0];
        if ((x < 0) || (x >= 0x141)) {
            if (arg1 > 0) {
                arg0[i * 2] = 0;
            } else {
                arg0[i * 2] = 0x140;
            }
            arg0[i * 2 + 1] = func_800AE0D0() % 240 - 0x78;
        } else if ((y < 0) || (y >= 0x1E1)) {
            arg0[i * 2] = func_800AE0D0() % 320;
            if (arg2 > 0) {
                arg0[i * 2 + 1] = -0x78;
            } else {
                arg0[i * 2 + 1] = 0x78;
            }
        }
        pkt[i]->x0 = x;
        pkt[i]->y0 = y;
        pkt[i]->x1 = x + 1;
        pkt[i]->y1 = y;
        pkt[i]->x2 = x;
        pkt[i]->y2 = y + 1;
        pkt[i]->x3 = x + 1;
        pkt[i]->y3 = y + 1;
        if ((func_800AE0D0() % 5) >= 3) {
            pkt[i]->r0 = 0x80;
            pkt[i]->g0 = 0x80;
            pkt[i]->b0 = 0x80;
        } else {
            pkt[i]->r0 = 0xFF;
            pkt[i]->g0 = 0xFF;
            pkt[i]->b0 = 0xFF;
        }
        if ((func_800AE0D0() % 3) > 0) {
            AddPrim((D_8011ECA0 << 10) + 0x190 + D_800E8CA0, (s32)pkt[i]);
        }
    }
    fade = GetWorkBase(0xC, D_8011ECA0);
    func_8009D294(fade, 0, 0, func_8009ECB0(1, 1, 0, 0), 0);
    AddPrim((D_8011ECA0 << 10) + D_800E8CA0, fade);
    safe_env(0);
}

void func_80144CC0(u8 *arg0) {
    s32 i;

    for (i = 0; i != 0x19; i++) {
        *(s32 *)(arg0 + i * 8) = func_800AE0D0() % 320;
        *(s32 *)(arg0 + i * 8 + 4) = func_800AE0D0() % 240 - 0x78;
    }
}

void func_80144D90(s32 arg0) {
    RECT r;

    if (arg0 < 0) {
        arg0 = 0;
    } else if (arg0 >= 8) {
        arg0 = 7;
    }
    /* FAKE: the assignment pairs share a source line each; as1 orders the stores by line (T-3330), T-6050 */
    r.x = arg0 * 8; r.y = 0x1F1;
    r.w = 8; r.h = 1;
    func_8009C93C(&r, 0xF8, 0x1F0);
}

void func_80144DF8(s32 arg0) {
    TcObj50 *o;
    TcPos *p;

    o = &D_80127480[arg0];
    p = &D_80128880[arg0];
    o->unk18 = 0;
    o->unk1C = 0;
    o->unk20 = 0;
    p->x = 0;
    p->y = 0;
    p->z = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80144E3C);

void func_8014559C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    if ((arg4 >= 0) && (arg4 < 0x1F)) {
        func_80144E3C(arg0, arg1, arg2, arg3, (arg4 & 0xFF) * 8, 0, arg5);
        return;
    }
    if ((arg4 >= 0x1F) && (arg4 < 0x23)) {
        func_80144E3C(arg0, arg1, arg2, arg3, 0, 2, arg5);
        return;
    }
    if ((arg4 >= 0x23) && (arg4 < 0x61)) {
        func_80144E3C(arg0, arg1, arg2, arg3, ((arg4 - 0x23) & 0xFF) * 4, 1, arg5);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80145650);

s32 func_80145DC4(void) {
    s32 t;
    s32 i;

    D_8012274C = 0;
    D_80122750 = D_80127480[1].unk1C;
    t = D_800E6280.unk_1104.w;
    D_80122754 = -0x7D0;
    func_80099540(&D_80122740);
    switch (D_800E6280.unk_110D) {
    case 0:
        func_801449D8(D_800E6280.unk_163C, 0, 0xA);
        func_801449D8((s32 *)D_800E6280.unk_183C, 0, 8);
        if (t < 0x1E) {
            for (i = 0; i != 2; i++) {
                func_80147488(i + 1, D_8015F82C[i], 1);
            }
        } else if ((t >= 0x1E) && (t < 0x5A)) {
            for (i = 0; i != 2; i++) {
                func_80147488(i + 1, D_8015F82C[i], 2);
            }
        } else if ((t >= 0x5A) && (t < 0x96)) {
            for (i = 0; i != 2; i++) {
                func_80147488(i + 1, D_8015F82C[i], 3);
            }
        } else if ((t >= 0x96) && (t < 0xB4)) {
            for (i = 0; i != 2; i++) {
                func_80147488(i + 1, D_8015F82C[i], 4);
            }
        } else if ((t >= 0xB4) && (t < 0xD2)) {
            for (i = 0; i != 2; i++) {
                func_80147488(i + 1, D_8015F82C[i], 5);
            }
        } else if ((t >= 0xD2) && (t < 0x10E)) {
            for (i = 0; i != 2; i++) {
                func_80147488(i + 1, D_8015F82C[i], 6);
            }
        } else if ((t >= 0x10E) && (t < 0x14A)) {
            for (i = 0; i != 2; i++) {
                func_80147488(i + 1, D_8015F82C[i], 7);
            }
        } else if ((t >= 0x14A) && (t < 0x168)) {
            for (i = 0; i != 2; i++) {
                func_80147488(i + 1, D_8015F82C[i], 8);
            }
        } else if (t == 0x168) {
            D_800E6280.unk_1104.w = 0;
            D_800E6280.unk_110D += 1;
        }
        if (t == 0x1E) {
            func_801335A0(0, 0x505);
        }
        if (t == 0xAA) {
            func_801335A0(0, 0x507);
        }
        if (t == 0x12C) {
            func_801335A0(0, 0x506);
        }
        break;
    case 1:
        if (t == 1) {
            D_80127080[1].unk0 = 0x80000000;
            D_80127080[2].unk0 = 0x80000000;
        } else if (t >= 0x3C) {
            func_8004284C();
        }
        break;
    }
    D_800E6280.unk_1104.w += 1;
}

s32 func_8014616C(void) {
    s32 t;

    t = D_800E6280.unk_1104.w;
    if (t == 1) {
        func_80144DF8(1);
        func_80147488(1, D_8015F82C[0], 9);
        D_80127090.unk0 = 0;
        D_80122754 = 0;
        D_80122750 = 0;
        D_8012274C = 0;
        func_80099540(&D_80122740);
    } else if ((t >= 2) && (t < 0x32)) {
        func_80147488(1, D_8015F82C[0], 0xA);
    } else if ((t >= 0x32) && (t < 0x64)) {
        func_80147488(1, D_8015F82C[0], 0xB);
    } else if ((t >= 0x64) && (t < 0x78)) {
        func_80147488(1, D_8015F82C[0], 0xC);
    } else if ((t >= 0x78) && (t < 0x91)) {
        func_80147488(1, D_8015F82C[0], 0xD);
    } else if ((t >= 0x91) && (t < 0xB4)) {
        func_80147488(1, D_8015F82C[0], 0xE);
    } else if ((t >= 0xB4) && (t < 0xC8)) {
        func_80147488(1, D_8015F82C[0], 0x10);
    } else if ((t >= 0xC8) && (t < 0xEB)) {
        func_80147488(1, D_8015F82C[0], 0x11);
    } else if ((t >= 0xEB) && (t < 0x11D)) {
        func_80147488(1, D_8015F82C[0], 0x12);
    } else if ((t >= 0x11D) && (t < 0x14F)) {
        func_80147488(1, D_8015F82C[0], 0x13);
    } else if ((t >= 0x14F) && (t < 0x1C2)) {
        func_80147488(1, D_8015F82C[0], 0x14);
    } else if ((t >= 0x1CC) && (t < 0x1EA)) {
        func_80147488(1, D_8015F82C[0], 0x15);
    } else if ((t >= 0x1F4) && (t < 0x208)) {
        func_80147488(1, D_8015F82C[0], 0x16);
    }
    if ((t >= 0x64) && (t < 0xBE)) {
        func_8014559C(0, -0x41, 0x32, 0x11, t - 0x55, 0);
    }
    if ((t >= 0xB4) && (t < 0x118)) {
        func_8014559C(0, -0x1E, 0x1E, 0x4B, t - 0xAA, 0);
    }
    if ((t >= 0xAA) && (t < 0x10E)) {
        func_8014559C(0, 0x32, 0x3C, 0xA, t - 0xAA, 0);
    }
    if ((t >= 0xBE) && (t < 0x122)) {
        func_8014559C(0, -0x50, 0xA, 0x50, t - 0xBE, 0);
    }
    if ((t >= 0xEB) && (t < 0x14F)) {
        func_8014559C(0, -0x3C, 0x19, 0x14, t - 0xE1, 0);
    }
    if ((t >= 0x10E) && (t < 0x168)) {
        func_8014559C(0, -0x17, 0x1C, 0x1E, t - 0xFA, 0);
    }
    if ((t >= 0x11D) && (t < 0x181)) {
        func_8014559C(0, 0x14, 0x23, 0x28, t - 0x113, 0);
    }
    if ((t >= 0x136) && (t < 0x19A)) {
        func_8014559C(0, 0x3C, 0x2D, 0x1E, t - 0x12C, 0);
    }
    if ((t >= 0x154) && (t < 0x1B8)) {
        func_8014559C(0, -0x50, 0x3C, 0x14, t - 0x140, 0);
    }
    if ((t >= 0x168) && (t < 0x1CC)) {
        func_8014559C(0, 0, 0x14, 0x1E, t - 0x168, 0);
    }
    if ((t >= 0x186) && (t < 0x1EA)) {
        func_8014559C(0, 0x28, 0x32, 0x1E, t - 0x186, 0);
    }
    if ((t >= 0x1AE) && (t < 0x212)) {
        func_8014559C(0, -0x5A, 0x14, 0x1E, t - 0x1AE, 0);
    }
    if ((t >= 0x1CC) && (t < 0x230)) {
        func_8014559C(0, -0x28, 0x28, 0x1E, t - 0x1CC, 0);
    }
    if ((t >= 0x1E0) && (t < 0x244)) {
        func_8014559C(0, 0x32, 0x3C, 5, t - 0x1E0, 0);
    }
    if (t == 0xA) {
        func_801335A0(0, 0x505);
    }
    if (t == 0x64) {
        func_801335A0(0, 0x506);
    }
    if (t == 0xDC) {
        func_801335A0(0, 0x506);
    }
    if (t == 0x190) {
        func_801335A0(0, 0x507);
    }
    if (t == 0x1E0) {
        func_801335A0(0, 0x505);
    }
    if (t < 0x244) {
        func_801449D8(D_800E6280.unk_163C, -0xA, 0);
        func_801449D8((s32 *)D_800E6280.unk_183C, -8, 0);
    }
    if (t == 0x280) {
        D_80127090.unk0 = 0x80000000;
        func_8004284C();
    }
    /* FAKE: the frame counter is read through its own symbol for the final `+= 1`. The original reads it twice, as two uopt
     * variables (the compare chain, then the increment); through GameState both reads join one web and the function is 165
     * rows off. Keep line with proof=func_8014616C:165:bb8762f0 in config/migrate_globals.txt, wiki/keep-lines.md. T-20820, T-21520 */
    D_800E7384 += 1;
}

s32 func_80146754(void) {
    s32 t;
    s32 x;

    t = D_800E6280.unk_1104.w;
    if (t == 1) {
        func_80144DF8(2);
        func_80147488(2, D_8015F82C[1], 9);
        D_80127080[2].unk0 = 0;
        D_80122754 = 0;
        D_80122750 = 0;
        D_8012274C = 0;
        func_80099540(&D_80122740);
    } else if ((t >= 2) && (t < 0x64)) {
        func_80147488(2, D_8015F82C[1], 0xA);
    } else if ((t >= 0x65) && (t < 0x78)) {
        func_80147488(2, D_8015F82C[1], 0xB);
    } else if ((t >= 0x79) && (t < 0x8C)) {
        func_80147488(2, D_8015F82C[1], 0xC);
    } else if ((t >= 0x8D) && (t < 0xA0)) {
        func_80147488(2, D_8015F82C[1], 0xD);
    } else if ((t >= 0xA1) && (t < 0xB4)) {
        func_80147488(2, D_8015F82C[1], 0xE);
    } else if ((t >= 0xB5) && (t < 0xC8)) {
        func_80147488(2, D_8015F82C[1], 0xF);
    } else if ((t >= 0xC9) && (t < 0xDC)) {
        func_80147488(2, D_8015F82C[1], 0x10);
    } else if ((t >= 0xDD) && (t < 0xFA)) {
        func_80147488(2, D_8015F82C[1], 0x11);
    } else if ((t >= 0xFB) && (t < 0x122)) {
        func_80147488(2, D_8015F82C[1], 0x12);
    } else if ((t >= 0x123) && (t < 0x15E)) {
        func_80147488(2, D_8015F82C[1], 0x13);
    } else if ((t >= 0x15F) && (t < 0x186)) {
        func_80147488(2, D_8015F82C[1], 0x14);
    } else if ((t >= 0x187) && (t < 0x1A4)) {
        func_80147488(2, D_8015F82C[1], 0x15);
    } else if ((t >= 0x1A5) && (t < 0x1F4)) {
        func_80147488(2, D_8015F82C[1], 0x16);
    }
    if ((t >= 0x32) && (t < 0x96)) {
        func_8014559C(0x14, -0xAA, 0x1E, 0x14, t - 0x1E, 0x3C);
    }
    if ((t >= 0x69) && (t < 0xC8)) {
        func_8014559C(0xAA, -0xAA, 0x50, 8, t - 0x50, 0x3C);
    }
    if ((t >= 0x78) && (t < 0xDC)) {
        func_8014559C(-0x1E, -0xAA, 0x50, 0xA, t - 0x78, 0x3C);
    }
    if ((t >= 0xB4) && (t < 0x118)) {
        func_8014559C(0xC8, -0xAA, 0x32, 0xF, t - 0xAA, 0x3C);
    }
    if ((t >= 0xDC) && (t < 0x140)) {
        func_8014559C(0x5A, -0xAA, 0x3C, 0xA, t - 0xD2, 0x3C);
    }
    if ((t >= 0x122) && (t < 0x136)) {
        func_8014559C(0xF, -0x96, 0x2D, 0xA, t - 0x118, 0x3C);
    }
    if ((t >= 0x136) && (t < 0x186)) {
        func_8014559C((t * 2) - 0x25D, -0x96, 0x2D, 0xA, t - 0x118, (t - 0x136) / 3 + 0x3C);
    }
    if ((t >= 0x190) && (t < 0x1F4)) {
        func_8014559C(0x15E, -0x96, 0x50, 0xA, t - 0x186, 0x78);
    }
    if ((t >= 0x1A4) && (t < 0x1F4)) {
        func_8014559C(0xDC, -0x96, 0x3C, 0x14, t - 0x19A, 0x78);
    }
    if ((t >= 0x1B8) && (t < 0x21C)) {
        func_8014559C(0x96, -0x96, 0x28, 0x14, t - 0x1AE, 0x78);
    }
    if (t == 0x64) {
        func_801335A0(0, 0x507);
    }
    if (t == 0xE6) {
        func_801335A0(0, 0x505);
    }
    if (t == 0x12C) {
        func_801335A0(0, 0x506);
    }
    if (t == 0x190) {
        func_801335A0(0, 0x505);
    }
    if (t < 0x12C) {
        func_801449D8(D_800E6280.unk_163C, -5, -0xA);
        func_801449D8((s32 *)D_800E6280.unk_183C, -5, -8);
    } else if ((t >= 0x12C) && (t < 0x190)) {
        x = (t - 0x12C) / 10 - 5;
        func_801449D8(D_800E6280.unk_163C, x, -0xA);
        func_801449D8((s32 *)D_800E6280.unk_183C, x, -8);
    } else if (t >= 0x190) {
        func_801449D8(D_800E6280.unk_163C, 5, -0xA);
        func_801449D8((s32 *)D_800E6280.unk_183C, 5, -8);
    }
    if (t == 0x226) {
        func_80042940(4);
    }
    /* FAKE: the frame counter is read through its own symbol for the final `+= 1`. The original reads it twice, as two uopt
     * variables (the compare chain, then the increment); through GameState both reads join one web and the function is 156
     * rows off. Keep line with proof=func_80146754:156:5e8407a9 in config/migrate_globals.txt, wiki/keep-lines.md. T-20820, T-24129 */
    D_800E7384 += 1;
}
