/* cycling_pickups.c: src/ovl/TT/8013BD10.c:276: pickup pool with motion, cloud spawn, shot-driven kind changes and popup labels; avoids unconfirmed bell identity; body 280: c = tt_run; 281: e = tt_player;. Original object 0x8013BD10-0x8013C6D0 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

u8 *tt_bell_find_free(u8 *cursor, u8 *end) {
    while (cursor < end) {
        if (*cursor == 0) {
            return cursor;
        }
        cursor += 0x6C;
    }
    return 0;
}

/* i = 0 before p = tt_bells gives the original's registers ($v0 counter, $v1 pointer); T-4060 */
void tt_bells_clear(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = tt_bells;
    for (; i < 0x20; i++) {
        *p = 0;
        p += 0x6C;
    }
}

void tt_bells_reset(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = tt_bells;
    for (; i < 0x20; i++) {
        *p = 0;
        *(s16 *)(p + 0x10) = 6;
        p += 0x6C;
    }
}

void tt_bell_setup(u8 *arg0) {
    arg0[0] = 1;
    *(s16 *)(arg0 + 2) = 0x20;
    *(s16 *)(arg0 + 0x52) = 0xA;
    *(s16 *)(arg0 + 0x54) = 0x3C84;
    *(s32 *)(arg0 + 0x38) = tt_animation_lists;
    arg0[0x40] = 0x80;
    *(s16 *)(arg0 + 0x10) = 6;
}

void tt_bell_set_popup_label(u8 *arg0, u16 arg1) {
    *(s16 *)(arg0 + 2) = arg1;
    switch (arg1) {
    case 40:
        arg0[0x56] = 0;
        arg0[0x57] = 0;
        break;
    case 41:
        /* FAKE: both stores on one source line give the original's li/sb order (as1 schedules by line, T-7020) */
        arg0[0x56] = 0x40; arg0[0x57] = 0x10;
        break;
    case 42:
        arg0[0x56] = 0x40;
        arg0[0x57] = 0;
        break;
    case 43:
        arg0[0x57] = 0x20;
        arg0[0x56] = 0;
        break;
    case 44:
        arg0[0x57] = 0x10;
        arg0[0x56] = 0;
        break;
    }
    arg0[0x3C] = 0;
    *(s16 *)(arg0 + 0x52) = 0xB;
    *(s16 *)(arg0 + 0x54) = 0x3C81;
}

void tt_bell_popup_spawn(u8 *arg0) {
    u8 *base;
    u8 *p;

    base = tt_bells;
    tt_se_request(TT_SE_POPUP);
    p = tt_bell_find_free(base, base + 0xD80);
    *(s32 *)(p + 0x20) = *(s32 *)(arg0 + 0xC);
    *(s32 *)(p + 0x24) = *(s32 *)(arg0 + 0x10);
    *(s16 *)(p + 0x14) = *(s16 *)arg0;
    *(s16 *)(p + 0x16) = *(s16 *)(arg0 + 2);
    *(s32 *)(p + 0x30) = 0;
    *(s32 *)(p + 0x34) = 0;
    *(s32 *)(p + 0x2C) = 0;
    *(s32 *)(p + 0x28) = 0;
    p[0] = 1;
    *(s32 *)(p + 0x38) = tt_animation_lists;
    p[0x40] = 0x80;
    *(s16 *)(p + 0x10) = 6;
    *(s16 *)(p + 0x62) = 0;
    *(s16 *)(p + 0x50) = 0;
    *(s16 *)(p + 4) = 0x90;
    /* FAKE: TT zero-store chains (bytes first, u16 view); their reloads leave dead uopt ranges that put base in $a2 and arg0 in $a3. T-11860 */
    p[0xD] = p[0xE] = p[0xF] = 0;
    *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 0xA) = 0;
    tt_bell_set_popup_label(p, 0x2C);
}

void tt_bell_init_motion_unused(u8 *arg0) {
    *(s32 *)(arg0 + 0x20) = 0;
    *(s32 *)(arg0 + 0x24) = 0x200000;
    *(s32 *)(arg0 + 0x28) = 0;
    *(s32 *)(arg0 + 0x2C) = 0;
    *(s32 *)(arg0 + 0x30) = 0;
    *(s32 *)(arg0 + 0x34) = 0xC00;
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
}

/* FAKE: one-word struct copies compile to lw/sw through $at, as the original's copies do; real source unknown. T-9110 */
typedef struct {
    s32 w;
} TtCopyWord;

