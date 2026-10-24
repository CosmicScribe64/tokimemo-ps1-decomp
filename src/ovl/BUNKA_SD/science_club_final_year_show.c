/* science_club_final_year_show.c: src/ovl/BUNKA_SD/801359B0.c:7,16,47; resource/input/show flow; club/year dispatch is proved by src/ovl/BUNKA_SD/club_year_show_dispatch.c:14-51,63-89. Original object 0x801359B0-0x80136270 (config/objects/BUNKA_SD.txt). */
#define MAIN_API_OVERRIDE_D_80120656 /* 0x82 must not sign-extend (main_api.h: s8) */
#include "common.h"
#include "ovl/BUNKA_SD.h"

extern u8 D_80120656;

s32 func_801359B0(void) {
    if (game_state.substep == 0) {
        cd_read_sectors(0x6D, 0x80180000, 0x4F73);
        game_state.substep += 1;
    } else if ((game_state.substep == 1) && (cd_read_sync_status() & 1)) {
        mode_step_next();
    }
}

void func_80135A2C(void) {
    s32 i;

    if ((game_state.mode_step >= 3) && (game_state.mode_step < 7)) {
        if (game_state.pad_held & PAD_CIRCLE) {
            func_80135B44();
            D_80120656 = 0x82;
            D_8012069A = 0x82; for (i = 0; i < 2; i++) {
                D_8011ED58[(i + 1) * 0x110 + 0x18BA] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x18FE] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x1942] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x1876] = 0x82;
            }
        } else {
            D_80120656 = 1;
            D_8012069A = 1; for (i = 0; i < 2; i++) {
                D_8011ED58[(i + 1) * 0x110 + 0x18BA] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x18FE] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x1942] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x1876] = 1;
            }
        }
    }
    if ((game_state.mode_step >= 3) && (game_state.mode_step < 6) && (game_state.pad_held & PAD_CROSS)) {
        mode_step_set(5);
    }
    func_80135B44();
}

void func_80135B44(void) {
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    switch (game_state.mode_step) {
    case 0:
        func_80134260();
        return;
    case 1:
        func_801359B0();
        return;
    case 2:
        func_80135C3C();
        return;
    case 3:
        func_80134418();
        return;
    case 4:
        func_80135C70();
        return;
    case 5:
        audio_command(0xC1);
        k_reset(1);
        mode_step_next();
        return;
    case 6:
        func_801344A8();
        return;
    case 7:
        func_80134314();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_80135C3C(void) {
    audio_bank_start(1, 0x201);
    func_80135CD4();
    mode_step_next();
}

void func_80135C70(void) {
    u32 t;

    func_80135F38();
    func_801361B8();
    t = game_state.mode_step_timer.u + 1;
    game_state.mode_step_timer.u = t;
    if (t >= 0x21) {
        if (!(D_80120652 & 1)) {
            mode_step_next();
        }
    }
}

void func_80135CD4(void) {
    s32 i;

    for (i = 0; i < 0x10; i++) {
        load_palette(D_8013C73C, (i + 0x10) & 0xFF, 1, 1, i);
    }
    for (i = 0; i < 6; i++) {
        actor_clear(i + 0x60);
        ((Actor *)actor_pool)[96 + i].unk_01 = 4;
        ((Actor *)actor_pool)[96 + i].quad_flags = 0x01000000;
        ((Actor *)actor_pool)[96 + i].draw_flags = 0x80;
        ((Actor *)actor_pool)[96 + i].frame_table = D_8013C734;
        ((Actor *)actor_pool)[96 + i].sequence_table = D_8013C738;
        ((Actor *)actor_pool)[96 + i].tpage_table = (u8 *)D_8013C730;
        ((Actor *)actor_pool)[96 + i].sequence_slot = i + 3;
        ((Actor *)actor_pool)[96 + i].blink_period = 1;
        ((Actor *)actor_pool)[96 + i].brightness = 0;
        ((Actor *)actor_pool)[96 + i].x = -0xA0;
        ((Actor *)actor_pool)[96 + i].y = -0x78;
        ((Actor *)actor_pool)[96 + i].unk_05 = 0x80;
        ((Actor *)actor_pool)[96 + i].unk_43 = 0;
        ((Actor *)actor_pool)[96 + i].unk_04 = 0;
    }
    ((Actor *)actor_pool)[96].sequence_slot = D_80122EAC;
    ((Actor *)actor_pool)[99].draw_flags = 0;
    ((Actor *)actor_pool)[100].draw_flags = 0;
    ((Actor *)actor_pool)[101].flags = 0x40;
    ((Actor *)actor_pool)[101].unk_18 = 0x8B;
    load_tpage_buf_lock(5, D_8013C730[0], 1);
    load_tpage_buf_lock(6, D_8013C730[1], 1);
    load_tpage_buf_lock(7, D_8013C730[2], 1);
    load_tpage_buf_lock(8, D_8013C730[3], 1);
    load_tpage_buf_lock(9, D_8013C730[4], 1);
    load_tpage_buf_lock(0xA, D_8013C730[5], 1);
    load_tpage_buf_lock(0x25, D_8013C730[6], 1);
    load_tpage_buf_lock(0x26, D_8013C730[7], 1);
    load_tpage_buf_lock(0x27, D_8013C730[8], 1);
    load_tpage_buf_lock(0x28, D_8013C730[9], 1);
    load_tpage_buf_lock(0x29, D_8013C730[10], 1);
}

void func_80135F38(void) {
    switch ((u32)game_state.mode_step_timer.w) {
    case 0x92:
        audio_command(0x503);
        break;
    case 0x175:
        audio_command(0x503);
        break;
    case 0x205:
        audio_command(0x503);
        break;
    case 0x293:
        audio_command(0x503);
        break;
    case 0x45F:
        audio_command(0x503);
        break;
    case 0x4C3:
        audio_command(0x503);
        break;
    case 0x529:
        audio_command(0x503);
        break;
    case 0x269:
        audio_command(0x507);
        break;
    case 0x3C6:
        audio_command(0x50B);
        break;
    case 0x3C7:
        audio_command(0x506);
        break;
    case 0x3E6:
        audio_command(0x506);
        break;
    case 0x406:
        audio_command(0x506);
        break;
    case 0x426:
        audio_command(0x506);
        break;
    case 0x2DA:
        audio_command(0x504);
        break;
    case 0x31A:
        audio_command(0x504);
        break;
    case 0x35A:
        audio_command(0x504);
        break;
    }
    /* FAKE: (&D_80122EAC)[0] is a non-plain operand, so IDO keeps the constant first in the
     * compares (beq a0,v1); the original probably reads a field of a main-exe aggregate around
     * D_80122EA8 that is not declared yet. T-18100 */
    if ((&D_80122EAC)[0] != 3 && game_state.mode_step_timer.w == 0x3A7) {
        audio_command(0x505);
    } else if ((&D_80122EAC)[0] == 3 && game_state.mode_step_timer.w == 0x39D) {
        audio_command(0x505);
    }
}

void func_801361B8(void) {
    if (game_state.mode_step_timer.u == 0) {
        D_80120652 = 5;
        D_80120696 = 5;
        D_801206DA = 5;
    }
    if (game_state.mode_step_timer.u == 0x3A6) {
        D_8012071F = 0x80;
        D_8012071E = 5;
        D_80120723 = 0x80;
        D_80120763 = 0x80;
        D_80120762 = 5;
        D_80120767 = 0x80;
    }
    if (!(D_8012071E & 1)) {
        D_8012071F = 0;
        D_80120763 = 0;
    }
}
