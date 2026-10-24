#define MAIN_API_OVERRIDE_D_80122760 /* three-light block (3 x 16 bytes) sharing base symbol for as1 load hoisting (T-24107) */
#include "common.h"
#include "ovl/TACO.h"

typedef struct {
    s32 unk00[3];
    u8 r0, g0, b0, pad0;
    s32 unk10[3];
    u8 r1, g1, b1, pad1;
    s32 unk20[3];
    u8 r2, g2, b2, pad2;
} TacoLightBlock;

extern TacoLightBlock D_80122760;

void func_80134930(void) {
    game_state.clear_color_g = 0;
    game_state.clear_color_r = 0;
    game_state.clear_color_b = 0;
    draw2d3d(1, 1);
    initView();
    func_80146D60(-0xFA0);
    initModelingData_init(0x8019C800);
    func_801471D0(0x8019C800);
    func_80147488(1, D_8015F3E0[0], 0);
    D_80127090.unk0 = 0;
    D_80128880[1].x = 0x32;
    D_80128880[1].z = 0x32;
    D_80128880[1].y = 0xD2;
    func_80147488(2, D_8015F3E0[1], 0);
    D_801270A0.unk0 = 0;
    D_80128880[2].x = 0x4B;
    D_80128880[2].z = -0x32;
    D_80128880[2].y = 0xC3;
    D_8015F3E8 = 0;
}

s32 func_80134A18(void) {
    s32 i;
    s32 frame;

    frame = D_8015F3E8;
    if (frame < 0x20) {
        if (D_80122760.r0 < 0xC0) {
            /* Chained stores: IDO stores the middle and left targets first and the read one last, the original order. */
            D_80122760.r0 = D_80122760.g0 = D_80122760.b0 = D_80122760.b0 + 6;
            D_80122760.r1 = D_80122760.g1 = D_80122760.b1 = D_80122760.b1 + 1;
            /* FAKE: repeated masked sum makes a uopt CSE range in $v0 while keeping the ugen temporary slot (T-22006, T-24107). */
            D_80122760.g2 = (D_80122760.b2 + 1) & 0xFF;
            D_80122760.r2 = D_80122760.b2 = (D_80122760.b2 + 1) & 0xFF;
            func_8009AD70(0, (s32) &D_80122760);
            func_8009AD70(1, (s32) &D_80122770);
            func_8009AD70(2, (s32) &D_80122780);
        }
        if (frame < 0xF) {
            game_state.clear_color_b += 1;
        }
    } else if (frame >= 0x20 && frame < 0x54) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 1);
        }
    } else if (frame >= 0x54 && frame < 0x68) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 2);
        }
    } else if (frame >= 0x68 && frame < 0x72) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 3);
        }
    } else if (frame >= 0x72 && frame < 0x7C) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 4);
        }
    } else if (frame >= 0x7C && frame < 0x9A) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 5);
        }
    } else if (frame >= 0x9A && frame < 0xAE) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 6);
        }
    } else if (frame >= 0xAE && frame < 0xEA) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 7);
        }
    } else if (frame >= 0xF0) {
        return 1;
    }
    D_8015F3E8 = D_8015F3E8 + 1;
    return 0;
}


void func_80134D8C(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        func_80134EBC();
        break;
    case 1:
        func_80135220();
        break;
    case 2:
        func_80135278();
        break;
    case 3:
        func_801352CC();
        break;
    case 4:
        func_80135330();
        break;
    case 5:
        func_80135350();
        break;
    }
    func_80134A18();
    func_8013506C();
    if (game_state.mode_stage_timer == 0xA0) {
        func_801335A0(0, 0x504);
    }
    if (game_state.mode_stage_timer == 0x40) {
        func_801335A0(0, 0x505);
    }
    if (game_state.mode_stage_timer == 0x80) {
        func_801335A0(0, 0x506);
    }
    if (game_state.mode_stage_timer == 0xC0) {
        func_801335A0(0, 0x507);
    }
}

void func_80134EBC(void) {
    s32 i;

    D_8015EDB0 = 0;
    game_state.clear_color_g = 0;
    game_state.clear_color_r = 0;
    D_8015EDE8 = 0;
    game_state.clear_color_b = 0;
    func_80059048();
    draw2d3d(1, 1);
    actor_draw_enable(1);
    k_reset(0);
    StoreSquare(0, 0x1EF, 0x100, 1, &game_state.scratch_words[0]);
    for (i = 56; i < 72; i++) {
        game_state.scratch_words[i] = 0;
    }
    for (i = 0; i < 0x80; i++) {
        game_state.scratch_words[128 + i] = 0;
    }
    load_palette(&game_state.scratch_words[0], 0x1F, 1, 1, 0);
    func_80134FB8();
    func_80134930();
    mode_step_next();
}

void func_80134FB8(void) {
    actor_clear(1);
    D_8011ED15 = 8;
    D_8011ED4C = 0;
    D_8011ED16 = 0x40;
    D_8011ED17 = 0x84;
    D_8011ED20 = game_state.ovl_frame_table;
    D_8011ED24 = game_state.ovl_sequence_table;
    D_8011ED48 = game_state.ovl_tpage_table;
    D_8011ED18 = 8;
    D_8011ED19 = 0x81;
    D_8011ED57 = 0xF;
    D_8011ED3A = -0xA0;
    D_8011ED3E = -0x64;
}

