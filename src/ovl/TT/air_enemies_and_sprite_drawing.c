/* air_enemies_and_sprite_drawing.c: src/ovl/TT/8014AF00.c:637: air enemy waves/script, movement/explosions and generic tile/textured/transformed sprite drawing; body 640: g = (TtStageScript *)tt_stage_script; 642: if (*(s16 *)g->cmd == -1) {. Original object 0x8014AF00-0x8014D260 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/tt.h"

u8 *tt_air_enemy_find_free(u8 *cursor, u8 *end) {
    while (cursor < end) {
        if (*cursor == 0) {
            return cursor;
        }
        cursor += 0x78;
    }
    return 0;
}

void tt_air_enemies_clear(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = tt_air_enemies; i < 0x40; i++, p += 0x78) {
        *p = 0;
    }
}

void tt_air_enemies_reset(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = tt_air_enemies; i < 0x40; i++, p += 0x78) {
        *p = 0;
        *(s16 *)(p + 0x10) = 4;
    }
}

void tt_air_enemy_setup(u8 *enemy, u16 enemy_kind, s32 animation_index) {
    enemy[0] = 1;
    enemy[0x51] = 0;
    *(u16 *)(enemy + 2) = enemy_kind;
    *(s16 *)(enemy + 0x58) = 0xA;
    *(s32 *)(enemy + 0x38) = (&tt_animation_lists)[animation_index];
    enemy[0x40] = 0x80;
    switch (enemy_kind) {
    case 0x40:
        *(s16 *)(enemy + 0x5A) = 0x3CC0;
        return;
    case 0x41:
        *(s16 *)(enemy + 0x5A) = 0x3CC1;
        return;
    case 0x42:
        *(s16 *)(enemy + 0x5A) = 0x3CC1;
        return;
    case 0x43:
        *(s16 *)(enemy + 0x5A) = 0x3CC2;
        return;
    case 0x44:
        *(s16 *)(enemy + 0x5A) = 0x3CC3;
        return;
    case 0x45:
        *(s16 *)(enemy + 0x5A) = 0x3CC5;
        return;
    case 0x46:
        *(s16 *)(enemy + 0x5A) = 0x3CC9;
        return;
    case 0x47:
        *(s16 *)(enemy + 0x5A) = 0x3CC6;
        return;
    case 0x48:
        *(s16 *)(enemy + 0x5A) = 0x3CC5;
        return;
    }
}

void tt_air_wave_c0_spawn(u8 right_side) {
    u32 size;
    s32 i;
    u8 *p;
    s32 a2;

    size = 0x40;
    /* Wide local preserves the u16 call conversion. T-9570 */
    p = tt_air_enemies;
    i = 0;
    do {
        p = tt_air_enemy_find_free(p, p + 0x1E00);
        if (p == 0) {
            return;
        }
        if (right_side != 0) {
            a2 = 4;
            *(s32 *)(p + 0x20) = 0x500000;
        } else {
            a2 = 3;
            *(s32 *)(p + 0x20) = 0xFFB00000;
        }
        *(s32 *)(p + 0x24) = (-0x10 - i * 0x10) << 16;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0x20000;
        p[0x63] = right_side;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        tt_air_enemy_setup(p, size, a2);
        i += 1;
        p += 0x78;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = size;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 6);
}

void tt_air_enemy_kind40_move(u8 *enemy) {
    u8 *run;
    u8 t;

    run = tt_run;
    if (enemy[0x63] != 0) {
        *(s32 *)(enemy + 0x28) = *(s32 *)(enemy + 0x24) / -64;
    } else {
        *(s32 *)(enemy + 0x28) = *(s32 *)(enemy + 0x24) / 64;
    }
    if ((run[0xB] != 0) && (*(u16 *)(enemy + 0x5E) >= 0x19U)) {
        t = enemy[0x60];
        if (t == 0) {
            *(u16 *)(enemy + 0x5E) = 0;
            enemy[0x60] = t + 1;
            tt_enemy_shot_fire_aimed(*(s16 *)(enemy + 0x14), *(s16 *)(enemy + 0x16), 0x20, 0xA0);
        }
    }
    if (enemy[0x51] != 0) {
        *(u16 *)(enemy + 0x5E) = *(u16 *)(enemy + 0x5E) + 1;
    }
}

void tt_air_wave_c1_spawn(u8 right_side) {
    s32 i;
    u8 *p;
    u8 *src;
    s32 ang;
    s32 c;

    src = tt_player;
    p = tt_air_enemies;
    i = 0;
    do {
        p = tt_air_enemy_find_free(p, p + 0x1E00);
        if (p == 0) {
            return;
        }
        p[0x50] = i;
        if (right_side != 0) {
            *(s32 *)(p + 0x20) = (i * 16 + 0x30) << 16;
        } else {
            *(s32 *)(p + 0x20) = (-0x30 - i * 16) << 16;
        }
        *(s32 *)(p + 0x24) = -0x10;
        ang = tt_angle_between(p + 0x14, src + 0x14);
        if (i == 1) {
            tt_air_enemy_setup(p, 0x42, 6);
        } else {
            tt_air_enemy_setup(p, 0x41, 5);
            if (right_side != 0) {
                ang += (i == 0) ? 0x60 : -0x60;
            } else {
                ang -= (i == 0) ? 0x60 : -0x60;
            }
        }
        *(s32 *)(p + 0x28) = func_800A0140(ang) << 5;
        c = rsin(ang);
        i += 1;
        p += 0x78;
        *(s32 *)(p - 0x4C) = c << 5;
        p[-0x15] = right_side;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = 0x40;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
        *(s16 *)(p - 0x64) = *(s16 *)(p - 0x56);
        *(s16 *)(p - 0x62) = *(s16 *)(p - 0x52);
    } while (i != 3);
}

