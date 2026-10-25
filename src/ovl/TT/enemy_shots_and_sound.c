/* enemy_shots_and_sound.c: src/ovl/TT/80133970.c:7: sound bank loads and latched requests plus enemy shot pool, aim/fan/beam patterns and drawing; body 11: audio_command(0x7F); 12: func_800451E0(0x80162000);. Original object 0x80133970-0x80135360 (config/objects/TT.txt). */
#define MAIN_API_OVERRIDE_audio_command /* unprototyped call preserves u16 argument conversion */
#include "common.h"
#include "ovl/TT.h"

s32 audio_command();

void tt_sound_banks_load(void) {
    s32 i;
    s32 j;

    audio_command(0x7F);
    func_800451E0(0x80162000);
    audio_command(0x11);
    audio_command(0x22);
    audio_command(0x21);
    audio_command(0x2F);
    i = 0;
    do {
        audio_command_pump();
        func_800AD950(0);
    } while (i++ < 0x10000 && func_80044F94(0) == 0);
    audio_command(0x7F);
    func_800451E0(0x8019A000);
    audio_command(0x12);
    audio_command(0x23);
    audio_command(0x24);
    audio_command(0x2E);
    j = 0;
    do {
        audio_command_pump();
        func_800AD950(0);
    } while (j++ < 0x10000 && func_80044F94(1) == 0);
}

void tt_sound_bank_stage_reload(void) {
    s32 i;

    audio_command_pump();
    func_800451E0(0x8019A000);
    audio_command(0x12);
    i = 0;
    do {
        audio_command_pump();
        func_800AD950(0);
    } while (i++ < 0x10000 && func_80044F94(1) == 0);
}

void tt_se_request(sound_command)
u16 sound_command;
{
    TtSysView *data;
    u16 index;
    data = (TtSysView *)tt_sys;
    index = sound_command & 0x1F;
    index |= (sound_command & 0x100) >> 3;
    if (data->se_flags[index] == 0) {
        audio_command(sound_command);
        data->se_flags[index] = 1;
    }
}


void tt_se_flags_clear(void) {
    u8 *sys = tt_sys;
    s32 i;

    for (i = 0; i < 0x40; i++) {
        sys[0x56 + i] = 0;
    }
}

void tt_enemy_shots_clear(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = tt_enemy_shots;
    for (; i < 0x80; i++) {
        *p = 0;
        p += 0x68;
    }
}

u8 *tt_enemy_shot_find_free(u8 *cursor, u8 *end) {
    while (cursor < end) {
        if (*cursor == 0) {
            return cursor;
        }
        cursor += 0x68;
    }
    return 0;
}

/* The caller consumes the angle left in v0 by func_8014D260.
 * Keep the fallthrough return: an explicit return through its void API
 * is unavailable, and this definition matches the original helper. T-10020. */
s32 tt_enemy_shot_aim_angle(u8 *arg0, u8 *arg1) {
    s32 dx = *(s32 *)(arg1 + 0x20) - *(s32 *)(arg0 + 0x20);
    s32 dy = *(s32 *)(arg1 + 0x24) - *(s32 *)(arg0 + 0x24);

    func_8014D260(dy >> 16, dx >> 16);
}

void tt_enemy_shot_setup(TtEnemyShot *o, u16 kind, s32 idx) {
    o->active = 1;
    o->kind = kind;
    o->layer = 1;
    o->animation_data = (void *)(&tt_animation_lists)[idx];
    o->animation_flags = 0x80;
    switch (idx) {
    case 18:
    case 19:
        *(s16 *)((u8 *)o + 0x52) = 0xA;
        *(s16 *)((u8 *)o + 0x54) = 0x3C8B;
        o->hitbox = (u8 *)&tt_hitboxes[3];
        return;
    case 20:
        *(s16 *)((u8 *)o + 0x52) = 0xA;
        *(s16 *)((u8 *)o + 0x54) = 0x3C8C;
        o->hitbox = (u8 *)&tt_hitboxes[3];
        return;
    case 21:
        *(s16 *)((u8 *)o + 0x52) = 0x12;
        *(s16 *)((u8 *)o + 0x54) = 0x3CD3;
        o->hitbox = (u8 *)&tt_hitboxes[3];
        return;
    case 22:
        *(s16 *)((u8 *)o + 0x52) = 0x12;
        *(s16 *)((u8 *)o + 0x54) = 0x3CD4;
        o->hitbox = (u8 *)&tt_hitboxes[2];
        return;
    case 23:
        *(s16 *)((u8 *)o + 0x52) = 0x12;
        *(s16 *)((u8 *)o + 0x54) = 0x3CD5;
        o->hitbox = (u8 *)&tt_hitboxes[2];
    }
}

