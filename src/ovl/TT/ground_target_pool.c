/* ground_target_pool.c: src/ovl/TT/80132F10.c:76: ground target pool: spawn from script, update, draw; body 82: m = -1; 83: g = (u8 *)tt_stage_script;. Original object 0x80132F10-0x80133970 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/tt.h"

u8 *tt_ground_target_find_free(u8 *cursor, u8 *end) {
    while (cursor < end) {
        if (*cursor == 0) {
            return cursor;
        }
        cursor += 0x78;
    }
    return 0;
}

void tt_ground_targets_clear(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = tt_ground_targets; i < 0x20; i++, p += 0x78) {
        *p = 0;
    }
}

void tt_ground_targets_reset(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = tt_ground_targets; i < 0x20; i++, p += 0x78) {
        *p = 0;
        *(s16 *)(p + 0x10) = 0xC;
    }
}

void tt_ground_target_setup(u8 *ground_target, u16 kind, s32 animation_index) {
    ground_target[0] = 1;
    ground_target[0x51] = 0;
    ground_target[0x64] = 0;
    *(u16 *)(ground_target + 2) = kind;
    *(s16 *)(ground_target + 0x58) = 0xA;
    *(s32 *)(ground_target + 0x38) = (&tt_animation_lists)[animation_index];
    ground_target[0x40] = 0x80;
    *(s16 *)(ground_target + 0x56) = 0;
    switch (kind) {
    case 0x60:
        *(s16 *)(ground_target + 0x5A) = 0x3CC7;
        break;
    case 0x61:
    case 0x62:
        *(s16 *)(ground_target + 0x5A) = 0x3CC8;
        break;
    case 0x63:
        *(s16 *)(ground_target + 0x5A) = 0x3CC9;
        break;
    }
}

/* FAKE: the two byte stores of each case share one source line; as1 schedules them as a unit, which gives the original's store order. Real source unknown. T-8010 */
void tt_ground_target_set_drop_sprite(u8 *ground_target, u16 drop_kind) {
    *(u16 *)(ground_target + 2) = drop_kind;
    switch (drop_kind) {
    case 0x6E:
        ground_target[0x5C] = 0xF0; ground_target[0x5D] = 0xA0;
        *(u16 *)(ground_target + 0x5A) = 0x3C84;
        break;
    case 0x6F:
        ground_target[0x5C] = 0xF0; ground_target[0x5D] = 0xB0;
        *(u16 *)(ground_target + 0x5A) = 0x3C84;
        break;
    case 0x6D:
        ground_target[0x5C] = 0xF0; ground_target[0x5D] = 0xE0;
        *(u16 *)(ground_target + 0x5A) = 0x3C8D;
        break;
    }
}

/* T-11540/T-11550: matched without the t and x locals: the record's y is read inline in both loop tests (uopt CSEs it; the named s16 gave the beq operand order v0, s3) and a+0x24 is computed from the stored a+0x1C (the x local took v0 and shifted the ugen temporaries). */
void tt_ground_target_spawn_from_script(void) {
    u8 *stage_script;
    u8 *a;
    s32 id;
    s32 m;

    m = -1;
    stage_script = (u8 *)tt_stage_script;
    while (1) {
        if (m == *(s16 *)(*(u8 **)(stage_script + 8) + 2) || *(u16 *)(stage_script + 2) < (u32)*(s16 *)(*(u8 **)(stage_script + 8) + 2) || (a = tt_ground_target_find_free(tt_ground_targets, tt_ground_targets + 0xF00)) == 0) {
            break;
        }
        *(u16 *)(a + 2) = *(u16 *)(*(u8 **)(stage_script + 8) + 4);
        switch (*(u16 *)(a + 2)) {
        case 0x60:
            id = 0xE;
            break;
        case 0x61:
            id = 0xF;
            break;
        case 0x62:
            id = 0x10;
            break;
        case 0x63:
            id = 0x11;
            break;
        }
        *(s32 *)(a + 0x20) = *(s16 *)(*(u8 **)(stage_script + 8)) << 16;
        *(s32 *)(a + 0x1C) = *(s16 *)(*(u8 **)(stage_script + 8) + 2) << 16;
        *(s32 *)(a + 0x24) = *(s32 *)stage_script - *(s32 *)(a + 0x1C);
        *(s32 *)(a + 0x28) = 0;
        *(s32 *)(a + 0x2C) = 0;
        *(s16 *)(a + 0x5E) = 0;
        a[0x60] = 0;
        *(s16 *)(a + 0x14) = *(s16 *)(a + 0x22);
        *(s16 *)(a + 0x16) = *(s16 *)(a + 0x26);
        tt_ground_target_setup(a, *(u16 *)(a + 2), id);
        *(s16 *)(a + 4) = 0x40;
        a[0x40] |= 0x40;
        *(s16 *)(a + 10) = 0;
        *(s16 *)(a + 8) = 0;
        *(s16 *)(a + 6) = 0;
        a[15] = 0;
        a[14] = 0;
        a[13] = 0;
        *(u8 **)(stage_script + 8) += 6;
    }
}