void tt_air_enemy_kind41_move(u8 *enemy) {
    if (tt_run[0xB] != 0 && (u32)*(u16 *)(enemy + 0x5E) >= 0x19) {
        if (enemy[0x60] == 0) {
            *(u16 *)(enemy + 0x5E) = 0;
            enemy[0x60] += 1;
            tt_enemy_shot_fire_aimed(*(s16 *)(enemy + 0x14), *(s16 *)(enemy + 0x16), 0x20, 0xA0);
        }
    }
    if (enemy[0x51] != 0) {
        *(u16 *)(enemy + 0x5E) += 1;
    }
}

void tt_air_wave_c2_spawn(u8 right_side) {
    u8 *p = tt_air_enemies;
    s32 i = 0;
    s32 v;

    do {
        p = tt_air_enemy_find_free(p, p + 0x1E00);
        if (p == 0) {
            break;
        }
        if (right_side != 0) {
            v = 8;
            *(s32 *)(p + 0x20) = 0x500000;
        } else {
            v = 7;
            *(s32 *)(p + 0x20) = 0xFFB00000;
        }
        *(s32 *)(p + 0x24) = (-0x10 - i * 0x10) << 16;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0x10000;
        p[0x63] = right_side;
        tt_air_enemy_setup(p, 0x43, v);
        /* FAKE: the reset stores are written relative to the already advanced slot pointer (p - off); the original code does the increment first. T-2070 */
        i += 1;
        p += 0x78;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = 0x40;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 4);
}

void tt_air_enemy_kind43_move(u8 *enemy) {
    if (enemy[0x63] != 0) {
        *(s32 *)(enemy + 0x20) = -rsin(*(s16 *)(enemy + 0x16) << 5) * 0x600;
    } else {
        *(s32 *)(enemy + 0x20) = rsin(*(s16 *)(enemy + 0x16) << 5) * 0x600;
    }
    if ((u32)*(u16 *)(enemy + 0x5E) >= 0x11) {
        if (enemy[0x60] == 0) {
            *(u16 *)(enemy + 0x5E) = 0;
            enemy[0x60] += 1;
            tt_enemy_shot_fire_aimed(*(s16 *)(enemy + 0x14), *(s16 *)(enemy + 0x16), 0x20, 0xA0);
        }
    }
    if (enemy[0x51] != 0) {
        *(u16 *)(enemy + 0x5E) += 1;
    }
}

void tt_air_wave_c3_spawn(u8 right_side) {
    u8 *p = tt_air_enemies;
    s32 i = 0;

    do {
        p = tt_air_enemy_find_free(p, p + 0x1E00);
        if (p == 0) {
            break;
        }
        if (right_side != 0) {
            *(s32 *)(p + 0x20) = (i * 0x10 + 0x90) << 16;
            *(s32 *)(p + 0x28) = 0xFFFC0000;
        } else {
            *(s32 *)(p + 0x20) = (-0x90 - i * 0x10) << 16;
            *(s32 *)(p + 0x28) = 0x40000;
        }
        *(s32 *)(p + 0x24) = 0x380000;
        *(s32 *)(p + 0x2C) = 0;
        p[0x63] = right_side;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        tt_air_enemy_setup(p, 0x44, 9);
        /* FAKE: the reset stores are written relative to the already advanced slot pointer (p - off); the original code does the increment first. T-2070 */
        i += 1;
        p += 0x78;
        p[-0x38] = p[-0x38] | 0x40;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = 0x40;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 6);
}

