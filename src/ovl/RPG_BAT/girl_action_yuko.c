/* girl_action_yuko.c: src/ovl/RPG_BAT/801563A0.c:86: girl 8 action; body 87: switch (rpg_girl_step) {; 89: rpg_actor_set_animation(0x1D, 5, 5, 1); partner-id dispatch src/ovl/RPG_BAT/801439E0.c:24. Original object 0x801563A0-0x80156870 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

void rpg_dialog_yuko_listen(void) {
    switch (*rpg_dialog_step) {
    case 0:
        strcpy(rpg_text_girl_0, "朝日奈「ねえ、ねえっ。");
        strcpy(rpg_text_girl_1, "聞いて、聞いてっ！");
        rpg_message_show((s32) rpg_text_girl_pointers, 2, 0xA0);
        *rpg_dialog_step += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1F000015, 0x05000017, 0);
        rpg_actor_set_animation(0x1D, 7, 1, 1);
        *rpg_dialog_step += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_play_once(0x1F000018, 0x0600001D, 1);
        rpg_actor_set_animation(0x1D, 8, 1, 1);
        *rpg_dialog_step += 1;
        return;
    case 4:
        rpg_voice_wait(2);
        return;
    case 5:
        rpg_voice_reset();
        rpg_girl_step_next();
        /* fallthrough */
    default:
        return;
    }
}

/* FAKE: typed 0x44-byte record view coexists with unresolved shared byte-table/scalar declarations. T-9550 */
void rpg_dialog_enemy_what_is_it(void) {
    rpg_actor_set_animation(0x1D, 8, 1, 1);
    switch (*rpg_dialog_step) {
    case 0:
        if (rpg_enemy_action & 0x100000) {
            strcpy(rpg_text_girl_0, "不良達「なんだ、なんだ？");
        } else {
            strcpy(rpg_text_girl_0, "番長「なんだ、なんだ？");
        }
        rpg_message_show((s32) rpg_text_girl_pointers, 1, 0x82);
        *rpg_dialog_step += 1;
        return;
    case 1:
        if (rpg_enemy_action & 0x100000) {
            rpg_voice_play_once(0x1E00006F, 0x17000072, 0);
        } else {
            rpg_voice_play_once(0x10000093, 0x06000098, 0);
        }
        *rpg_dialog_step += 1;
        return;
    case 2:
        ((Actor *)actor_pool)[155].draw_flags |= rpg_rand(2);
        ((Actor *)actor_pool)[156].draw_flags |= rpg_rand(2);
        ((Actor *)actor_pool)[157].draw_flags |= rpg_rand(2);
        *rpg_dialog_step += 1;
        return;
    case 3:
        if (((s32) rpg_girl_timer_a % 15) == 0xE) {
            ((Actor *)actor_pool)[155].draw_flags ^= 1;
            ((Actor *)actor_pool)[156].draw_flags ^= 1;
            ((Actor *)actor_pool)[157].draw_flags ^= 1;
        }
        rpg_girl_timer_a += 1;
        rpg_voice_wait(0);
        return;
    case 4:
        ((Actor *)actor_pool)[155].draw_flags &= 0xFE;
        ((Actor *)actor_pool)[156].draw_flags &= 0xFE;
        ((Actor *)actor_pool)[157].draw_flags &= 0xFE;
        rpg_actor_set_animation(0x1D, 0, 1, 1);
        rpg_voice_reset();
        rpg_girl_step_next();
        /* fallthrough */
    default:
        return;
    }
}

void rpg_girl_action_yuko(void) {
    switch (rpg_girl_step) {
    case 0:
        rpg_actor_set_animation(0x1D, 5, 5, 1);
        rpg_girl_step_next();
        break;
    case 1:
        if (D_80120E1C == 2) {
            rpg_se_play_latched_5xx(0x513, 0, 1);
        }
        rpg_girl_wait_animation_step(0x1D);
        break;
    case 2:
        rpg_girl_slide_in(6, 0);
        break;
    case 3:
        rpg_dialog_yuko_listen();
        break;
    case 4:
        rpg_dialog_enemy_what_is_it();
        break;
    case 5:
        rpg_girl_wait_step(0x14);
        break;
    case 6:
        rpg_girl_slide_out(3, 0);
        break;
    case 7:
        rpg_girl_wait_step(0x14);
        break;
    case 8:
        rpg_girl_action_end();
        break;
    }
}
