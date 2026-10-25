/* boss_parts_and_hits.c: src/ovl/TT/80142570.c:4: field enemy destruction, boss sprite/part/hitbox clearing, hit tests and part animation/drawing; body 8: tt_se_request(0x60C); 9: p = tt_air_enemies;. Original object 0x80142570-0x80142D40 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

void tt_field_enemies_destroy(void) {
    u8 *p;
    s32 i;

    tt_se_request(0x60C);
    p = tt_air_enemies;
    i = 0;
    do {
        i++;
        if (*p != 0 && *(u16 *)(p + 4) == 0x40) {
            *(u16 *)(p + 4) = 0x70;
            /* FAKE: chained stores (u16 view); the reloads keep dead uopt ranges (no code) that move i, 0x70 and 0x40 to a1/a3/a2. T-11860 */
            *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 0xA) = 0;
            p[0xD] = p[0xE] = p[0xF] = 0;
        }
        p += 0x78;
    } while (i < 0x40);
    p = tt_ground_targets;
    for (i = 0; i < 0x20; i++) {
        if (*p != 0 && *(u16 *)(p + 4) == 0x40) {
            *(u16 *)(p + 4) = 0x70;
            /* FAKE: chained stores, as in the first loop. T-11860 */
            *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 0xA) = 0;
            p[0xD] = p[0xE] = p[0xF] = 0;
        }
        p += 0x78;
    }
}

void tt_boss_sprites_clear(void) {
    s32 i;
    u8 *boss = tt_boss;
    u8 *r = tt_sprites + 0xC80;

    boss[0] = 0;
    r[0xC8] = 0;
    r[0x64] = 0;
    r[0] = 0;
    r += 0x12C;
    for (i = 3; i < 0x1F; i++) {
        *r = 0;
        r += 0x64;
    }
}

void tt_boss_parts_hide_all(void) {
    u8 *boss = tt_boss;
    s32 i;

    for (i = 0; i < 0x18; i++) {
        boss[0x310 + i] = 0;
    }
}

void tt_boss_hitboxes_clear(void) {
    u8 *boss = tt_boss;
    s32 i;

    for (i = 0; i < 12; i++) {
        *(s32 *)(boss + 0x218 + i * 4) = 0;
    }
}

u8 tt_boss_check_hits(void) {
    u8 *core;
    u8 *run;
    u8 *boss;
    u8 *player;
    u8 *shot;
    s32 i;
    s32 j;
    s32 k;
    s8 ret; /* s8: an u8 ret = 1 would share the hoisted 1 (s0) of the 0x42 loop. T-11860 */

    core = tt_core;
    run = tt_run;
    boss = tt_boss;
    player = tt_player;
    shot = tt_shots;
    ret = 0;
    for (i = 0; i < 12; i++, shot += 0x54) {
        if (shot[0] != 0 && *(u16 *)(shot + 4) == 0x40) {
            for (j = 0; j < 4; j++) {
                if (*(u8 **)(boss + 0x218 + j * 4) != 0 && tt_box_overlap(shot + 0x14, &tt_hitbox_shot, boss + 0x14, *(u8 **)(boss + 0x218 + j * 4)) != 0) {
                    core[0x25] = core[0x25] + 1;
                    ret = 1;
                    if (core[0x24] != 0 && !(core[0x25] & 3)) {
                        if (run[0xB] < 2) {
                            if (!(core[0x25] & 7)) {
                                tt_enemy_shot_fire_fan4_aimed(*(s16 *)(shot + 0x14), *(s16 *)(shot + 0x16), 0x100, 0x20, 0xA0);
                            } else {
                                tt_enemy_shot_fire_3way_aimed(*(s16 *)(shot + 0x14), *(s16 *)(shot + 0x16), 0x100, 0x20, 0xA0);
                            }
                        } else {
                            tt_enemy_shot_fire_aimed(*(s16 *)(shot + 0x14), *(s16 *)(shot + 0x16), 0x20, 0xA0);
                        }
                    }
                    for (k = 0; k < 0x18; k++) {
                        boss[k * 0x14 + 0x42] = 1;
                    }
                    if (i >= 6) {
                        *(s16 *)(boss + 0x30C) -= 1;
                    } else if (*(u16 *)(shot + 2) == 9) {
                        *(s16 *)(boss + 0x30C) -= 4;
                    } else {
                        *(s16 *)(boss + 0x30C) -= 2;
                    }
                    *(u16 *)(shot + 4) = 0x80;
                    *(u16 *)(shot + 0xA) = 0;
                    *(u16 *)(shot + 8) = 0;
                    *(u16 *)(shot + 6) = 0;
                    shot[0xF] = 0;
                    shot[0xE] = 0;
                    shot[0xD] = 0;
                }
            }
            for (j = 4; j < 8; j++) {
                if (*(u8 **)(boss + 0x218 + j * 4) != 0 && tt_box_overlap(shot + 0x14, &tt_hitbox_shot, boss + 0x14, *(u8 **)(boss + 0x218 + j * 4)) != 0) {
                    *(u16 *)(shot + 4) = 0x80;
                    *(u16 *)(shot + 0xA) = 0;
                    *(u16 *)(shot + 8) = 0;
                    *(u16 *)(shot + 6) = 0;
                    shot[0xF] = 0;
                    shot[0xE] = 0;
                    shot[0xD] = 0;
                }
            }
        }
    }
    for (i = 4; i < 12; i++) {
        if (*(u8 **)(boss + 0x218 + i * 4) == 0) continue;
        if (player[0x52] != 0 || player[0] == 0 || tt_box_overlap(player + 0x14, tt_hitboxes, boss + 0x14, *(u8 **)(boss + 0x218 + i * 4)) == 0) continue;
        tt_player_hit();
        *(u16 *)(player + 4) = 0x60;
        *(u16 *)(player + 0xA) = 0;
        *(u16 *)(player + 8) = 0;
        *(u16 *)(player + 6) = 0;
        player[0xF] = 0;
        player[0xE] = 0;
        player[0xD] = 0;
    }
    return ret;
}

