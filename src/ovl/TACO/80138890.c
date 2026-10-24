#include "common.h"
#include "ovl/TACO.h"

s32 func_80138890(void) {
    game_state.mode_stage_timer += 1;
    switch (game_state.mode_step) {
    case 0:
        func_80138B2C();
        break;
    case 1:
        func_80138D08();
        break;
    case 2:
        func_80139118();
        break;
    }
    if (game_state.mode_step < 2) {
        /* u32 view of unk_1100: the compare and the subtraction then share one load, as in the original */
        if (*(u32 *)&game_state.mode_stage_timer < 0x32U) {
            return func_8013B460(0x64 - *(u32 *)&game_state.mode_stage_timer);
        }
        return func_8013B460(0x32);
    }
}

/* No value is returned from the reset arm on purpose: v0 then holds the target (promoted D_8015F4A4) as in the original; the other arm returns D_8015F4A8. T-16564 */
s32 func_80138964(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg3 == 0) {
        D_8015F4A4 = arg0;
        D_8015F4A8 = 0;
    } else {
        if ((D_8015F4A8 < D_8015F4A4) && !(game_state.frame_count & 1)) {
            if (D_8015F4A8 < (D_8015F4A4 % 10)) {
                D_8015F4A8 += 1;
            } else if (D_8015F4A8 < (D_8015F4A4 % 100)) {
                D_8015F4A8 += 10;
            } else if (D_8015F4A8 < (D_8015F4A4 % 1000)) {
                D_8015F4A8 += 100;
            } else if (D_8015F4A8 < (D_8015F4A4 % 10000)) {
                D_8015F4A8 += 1000;
            } else if (D_8015F4A8 < (D_8015F4A4 % 100000)) {
                D_8015F4A8 += 10000;
            } else if (D_8015F4A8 < (D_8015F4A4 % 1000000)) {
                D_8015F4A8 += 100000;
            } else if (D_8015F4A8 < (D_8015F4A4 % 10000000)) {
                D_8015F4A8 += 1000000;
            }
        }
        if (D_8015F4A4 < D_8015F4A8) {
            D_8015F4A8 = D_8015F4A4;
        }
        func_801430D0(D_8015F4A8, arg1, arg2, 0);
        return D_8015F4A8;
    }
}

/* FAKE: T-25152, the unused K&R parameter holds the stage and the total: a parameter live in the entry block cannot take $v0/$v1 (uopt cupcosts), so it gets $a0 as in the original. The original has no such parameter; real source unknown. */
s32 func_80138B2C(arg0)
s32 arg0;
{
    s32 var_a1;
    s32 var_v0;
    s32 var_v1;
    u8 temp_v0;

    game_state.clear_color_g = 0;
    game_state.clear_color_r = 0;
    /* FAKE: T-25152, byte view of the stage: the indirect load stops copy propagation, so arg0 stays a variable instead of the promoted global */
    arg0 = *(u8 *)&D_8015EDCC;
    D_8015F490[0] = D_8015EDF0 += arg0 * 0xF4240;
    game_state.clear_color_b = 0;
    var_a1 = 0;
    if (arg0 >= 3U) {
        temp_v0 = D_8015EDB4->unk84[1];
        if (temp_v0 == 0xFF) {
            var_v1 = 0x7A120;
        } else {
            var_v1 = temp_v0 * 0x3E8;
        }
        var_a1 = D_8015EDEC * 0x64;
    } else {
        var_v1 = D_8015EDB4->unk84[1] * 0x64;
    }
    if (D_8015EDB4->unk80 != 0) {
        var_v0 = 0x7A120;
    } else if (D_8015EDB4->unk7E != 0) {
        var_v0 = 0x3D090;
    } else if (D_8015EDB4->unk7C != 0) {
        var_v0 = 0x186A0;
    } else {
        var_v0 = 0;
    }
    /* FAKE: T-25152, word view of the score for the same reason: the total stays in arg0 ($a0) and is stored in place, before the time bonus */
    arg0 = *(s32 *)&D_8015EDF0 + var_v1 + var_v0 + var_a1;
    D_8015F490[2] = var_v0;
    D_8015F490[1] = var_v1;
    D_8015F490[4] = arg0;
    D_8015F490[3] = var_a1;
    if (arg0 >= 0x989680) {
        /* FAKE: T-25152, the zero term holds an indirect load, so uopt keeps this as a def of arg0 ($a0) and the store reads it; a plain constant is copied into the store and built in $t2. ugen folds the term away */
        arg0 = 0x98967F + *(u8 *)&D_8015EDCC * 0;
        D_8015F490[4] = arg0;
    }
    audio_command(0x24);
    func_80059048();
    func_80143AF4();
    mode_step_next();
}

