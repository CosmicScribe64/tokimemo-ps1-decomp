/* level_clear_and_collisions.c: src/ovl/TT/80144D70.c:6: stage-clear transition and asset requests plus smart-bomb and overlap/collision pass; body 16: scene = tt_sys; 17: g = (TtCore *)tt_core;. Original object 0x80144D70-0x80145E60 (config/objects/TT.txt). */
#define MAIN_API_OVERRIDE_bzero /* implicit int declaration, as matched: with the void prototype the reload of ctl after the call in tt_state_stage_clear takes v0, not v1. T-11840 */
#include "common.h"
#include "ovl/TT.h"

/* T-11840: matched. r is the selector of both switches and holds the cd_done_count result; with pool assigned again in case 0xA0 this raises r above pool in the uopt order, so the selectors take v1 and pool a3. The head order (scene, g, ctl, pool, stream, buttons, list) sets the ugen temporary order. */
void tt_state_stage_clear(void) {
    u8 *scene;
    TtCore *core;
    u8 *ctl;
    s32 *stream;
    TtPadView *buttons;
    TtText *pool;
    u8 *list;
    u32 r;

    scene = tt_sys;
    core = (TtCore *)tt_core;
    ctl = tt_run;
    pool = (TtText *)(tt_texts + 0x140);
    stream = tt_stage_script;
    buttons = (TtPadView *)(scene + 0x10);
    list = tt_sprites + 0xC80;
    r = core->sub_state;
    switch (r) {
    case 0x0:
        audio_command(0x22);
        audio_command(0x2F);
        tt_air_enemies_clear();
        tt_ground_targets_clear();
        tt_enemy_shots_clear();
        core->pause_allowed = 0;
        if (ctl[0xB] < 3) {
            audio_command(4);
            audio_command(0x202);
            r = ctl[0xB];
            switch (r) {
            case 0:
                *(s16 *)(scene + 0xA4) = 1;
                tt_cd_read_start(0x80016000, 0xC869, 0x2420);
                tt_cd_read_start(0x8002E800, 0xC817, 0x69E4);
                tt_cd_read_start(0x8019A000, 0xC81A, 0x27D60);
                break;
            case 1:
                *(s16 *)(scene + 0xA4) = 2;
                tt_cd_read_start(0x80016000, 0xC8BD, 0x2420);
                tt_cd_read_start(0x8002E800, 0xC86D, 0x69E4);
                tt_cd_read_start(0x8019A000, 0xC871, 0x27D60);
                break;
            case 2:
                *(s16 *)(scene + 0xA4) = 3;
                tt_cd_read_start(0x80016000, 0xC916, 0x2420);
                tt_cd_read_start(0x8002E800, 0xC8C1, 0x69E4);
                tt_cd_read_start(0x8019A000, 0xC8C6, 0x27D60);
                break;
            }
            tt_cd_read_start(0x8001A000, 0xBFD1, 0xE20);
            tt_cd_read_start(0x801C2000, 0xBF9C, 0x1A680);
            pool->x = -0x38;
        } else {
            audio_command(4);
            audio_command(0x200);
            pool->x = -0x20;
        }
        pool->y = 0x74;
        pool->text = 0xB;
        pool->palette = 4;
        pool->active = 1;
        pool->flags = 0;
        pool->layer = 0;
        pool++;
        if (ctl[0xB] < 3) {
            pool->x = 8;
        } else {
            pool->x = 0x10;
        }
        pool->y = 0x74;
        pool->text = 0xC;
        pool->palette = 4;
        pool->active = 1;
        pool->flags = 0;
        pool->layer = 0;
        pool++;
        if (ctl[0xB] < 3) {
            list[0] = 1;
            list[1] = 0;
            *(s16 *)(list + 0x44) = 0;
            *(s16 *)(list + 0x14) = -0xC;
            *(s16 *)(list + 0x16) = 0x6C;
            *(s16 *)(list + 0x3C) = 0x10;
            *(s16 *)(list + 0x3E) = 0x10;
            list[0x46] = ctl[0xB] * 0x10 + 0x10;
            list[0x47] = 0x20;
            *(s16 *)(list + 0x40) = 0xA;
            *(s16 *)(list + 0x42) = 0x3C08;
            list[0x3B] = 0x65;
            *(s16 *)(list + 0x10) = 0;
        } else {
            pool->x = -0x40;
            pool->y = 0x74;
            pool->text = 0xA;
            pool->palette = 4;
            pool->active = 1;
            pool->flags = 0;
            pool->layer = 0;
        }
        *(u16 *)&core->fade_level = 0x80;
        core->fade_r = 0x80;
        core->fade_g = 0x80;
        core->fade_b = 0x80;
        core->bg_mode = 3;
        *(s32 *)(ctl + 4) = 0;
        tt_bomb_targets_clear();
        core->sub_state = 0x40;
        break;
    case 0x40:
        if ((*(u16 *)&core->fade_level -= 4) == 0) {
            *(s32 *)ctl = 0;
            core->timer = 0;
            core->sub_state = 0x50;
        }
        core->fade_r = *(u16 *)&core->fade_level;
        core->fade_g = *(u16 *)&core->fade_level;
        core->fade_b = *(u16 *)&core->fade_level;
        if (*(s32 *)(ctl + 4) < *(s32 *)ctl) {
            *(s32 *)ctl -= 0x400;
        } else {
            *(s32 *)ctl += 0x200;
        }
        if ((*(s32 *)ctl - *(s32 *)(ctl + 4) < 0 ? -(*(s32 *)ctl - *(s32 *)(ctl + 4)) : *(s32 *)ctl - *(s32 *)(ctl + 4)) < 0x1000) {
            *(s32 *)ctl = *(s32 *)(ctl + 4);
        }
        break;
    case 0x50:
        if (tt_run[0xB] < 3) {
            if (((TtCore *)tt_core)->timer >= 0x191) {
                core->sub_state = 0xA0;
            } else {
                core->timer++;
            }
        } else if (((TtCore *)tt_core)->timer >= 0x1AF) {
            core->sub_state = 0xA0;
        } else {
            core->timer++;
        }
        if (core->autoplay == 0 && (buttons->pressed & 0x820)) {
            core->sub_state = 0xA0;
        }
        break;
    case 0xA0:
        if (tt_run[0xB] < 3) {
            r = cd_done_count();
            if ((s32)((r - tt_cd_read_counter_before() + 0x100) & 0xFF) < 5) {
                break;
            }
        }
        pool = (TtText *)(tt_texts + 0x140);
        pool->active = 0;
        pool++;
        pool->active = 0;
        list[0] = 0;
        bzero(stream, 0x20);
        if ((u8)++ctl[0xB] < 4) {
            core->state = 2;
            core->sub_state = 1;
        } else {
            ctl[0xF] = 1;
            if (core->autoplay != 0) {
                core->state = 8;
            } else {
                core->state = 6;
            }
            core->phase = 0;
            core->sub_step = 0;
            core->sub_state = 0;
            core->flag_b = 0;
            core->flag_a = 0;
            core->request_next = 0;
        }
        break;
    }
}