void tt_enemy_shot_fire_aimed(s16 x, s16 y, s32 speed, u16 kind) {
    TtEnemyShot *object;
    u8 *source;
    s32 angle;

    source = tt_player;
    object = (TtEnemyShot *)tt_enemy_shot_find_free(tt_enemy_shots, tt_enemy_shots + 0x3400);
    if (object != 0) {
        object->position_x = x << 16;
        object->position_y = y << 16;
        angle = tt_enemy_shot_aim_angle((u8 *)object, source);
        object->velocity_x = func_800A0140(angle) * speed;
        object->velocity_y = rsin(angle) * speed;
        switch (kind) {
        case 0xA0:
            tt_enemy_shot_setup(object, 0xA0, 0x12);
            break;
        case 0xA8:
            tt_enemy_shot_setup(object, 0xA8, 0x15);
            break;
        }
        object->state = 0x40;
        object->step2 = 0;
        object->step = 0;
        object->phase = 0;
        object->tick2 = 0;
        object->tick1 = 0;
        object->tick0 = 0;
    }
}

void tt_enemy_shot_fire_angle(s16 x, s16 y, s32 speed, s32 angle, u16 kind) {
    TtEnemyShot *object;

    object = (TtEnemyShot *)tt_enemy_shot_find_free(tt_enemy_shots, tt_enemy_shots + 0x3400);
    if (object != 0) {
        object->position_x = x << 16;
        object->position_y = y << 16;
        object->velocity_x = func_800A0140(angle) * speed;
        object->velocity_y = rsin(angle) * speed;
        switch (kind) {
        case 0xA0:
            tt_enemy_shot_setup(object, 0xA0, 0x12);
            break;
        case 0xA1:
            tt_enemy_shot_setup(object, 0xA1, 0x13);
            break;
        case 0xA8:
            tt_enemy_shot_setup(object, 0xA8, 0x15);
            break;
        case 0xA9:
            tt_enemy_shot_setup(object, 0xA9, 0x16);
            break;
        case 0xAC:
            object->heading = angle + 0x800;
            object->homing_age = 0;
            tt_enemy_shot_setup(object, 0xAC, 0x14);
            object->hitbox = (u8 *)&tt_hitboxes[2];
            break;
        }
        object->state = 0x40;
        object->step2 = 0;
        object->step = 0;
        object->phase = 0;
        object->tick2 = 0;
        object->tick1 = 0;
        object->tick0 = 0;
    }
}

void tt_enemy_shot_fire_3way_aimed(s16 x, s16 y, s32 spread, s32 speed, u16 kind) {
    TtEnemyShot *object;
    u8 *source;
    s32 angle;
    s32 i;

    object = (TtEnemyShot *)tt_enemy_shots;
    source = tt_player;
    for (i = 0; i < 3; i++) {
        object = (TtEnemyShot *)tt_enemy_shot_find_free((u8 *)object, (u8 *)object + 0x3400);
        if (object == 0) {
            break;
        }
        object->position_x = x << 16;
        object->position_y = y << 16;
        angle = tt_enemy_shot_aim_angle((u8 *)object, source);
        switch (i) {
        case 1:
            angle = angle - spread;
            break;
        case 2:
            angle = angle + spread;
            break;
        }
        object->velocity_x = func_800A0140(angle) * speed;
        object->velocity_y = rsin(angle) * speed;
        switch (kind) {
        case 0xA0:
            tt_enemy_shot_setup(object, 0xA0, 0x12);
            break;
        case 0xA2:
            tt_enemy_shot_setup(object, 0xA2, 0x14);
            break;
        case 0xA8:
            tt_enemy_shot_setup(object, 0xA8, 0x15);
            break;
        case 0xAA:
            tt_enemy_shot_setup(object, 0xAA, 0x17);
            break;
        }
        object->state = 0x40;
        object->step2 = 0;
        object->step = 0;
        object->phase = 0;
        object->tick2 = 0;
        object->tick1 = 0;
        object->tick0 = 0;
    }
}