void tt_air_enemy_kind44_move(u8 *enemy) {
    s16 idx;
    u8 *e;

    switch (*(u16 *)(enemy + 6)) {
    case 0:
        if (enemy[0x63] != 0) {
            enemy[0x5C] = 0xD0;
            enemy[0x5D] = 0x30;
            enemy[0x3C] = 0;
            if (*(s32 *)(enemy + 0x20) < 0x80001) {
                *(u16 *)(enemy + 6) = 1;
                *(u16 *)(enemy + 0x6E) = 0x1C00;
            }
        } else {
            enemy[0x5C] = 0xD0;
            enemy[0x5D] = 0x30;
            enemy[0x3C] = 2;
            if (*(s32 *)(enemy + 0x20) >= -0x80000) {
                *(u16 *)(enemy + 6) = 1;
                *(u16 *)(enemy + 0x6E) = 0xC00;
            }
        }
        break;
    case 1:
        if (enemy[0x63] != 0) {
            *(s32 *)(enemy + 0x20) = func_800A0140(*(u16 *)(enemy + 0x6E)) * 0x300;
            *(s32 *)(enemy + 0x24) = rsin(*(u16 *)(enemy + 0x6E)) * 0x300 + 0x680000;
            *(s32 *)(enemy + 0x28) = 0;
            if (*(u16 *)(enemy + 0x6E) < 0xC00) {
                *(u16 *)(enemy + 6) = 2;
            }
            *(u16 *)(enemy + 0x6E) -= 0x40;
            idx = ((*(u16 *)(enemy + 0x6E) + 0x100) & 0xFFF) / 512 * 3;
        } else {
            *(s32 *)(enemy + 0x20) = func_800A0140(*(u16 *)(enemy + 0x6E)) * 0x300;
            *(s32 *)(enemy + 0x24) = rsin(*(u16 *)(enemy + 0x6E)) * 0x300 + 0x680000;
            *(s32 *)(enemy + 0x28) = 0;
            if (*(u16 *)(enemy + 0x6E) > 0x1C00) {
                *(u16 *)(enemy + 6) = 2;
            }
            *(u16 *)(enemy + 0x6E) += 0x40;
            idx = ((*(u16 *)(enemy + 0x6E) + 0x900) & 0xFFF) / 512 * 3;
        }
        e = tt_air_enemy_kind44_pose + idx;
        enemy[0x5C] = e[0];
        enemy[0x5D] = e[1];
        enemy[0x3C] = e[2];
        break;
    case 2:
        if (enemy[0x63] != 0) {
            *(u16 *)(enemy + 0x6E) = 0;
            *(s32 *)(enemy + 0x28) = 0xFFFC0000;
            enemy[0x5C] = 0xD0;
            enemy[0x5D] = 0x30;
            enemy[0x3C] = 0;
        } else {
            *(u16 *)(enemy + 0x6E) = 0;
            *(s32 *)(enemy + 0x28) = 0x40000;
            enemy[0x5C] = 0xD0;
            enemy[0x5D] = 0x30;
            enemy[0x3C] = 2;
        }
        break;
    }
    if (*(u16 *)(enemy + 0x5E) > 0x3C) {
        if (enemy[0x60] == 0) {
            *(u16 *)(enemy + 0x5E) = 0;
            enemy[0x60] += 1;
            tt_enemy_shot_fire_aimed(*(s16 *)(enemy + 0x14), *(s16 *)(enemy + 0x16), 0x20, 0xA0);
        }
    }
    if (enemy[0x51] != 0) {
        *(u16 *)(enemy + 0x5E) += 1;
    }
}

void tt_air_wave_c4_spawn(s32 arg0) {
    u8 *p = tt_air_enemies;
    s32 i = 0;

    do {
        p = tt_air_enemy_find_free(p, p + 0x1E00);
        if (p == 0) {
            break;
        }
        *(s32 *)(p + 0x20) = 0;
        *(s32 *)(p + 0x24) = 0xFFF00000;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0x20000;
        p[0x50] = i;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        tt_air_enemy_setup(p, 0x45, 0xA);
        /* FAKE: the reset stores are written relative to the already advanced slot pointer (p - off); the original code does the increment first. T-2070 */
        i += 1;
        p += 0x78;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = 0x40;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 5);
}

void tt_air_enemy_kind45_move(TtObjectView *enemy) {
    s32 value;
    u8 flag;

    if ((s32)(rsin(enemy->position_y.w / 3072) << 9) < 0) {
        value = -(rsin(enemy->position_y.w / 3072) << 9);
    } else {
        value = rsin(enemy->position_y.w / 3072) << 9;
    }
    enemy->position_x.w = (value + 0x100000) * (enemy->formation_index - 2);
    if (((TtFieldEnemyTail *)enemy->pad_5E)->shot_timer >= 0x19U) {
        flag = ((TtFieldEnemyTail *)enemy->pad_5E)->shot_fired;
        if (flag == 0) {
            ((TtFieldEnemyTail *)enemy->pad_5E)->shot_timer = 0;
            ((TtFieldEnemyTail *)enemy->pad_5E)->shot_fired = flag + 1;
            tt_enemy_shot_fire_aimed(enemy->x, enemy->y, 0x20, 0xA0);
        }
    }
    if (enemy->in_bounds != 0) {
        ((TtFieldEnemyTail *)enemy->pad_5E)->shot_timer += 1;
    }
}

void tt_air_wave_c5_spawn(s32 arg0) {
    u8 *p = tt_air_enemies;
    s32 i = 0;

    do {
        p = tt_air_enemy_find_free(p, p + 0x1E00);
        if (p == 0) {
            break;
        }
        *(s32 *)(p + 0x20) = (i << 21) + 0xFFC00000;
        *(s32 *)(p + 0x24) = 0xFFF00000;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0x20000;
        *(s16 *)(p + 0x6E) = 0;
        p[0x50] = i;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        tt_air_enemy_setup(p, 0x46, 0xB);
        /* FAKE: the reset stores are written relative to the already advanced slot pointer (p - off); the original code does the increment first. T-2070 */
        i += 1;
        p += 0x78;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = 0x40;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 5);
}

