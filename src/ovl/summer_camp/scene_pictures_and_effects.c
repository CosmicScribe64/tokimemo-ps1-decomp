/* scene_pictures_and_effects.c: src/ovl/KANGEI/80135F10.c:18-48; picture/animation resource binders, callback dispatchers, talk and effect flow; no unconfirmed narrative or folder identity is claimed. Original object 0x80135F10-0x80137B90 (config/objects/KANGEI.txt). */
#define MAIN_API_OVERRIDE_srand /* T-22016: int-returning view for camp_scene_result_resource_load */
#include "common.h"
#include "ovl/summer_camp.h"
#include "main/sprite.h"
#include "main/scene.h"

s32 srand();

typedef struct {
    void (*f[24])();
} FnTbl24; /* size 0x60 */
extern FnTbl24 D_80139F4C;

typedef struct {
    void (*f[40])();
} FnTbl40; /* size 0xA0 */
extern FnTbl40 D_80139E70;

void camp_scene_still_picture_pointer_bind(void) {
    D_80139DB0 = 0x801B4400;
    D_80139DB4 = 0x801B6400;
    D_80139DB8 = 0x801BA400;
    D_80139DBC = 0x801BE400;
    D_80139DC0 = 0x801C2400;
    D_80139DC4 = 0x801C6400;
    D_80139DC8 = 0x801CA400;
}

void camp_scene_animation_picture_pointer_bind(void) {
    D_80139DCC = 0x801EAFC4;
    D_80139DD0 = 0x801EB144;
    D_80139DD4 = 0x801EAFF0;
    D_80139DD8 = 0x801EB14C;
    D_80139DDC = 0x801EB030;
    D_80139DE0 = 0x801EB174;
    D_80139DE4 = *(s16 *)0x801EB17C;
    D_80139DE8 = *(s16 *)0x801EB180;
    D_80139DEC = 0x801B4400;
}

void camp_scene_picture_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = D_80139E70;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void camp_scene_icon_parameter_reset(void) {
    s16 i;

    hud_icon_bar_init();
    for (i = 0; i < 0xF; i++) {
        icon_can_use_set(i, 0);
    }
    hud_icon_bar_update(1);
    parameter_show_init();
    parameter_show_init();
    bg_brightness = 0;
    D_80139AC0 = 0;
    mode_step_next();
}

void camp_talk_subindex_select(void) {
    if (camp_talk_line == 0) {
        D_80139AE4 = 2;
    } else {
        D_80139AE4 = 0;
    }
    camp_opening_talk_run();
}

void camp_day_event_result_update(void) {
    if ((camp_choice_results[(u8) camp_result_choice] == 0) && (game_state.date_girl != 0xFF) && game_state.girls[game_state.date_girl].flags.f.met) {
        game_state.girls[game_state.date_girl].tokimeki -= 0xA;
    } else if (camp_choice_results[(u8) camp_result_choice] == 1) {
        game_state.param_stress.value += 0xA;
    }
    check_para_limit();
    mode_step_next();
}

void camp_scene_audio_transition_c(void) {
    audio_command(0x23);
    audio_command(0x24);
    audio_command(0x2E);
    audio_command(0x203);
    mode_step_next();
}

void camp_scene_talk_and_audio_tick(void) {
    camp_opening_talk_run();
    if (game_state.mode_step_timer.w++ == 0 && camp_talk_line == 3) {
        if (camp_choice_results[(u8)camp_result_choice] == 0 && camp_scene_coin_flip == 0) {
            audio_command(0x503);
        }
    }
}

void camp_scene_result_index_latch(void) {
    audio_command(0x500);
    camp_result_choice = talk_choice;
    mode_step_next();
}

