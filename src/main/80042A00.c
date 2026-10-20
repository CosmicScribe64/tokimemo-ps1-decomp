#include "common.h"
#include "game.h"

void func_80042A00(void) {
    if (func_80042AC8() == 0) {
        if (D_800E6280.unk_F74 == 0) {
            func_80042CB0();
        } else if (D_800E6280.unk_F74 != 0xFF) {
            func_800430C0();
        }
        if (D_800B3CA0 != 0 && (D_800E6280.unk_F88 & 0xF9DF)) {
            D_800B3CA0 = 0;
        }
        if ((D_800E6280.unk_F80 & 0xF00000) == 0xF00000) {
            D_800B3CA0 = 1;
        }
        if (D_800E6280.unk_F88 & 0x08000000) {
            D_800E6280.unk_F6E = 1 - (u8)D_800E6280.unk_F6E;
        }
    }
}

s32 func_80042AC8(void) {
    GameState *g = &D_800E6280;
    u8 old;

    old = g->unk_F74;
    if (D_800E8BF0[0] == 0) {
        if ((D_800E8BF0[1] & 0xF) * 2 <= 0x40) {
            if ((D_800E8BF0[1] & 0xF0) == 0x10) {
                g->unk_F74 = 0;
                g->unk_1092 = 0;
            } else {
                g->unk_F74 = 1;
            }
        }
    } else if (D_800E8BF0[0] != 0xFF || g->unk_035 == 0) {
        g->unk_F74 = 0xFF;
    }
    if (old != g->unk_F74 && (g->unk_F74 != 0xFF || old != 0xFF)) {
        if (g->unk_F74 == 0) {
            func_80042C30();
            func_80042C74();
            func_80042BD4();
            return 1;
        }
        if (g->unk_F74 == 1) {
            func_80042C74();
            func_80042C30();
        }
        return 1;
    }
    return 0;
}

void func_80042BD4(void) {
    InitMouse(D_800E8BF0, D_800E8C30);
    SenseMouse(3, 4);
    SetMouse(0, 0xA0, 0x78);
    RangeMouse(0, 0x140, 0, 0xF0);
}

void func_80042C30(void) {
    InitPAD(D_800E8BF0, 8, D_800E8C30, 8);
    StartPAD();
    ChangeClearPAD(0);
}

void func_80042C74(void) {
    D_8011ECF6 = 0;
    D_8011ECFA = 0;
    D_800E6280.unk_F7A = 0;
    D_800E6280.unk_F7C = 0;
    D_800E6280.unk_F80 = 0;
    D_800E6280.unk_F84 = 0;
    D_800E6280.unk_F88 = 0;
}

/* Mouse/pad polling and the double-click state machine. Fields go through `g`; the F6F reads use the
 * global (it keeps the pad bytes loaded first, T-24301). */
void func_80042CB0(void) {
    GameState *g = &D_800E6280;
    s32 dx;
    s32 dy;
    s16 qx;
    s16 qy;
    s32 b2;
    s32 b3;

    dx = *(s8 *)&D_800E8BF0[4];
    dy = *(s8 *)&D_800E8BF0[5];
    qx = (s32)(dx * D_800E6280.unk_F6F) / 3;
    qy = (s32)(dy * D_800E6280.unk_F6F) / 3;
    /* FAKE: T-24301, dead stores; without them uopt colours the clamps differently (134 rows) */
    dx = 0;
    dy = 0;
    g->unk_F7A += qx;
    g->unk_F7C += qy;
    if (g->unk_F7A >= 0x8D) {
        g->unk_F7A = 0x8C;
    }
    if (g->unk_F7A < -0x96) {
        g->unk_F7A = -0x96;
    }
    if (g->unk_F7C < -0x64) {
        g->unk_F7C = -0x64;
    }
    if (g->unk_F7C >= 0x65) {
        g->unk_F7C = 0x64;
    }
    D_8011ECF6 = g->unk_F7A;
    D_8011ECFA = g->unk_F7C;
    g->unk_F76 = ~D_800E8BF0[3];
    g->unk_F84 = g->unk_F80;
    g->unk_F77 = g->unk_F76 & ~g->unk_F78;
    if (*(u8 *)&g->unk_F70 != 0 && (*(u8 *)&g->unk_F71 == 0 || g->unk_F72 != g->unk_F77) && g->unk_F77 != 0) {
        if (g->unk_F77 & 4) {
            *(u8 *)&g->unk_F71 = *(u8 *)&g->unk_F73 + 8;
            g->unk_F72 = g->unk_F77;
        } else if (g->unk_F77 & 8) {
            *(u8 *)&g->unk_F71 = *(u8 *)&g->unk_F73 + 8;
            g->unk_F72 = g->unk_F77;
        }
    }
    g->unk_F80 = 0;
    if (*(u8 *)&g->unk_F70 == 0 || (*(u8 *)&g->unk_F70 != 0 && *(u8 *)&g->unk_F71 != 0 && *(u8 *)&g->unk_F73 + 8 != *(u8 *)&g->unk_F71 && g->unk_F72 == g->unk_F77)) {
        g->unk_F72 = 0;
        *(u8 *)&g->unk_F71 = 0;
        if (g->unk_F75 != 0) {
            if (g->unk_F76 & 4) {
                g->unk_F80 |= 0x20;
            } else if (g->unk_F76 & 8) {
                g->unk_F80 |= 0x40;
            }
        } else if (g->unk_F76 & 8) {
            g->unk_F80 |= 0x20;
        } else if (g->unk_F76 & 4) {
            g->unk_F80 |= 0x40;
        }
    }
    if (*(u8 *)&g->unk_F71 != 0) {
        *(u8 *)&g->unk_F71 -= 1;
    } else {
        g->unk_F72 = 0;
    }
    if (D_800E8C30[0] == 0 && (D_800E8C30[1] & 0xF0) == 0x40) {
        b2 = ~D_800E8C30[3];
        b3 = D_800E8C30[2];
        g->unk_F80 |= ~b3 << 24;
        g->unk_F80 |= (b2 & 0xFF) << 16;
    }
    g->unk_F88 = ~g->unk_F84 & g->unk_F80;
    g->unk_F8C = g->unk_F84 & (u32)~g->unk_F80;
    g->unk_F78 = g->unk_F76;
}