void tt_air_enemy_kind46_move(u8 *enemy) {
    u32 t;

    *(s32 *)(enemy + 0x20) = (enemy[0x50] - 2) * (func_800A0140(*(u16 *)(enemy + 0x6E) * 0x1E) << 9);
    t = *(u16 *)(enemy + 0x6E);
    *(s32 *)(enemy + 0x24) = (t - 0x10) << 16;
    *(s32 *)(enemy + 0x24) += (rsin(t * 0x1E) << 9) * (enemy[0x50] - 2);
    *(u16 *)(enemy + 0x6E) += 1;
    if ((u32)*(u16 *)(enemy + 0x5E) >= 0x19) {
        if (enemy[0x60] == 0) {
            *(u16 *)(enemy + 0x5E) = 0;
            enemy[0x60] += 1;
            tt_enemy_shot_fire_aimed(*(s16 *)(enemy + 0x14), *(s16 *)(enemy + 0x16), 0x20, 0xA0);
        }
    }
    if (enemy[0x51] != 0) {
        *(u16 *)(enemy + 0x5E) += 1;
    }
}

void tt_air_wave_c6_spawn(s32 arg0) {
    u8 *p = tt_air_enemies;
    s32 i = 0;

    do {
        p = tt_air_enemy_find_free(p, p + 0x1E00);
        if (p == 0) {
            break;
        }
        *(s32 *)(p + 0x20) = (i << 21) + 0xFFC00000;
        *(s32 *)(p + 0x24) = 0x01000000;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0xFFFE0000;
        *(s16 *)(p + 0x6E) = 0;
        p[0x50] = i;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        tt_air_enemy_setup(p, 0x47, 0xC);
        /* FAKE: the reset stores are written relative to the already advanced slot pointer (p - off); the original code does the increment first. T-2070 */
        i += 1;
        p += 0x78;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = 0x40;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 5);
}

void tt_air_enemy_kind47_move(void *enemy) {
    s32 shifted;
    s32 factor;
    s32 value;
    u16 state;
    u16 counter;
    u8 flag;

    state = *(u16 *)((u8 *)enemy + 0x6);
    switch (state) {
    case 0:
        factor = *(u8 *)((u8 *)enemy + 0x50) - 2;
        value = *(s32 *)((u8 *)enemy + 0x24);
        shifted = value >> 0x10;
        *(s32 *)((u8 *)enemy + 0x20) = (s32) ((factor << 0x15) + (factor * shifted * shifted * 0x30));
        if (value < 0x540000) {
            *(s32 *)((u8 *)enemy + 0x2c) = 0;
            *(u16 *)((u8 *)enemy + 0x6) = 1U;
            *(u16 *)((u8 *)enemy + 0x6e) = 0x400U;
        }
        *(u8 *)((u8 *)enemy + 0x3c) = 1;
        break;
    case 1:
        *(s32 *)((u8 *)enemy + 0x24) = (s32) ((func_800A0140((s32) *(u16 *)((u8 *)enemy + 0x6e)) << 9) + 0x500000);
        counter = *(u16 *)((u8 *)enemy + 0x6e);
        if (counter >= 0xC81U) {
            *(s32 *)((u8 *)enemy + 0x2c) = 0x20000;
            *(u16 *)((u8 *)enemy + 0x6e) = 0U;
            *(u16 *)((u8 *)enemy + 0x6) = 2U;
            counter = *(u16 *)((u8 *)enemy + 0x6e);
        }
        *(u16 *)((u8 *)enemy + 0x6e) = (u16) (counter + 0x30);
        *(u8 *)((u8 *)enemy + 0x3c) = 0;
        break;
    case 2:
        if ((u16) *(u16 *)((u8 *)enemy + 0x5e) >= 5U) {
            flag = *(u8 *)((u8 *)enemy + 0x60);
            if (flag == 0) {
                *(u16 *)((u8 *)enemy + 0x5e) = 0U;
                *(u8 *)((u8 *)enemy + 0x60) = (u8) (flag + 1);
                tt_enemy_shot_fire_aimed((s32) *(s16 *)((u8 *)enemy + 0x14), (s32) *(s16 *)((u8 *)enemy + 0x16), 0x20, 0xA0);
            }
        }
        if (*(u8 *)((u8 *)enemy + 0x51) != 0) {
            *(u16 *)((u8 *)enemy + 0x5e) = (u16) (*(u16 *)((u8 *)enemy + 0x5e) + 1);
        }
        break;
    }
}