/* No return value on purpose: implicit-int function (v0 of the last call falls through). T-16564 */
s32 func_80138D08(void) {
    switch (game_state.substep) {
    case 8:
    case 9:
    default:
        func_801432F0("TOTAL", -0xE4, 0x48, 1);
    case 6:
    case 7:
        func_801432F0("SHIELD BONUS", -0xE4, 0x18, 5);
    case 4:
    case 5:
        func_801432F0("BOMB   BONUS", -0xE4, -8, 2);
    case 2:
    case 3:
        func_801432F0("TIME   BONUS", -0xE4, -0x28, 3);
        break;
    case 0:
    case 1:
        break;
    }
    func_801432F0("SCORE", -0xE4, -0x48, 6);
    switch (game_state.substep) {
    case 10:
    default:
        func_801430D0(D_8015F490[4], -0x40, 0x58, 0);
    case 8:
    case 9:
        func_801430D0(D_8015F490[1], -0x40, 0x28, 0);
    case 6:
    case 7:
        func_801430D0(D_8015F490[2], -0x40, 8, 0);
    case 4:
    case 5:
        func_801430D0(D_8015F490[3], -0x40, -0x18, 0);
    case 2:
    case 3:
        func_801430D0(D_8015F490[0], -0x40, -0x38, 0);
        break;
    case 0:
    case 1:
        break;
    }
    /* FAKE: u8 view of the selector keeps it a plain ugen temporary for the third switch; with the field it is promoted to a register (the 0xA test and the stores below). T-16564 */
    switch (*(u8 *)&game_state.substep) {
    case 0:
        func_80138964(D_8015F490[0], -0x40, -0x38, 0);
        game_state.substep += 1;
        break;
    case 1:
        if (func_80138964(D_8015F490[0], -0x40, -0x38, 1) == D_8015F490[0]) {
            game_state.substep += 1;
        }
        break;
    case 2:
        func_80138964(D_8015F490[3], -0x40, -0x18, 0);
        game_state.substep += 1;
        break;
    case 3:
        if (func_80138964(D_8015F490[3], -0x40, -0x18, 1) == D_8015F490[3]) {
            game_state.substep += 1;
        }
        break;
    case 4:
        func_80138964(D_8015F490[2], -0x40, 8, 0);
        game_state.substep += 1;
        break;
    case 5:
        if (func_80138964(D_8015F490[2], -0x40, 8, 1) == D_8015F490[2]) {
            game_state.substep += 1;
        }
        break;
    case 6:
        func_80138964(D_8015F490[1], -0x40, 0x28, 0);
        game_state.substep += 1;
        break;
    case 7:
        if (func_80138964(D_8015F490[1], -0x40, 0x28, 1) == D_8015F490[1]) {
            game_state.substep += 1;
        }
        break;
    case 8:
        func_80138964(D_8015F490[4], -0x40, 0x58, 0);
        game_state.substep += 1;
        break;
    case 9:
        if (func_80138964(D_8015F490[4], -0x40, 0x58, 1) == D_8015F490[4]) {
            game_state.substep += 1;
        }
        break;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if ((u8) game_state.substep < 0xAU) {
            game_state.substep = 0xA;
        } else {
            D_8015EDF0 = D_8015F490[4];
            func_80059308();
            mode_step_next();
        }
    }
}

void func_80139118(void) {
    func_800591D8(0);
    if (D_8015EE00 < D_8015EDF0) {
        mode_stage_set(9);
        return;
    }
    mode_stage_set(5);
}