void camp_scene_result_resource_load(void) {
    u8 scene_result;

    srand(game_state.frame_count);
    scene_result = camp_choice_results[(u8) camp_result_choice];
    if (scene_result == 1) {
        mode_step_next();
        mode_step_next();
    } else if (scene_result == 2) {
        mode_step_next();
        mode_step_next();
    } else {
        game_state.current_girl = GIRL_MIHARU;
        camp_miharu_resource_variant = (u8) (rand() % 4);
        if ((((u16) game_state.player_flags.h >> 0xC) == 0xA) && (rand() % 2)) {
            camp_miharu_resource_variant = 4;
        }
        switch (*(u8 *)&camp_miharu_resource_variant) /* FAKE: T-22016, byte view of the selector keeps it in a ugen temporary */ {
        case 0:
            cd_read_sectors(0x35U, 0x801B4400, 0x8FF3);
            break;
        case 1:
            cd_read_sectors(0x35U, 0x801B4400, 0x9028);
            break;
        case 2:
            cd_read_sectors(0x35U, 0x801B4400, 0x905D);
            break;
        case 3:
            cd_read_sectors(0x35U, 0x801B4400, 0x9092);
            break;
        default:
            cd_read_sectors(0x35U, 0x801B4400, 0x90C7);
            break;
        }
    }
    mode_step_next();
}

void camp_scene_still_picture_display(void) {
    load_palette(D_80139DB0, 0x11, 1, 2, 0);
    scene_still_show(D_80139DB4, D_80139DB8, D_80139DBC, D_80139DC0, D_80139DC4, D_80139DC8);
    scene_anim_show(0, 0, 0, 0);
    mode_step_next();
}

void camp_scene_background_fade_in(void) {
    if (game_state.mode_step_timer.w++ == 0) {
        if (camp_choice_results[(u8)camp_result_choice] == 0) {
            audio_command(0x204);
        } else {
            audio_command(0x205);
        }
    }
    if (camp_choice_results[(u8)camp_result_choice] == 1) {
        normal_date_bg_fadein();
        return;
    }
    scene_still_fade_in();
}

void camp_scene_background_fade_out(void) {
    if (camp_choice_results[(u8)camp_result_choice] != 0) {
        normal_date_bg_fadeout();
        return;
    }
    if (camp_scene_coin_flip != 0) {
        scene_still_fade_out();
        return;
    }
    camp_scene_picture_transition_animate();
}

void camp_scene_picture_transition_animate(void) {
    s16 i;

    if (game_state.mode_step_timer.w == 0) {
        D_80139E04 = 0;
        D_80139E08 = 0;
        D_80139E00 = 0;
        game_state.mode_step_timer.w = 1;
    } else if (game_state.mode_step_timer.w == 1) {
        D_80139E00 += 0x10;
        D_80139E04 += 0x10;
        D_80139E08 += 0x10;
        if (D_80139E00 >= 0xF0) {
            game_state.mode_step_timer.w = 2;
            D_80139E04 = 0xFF;
            D_80139E08 = 0xFF;
            D_80139E00 = 0xFF;
            for (i = 0x3A; i < 0x40; i++) {
                func_80084C98(i, 0);
            }
            bg_brightness = 0;
        }
    } else {
        D_80139E00 -= 8;
        D_80139E04 -= 8;
        D_80139E08 -= 8;
        if (D_80139E00 < 9) {
            D_80139E04 = 0;
            D_80139E08 = 0;
            D_80139E00 = 0;
            k_reset(1);
            sprite_tables_clear();
            mode_step_next();
        }
    }
    camp_transition_quad_primitive_build(-0xA0, -0x78, 0x140, 0xA0, D_80139E00, D_80139E04, D_80139E08, 9, 1);
    dtd_on_tpage(0, 0, 9, 1, 0);
}

void camp_transition_quad_primitive_build(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8) {
    s32 unused; /* FAKE: unused local, only its 4-byte frame slot above p is observed (T-6030) */
    u8 *p;

    p = GetWorkBase(0x10, draw_buffer);
    func_8009F0F4(p);
    SetSemiTrans(p, arg8);
    p[4] = arg4;
    p[5] = arg5;
    p[6] = arg6;
    *(s16 *) (p + 8) = game_state.draw_origin_x[draw_buffer] + arg0;
    *(s16 *) (p + 0xA) = game_state.draw_origin_y[draw_buffer] + arg1;
    *(s16 *) (p + 0xC) = arg2;
    *(s16 *) (p + 0xE) = arg3;
    AddPrim(&draw_ot_tags[(draw_buffer << 0xA) + (arg7 * 4)], p);
}

