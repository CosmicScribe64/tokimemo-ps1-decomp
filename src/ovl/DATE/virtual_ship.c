/* virtual_ship.c: src/ovl/DATE/8014A1B0.c:24: virtual ship ride (3D, GTE). Original object 0x8014A1B0-0x8014B1F0 (config/objects/DATE.txt). */
#define MAIN_API_OVERRIDE_D_80122760 /* three-light block (3 x 16 bytes) sharing base symbol for as1 load hoisting (T-24107, T-24108) */
#include "common.h"
#include "libgte.h"
#include "ovl/DATE.h"

typedef struct {
    s32 unk00[3];
    u8 r0, g0, b0, pad0;
    s32 unk10[3];
    u8 r1, g1, b1, pad1;
    s32 unk20[3];
    u8 r2, g2, b2, pad2;
} DateLightBlock;

extern DateLightBlock D_80122760;
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
s16 D_8015DDD0[0x2C] = { 0 };
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/virtual_ship.data", D_8015DE28);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/virtual_ship.data", D_8015DE2C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/virtual_ship.data", D_8015DE30);
s32 D_8015DE34[3] = { 0, 0, 0 };

void date_virtual_ship_sections_init(void) {
    D_8015DE28 = 0x80195000;
    D_8015DE2C = 0x80195060;
    D_8015DE30 = 0x801950C0;
}

void date_virtual_ship_dispatch(void) {
    /* The dispatcher reads the selector as an unsigned halfword. */
    switch (*(u16 *)&date_virtual_ship_phase) {
    case 0:
        date_virtual_ship_phase_load();
        return;
    case 1:
        date_virtual_ship_phase_run();
        return;
    case 2:
        date_virtual_ship_read_portrait();
        return;
    case 3:
        date_virtual_ship_wait_read();
        return;
    case 4:
        date_virtual_ship_build_portrait();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void date_virtual_ship_phase_load(void) {
    if (date_virtual_ship_subphase == 0) {
        date_virtual_ship_load();
    }
}

void date_virtual_ship_phase_run(void) {
    switch (date_virtual_ship_subphase) {
    case 0:
        date_virtual_ship_setup();
        return;
    case 1:
        date_virtual_ship_run();
        return;
    case 2:
        date_virtual_ship_finish();
        return;
    }
}

void date_virtual_ship_read_portrait(void) {
    audio_command(0x200);
    read_bustup();
    game_state.mode_step -= 1;
    date_virtual_ship_phase += 1;
}

void date_virtual_ship_wait_read(void) {
    if (cd_read_sync_status() & 1) {
        date_virtual_ship_phase += 1;
    }
}

s32 date_virtual_ship_build_portrait(void) {
    s32 var_v1;
    u8 r;

    if (*(s32 *)D_800CA120 != 0x801820AC || *(s32 *)(D_800CA120 + 4) != 0x801860AC || *(s32 *)(D_800CA120 + 8) != 0x8018A0AC
        || *(s32 *)(D_800CA120 + 0xC) != 0x8018E0AC || *(s32 *)(D_800CA120 + 0x10) != 0x801920AC || *(s32 *)D_800CA128 != 0x8018005C
        || *(s32 *)(D_800CA128 + 4) != 0x80180070 || *(s32 *)D_800CA124 != 0x80180000) {
        date_virtual_ship_phase -= 2;
        return 0;
    }
    r = get_g_zyotai_h(game_state.current_girl);
    var_v1 = r & 0x7F;
    if (game_state.current_girl >= GIRL_REI) {
        var_v1 = 0;
    }
    portrait_build(D_800CA130, D_800CA120, D_800CA124, D_800CA128, var_v1);
    hizuke_disp_switch(1);
    mode_step_next();
}

void date_virtual_ship_load(void) {
    u8 *var_s1;
    s32 var_s0;

    switch (*(u16 *)&date_virtual_ship_step) {
    case 0:
        date_virtual_ship_sections_init();
        D_8015DE34[0] = D_8015DE28;
        D_8015DE34[1] = D_8015DE2C;
        D_8015DE34[2] = D_8015DE30;
        cd_read_sectors_nowait(1U, 0x80195000, 0xBD9E);
        date_virtual_ship_step += 1;
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            date_virtual_ship_step += 1;
            return;
        }
    default:
        return;
    case 2:
        audio_command(0xCF);
        audio_command(0x202);
        tpage_buf_clear();
        back_clear_switch(1);
        actor_draw_enable(1);
        game_state.clear_color_g = 0;
        game_state.clear_color_r = 0;
        game_state.clear_color_b = 0;
        hizuke_disp_switch(0);
        cd_read_sectors_nowait(2U, 0x801F8000, 0xB471);
        date_virtual_ship_step += 1;
        return;
    case 3:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors_nowait(1U, 0x801F9000, 0xB470);
            date_virtual_ship_step += 1;
            return;
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            draw2d3d(1, 1);
            initView();
            date_virtual_ship_matrix_helper();
            initCoordinate();
            initModelingData_init((s32 *)0x801F8000);
            date_virtual_ship_vector_helper();
            cd_read_sectors_nowait(0x21U, 0x80180000, 0xB57F);
            date_virtual_ship_step += 1;
            return;
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors_nowait(1U, 0x80190800, 0xB456);
            date_virtual_ship_step += 1;
            return;
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            load_palette(0x80190800, 0x10, 1, 2, 0);
            var_s0 = 0;
            var_s1 = (u8 *)0x80180000;
            do {
                load_tpage_buf_lock(var_s0 + 5, var_s1, 1);
                var_s0 += 1;
                var_s1 += 0x4000;
            } while (var_s0 < 2);
            var_s1 = (u8 *)0x80188000;
            var_s0 = 0;
            do {
                load_tpage_buf_lock(var_s0 + 0x25, var_s1, 1);
                var_s0 += 1;
                var_s1 += 0x4000;
            } while (var_s0 != 2);
            cd_read_sectors_nowait(9U, 0x80180000, 0xB467);
            date_virtual_ship_step += 1;
            return;
        }
        break;
    case 7:
        if (cd_read_sync_status() & 1) {
            load_palette(0x80180000, 0x11, 1, 2, 0);
            load_csr_tp(0x80180200, 7, 1, 0, 0, 0x80, 0x80);
            date_virtual_ship_next_phase();
        }
        break;
    }
}

