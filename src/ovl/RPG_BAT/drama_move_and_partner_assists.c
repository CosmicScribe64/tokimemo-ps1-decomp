/* drama_move_and_partner_assists.c: src/ovl/RPG_BAT/801504E0.c:29: drama club move, girl-index assist dispatch and shared run-in/run-out animation; body 32: switch (rpg_player_step) {; 35: rpg_message_show(&rpg_text_player_pointers, 1, 0x5A); club-index dispatch src/ovl/RPG_BAT/8013CC90.c:96. Original object 0x801504E0-0x80151C10 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_player_step /* switched and compared as s32 here (RPG_BAT.h: u32), T-17111 */
#include "common.h"
#include "ovl/RPG_BAT.h"

extern s32 rpg_player_step;

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_yuina_stop_nonsense(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_player_0, "紐緒「馬鹿な事言ってるんじゃないわ");
        rpg_message_show(&rpg_text_player_pointers, 1, 0x96);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x27, 0x0600002D, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(0);
        return;
    case 3:
        rpg_voice_reset();
        rpg_player_step_next();
        return;
    }
}

void rpg_club_move_drama(void) {
    s32 i;

    switch (rpg_player_step) {
    case 0:
        strcpy(rpg_text_player_0, "　　　　　　　脱力漫才　　　　　　　");
        rpg_message_show(&rpg_text_player_pointers, 1, 0x5A);
        rpg_pick_target();
        rpg_set_target_position(rpg_enemy_slot);
        rpg_player_damage_roll();
        rpg_drama_overlay_spawn();
        rpg_player_step_next();
        rpg_player_step_next();
        break;
    case 2:
        rpg_move_approach();
        break;
    case 3:
        rpg_drama_step_3();
        break;
    case 4:
        if (rpg_encounter == 0x08000000) {
            rpg_player_step = 0xC8;
        } else {
            rpg_drama_overlay_fade(rpg_player_channel.unk_04);
            rpg_player_channel.unk_04 += 4;
            if (rpg_player_channel.unk_04 >= 0xE1) {
                rpg_player_step_next();
            }
        }
        break;
    case 5:
        rpg_apply_damage_to_targets();
        if (rpg_enemy_action & 0x100000) {
            for (i = 0; i < 3; i++) {
                ((Actor *)actor_pool)[149 + i].draw_flags &= ~1;
            }
        }
        rpg_player_step_next();
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        rpg_move_assist_dispatch();
        break;
    default:
        rpg_player_step = 0x64;
    case 0x64:
        rpg_drama_overlay_fade(0xE0 - rpg_player_channel.unk_04);
        rpg_player_channel.unk_04 += 4;
        if (rpg_player_channel.unk_04 >= 0xE1) {
            rpg_drama_overlay_hide();
            rpg_player_step_next();
        }
        break;
    case 0x65:
        rpg_player_wait_step(0x14);
        break;
    case 0x66:
        rpg_move_return();
        break;
    case 0x67:
        rpg_player_action_end();
        break;
    case 0xC8:
        rpg_player_wait_step(0x3C);
        break;
    case 0xC9:
        strcpy(rpg_text_player_0, "ロボ「ガガッ・・理解不能 ");
        rpg_message_show(&rpg_text_player_pointers, 1, 0x5A);
        rpg_player_step_next();
        break;
    case 0xCA:
        rpg_player_wait_msg_step(0);
        break;
    case 0xCB:
        rpg_apply_damage_single();
        rpg_player_step_next();
        break;
    case 0xCC:
        rpg_player_wait_step(0x3C);
        break;
    case 0xCD:
        rpg_dialog_yuina_stop_nonsense();
        break;
    case 0xCE:
        rpg_actor_set_position(0x38, 0x140, 0x5E);
        rpg_actor_set_animation_alt(0x38, 1, 1, 1);
        rpg_actor_set_position(0x32, 0x140, 0x60);
        rpg_actor_set_animation_alt(0x32, 6, 5, 1);
        ((Actor *)actor_pool)[152].draw_flags |= 1;
        ((Actor *)actor_pool)[152].unk_01 = 4;
        ((Actor *)actor_pool)[146].unk_01 = 4;
        rpg_player_step_next();
        break;
    case 0xCF:
        ((Actor *)actor_pool)[152].x -= 0x10;
        ((Actor *)actor_pool)[146].x -= 0x10;
        rpg_player_channel.unk_04 += 1;
        if (rpg_player_channel.unk_04 == 2) {
            rpg_se_play_latched_6xx(0x513, 0, 5);
            rpg_actor_set_animation(0x34, 0xC, 1, 1);
            rpg_actor_set_animation(0x33, 8, 1, 1);
            ((Actor *)actor_pool)[146].unk_18 = 2;
        }
        if (rpg_player_channel.unk_04 == 0xA) {
            ((Actor *)actor_pool)[152].unk_01 = 6;
            ((Actor *)actor_pool)[146].unk_01 = 6;
        }
        if (rpg_player_channel.unk_04 >= 0x16) {
            rpg_player_step_next();
        }
        break;
    case 0xD0:
        if (rpg_player_channel.unk_08 == 0x14) {
            rpg_actor_set_animation(0x33, 0xFF, 1, 0);
        }
        rpg_player_wait_step(0x3C);
        break;
    case 0xD1:
        rpg_actor_set_animation(0x34, 2, 1, 1);
        rpg_actor_set_animation(0x33, 0xFF, 1, 0);
        rpg_player_step = 0x65;
        break;
    }
}