void camp_scene_result_frame_tick(void) {
    camp_scene_result_timer += 1;
    normal_date_two_select();
}

void camp_choice_scene_branch(void) {
    if (talk_choice != 0) {
        game_state.mode_step += 0x13;
    }
    mode_step_next();
}

/* FAKE: shared constant and byte selector preserve compare operand order. T-10850 */
void camp_scene_script_index_select(void) {
    u8 t;
    s32 r;
    u32 one;
    u8 coin;

    one = 1;
    srand(game_state.frame_count);
    r = rand();
    camp_scene_coin_flip = r % 2;
    t = camp_choice_results[(u8)camp_result_choice];
    camp_talk_group = (t == 2) * 6 + (one == t) * 4 + 4;
    coin = camp_scene_coin_flip;
    if (coin == one && t == 0) {
        camp_talk_group += 2;
    }
    mode_step_next();
}

void camp_scene_result_progress_select(void) {
    camp_talk_group = 0xC;
    camp_talk_line = (camp_scene_result_timer >= 0x97U) + (camp_scene_result_timer >= 0x12DU);
    mode_step_next();
}

void camp_scene_result_categories_initialize(void) {
    srand(game_state.frame_count);
    game_state.current_girl = GIRL_SOTOI;
    camp_scene_result_timer = 0;
    camp_talk_group = 1;
    camp_talk_line = 0;
    srand(game_state.frame_count);
    camp_choice_results[0] = rand() % 2 + 1;
    camp_choice_results[1] = rand() % 2 + 1;
    camp_choice_results[2] = rand() % 2 + 1;
    if (rand() % 2) {
        camp_choice_results[0] = 0;
    }
    if (rand() % 2) {
        camp_choice_results[1] = 0;
    }
    if (rand() % 2) {
        camp_choice_results[2] = 0;
    }
    mode_step_next();
}

INCLUDE_RODATA("asm/ovl/KANGEI/data/summer_camp/scene_pictures_and_effects.rodata", D_801398E0);

void camp_scene_background_and_label_load(void) {
    bg_read_sub2(0x42BC);
    mode_step_next();
    strcpy(scene_place_title, &D_801398E0);
}

void camp_scene_result_background_select(void) {
    if (camp_choice_results[(u8)camp_result_choice] == 2) {
        bg_read_sub2(0x42B3);
    } else if (camp_choice_results[(u8)camp_result_choice] == 1) {
        bg_read_sub2(0x477D);
    } else {
        switch (camp_miharu_resource_variant) {
        case 0:
            bg_read_sub2(0x4787);
            break;
        case 1:
            bg_read_sub2(0x478E);
            break;
        case 2:
            bg_read_sub2(0x4797);
            break;
        case 3:
            bg_read_sub2(0x47A9);
            break;
        default:
            bg_read_sub2(0x47B2);
            break;
        }
    }
    mode_step_next();
}

void camp_scene_picture_tiles_initialize(void) {
    place_init();
    LoadSquare(0x320, 0x100, 4, 0xC, &D_80139E10);
    LoadSquare(0x324, 0x100, 4, 0xC, &D_80139E10);
    LoadSquare(0x328, 0x100, 4, 0xC, &D_80139E10);
}

void camp_scene_result_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl24 tbl;

    tbl = D_80139F4C;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
    if (game_state.mode_step >= 9U) {
        if (game_state.mode_step < 0x14U) {
            camp_scene_actor_and_particle_update();
        }
    }
}

INCLUDE_RODATA("asm/ovl/KANGEI/data/summer_camp/scene_pictures_and_effects.rodata", D_801398E8);

INCLUDE_RODATA("asm/ovl/KANGEI/data/summer_camp/scene_pictures_and_effects.rodata", D_801398EC);