void date_virtual_ship_setup(void) {
    s32 i;
    s32 j;

    date_virtual_ship_draw_object(0, D_8015DE34[0], 0);
    D_80122640[0] = 0;
    D_8015DDD0[0] = 0x32;
    D_8015DDD0[2] = 0x32;
    D_8015DDD0[1] = 0xD2;
    date_virtual_ship_draw_object(1, D_8015DE38, 0);
    D_80122640[4] = 0;
    D_8015DDD0[4] = 0x4B;
    D_8015DDD0[6] = -0x32;
    D_8015DDD0[5] = 0xC3;
    for (i = 2, j = 0; i != 0xB; i++) {
        date_virtual_ship_draw_object(i, D_8015DE3C, j);
        j++;
        D_80122640[i * 4] = 0;
    }
    D_8015DE4C = 0;
    date_virtual_ship_next_subphase();
}

/* FAKE: s32 without a return keeps the s16 counter in $v0 at the exit (sll/sra before the sh), the real return type is unknown. T-17208 */
s32 date_virtual_ship_run(void) {
    s32 frame;
    s32 i;
    s32 j;

    message_window_show();
    hud_screen_frame_draw(2);
    message_window_title_update();
    frame = D_8015DE4C;
    if (frame < 0x20) {
        if (D_80122760.r0 < 0xC0U) {
            D_80122760.r0 = D_80122760.g0 = (D_80122760.b0 += 6);
            D_80122760.r1 = D_80122760.g1 = (D_80122760.b1 += 1);
            /* FAKE: the two equal masked sums make the third group a uopt range ($v0, the bytes get $v1/$a2/$a3) and keep its ugen slot, as in the original (T-22006, T-24107, T-24108). */
            D_80122760.g2 = (D_80122760.b2 + 1) & 0xFF;
            D_80122760.r2 = D_80122760.b2 = (D_80122760.b2 + 1) & 0xFF;
            func_8009AD70(0, (s32 *)&D_80122760);
            func_8009AD70(1, &D_80122770);
            func_8009AD70(2, &D_80122780);
        }
        if (frame < 0xF) {
            game_state.clear_color_b += 1;
        }
    } else if (frame >= 0x20 && frame < 0x54) {
        for (i = 0; i != 2; i++) {
            date_virtual_ship_draw_object(i, D_8015DE34[i], 1);
        }
    } else if (frame >= 0x54 && frame < 0x68) {
        for (i = 0; i != 2; i++) {
            date_virtual_ship_draw_object(i, D_8015DE34[i], 2);
        }
    } else if (frame >= 0x68 && frame < 0x72) {
        for (i = 0; i != 2; i++) {
            date_virtual_ship_draw_object(i, D_8015DE34[i], 3);
        }
    } else if (frame >= 0x72 && frame < 0x7C) {
        for (i = 0; i != 2; i++) {
            date_virtual_ship_draw_object(i, D_8015DE34[i], 4);
        }
    } else if (frame >= 0x7C && frame < 0x9A) {
        for (i = 0; i != 2; i++) {
            date_virtual_ship_draw_object(i, D_8015DE34[i], 5);
        }
    } else if (frame >= 0x9A && frame < 0xAE) {
        for (i = 0; i != 2; i++) {
            date_virtual_ship_draw_object(i, D_8015DE34[i], 6);
        }
    } else if (frame >= 0xAE && frame < 0xEA) {
        for (i = 0; i != 2; i++) {
            date_virtual_ship_draw_object(i, D_8015DE34[i], 7);
        }
    } else if (frame >= 0x10E && frame < 0x12E) {
        if (D_80122760.r0 != 0) {
            D_80122760.r0 = D_80122760.g0 = (D_80122760.b0 -= 6);
            D_80122760.r1 = D_80122760.g1 = (D_80122760.b1 -= 1);
            /* FAKE: the two equal masked sums make the third group a uopt range ($v0, the bytes get $v1/$a2/$a3) and keep its ugen slot, as in the original (T-22006, T-24107, T-24108). */
            D_80122760.g2 = (D_80122760.b2 - 1) & 0xFF;
            D_80122760.r2 = D_80122760.b2 = (D_80122760.b2 - 1) & 0xFF;
            func_8009AD70(0, (s32 *)&D_80122760);
            func_8009AD70(1, &D_80122770);
            func_8009AD70(2, &D_80122780);
        }
        if (frame < 0x11D) {
            game_state.clear_color_b -= 1;
        }
        /* FAKE: the byte view makes this second read of D_8012276C a reload; with the plain global uopt promotes it into $v0 and the first test is no longer a $t2 load. T-17208 */
        if (*(u8 *)&D_8012276C == 0) {
            /* The nine ordering-table heads are addressed from D_80122660 (stride 0x10), not from D_80122640. */
            for (i = 0; i < 9; i++) {
                ((u32 *)D_80122660)[i * 4] = 0x80000000;
            }
        }
    } else if (frame >= 0x138) {
        date_virtual_ship_next_subphase();
    }
    for (i = 2, j = 9; i != 0xB; i++, j++) {
        date_virtual_ship_draw_object(i, D_8015DE3C, j);
    }
    D_8015DE4C = D_8015DE4C + 1;
}