void tt_bell_spawn_in_cloud(u8 *arg0) {
    u8 *p;

    p = tt_bell_find_free(tt_bells, tt_bells + 0xD80);
    *(TtCopyWord *)(p + 0x20) = *(TtCopyWord *)(arg0 + 0x20);
    *(TtCopyWord *)(p + 0x24) = *(TtCopyWord *)(arg0 + 0x24);
    *(TtCopyWord *)(p + 0x28) = *(TtCopyWord *)(arg0 + 0x28);
    *(TtCopyWord *)(p + 0x2C) = *(TtCopyWord *)(arg0 + 0x2C);
    *(s32 *)(p + 0x30) = 0;
    *(s32 *)(p + 0x34) = 0;
    p[0x59] = 0;
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    tt_bell_setup(p);
    *(s16 *)(p + 0x50) = 1;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 0xA) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 6) = 0;
    p[0xF] = 0;
    p[0xE] = 0;
    p[0xD] = 0;
}

typedef struct {
    /* 0x00 */ u8 flag;
    /* 0x01 */ u8 pad1[0x13];
    /* 0x14 */ s16 x;
    /* 0x16 */ s16 y;
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ s32 position_x; /* 16.16 position */
    /* 0x24 */ s32 position_y;
    /* 0x28 */ s32 velocity_x;
    /* 0x2C */ s32 velocity_y;
    /* 0x30 */ s32 acceleration_x;
    /* 0x34 */ s32 acceleration_y;
} TtMover; /* partial */

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s16 position_x_low;
    /* 0x22 */ s16 position_x_integer;
    /* 0x24 */ s16 position_y_low;
    /* 0x26 */ s16 position_y_integer;
} TtMoverHalfView; /* partial: the integer halves of the 16.16 positions at 0x20 and 0x24 */

void tt_bell_integrate(TtMover *mover) {
    mover->velocity_x += mover->acceleration_x;
    mover->velocity_y += mover->acceleration_y;
    mover->position_x += mover->velocity_x;
    mover->position_y += mover->velocity_y;
    mover->x = ((TtMoverHalfView *)mover)->position_x_integer;
    mover->y = ((TtMoverHalfView *)mover)->position_y_integer;
    if (mover->y >= 0x101) {
        mover->flag = 0;
    }
}

void tt_bells_update(void) {
    s32 i;
    u8 *run;
    u8 *p;

    run = tt_run;
    p = tt_bells;
    for (i = 0; i < 0x20; i++, p += 0x6C) {
        if (*p == 0) {
            continue;
        }
        switch (*(u16 *)(p + 4)) {
        case 0x40:
            break;
        case 0x90:
            *(u16 *)(p + 98) = *(u16 *)(p + 98) + 0x80;
            if (*(u16 *)(p + 98) > 0x1000) {
                *(u16 *)(p + 98) = 0x1000;
                *(u16 *)(p + 4) = 0x91;
            }
            break;
        case 0x91:
            *(s32 *)(p + 44) = *(s32 *)run;
            *(u16 *)(p + 90) = 0x3C;
            *(u16 *)(p + 4) = 0x92;
            break;
        case 0x92:
            *(s32 *)(p + 44) = *(s32 *)run;
            *(u16 *)(p + 90) = *(u16 *)(p + 90) - 1;
            if (*(u16 *)(p + 90) == 0) {
                *p = 0;
            }
            break;
        }
        tt_bell_integrate(p);
        if (*(u16 *)(p + 4) < 0x90) {
            tt_animation_list_step(p + 0x38, p + 0x56, p + 0x57);
        }
    }
}

