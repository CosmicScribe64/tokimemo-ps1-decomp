/* girl_action_saki.c: src/ovl/RPG_BAT/80136D50.c:26: girl 4 action; body 27: rpg_saki_effects_tick(); 28: switch (rpg_girl_step) {; partner-id dispatch src/ovl/RPG_BAT/partner_scheduler_and_motion.c:24. Original object 0x80136D50-0x80137280 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_saki_cheer(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_girl_0, "虹野「がんばってっ！");
        rpg_message_show(rpg_text_girl_pointers, 1, 0x82);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x06000032, 0x08000034, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_reset();
        rpg_girl_step_next();
        return;
    }
}

void rpg_girl_action_saki(void) {
    rpg_saki_effects_tick();
    switch (rpg_girl_step) {
    case 0:
        rpg_girl_move_linear(0x1D, 0, 0, -0x20, -0x30, 0x64);
    default:
        break;
    case 1:
        rpg_actor_set_animation(0x1D, 5, 5, 1);
        rpg_girl_step_next();
        break;
    case 2:
        if (D_80120E1C == 8) {
            rpg_effects_select_asset_set(5);
            rpg_girl_damage_roll();
            rpg_girl_step_next();
        }
        break;
    case 3:
        rpg_dialog_saki_cheer();
        break;
    case 4:
        rpg_girl_wait_animation_step(0x1D);
        break;
    case 5:
        switch (rpg_girl_timer_a) {
        case 0:
            rpg_se_play_latched_5xx(0x511, 1, 0);
            rpg_saki_effect_step(4, 6);
            break;
        case 1:
            rpg_saki_effect_step(5, 7);
            break;
        case 5:
            rpg_saki_effect_step(6, 7);
            break;
        case 6:
            rpg_saki_effect_step(7, 6);
            break;
        case 9:
            rpg_saki_effect_step(8, 7);
            break;
        case 13:
            rpg_saki_effect_step(9, 7);
            break;
        case 14:
            rpg_saki_effect_step(0xA, 6);
            break;
        case 20:
            rpg_saki_effect_step(0xB, 7);
            break;
        case 22:
            rpg_saki_effect_step(0xC, 7);
            break;
        case 26:
            rpg_saki_effect_step(0xD, 6);
            break;
        case 27:
            rpg_saki_effect_step(0xE, 7);
            break;
        case 32:
            rpg_saki_effect_step(0xF, 7);
            break;
        case 40:
            rpg_girl_step_next();
            break;
        }
        rpg_girl_timer_a += 1;
        break;
    case 6:
        rpg_apply_damage(0, 0x21, rpg_damage, 0);
        rpg_se_play_latched_5xx(0x502, 1, 0);
        rpg_girl_step_next();
        break;
    case 7:
        rpg_girl_wait_step(0x28);
        break;
    case 8:
        rpg_actor_set_animation(0x1D, 2, 1, 1);
        rpg_girl_step_next();
        break;
    case 9:
        rpg_girl_move_linear(0x1D, -0x20, -0x30, 0, 0, 0x64);
        break;
    case 10:
        rpg_actor_set_animation(0x1D, 0, 1, 1);
        rpg_effects_select_asset_set(0xFF);
        rpg_girl_step_next();
        break;
    case 11:
        rpg_girl_wait_step(0x14);
        break;
    case 12:
        rpg_girl_action_end();
        break;
    }
}

void rpg_saki_effect_step(s32 actor, s32 animation) {
    if (animation == 7) {
        if (!(rpg_scratch & 1)) {
            rpg_actor_set_position(actor, rpg_rand(0x10) + 0x108, 0x40);
        } else {
            rpg_actor_set_position(actor, rpg_rand(0x10) + 0x108, 0x58);
        }
        rpg_scratch += 1;
    } else {
        rpg_actor_set_position(actor, rpg_rand(0x10) + 0x108, 0x40);
    }
    rpg_actor_set_animation_alt(actor, animation, 5, 1);
}

void rpg_saki_effects_tick(void) {
    s32 var_s1;
    u8 *var_s0;

    var_s0 = actor_pool;
    var_s1 = 0;
    do {
        *(s16 *)(var_s0 + 0x1ABA) = (s16) (*(s16 *)(var_s0 + 0x1ABA) + 1);
        if (!(*(u8 *)(var_s0 + 0x1A92) & 1)) {
            rpg_actor_set_animation(var_s1 + 4, 0xFF, 1, 0);
        }
        var_s1 += 1;
        var_s0 += 0x44;
    } while (var_s1 != 0xC);
}