void camp_scene_talk_render_top(void) {
    talk_render(-0x64, 0x30, 2, camp_talk_text_table->unk_34->unk_08, 0, &D_801398E8, &D_801398EC, 0);
    mode_step_next();
}

INCLUDE_RODATA("asm/ovl/KANGEI/data/summer_camp/scene_pictures_and_effects.rodata", D_801398F0);

INCLUDE_RODATA("asm/ovl/KANGEI/data/summer_camp/scene_pictures_and_effects.rodata", D_801398F4);

void camp_scene_talk_render_middle(void) {
    talk_render(-0x56, 0x40, 1, camp_talk_text_table->unk_34->unk_0C, 0, &D_801398F0, &D_801398F4, 0);
    mode_step_next();
}

void camp_scene_talk_render_bottom(void) {
    talk_render(-0x2C, 0x50, 0, ((s32 *)camp_talk_text_table->unk_34)[4], 0, "", "", 0);
    camp_talk_line = 5;
    mode_step_next();
}

void camp_scene_talk_effect_select_two(void) {
    D_80139AE4 = 2;
    mode_step_next();
}

void camp_scene_talk_effect_select_one(void) {
    D_80139AE4 = 1;
    mode_step_next();
}

void camp_scene_talk_effect_select_zero(void) {
    D_80139AE4 = 0;
    mode_step_next();
}

void camp_scene_audio_bank_load_start(void) {
    audio_command(0xBF);
    audio_bank_load_start(1, 0xBF98, 0xBF79, 0xC982, 0xC935, 0xC91B);
    mode_step_next();
}