void rpg_move_assist_dispatch(void) {
    switch (rpg_girl_id) {
    case 0:
        rpg_move_assist_shiori();
        return;
    case 1:
        rpg_move_assist_mio();
        return;
    case 2:
        rpg_move_assist_yuina();
        return;
    case 3:
        rpg_move_assist_ayako();
        return;
    case 4:
        rpg_move_assist_saki();
        return;
    case 5:
        rpg_move_assist_yukari();
        return;
    case 6:
        rpg_move_assist_nozomi();
        return;
    case 7:
        rpg_move_assist_mira();
        return;
    case 8:
        rpg_move_assist_yuko();
        return;
    case 9:
        rpg_move_assist_megumi();
        return;
    case 10:
        rpg_move_assist_yumi();
        return;
    }
}

/* returns int without a value, selector read as a word: the switch temporary stays in $v1/$v0 (T-8080, T-9330) */
s32 rpg_girl_effect_a(void) {
    switch (*(u32 *) &rpg_scratch) {
    case 0:
        rpg_actor_set_animation_alt(0x32, 6, 5, 1);
        rpg_scratch += 1;
        return;
    case 1:
        rpg_move_timer += 1;
        if (rpg_move_timer >= 0xF) {
            rpg_scratch += 1;
        }
        return;
    case 2:
        rpg_actor_set_animation(0x34, 0xC, 1, 1);
        rpg_actor_set_position(0x33, 0, 0);
        rpg_actor_set_animation(0x33, 8, 1, 1);
        rpg_se_play_latched_6xx(0x513, 0, 5);
        rpg_scratch += 1;
        return;
    case 3:
        rpg_move_timer += 1;
        if (rpg_move_timer >= 0x1F) {
            rpg_actor_set_animation(0x33, 8, 1, 0);
            rpg_scratch += 1;
        }
        return;
    }
}

/* (u32) on the s16 compare: its constant 1 is then the unsigned 1 of the case label and shares $v0 (T-9330) */
void rpg_girl_effect_b(void) {
    switch (*(u32 *) &rpg_scratch) {
    case 0:
        rpg_actor_set_animation_alt(0x32, 7, 5, 1);
        rpg_scratch += 1;
        return;
    case 1:
        if ((u32) D_801213B0 == 1) {
            rpg_actor_set_position(0x33, 0, 0);
            rpg_actor_set_animation(0x33, 8, 1, 1);
            rpg_se_play_latched_6xx(0x512, 0, 5);
            rpg_scratch += 1;
            return;
        }
    case 2:
        rpg_move_timer += 1;
        if (rpg_move_timer >= 0x1F) {
            rpg_actor_set_animation(0x33, 8, 1, 0);
            rpg_scratch += 1;
        }
        return;
    }
}

