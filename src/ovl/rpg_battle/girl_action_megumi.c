/* girl_action_megumi.c: src/ovl/RPG_BAT/80135AE0.c:4: girl 9 actions; body 5: switch (rpg_girl_action) {; 7: rpg_megumi_flinch(); partner-id dispatch src/ovl/rpg_battle/partner_scheduler_and_motion.c:24. Original object 0x80135AE0-0x80136070 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_girl_action_megumi(void) {
    switch (rpg_girl_action) {
    case 1:
        rpg_megumi_flinch();
        return;
    case 2:
        rpg_megumi_pause();
        return;
    case 3:
        rpg_megumi_shout();
        return;
    }
}

void rpg_megumi_flinch(void) {

    switch (rpg_girl_channel.unk_04) {
    case 0:
        rpg_actor_set_animation(0x1D, 5, 1, 1);
        rpg_se_play_latched_5xx(0x512, 1, 0);
        rpg_girl_step_next();
    default:
        break;
    case 1:
        rpg_girl_wait_step(0x14);
        break;
    case 2:
        rpg_actor_set_animation(0x1D, 4, 1, 1);
        rpg_se_play_latched_5xx(0x512, 1, 0);
        rpg_girl_step_next();
        break;
    case 3:
        rpg_girl_wait_step(0x14);
        break;
    case 4:
        rpg_actor_set_animation(0x1D, 5, 1, 1);
        rpg_se_play_latched_5xx(0x512, 1, 0);
        rpg_girl_step_next();
        break;
    case 5:
        rpg_girl_wait_step(0x14);
        break;
    case 6:
        rpg_actor_set_animation(0x1D, 4, 1, 1);
        rpg_se_play_latched_5xx(0x512, 1, 0);
        rpg_girl_step_next();
        break;
    case 7:
        rpg_girl_wait_step(0x14);
        break;
    case 8:
        strcpy(rpg_text_girl_0, "美樹原「きゃっ！");
        rpg_voice_play_once(0x1F000087, 0x18000089, 0);
        rpg_message_show((s32) rpg_text_girl_pointers, 1, 60);
        rpg_girl_step_next();
        break;
    case 9:
        rpg_actor_set_animation(0x1D, 6, 1, 1);
        rpg_girl_step_next();
        break;
    case 10:
        rpg_girl_channel.unk_0C += 1;
        rpg_actor_set_position(0x1D, rpg_girl_channel.unk_0C, 0);
        if (rpg_girl_channel.unk_0C >= 0x41) {
            rpg_girl_step_next();
        }
        break;
    case 11:
        rpg_girl_wait_msg_step(0);
        break;
    case 12:
        rpg_girl_wait_step(0x14);
        break;
    case 13:
        rpg_girl_action_end();
        rpg_girl_action = 0x40000000;
        break;
    }
}

void rpg_megumi_pause(void) {
    switch (rpg_girl_channel.unk_04) {
    case 0:
        rpg_actor_set_animation(0x1D, 1, 1, 1);
        rpg_girl_step_next();
        return;
    case 1:
        rpg_girl_channel.unk_0C += 1;
        rpg_actor_set_position(0x1D, 0x40 - rpg_girl_channel.unk_0C, 0);
        if (rpg_girl_channel.unk_0C >= 0x41) {
            rpg_actor_set_position(0x1D, 0, 0);
            rpg_actor_set_animation(0x1D, 0, 1, 1);
            rpg_girl_step_next();
            return;
        }
        return;
    case 2:
        rpg_girl_wait_step(0x14);
        return;
    case 3:
        rpg_girl_action_end();
        break;
    }
}

void rpg_megumi_shout(void) {

    switch (rpg_girl_channel.unk_04) {
    case 0:
        rpg_actor_set_animation(0x1D, 1, 1, 1);
        rpg_girl_step_next();
    default:
        break;
    case 1:
        rpg_girl_channel.unk_0C += 1;
        rpg_actor_set_position(0x1D, 0x40 - rpg_girl_channel.unk_0C, 0);
        if (rpg_girl_channel.unk_0C >= 0x41) {
            rpg_actor_set_position(0x1D, 0, 0);
            rpg_actor_set_animation(0x1D, 0, 1, 1);
            rpg_girl_step_next();
        }
        break;
    case 2:
        rpg_girl_wait_step(0x14);
        break;
    case 3:
        strcpy(rpg_text_girl_0, "美樹原「はっ！");
        rpg_voice_play_once(0x1F00008A, 0x1900008C, 0);
        rpg_message_show((s32) rpg_text_girl_pointers, 1, 70);
        rpg_girl_step_next();
        break;
    case 4:
        rpg_girl_wait_step(0xA);
        break;
    case 5:
        rpg_actor_set_animation(0x1D, 6, 1, 1);
        rpg_girl_step_next();
        break;
    case 6:
        rpg_girl_channel.unk_0C += 1;
        rpg_actor_set_position(0x1D, rpg_girl_channel.unk_0C, 0);
        if (rpg_girl_channel.unk_0C >= 0x41) {
            rpg_girl_step_next();
        }
        break;
    case 7:
        rpg_girl_wait_msg_step(0);
        break;
    case 8:
        rpg_girl_wait_step(0x14);
        break;
    case 9:
        rpg_girl_action_end();
        rpg_girl_action = 0x40000000;
        break;
    }
}
