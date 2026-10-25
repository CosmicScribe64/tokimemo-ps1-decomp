/* bomb_pool.c: src/ovl/TT/80146BE0.c:4: bomb pool; body 8: i = 0; 9: p = tt_bombs;. Original object 0x80146BE0-0x80147380 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

void tt_bombs_clear(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = tt_bombs;
    for (; i < 8; i++) {
        *p = 0;
        p += 0x58;
    }
}

void tt_bombs_reset(void) {
    s32 i;
    u8 *bomb;

    i = 0;
    bomb = tt_bombs;
    for (; i < 8; i++) {
        *(s16 *)(bomb + 0x10) = 3;
        *(s16 *)(bomb + 0x40) = 0xA;
        *(s16 *)(bomb + 0x42) = 0x3C81;
        bomb += 0x58;
    }
}

void tt_bomb_launch_init(u8 *arg0, s32 arg1) {
    u8 *p = tt_player;

    if (arg1 < 2) {
        if (arg0[0x3D] != 0) {
            *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) + 6;
        } else {
            *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) - 6;
        }
        *(s32 *)(arg0 + 0x20) = *(s16 *)(arg0 + 0x14) << 16;
        *(s16 *)(arg0 + 0x16) = *(s16 *)(p + 0x16) - 0x10;
        *(s32 *)(arg0 + 0x24) = *(s16 *)(arg0 + 0x16) << 16;
        *(s32 *)(arg0 + 0x2C) = 0xFFFC0000;
        arg0[0x44] = 0xA0;
        arg0[0x45] = 0xD0;
        *(s16 *)(arg0 + 0x10) = 3;
        *(s16 *)(arg0 + 0x40) = 0xA;
        *(s16 *)(arg0 + 0x42) = 0x3C81;
        *(s16 *)(arg0 + 0x46) = 0;
        return;
    }
    p = tt_options + (arg1 >> 1) * 0xA4 - 0xA4;
    if (arg0[0x3D] != 0) {
        *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) + 6;
    } else {
        *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) - 6;
    }
    *(s32 *)(arg0 + 0x20) = *(s16 *)(arg0 + 0x14) << 16;
    *(s16 *)(arg0 + 0x16) = *(s16 *)(p + 0x16) - 0x10;
    *(s32 *)(arg0 + 0x24) = *(s16 *)(arg0 + 0x16) << 16;
    *(s32 *)(arg0 + 0x2C) = 0xFFFC0000;
    arg0[0x44] = 0xA0;
    arg0[0x45] = 0xD0;
    *(s16 *)(arg0 + 0x10) = 3;
    *(s16 *)(arg0 + 0x40) = 0xA;
    *(s16 *)(arg0 + 0x42) = 0x3C81;
    *(s16 *)(arg0 + 0x46) = 0;
}

void tt_bomb_animation_step(u8 *arg0) {
    u32 v = (u16)++*(u16 *)(arg0 + 0x46);

    if (v >= 0x15) {
        arg0[0x44] = 0xC0;
    } else if (v >= 0xB) {
        arg0[0x44] = 0xB0;
    } else {
        arg0[0x44] = 0xA0;
    }
}

void tt_bomb_move(u8 *a, s32 idx) {
    u8 *p;
    u8 *q;
    s32 t;
    s32 i;

    p = tt_bomb_targets + idx * 0x58;
    *(s32 *)(a + 0x20) += *(s32 *)(a + 0x28);
    *(s32 *)(a + 0x24) += *(s32 *)(a + 0x2C);
    *(s16 *)(a + 0x14) = *(s16 *)(a + 0x22);
    *(s16 *)(a + 0x16) = *(s16 *)(a + 0x26);
    /* abs(dx) as the usual ABS() expansion; a local for the difference gives other registers. T-11860 */
    t = (*(s16 *)(a + 0x14) - *(s16 *)(p + 0x14) < 0) ? -(*(s16 *)(a + 0x14) - *(s16 *)(p + 0x14))
                                                      : *(s16 *)(a + 0x14) - *(s16 *)(p + 0x14);
    if (t < 2) {
        *(s32 *)(a + 0x28) = 0;
    } else {
        *(s32 *)(a + 0x28) = (*(s16 *)(p + 0x14) - *(s16 *)(a + 0x14)) << 12;
        t = (*(s16 *)(a + 0x14) < *(s16 *)(p + 0x14)) ? 0x20000 : -0x20000;
        *(s32 *)(a + 0x28) += t;
    }
    if (*(s16 *)(a + 0x16) < *(s16 *)(p + 0x16) || *(s16 *)(a + 0x16) < 0) {
        a[0] = 0;
        p[0] = 0;
        if (*(u16 *)(p + 0x44) != 0xFFFF) {
            q = tt_ground_targets + *(u16 *)(p + 0x44) * 0x78;
            if (*(u16 *)(q + 4) == 0x40) {
                *(u16 *)(q + 4) = 0x70;
                /* FAKE: TT zero-store chains; separate stores change registers. T-11860 */
                *(u16 *)(q + 6) = *(u16 *)(q + 8) = *(u16 *)(q + 0xA) = 0;
                q[0xD] = q[0xE] = q[0xF] = 0;
            }
        } else {
            q = tt_ground_targets;
            for (i = 0; i < 0x20; i++, q += 0x78) {
                if (*(u16 *)(q + 4) == 0x40) {
                    t = (*(s16 *)(p + 0x14) - *(s16 *)(q + 0x14) < 0) ? -(*(s16 *)(p + 0x14) - *(s16 *)(q + 0x14))
                                                                      : *(s16 *)(p + 0x14) - *(s16 *)(q + 0x14);
                    if (t < 8) {
                        t = (*(s16 *)(p + 0x16) - *(s16 *)(q + 0x16) < 0) ? -(*(s16 *)(p + 0x16) - *(s16 *)(q + 0x16))
                                                                          : *(s16 *)(p + 0x16) - *(s16 *)(q + 0x16);
                        if (t < 8) {
                            *(u16 *)(q + 4) = 0x70;
                            /* FAKE: TT zero-store chains; separate stores change registers. T-11860 */
                            *(u16 *)(q + 6) = *(u16 *)(q + 8) = *(u16 *)(q + 0xA) = 0;
                            q[0xD] = q[0xE] = q[0xF] = 0;
                        }
                    }
                }
            }
        }
    }
}