void camp_scene_audio_bank_poll_retry(void) {
    if (audio_bank_load_poll() == 1) {
        audio_command(0x203);
        mode_step_next();
    }
    if (game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void camp_scene_animation_data_sector_load(void) {
    cd_read_sectors(0x6E, 0x801B4400, 0xBBA5);
    camp_scene_animation_picture_pointer_bind();
    mode_step_next();
}

/* Port of the MASTER clone func_80137BF4 (same opcodes, T-18400). */
void camp_scene_actor_and_particle_update(void) {
    KangeiPoint2 *temp_s0_2;
    s16 *temp_s1;
    /* FAKE: wide X local plus a narrow third argument preserves the
     * original sign-extension sequence (as MASTER func_80137BF4, T-10630). */
    s32 temp_a0;
    s16 temp_a1;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 var_s2;
    s32 temp_hi;
    ActorFixed *temp_s0;

    if ((((ActorFixed *)actor_pool_96)->sequence_slot == 0) && (((ActorFixed *)actor_pool_96)->frame == 7) && (((ActorFixed *)actor_pool_96)->unk_08 == 2)) {
        ((ActorFixed *)actor_pool_96)->sequence_slot = 1;
        ((ActorFixed *)actor_pool_96)->frame = 0;
        ((ActorFixed *)actor_pool_96)->unk_08 = 0;
        ((ActorFixed *)actor_pool_96)->flags = 1;
    }
    var_s2 = 0x61;
    if ((((ActorFixed *)actor_pool_96)->sequence_slot == 2) && (((ActorFixed *)actor_pool_96)->frame == 1) && (((ActorFixed *)actor_pool_96)->unk_08 == 4)) {
        ((ActorFixed *)actor_pool_96)->sequence_slot = 3;
        ((ActorFixed *)actor_pool_96)->frame = 0;
        ((ActorFixed *)actor_pool_96)->unk_08 = 0;
        ((ActorFixed *)actor_pool_96)->flags = 1;
    }
    do {
        temp_s0 = &((ActorFixed *)actor_pool)[var_s2];
        temp_v0 = temp_s0->frame;
        if ((temp_v0 < 2) || (temp_v0 >= 8)) {
            temp_s0->y.w = temp_s0->y.w - ((((ActorFixed *)actor_pool_96)->sequence_slot == 3) * 0x50000) - 0x50000;
        } else if ((temp_v0 == 4) || (temp_v0 == 7)) {
            temp_s0->y.w = temp_s0->y.w - ((((ActorFixed *)actor_pool_96)->sequence_slot == 3) * 0x30000) - 0x30000;
        } else {
            temp_s0->y.w = temp_s0->y.w - ((((ActorFixed *)actor_pool_96)->sequence_slot == 3) * 0x20000) - 0x20000;
        }
        if (temp_s0->y.h[1] < -0x9F) {
            temp_hi = rand() % 320;
            temp_s0->x.h[1] = (s16) (temp_hi - 0xA0);
            temp_s0->y.h[1] = 0x3C;
            temp_s0->frame = (s16) (rand() % 10);
        }
        var_s2 += 1;
    } while (var_s2 < 0x74);
    var_s2 = 0;
    if ((u8) ((ActorFixed *)actor_pool_96)->unk_07 >= 0x78U) {
        do {
            temp_s0_2 = &D_80139F10[var_s2];
            temp_s1 = &D_80139F38[var_s2];
            temp_a0 = temp_s0_2->x;
            temp_v0_2 = *temp_s1;
            temp_s0_2->y -= temp_v0_2 * 4 * ((((ActorFixed *)actor_pool_96)->sequence_slot == 3) + 1);
            temp_a1 = temp_s0_2->y;
            prim_gradient_line((s32) temp_a0, (s32) temp_a1, (s16) temp_a0, (s32) (s16) ((temp_v0_2 * 0x14) + temp_a1), 0xFFFFFF, 0xFFFFFF, 0xB);
            if (temp_s0_2->y < -0xAA) {
                temp_s0_2->y = 0x3C;
                temp_s0_2->x = (rand() % 320) - 0xA0;
                *temp_s1 = (rand() % 5) + 1;
            }
            var_s2 += 1;
        } while (var_s2 < 0xA);
    }
}

void camp_scene_actor_effect_initialize(void) {
    D_80120666 = 2;
    D_80120668 = 0;
    D_80120658 = 0;
    D_80120652 = 5;
    mode_step_next();
}

void camp_scene_actor_effect_fade_in(void) {
    s16 i;
    ActorFixed *sprite;

    ((ActorFixed *)actor_pool)[96].draw_flags |= 0x80;
    ((ActorFixed *)actor_pool)[96].unk_07 += 8;
    for (i = 0x61; i < 0x74; i++) {
        sprite = &((ActorFixed *)actor_pool)[i];
        sprite->draw_flags |= 0x80;
        sprite->unk_07 += 8;
    }
    ((ActorFixed *)actor_pool)[96].flags = 5;
    if (((ActorFixed *)actor_pool)[96].unk_07 >= 0x80U) {
        ((ActorFixed *)actor_pool)[96].unk_07 = 0x80;
        ((ActorFixed *)actor_pool)[97].unk_07 = 0x80;
        mode_step_next();
    }
}

/* T-9740: integer definition without an explicit return matches the original. */
s32 camp_scene_actor_effect_fade_out(void) {
    s16 i;
    s32 mask;
    ActorFixed *sprite;
    ((ActorFixed *)actor_pool)[96].unk_07 -= 8;
    for (i = 0x61; i < 0x74; i++) {
        sprite = &((ActorFixed *)actor_pool)[i];
        sprite->unk_07 -= 8;
    }
    /* The signed mask is shared by the scalar and loop flag clears. */
    mask = ~0x80;
    if (((ActorFixed *)actor_pool)[96].unk_07 < 9U) {
        ((ActorFixed *)actor_pool)[96].unk_07 = 0;
        ((ActorFixed *)actor_pool)[96].draw_flags &= mask;
        for (i = 0x61; i < 0x74; i++) {
            sprite = &((ActorFixed *)actor_pool)[i];
            sprite->unk_07 = 0;
            sprite->draw_flags &= mask;
        }
        D_800B5BD4 = 0xF;
        sprite_tables_clear();
        audio_bank_start(0, 0x200);
        mode_step_next();
    }
}


s32 camp_club_practice_point_threshold_gate(void) {
    if (((ClubFlagBits *) &game_state.clubs[(u32) game_state.player_flags.h >> 12])->club_progress_grade < 3) {
        mode_stage_next();
        return 0;
    }
    D_800B5BD4 = 0xA;
    mode_step_next();
}

void camp_club_scene_actor_setup(void) {
    s16 var_s0;
    s32 pad; /* FAKE: unused slot above the name tables, the original puts them 8 bytes lower (sp+0x13C, sp+0x54); real source unknown. T-22016 */
    KangeiNames20 club_names_a;
    KangeiNames21 club_names_m;

    club_names_a = D_80139FAC;
    club_names_m = D_8013A088;
    game_state.player_flags_upper.b[0] = game_state.player_flags_upper.b[0] & 0xFFEF;
    camp_talk_group = 0xD;
    camp_talk_line = 0;
    camp_talk_text_table = (KObj *) D_80139A5C[0][6];
    camp_talk_command_table = D_80139A5C[1][6];
    camp_talk_voice_table = D_80139A5C[2][6];
    load_palette(D_80139DEC, 0x10, 1, 0, 0);
    load_palette(D_80139DEC, 0x11, 1, 0, 1);
    strcpy(name_code_a, club_names_a.name[(u16) game_state.player_flags.h >> 0xC]);
    strcpy(&name_code_m, club_names_m.name[(u16) game_state.player_flags.h >> 0xC]);
    actor_clear(0x60);
    ((Actor *)actor_pool)[96].unk_01 = 0xB;
    ((Actor *)actor_pool)[96].flags = 0;
    ((Actor *)actor_pool)[96].quad_flags = 0x01000000;
    ((Actor *)actor_pool)[96].draw_flags = 4;
    *(s32 *)&((Actor *)actor_pool)[96].frame_table = (s32) D_80139DD4;
    *(s32 *)&((Actor *)actor_pool)[96].sequence_table = (s32) D_80139DDC;
    *(s32 *)&((Actor *)actor_pool)[96].tpage_table = (s32) D_80139DCC;
    ((Actor *)actor_pool)[96].unk_14 = (s16) D_80139DE4;
    ((Actor *)actor_pool)[96].sequence_slot = 0;
    ((Actor *)actor_pool)[96].unk_18 = 0;
    ((Actor *)actor_pool)[96].blink_period = 1;
    ((Actor *)actor_pool)[96].unk_43 = 0;
    ((Actor *)actor_pool)[96].x = -0xA0;
    ((Actor *)actor_pool)[96].y = -0x78;
    ((Actor *)actor_pool)[96].brightness = 0;
    ((Actor *)actor_pool)[96].unk_05 = 0x80;
    ((Actor *)actor_pool)[96].unk_04 = 0;
    srand((s32) game_state.frame_count);
    ((Actor *)actor_pool)[97] = ((Actor *)actor_pool)[96];
    ((Actor *)actor_pool)[97].flags = 0x40;
    *(s32 *)&((Actor *)actor_pool)[97].frame_table = (s32) D_80139DD8;
    *(s32 *)&((Actor *)actor_pool)[97].sequence_table = (s32) D_80139DE0;
    *(s32 *)&((Actor *)actor_pool)[97].tpage_table = (s32) D_80139DD0;
    ((Actor *)actor_pool)[97].unk_14 = (s16) D_80139DE8;
    ((Actor *)actor_pool)[97].x = (s16) ((rand() % 320) - 0xA0);
    ((Actor *)actor_pool)[97].y = 0x3C;
    for (var_s0 = 0x62; var_s0 < 0x74; var_s0++) {
        ((Actor *)actor_pool)[var_s0] = ((Actor *)actor_pool)[97];
        ((Actor *)actor_pool)[var_s0].unk_18 = (s16) (rand() % 10);
    }
    for (var_s0 = 0; var_s0 < 0xA; var_s0++) {
        D_80139F38[var_s0] = (rand() % 5) + 1;
        D_80139F10[var_s0].x = (rand() % 320) - 0xA0;
        D_80139F10[var_s0].y = 0x3C;
    }
    mode_step_next();
}