void tt_enemy_shot_fire_3way_down(s16 x, s16 y, s32 spread, s32 speed, u16 kind) {
    TtEnemyShot *object;
    s32 angle;
    s32 i;

    object = (TtEnemyShot *)tt_enemy_shots;
    for (i = 0; i < 3; i++) {
        object = (TtEnemyShot *)tt_enemy_shot_find_free((u8 *)object, (u8 *)object + 0x3400);
        if (object == 0) {
            break;
        }
        object->position_x = x << 16;
        object->position_y = y << 16;
        angle = 0x400;
        switch (i) {
        case 1:
            angle = 0x400 - spread;
            break;
        case 2:
            angle = spread + 0x400;
            break;
        }
        object->velocity_x = func_800A0140(angle) * speed;
        object->velocity_y = rsin(angle) * speed;
        switch (kind) {
        case 0xA8:
            tt_enemy_shot_setup(object, 0xA8, 0x15);
            break;
        }
        object->state = 0x40;
        object->step2 = 0;
        object->step = 0;
        object->phase = 0;
        object->tick2 = 0;
        object->tick1 = 0;
        object->tick0 = 0;
    }
}

void tt_enemy_shot_fire_fan4_aimed(s16 x, s16 y, s32 spread, s32 speed, u16 kind) {
    TtEnemyShot *object;
    u8 *source;
    s32 half;
    s32 angle;
    s32 i;

    object = (TtEnemyShot *)tt_enemy_shots;
    i = 0;
    source = tt_player;
    for (;;) {
        object = (TtEnemyShot *)tt_enemy_shot_find_free((u8 *)object, (u8 *)object + 0x3400);
        if (object == 0) {
            break;
        }
        object->position_x = x << 16;
        object->position_y = y << 16;
        half = spread / 2;
        angle = tt_enemy_shot_aim_angle((u8 *)object, source);
        if (i & 1) {
            angle = (angle + half) - ((i / 2 + 1) * spread);
        } else {
            angle = angle + half + (i / 2) * spread;
        }
        object->velocity_x = func_800A0140(angle) * speed;
        object->velocity_y = rsin(angle) * speed;
        if (kind == 0xA0) {
            tt_enemy_shot_setup(object, 0xA0, 0x12);
        }
        i++;
        object->state = 0x40;
        object->step2 = 0;
        object->step = 0;
        object->phase = 0;
        object->tick2 = 0;
        object->tick1 = 0;
        object->tick0 = 0;
        if (i == 4) {
            break;
        }
    }
}

void tt_enemy_shot_fire_fan9_aimed(s16 x, s16 y, s32 spread, s32 speed, u16 kind) {
    TtEnemyShot *object;
    u8 *source;
    s32 angle;
    s32 i;

    object = (TtEnemyShot *)tt_enemy_shots;
    source = tt_player;
    for (i = 0; i < 9; i++) {
        object = (TtEnemyShot *)tt_enemy_shot_find_free((u8 *)object, (u8 *)object + 0x3400);
        if (object == 0) {
            break;
        }
        object->position_x = x << 16;
        object->position_y = y << 16;
        angle = tt_enemy_shot_aim_angle((u8 *)object, source);
        if (i != 0) {
            if (i & 1) {
                angle -= (i / 2 + 1) * spread;
            } else {
                angle += (i / 2) * spread;
            }
        }
        object->velocity_x = func_800A0140(angle) * speed;
        object->velocity_y = rsin(angle) * speed;
        switch (kind) {
        case 0xA0:
            tt_enemy_shot_setup(object, 0xA0, 0x12);
            break;
        case 0xAA:
            tt_enemy_shot_setup(object, 0xAA, 0x17);
            break;
        }
        object->state = 0x40;
        object->step2 = 0;
        object->step = 0;
        object->phase = 0;
        object->tick2 = 0;
        object->tick1 = 0;
        object->tick0 = 0;
    }
}