/* T-11540/T-11550: matched by passing the ordering-table entry as &((TtDrawBuffer *)buf)->ordering[idx] (as in tt_sprite_draw_tile): the typed index gives the original addu buf, idx*4 operand order. */
void tt_bells_draw(void) {
    u8 *q;
    u8 *pickup;
    u8 *buf;
    u8 *v;
    s32 i;

    q = tt_transforms;
    pickup = tt_bells;
    i = 0;
    buf = *(s32 *)(tt_video + 8) * 0x80B8 + tt_frames;
    do {
        if (*pickup != 0 && !(*(u16 *)(pickup + 0x50) & 1)) {
            v = *(u8 **)(buf + 4);
            *(u8 **)(buf + 4) += 0x30;
            *(s16 *)(q + 4) = *(s16 *)(pickup + 0x14);
            *(s16 *)(q + 6) = *(s16 *)(pickup + 0x16);
            v[0x29] = 0;
            v[0x2A] = 1;
            v[0x28] = pickup[0x3C];
            *(u16 *)(v + 0x16) = *(u16 *)(pickup + 0x52);
            *(u16 *)(v + 0xE) = *(u16 *)(pickup + 0x54);
            v[0x2C] = 0;
            if (*(u16 *)(pickup + 4) < 0x90) {
                *(s16 *)(v + 8) = *(s16 *)(pickup + 0x14) - 8;
                *(s16 *)(v + 0xA) = *(s16 *)(pickup + 0x16) - 8;
                v[0xC] = pickup[0x56];
                v[0xD] = pickup[0x57] + 0x80;
                v[0x2B] = 0;
                tt_sprite16_submit(&((TtDrawBuffer *)buf)->ordering[*(u16 *)(pickup + 0x10)], v, q);
            } else {
                s16 t = *(s16 *)(pickup + 0x62);

                *(s16 *)(q + 2) = t;
                *(s16 *)q = t;
                *(s16 *)(v + 8) = *(s16 *)(pickup + 0x14) - 0x1F;
                *(s16 *)(v + 0xA) = *(s16 *)(pickup + 0x16) - 7;
                v[0xC] = pickup[0x56];
                v[0xD] = pickup[0x57];
                v[0x2B] = 1;
                *(s16 *)(v + 0x10) = 0x40;
                *(s16 *)(v + 0x12) = 0x10;
                *(u16 *)(v + 0x16) = 0xB;
                *(u16 *)(v + 0xE) = 0x3C81;
                v[0x28] = 0;
                tt_sprite_sized_submit(&((TtDrawBuffer *)buf)->ordering[*(u16 *)(pickup + 0x10)], v, q);
            }
        }
        i++;
        pickup += 0x6C;
    } while (i != 0x20);
}

void tt_bell_hit_by_shot(u8 *p, s16 d) {
    u8 *run;
    u8 *player;

    run = tt_run;
    player = tt_player;
    run[0xD]++;
    switch (*(u16 *)(p + 4)) {
    case 0x20:
        *(u16 *)(p + 4) = 0x40;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
    case 0x40:
        tt_se_request(TT_SE_PICKUP_HIT);
        if (run[0xD] >= 0x15) {
            run[0xD] = 1;
            if (++p[0x59] >= 2) {
                run[0xD] = 0;
                *(s32 *)(p + 0x38) = tt_animation_list_bell_hit;
                p[0x40] = 0x80;
                *(u16 *)(p + 4) = 0x60;
                p[0xD] = 0;
                p[0xE] = 0;
                p[0xF] = 0;
                *(s16 *)(p + 6) = 0;
                *(s16 *)(p + 8) = 0;
                *(s16 *)(p + 0xA) = 0;
            }
        }
        p[0x58] = tt_bell_kind_sequence[run[0xD]];
        switch (p[0x58]) {
        case 1:
            if (player[0x55] == 9) {
                p[0x58] = 0;
            }
            break;
        case 2:
            if (player[0x54] >= 0xA) {
                p[0x58] = 0;
            }
            break;
        case 3:
            if (player[0x53] != 0) {
                p[0x58] = 0;
            }
            break;
        case 6:
            if (player[0x54] == 0) {
                p[0x58] = 0;
            }
            break;
        }
        if (run) { /* FAKE: an extra use of run lowers its priority below the p[0x58] value (run in a3, player in t0). T-11810 */
        }
        *(s16 *)(p + 0x54) = p[0x58] + 0x3C84;
        *(u16 *)(p + 0x50) &= 0xFFFE;
        break;
    case 0x60:
        tt_se_request(TT_SE_PICKUP_HIT);
        if (run[0xD] >= 4) {
            *(s16 *)(p + 0x54) = 0x3C86;
            *(s32 *)(p + 0x38) = tt_animation_list_bell_fall;
            if (run) { /* FAKE: as above, stretches run over this block. T-11810 */
            }
            p[0x40] = 0xC0;
            *(s32 *)(p + 0x34) = 0;
            *(s32 *)(p + 0x2C) = 0x20000;
            *(s32 *)(p + 0x28) = 0;
            *(u16 *)(p + 4) = 0x70;
            p[0xD] = 0;
            p[0xE] = 0;
            p[0xF] = 0;
            *(s16 *)(p + 6) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 0xA) = 0;
            return;
        }
        break;
    case 0x70:
        tt_se_request(TT_SE_ENEMY_EXPLODE);
        p[0] = 0;
        break;
    }
    *(s32 *)(p + 0x34) = 0x1000;
    *(s32 *)(p + 0x2C) = 0xFFFE8000;
    if (d < -1) {
        *(s32 *)(p + 0x28) = 0x1000;
    } else if (d >= 2) {
        *(s32 *)(p + 0x28) = -0x1000;
    } else {
        *(s32 *)(p + 0x28) = 0;
    }
}