void rpg_girl_run_set_rect(void) {
    switch (rpg_girl_id) {
    case 0:
        rpg_move_params.param_4 = 4;
        rpg_move_params.param_5 = 0;
        break;
    case 1:
        rpg_move_params.param_4 = 5;
        rpg_move_params.param_5 = -2;
        break;
    case 2:
        rpg_move_params.param_4 = 6;
        rpg_move_params.param_5 = 2;
        break;
    case 3:
        rpg_move_params.param_4 = 5;
        rpg_move_params.param_5 = 2;
        break;
    case 4:
        rpg_move_params.param_4 = 5;
        rpg_move_params.param_5 = 2;
        break;
    case 5:
        rpg_move_params.param_4 = 5;
        rpg_move_params.param_5 = 2;
        break;
    case 6:
        rpg_move_params.param_4 = 3;
        rpg_move_params.param_5 = 2;
        break;
    case 7:
        rpg_move_params.param_4 = 4;
        rpg_move_params.param_5 = 1;
        break;
    case 8:
        rpg_move_params.param_4 = 5;
        rpg_move_params.param_5 = 1;
        break;
    case 9:
        rpg_move_params.param_4 = 2;
        rpg_move_params.param_5 = 0;
        break;
    case 10:
        rpg_move_params.param_4 = 8;
        rpg_move_params.param_5 = 2;
        break;
    }
    rpg_move_params.param_0 = 0x118;
    rpg_move_params.param_1 = 0x88;
    rpg_move_params.param_2 = 0x118 - rpg_move_params.param_4;
    rpg_move_params.param_3 = 0x60 - rpg_move_params.param_5;
}

typedef struct {
    u8 *s[41];
} StrTbl41; /* size 0xA4 */
extern StrTbl41 rpg_drama_lines;

void rpg_drama_pick_lines(void) {
    s32 a;
    s32 b;
    s32 c;
    StrTbl41 tbl;

    tbl = rpg_drama_lines;
    a = rpg_rand(0x29);
    b = rpg_rand(0x29);
    if (b == a) {
        b = (b + 1) % 41;
    }
    c = rpg_rand(0x29);
    if (c == a) {
        c = (c + 1) % 41;
    }
    if (c == b) {
        c = (c + 1) % 41;
    }
    strcpy(rpg_text_player_0, tbl.s[a]);
    strcpy(rpg_text_player_1, tbl.s[b]);
    strcpy(rpg_text_player_2, tbl.s[c]);
}

void rpg_drama_step_3(void) {
    switch (rpg_player_channel.unk_00) {
    case 0:
        rpg_drama_pick_lines();
        rpg_message_show(&rpg_text_player_pointers, 3, 0x5A);
        rpg_actor_set_animation(0x34, 3, 1, 1);
        rpg_player_substep_next();
        return;
    case 1:
        if (game_state.pad_pressed & PAD_CROSS) {
            rpg_player_substep_next();
            return;
        }
        rpg_player_wait_substep(0x3C);
        return;
    case 2:
        rpg_actor_set_animation(0x34, 4, 5, 1);
        rpg_player_substep_next();
        return;
    case 3:
        if (D_80121438 == 1) {
            rpg_se_play_latched_6xx(0x511, 0, 3);
        }
        rpg_player_wait_msg_substep(1);
        return;
    case 4:
        rpg_actor_set_animation(0x34, 3, 1, 1);
        rpg_player_substep_next();
        return;
    case 5:
        if (game_state.pad_pressed & PAD_CROSS) {
            rpg_player_substep_next();
            return;
        }
        rpg_player_wait_substep(0x32);
        return;
    case 6:
        rpg_actor_set_animation(0x34, 4, 5, 1);
        rpg_player_substep_next();
        return;
    case 7:
        if (D_80121438 == 1) {
            rpg_se_play_latched_6xx(0x511, 0, 1);
        }
        rpg_player_wait_msg_substep(2);
        return;
    case 8:
        rpg_actor_set_animation(0x34, 3, 1, 1);
        rpg_player_substep_next();
        return;
    case 9:
        if (game_state.pad_pressed & PAD_CROSS) {
            rpg_player_substep_next();
            return;
        }
        rpg_player_wait_substep(0x32);
        return;
    case 10:
        rpg_actor_set_animation(0x34, 4, 5, 1);
        rpg_player_substep_next();
        return;
    case 11:
        if (D_80121438 == 1) {
            rpg_se_play_latched_6xx(0x511, 0, 2);
        }
        rpg_player_wait_msg_substep(3);
        return;
    case 12:
        rpg_player_step_next();
        return;
    }
}