void tt_boss_part_animation_step(void *arg0, s32 arg1) {
    u8 *boss = tt_boss;
    TtPartAnimation *a = (TtPartAnimation *)(boss + 0x38) + arg1;
    s32 k;
    s16 *s;
    s16 v;

    /* FAKE: the two reads of the index tables go through an s32 view of tt_boss, so uopt reloads the pointer (lw t9) instead of reusing boss. T-11860 */
    if (boss[0] != 0 && ((u8 *)*(s32 *)&tt_boss)[arg1 + 0x310] != 0) {
        if (a->flags & 0x80) {
            a->flags &= ~0x80;
            a->frame = ((s16 *)arg0)[0];
            a->hold = ((s16 *)arg0)[1];
            boss[arg1 + 0x328] = 0;
            return;
        }
        if (a->flags & 0x20) {
            a->flags &= ~0x20;
            boss[arg1 + 0x328] = 0;
            return;
        }
        if (a->hold == 0) {
            k = ((u8 *)*(s32 *)&tt_boss)[arg1 + 0x328] + 2; /* FAKE: s32 view, see above. T-11860 */
            s = (s16 *)((u8 *)arg0 + k * 2);
            v = *s;
            if (v == -1) {
                a->flags |= 1;
                a->flags |= 0x40;
                a->frame = s[-2];
            } else if (v == -2) {
                a->frame = ((s16 *)arg0)[0];
                a->hold = ((s16 *)arg0)[1];
                k = 0;
            } else if (v == -3) {
                boss[arg1 + 0x310] = 0;
                a->flags |= 1;
            } else {
                a->frame = v;
                a->hold = s[1];
            }
            boss[arg1 + 0x328] = k;
        } else {
            a->hold--;
        }
    }
}

void tt_boss_draw(void) {
    s32 i;
    u8 *base;

    base = tt_boss;
    if (base[0] != 0) {
        for (i = 0; i != 0x18; i++) {
            if (base[i + 0x310] != 0) {
                *(s16 *)(base + 0x14) = *(s16 *)(base + 0x22) + *(s16 *)(base + 0x248 + i * 2);
                *(s16 *)(base + 0x16) = *(s16 *)(base + 0x26) + *(s16 *)(base + 0x278 + i * 2);
                tt_part_sprite_draw(base + 0x14, base + 0x38 + i * 0x14, *(u16 *)(base + 0x2A8 + i * 2));
            }
        }
        *(s16 *)(base + 0x14) = *(s16 *)(base + 0x22);
        *(s16 *)(base + 0x16) = *(s16 *)(base + 0x26);
    }
}