/* T-11800: matched. T-10820 dead compare locals reserve a0 in both loops; d2, assigned before the first loop and tested by an empty if inside it (FAKE), also reserves a1 there, so the hoisted 0x40/0x70 take a2/a3 as in the original. */
void tt_smart_bomb(void) {
    s32 i;
    u8 *p;
    int d1;
    int d2;

    i = 0;
    p = tt_air_enemies;
    d2 = (int)tt_enemy_shots; /* FAKE: with the empty if (d2) {} below, d2 lives through the first loop and reserves a1, so the hoisted 0x70 takes a3 as in the original; no code is emitted. T-11800 */
    for (; i < 0x40; i++) {
        /* FAKE: dead compare locals with an else branch (decomp-permuter); they only reserve registers so that the hoisted constants sit in the original's registers. T-10820 */
        if (p[0] != 0 && (d1 = (*(u16 *)(p + 4) == 0x40))) {
            *(u16 *)(p + 4) = 0x70;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            if (d2) { /* FAKE: see d2 above. T-11800 */
            }
            p[0xE] = 0;
            p[0xD] = 0;
        } else {
            d1 = 0;
        }
        p += 0x78;
    }
    i = 0;
    p = tt_enemy_shots;
    for (; i < 0x80; i++) {
        if (p[0] != 0 && (d2 = (*(u16 *)(p + 4) == 0x40))) {
            p[0] = 0;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = (*(u16 *)(p + 4) == 0x40) * 0;
            p[0xD] = 0;
        } else {
            d2 = 0;
        }
        p += 0x68;
    }
}