void tt_air_wave_c7_spawn(u8 right_side) {
    u8 *p = tt_air_enemies;
    s32 i = 0;

    do {
        p = tt_air_enemy_find_free(p, p + 0x1E00);
        if (p == 0) {
            break;
        }
        if (right_side != 0) {
            *(s32 *)(p + 0x20) = 0x500000;
        } else {
            *(s32 *)(p + 0x20) = 0xFFB00000;
        }
        *(s32 *)(p + 0x24) = (-0x10 - i * 0x10) << 16;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0x20000;
        p[0x63] = right_side;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        tt_air_enemy_setup(p, 0x48, 0xD);
        /* FAKE: the reset stores are written relative to the already advanced slot pointer (p - off); the original code does the increment first. T-2070 */
        i += 1;
        p += 0x78;
        *(s16 *)(p - 0x1A) = 0;
        p[-0x18] = 0;
        *(s16 *)(p - 0x74) = 0x40;
        *(s16 *)(p - 0x6E) = 0;
        *(s16 *)(p - 0x70) = 0;
        *(s16 *)(p - 0x72) = 0;
        p[-0x69] = 0;
        p[-0x6A] = 0;
        p[-0x6B] = 0;
    } while (i != 6);
}

void tt_air_enemy_kind48_move(u8 *enemy) {
    switch (*(u16 *)(enemy + 6)) {
    case 0:
        if (*(s32 *)(enemy + 0x24) >= *(s32 *)(tt_player + 0x24)) {
            *(s32 *)(enemy + 0x2C) = 0;
            if (enemy[0x63] != 0) {
                *(s32 *)(enemy + 0x28) = 0xFFFE0000;
            } else {
                *(s32 *)(enemy + 0x28) = 0x20000;
            }
            *(u16 *)(enemy + 6) = 1;
        }
        break;
    case 1:
        if (enemy[0x63] != 0) {
            if (*(s32 *)(tt_player + 0x20) >= *(s32 *)(enemy + 0x20)) {
                *(s32 *)(enemy + 0x28) = 0;
                *(s32 *)(enemy + 0x2C) = 0xFFFE0000;
            }
        } else if (*(s32 *)(enemy + 0x20) >= *(s32 *)(tt_player + 0x20)) {
            *(s32 *)(enemy + 0x28) = 0;
            *(s32 *)(enemy + 0x2C) = 0xFFFE0000;
        }
        break;
    }
    if ((u32)*(u16 *)(enemy + 0x5E) >= 0x19) {
        if (enemy[0x60] == 0) {
            *(u16 *)(enemy + 0x5E) = 0;
            enemy[0x60] += 1;
            tt_enemy_shot_fire_aimed(*(s16 *)(enemy + 0x14), *(s16 *)(enemy + 0x16), 0x20, 0xA0);
        }
    }
    if (enemy[0x51] != 0) {
        *(u16 *)(enemy + 0x5E) += 1;
    }
}

void tt_stage_script_air_update(void) {
    TtStageScript *stage_script;

    stage_script = (TtStageScript *)tt_stage_script;
    while (1) {
        if (*(s16 *)stage_script->air_script_cursor == -1) {
            return;
        }
        if (stage_script->progress < (u32) * (s16 *)stage_script->air_script_cursor) {
            return;
        }
        switch (*(u16 *)(stage_script->air_script_cursor + 2)) {
        case 0xC0:
            tt_air_wave_c0_spawn(stage_script->air_script_cursor[4]);
            break;
        case 0xC1:
            tt_air_wave_c1_spawn(stage_script->air_script_cursor[4]);
            break;
        case 0xC2:
            tt_air_wave_c2_spawn(stage_script->air_script_cursor[4]);
            break;
        case 0xC3:
            tt_air_wave_c3_spawn(stage_script->air_script_cursor[4]);
            break;
        case 0xC4:
            tt_air_wave_c4_spawn(stage_script->air_script_cursor[4]);
            break;
        case 0xC5:
            tt_air_wave_c5_spawn(stage_script->air_script_cursor[4]);
            break;
        case 0xC6:
            tt_air_wave_c6_spawn(stage_script->air_script_cursor[4]);
            break;
        case 0xC7:
            tt_air_wave_c7_spawn(stage_script->air_script_cursor[4]);
            break;
        case 0x30:
        case 0x31:
            tt_cloud_spawn((s8)stage_script->air_script_cursor[4], *(u16 *)(stage_script->air_script_cursor + 2), stage_script->air_script_cursor[5]);
            break;
        }
        stage_script->air_script_cursor += 6;
    }
}

void tt_air_enemy_move(u8 *enemy) {
    s16 v;

    *(s32 *)(enemy + 0x20) += *(s32 *)(enemy + 0x28);
    *(s32 *)(enemy + 0x24) += *(s32 *)(enemy + 0x2C);
    *(s16 *)(enemy + 0x14) = *(s16 *)(enemy + 0x22);
    *(s16 *)(enemy + 0x16) = *(s16 *)(enemy + 0x26);
    if (enemy[0x51] != 0) {
        if ((v = *(s16 *)(enemy + 0x14)) < -0x90 || v >= 0x91 || (v = *(s16 *)(enemy + 0x16)) < -0x10 || v >= 0x101) {
            *(s16 *)(enemy + 4) = 0x80;
            *(s16 *)(enemy + 0xA) = 0;
            *(s16 *)(enemy + 8) = 0;
            *(s16 *)(enemy + 6) = 0;
            enemy[0xF] = 0;
            enemy[0xE] = 0;
            enemy[0xD] = 0;
            enemy[0x51] = 0;
        }
    } else {
        v = *(s16 *)(enemy + 0x14);
        if (v >= -0x7F && v < 0x80) {
            v = *(s16 *)(enemy + 0x16);
            if (v > 0 && v < 0xF0) {
                enemy[0x51] = 1;
            }
        }
    }
}

