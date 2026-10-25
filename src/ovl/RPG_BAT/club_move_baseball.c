/* club_move_baseball.c: src/ovl/RPG_BAT/8015A1F0.c:4: club 6 special move; body 5: if (((s32) rpg_player_step >= 6) && ((s32) rpg_player_step < 9)) {; 6: rpg_se_play_latched_6xx(0x500, 0xE, 1); club-index dispatch src/ovl/RPG_BAT/8013CC90.c:96. Original object 0x8015A1F0-0x8015A9A0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

void rpg_club_move_baseball(void) {
    if (((s32) rpg_player_step >= 6) && ((s32) rpg_player_step < 9)) {
        rpg_se_play_latched_6xx(0x500, 0xE, 1);
    }
    switch (rpg_player_step) {
    case 0:
        strcpy(rpg_text_player_0, "　　　　　　超球百裂弾　　　　　　　");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A);
        rpg_pick_target();
        rpg_player_damage_roll();
        rpg_player_step_next();
        rpg_player_step_next();
    default:
        break;
    case 2:
        rpg_move_approach();
        break;
    case 3:
        rpg_actor_set_animation(0x34, 3, 5, 1);
        rpg_player_step_next();
        break;
    case 4:
        rpg_player_wait_animation_step(0x34);
        break;
    case 5:
        rpg_actor_set_animation(0x33, 6, 1, 1);
        rpg_actor_set_animation(0x34, 4, 1, 1);
        rpg_actor_set_animation(0x35, 5, 1, 1);
        rpg_player_step_next();
        break;
    case 6:
        rpg_actor_set_position(0x33, 0, (s32) (rpg_player_channel.unk_04 * -0x30 * rpg_player_channel.unk_04) / 3600);
        rpg_actor_set_position(0x34, 0, (s32) (rpg_player_channel.unk_04 * -0x30 * rpg_player_channel.unk_04) / 3600);
        rpg_actor_set_position(0x35, 0, (s32) (rpg_player_channel.unk_04 * -0x30 * rpg_player_channel.unk_04) / 3600);
        rpg_player_channel.unk_04 += 1;
        if (rpg_player_channel.unk_04 >= 0x3D) {
            rpg_actor_set_position(0x33, 0, -0x30);
            rpg_actor_set_position(0x34, 0, -0x30);
            rpg_actor_set_position(0x35, 0, -0x30);
            rpg_player_step_next();
        }
        break;
    case 7:
        rpg_baseball_step_7();
        break;
    case 8:
        rpg_baseball_step_8();
        rpg_player_channel.unk_04 += 1;
        if (rpg_player_channel.unk_04 >= 0x12D) {
            rpg_player_step_next();
        }
        break;
    case 9:
        rpg_baseball_step_9();
        if (rpg_player_channel.unk_04 == 0) {
            rpg_player_channel.unk_04 = 0x3C;
        }
        rpg_actor_set_position(0x33, 0, (rpg_player_channel.unk_04 * -0x30 * rpg_player_channel.unk_04) / 3600);
        rpg_actor_set_position(0x34, 0, (s32) (rpg_player_channel.unk_04 * -0x30 * rpg_player_channel.unk_04) / 3600);
        rpg_actor_set_position(0x35, 0, (s32) (rpg_player_channel.unk_04 * -0x30 * rpg_player_channel.unk_04) / 3600);
        rpg_player_channel.unk_04 -= 1;
        if (rpg_player_channel.unk_04 >= 0xF) {
            rpg_se_play_latched_6xx(0x500, 0xE, 1);
        }
        if (rpg_player_channel.unk_04 <= 0) {
            rpg_se_play_latched_6xx(0x502, 1, 0);
            rpg_apply_damage_to_targets();
            rpg_actor_set_position(0x33, 0, 0);
            rpg_actor_set_position(0x34, 0, 0);
            rpg_actor_set_position(0x35, 0, 0);
            rpg_player_step_next();
        }
        break;
    case 10:
        rpg_actor_set_animation(0x33, 0xFF, 1, 0);
        rpg_actor_set_animation(0x34, 8, 5, 1);
        rpg_actor_set_animation(0x35, 0xFF, 1, 0);
        rpg_player_step_next();
        break;
    case 11:
        rpg_player_wait_animation_step(0x34);
        break;
    case 12:
        rpg_actor_set_animation(0x34, 2, 1, 1);
        rpg_player_step_next();
        break;
    case 13:
        rpg_player_wait_step(0x14);
        break;
    case 14:
        rpg_move_return();
        break;
    case 15:
        rpg_player_action_end();
        break;
    }
}


void rpg_baseball_step_7(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        rpg_actor_set_animation_alt(i + 0x28, 7, 1, 1);
        rpg_actor_set_position(i + 0x28, -0x19, 0x109);
    }
    rpg_player_step_next();
}

/* FAKE: T-22013, two views of the displacement words: the scalars rpg_motion/rpg_motion_dy around the calls and
 * the RpgMoveMotion pointer m for the negation and the rect word. Only this mix gives the original's promoted
 * registers (dx in $a0, the dead copy of dy in $a1, the copy of unk_26 in $a0 before the div); real source unknown. */
void rpg_baseball_step_8(void) {
    Actor *a;
    RpgMoveMotion *m;
    s32 i;
    s32 r;

    m = (RpgMoveMotion *)&rpg_motion;
    a = (Actor *)actor_pool;
    for (i = 0; i < 8; i++) {
        if (a[i + 136].x < -0xB4 || a[i + 136].y >= 0x8D) {
            if (rpg_rand(5) == 0) {
                rpg_se_play_latched_6xx(0x501, 2, 2);
                rpg_actor_set_position(i + 0x28, 0xF0, 0x28);
                rpg_motion = rpg_rand(8) + 5;
                rpg_motion_dy = rpg_rand(rpg_motion);
                m->dx = -m->dx;
                a[i + 136].draw_flags = 0x94;
                a[i + 136].dx = m->dx;
                a[i + 136].dy = rpg_motion_dy;
            } else {
                rpg_actor_set_animation_alt(i + 0x28, 7, 1, 0);
            }
        }
        m->rect.param_4 = a[i + 136].x;
        r = ((0x70 - m->rect.param_4) << 12) / 240;
        a[i + 136].scale_x = r;
        a[i + 136].scale_y = r;
    }
}

/* FAKE: T-22013, x is reused as the offset of the first store. Reassigning x makes uopt keep the original's copy of the halfword (stored to D_8015EC68 from $a0) and compute i + 0x28 in the first block; real source unknown. p walks actor_pool as raw bytes: p + 0x2446/0x244A are Actor unk_26/unk_2A of record i + 136, and after p += 0x44 the stores at p + 0x23F6/0x23F8 are unk_1A/unk_1C of the same record. */
void rpg_baseball_step_9(void) {
    u8 *p;
    s32 i;
    s32 x;
    s32 r;
    s32 y;

    p = actor_pool;
    i = 0;
    do {
        x = *(s16 *)(p + 0x2446);
        if (x < -0xB4 || *(s16 *)(p + 0x244A) >= 0x8D) {
            rpg_actor_set_animation_alt(i + 0x28, 7, 1, 0);
            x = *(s16 *)(p + 0x2446);
        }
        i++;
        y = x;
        r = ((0x70 - x) << 12) / 240;
        p += 0x44;
        x = 0x23F6;
        *(s16 *)(p + x) = r;
        *(s16 *)(p + 0x23F8) = r;
        D_8015EC68 = y;
    } while (i != 8);
}
