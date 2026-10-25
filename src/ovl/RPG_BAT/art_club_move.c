/* art_club_move.c: src/ovl/RPG_BAT/80154430.c:8: club 4 special move; body 11: switch (rpg_player_step) {; 14: rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A); club-index dispatch src/ovl/RPG_BAT/8013CC90.c:96. Original object 0x80154430-0x80155A70 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_actor_set_position /* Matched implicit-int call view keeps counter webs out of v0 (T-24027). */
#include "common.h"
#include "ovl/RPG_BAT.h"
#include "psx_types.h"

s32 rpg_actor_set_position();

void rpg_club_move_art(void) {
    s16 *state;

    switch (rpg_player_step) {
    case 0:
        strcpy(rpg_text_player_0, "　　　　　　　色素分解　　　　　　　");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A);
        rpg_pick_target();
        rpg_set_target_position(rpg_enemy_slot);
        rpg_player_damage_roll();
        rpg_player_step_next();
        rpg_player_step_next();
        return;
    case 2:
        rpg_move_approach();
        return;
    case 3:
        rpg_actor_set_animation(0x34, 3, 5, 1);
        rpg_player_step_next();
        return;
    case 4:
        state = &D_80121438;
        switch (*state) {
        case 1:
            rpg_se_play_latched_6xx(0x509, 0, 2);
            break;
        case 3:
            rpg_se_play_latched_6xx(0x509, 0, 3);
            break;
        case 5:
            rpg_se_play_latched_6xx(0x509, 0, 4);
            break;
        case 7:
            rpg_se_play_latched_6xx(0x509, 0, 5);
            break;
        }
        rpg_player_wait_animation_step(0x34);
        return;
    case 5:
        rpg_actor_set_animation(0x34, 4, 1, 1);
        rpg_player_step_next();
        return;
    case 6:
        rpg_art_step_6();
        return;
    case 7:
        rpg_art_step_7();
        return;
    case 8:
        rpg_art_step_8();
        return;
    case 9:
        rpg_player_wait_step(0x14);
        return;
    case 10:
        rpg_move_return();
        return;
    case 11:
        rpg_player_action_end();

    default:
        return;
    }
}

void rpg_art_step_8(void) {
    s32 i;

    switch (rpg_player_channel.unk_00) {
    case 0:
        rpg_fade_set_level(255);
        rpg_player_substep_next();
        return;
    case 1:
        rpg_player_wait_substep(5);
        return;
    case 2:
        rpg_fade_set_level(128);
        for (i = 0; i < 6; i++) {
            ((Actor *)actor_pool)[i + 136].draw_flags ^= 128;
        }
        if ((rpg_enemy_action & 0x100000) && rpg_damage == 1000) {
            rpg_delinquent_gauges = 0;
        } else {
            ((Actor *)actor_pool)[rpg_move_target_actor + 96].draw_flags ^= 128;
            rpg_apply_damage_single();
        }
        rpg_player_step_next();
        return;
    }
}