void tt_air_enemy_explode_init(u8 *enemy) {
    tt_se_request(TT_SE_ENEMY_EXPLODE, enemy);
    *(s32 *)(enemy + 0x28) = 0;
    *(s32 *)(enemy + 0x2C) = 0;
    *(u8 **)(enemy + 0x38) = tt_sprite_set_table;
    enemy[0x3C] = 0;
    enemy[0x3D] = 2;
    enemy[0x3E] = 9;
    enemy[0x40] = 0x20;
    *(s16 *)(enemy + 0x6E) = 0;
    *(s16 *)(enemy + 0xA) = 0;
    *(s16 *)(enemy + 8) = 0;
    *(s16 *)(enemy + 6) = 0;
    enemy[0xF] = 0;
    enemy[0xE] = 0;
    enemy[0xD] = 0;
}

void tt_air_enemy_explosion_spawn(u8 *enemy) {
    u8 *p;

    p = (u8 *)tt_air_enemy_find_free(tt_air_enemies, tt_air_enemies + 0x1E00);
    if (p != 0) {
        p[0] = 1;
        *(s16 *)(p + 4) = 0x71;
        /* FAKE: both chains on one line; as1 then emits the 0xE/0xD byte stores (zero kept in a0) first. T-11850 */
        *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 0xA) = 0; p[0xD] = p[0xE] = p[0xF] = 0;
        *(s32 *)(p + 0x20) = *(s32 *)(enemy + 0xC);
        *(s32 *)(p + 0x24) = *(s32 *)(enemy + 0x10);
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        tt_air_enemy_explode_init(p);
    }
}

/* FAKE: permuter source layout preserves branch operand order; original source unknown. T-9710. */
void tt_air_enemies_update(void)
{
    s32 var_s1;
    u16 temp_v0;

    TtObjectView *var_s0;
    tt_stage_script_air_update();
    var_s0 = (TtObjectView *) tt_air_enemies;
    var_s1 = 0;
    do
    {
        if (var_s0->active != 0)
        {
            temp_v0 = var_s0->state;
            switch (temp_v0)
            {
            case 0x71:
                break;

            case 0x40:
                switch (var_s0->kind)
                {
                case 0x40:
                    tt_air_enemy_kind40_move((u8 *) var_s0);
                    break;

                case 0x41:

                case 0x42:
                    tt_air_enemy_kind41_move((u8 *) var_s0);
                    break;

                case 0x43:
                    tt_air_enemy_kind43_move((u8 *) var_s0);
                    break;

                case 0x44:
                    tt_air_enemy_kind44_move((s32) var_s0);
                    break;

                case 0x45:
                    tt_air_enemy_kind45_move((s32) var_s0);
                    break;

                case 0x46:
                    tt_air_enemy_kind46_move((u8 *) var_s0);
                    break;

                case 0x47:
                    tt_air_enemy_kind47_move((void *) var_s0);
                    break;

                case 0x48:
                    tt_air_enemy_kind48_move((u8 *) var_s0);
                    break;

                }

                break;

            case 0x70:
                tt_air_enemy_explode_init((u8 *) var_s0);
                var_s0->state = 0x71U;
                var_s0->step2 = 0;
                var_s0->step = 0;
                var_s0->phase = 0;
                var_s0->tick2 = 0;
                var_s0->tick1 = 0;
                var_s0->tick0 = 0;
                break;

            case 0x80:
                var_s0->active = 0;
                var_s0->in_bounds = 0;
                break;

            }

            tt_air_enemy_move((u8 *) var_s0);

            if (0x40 == var_s0->state)
            {
                tt_animation_list_step((s32) (&var_s0->unk_38), (s32) (&var_s0->unk_5C), (s32) (&var_s0->unk_5D));
            }
            else
            if (0x71 == var_s0->state)
            {
                tt_sprite_animation_step((s32) (&var_s0->unk_38));
                if (var_s0->unk_40.b.lo & 1)
                {
                    var_s0->active = 0;
                }
            }
        }
        var_s1 += 1;
        var_s0 += 1;
    }
    while (var_s1 != 0x40);
}