void tt_bombs_update(void) {
    u32 limit;
    s32 i;
    u16 mode;
    TtBomb *bomb;

    limit = 8;
    bomb = (TtBomb *)tt_bombs;
    i = 0;
    do {
        if (bomb->active != 0 && bomb->hidden == 0) {
            mode = bomb->state;
            switch (mode) {
            case 0x20:
                tt_bomb_launch_init((u8 *)bomb, i);
                bomb->state = 0x40U;
                bomb->step2 = 0;
                bomb->step = 0;
                bomb->phase = 0;
                bomb->tick2 = 0;
                bomb->tick1 = 0;
                bomb->tick0 = 0;
                break;
            case 0x40:
                tt_bomb_animation_step((u8 *)bomb);
                break;
            }
            tt_bomb_move((u8 *)bomb, i);
        }
        i++;
        bomb++;
    /* FAKE: narrowing the constant limit keeps 8 in s3 after 0x40. T-9520. */
    } while (i != (limit & 0xFF));
}

void tt_bombs_draw(void) {
    TtDrawBuffer *buffer;
    TtSpritePacket *q;
    DR_MODE *m;
    u8 *bomb;
    s32 i;
    s32 n;

    bomb = tt_bombs;
    buffer = (TtDrawBuffer *)(tt_frames + ((TtVideoView *)tt_video)->index * 0x80B8);
    for (i = 0, n = 8; i != n; i++, bomb += 0x58) {
        if (bomb[0] != 0 && !(*(u16 *)(bomb + 0x3E) & 1)) {
            q = (TtSpritePacket *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x14);
            q->tag.len = 4; q->code = 0x65;
            q->x = *(s16 *)(bomb + 0x22) - 8;
            q->y = *(s16 *)(bomb + 0x26) - 8;
            q->h = 0x10;
            q->w = 0x10;
            q->u = bomb[0x44];
            q->v = bomb[0x45];
            q->clut = *(u16 *)(bomb + 0x42);
            q->tag.addr = buffer->ordering[*(u16 *)(bomb + 0x10)].addr;
            buffer->ordering[*(u16 *)(bomb + 0x10)].addr = (u32)q;
            m = buffer->cursor;
            buffer->cursor++;
            func_8009D294((s32)m, 0, 0, *(u16 *)(bomb + 0x40), 0);
            ((TtOtTag *)m)->addr = buffer->ordering[*(u16 *)(bomb + 0x10)].addr;
            buffer->ordering[*(u16 *)(bomb + 0x10)].addr = (u32)m;
        }
    }
}
