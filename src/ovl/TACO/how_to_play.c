/* how_to_play.c: src/ovl/TACO/80137A60.c:5 dispatches instruction sequence reached from main-menu row 3 at src/ovl/TACO/80134930.c:343; :45 loads instruction imagery; :69 plays indexed voice ranges; :79 and :118 wait for voice and scroll presentation. Original object 0x80137A60-0x80137F90 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

/* No return value on purpose: implicit-int function, see func_80137BD8. T-6070 */
s32 func_80137A60(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80137BD8();
        break;
    case 1:
        func_80137D00();
        break;
    case 2:
        func_80137E48();
        break;
    case 3:
        func_80137F54();
        break;
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        audio_command(0x74);
        audio_command(0xB1);
        mode_stage_set(2);
    }
}

void func_80137B14(s32 arg0) {
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
    D_8011ED2C = arg0;
    D_8011ED3A = -0xAA;
    D_8011ED3E = 0x78;
}

/* No return value on purpose: the original is an implicit-int function (the `or v0,v1,zero` in the first delay slot). T-6070 */
s32 func_80137BD8(void) {
    switch (game_state.substep) {
    case 0:
        func_800450F4(0, 0x201);
        func_80059048();
        dec_bg_reset();
        cd_read_sectors(9U, 0x80162000, 0xB380);
        k_reset(0);
        D_8015EDEC = 0;
        actor_draw_enable(1);
        draw2d3d(1, 0);
        func_80137B14(1);
        game_state.substep += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x2C0, 0, 0x140, 0xF0);
            func_80135944();
            mode_step_next();
        }
        break;
    }
}

void func_80137CBC(s32 arg0) {
    s32 *p;
    s32 i;

    i = arg0; /* FAKE: copy of the argument reproduces the original's `move t6,a0`; real source unknown. T-6070 */
    p = &D_8015F440[i * 2];
    cd_audio_request_set(p[0], p[1], 8);
    audio_command(0x300);
}

void func_80137D00(void) {
    u16 *p;

    switch (game_state.substep) {
    case 0:
        if (D_8015EDEC < 3) {
            func_80137CBC(D_8015EDEC);
            D_8015EDEC += 1;
            game_state.substep += 1;
        } else {
            mode_step_next();
            /* FAKE: absolute-address load; uopt then colours the pointer $v1 with `lui t9; lw v1,lo(t9)` as the original does (reading D_801FAA84 gives `lw t9,0(t9)`); real source unknown. T-19002 */
            p = *(u16 **)0x801FAA84;
            p[5] = 0x28;
            p[11] = 0xFF00;
            p[18] = 0xFF00;
            p[25] = 0xFF00;
            p[32] = 0xFF00;
            func_80137B14(2);
            D_8011ED3E -= 0x30;
        }
        break;
    case 1:
        if (cd_audio_state() & 4) {
            game_state.substep += 1;
        }
        break;
    case 2:
        if (cd_audio_state() & 2) {
            game_state.substep = 0;
        }
        break;
    }
    dec_bg_draw(2);
    if (game_state.frame_count & 1) {
        D_8011ED3E -= 1;
    }
}

void func_80137E48(void) {
    switch (game_state.substep) {
    case 0:
        if (D_8015EDEC < 8) {
            func_80137CBC(D_8015EDEC);
            D_8015EDEC += 1;
            game_state.substep += 1;
        } else {
            mode_step_next();
        }
        break;
    case 1:
        if (cd_audio_state() & 4) {
            game_state.substep += 1;
        }
        break;
    case 2:
        if (cd_audio_state() & 2) {
            game_state.substep = 0;
        }
        break;
    }
    dec_bg_draw(2);
    if ((game_state.frame_count & 3) == 3) {
        D_8011ED3E -= 1;
    }
}
void func_80137F54(void) {
    func_80137CBC(8);
    audio_command(0xB1);
    mode_stage_set(2);
}