void tt_pickup_maximum_shot(u8 *arg0) {
    tt_player[0x55] = 0xA;
    tt_bell_popup_spawn(arg0 + 0x14);
}

s32 tt_box_overlap(u8 *position_a, u8 *hitbox_a, u8 *position_b, u8 *hitbox_b) {
    s16 x1 = *(s16 *)hitbox_a + *(s16 *)position_a;
    s16 y1 = *(s16 *)(hitbox_a + 2) + *(s16 *)(position_a + 2);
    s16 x2 = *(s16 *)hitbox_b + *(s16 *)position_b;
    s16 y2 = *(s16 *)(hitbox_b + 2) + *(s16 *)(position_b + 2);
    s32 dx = x1 - x2;
    s32 dy;
    s32 absolute_delta;

    if (dx < 0) {
        absolute_delta = -dx;
    } else {
        absolute_delta = dx;
    }
    dy = y1 - y2;
    if ((u32)absolute_delta < (u32)(hitbox_b[4] + hitbox_a[4])) {
        if (dy < 0) {
            absolute_delta = -dy;
        } else {
            absolute_delta = dy;
        }
        if ((u32)absolute_delta < (u32)(hitbox_b[5] + hitbox_a[5])) {
            return 1;
        }
    }
    return 0;
}

