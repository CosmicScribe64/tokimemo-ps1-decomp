/* sprite_animation_and_followers.c: src/ovl/TT/801478D0.c:4: part-sprite animation/drawing plus following-ship trail updates; body 13: temp_v0 = arg0->unk_08; 14: if (temp_v0 & 0x80) {. Original object 0x801478D0-0x80148210 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/tt.h"

void tt_sprite_animation_step(TtAnimationView *arg0) {
    TtAnimFrame *frame;
    TtAnimFrame ***temp_v0_2;
    u8 temp_a1;
    u8 temp_a2;
    u8 temp_t1;
    u8 temp_t9;
    u8 temp_v0;

    temp_v0 = arg0->flags;
    if (temp_v0 & 0x80) {
        arg0->flags = (u8) (temp_v0 & 0xFF7F);
        arg0->frame = 0U;
        frame = **((TtAnimFrame ***)tt_sprite_set_table + arg0->set);
        arg0->hold = frame->hold;
        return;
    }
    if (temp_v0 & 0x20) {
        arg0->flags = (u8) (temp_v0 & 0xFFDF);
        frame = **((TtAnimFrame ***)tt_sprite_set_table + arg0->set) + arg0->frame;
        arg0->hold = frame->hold;
        return;
    }
    if (!(temp_v0 & 0x40) && (--arg0->hold == 0)) {
        temp_v0_2 = arg0->set + (TtAnimFrame ***)tt_sprite_set_table;
        temp_a1 = arg0->frame;
        frame = **temp_v0_2 + temp_a1;
        temp_a2 = frame->loop_back;
        temp_t1 = temp_a1 + 1;
        if (temp_a2 == 0) {
            arg0->frame = temp_t1;
            frame = **temp_v0_2 + (temp_t1 & 0xFF);
            arg0->hold = frame->hold;
            return;
        }
        temp_t9 = temp_a1 - temp_a2;
        if (temp_a2 == 0xFF) {
            arg0->flags = (u8) (arg0->flags | 1);
            return;
        }
        arg0->frame = temp_t9;
        frame = **temp_v0_2 + (temp_t9 & 0xFF);
        arg0->hold = frame->hold;
    }
}

void tt_part_sprite_draw(arg0, arg1, arg2)
    u8 *arg0;
    TtSpriteAnimation *arg1;
    u16 arg2;
{
    s32 pad[2]; /* FAKE: unused, sizes the 0x78 frame with the call spill at sp+0x50. T-11850 */
    TtDrawBuffer *buf;
    u8 *transform;
    TtSpriteSet **sets;
    TtSpriteSet *set;
    TtSpriteFrame *fr;
    TtSpriteSequence *seq;
    TtSpritePart *part;
    u8 *pal;
    u16 clut;
    s32 n;
    u8 *v;
    u8 *c;
    s32 code;
    s32 idx;
    s32 x;
    s32 y;
    u8 b;

    buf = (TtDrawBuffer *)(tt_frames + ((TtVideoView *)tt_video)->index * 0x80B8);
    transform = tt_transforms;
    sets = arg1->sets;
    if (sets != 0) {
        set = sets[arg1->set];
        fr = &set->frames[arg1->frame];
        seq = &set->seqs[fr->seq];
        part = set->parts + seq->start + seq->count - 1;
        pal = set->attr;
        clut = set->clut;
        *(s16 *)(transform + 4) = *(s16 *)(arg0 + 0);
        *(s16 *)(transform + 6) = *(s16 *)(arg0 + 2);
        if (arg1->transform_enable != 0) {
            *(s16 *)(transform + 0) = arg1->x;
            *(s16 *)(transform + 2) = arg1->y;
        }
        for (n = seq->count; n > 0; n--, part--) {
            v = (u8 *)buf->cursor;
            buf->cursor = (DR_MODE *)(v + 0x30);
            v[0x28] = part->flags ^ arg1->flip;
            code = arg1->flip;
            if (code & 1) {
                y = *(s16 *)(arg0 + 2) - part->y - 0x10;
            } else {
                y = part->y + *(s16 *)(arg0 + 2);
            }
            if (code & 2) {
                x = *(s16 *)(arg0 + 0) - part->x - 0x10;
            } else {
                x = part->x + *(s16 *)(arg0 + 0);
            }
            *(s16 *)(v + 8) = x;
            *(s16 *)(v + 0xA) = y;
            idx = part->tile >> 6;
            c = pal + idx;
            code = part->tile & 0x3F;
            v[0xC] = (*c & 0x80) + (code & 7) * 16;
            v[0xD] = (*c & 0x40) * 2 + (code >> 3) * 16;
            v[0x29] = 0;
            v[0x2A] = 1;
            *(s16 *)(v + 0x16) = *c & 0x1F;
            if (arg1->clut_override != 0) {
                b = arg1->clut;
                *(s16 *)(v + 0xE) = clut + (b & 0xF) + (b & 0xF0) * 4;
            } else {
                b = part->clut;
                *(s16 *)(v + 0xE) = clut + (b & 0xF) + (b & 0xF0) * 4;
            }
            v[0x2C] = 0;
            v[0x2B] = arg1->transform_enable;
            tt_sprite16_submit(&buf->ordering[arg2], v, transform);
        }
    }
}