/* Fall-through s32 preserves the existing caller ABI. T-24027. */
s32 rpg_art_step_7(void) {
    s32 i;
    s32 v;

    switch (rpg_player_channel.unk_00) {
    case 0:
        /* FAKE: scalar address view selects the original v1 web (T-17114, T-24027). */
        switch (*(s32 *)&rpg_enemy_slot) {
        default:
            rpg_move_target_actor = 0x3B;
            break;
        case 0:
            if (rpg_delinquent_charmed_0[0] == 1) {
                rpg_result_line_count = 1;
            }
            rpg_move_target_actor = 0x3B;
            break;
        case 1:
            if (rpg_delinquent_charmed_0[1] == 1) {
                rpg_result_line_count = 1;
            }
            rpg_move_target_actor = 0x3C;
            break;
        case 2:
            if (rpg_delinquent_charmed_0[2] == 1) {
                rpg_result_line_count = 1;
            }
            rpg_move_target_actor = 0x3D;
            break;
        }
        ((Actor *)actor_pool)[rpg_move_target_actor + 96].draw_flags ^= 0x80;
        /* FAKE: unsigned 1 stays separate from the hoisted signed constant (T-17106, T-24027). */
        load_palette(rpg_asset_palette_4, 0x18, 1U, 3, 0xA);
        load_palette(rpg_asset_palette_5, 0x19, 1, 3, 1);
        load_palette(rpg_asset_palette_5, 0x1A, 1, 3, 0);
        ((Actor *)actor_pool)[136].unk_43 = 0x18;
        ((Actor *)actor_pool)[139].unk_43 = 0x18;
        ((Actor *)actor_pool)[137].unk_43 = 0x19;
        ((Actor *)actor_pool)[140].unk_43 = 0x19;
        ((Actor *)actor_pool)[138].unk_43 = 0x1A;
        ((Actor *)actor_pool)[141].unk_43 = 0x1A;
        for (i = 0; i < 3; i++) {
            ((Actor *)actor_pool)[136 + i].quad_flags = 0x51000000;
            ((Actor *)actor_pool)[139 + i].quad_flags = 0x41000000;
            ((Actor *)actor_pool)[136 + i].brightness = 0x80;
            ((Actor *)actor_pool)[139 + i].brightness = 0xFF;
            rpg_actor_set_position(i + 0x28, rpg_target_x, rpg_target_y);
            rpg_actor_set_position(i + 0x2B, rpg_target_x, rpg_target_y);
            switch (rpg_enemy_action & 0xFFFF0000) {
            case 0x100000:
                rpg_actor_set_animation_alt(i + 0x28, 5, 1, 1);
                rpg_actor_set_animation_alt(i + 0x2B, 5, 1, 1);
                break;
            case 0x200000:
                rpg_actor_set_animation_alt(i + 0x28, 6, 1, 1);
                rpg_actor_set_animation_alt(i + 0x2B, 6, 1, 1);
                break;
            case 0x400000:
                rpg_actor_set_animation_alt(i + 0x28, 7, 1, 1);
                rpg_actor_set_animation_alt(i + 0x2B, 7, 1, 1);
                break;
            default:
            case 0x8000000:
                rpg_actor_set_animation_alt(i + 0x28, 8, 1, 1);
                rpg_actor_set_animation_alt(i + 0x2B, 8, 1, 1);
                break;
            }
        }
        if (rpg_result_line_count == 1) {
            ((Actor *)actor_pool)[136].draw_flags |= 1;
            for (i = 1; i < 6; i++) {
                ((Actor *)actor_pool)[i + 136].draw_flags |= 1;
            }
        }
        rpg_se_play_latched_6xx(0x50B, 1, 0);
        rpg_player_substep_next();
        break;
    case 1:
        v = rsin(rpg_player_timer_2 * 0x800 / 1000) * 8 >> 12;
        if (v >= 8) {
            v = 7;
        }
        rpg_actor_set_position(0x28, rpg_target_x - v, rpg_target_y);
        rpg_actor_set_position(0x2B, rpg_target_x - v, rpg_target_y);
        rpg_actor_set_position(0x2A, rpg_target_x + v, rpg_target_y);
        rpg_actor_set_position(0x2D, rpg_target_x + v, rpg_target_y);
        rpg_player_timer_2 += 0xA;
        if (rpg_player_timer_2 >= 0x1F5) {
            rpg_player_timer_2 += 0xA;
        }
        if (rpg_player_timer_2 >= 0x3E9) {
            rpg_se_play_latched_6xx(0x50B, 1, 0);
            rpg_player_substep_next();
        }
        break;
    case 2:
        v = rsin(rpg_player_timer_2 * 0x800 / 1000) * 16 >> 12;
        if (v >= 16) {
            v = 15;
        }
        rpg_actor_set_position(0x28, rpg_target_x + v, rpg_target_y);
        rpg_actor_set_position(0x2B, rpg_target_x + v, rpg_target_y);
        rpg_actor_set_position(0x2A, rpg_target_x - v, rpg_target_y);
        rpg_actor_set_position(0x2D, rpg_target_x - v, rpg_target_y);
        rpg_player_timer_2 += 0xA;
        if (rpg_player_timer_2 >= 0x1F5) {
            rpg_player_timer_2 += 0xA;
        }
        if (rpg_player_timer_2 >= 0x3E9) {
            rpg_se_play_latched_6xx(0x50B, 1, 0);
            rpg_player_substep_next();
        }
        break;
    case 3:
        v = rsin(rpg_player_timer_2 * 0x800 / 1000) * 32 >> 12;
        if (v >= 32) {
            v = 31;
        }
        rpg_actor_set_position(0x28, rpg_target_x - v, rpg_target_y);
        rpg_actor_set_position(0x2B, rpg_target_x - v, rpg_target_y);
        rpg_actor_set_position(0x2A, rpg_target_x + v, rpg_target_y);
        rpg_actor_set_position(0x2D, rpg_target_x + v, rpg_target_y);
        rpg_player_timer_2 += 0xA;
        if (rpg_player_timer_2 >= 0x1F5) {
            rpg_player_timer_2 += 0xA;
        }
        if (rpg_player_timer_2 >= 0x3E9) {
            rpg_player_step_next();
        }
        break;
    }
}


/* The s32 return matches the implicit-int call in rpg_club_move_art (a void definition is a redeclaration
 * error). */