void tt_air_enemies_draw(void) {
    u8 *buf;
    u8 *transform;
    u8 *air_enemy;
    u8 *v;
    s32 i;

    buf = *(s32 *)(tt_video + 8) * 0x80B8 + tt_frames;
    transform = tt_transforms;
    air_enemy = tt_air_enemies;
    for (i = 0; i != 0x40; i++, air_enemy += 0x78) {
        if (*air_enemy == 0) {
            continue;
        }
        if (*(u16 *)(air_enemy + 4) == 0x71) {
            tt_part_sprite_draw(air_enemy + 0x14, air_enemy + 0x38, *(u16 *)(air_enemy + 0x10));
        } else if (*(u16 *)(air_enemy + 2) == 0x47) {
            v = *(u8 **)(buf + 4);
            *(u8 **)(buf + 4) += 0x30;
            *(s16 *)(transform + 4) = *(s16 *)(air_enemy + 0x14);
            *(s16 *)(transform + 6) = *(s16 *)(air_enemy + 0x16);
            *(s16 *)(v + 8) = *(s16 *)(air_enemy + 0x14) - 8;
            *(s16 *)(v + 0xA) = *(s16 *)(air_enemy + 0x16) - 8;
            v[0x29] = 0;
            v[0x2A] = 1;
            *(s16 *)(v + 0x10) = 0x10;
            *(s16 *)(v + 0x12) = 0x10;
            v[0x28] = air_enemy[0x3C];
            v[0xC] = air_enemy[0x5C];
            v[0xD] = air_enemy[0x5D];
            *(u16 *)(v + 0x16) = *(u16 *)(air_enemy + 0x58);
            *(u16 *)(v + 0xE) = *(u16 *)(air_enemy + 0x5A);
            v[0x2C] = 0;
            if (*(u16 *)(air_enemy + 6) == 2) {
                v[0x2B] = 0;
                tt_sprite_sized_submit(&((TtDrawBuffer *)buf)->ordering[*(u16 *)(air_enemy + 0x10)], v, transform);
            } else {
                v[0x2B] = 1;
                *(s16 *)(transform + 0) = *(s16 *)(transform + 2) = ((u32)(*(u16 *)(air_enemy + 0x6E) - 0x400) >> 1) + 0xC00;
                if (*(s16 *)(transform + 0) > 0x1000) {
                    v[0x2B] = 0;
                }
                tt_sprite_sized_submit(&((TtDrawBuffer *)buf)->ordering[7], v, transform);
            }
        } else {
            v = *(u8 **)(buf + 4);
            *(u8 **)(buf + 4) += 0x30;
            *(s16 *)(transform + 4) = *(s16 *)(air_enemy + 0x14);
            *(s16 *)(transform + 6) = *(s16 *)(air_enemy + 0x16);
            if (*(u16 *)(air_enemy + 2) == 0x42) {
                *(s16 *)(v + 8) = *(s16 *)(air_enemy + 0x14) - 8;
                *(s16 *)(v + 0xA) = *(s16 *)(air_enemy + 0x16) - 0x10;
            } else {
                *(s16 *)(v + 8) = *(s16 *)(air_enemy + 0x14) - 8;
                *(s16 *)(v + 0xA) = *(s16 *)(air_enemy + 0x16) - 8;
            }
            v[0xC] = air_enemy[0x5C];
            v[0xD] = air_enemy[0x5D];
            v[0x28] = air_enemy[0x3C];
            v[0x29] = 0;
            v[0x2A] = 1;
            *(u16 *)(v + 0x16) = *(u16 *)(air_enemy + 0x58);
            *(u16 *)(v + 0xE) = *(u16 *)(air_enemy + 0x5A);
            v[0x2C] = 0;
            v[0x2B] = 0;
            tt_sprite16_submit(&((TtDrawBuffer *)buf)->ordering[*(u16 *)(air_enemy + 0x10)], v, transform);
            if (*(u16 *)(air_enemy + 2) == 0x42) {
                v = *(u8 **)(buf + 4);
                *(u8 **)(buf + 4) += 0x30;
                *(s16 *)(transform + 4) = *(s16 *)(air_enemy + 0x14);
                *(s16 *)(transform + 6) = *(s16 *)(air_enemy + 0x16);
                *(s16 *)(v + 8) = *(s16 *)(air_enemy + 0x14) - 8;
                *(s16 *)(v + 0xA) = *(s16 *)(air_enemy + 0x16);
                v[0xC] = 0xD0; v[0xD] = 0x10; /* FAKE: one line keeps as1's store order of the two uv bytes. T-11850 */
                v[0x28] = air_enemy[0x3C];
                v[0x29] = 0;
                v[0x2A] = 1;
                *(u16 *)(v + 0x16) = *(u16 *)(air_enemy + 0x58);
                *(u16 *)(v + 0xE) = *(u16 *)(air_enemy + 0x5A);
                v[0x2C] = 0;
                v[0x2B] = 0;
                tt_sprite16_submit(&((TtDrawBuffer *)buf)->ordering[*(u16 *)(air_enemy + 0x10)], v, transform);
            }
        }
    }
}

void tt_sprites_clear(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = tt_sprites; i < 0x80; i++, p += 0x64) {
        *p = 0;
    }
}