void date_virtual_ship_finish(void) {
    draw2d3d(1, 0);
    date_virtual_ship_next_phase();
}

void date_virtual_ship_draw_object(s32 arg0, s16 *arg1, s32 arg2) {
    MATRIX matrix;
    SVECTOR *rotation;

    arg1 += arg2 * 6;
    matrix = *(MATRIX *)D_80125C60;
    matrix.t[0] = ((MATRIX *)D_801227A0[arg0].unk4)->t[0] + arg1[0];
    matrix.t[1] = ((MATRIX *)D_801227A0[arg0].unk4)->t[1] + arg1[1];
    matrix.t[2] = ((MATRIX *)D_801227A0[arg0].unk4)->t[2] + arg1[2];
    rotation = &((SVECTOR *)D_8015DDD0)[arg0];
    rotation->vx += arg1[3];
    arg1 += 4;
    func_8015A640((rotation->vx << 12) / 360, &matrix);
    rotation->vz += *arg1;
    arg1++;
    func_8015A7E0((rotation->vz << 12) / 360, &matrix);
    rotation->vy += *arg1;
    func_8015A980((rotation->vy << 12) / 360, &matrix);
    *(MATRIX *)D_801227A0[arg0].unk4 = matrix;
    D_801227A0[arg0].unk0 = 0;
}

/* The display-list argument is the pointer variable dop (base plus steps), as in TACO func_801347F4;
 * with immediates the hoisted constants take other saved registers. T-17208 */
void date_virtual_ship_vector_helper(void) {
    u32 *dop;
    s32 i;

    dop = (u32 *)0x801F8000;
    dop++;
    func_8009B3C0((s32)dop);
    dop += 2;
    for (i = 0; i < 2; i++) {
        func_8009B430((s32)dop, (u8 *)&D_80122640[i * 4], i);
        D_80122640[i * 4 + 1] = (u32)&D_801227A0[i];
        D_80122640[i * 4] = 0x80000000;
    }
    dop = (u32 *)0x801F9000;
    dop++;
    func_8009B3C0((s32)dop);
    dop += 2;
    for (i = 0; i < 9; i++) {
        func_8009B430((s32)dop, D_80122660 + i * 0x10, 0);
        D_80122640[i * 4 + 9] = (u32)&D_80122840[i];
        D_80122640[i * 4 + 8] = 0x80000000;
    }
}

void date_virtual_ship_matrix_helper(void) {

    D_80122760.unk00[0] = 0x64;
    D_80122764 = 0x1E;
    D_80122768 = -0x32;
    D_8012276C = 0;
    D_8012276D = 0;
    D_8012276E = 0;
    func_8009AD70(0, (s32 *)&D_80122760);
    D_80122770 = 0x14;
    D_80122774 = 0xA;
    D_80122778 = -0x64;
    D_8012277C = 0;
    D_8012277D = 0;
    D_8012277E = 0;
    func_8009AD70(1, &D_80122770);
    D_80122780 = -0x14;
    D_80122784 = -0x14;
    D_80122788 = -0x64;
    D_8012278C = 0;
    D_8012278D = 0;
    D_8012278E = 0;
    func_8009AD70(2, &D_80122780);
    GsSetAmbient(0, 0, 0);
    func_8009B340(0);
}

void date_virtual_ship_next_phase(void) {
    date_virtual_ship_phase += 1;
    date_virtual_ship_subphase = 0;
    D_8015DE4C = 0;
    date_virtual_ship_step = 0;
}

void date_virtual_ship_next_subphase(void) {
    date_virtual_ship_subphase += 1;
    D_8015DE4C = 0;
    date_virtual_ship_step = 0;
}