extern s32 D_8015EC9C;
void rpg_art_helper_blobs(s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 rpg_art_step_6(void) {
    s32 x;
    s32 y;

    rpg_se_play_latched_6xx(0x50A, 0, 1);
    if (((rpg_player_channel.unk_00 == 1) && (D_8015EC9C >= 0x3D)) || (rpg_player_channel.unk_00 >= 2)) {
        rpg_se_play_latched_6xx(0x50C, 0xA, 7);
    }
    switch (rpg_player_channel.unk_00) {
    case 0:
        /* FAKE: D_8015EC9C written as element 60 of rpg_enemy_slot. as1 does not move a load above a store
         * through the same symbol, so the selector load stays in front of the stores, as in the original
         * (the counter and the selector are one int array there). Writing all five stores this way
         * matches too. T-25308 */
        (&rpg_enemy_slot)[60] = 1;
        rpg_scratch = 0xA;
        rpg_move_timer = 0;
        rpg_move_value_0 = 0xA;
        rpg_move_value_1 = 0;
        /* FAKE: scalar-address view keeps the selector in $v1 and the constant 1 in $a3 (T-17114, T-24910;
         * 20 rows without it, T-25308). */
        switch (*(s32 *)&rpg_enemy_slot) {
        default:
            rpg_move_params.param_4 = 0xB8;
            rpg_move_params.param_1 = 0x48;
            rpg_move_params.param_3 = -0x50;
            break;
        case 0:
            rpg_move_params.param_4 = 0x70;
            rpg_move_params.param_1 = 0x10;
            rpg_move_params.param_3 = -0x30;
            break;
        case 1:
            rpg_move_params.param_4 = 0xA8;
            rpg_move_params.param_1 = 0x24;
            rpg_move_params.param_3 = -0x1C;
            D_801215FD = 4;
            D_80121685 = 4;
            break;
        case 2:
            rpg_move_params.param_4 = 0xB0;
            rpg_move_params.param_1 = -0x18;
            rpg_move_params.param_3 = -0x50;
            break;
        }
        rpg_player_substep_next();
        break;
    case 1:
        /* FAKE: the address views keep x and y as their own webs ($v0/$v1); with plain reads the
         * quotients are folded and spilled (281 rows). uopt then keeps -x and -y in temporaries that
         * cross the first call. T-25114, T-25308 */
        x = (*(s32 *)&rpg_move_params.param_1 * D_8015EC9C) / rpg_move_params.param_4;
        y = (*(s32 *)&rpg_move_params.param_3 * D_8015EC9C) / rpg_move_params.param_4;
        rpg_art_helper_blobs(D_8015EC9C, -x, -y, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        rpg_art_helper_blobs(D_8015EC9C, -x, -y, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        D_8015EC9C++;
        if (rpg_move_params.param_4 < D_8015EC9C) {
            rpg_player_substep_next();
            D_8015EC9C = 1;
        }
        break;
    case 2:
        rpg_art_helper_blobs(rpg_move_params.param_4, -rpg_move_params.param_1, -rpg_move_params.param_3, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        rpg_art_helper_blobs(rpg_move_params.param_4, -rpg_move_params.param_1, -rpg_move_params.param_3, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        D_8015EC9C++;
        if (D_8015EC9C >= 0x79) {
            rpg_player_substep_next();
            D_8015EC9C = 1;
        }
        break;
    case 3:
        /* FAKE: same web-keeping view as case 1 (297 rows without it); the view on the counter also keeps
         * it as the first addu operand of y. T-25114, T-25308 */
        x = rpg_move_params.param_1 - *(s32 *)&D_8015EC9C;
        y = *(s32 *)&D_8015EC9C + rpg_move_params.param_3;
        rpg_art_helper_blobs(rpg_move_params.param_4, -x, -y, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        rpg_art_helper_blobs(rpg_move_params.param_4, -x, -y, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        D_8015EC9C++;
        /* FAKE: D_8015EC5C read through the counter's symbol, so as1 keeps the test loads behind the
         * counter store, as in the original. T-25308 */
        if (((&D_8015EC9C)[-16] - D_8015EC9C) < (D_8015EC9C + rpg_move_params.param_3)) {
            rpg_player_step_next();
        }
        break;
    }
}

void rpg_art_helper_blobs(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 *phase;
    s32 end_candidate;
    s32 phase_sum;
    s32 start;
    s32 end;
    s32 i;
    s32 color_phase;

    /* FAKE: T-25114, rpg_enemy_slot is an int array that the header declares as a scalar (as in T-9310);
     * its subscripts give the original's base-first addresses. The two integer address sums keep the
     * arm's base-first and the join's index-first `addu` in one register range ($s7). */
    if (arg3 < ((&rpg_enemy_slot)[arg6] / 10)) {
        phase = (s32 *)((s32)&rpg_enemy_slot + arg7 * 4);
        (&rpg_enemy_slot)[arg6] -= arg3 * 0xA;
        *phase -= 1;
        if (*phase < 0) {
            *phase = 2;
        }
    }
    phase = (s32 *)(arg7 * 4 + (s32)&rpg_enemy_slot);
    for (i = 0; i < 0x32; i++) {
        end_candidate = (&rpg_enemy_slot)[arg6] / 10 + (i * arg3);
        phase_sum = *phase + i;
        if (end_candidate < arg0) {
            start = (&rpg_enemy_slot)[arg6] / 10 + (i - 1) * arg3;
            /* FAKE: T-24522, T-25114: the `* 1` keeps this arm's remainder in its own range, as in the original. */
            color_phase = (phase_sum % 3) * 1;
            end = end_candidate;
            /* FAKE: T-25114, the shared clamp label keeps both `start < 0` tests as in the original (merging
             * them after the if/else costs 18 rows). */
            if (start < 0) {
                goto block_9;
            }
        } else {
            start = (&rpg_enemy_slot)[arg6] / 10 + (i - 1) * arg3;
            color_phase = phase_sum % 3;
            end = arg0;
            i = 0x64;
            if (start < 0) {
block_9:
                start = 0;
            }
        }
        switch (color_phase) {
        case 0:
            rpg_art_draw_shape(start, arg1 * start / arg0, arg2 * start / arg0, end, arg1 * end / arg0, arg2 * end / arg0, arg8 << 0x10, arg8 << 8, 4);
            break;
        case 1:
            rpg_art_draw_shape(start, arg1 * start / arg0, arg2 * start / arg0, end, arg1 * end / arg0, arg2 * end / arg0, arg8 << 8, arg8, 4);
            break;
        case 2:
            rpg_art_draw_shape(start, arg1 * start / arg0, arg2 * start / arg0, end, arg1 * end / arg0, arg2 * end / arg0, arg8, arg8 << 0x10, 4);
            break;
        }
    }
    dtd_on_tpage(0, 0, 4, arg5 - 4, 0);
    (&rpg_enemy_slot)[arg6] += arg4;
}

void rpg_art_draw_shape(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 unused; /* FAKE: unused local, only its 4-byte frame slot above p is observed (DATE func_8014EF1C; T-17106) */
    POLY_G4 *p;

    arg0 = 0x2F - arg0;
    arg3 = 0x2FU - arg3; /* FAKE: the unsigned constant keeps this 0x2F out of the constant hoisted for arg0 (the original has two separate li); T-17106 */
    arg1 -= 0x1F;
    arg2 -= 0x1F;
    arg4 -= 0x1F; arg5 -= 0x1F; /* one line: as1 then stores arg5 before arg4 like the original */
    p = (POLY_G4 *)GetWorkBase(0x24, draw_buffer);
    func_8009F054(p);
    SetSemiTrans(p, 1);
    p->r0 = ((arg6 >> 16) & 0xFF); p->g0 = ((arg6 >> 8) & 0xFF); p->b0 = (arg6 & 0xFF);
    p->r1 = ((arg6 >> 16) & 0xFF); p->g1 = ((arg6 >> 8) & 0xFF); p->b1 = (arg6 & 0xFF);
    p->r2 = ((arg7 >> 16) & 0xFF); p->g2 = ((arg7 >> 8) & 0xFF); p->b2 = (arg7 & 0xFF);
    p->r3 = ((arg7 >> 16) & 0xFF); p->g3 = ((arg7 >> 8) & 0xFF); p->b3 = (arg7 & 0xFF);
    /* FAKE: each (unused - unused) adds one variable reference and no code. uopt ends a block after 20 references and
     * the original has its boundary after y1 (arg3 is reloaded for x2, arg0 for x0); without them it falls after y2 and
     * arg3 is reloaded early into $a3. T-17106 */
    p->x0 = game_state.draw_origin_x[draw_buffer] + arg0 + (unused - unused);
    p->y0 = game_state.draw_origin_y[draw_buffer] + arg1 + (unused - unused);
    p->x1 = game_state.draw_origin_x[draw_buffer] + arg0 + (unused - unused);
    p->y1 = game_state.draw_origin_y[draw_buffer] + arg2 + (unused - unused);
    p->x2 = game_state.draw_origin_x[draw_buffer] + arg3;
    p->y2 = game_state.draw_origin_y[draw_buffer] + arg4;
    p->x3 = game_state.draw_origin_x[draw_buffer] + arg3;
    p->y3 = game_state.draw_origin_y[draw_buffer] + arg5;
    AddPrim(&draw_ot_tags[(draw_buffer << 0xA) + (arg8 * 4)], (s32)p);
}