/* Implicit int (no return value on the unk_F74 == 0xFF path in the original). */
s32 func_80043010(void) {
    s32 b0;
    s32 b1;
    s32 b2;
    s32 b3;
    s32 a0;

    if (D_800E6280.unk_F74 != 0xFF) {
        if (D_800E6280.unk_F74 != 0) {
            b0 = ~D_800E8BF0[3];
            b1 = ~D_800E8BF0[2];
            b2 = ~D_800E8C30[3];
            b3 = D_800E8C30[2];
            return (~b3 << 24) + ((b2 & 0xFF) << 16) + ((b1 & 0xFF) << 8) + (b0 & 0xFF);
        }
        a0 = 0;
        b0 = ~D_800E8BF0[3] & 0xFF;
        if (b0 & 8) {
            a0 = 0x20;
        }
        if (b0 & 4) {
            a0 |= 0x40;
        }
        return a0;
    }
}

s32 func_800430C0(void) {
    s32 raw;

    D_800E6280.unk_F84 = D_800E6280.unk_F80;
    raw = func_80043010();
    if ((raw & 0xFFFF0000) == 0xFFFF0000) {
        D_800E6280.unk_F80 = raw & 0xFFFF;
        if (D_800E6280.unk_F80 == 0xFFFF) {
            D_800E6280.unk_F80 = 0;
        }
    } else {
        D_800E6280.unk_F80 = raw;
        if ((raw & 0xFFFF) == 0xFFFF) {
            if ((raw & 0xFFFF0000) == 0xFFFF0000) {
                D_800E6280.unk_F80 = 0;
            } else {
                D_800E6280.unk_F80 = raw & 0xFFFF0000;
            }
        }
    }
    D_800E6280.unk_F88 = ~D_800E6280.unk_F84 & D_800E6280.unk_F80;
    /* FAKE: the (u32) cast keeps ugen's operand order (prev, ~pad) of the original; real source unknown. T-17009 */
    D_800E6280.unk_F8C = D_800E6280.unk_F84 & (u32)~D_800E6280.unk_F80;
    if (D_800B3CA0 != 0 && (D_800E6280.unk_10F8 & 1)) {
        D_800E6280.unk_F88 |= 0x20;
    }
    if (D_800E6280.unk_1111 == 1) {
        if (D_800E6280.unk_F88 & 0x08000800) {
            if (D_800E6280.unk_1110 == 0) {
                D_800E6280.unk_1110 = 1;
                D_800E6280.unk_1112 = 0;
            } else {
                D_800E6280.unk_1112 = 1;
                D_800E6280.unk_1110 = 0;
            }
        }
    }
    if (D_800E6280.unk_1108 != 0xD0 && D_800E6280.unk_1108 != 0x20 && D_800E6280.unk_1108 != 0x90 && D_800E6280.unk_F74 == 1) {
        if ((D_800E6280.unk_F80 & 0x90C) == 0x90C) {
            if (D_800B3CA4++ >= 0x81) {
                D_800B3CA4 = 0;
                func_800789E0();
                func_800591D8(0);
                func_800412E0(0);
                main_state_set(0);
                func_80041584();
                func_80048DD0(0);
                back_clear_switch(1);
                dec_bg_reset();
                func_8004482C();
                tpage_buf_clear();
                func_80044750(0x7F);
                func_800424FC();
                func_8007BDE8();
                func_8007B844();
                func_80042488();
                func_800452C4();
                func_80041878();
                func_80042058();
                func_80041F48();
                if (D_800E6280.unk_035 != 0) {
                    func_80053D10();
                }
                func_8006BC28(0);
                return -1;
            }
        } else if ((D_800E6280.unk_F80 & 0x090C0000) == 0x090C0000) {
            if (D_800B3CA4++ >= 0x81) {
                D_800B3CA4 = 0;
                func_800789E0();
                func_800591D8(0);
                func_800412E0(0);
                main_state_set(0x11);
                func_80041584();
                func_80048DD0(0);
                back_clear_switch(1);
                dec_bg_reset();
                func_8004482C();
                tpage_buf_clear();
                func_80044750(0x7F);
                func_800424FC();
                func_8007BDE8();
                func_8007B844();
                func_80042488();
                func_800452C4();
                func_80041878();
                func_80042058();
                func_80041F48();
                if (D_800E6280.unk_035 != 0) {
                    func_80053D10();
                }
                func_8006BC28(0);
                return -1;
            }
        } else {
            D_800B3CA4 = 0;
        }
    }
    /* no return value on this path, as in the original (v0 is whatever func_80043448 leaves) */
    func_80043448();
}

void func_80043448(void) {
    if (D_800B3CFC != 0) {
        if (++D_800B3D00 == 0) {
            D_800B3CFC = 0;
            D_800B3CF8 = 0;
        }
    } else {
        if (D_800E6280.unk_F80 == D_800B3CA8[D_800B3CF8]) {
            if (++D_800B3CF8 >= 0x12U) {
                D_800B3CFC = 1;
            }
            D_800B3D00 = 0x10;
        }
        if (--D_800B3D00 == 0) {
            D_800B3CF8 = 0;
        }
    }
}

void func_80043504(void) {}
