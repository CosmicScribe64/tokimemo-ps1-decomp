/* girl_action_shiori.c: src/ovl/RPG_BAT/80142000.c:27: girl 0 action; body 28: switch (rpg_girl_channel.unk_04) {; 30: rpg_girl_slide_in(2, 0); partner-id dispatch src/ovl/RPG_BAT/partner_scheduler_and_motion.c:24. Original object 0x80142000-0x801421E0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_shiori_stop_it(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_girl_0, "藤崎「やめてっ！");
        rpg_message_show(rpg_text_girl_pointers, 1, 0x64);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x1F00001E, 0x07000020, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_actor_set_animation(0x1D, 0, 1, 1);
        rpg_voice_reset();
        rpg_girl_step_next();
        return;
    }
}

void rpg_girl_action_shiori(void) {
    switch (rpg_girl_channel.unk_04) {
    case 0:
        rpg_girl_slide_in(2, 0);
        break;
    case 1:
        rpg_girl_wait_step(0x14);
        break;
    case 2:
        rpg_actor_set_animation(0x1D, 5, 1, 1);
        rpg_girl_step_next();
        break;
    case 3:
        rpg_dialog_shiori_stop_it();
        break;
    case 4:
        rpg_actor_set_animation(0x1D, 0, 1, 1);
        rpg_girl_wait_step(0x28);
        break;
    case 5:
        rpg_girl_slide_out(3, 0);
        break;
    case 6:
        rpg_girl_wait_step(0x14);
        break;
    case 7:
        rpg_girl_action_end();
        break;
    }
}