void tt_ground_target_update(u8 *ground_target) {
    u8 *player;
    s16 y;
    s16 x;
    s32 d;
    s32 a;

    player = tt_player;
    *(s32 *)(ground_target + 0x24) = *tt_stage_script - *(s32 *)(ground_target + 0x1C);
    *(s32 *)(ground_target + 0x20) += *(s32 *)(ground_target + 0x28);
    *(s32 *)(ground_target + 0x24) += *(s32 *)(ground_target + 0x2C);
    *(s16 *)(ground_target + 0x14) = *(s16 *)(ground_target + 0x22);
    *(s16 *)(ground_target + 0x16) = *(s16 *)(ground_target + 0x26);
    if (*(u16 *)(ground_target + 4) == 0x40) {
        if (*(u16 *)(ground_target + 0x5E) > 0x28 && ground_target[0x60] == 0) {
            ground_target[0x40] &= 0xFFBF;
        }
        if (ground_target[0x40] & 1) {
            y = *(s16 *)(ground_target + 0x16);
            ground_target[0x40] = 0xC0;
            *(u16 *)(ground_target + 0x5E) = 0;
            if (y < 0xC0) {
                x = *(s16 *)(ground_target + 0x14);
                d = *(s16 *)(player + 0x14) - x;
                if (d < 0) {
                    a = -d;
                } else {
                    a = d;
                }
                if (a < 0x20) {
                    d = *(s16 *)(player + 0x16) - y;
                    if (d < 0) {
                        a = -d;
                    } else {
                        a = d;
                    }
                    if (a >= 0x20) {
                        goto call;
                    }
                } else {
call:
                    ground_target[0x60] += 1;
                    tt_enemy_shot_fire_aimed(x, y, 0x20, 0xA0);
                }
            }
        }
        *(u16 *)(ground_target + 0x5E) += 1;
    }
}