void tt_enemy_shot_fire_twin_down(s16 x, s16 y, s32 speed, u16 kind) {
    TtEnemyShot *object;
    s32 i;

    object = (TtEnemyShot *)tt_enemy_shots;
    for (i = 0; i < 2; i++) {
        object = (TtEnemyShot *)tt_enemy_shot_find_free((u8 *)object, (u8 *)object + 0x3400);
        if (object == 0) {
            break;
        }
        object->position_x = ((x - i * 0x30) + 0x18) << 16;
        object->position_y = y << 16;
        object->velocity_x = 0;
        object->velocity_y = speed << 16;
        switch (kind) {
        case 0xA0:
            tt_enemy_shot_setup(object, 0xA0, 0x12);
            break;
        case 0xA2:
            tt_enemy_shot_setup(object, 0xA2, 0x14);
            break;
        }
        object->state = 0x40;
        object->step2 = 0;
        object->step = 0;
        object->phase = 0;
        object->tick2 = 0;
        object->tick1 = 0;
        object->tick0 = 0;
    }
}

void tt_enemy_shot_fire_animated(s16 x, s16 y, s32 speed, s32 angle) {
    TtEnemyShot *object;

    object = (TtEnemyShot *)tt_enemy_shot_find_free(tt_enemy_shots, tt_enemy_shots + 0x3400);
    if (object != 0) {
        object->active = 1;
        object->kind = 0xAB;
        object->state = 0x40;
        /* FAKE: u16 view of the s16 fields; the u16 chain reloads 0 into a0 and keeps a dead range in a1, so 1 lands in a2 as in the original. T-11850 */
        *(u16 *)&object->phase = *(u16 *)&object->step = *(u16 *)&object->step2 = 0;
        object->tick0 = object->tick1 = object->tick2 = 0;
        object->layer = 1;
        object->position_x = x << 16;
        object->position_y = y << 16;
        object->velocity_x = func_800A0140(angle) * speed;
        object->velocity_y = rsin(angle) * speed;
        object->animation_data = tt_sprite_set_table;
        object->flip = 0;
        object->animation_set = 9;
        object->animation_frame = 0x14;
        object->animation_flags = 0x60;
        object->animation_hold = 0;
        object->hitbox = (u8 *)&tt_hitboxes[5];
    }
}

void tt_enemy_shot_fire_beam(x, y)
s16 x;
s16 y;
{
    TtEnemyShot *object;
    u8 *source;

    source = tt_boss;
    object = (TtEnemyShot *)tt_enemy_shot_find_free(tt_enemy_shots, tt_enemy_shots + 0x3400);
    if (object != 0) {
        object->active = 1;
        object->kind = 0xAD;
        object->state = 0x20;
        /* FAKE: u16 view of the s16 fields; the u16 and byte chains keep dead ranges in v1/a0 (no code), so the shared 1 lands in a1 as in the original. T-11850 */
        *(u16 *)&object->phase = *(u16 *)&object->step = *(u16 *)&object->step2 = 0;
        object->tick0 = object->tick1 = object->tick2 = 0;
        object->layer = 1;
        object->position_x = x << 16;
        object->position_y = y << 16;
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->heading = *(s16 *)(source + 0x14);
        object->homing_age = 0;
        object->hitbox = (u8 *)&tt_hitboxes[5];
    }
}

void tt_enemy_shot_move(u8 *arg0) {
    s16 x;
    s16 y;

    *(s32 *)(arg0 + 0x20) += *(s32 *)(arg0 + 0x28);
    *(s32 *)(arg0 + 0x24) += *(s32 *)(arg0 + 0x2C);
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
    x = *(s16 *)(arg0 + 0x16);
    if (x >= 0x101 || x < -0x10 || (y = *(s16 *)(arg0 + 0x14)) < -0x88 || y >= 0x89) {
        *(s16 *)(arg0 + 4) = 0;
        *(s16 *)(arg0 + 0xA) = 0;
        *(s16 *)(arg0 + 8) = 0;
        *(s16 *)(arg0 + 6) = 0;
        arg0[0xF] = 0;
        arg0[0xE] = 0;
        arg0[0xD] = 0;
        arg0[0] = 0;
    }
}