void tt_options_clear(void) {
    s32 i = 0;
    u8 *follower = tt_options;

    do {
        i += 1;
        follower += 0xA4;
        follower[-0xA4] = 0;
    } while (i != 3);
}

void tt_options_reset(void) {
    s32 i;
    u8 *follower;
    u8 *q;
    int new_var; /* FAKE: the dead test of tt_options reserves a0/a1 so the hoisted constants start at a2, as in the original; same trick as tt_shots_reset. T-10910 */

    for (i = 0, follower = tt_options; i < 3; i++, follower += 0xA4) {
        follower[0] = 0;
        new_var = tt_options && tt_options;
        *(s16 *)(follower + 2) = 4;
        *(s16 *)(follower + 0x10) = 2;
        *(u8 **)(follower + 0x38) = tt_sprite_set_table;
        follower[0x3C] = 0;
        follower[0x3D] = 0;
        follower[0x3E] = 0x37;
        follower[0x40] = 0x60;
        follower[0x48] = 0;
        *(s16 *)(follower + 4) = 0x40;
        *(s16 *)(follower + 0xA) = 0;
        *(s16 *)(follower + 8) = 0;
        *(s16 *)(follower + 6) = 0;
        follower[0xF] = 0;
        if (new_var && tt_options) { /* FAKE: see above. T-10910 */
        }
        follower[0xE] = 0;
        follower[0xD] = 0;
    }
    for (i = 0, q = tt_arms + 0xC0; i < 6; i++, q += 0x60) {
        q[0] = 0;
        *(s16 *)(q + 2) = 0;
        *(s16 *)(q + 4) = 0;
        q[0xC] = 0;
        *(s16 *)(q + 0xA) = 0;
        *(s16 *)(q + 8) = 0;
        *(s16 *)(q + 6) = 0;
        q[0xF] = 0;
        q[0xE] = 0;
        q[0xD] = 0;
        *(s16 *)(q + 0x10) = 2;
        *(u8 **)(q + 0x38) = tt_sprite_set_table;
        q[0x3C] = 0;
        q[0x3D] = 0;
        q[0x3E] = 0;
        q[0x40] = 0x60;
    }
}

void tt_option_trail_step(u8 *option) {
    s32 i;
    u8 *player;

    player = tt_player;
    if (option == tt_options) {
        *(s16 *)(option + 0x62) = *(s16 *)(player + 0x14);
        *(s16 *)(option + 0x82) = *(s16 *)(player + 0x16);
    } else {
        *(s16 *)(option + 0x62) = *(s16 *)(option - 0x54);
        *(s16 *)(option + 0x82) = *(s16 *)(option - 0x34);
    }
    for (i = 1; i < 10; i++) {
        *(s16 *)(option + 0x4E + i * 2) = *(s16 *)(option + 0x50 + i * 2);
        *(s16 *)(option + 0x6E + i * 2) = *(s16 *)(option + 0x70 + i * 2);
    }
    *(s16 *)(option + 0x14) = *(s16 *)(option + 0x50);
    *(s16 *)(option + 0x16) = *(s16 *)(option + 0x70);
}

/* Three-store chains (u16 6, 8, A; bytes D, E, F) keep their zero in v1/a1 like the original; two-store chains or s16
 * give $zero stores. The loop is unrolled by IDO (T-11850). */
void tt_option_activate(u8 *option) {
    s32 i;
    u8 *src;

    src = tt_player;
    option[0] = 1;
    *(s16 *)(option + 4) = 0x40;
    /* FAKE: both chains on one line give the original store order (as1 schedules by source line). T-11850 */
    *(u16 *)(option + 6) = *(u16 *)(option + 8) = *(u16 *)(option + 0xA) = 0; option[0xD] = option[0xE] = option[0xF] = 0;
    *(s16 *)(option + 0x14) = *(s16 *)(src + 0x14);
    *(s16 *)(option + 0x16) = *(s16 *)(src + 0x16);
    for (i = 0; i < 10; i++) {
        ((s16 *)(option + 0x50))[i] = *(s16 *)(src + 0x14);
        ((s16 *)(option + 0x70))[i] = *(s16 *)(src + 0x16);
    }
}

void tt_options_update(void) {
    s32 i;
    u8 *player;
    u8 *follower;
    u8 *b;

    player = tt_player;
    if (player[0x53] != 0) {
        follower = tt_options;
        i = 0;
        b = tt_arms + 0xC0;
        for (; i < 3; i++) {
            tt_option_trail_step(follower);
            b[0x3E] = player[0x5A] + 0x38;
            follower += 0xA4;
            b += 0xC0;
            b[-0x22] = player[0x5B] + 0x3D;
        }
    }
}

void tt_options_draw(void) {
    s32 i;
    u8 *pos;
    u8 *p;
    u8 *q;

    if (tt_player[0x53] != 0) {
        for (i = 0, p = tt_options, q = tt_arms + 0xC0; i != 3; i++, p += 0xA4, q += 0x60) {
            pos = p + 0x14;
            if (p[0] != 0) {
                tt_part_sprite_draw(pos, p + 0x38, *(u16 *)(p + 0x10));
                tt_part_sprite_draw(pos, q + 0x38, *(u16 *)(q + 0x10));
                q += 0x60;
                tt_part_sprite_draw(pos, q + 0x38, *(u16 *)(q + 0x10));
            }
        }
    }
}
