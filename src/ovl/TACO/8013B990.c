#include "common.h"
#include "ovl/TACO.h"

void func_8013B990(void) {
    func_8013C1F4();
    func_8013C440();
    if (game_state.mode_stage == 0xC && D_8015EDCC != 0) {
        return;
    }
    if (game_state.mode_stage == 0x16 && D_8015EDCC >= 2) {
        return;
    }
    if (game_state.mode_stage == 0x20 && D_8015EDCC >= 3) {
        return;
    }
    func_8013BAC4(D_8015E270, 1, 0);
    func_8013BAC4(D_8015E4F0, 4, 0);
    func_8013BAC4(D_8015E3B0, 1, 1);
    func_8013BAC4(D_8015E630, 4, 1);
    D_8015EDF0 = D_8015EDF0 / 10 * 10 + 1;
    if (D_8015EDF0 >= 0x989680) {
        D_8015EDF0 = 0x98967F;
    }
}

/* FAKE: s32 with no return keeps v0 live at the exit, so as1 fills the first branch slot of the
 * particle loop from the latch (void moves the kind test there), as in func_8013C1F4. T-25111. */
s32 func_8013BAC4(Tc14 *arg0, s32 arg1, s32 arg2) {
    /* The order of these scalars sets their frame homes (i third, damage tenth). T-25111. */
    s32 x;
    s32 y;
    s32 i;
    s32 z;
    s32 j;
    s32 radius;
    s32 xdist;
    s32 ydist;
    s32 kind;
    u32 damage;
    s32 width;
    TcDamage9 damageTable;
    u32 scores[256];

    damageTable = D_8015F548;
    /* FAKE: through the byte view, tools/cvt_pass.py does not see a switch on an unsigned global
     * and leaves global copy propagation on. The y distances then fold into their compares, as in
     * the original (no `move a0,v0`), and the stage is still compared in v1. T-25111. */
    switch (*(u8 *)&game_state.mode_stage) {
    case 0xC:
        for (j = 0; j < 256; j++) scores[j] = 0x190;
        break;
    case 0x16:
        for (j = 0; j < 256; j++) scores[j] = 0xC8;
        break;
    case 0x20:
        for (j = 0; j < 256; j++) scores[j] = 0x320;
        break;
    default:
        for (j = 0; j < 256; j++) scores[j] = D_8015F504[j];
        break;
    }
    D_8015F500 = 0;
    if (arg0->unk13 & 0x20) {
        for (i = 0; i != 2; i++) {
            if (game_state.frame_count & 1) {
                x = (i + arg0)->unk0;
                y = (i + arg0)->unk2;
                if (arg2 == 0) z = D_8015EDB4[0].unk78;
                else z = D_8015EDB4[8].unk78;
                for (j = 16; j != 63; j++) {
                    if (((j + D_8015EDB4)->unk0[1] & 0x80) && ((j + D_8015EDB4)->unk84[3] & 1) && (j + D_8015EDB4)->unk84[1] != 0 && (j + D_8015EDB4)->unk84[0] != 0) {
                        radius = (j + D_8015EDB4)->unk84[0] * 16;
                        xdist = abs(x - (j + D_8015EDB4)->unk74);
                        ydist = abs(y - (j + D_8015EDB4)->unk76);
                        if (xdist < radius + 0x18 && ydist < radius + 0x18) {
                            if ((j + D_8015EDB4)->unk78 < z) {
                                if (D_8015F500 == 0 && arg2 == 0) {
                                    D_8015F500 = 1;
                                    func_801335A0(1, 0x507);
                                }
                                if ((j + D_8015EDB4)->unk84[1] != 0) (j + D_8015EDB4)->unk84[1]--;
                                func_8013D2A0(x, y, (j + D_8015EDB4)->unk78 - 0x200);
                                D_8015EDF0 += scores[(D_8015EDB4 + j)->unk84[2] % 17] / 20;
                                /* continue, not a fall-through: with a plain if, ugen reuses the stored D_8015EDF0 (T-25111) */
                                if ((j + D_8015EDB4)->unk84[1] != 0) continue;
                                D_8015EDF0 += scores[(D_8015EDB4 + j)->unk84[2] % 17];
                            }
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (((i + arg0)->unk13 & 0x80) && (kind = (i + arg0)->unk13 & 15) != 0) {
            x = (i + arg0)->unk0;
            y = (i + arg0)->unk2;
            z = (i + arg0)->unk4;
            damage = damageTable.bytes[kind];
            if (kind == 8) width = 0x60;
            else width = 0x30;
            for (j = 16; j != 63; j++) {
                if (((j + D_8015EDB4)->unk0[1] & 0x80) && ((j + D_8015EDB4)->unk84[3] & 1) && (j + D_8015EDB4)->unk84[1] != 0 && (j + D_8015EDB4)->unk84[0] != 0) {
                    radius = (j + D_8015EDB4)->unk84[0] * 16;
                    if (abs(z - (j + D_8015EDB4)->unk78) < radius + 0x100) {
                        xdist = abs(x - (j + D_8015EDB4)->unk74);
                        ydist = abs(y - (j + D_8015EDB4)->unk76);
                        if (xdist < radius + width && ydist < radius + width) {
                            if (D_8015F500 == 0 && arg2 == 0) {
                                D_8015F500 = 1;
                                func_801335A0(1, 0x507);
                            }
                            if (damage < (j + D_8015EDB4)->unk84[1]) (j + D_8015EDB4)->unk84[1] -= damage;
                            else (j + D_8015EDB4)->unk84[1] = 0;
                            if ((j + D_8015EDB4)->unk84[1] == 0) (j + D_8015EDB4)->unk84[3] &= 0xF0;
                            (i + arg0)->unk12 = 0;
                            (i + arg0)->unk13 = 0;
                            func_8013D2A0(((j + D_8015EDB4)->unk74 + x) / 2, ((j + D_8015EDB4)->unk76 + y) / 2, z - 0x200);
                            D_8015EDF0 += scores[(D_8015EDB4 + j)->unk84[2] % 17] / 10 * damage;
                            if ((j + D_8015EDB4)->unk84[1] == 0) {
                                func_801335A0(1, 0x508);
                                D_8015EDF0 += scores[(D_8015EDB4 + j)->unk84[2] % 17];
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}

/* FAKE: s32 with no return keeps the loop counter init (v0) live at the exit (void drops two
 * instructions); the hit in the second loop returns before it. T-19006. */
s32 func_8013C1F4(void) {
    u32 i;
    s32 a;
    s32 b;
    u8 value;
    u8 value2;
    D_8015F500 = 0;
    if (!(D_8015EDB4->unk2 & 0x80)) {
        for (i = 0; i < 16; i++) {
            if (D_8015EB30[i].unk13 & 0x80) {
                a = abs(D_8015EB30[i].unk0 - D_8015EDB4->unk74);
                b = abs(D_8015EB30[i].unk2 - D_8015EDB4->unk76);
                if (abs(D_8015EB30[i].unk4 - D_8015EDB4->unk78) < 0x100 && b < 0x80 && a < 0x80) {
                    D_8015EB30[i].unk13 = 0;
                    func_801335A0(1, 0x506);
                    D_8015EDB4->unk2 |= 4;
                    value = D_8015EDB4->unk84[1];
                    if (value >= 5) {
                        D_8015EDB4->unk84[1] = value - 4;
                    } else {
                        D_8015EDB4->unk84[1] = 0;
                    }
                    D_8015EDB4->unk7A = 0x10;
                    break;
                }
            }
        }
    }
    if (!(D_8015EDB4[8].unk2 & 0x80)) {
        for (i = 0; i < 16; i++) {
            if (D_8015EB30[i].unk13 & 0x80) {
                a = abs(D_8015EB30[i].unk0 - D_8015EDB4[8].unk74);
                b = abs(D_8015EB30[i].unk2 - D_8015EDB4[8].unk76);
                if (abs(D_8015EB30[i].unk4 - D_8015EDB4[8].unk78) < 0x100 && b < 0x80 && a < 0x80) {
                    D_8015EB30[i].unk13 = 0;
                    value2 = D_8015EDB4[8].unk84[1];
                    if (value2 >= 5) {
                        D_8015EDB4[8].unk84[1] = value2 - 4;
                    } else {
                        D_8015EDB4[8].unk84[1] = 0;
                    }
                    D_8015EDB4[8].unk7A = 0x10;
                    return;
                }
            }
        }
    }
}

/* FAKE: s32 with no return keeps the loop counter init (v0) live at the exit (void changes the output),
 * as in func_8013C1F4. The first loop is entered by a goto instead of an enclosing if: the if form
 * shares the guard's load of D_8015EDB4 with the loop's pointer, the original loads it again. T-19006. */
s32 func_8013C440(void) {
    u32 i;
    s32 a;
    s32 b;
    s32 c;
    s32 limit;
    u8 value;
    u8 value2;
    D_8015F500 = 0;
    if (D_8015EDB4->unk2 & 0x80) {
        goto skip1;
    }
    for (i = 16; i < 63; i++) {
        if (((i + D_8015EDB4)->unk0[1] & 0xC0) && ((i + D_8015EDB4)->unk84[1] != 0)) {
            a = abs((i + D_8015EDB4)->unk74 - D_8015EDB4->unk74);
            b = abs((i + D_8015EDB4)->unk76 - D_8015EDB4->unk76);
            c = abs((i + D_8015EDB4)->unk78 - D_8015EDB4->unk78);
            limit = (s32)(i + D_8015EDB4)->unk84[0] << 4;
            if (c < 0x100 && b < limit && a < limit) {
                func_801335A0(1, 0x506);
                D_8015EDB4->unk2 |= 4;
                value = D_8015EDB4->unk84[1];
                if (value >= 0xD) {
                    D_8015EDB4->unk84[1] = value - 0xC;
                } else {
                    D_8015EDB4->unk84[1] = 0;
                }
                D_8015EDB4->unk7A = 0x10;
                break;
            }
        }
    }
skip1:
    if (D_8015EDB4[8].unk2 & 0x80) {
        return;
    }
    for (i = 0; i < 16; i++) {
        if (((i + D_8015EDB4)->unk0[1] & 0xC0) && ((i + D_8015EDB4)->unk84[1] != 0)) {
            a = abs((i + D_8015EDB4)->unk74 - D_8015EDB4[8].unk74);
            b = abs((i + D_8015EDB4)->unk76 - D_8015EDB4[8].unk76);
            c = abs((i + D_8015EDB4)->unk78 - D_8015EDB4[8].unk78);
            limit = (s32)(i + D_8015EDB4)->unk84[0] << 4;
            if (c < 0x100 && b < limit && a < limit) {
                value2 = D_8015EDB4[8].unk84[1];
                if (value2 >= 0xD) {
                    D_8015EDB4[8].unk84[1] = value2 - 0xC;
                } else {
                    D_8015EDB4[8].unk84[1] = 0;
                }
                D_8015EDB4[8].unk7A = 0x10;
                return;
            }
        }
    }
}

/* FAKE: s32 with no return keeps the second loop's counter init (v0) live at the first loop's exit
 * (void drops the `move v0,zero`), as in func_8013C1F4. T-25111. */
s32 func_8013C6D4(void) {
    s32 pad[6]; /* FAKE: T-25111, six home words above the locals; the original frame has them, their source is unknown */
    Tc14 *p;
    s32 dy;
    s32 dx;
    u8 kind;
    u8 value;
    s32 last_kind;
    s32 v;
    s32 got2;
    u32 i; /* declared between the flags: its home sits between them in the original frame */
    s32 got_bonus;
    s32 got5;
    s32 sound_ok;

    sound_ok = 1;
    last_kind = -7;
    got2 = -7;
    got_bonus = -7;
    got5 = -7;
    for (p = D_8015EC70; p < D_8015EC70 + 16; p++) {
        if (p->unk13 & 0x80) {
            dx = abs(p->unk0 - D_8015EDB4->unk74);
            dy = abs(p->unk2 - D_8015EDB4->unk76);
            if (abs(p->unk4 - D_8015EDB4->unk78) < 0x200 && dy < 0x200 && dx < 0x200) {
                D_8015EDF0 += 1000;
                kind = p->unk11;
                p->unk13 = 0;
                switch (kind) {
                case 0:
                case 1:
                    last_kind = kind;
                    break;
                case 2:
                    got2 = 1;
                    break;
                case 3:
                case 4:
                    func_801335A0(1, 0x509);
                    if (D_8015EDB4->unk7C == 0) {
                        D_8015EDB4->unk7C = p->unk11;
                    } else if (D_8015EDB4->unk7E == 0) {
                        D_8015EDB4->unk7E = p->unk11;
                    } else {
                        D_8015EDB4->unk80 = p->unk11;
                    }
                    break;
                case 5:
                    got5 = 1;
                    value = D_8015EDB4->unk84[1];
                    if (value < 0xEF) {
                        D_8015EDB4->unk84[1] = value + 0x10;
                    } else {
                        D_8015EDB4->unk84[1] = 0xFF;
                    }
                    break;
                default:
                    D_8015EDD0 = 0x20;
                    got_bonus = 1;
                    D_8015EDD4 = 1;
                    switch (rand() & 7) {
                    case 0:
                        D_8015EDD4 = 100;
                        break;
                    case 1:
                        D_8015EDD4 = 200;
                        break;
                    case 2:
                        D_8015EDD4 = 400;
                        break;
                    case 3:
                        D_8015EDD4 = 800;
                        break;
                    case 4:
                        D_8015EDD4 = 1600;
                        break;
                    case 5:
                        D_8015EDD4 = 3200;
                        break;
                    case 6:
                        D_8015EDD4 = 6400;
                        break;
                    case 7:
                    default: /* every path assigns D_8015EDD4, so uopt keeps it in v0 up to the sum */
                        D_8015EDD4 = 57300;
                        break;
                    }
                    D_8015EDF0 += D_8015EDD4;
                    break;
                case 7:
                    last_kind = -1;
                    break;
                }
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (D_8015EC70[i].unk13 & 0x80) {
            v = D_8015EC70[i].unk11;
            if (v != 3 && v != 4) {
                dx = abs(D_8015EC70[i].unk0 - D_8015EDB4[8].unk74);
                dy = abs(D_8015EC70[i].unk2 - D_8015EDB4[8].unk76);
                v = abs(D_8015EC70[i].unk4 - D_8015EDB4[8].unk78);
                if (v < 0x200 && dy < 0x200 && dx < 0x200) {
                    v = D_8015EC70[i].unk11;
                    D_8015EC70[i].unk13 = 0;
                    switch (v) {
                    case 0:
                    case 1:
                    case 7:
                        D_8015EDB4[8].unk72 = v;
                        break;
                    case 2:
                        v = D_8015EDB4[8].unk82;
                        if (v == -1) {
                            D_8015EDB4[8].unk82 = 1;
                            v = D_8015EDB4[8].unk82;
                        }
                        D_8015EDB4[8].unk82 = 1 - v;
                        break;
                    case 5:
                        value = D_8015EDB4[8].unk84[1];
                        if (value < 0xEF) {
                            D_8015EDB4[8].unk84[1] = value + 0x10;
                        } else {
                            D_8015EDB4[8].unk84[1] = 0xFF;
                        }
                        break;
                    }
                }
            }
        }
    }
    if (last_kind != D_8015EDB4->unk72) {
        if (last_kind != -7) {
            D_8015EDB4->unk72 = last_kind;
            switch (last_kind) {
            case -1:
                func_80133410(0);
                func_801335A0(1, 0x500);
                sound_ok = 0;
                break;
            case 0:
                func_80133410(0);
                func_801335A0(1, 0x514);
                sound_ok = 0;
                break;
            case 1:
                func_80133410(0);
                func_801335A0(1, 0x512);
                sound_ok = 0;
                break;
            }
        }
    }
    if (got2 == 1) {
        v = D_8015EDB4->unk82;
        if (v == -1) {
            D_8015EDB4->unk82 = 1;
            v = D_8015EDB4->unk82;
        }
        D_8015EDB4->unk82 = 1 - v;
        if (sound_ok != 0) {
            switch (D_8015EDB4->unk82) {
            case 0:
                func_801335A0(1, 0x511);
                sound_ok = 0;
                break;
            case 1:
                func_801335A0(1, 0x516);
                sound_ok = 0;
                break;
            }
        }
    }
    if (got5 == 1) {
        if (sound_ok != 0) {
            func_801335A0(1, 0x510);
            sound_ok = 0;
        }
    }
    if (got_bonus == 1) {
        if (sound_ok != 0) {
            func_801335A0(1, 0x509);
        }
    }
}

/* FAKE: s32 with no return keeps v0 live at the exit (void changes one instruction), as in
 * func_8013CE78. The i == 8 test is dead (the loop starts at 16) but the original has it. T-19006. */
s32 func_8013CC40(void) {
    u32 i;
    for (i = 16; i < 64; i++) {
        if (((i + D_8015EDB4)->unk84[1] == 0) && ((i + D_8015EDB4)->unk0[1] & 0x80)) {
            if (!(game_state.frame_count & 0x1F)) {
                func_801335A0(1, 0x508);
            }
            func_8013D1E0(*(TcPos *)&(i + D_8015EDB4)->unk74, *(TcPos *)&(i + D_8015EDB4)->unk7C, 0);
            func_8013D1E0(*(TcPos *)&(i + D_8015EDB4)->unk74, *(TcPos *)&(i + D_8015EDB4)->unk7C, 0);
            if (i == 8) {
                (D_8015EDB4 + i)->unk84[1] = rand() & 0x3F;
                (D_8015EDB4 + i)->unk74 = rand() % 100;
                (D_8015EDB4 + i)->unk76 = rand() % 100;
            }
        }
    }
}

void func_8013CE70(void) {
}

/* FAKE: s32 fall-through and the two pointer-addition orders retain the
 * original result/address registers. T-10010. */
s32 func_8013CE78(void) {
    u32 i;
    u8 value;
    for (i = 16; i < 64; i++) {
        if (((i + D_8015EDB4)->unk0[1] & 0x80) && ((i + D_8015EDB4)->unk84[3] & 1)) {
            value = (i + D_8015EDB4)->unk84[1];
            if (value != 0 && (i + D_8015EDB4)->unk84[0] != 0) {
                if (value >= 3U) {
                    (i + D_8015EDB4)->unk84[1] = value - 2;
                } else {
                    (i + D_8015EDB4)->unk84[1] = 0;
                }
                func_8013D2A0((i + D_8015EDB4)->unk74, (i + D_8015EDB4)->unk76, (i + D_8015EDB4)->unk78 - 0x200);
                if ((i + D_8015EDB4)->unk84[1] == 0) {
                    func_801335A0(1, 0x508);
                    D_8015EDF0 += D_8015F504[(D_8015EDB4 + i)->unk84[2] % 17U];
                }
            }
        }
    }
}

/* FAKE: s32 with no return keeps v0 live at the exit, as in func_8013CE78 (void changes one
 * instruction). T-19006. */
s32 func_8013CFD8(void) {
    u32 i;
    s16 x;
    s16 y;
    s32 a;
    s32 b;
    s32 d;
    s32 t;
    s32 u;
    x = D_8015E770[0].unk0;
    y = D_8015E770[0].unk2;
    for (i = 16; i < 64; i++) {
        if (((i + D_8015EDB4)->unk0[1] & 0x80) && ((i + D_8015EDB4)->unk84[3] & 1) && ((i + D_8015EDB4)->unk84[1] != 0) && ((i + D_8015EDB4)->unk84[0] != 0)) {
            t = abs(x - (i + D_8015EDB4)->unk74);
            u = abs(y - (i + D_8015EDB4)->unk76);
            a = 0x1000 - t;
            if (a < 0) {
                a = 0;
            }
            b = 0x1000 - u;
            if (b < 0) {
                b = 0;
            }
            d = (a + b) / 256;
            if (d != 0) {
                func_8013D2A0((i + D_8015EDB4)->unk74, (i + D_8015EDB4)->unk76, (i + D_8015EDB4)->unk78 - 0x200);
            }
            if ((u32)d < (i + D_8015EDB4)->unk84[1]) {
                (i + D_8015EDB4)->unk84[1] -= d;
            } else {
                (i + D_8015EDB4)->unk84[1] = 0;
            }
            if ((i + D_8015EDB4)->unk84[1] == 0) {
                func_801335A0(1, 0x508);
                D_8015EDF0 += D_8015F504[(D_8015EDB4 + i)->unk84[2] % 17U];
            }
        }
    }
}