void tt_enemy_shot_home(TtEnemyShot *p) {
    u8 *target;
    s32 angle;
    s32 v;

    target = tt_player;
    v = p->heading;
    if (p->homing_age < 0x100) {
        p->homing_age = p->homing_age + 1;
    } else {
        return;
    }
    angle = func_8014D260((*(s32 *)(target + 0x24) - p->position_y) >> 16,
                          (*(s32 *)(target + 0x20) - p->position_x) >> 16);
    if (v < 0x800) {
        if (v < ((angle + 0x800) & 0xFFF) && ((angle + 0x800) & 0xFFF) < v + 0x800) {
            v += 0x18;
        } else {
            v -= 0x18;
        }
    }
    if (v >= 0x800) {
        if (((angle + 0x800) & 0xFFF) < v && v - 0x800 < ((angle + 0x800) & 0xFFF)) {
            v -= 0x18;
        } else {
            v += 0x18;
        }
    }
    if (v < 0) {
        v += 0x1000;
    }
    v &= 0xFFF;
    angle = v - 0x800;
    p->velocity_x = func_800A0140(angle) * 0x18;
    p->velocity_y = rsin(angle) * 0x18;
    p->heading = v;
}

void tt_enemy_shot_beam_update(TtEnemyShot *p) {
    TtBossView *scene;
    u16 state;

    scene = (TtBossView *)tt_boss;
    if (p->homing_age == 15) {
        p->beam_length = 0;
        p->state = 0x40;
        p->step2 = 0;
        p->step = 0;
        p->phase = 0;
        p->tick2 = 0;
        p->tick1 = 0;
        p->tick0 = 0;
    }
    state = p->state;
    switch (state) {
    case 0x20:
        /* FAKE: empty comparison reserves a2 for the shared 0x20 constant
         * while state stays in v1; real source unknown. T-9760 */
        if ((!p->state) && (!p->state)) {}
        p->position_x += (scene->unk_14 - p->heading) << 16;
        p->heading = scene->unk_14;
        break;
    case 0x40:
        p->beam_length += 4;
        if (p->beam_length >= 0x20) {
            p->beam_length = 0x20;
            p->velocity_y = 0x40000;
            p->state = 0x50;
            p->step2 = 0;
            p->step = 0;
            p->phase = 0;
            p->tick2 = 0;
            p->tick1 = 0;
            p->tick0 = 0;
        }
        break;
    }
    p->homing_age++;
}

void tt_enemy_shots_update(void) {
    TtEnemyShot *object;
    s32 i;
    u8 *sys;

    object = (TtEnemyShot *)tt_enemy_shots;
    for (i = 0; i < 0x80; i++, object++) {
        /* FAKE: empty test; uopt hoists 0x80 and 0x40 into s2/s3 ahead of the other constants (the original keeps an unused 0x40 in s3), ugen emits no code. T-11850 */
        if (i == 0x80 || i == 0x40) {
        }
        if (object->active != 0) {
            if (object->kind == 0xAD) {
                tt_enemy_shot_beam_update(object);
            } else if (object->kind == 0xAC) {
                tt_enemy_shot_home(object);
            }
            tt_enemy_shot_move((u8 *)object);
            if (object->kind == 0xAB) {
                sys = tt_sys;
                object->animation_frame = object->animation_hold / 12 + 0xF;
                object->animation_frame += *(u16 *)(sys + 0xA) & 1;
                object->animation_hold++;
                if (object->animation_hold >= 0x31) {
                    object->animation_hold = 0x30;
                }
            } else {
                tt_animation_list_step((u8 *)object + 0x38, (u8 *)object + 0x56, (u8 *)object + 0x57);
            }
        }
    }
}

