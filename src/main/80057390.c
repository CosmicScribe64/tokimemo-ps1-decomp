#include "common.h"
#include "game.h"
#include "psx_types.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
s32 D_800B5920[4] = { 0, 0, 0, 0 };
s32 D_800B5930 = 0;
s32 D_800B5934 = 0;
u8 D_800B5938[4] = { 0 };
u8 D_800B593C = 0;
u8 D_800B5940 = 0;
s32 D_800B5944 = 0;
s32 D_800B5948 = 0;
s32 D_800B594C = 0;

void set_dec_bri(u8 arg0) {
    D_800B593C = arg0;
}

s32 get_last_gamen_mode(void) {
    return D_800B5948;
}

void dec_bg_reset(void) {
    D_800B5920[3] = 0;
    D_800B5920[1] = 0;
    D_800B5920[2] = 0;
    D_800B5920[0] = 0;
    D_800B5938[1] = 0;
    D_800B5938[0] = 0;
    D_800B593C = 0x80;
    D_800B5948 = -1;
    D_800B5934 = 0;
    D_800B5938[2] = 0;
}

void dec_bg_show_switch(s32 arg0) {
    D_800B5938[arg0] = 1 - D_800B5938[arg0];
}

void dec_bg_show_set(s32 arg0, s32 arg1) {
    D_800B5938[arg0] = arg1 & 1;
}

s32 dec_bg_cd_read(s32 arg0, s32 arg1) {
    if (arg1 == 1) {
        if (arg0 == D_800B5920[2]) {
            return 0;
        }
        if (arg0 == D_800B5920[3]) {
            return 1;
        }
        func_800462C8(0xA, 0x80162000, arg0);
        *(s32 *)((u8 *)&D_800B5920[3] + -(D_800B5939 * 4)) = arg0;
        return -1;
    }
    if (arg0 == D_800B5920[0]) {
        return 0;
    }
    if (arg0 == D_800B5920[1]) {
        return 1;
    }
    func_800462C8(0xA, 0x80162000, arg0);
    *(s32 *)((u8 *)&D_800B5920[1] + -(D_800B5938[0] * 4)) = arg0;
    return -1;
}

s32 func_8005751C(s32 arg0) {
    D_800B594C = 0;
    func_80053650(0x80167000, 0x80162000, 0x80162000);
    if (arg0 == 0) {
        switch (1 - D_800B5938[arg0]) {
        case 0:
            D_800B594C = func_800536AC(0x2C0, 0xA0, 0x140, 0x58);
            return 0;
        case 1:
            D_800B594C = func_800536AC(0x140, 0x1A0, 0x140, 0x58);
            return 1;
        }
    } else if (arg0 == 1) {
        switch (1 - D_800B5938[arg0]) {
        case 0:
            D_800B594C = func_800536AC(0x2C0, 0, 0x140, 0xA0);
            return 0;
        case 1:
            D_800B594C = func_800536AC(0x140, 0x100, 0x140, 0xA0);
            return 1;
        }
    }
}

void func_80057640(s32 arg0) {
    s32 v;
    RECT rect;

    rect.x = 0;
    rect.y = D_8011ECA0 * 0xF0;
    rect.w = 0x140;
    rect.h = arg0;
    v = 1 - (s32)D_800B5939;
    (&D_800B5920[2])[v] = 0;
    switch (v) {
    case 0:
        func_8009C93C(&rect, 0x2C0, 0);
        v = 1 - (s32)D_800B5939;
        break;
    case 1:
        func_8009C93C(&rect, 0x140, 0x100);
        v = 1 - (s32)D_800B5939;
        break;
    }
    /* FAKE: the cast makes IDO recompute 1 - (s32)D_800B5939 for the argument instead of passing v (decomp-permuter, score 0); real source unknown. T-4090 */
    dec_bg_show_set(1, (u32) (1 - (s32)D_800B5939));
    D_800B5944 = 0;
    func_8009C674(0);
}

s32 func_80057710(void) {
    s16 width;
    s16 x;
    s16 band;
    s32 doubled;
    s32 offset;
    s32 edge;
    s32 point;

    D_800B593C -= 8;
    if (D_800B593C >= 0x81U) {
        D_800B593C = 0;
    }
    D_800B5944++;
    doubled = D_800B5944 * 2;
    width = doubled;
    if ((s16)doubled > 0x20) {
        width = 0x20;
    }
    band = 0;
    do {
        offset = band << 5;
        x = 0;
        if (offset < 0xA0) {
            edge = offset + 0x1F;
            while (x < width) {
                point = edge - x;
                func_80049FF0((s16)(point - 0xA0), -0x78,
                             -0xA0, (s16)(point - 0x78), 0, 0, 0xD);
                x++;
            }
        } else if (offset < 0x140) {
            edge = offset + 0x1F;
            while (x < width) {
                point = edge - x;
                func_80049FF0((s16)(point - 0xA0), -0x78,
                             (s16)(point - 0x140), 0x28, 0, 0, 0xD);
                x++;
            }
        } else {
            edge = offset + 0x1F;
            while (x < width) {
                point = edge - x;
                func_80049FF0(0xA0, (s16)(point - 0x1B8),
                             (s16)(point - 0x140), 0x28, 0, 0, 0xD);
                x++;
            }
        }
        band++;
    } while (band < 0xF);
}