void tt_collisions_update(void) {
    u8 *core;
    u8 *player;
    u8 *p;
    u8 *q;
    s32 j;
    u8 *e;
    u8 *o;
    u8 *ground_target;
    TtHitbox box;
    s32 i;

    core = tt_core;
    p = tt_air_enemies;
    player = tt_player;
    for (i = 0; i < 0x40; i++) {
        if (p[0] != 0 && *(u16 *)(p + 4) == 0x40 && (*(u16 *)(p + 2) != 0x47 || *(u16 *)(p + 6) >= 2)
            && tt_box_overlap(player + 0x14, (u8 *)tt_hitboxes, p + 0x14, (u8 *)&tt_hitboxes[2])) {
            if (*(u16 *)(player + 4) < 0x60) {
                tt_air_enemy_explode_init(p);
                *(u16 *)(p + 4) = 0x71;
                /* FAKE: TT zero-store chain pair; dead reloads put the state in a1. T-11860 */
                *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 0xA) = 0;
                p[0xD] = p[0xE] = p[0xF] = 0;
            }
            if (player[0x52] == 0) {
                tt_player_hit();
                *(u16 *)(player + 4) = 0x60;
                break;
            }
        }
        p += 0x78;
    }
    e = tt_enemy_shots;
    for (i = 0; i < 0x80; i++) {
        if (e[0] != 0 && (*(u16 *)(e + 4) == 0x40 || *(u16 *)(e + 4) == 0x50)) {
            if (*(u16 *)(e + 2) == 0xAD) {
                switch (*(u16 *)(e + 4)) {
                case 0x40:
                    box.x = 0;
                    box.y = *(u16 *)(e + 0x62) >> 1;
                    box.half_w = 2;
                    box.half_h = *(u16 *)(e + 0x62) >> 1;
                    break;
                case 0x50:
                    box.x = 0;
                    box.y = 0x10;
                    box.half_w = 2;
                    box.half_h = 0x10;
                    break;
                }
                *(TtHitbox **)(e + 0x4C) = &box;
            }
            if (tt_box_overlap(player + 0x14, (u8 *)tt_hitboxes, e + 0x14, *(u8 **)(e + 0x4C))) {
                if (*(u16 *)(player + 4) < 0x60) {
                    e[0] = 0;
                }
                if (player[0x52] == 0) {
                    tt_player_hit();
                    *(u16 *)(player + 4) = 0x60;
                    /* FAKE: TT zero-store chain pair; dead reloads put the state in a1. T-11860 */
                    *(u16 *)(player + 6) = *(u16 *)(player + 8) = *(u16 *)(player + 0xA) = 0;
                    player[0xD] = player[0xE] = player[0xF] = 0;
                    break;
                }
            }
        }
        e += 0x68;
    }
    if (*(u16 *)(player + 4) == 0x30 || *(u16 *)(player + 4) == 0x40) {
        o = tt_bells;
        for (i = 0; i < 0x20; i++) {
            if (o[0] != 0 && tt_box_overlap(player + 0x14, (u8 *)tt_hitboxes, o + 0x14, (u8 *)&tt_hitboxes[4])) {
                tt_bell_collect(o);
            }
            o += 0x6C;
        }
    }
    ground_target = tt_ground_targets;
    for (i = 0; i < 0x20; i++) {
        if (ground_target[0] != 0 && *(u16 *)(ground_target + 4) == 0x7A
            && tt_box_overlap(player + 0x14, (u8 *)tt_hitboxes, ground_target + 0x14, (u8 *)&tt_hitboxes[4])) {
            if (*(u16 *)(ground_target + 2) == 0x6E) {
                tt_se_request(0x60F);
                ground_target[0] = 0;
                tt_smart_bomb();
            } else if (*(u16 *)(ground_target + 2) == 0x6F) {
                ground_target[0] = 0;
                tt_pickup_maximum_shot(ground_target);
            }
        }
        ground_target += 0x78;
    }
    if (player[0x53] != 0) {
        q = tt_options;
        for (i = 0; i < 3; i++) {
            if (q[0] != 0) {
                p = tt_air_enemies;
                for (j = 0; j < 0x40; j++) {
                    if (p[0] != 0 && *(u16 *)(p + 4) == 0x40 && (*(u16 *)(p + 2) != 0x47 || *(u16 *)(p + 6) >= 2)
                        && tt_box_overlap(q + 0x14, (u8 *)&tt_hitbox_shot, p + 0x14, (u8 *)&tt_hitboxes[2])) {
                        tt_air_enemy_explode_init(p);
                        *(u16 *)(p + 4) = 0x71;
                        /* FAKE: TT zero-store chain pair; dead reloads put the state in a1. T-11860 */
                        *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 0xA) = 0;
                        p[0xD] = p[0xE] = p[0xF] = 0;
                    }
                    p += 0x78;
                }
                e = tt_enemy_shots;
                for (j = 0; j < 0x80; j++, e += 0x68) {
                    if (e[0] == 0) {
                        continue;
                    }
                    if (*(u16 *)(e + 4) != 0x40 && *(u16 *)(e + 4) != 0x50) {
                        continue;
                    }
                    if (*(u16 *)(e + 2) == 0xAB) {
                        continue;
                    }
                    if (*(u16 *)(e + 2) == 0xAD) {
                        continue;
                    }
                    if (tt_box_overlap(q + 0x14, (u8 *)tt_hitboxes, e + 0x14, *(u8 **)(e + 0x4C))) {
                        e[0] = 0;
                    }
                }
            }
            q += 0xA4;
        }
    }
    q = tt_shots;
    for (i = 0; i < 0xC; i++) {
        if (q[0] != 0 && *(u16 *)(q + 4) == 0x40) {
            p = tt_air_enemies;
            for (j = 0; j < 0x40; j++) {
                if (p[0] != 0 && *(u16 *)(p + 4) == 0x40 && (*(u16 *)(p + 2) != 0x47 || *(u16 *)(p + 6) >= 2)
                    && tt_box_overlap(q + 0x14, (u8 *)&tt_hitbox_shot, p + 0x14, (u8 *)&tt_hitboxes[2])) {
                    core[0x25]++;
                    if (core[0x24] != 0 && (core[0x25] & 1)) {
                        if (core[0x25] & 2) {
                            tt_enemy_shot_fire_fan4_aimed(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16), 0x100, 0x20, 0xA0);
                        } else {
                            tt_enemy_shot_fire_3way_aimed(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16), 0x100, 0x20, 0xA0);
                        }
                    }
                    *(u16 *)(p + 4) = 0x70;
                    /* FAKE: TT zero-store chain pair; dead reloads put the state in a1. T-11860 */
                    *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 0xA) = 0;
                    p[0xD] = p[0xE] = p[0xF] = 0;
                    *(u16 *)(q + 4) = 0x80;
                    q[0xD] = 0;
                    q[0xE] = 0;
                    q[0xF] = 0;
                    *(s16 *)(q + 6) = 0;
                    *(s16 *)(q + 8) = 0;
                    *(s16 *)(q + 0xA) = 0;
                }
                p += 0x78;
            }
            e = tt_enemy_shots;
            for (j = 0; j < 0x80; j++) {
                if (e[0] != 0 && (*(u16 *)(e + 4) == 0x40 || *(u16 *)(e + 4) == 0x50)) {
                    switch (*(u16 *)(e + 2)) {
                    case 0xA2:
                    case 0xA9:
                    case 0xAA:
                        if (tt_box_overlap(q + 0x14, (u8 *)&tt_hitbox_shot, e + 0x14, *(u8 **)(e + 0x4C))) {
                            tt_se_request(0x509);
                            e[0] = 0;
                            *(u16 *)(q + 4) = 0x80;
                            q[0xD] = 0;
                            q[0xE] = 0;
                            q[0xF] = 0;
                            *(s16 *)(q + 6) = 0;
                            *(s16 *)(q + 8) = 0;
                            *(s16 *)(q + 0xA) = 0;
                            goto done;
                        }
                        break;
                    case 0xAC:
                        if (tt_box_overlap(q + 0x14, (u8 *)&tt_hitbox_shot, e + 0x14, *(u8 **)(e + 0x4C))) {
                            core[0x25]++;
                            if (core[0x24] != 0 && (core[0x25] & 1)) {
                                tt_enemy_shot_fire_aimed(*(s16 *)(e + 0x14), *(s16 *)(e + 0x16), 0x20, 0xA0);
                            }
                            e[0] = 0;
                            tt_air_enemy_explosion_spawn(e + 0x14);
                            *(u16 *)(q + 4) = 0x80;
                            /* FAKE: TT zero-store chain pair; dead reloads put the state in a1. T-11860 */
                            *(u16 *)(q + 6) = *(u16 *)(q + 8) = *(u16 *)(q + 0xA) = 0;
                            q[0xD] = q[0xE] = q[0xF] = 0;
                        }
                        break;
                    }
                }
                e += 0x68;
            }
        done:
            if (*(u16 *)(player + 4) == 0x30 || *(u16 *)(player + 4) == 0x40) {
                o = tt_bells;
                for (j = 0; j < 0x20; j++) {
                    if (o[0] != 0 && *(u16 *)(o + 4) < 0x90
                        && tt_box_overlap(q + 0x14, (u8 *)&tt_hitbox_shot, o + 0x14, (u8 *)&tt_hitboxes[4])) {
                        tt_bell_hit_by_shot(o, *(s16 *)(q + 0x14) - *(s16 *)(o + 0x14));
                        *(u16 *)(q + 4) = 0x80;
                        /* FAKE: TT zero-store chain pair; dead reloads put the state in a1. T-11860 */
                        *(u16 *)(q + 6) = *(u16 *)(q + 8) = *(u16 *)(q + 0xA) = 0;
                        q[0xD] = q[0xE] = q[0xF] = 0;
                    }
                    o += 0x6C;
                }
            }
        }
        q += 0x54;
    }
}
