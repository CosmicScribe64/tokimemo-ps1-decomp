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

/* bulk_m2c declarations (T-11190) */
extern s8 D_800E8BF4;
extern s8 D_800E8BF5;
extern u8 D_800E8BF3;
extern u8 D_800E8C31;
extern u8 D_800E8C32;
extern u8 D_800E8C33;
/* end bulk_m2c declarations */
void func_80042CB0(void) {
    s16 temp_a3;
    s16 var_a3;
    s16 var_t0;
    s32 var_at;
    s32 var_v0;
    u8 temp_t3;
    u8 temp_t8;
    u8 var_a1;

    temp_a3 = D_800E6280.unk_F7A + (s16) ((s32) (D_800E8BF4 * D_800E6280.unk_F6F) / 3);
    var_a3 = temp_a3;
    var_t0 = D_800E6280.unk_F7C + (s16) ((s32) (D_800E8BF5 * D_800E6280.unk_F6F) / 3);
    if (temp_a3 >= 0x8D) {
        var_a3 = 0x8C;
    }
    if (var_a3 < -0x96) {
        var_a3 = -0x96;
    }
    var_at = var_t0 < 0x65;
    if (var_t0 < -0x64) {
        var_t0 = -0x64;
        var_at = -0x64 < 0x65;
    }
    if (var_at == 0) {
        var_t0 = 0x64;
    }
    ((DisplayActor44 *)D_8011ECD0)->unk_26 = var_a3;
    ((DisplayActor44 *)D_8011ECD0)->unk_2A = var_t0;
    temp_t8 = ~D_800E8BF3 & 0xFF;
    D_800E6280.unk_F7A = var_a3;
    temp_t3 = temp_t8 & ~D_800E6280.unk0 & 0xFF;
    D_800E6280.unk_F7C = var_t0;
    if ((D_800E6280.unk0 != 0) && ((D_800E6280.unk0 == 0) || (temp_t3 != D_800E6280.unk_F72)) && (temp_t3 != 0)) {
        if (temp_t3 & 4) {
            D_800E6280.unk_F72 = temp_t3;
            D_800E6280.unk_F71 = (u8) D_800E6280.unk_F73 + 8;
        } else if (temp_t3 & 8) {
            D_800E6280.unk_F72 = temp_t3;
            D_800E6280.unk_F71 = (u8) D_800E6280.unk_F73 + 8;
        }
    }
    var_a1 = (u8) D_800E6280.unk_F71;
    var_v0 = 0;
    D_800E6280.unk_F77 = temp_t3;
    if ((D_800E6280.unk0 == 0) || ((D_800E6280.unk_F77 = temp_t3, (D_800E6280.unk0 != 0)) && (D_800E6280.unk_F77 = temp_t3, (var_a1 != 0)) && (D_800E6280.unk_F77 = temp_t3, (((u8) D_800E6280.unk_F73 + 8) != var_a1)) && (D_800E6280.unk_F77 = temp_t3, (temp_t3 == D_800E6280.unk_F72)))) {
        D_800E6280.unk_F72 = 0;
        var_a1 = 0;
        if (D_800E6280.unk_F75 != 0) {
            if (temp_t8 & 4) {
                var_v0 = 0x20;
            } else if (temp_t8 & 8) {
                var_v0 = 0x40;
            }
        } else if (temp_t8 & 8) {
            var_v0 = 0x20;
        } else if (temp_t8 & 4) {
            var_v0 = 0x40;
        }
    }
    if (var_a1 != 0) {
        D_800E6280.unk_F71 = var_a1 - 1;
    } else {
        D_800E6280.unk_F71 = (s8) var_a1;
        D_800E6280.unk_F72 = 0;
    }
    if ((*D_800E8C30 == 0) && ((D_800E8C31 & 0xF0) == 0x40)) {
        var_v0 = var_v0 | (~D_800E8C32 << 0x18) | ((~D_800E8C33 & 0xFF) << 0x10);
    }
    D_800E6280.unk_F88 = ~(s32) D_800E6280.unk0 & var_v0;
    D_800E6280.unk_F8C = (s32) D_800E6280.unk0 & ~var_v0;
    D_800E6280.unk_F78 = temp_t8;
    D_800E6280.unk_F80 = var_v0;
    D_800E6280.unk_F76 = temp_t8;
    D_800E6280.unk_F84 = (s32) D_800E6280.unk0;
}

INCLUDE_ASM("asm/nonmatchings/main/80042A00", func_80043010);

INCLUDE_ASM("asm/nonmatchings/main/80042A00", func_800430C0);

INCLUDE_ASM("asm/nonmatchings/main/80042A00", func_80043448);

void func_80043504(void) {}