void tt_ground_targets_update(void) {
    u8 *core;
    u8 *ground_target;
    s32 i;

    core = tt_core;
    tt_ground_target_spawn_from_script();
    for (i = 0, ground_target = tt_ground_targets; i != 0x20; i++, ground_target += 0x78) {
        if (ground_target[0] == 0) {
            continue;
        }
        switch (*(u16 *)(ground_target + 4)) {
        case 0x40:
            if (*(s16 *)(ground_target + 0x16) >= 0xF9) {
                ground_target[0] = 0;
            }
            break;
        case 0x70:
            tt_se_request(TT_SE_GROUND_EXPLODE);
            core[0x25]++;
            if (core[0x24] != 0 && (core[0x25] & 1)) {
                if (core[0x25] & 2) {
                    tt_enemy_shot_fire_fan4_aimed(*(s16 *)(ground_target + 0x14), *(s16 *)(ground_target + 0x16), 0x100, 0x20, 0xA0);
                } else {
                    tt_enemy_shot_fire_3way_aimed(*(s16 *)(ground_target + 0x14), *(s16 *)(ground_target + 0x16), 0x100, 0x20, 0xA0);
                }
            }
            *(u8 **)(ground_target + 0x38) = tt_sprite_set_table;
            ground_target[0x3C] = 0;
            ground_target[0x3D] = 2;
            ground_target[0x3E] = 0xE;
            ground_target[0x40] = 0x20;
            *(u16 *)(ground_target + 4) = 0x71;
            *(u16 *)(ground_target + 0xA) = 0;
            *(u16 *)(ground_target + 8) = 0;
            *(u16 *)(ground_target + 6) = 0;
            ground_target[0xF] = 0;
            ground_target[0xE] = 0;
            ground_target[0xD] = 0;
            break;
        case 0x78:
            if (--*(u16 *)(ground_target + 0x66) == 0) {
                *(u16 *)(ground_target + 0x56) = 2;
                *(u16 *)(ground_target + 0x66) = 0x1E;
                *(u16 *)(ground_target + 4) = 0x79;
                *(u16 *)(ground_target + 0xA) = 0;
                *(u16 *)(ground_target + 8) = 0;
                *(u16 *)(ground_target + 6) = 0;
                ground_target[0xF] = 0;
                ground_target[0xE] = 0;
                ground_target[0xD] = 0;
            }
            break;
        case 0x79:
            if (--*(u16 *)(ground_target + 0x66) == 0) {
                *(u16 *)(ground_target + 4) = 0x80;
            }
            break;
        case 0x7A:
            if (*(s16 *)(ground_target + 0x16) >= 0xF9) {
                ground_target[0] = 0;
            }
            break;
        case 0x80:
            ground_target[0] = 0;
            break;
        }
        tt_ground_target_update(ground_target);
        if (*(u16 *)(ground_target + 4) == 0x40) {
            tt_animation_list_step(ground_target + 0x38, ground_target + 0x5C, ground_target + 0x5D);
        } else if (*(u16 *)(ground_target + 4) == 0x71) {
            tt_sprite_animation_step(ground_target + 0x38);
            if (ground_target[0x40] & 1) {
                switch (tt_drop_roll()) {
                case 0:
                    tt_ground_target_set_drop_sprite(ground_target, 0x6D);
                    *(u16 *)(ground_target + 0x66) = 0x3C;
                    *(u16 *)(ground_target + 4) = 0x78;
                    *(u16 *)(ground_target + 0xA) = 0;
                    *(u16 *)(ground_target + 8) = 0;
                    *(u16 *)(ground_target + 6) = 0;
                    ground_target[0xF] = 0;
                    ground_target[0xE] = 0;
                    ground_target[0xD] = 0;
                    break;
                case 1:
                    tt_ground_target_set_drop_sprite(ground_target, 0x6E);
                    *(u16 *)(ground_target + 4) = 0x7A;
                    *(u16 *)(ground_target + 0xA) = 0;
                    *(u16 *)(ground_target + 8) = 0;
                    *(u16 *)(ground_target + 6) = 0;
                    ground_target[0xF] = 0;
                    ground_target[0xE] = 0;
                    ground_target[0xD] = 0;
                    break;
                case 2:
                    tt_ground_target_set_drop_sprite(ground_target, 0x6F);
                    *(u16 *)(ground_target + 4) = 0x7A;
                    *(u16 *)(ground_target + 0xA) = 0;
                    *(u16 *)(ground_target + 8) = 0;
                    *(u16 *)(ground_target + 6) = 0;
                    ground_target[0xF] = 0;
                    ground_target[0xE] = 0;
                    ground_target[0xD] = 0;
                    break;
                }
            }
        }
    }
}

void tt_ground_targets_draw(void) {
    TtDrawBuffer *buffer;
    TtSpritePacket *q;
    DR_MODE *m;
    u8 *ground_target;
    u8 *sys;
    s32 i;

    sys = tt_sys;
    ground_target = tt_ground_targets;
    buffer = (TtDrawBuffer *)(tt_frames + ((TtVideoView *)tt_video)->index * 0x80B8);
    for (i = 0; i != 0x20; i++, ground_target += 0x78) {
        if (ground_target[0] != 0 && (!(*(u16 *)(ground_target + 0x56) & 2) || !((*(u16 *)(sys + 8) >> 1) & 1))) {
            if (*(u16 *)(ground_target + 4) == 0x71) {
                tt_part_sprite_draw(ground_target + 0x14, ground_target + 0x38, *(u16 *)(ground_target + 0x10));
            } else {
                q = (TtSpritePacket *)buffer->cursor;
                buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x14);
                q->tag.len = 4; q->code = 0x65;
                q->x = *(s16 *)(ground_target + 0x14) - 8;
                q->y = *(s16 *)(ground_target + 0x16) - 8;
                q->h = 0x10;
                q->w = 0x10;
                q->u = ground_target[0x5C];
                q->v = ground_target[0x5D];
                q->clut = *(u16 *)(ground_target + 0x5A);
                q->tag.addr = buffer->ordering[*(u16 *)(ground_target + 0x10)].addr;
                buffer->ordering[*(u16 *)(ground_target + 0x10)].addr = (u32)q;
                m = buffer->cursor;
                buffer->cursor++;
                func_8009D294((s32)m, 0, 0, *(u16 *)(ground_target + 0x58), 0);
                ((TtOtTag *)m)->addr = buffer->ordering[*(u16 *)(ground_target + 0x10)].addr;
                buffer->ordering[*(u16 *)(ground_target + 0x10)].addr = (u32)m;
            }
        }
    }
}