void tt_sprite_draw_tile(TtObjectView *arg0) {
    TtDrawBuffer *frame;
    TILE *v;
    u32 r;
    u32 g;
    u32 b;

    frame = ((TtVideoView *)tt_video)->index + (TtDrawBuffer *)tt_frames;
    v = (TILE *)frame->cursor;
    frame->cursor = (DR_MODE *)(v + 1);
    ((TtOtTag *)v)->len = 3;
    v->code = arg0->unk_3B;
    if (arg0->unk_44 & 8) {
        r = (arg0->unk_38 * tt_core[0x1D]) >> 7;
        if ((s32)r >= 0x100) {
            r = 0xFF;
        }
        g = (arg0->unk_39 * tt_core[0x1E]) >> 7;
        if ((s32)g >= 0x100) {
            g = 0xFF;
        }
        b = (arg0->unk_3A * tt_core[0x1F]) >> 7;
        if ((s32)b >= 0x100) {
            b = 0xFF;
        }
        v->r0 = r;
        v->g0 = g;
        v->b0 = b;
    } else {
        v->r0 = arg0->unk_38;
        v->g0 = arg0->unk_39;
        v->b0 = arg0->unk_3A;
    }
    v->x0 = arg0->x;
    v->y0 = arg0->y;
    v->w = arg0->unk_3C;
    v->h = arg0->unk_3E;
    ((TtOtTag *)v)->addr = *(u32 *)&frame->ordering[arg0->layer];
    (&frame->ordering[arg0->layer])->addr = (u32)v;
}

void tt_sprite_draw_textured(u8 *arg0) {
    DR_MODE *m;
    TtDrawBuffer *buffer;
    TtSpritePacket *q;
    buffer = (TtDrawBuffer *)(tt_frames + ((TtVideoView *)tt_video)->index * 0x80B8);
    q = (TtSpritePacket *)buffer->cursor;
    buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x14);
    q->tag.len = 4;
    if (*(u16 *)(arg0 + 0x44) & 8) {
        TtCore *core;

        core = (TtCore *)tt_core;
        q->code = arg0[0x3B] & 0xFFFE;
        q->r = core->fade_r;
        q->g = core->fade_g;
        q->b = core->fade_b;
    } else {
        q->code = arg0[0x3B];
        q->r = arg0[0x38];
        q->g = arg0[0x39];
        q->b = arg0[0x3A];
    }
    q->x = *(s16 *)(arg0 + 0x14);
    q->y = *(s16 *)(arg0 + 0x16);
    q->w = *(s16 *)(arg0 + 0x3C);
    q->h = *(s16 *)(arg0 + 0x3E);
    q->u = arg0[0x46];
    q->v = arg0[0x47];
    q->clut = *(u16 *)(arg0 + 0x42);
    q->tag.addr = buffer->ordering[*(u16 *)(arg0 + 0x10)].addr;
    buffer->ordering[*(u16 *)(arg0 + 0x10)].addr = (u32)q;
    m = buffer->cursor;
    buffer->cursor++;
    func_8009D294((s32)m, 0, 0, *(u16 *)(arg0 + 0x40), 0);
    ((TtOtTag *)m)->addr = buffer->ordering[*(u16 *)(arg0 + 0x10)].addr;
    buffer->ordering[*(u16 *)(arg0 + 0x10)].addr = (u32)m;
}

void tt_sprite_draw_transformed(TtObjectView *arg0) {
    TtDrawBuffer *buffer;
    u8 *q;

    buffer = ((TtVideoView *)tt_video)->index + (TtDrawBuffer *)tt_frames;
    q = (u8 *)buffer->cursor;
    buffer->cursor = (DR_MODE *)(q + 0x30);
    *(s16 *)(q + 8) = arg0->x;
    *(s16 *)(q + 0xA) = arg0->y;
    q[0xC] = 0x38; q[0xD] = 0xB8; /* one line, as a setUV0-style macro: the line orders the two stores */
    if (arg0->unk_44 & 8) {
        u8 *core;

        core = tt_core;
        q[4] = core[0x1D];
        q[5] = core[0x1E];
        q[6] = core[0x1F];
        q[0x2A] = 0;
    } else {
        q[4] = arg0->unk_38;
        q[5] = arg0->unk_39;
        q[6] = arg0->unk_3A;
        q[0x2A] = arg0->unk_4E;
    }
    q[0x29] = arg0->unk_4D;
    q[0x2B] = arg0->unk_4F;
    q[0x2C] = arg0->formation_index;
    *(s16 *)(q + 0x10) = arg0->unk_3C;
    *(s16 *)(q + 0x12) = arg0->unk_3E;
    q[0xC] = arg0->unk_46;
    q[0xD] = arg0->unk_47;
    *(u16 *)(q + 0x16) = arg0->unk_40.h;
    *(u16 *)(q + 0xE) = arg0->unk_42;
    q[0x28] = arg0->unk_4C;
    tt_sprite_sized_submit(&buffer->ordering[arg0->layer], q, arg0->unk_48);
}

/* FAKE: `if (1)` after the second case and the bound in the local n give the original's constant registers (the bound 0x80 apart from case 0x80); found with the permuter, T-9380 */
void tt_sprites_draw(void) {
    s32 i;
    s32 n;
    u8 *sprite;

    sprite = tt_sprites;
    i = 0;
    n = 0x80;
    do {
        if ((sprite[0] != 0) && (sprite[1] == 0)) {
            switch (sprite[0x3B] & ~3) {
            case 0x60:
                tt_sprite_draw_tile(sprite);
                break;
            case 0x64:
                tt_sprite_draw_textured(sprite);
                if (1) {
                    break;
                }
            case 0x80:
                tt_sprite_draw_transformed(sprite);
                break;
            }
        }
        i += 1;
        sprite += 0x64;
    } while (i != n);
}