/* q is passed as the unused third argument of tt_sprite_sized_submit, as its other callers do; this puts q in a2 (T-11620). */
void tt_enemy_shot_draw_homing(u8 *p) {
    u8 *buf;
    u8 *v;
    u8 *sys;
    u8 *transform;

    buf = *(s32 *)(tt_video + 8) * 0x80B8 + tt_frames;
    v = *(u8 **)(buf + 4);
    sys = tt_sys;
    transform = tt_transforms;
    *(u8 **)(buf + 4) += 0x30;
    *(s16 *)(transform + 8) = *(s16 *)(p + 0x14);
    *(s16 *)(transform + 0xA) = *(s16 *)(p + 0x16);
    *(s32 *)(transform + 0xC) = *(u16 *)(p + 0x60) + 0x800;
    *(s16 *)(v + 8) = *(s16 *)(p + 0x14) - 0xC;
    *(s16 *)(v + 0xA) = *(s16 *)(p + 0x16) - 0xC;
    v[0x29] = 0;
    v[0x2A] = 1;
    *(s16 *)(v + 0x10) = 0x18;
    *(s16 *)(v + 0x12) = 0x18;
    v[0x28] = 0;
    *(u16 *)(v + 0x16) = 0xB;
    if (*(u16 *)(sys + 8) & 1) {
        v[0xC] = 0xE0;
    } else {
        v[0xC] = 0xB0;
    }
    v[0xD] = 0;
    *(u16 *)(v + 0xE) = 0x3D94;
    v[0x2B] = 0;
    v[0x2C] = 1;
    tt_sprite_sized_submit(&((TtDrawBuffer *)buf)->ordering[*(u16 *)(p + 0x10)], v, transform);
}

/* Tile packets written one PsyQ macro per line (setXY0, setRGB0, setWH); the line order sets the schedule. q is loaded before the test because it is the third argument of tt_sprite16_submit (T-11620). */
void tt_enemy_shot_draw_beam(u8 *p) {
    u8 *buf;
    u8 *v;
    u8 *transform;
    u8 *t;

    buf = *(s32 *)(tt_video + 8) * 0x80B8 + tt_frames;
    transform = tt_transforms;
    if (*(u16 *)(p + 4) == 0x20) {
        v = *(u8 **)(buf + 4);
        *(u8 **)(buf + 4) += 0x30;
        *(s16 *)(transform + 8) = *(s16 *)(p + 0x14);
        *(s16 *)(transform + 0xA) = *(s16 *)(p + 0x16);
        *(s16 *)(v + 8) = *(s16 *)(p + 0x14) - 8;
        *(s16 *)(v + 0xA) = *(s16 *)(p + 0x16) - 8;
        v[0x29] = 0;
        v[0x2A] = 1;
        v[0x28] = 0;
        *(u16 *)(v + 0x16) = 0xA;
        v[0xC] = 0x90;
        v[0xD] = 0xF0;
        *(u16 *)(v + 0xE) = 0x3E56;
        v[0x2B] = 0;
        v[0x2C] = 0;
        tt_sprite16_submit(&((TtDrawBuffer *)buf)->ordering[*(u16 *)(p + 0x10)], v, transform);
        return;
    }
    t = *(u8 **)(buf + 4);
    *(u8 **)(buf + 4) += 0x10;
    ((TtOtTag *)t)->len = 3; t[7] = 0x61;
    *(s16 *)(t + 8) = *(s16 *)(p + 0x14) - 1; *(s16 *)(t + 0xA) = *(s16 *)(p + 0x16);
    t[4] = 0xF0; t[5] = 0xE0; t[6] = 0xF0;
    *(s16 *)(t + 0xC) = 2; *(u16 *)(t + 0xE) = *(u16 *)(p + 0x62);
    ((TtOtTag *)t)->addr = ((TtDrawBuffer *)buf)->ordering[*(u16 *)(p + 0x10)].addr;
    ((TtDrawBuffer *)buf)->ordering[*(u16 *)(p + 0x10)].addr = (u32)t;
    t = *(u8 **)(buf + 4);
    *(u8 **)(buf + 4) += 0x10;
    ((TtOtTag *)t)->len = 3; t[7] = 0x61;
    t[4] = 0xC0; t[5] = 0x68; t[6] = 0xE8;
    *(s16 *)(t + 8) = *(s16 *)(p + 0x14) - 2; *(s16 *)(t + 0xA) = *(s16 *)(p + 0x16);
    *(s16 *)(t + 0xC) = 2; *(u16 *)(t + 0xE) = *(u16 *)(p + 0x62);
    ((TtOtTag *)t)->addr = ((TtDrawBuffer *)buf)->ordering[*(u16 *)(p + 0x10)].addr;
    ((TtDrawBuffer *)buf)->ordering[*(u16 *)(p + 0x10)].addr = (u32)t;
    t = *(u8 **)(buf + 4);
    *(u8 **)(buf + 4) += 0x10;
    ((TtOtTag *)t)->len = 3; t[7] = 0x61;
    t[4] = 0x78; t[5] = 0x10; t[6] = 0x90;
    *(s16 *)(t + 8) = *(s16 *)(p + 0x14) - 3; *(s16 *)(t + 0xA) = *(s16 *)(p + 0x16);
    *(s16 *)(t + 0xC) = 6; *(u16 *)(t + 0xE) = *(u16 *)(p + 0x62);
    ((TtOtTag *)t)->addr = ((TtDrawBuffer *)buf)->ordering[*(u16 *)(p + 0x10)].addr;
    ((TtDrawBuffer *)buf)->ordering[*(u16 *)(p + 0x10)].addr = (u32)t;
}