void func_800578F4(s32 a) {
    u8 *mode_flags;
    SPRT *sprite;
    u8 *flags;

    if (a != -1) {
        D_800B5948 = a;
        sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
        func_8009F0A4(sprite);
        SetSemiTrans(sprite, 0);
        SetShadeTex(sprite, 0);
        sprite->r0 = (u8) D_800B593C;
        sprite->g0 = (u8) D_800B593C;
        sprite->b0 = (u8) D_800B593C;
        if (a == 1) {
            sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
            sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
            sprite->v0 = 0;
            sprite->u0 = 0;
            setWH(sprite, 0xC0, 0xA0);
        } else if (a == 2) {
            sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
            sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
            sprite->v0 = 0;
            sprite->u0 = 0;
            setWH(sprite, 0xC0, 0xF0);
        } else {
            sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
            sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
            sprite->v0 = 0xA0;
            sprite->u0 = 0;
            setWH(sprite, 0xC0, 0x58);
        }
        AddPrim((D_8011ECA0 << 0xA) + 0x34 + D_800E8CA0, sprite);
        flags = &D_800B5938[a];
        mode_flags = flags;
        if (*flags != 0) {
            goto_tpage(0x140, 0x100, 0xD, 0, 2);
        } else {
            goto_tpage(0x2C0, 0, 0xD, 0, 2);
        }
        sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
        func_8009F0A4(sprite);
        SetSemiTrans(sprite, 0);
        SetShadeTex(sprite, 0);
        sprite->r0 = (u8) D_800B593C;
        sprite->g0 = (u8) D_800B593C;
        sprite->b0 = (u8) D_800B593C;
        if (a == 1) {
            sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20);
            sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
            sprite->v0 = 0;
            sprite->u0 = 0;
            setWH(sprite, 0x80, 0xA0);
        } else if (a == 2) {
            sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20);
            sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
            sprite->v0 = 0;
            sprite->u0 = 0;
            setWH(sprite, 0x80, 0xF0);
        } else {
            sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20);
            sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
            sprite->v0 = 0xA0;
            sprite->u0 = 0;
            setWH(sprite, 0x80, 0x58);
        }
        AddPrim((D_8011ECA0 << 0xA) + 0x34 + D_800E8CA0, sprite);
        if (*mode_flags != 0) {
            goto_tpage(0x200, 0x100, 0xD, 0, 2);
            return;
        }
        goto_tpage(0x380, 0, 0xD, 0, 2);
    }
}

void func_80057D1C(u8 arg0) {
    D_800B5940 = arg0;
}

void func_80057D28(s32 arg0) {
    /* FAKE: unused word retains the original spill home and frame. T-14200. */
    s32 pad;
    u8 *mode_flags;
    SPRT *sprite;
    u8 *flags;

    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 1);
    SetShadeTex(sprite, 0);
    sprite->r0 = (s8) (0x80 - D_800B5940);
    sprite->g0 = (s8) (0x80 - D_800B5940);
    sprite->b0 = (s8) (0x80 - D_800B5940);
    if (arg0 == 1) {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0xC0, 0xA0);
    } else {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0xC0, 0x58);
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x28 + D_800E8CA0, sprite);
    flags = &D_800B5938[arg0];
    mode_flags = flags;
    if (*flags != 0) {
        goto_tpage(0x140, 0x100, 0xA, 1, 2);
    } else {
        goto_tpage(0x2C0, 0, 0xA, 1, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 1);
    SetShadeTex(sprite, 0);
    sprite->r0 = (s8) (0x80 - D_800B5940);
    sprite->g0 = (s8) (0x80 - D_800B5940);
    sprite->b0 = (s8) (0x80 - D_800B5940);
    if (arg0 == 1) {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0x80, 0xA0);
    } else {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0x80, 0x58);
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x28 + D_800E8CA0, sprite);
    if (*mode_flags != 0) {
        goto_tpage(0x200, 0x100, 0xA, 1, 2);
    } else {
        goto_tpage(0x380, 0, 0xA, 1, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    SetShadeTex(sprite, 0);
    sprite->r0 = (u8) D_800B5940;
    sprite->g0 = (u8) D_800B5940;
    sprite->b0 = (u8) D_800B5940;
    if (arg0 == 1) {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0xC0, 0xA0);
    } else {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0xC0, 0x58);
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x34 + D_800E8CA0, sprite);
    if (*mode_flags != 0) {
        goto_tpage(0x140, 0x100, 0xD, 0, 2);
    } else {
        goto_tpage(0x2C0, 0, 0xD, 0, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    SetShadeTex(sprite, 0);
    sprite->r0 = (u8) D_800B5940;
    sprite->g0 = (u8) D_800B5940;
    sprite->b0 = (u8) D_800B5940;
    if (arg0 == 1) {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0x80, 0xA0);
    } else {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0x80, 0x58);
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x34 + D_800E8CA0, sprite);
    if (*mode_flags != 0) {
        goto_tpage(0x200, 0x100, 0xD, 0, 2);
        return;
    }
    goto_tpage(0x380, 0, 0xD, 0, 2);
}

