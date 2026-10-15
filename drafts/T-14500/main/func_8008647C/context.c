#include "common.h"
#include "game.h"

void func_80085E30(s32 arg0, s32 arg1) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048E78();
    back_clear_switch(arg0);
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_038 = 0;
    D_800E6280.unk_03A = 0x80;
    func_8004E58C();
    D_800E6280.unk_10A2 = 0;
    if (arg1 != 0) {
        tpage_buf_clear_all();
        return;
    }
    tpage_buf_clear();
}

void Vblnk_Timer_Init(void) {
    D_800E36F0 = D_800E6280.unk_10F4;
    D_800E36F4 = 0;
}

s32 Vblnk_Timer(void) {
    D_800E36F4 = D_800E6280.unk_10F4 - D_800E36F0;
    return D_800E36F4;
}

INCLUDE_ASM("asm/nonmatchings/main/80085E30", Scroll);

void Rot_2D(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    s32 s;
    s32 c;

    s = rsin();
    c = func_800A0140(arg0);
    *arg3 = c * *arg1 / 4096 - *arg2 * s / 4096;
    *arg4 = s * *arg1 / 4096 + *arg2 * c / 4096;
}

void Fade_Out_Init(void) {
    Vblnk_Timer_Init();
}

INCLUDE_ASM("asm/nonmatchings/main/80085E30", Fade_Out_Main);

void Fade_In_Init(void) {
    Vblnk_Timer_Init();
}

INCLUDE_ASM("asm/nonmatchings/main/80085E30", Fade_In_Main);

void Yubi(s32 idx) {
    u8 *p;
    func_8006BA40();
    p = D_8011ECD0 + idx * 0x44;
    *(s16 *)(p + 0x26) = D_8011ECF6;
    *(s16 *)(p + 0x2A) = D_8011ECFA;
}

void Default_Disp(void) {
    k_disp_inc2();
    check_k_scroll();
    func_80083A10();
    message_window_show();
    hizuke_show();
    func_80066C08(2);
    func_80066334();
    func_80047560();
}

s32 func_8008647C(void) {
    s32 sp6C[16];
    s32 sp70;
    GameState *var_v0;
    s32 *var_s1;
    s32 temp_a2;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s3;
    s32 var_s2;
    s32 var_s4;
    s32 var_s5;
    u32 temp_s0_3;
    u32 temp_s0_4;
    u32 var_s2_2;
    u32 var_s6;

    var_s6 = 0;
    var_v0 = &D_800E6280;
    var_s2 = 0;
    do {
        if (((u32) (var_v0->unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F) == 1) {
            (&sp6C[0])[var_s6] = var_s2;
            var_s6 = (var_s6 + 1) & 0xFF;
        }
        var_s2 += 1;
        var_v0 += 0x38;
    } while (var_s2 < 0xB);
    temp_s0 = get_h_yuukou(0) & 0xFF;
    var_s5 = get_h_tokimeki(0) + temp_s0;
    var_s4 = 0;
    var_s2_2 = 1;
    if (var_s6 >= 2U) {
        var_s1 = &sp70;
        do {
            temp_s0_2 = get_h_tokimeki(*var_s1) & 0xFF;
            temp_s3 = get_h_yuukou(*var_s1) + temp_s0_2;
            if ((var_s5 < temp_s3) && (temp_s0_3 = func_8008667C(var_s4) & 0xFF, ((temp_s0_3 < func_8008667C(*var_s1)) == 0))) {
                var_s4 = *var_s1;
                var_s5 = temp_s3;
            } else if (var_s5 == temp_s3) {
                temp_s0_4 = func_8008667C(var_s4) & 0xFF;
                if (temp_s0_4 >= func_8008667C(*var_s1)) {
                    temp_a2 = *var_s1;
                    if ((D_800E6280.unk_1BC[var_s4].unk_06 + D_800E6280.unk_1BC[var_s4].unk_02) < (D_800E6280.unk_1BC[temp_a2].unk_06 + D_800E6280.unk_1BC[temp_a2].unk_02)) {
                        var_s5 = temp_s3;
                        var_s4 = temp_a2;
                    }
                }
            }
            var_s2_2 += 1;
            var_s1 += 4;
        } while (var_s2_2 < var_s6);
    }
    return var_s4;
}

/* FAKE: same-line swap and pointer read retain the original halfword load order. T-9550 */
s32 func_80086640(void)
{

  s16 *right;
  s16 old_right;
  s16 old_left;
  old_left = D_80120666; right = &D_801206EE; old_right = *right; D_80120666 = old_right; D_801206EE = old_left;
  D_80120668 = 0;
  D_801206F0 = 0;
  D_80120658 = 0;
}

s32 func_8008667C(s32 a) {
    /* FAKE: the (u8) cast reserves the extra temp the original has (same as GYOZI func_80140D7C); real return type unknown. T-4060 */
    return (u8)get_g_zyotai_s(a) & 0x7F;
}

void func_800866A0(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80062CD0();
        D_800E6280.unk_110D += 1;
        return;
    }
    if (cd_read_sync_status() & 1) {
        if (*(s32 *)D_800CA120 != 0x801820AC || *(s32 *)(D_800CA120 + 4) != 0x801860AC || *(s32 *)(D_800CA120 + 8) != 0x8018A0AC || *(s32 *)(D_800CA120 + 0xC) != 0x8018E0AC || *(s32 *)(D_800CA120 + 0x10) != 0x801920AC || *(s32 *)D_800CA128 != 0x8018005C || *(s32 *)(D_800CA128 + 4) != 0x80180070 || *(s32 *)D_800CA124 != 0x80180000) {
            D_800E6280.unk_110D = 0;
            return;
        }
        if (file_checksum_ok(0x2D, 0x80180000) == 0) {
            D_800E6280.unk_110D = 0;
            return;
        }
        func_8004284C();
    }
}

void func_800867CC(void) {
    if (D_80122EA0 == 0) {
        if (func_80044E8C() == 1) {
            D_80122EA0 = 1;
        }
    }
}