void tt_enemy_shots_draw(void) {
    u8 *transform;
    u8 *enemy_shot;
    u8 *buf;
    u8 *v;
    s32 i;
    u16 kind;
    s32 n;

    buf = *(s32 *)(tt_video + 8) * 0x80B8 + tt_frames;
    transform = tt_transforms;
    enemy_shot = tt_enemy_shots;
    /* The bound in a local, set in the for init, puts 0x80 in s5 ahead of the chain constants (T-11620). */
    for (i = 0, n = 0x80; i != n; i++) {
        if (*enemy_shot != 0) {
            if (*(u16 *)(enemy_shot + 2) == 0xAB) {
                tt_part_sprite_draw(enemy_shot + 0x14, enemy_shot + 0x38, *(u16 *)(enemy_shot + 0x10));
            } else if (*(u16 *)(enemy_shot + 2) == 0xAC) {
                tt_enemy_shot_draw_homing(enemy_shot);
            } else if (*(u16 *)(enemy_shot + 2) == 0xAD) {
                tt_enemy_shot_draw_beam(enemy_shot);
            } else {
                v = *(u8 **)(buf + 4);
                *(u8 **)(buf + 4) += 0x30;
                *(s16 *)(transform + 4) = *(s16 *)(enemy_shot + 0x14);
                *(s16 *)(transform + 6) = *(s16 *)(enemy_shot + 0x16);
                *(s16 *)(v + 8) = *(s16 *)(enemy_shot + 0x14) - 8;
                *(s16 *)(v + 0xA) = *(s16 *)(enemy_shot + 0x16) - 8;
                v[0x29] = 0;
                v[0x2A] = 1;
                *(s16 *)(v + 0x10) = 0x10;
                *(s16 *)(v + 0x12) = 0x10;
                v[0x28] = enemy_shot[0x3C];
                kind = *(u16 *)(enemy_shot + 2);
                if (kind != 0xA8 && kind != 0xA9 && kind != 0xAA) {
                    v[0xC] = enemy_shot[0x56];
                    v[0xD] = enemy_shot[0x57] + 0x80;
                } else {
                    v[0xC] = enemy_shot[0x56];
                    v[0xD] = enemy_shot[0x57];
                }
                *(u16 *)(v + 0x16) = *(u16 *)(enemy_shot + 0x52);
                *(u16 *)(v + 0xE) = *(u16 *)(enemy_shot + 0x54);
                v[0x2B] = 0;
                v[0x2C] = 0;
                tt_sprite16_submit(&((TtDrawBuffer *)buf)->ordering[*(u16 *)(enemy_shot + 0x10)], v, transform);
            }
        }
        enemy_shot += 0x68;
    }
}