void func_80058398(s32 arg0) {
    /* FAKE: unused word retains the original spill home and frame. T-14200. */
    s32 pad;
    u8 *mode_flags;
    SPRT *sprite;
    u8 *flags;

    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 1);
    SetShadeTex(sprite, 0);
    sprite->r0 = (s8) (0x80 - D_800B5940);
    sprite->g0 = (s8) (0x80 - D_800B5940);
    sprite->b0 = (s8) (0x80 - D_800B5940);
    if (arg0 == 1) {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0xC0, 0xA0);
    } else {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0xC0, 0x58);
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x28 + D_800E8CA0, sprite);
    flags = &D_800B5938[arg0];
    mode_flags = flags;
    if (*flags == 0) {
        goto_tpage(0x140, 0x100, 0xA, 1, 2);
    } else {
        goto_tpage(0x2C0, 0, 0xA, 1, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 1);
    SetShadeTex(sprite, 0);
    sprite->r0 = (s8) (0x80 - D_800B5940);
    sprite->g0 = (s8) (0x80 - D_800B5940);
    sprite->b0 = (s8) (0x80 - D_800B5940);
    if (arg0 == 1) {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0x80, 0xA0);
    } else {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0x80, 0x58);
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x28 + D_800E8CA0, sprite);
    if (*mode_flags == 0) {
        goto_tpage(0x200, 0x100, 0xA, 1, 2);
    } else {
        goto_tpage(0x380, 0, 0xA, 1, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    SetShadeTex(sprite, 0);
    sprite->r0 = (u8) D_800B5940;
    sprite->g0 = (u8) D_800B5940;
    sprite->b0 = (u8) D_800B5940;
    if (arg0 == 1) {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0xC0, 0xA0);
    } else {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0xC0, 0x58);
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x30 + D_800E8CA0, sprite);
    if (*mode_flags != 0) {
        goto_tpage(0x140, 0x100, 0xC, 0, 2);
    } else {
        goto_tpage(0x2C0, 0, 0xC, 0, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    SetShadeTex(sprite, 0);
    sprite->r0 = (u8) D_800B5940;
    sprite->g0 = (u8) D_800B5940;
    sprite->b0 = (u8) D_800B5940;
    if (arg0 == 1) {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0x80, 0xA0);
    } else {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0x80, 0x58);
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x30 + D_800E8CA0, sprite);
    if (*mode_flags != 0) {
        goto_tpage(0x200, 0x100, 0xC, 0, 2);
    } else {
        goto_tpage(0x380, 0, 0xC, 0, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    SetShadeTex(sprite, 0);
    sprite->r0 = (u8) D_800B5940;
    sprite->g0 = (u8) D_800B5940;
    sprite->b0 = (u8) D_800B5940;
    if (arg0 == 1) {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0xC0, 0xA0);
    } else {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0xC0, 0x58);
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x34 + D_800E8CA0, sprite);
    if (*mode_flags == 0) {
        goto_tpage(0x140, 0x100, 0xD, 0, 2);
    } else {
        goto_tpage(0x2C0, 0, 0xD, 0, 2);
    }
    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    SetShadeTex(sprite, 0);
    sprite->r0 = (u8) D_800B5940;
    sprite->g0 = (u8) D_800B5940;
    sprite->b0 = (u8) D_800B5940;
    if (arg0 == 1) {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
        sprite->u0 = 0, sprite->v0 = 0;
        setWH(sprite, 0x80, 0xA0);
    } else {
        sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20), sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x30);
        sprite->u0 = 0, sprite->v0 = 0xA0;
        setWH(sprite, 0x80, 0x58);
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x34 + D_800E8CA0, sprite);
    if (*mode_flags == 0) {
        goto_tpage(0x200, 0x100, 0xD, 0, 2);
        return;
    }
    goto_tpage(0x380, 0, 0xD, 0, 2);
}