void rpg_drama_overlay_fade(s32 arg0) {
    s32 temp_v0;

    D_8012146B = arg0;
    D_801214AF = arg0;
    D_801214F3 = arg0;
    if (arg0 < 0x80) {
        temp_v0 = 0x80 - arg0;
        D_80121603 = temp_v0;
        D_80121647 = temp_v0;
        D_8012168B = temp_v0;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_drama_overlay_spawn(void) {
    load_palette(rpg_asset_palette_4, 0x1B, 1, 3, 7);
    switch (rpg_enemy_action & 0xFFFF0000) {
    case 0x100000:
        rpg_actor_set_position(0x35, 0x78, 0x68);
        rpg_actor_set_position(0x36, 0x40, 0x58);
        rpg_actor_set_position(0x37, 0x38, 0x88);
        rpg_actor_set_animation_alt(0x35, 9, 1, 1);
        rpg_actor_set_animation_alt(0x36, 9, 1, 1);
        rpg_actor_set_animation_alt(0x37, 9, 1, 1);
        D_8012149C = 0x51000000;
        D_801214E0 = 0x51000000;
        D_80121524 = 0x51000000;
        D_8012146B = 0;
        D_801214AF = 0;
        D_801214F3 = 0;
        D_801214A7 = 0x1B;
        D_801214EB = 0x1B;
        D_8012152F = 0x1B;
        if (rpg_delinquent_charmed_0[0] == 1) {
            D_80121467 |= 1;
        }
        if (rpg_delinquent_charmed_0[1] == 1) {
            D_801214AB |= 1;
        }
        if (rpg_delinquent_charmed_0[2] == 1) {
            D_801214EF |= 1;
        }
        break;
    case 0x200000:
        rpg_actor_set_position(0x35, 0, 0);
        rpg_actor_set_animation(0x35, 0xA, 1, 1);
        D_8012149C = 0x51000000;
        D_8012146B = 0;
        D_801214A7 = 0x1B;
        break;
    case 0x400000:
        rpg_actor_set_position(0x35, 0, 0);
        rpg_actor_set_animation(0x35, 0xB, 1, 1);
        D_8012149C = 0x51000000;
        D_8012146B = 0;
        D_801214A7 = 0x1B;
        break;
    }
}

s32 rpg_drama_overlay_hide(void) {
    switch (rpg_enemy_action & 0xFFFF0000) {
    case 0x100000:
        rpg_actor_set_animation_alt(0x35, 9, 1, 0);
        rpg_actor_set_animation_alt(0x36, 9, 1, 0);
        rpg_actor_set_animation_alt(0x37, 9, 1, 0);
        *(u8 *)&D_80121603 = 0x80;
        *(u8 *)&D_80121647 = 0x80;
        *(u8 *)&D_8012168B = 0x80;
        break;
    case 0x200000:
        rpg_actor_set_animation_alt(0x35, 9, 1, 0);
        *(u8 *)&D_80121603 = 0x80;
        break;
    case 0x400000:
        rpg_actor_set_animation_alt(0x35, 9, 1, 0);
        *(u8 *)&D_80121603 = 0x80;
        break;
    }
}

void rpg_girl_run_in(void) {
    switch (rpg_player_channel.unk_00) {
    case 0:
        rpg_girl_run_set_rect();
        rpg_actor_set_position(0x1D, 0x118, 0x88);
        rpg_actor_set_position(0x32, rpg_move_params.param_4 + 0x118, rpg_move_params.param_5 + 0x88);
        rpg_actor_set_animation_alt(0x1D, 0, 1, 1);
        rpg_actor_set_animation_alt(0x32, 7, 5, 1);
        D_801213B0 = 1;
        D_80120E05 = 4;
        D_80121399 = 3;
        rpg_player_substep_next();
        break;
    case 1:
        rpg_actor_set_position(0x1D,
                      (rpg_move_params.param_2 - rpg_move_params.param_0) * rpg_player_channel.unk_08 / 100 + rpg_move_params.param_0,
                      (rpg_move_params.param_3 - rpg_move_params.param_1) * rpg_player_channel.unk_08 / 100 + rpg_move_params.param_1);
        rpg_actor_set_position(0x32,
                      (rpg_move_params.param_2 - rpg_move_params.param_0) * rpg_player_channel.unk_08 / 100 + rpg_move_params.param_0 + rpg_move_params.param_4,
                      (rpg_move_params.param_3 - rpg_move_params.param_1) * rpg_player_channel.unk_08 / 100 + rpg_move_params.param_1 + rpg_move_params.param_5);
        rpg_player_channel.unk_08 = rpg_player_channel.unk_08 + 4;
        if (rpg_player_channel.unk_08 >= 0x65) {
            rpg_player_substep_next();
        }
        break;
    case 2:
        rpg_player_step_next();
        break;
    }
}

void rpg_girl_run_out(void) {
    if (rpg_player_timer_2 == 0) {
        rpg_actor_set_animation(0x34, 2, 1, 1);
        rpg_actor_set_animation(0x33, 8, 1, 0);
    }
    rpg_actor_set_position(0x1D, (rpg_move_params.param_0 - rpg_move_params.param_2) * rpg_player_timer_2 / 100 + rpg_move_params.param_2,
                  (rpg_move_params.param_1 - rpg_move_params.param_3) * rpg_player_timer_2 / 100 + rpg_move_params.param_3);
    rpg_actor_set_position(0x32, (rpg_move_params.param_0 - rpg_move_params.param_2) * rpg_player_timer_2 / 100 + rpg_move_params.param_2 + rpg_move_params.param_4,
                  (rpg_move_params.param_1 - rpg_move_params.param_3) * rpg_player_timer_2 / 100 + rpg_move_params.param_3 + rpg_move_params.param_5);
    rpg_player_timer_2 += 4;
    if (rpg_player_timer_2 >= 0x65) {
        rpg_actor_set_position(0x1D, 0, 0);
        if (rpg_girl_id == 1 && rpg_girl_action == 0x1000) {
            rpg_actor_set_animation(0x1D, 7, 1, 1);
        } else {
            rpg_actor_set_animation(0x1D, 0, 1, 1);
        }
        rpg_actor_set_animation_alt(0x32, 0xFF, 1, 0);
        D_80121399 = 5;
        rpg_player_step_next();
    }
}

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D1B8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D1C4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D1D0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D1E0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D1F0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D200);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D210);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D220);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D230);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D240);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D254);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D268);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D27C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D290);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D2A4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D2B8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D2CC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D2E0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D2F4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D308);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D31C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D330);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D348);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D360);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D378);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D38C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D3A0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D3B8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D3D0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D3E8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D400);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D41C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D438);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D454);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D470);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D48C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D4AC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D4CC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D4EC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D50C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/drama_move_and_partner_assists.rodata", D_8015D52C);
