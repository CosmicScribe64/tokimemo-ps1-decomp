/* text_and_time_display.c: src/ovl/TT/80148210.c:4: text pool, sprite-font records, time/name/countdown formatting and random drop helper; body 8: i = 0; 9: p = tt_texts;. Original object 0x80148210-0x801487D0 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/tt.h"

void tt_texts_clear(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = tt_texts;
    for (; i < 0x80; i++) {
        *p = 0;
        p += 0x14;
    }
}

void tt_text_draw(TtText *r) {
    u8 *s;
    s16 x;
    u16 i;
    u8 c;
    TtDrawBuffer *buffer;
    TtRectPacket *q;

    buffer = (TtDrawBuffer *)(tt_frames + ((TtVideoView *)tt_video)->index * 0x80B8);
    s = tt_text_strings[r->text];
    x = r->x;
    i = 0;
    c = *s++;
    while (c != 0xFF) {
        if (r->limited != 0 && i >= r->count) {
            break;
        }
        if (c != 0x3F) {
            q = (TtRectPacket *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x10);
            if (r->flags & 4) {
                q->tag.len = 3;
                q->code = 0x74;
                q->r = r->rgb[0];
                q->g = r->rgb[1];
                q->b = r->rgb[2];
            } else if (r->flags & 8) {
                TtCore *core;

                core = (TtCore *)tt_core;
                q->code = 0x74;
                q->tag.len = 3;
                q->r = core->fade_r;
                q->g = core->fade_g;
                q->b = core->fade_b;
            } else {
                q->tag.len = 3;
                q->code = 0x75;
            }
            /* T-11000: IDO orders these stores by source line; keep them on one line. */
            q->x = x; q->y = r->y; q->u = (c & 0xF) * 8; q->v = (c >> 4) * 8; q->clut = (r->palette & 0xF) + 0x3C00; q->tag.addr = buffer->ordering[r->layer].addr; buffer->ordering[r->layer].addr = (u32)q;
        }
        c = *s;
        x += 8;
        i++;
        s++;
    }
}

void tt_texts_draw(void) {
    /* FAKE: unused homes retain the observed 0x50-byte frame. T-9760. */
    s32 frame_pad[2];
    TtDrawBuffer *buffer;
    DR_MODE *packet;
    TtText *text_record;
    s32 i;

    buffer = (TtDrawBuffer *)(tt_frames + ((TtVideoView *)tt_video)->index * 0x80B8);
    text_record = (TtText *)tt_texts;
    i = 0;
    do {
        if ((text_record->active != 0) && !(text_record->flags & 1)) {
            tt_text_draw(text_record);
        }
        i++;
        text_record++;
    } while (i != 0x80);
    packet = buffer->cursor;
    buffer->cursor++;
    func_8009D294((s32)packet, 0, 0, 0xA, 0);
    ((TtOtTag *)packet)->addr = buffer->ordering[0].addr;
    buffer->ordering[0].addr = (u32)packet;
}

void tt_time_text_set(u16 arg0, s32 arg1) {
    u32 t = arg0;
    u8 *p = tt_text_strings[arg1];

    if (t == 0x8CA0) {
        p[0] = 9;
        p[2] = 5;
        p[3] = 9;
        p[5] = 9;
        p[6] = 9;
        return;
    }
    p[0] = t / 3600;
    p[2] = (t % 3600) / 600;
    p[3] = (t % 600) / 60;
    p[5] = (t % 60) * 10 / 60;
    p[6] = t * 10 / 6 % 10;
}

void tt_name_text_set(s32 arg0, s32 arg1) {
    memcpy(tt_text_strings[arg1], arg0, 3);
}

void tt_countdown_text_set(u16 arg0, s32 arg1) {
    u8 *p = tt_text_strings[arg1];

    arg0 = arg0 % 100;
    p[6] = arg0 / 10;
    p[7] = arg0 % 10;
}

s32 tt_drop_roll(void) {
    u8 *p;

    p = tt_text_strings_tail[0];
    if (p[5] == 0 && p[6] == 0) {
        return 1;
    }
    if (p[5] == 3 && p[6] == 3) {
        return 2;
    }
    return 0;
}