void func_8013506C(void) {
    /* FAKE: unused 4-byte local reserves the top frame word so the $a3 spill sits at 0x3C($sp) in the 0x48 frame (T-24120). */
    s32 pad[1];
    s32 i;
    u32 x;
    u32 y;

    for (i = 0; i < 0x80; i++) {
        if (game_state.scratch_words[128 + i] == 0) {
            x = rand() % 320;
            y = rand() % 240;
            game_state.scratch_words[128 + i] = (x << 16) + y;
            break;
        }
    }
    for (i = 0; i < 0x80; i++) {
        if (game_state.scratch_words[128 + i] != 0) {
            x = (u32)game_state.scratch_words[128 + i] >> 16;
            y = game_state.scratch_words[128 + i] & 0xFFFF;
            func_80143574(x - 0xA0, y - 0x78, (x * y + game_state.frame_count) & 0xFFFFFF);
        }
    }
}


s32 func_8013515C(void) {
    s32 i;
    s32 n;

    n = 0;
    for (i = 56; i < 72; i++) {
        if (game_state.scratch_words[i] == 0) {
            if ((rand() & 0xFF) < 0x7F && n == 0) {
                game_state.scratch_words[i] = ((s32 *)game_state.ovl_palette_a)[i];
            } else {
                n++;
            }
        }
    }
    load_palette(&game_state.scratch_words[0], 0x1F, 1, 1, 0);
    return n;
}

void func_80135220(void) {
    if ((u32)game_state.mode_step_timer.w >= 0xD1U) {
        mode_step_next();
    } else if (game_state.pad_pressed & 0x860) {
        mode_step_set(5);
    }
}

void func_80135278(void) {
    if (func_8013515C() == 0) {
        mode_step_next();
    } else if (game_state.pad_pressed & 0x860) {
        mode_step_set(5);
    }
}

void func_801352CC(void) {
    if ((u32)game_state.mode_step_timer.w < 0xFFU) {
        if (game_state.pad_pressed != 0) {
            mode_step_set(5);
            return;
        }
        func_8004AC18(0xFF - game_state.mode_step_timer.w);
        return;
    }
    mode_step_next();
}

void func_80135330(void) {
    mode_step_next();
}

void func_80135350(void) {
    func_80059048();
    load_palette(game_state.ovl_palette_a, 0x1F, 1, 1, 0);
    mode_stage_set(3);
}

void func_80135394(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        func_80135418();
        break;
    case 1:
        func_8013546C();
        break;
    case 2:
        func_80135638();
        break;
    }
    func_8013506C();
}
void func_80135418(void) {
    func_8009C210(0);
    func_80059048();
    func_800591D8(0);
    func_80134FB8();
    D_8011ED1B = 0x40;
    D_8011ED3E = -0x78;
    mode_step_next();
}

void func_8013546C(void) {
    if (game_state.pad_pressed & 0x5100) {
        if (game_state.pad_pressed & PAD_UP) {
            D_8015EDE8 -= 1;
        }
        if (game_state.pad_pressed & PAD_DOWN) {
            D_8015EDE8 += 1;
        }
        if (game_state.pad_pressed & PAD_SELECT) {
            D_8015EDE8 += 1;
        }
        if (D_8015EDE8 < 0) {
            D_8015EDE8 = 4;
        }
        if (D_8015EDE8 >= 5) {
            D_8015EDE8 = 0;
        }
        func_801335A0(0, 0x500);
    }
    if (game_state.pad_pressed & 0x820) {
        func_801335A0(0, 0x501);
        mode_step_next();
    }
    func_8013555C();
    if ((u32) game_state.mode_stage_timer >= 0x385U) {
        mode_stage_set(6);
    }
}

/* The menu index D_8015EDE8 is an s8 elsewhere (compared < 0); these switches read it as the unsigned byte the original uses. T-16560 */
s32 func_8013555C(void) {
    switch (*(u8 *)&D_8015EDE8) {
    case 0:
        func_801432F0("GAME START", -0x40, 0x48, 2);
        break;
    case 1:
        func_801432F0("  OPTION  ", -0x40, 0x48, 3);
        break;
    case 2:
        func_801432F0("  SCORES  ", -0x40, 0x48, 5);
        break;
    case 3:
        func_801432F0("OPERATIONS", -0x40, 0x48, 6);
        break;
    case 4:
        func_801432F0("   EXIT   ", -0x40, 0x48, 0);
        break;
    }
}

/* The switch reads D_8015EDE8 as an unsigned byte, as func_8013555C does. T-16560 */
s32 func_80135638(void) {
    if ((u32) game_state.mode_step_timer.u >= 0x41U) {
        switch (*(u8 *)&D_8015EDE8) {
        case 0:
            mode_stage_set(0xA);
            break;
        case 1:
            mode_stage_set(4);
            break;
        case 2:
            mode_stage_set(5);
            break;
        case 3:
            mode_stage_set(7);
            break;
        case 4:
            mode_stage_set(0x2E);
            break;
        }
    }
    if (game_state.mode_stage != 3) {
        actor_draw_enable(0);
    }
    if (game_state.mode_step_timer.u & 4) {
        func_8013555C();
    }
}
