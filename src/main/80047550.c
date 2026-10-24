#include "common.h"
#include "game.h"


void func_80047550(void) {
    D_801255DC = 4;
}

void func_80047560(void) {
    s32 sum;
    s32 i;
    s32 q;
    s16 off;

    if (D_801255DC != 0) {
        for (i = 0, sum = 0; i < D_801255DC; i++) {
            sum += i;
        }
        /* FAKE: T-25116, a u8 view of the u8 global (same value); with the plain read the conversion of
           the reloaded divisor takes the global's register */
        q = (s16)rsin(sum * 4) / 4 / (s32)(*(u8 *)&D_801255DC + 0x100);
        off = q;
        D_800E6280.screen_center_x[0] = (s16)q + 0xA0;
        /* FAKE: T-25116, dead store; redefining q stops uopt from forwarding off to its uses, so the
           narrowing is computed on the off line, before the unk_014[0] store (t0 and the original order) */
        q = 0;
        D_800E6280.screen_center_x[1] = off + 0xA0;
        D_800E6280.screen_center_y[0] = off + 0x168;
        D_800E6280.screen_center_y[1] = off + 0x78;
        D_801255E0 = off + 0xA0;
        D_801255E2 = off + 0x78;
        D_801255DC += 4;
        if (D_801255DC > 0x80) {
            D_801255DC = 0;
        }
    } else {
        D_801255E0 = 0xA0;
        D_801255E2 = 0x78;
        D_800E6280.screen_center_x[0] = 0xA0;
        D_800E6280.screen_center_x[1] = 0xA0;
        D_800E6280.screen_center_y[0] = 0x168;
        D_800E6280.screen_center_y[1] = 0x78;
    }
}

void func_800476C0(void) {
    GameState *gs;
    SpriteQuad spr;
    s32 step;
    s32 slot;
    s16 uv; /* T-25606: s16, the tpage UV byte is computed through a halfword */
    SpriteFrame *anim;
    SpritePart *part;
    s32 j;
    s16 *seq;
    SpritePart *parts;
    s32 tpx;
    s32 tpy;
    s32 cnt;
    s16 bx;
    s16 by;
    s16 ox;
    s16 oy;
    s32 i;
    u16 tpage;
    u8 col;

    gs = &D_800E6280;
    col = func_8004901C();
    search_tpage(0x7FFF, i, 0);
    for (i = 0; i <= *(s32 *)&gs->menu_hit[3]; i++) {
        {
            if ((((ActorFixed *)D_8011ECD0)[i].draw_flags & 0x80) &&
                (((((ActorFixed *)D_8011ECD0)[i].draw_flags & 0x40) != 0x40) || (D_800E6280.frame_count % (((ActorFixed *)D_8011ECD0)[i].unk_06 + 1) != 0)) &&
                !(((ActorFixed *)D_8011ECD0)[i].flags & 0x80)) {
                if (!(*(s32 *)((ActorFixed *)D_8011ECD0)[i].tpage_table & 0x80000000) || !(*(s32 *)((ActorFixed *)D_8011ECD0)[i].frame_table & 0x80000000)) {
                    printf("cls%x[m%xs%xs%xf%x] ", i, D_800E6280.mode, D_800E6280.stage,
                           D_800E6280.step, D_800E6280.substep);
                /* FAKE: T-25606, label for the never-taken goto at the end of the main path (see there) */
                print_done:
                    continue;
                } else {
                    if (((ActorFixed *)D_8011ECD0)[i].flags & 0x40) {
                        tpy = ((ActorFixed *)D_8011ECD0)[i].frame;
                        anim = ((SpriteFrame **)((ActorFixed *)D_8011ECD0)[i].frame_table)[tpy];
                    } else {
                        step = ((ActorFixed *)D_8011ECD0)[i].frame;
                        slot = ((ActorFixed *)D_8011ECD0)[i].sequence_slot;
                        /* FAKE: T-25317, anim reused for the s16 sequence row (the original holds it in v0,
                           the register anim gets next); 649 -> 595 rows, the shift stays folded */
                        anim = (SpriteFrame *)((s16 **)((ActorFixed *)D_8011ECD0)[i].sequence_table)[slot];
                        tpy = ((s16 *)anim)[step * 2 + 1];
                        anim = ((SpriteFrame **)((ActorFixed *)D_8011ECD0)[i].frame_table)[tpy];
                    }
                    ox = anim->origin_x;
                    oy = anim->origin_y;
                    bx = ((ActorFixed *)D_8011ECD0)[i].x.h[1];
                    by = ((ActorFixed *)D_8011ECD0)[i].y.h[1];
                    parts = anim->part;
                    cnt = anim->count;
                    if (cnt != 0) {
                        {
                            for (j = cnt - 1; j >= 0; j--) {
                                if ((((ActorFixed *)D_8011ECD0)[i].draw_flags & 8) && j >= 4) {
                                    j = 3;
                                }
                                /* T-25606: the part is indexed as seven halfwords (index j * 7, scaled by 2 in the
                                   array step); ugen then loads parts before j * 7, as the original does */
                                part = (SpritePart *)((s16 *)parts + j * 7);
                                spr.flags = ((ActorFixed *)D_8011ECD0)[i].quad_flags;
                                /* FAKE: T-25606, empty test of part in the join block; without a use there uopt
                                   moves the part sum into both mirror arms (j * 7 becomes a v0 CSE) */
                                if (part) {}
                                if (!(((ActorFixed *)D_8011ECD0)[i].draw_flags & 1)) {
                                    if (part->unk_08 & 2) {
                                        spr.x = bx + ox;
                                        spr.anchor_x = (part->unk_0A - ox) + part->unk_00;
                                        spr.unk_1C = -0x1000;
                                    } else {
                                        spr.x = bx + ox;
                                        spr.anchor_x = ox - part->unk_00;
                                        spr.unk_1C = 0x1000;
                                    }
                                    if (part->unk_08 & 1) {
                                        spr.y = by + oy;
                                        spr.anchor_y = (part->unk_0C - oy) + part->unk_02;
                                        spr.unk_1E = -0x1000;
                                    } else {
                                        spr.y = by + oy;
                                        spr.anchor_y = oy - part->unk_02;
                                        spr.unk_1E = 0x1000;
                                    }
                                } else {
                                    if (part->unk_08 & 2) {
                                        spr.x = bx + ox;
                                        spr.anchor_x = (part->unk_0A - ox) + part->unk_00;
                                        spr.unk_1C = 0x1000;
                                    } else {
                                        spr.x = bx + ox;
                                        spr.anchor_x = ox - part->unk_00;
                                        spr.unk_1C = -0x1000;
                                    }
                                    if (part->unk_08 & 1) {
                                        spr.y = by + oy;
                                        spr.anchor_y = (part->unk_0C - oy) + part->unk_02;
                                        spr.unk_1E = 0x1000;
                                    } else {
                                        spr.y = by + oy;
                                        spr.anchor_y = oy - part->unk_02;
                                        spr.unk_1E = 0x1000;
                                    }
                                }
                                if (((ActorFixed *)D_8011ECD0)[i].draw_flags & 4) {
                                    spr.x -= ox;
                                    spr.y -= oy;
                                }
                                if (((ActorFixed *)D_8011ECD0)[i].unk_1A != 0x1000 && ((ActorFixed *)D_8011ECD0)[i].unk_1C != 0x1000) {
                                    spr.w = part->unk_0A + 1;
                                    spr.h = part->unk_0C + 1;
                                } else {
                                    spr.w = part->unk_0A;
                                    spr.h = part->unk_0C;
                                }
                                if ((((ActorFixed *)D_8011ECD0)[i].unk_04 & 8) == 8) {
                                    switch (((ActorFixed *)D_8011ECD0)[i].quad_flags & 0x03000000) {
                                    case 0:
                                        tpage = search_tpage8(part->unk_04, i, 2);
                                        break;
                                    case 0x01000000:
                                        tpage = search_tpage8(part->unk_04, i, 1);
                                        break;
                                    }
                                    tpx = tpage & 0x1F;
                                    tpy = (tpage & 0x20) * 4;
                                    spr.unk_0C = tpx;
                                    uv = part->unk_04 & 0xFF;
                                    spr.unk_0E = (uv & 0xF) * 8;
                                    spr.unk_0F = ((uv >> 4) * 8) + tpy;
                                } else {
                                    switch (((ActorFixed *)D_8011ECD0)[i].quad_flags & 0x03000000) {
                                    case 0:
                                        tpage = search_tpage(part->unk_04, i, 2);
                                        break;
                                    case 0x01000000:
                                        tpage = search_tpage(part->unk_04, i, 1);
                                        break;
                                    }
                                    tpx = tpage & 0x1F;
                                    tpy = (tpage & 0x20) * 4;
                                    spr.unk_0C = tpx;
                                    uv = part->unk_04 & 0x3F;
                                    spr.unk_0E = (uv & 7) * 0x10;
                                    spr.unk_0F = ((uv >> 3) * 0x10) + tpy;
                                }
                                if (((ActorFixed *)D_8011ECD0)[i].unk_43 != 0) {
                                    spr.unk_10 = 0;
                                    switch (((ActorFixed *)D_8011ECD0)[i].unk_43 & 0x60) {
                                    case 0x20:
                                        spr.unk_10 = 0x100;
                                        break;
                                    case 0x40:
                                        spr.unk_10 = 0x120;
                                        break;
                                    default:
                                        if (((ActorFixed *)D_8011ECD0)[i].unk_05 & 0x80) {
                                            switch (((ActorFixed *)D_8011ECD0)[i].unk_05 & 0x7F) {
                                            case 1:
                                                spr.unk_10 = ((part->unk_06 & 0xF0) >> 4) * 0x10;
                                                break;
                                            case 2:
                                                spr.unk_10 = ((part->unk_06 & 0xE0) >> 4) * 0x10;
                                                break;
                                            case 4:
                                                spr.unk_10 = ((part->unk_06 & 0xC0) >> 4) * 0x10;
                                                break;
                                            case 8:
                                                spr.unk_10 = ((part->unk_06 & 0x80) >> 4) * 0x10;
                                                break;
                                            case 0:
                                            case 0x20:
                                            case 0x40:
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                    spr.unk_12 = (((ActorFixed *)D_8011ECD0)[i].unk_43 & 0x1F) + 0x1E0;
                                } else {
                                    switch (((ActorFixed *)D_8011ECD0)[i].unk_05 & 0x7F) {
                                    case 1:
                                        spr.unk_10 = ((part->unk_06 & 0xF0) >> 4) * 0x10;
                                        break;
                                    case 2:
                                        spr.unk_10 = ((part->unk_06 & 0xE0) >> 4) * 0x10;
                                        break;
                                    case 4:
                                        spr.unk_10 = ((part->unk_06 & 0xC0) >> 4) * 0x10;
                                        break;
                                    case 8:
                                        spr.unk_10 = ((part->unk_06 & 0x80) >> 4) * 0x10;
                                        break;
                                    case 0:
                                    case 0x20:
                                    case 0x40:
                                    default:
                                        spr.unk_10 = 0;
                                        break;
                                    }
                                    if (((ActorFixed *)D_8011ECD0)[i].unk_05 & 0x80) {
                                        spr.unk_12 = ((part->unk_06 & 0xF00) >> 8) + 0x1F0;
                                    } else {
                                        spr.unk_12 = ((part->unk_06 & 0xF00) >> 8) + 0x1E0;
                                    }
                                }
                                if (((ActorFixed *)D_8011ECD0)[i].draw_flags & 0x20) {
                                    spr.tint_r = col;
                                    spr.tint_g = col;
                                    spr.tint_b = col;
                                } else if (((ActorFixed *)D_8011ECD0)[i].unk_40 != 0 || ((ActorFixed *)D_8011ECD0)[i].unk_41 != 0 || ((ActorFixed *)D_8011ECD0)[i].unk_42 != 0) {
                                    spr.tint_r = ((ActorFixed *)D_8011ECD0)[i].unk_40;
                                    spr.tint_g = ((ActorFixed *)D_8011ECD0)[i].unk_41;
                                    spr.tint_b = ((ActorFixed *)D_8011ECD0)[i].unk_42;
                                } else {
                                    spr.tint_r = ((ActorFixed *)D_8011ECD0)[i].unk_07;
                                    spr.tint_g = ((ActorFixed *)D_8011ECD0)[i].unk_07;
                                    spr.tint_b = ((ActorFixed *)D_8011ECD0)[i].unk_07;
                                }
                                spr.unk_20 = ((ActorFixed *)D_8011ECD0)[i].unk_20;
                                if (((ActorFixed *)D_8011ECD0)[i].draw_flags & 8) {
                                    /* T-25606: the rotate is tested in spr (a stack reload that as1 forwards) */
                                    if (spr.unk_20 != 0 || spr.unk_1C != 0x1000 || spr.unk_1E != 0x1000) {
                                        spr.w = (u16)spr.w - 1;
                                        spr.h = spr.h - 1;
                                    }
                                }
                                if (((ActorFixed *)D_8011ECD0)[i].unk_1A != 0x1000 || ((ActorFixed *)D_8011ECD0)[i].unk_1C != 0x1000) {
                                    spr.unk_1C = ((ActorFixed *)D_8011ECD0)[i].unk_1A * (spr.unk_1C / 0x1000);
                                    spr.unk_1E = ((ActorFixed *)D_8011ECD0)[i].unk_1C * (spr.unk_1E / 0x1000);
                                }
                                /* FAKE: T-25317, a second scale test around a dead store. uopt counts its two 0x1000
                                   compares (8 use blocks: the constant outranks ox and takes s4), then drops the empty
                                   branch after colouring, so no instruction is added */
                                if (((ActorFixed *)D_8011ECD0)[i].unk_1A != 0x1000 || ((ActorFixed *)D_8011ECD0)[i].unk_1C != 0x1000) {
                                    tpx = 0;
                                }
                                if (((ActorFixed *)D_8011ECD0)[i].unk_01 == 0) {
                                    func_80098680(&spr, &((u8 *)&D_800E8C70)[D_8011ECA0 * 0x14], 0);
                                } else {
                                    func_80098680(&spr, &((u8 *)&D_800E8C70)[D_8011ECA0 * 0x14],
                                                  (((ActorFixed *)D_8011ECD0)[i].unk_01 - 1) & 0xFFFF);
                                }
                            }
                        }
                    }
                    if (!(((ActorFixed *)D_8011ECD0)[i].flags & 0x40) && ((((ActorFixed *)D_8011ECD0)[i].flags & 1) == 1)) {
                        if ((((ActorFixed *)D_8011ECD0)[i].unk_06 & 0x80) && (((ActorFixed *)D_8011ECD0)[i].unk_06 & 0x7F) != 0) {
                            ((ActorFixed *)D_8011ECD0)[i].unk_08 += ((ActorFixed *)D_8011ECD0)[i].unk_06 & 0x7F;
                        } else if (((ActorFixed *)D_8011ECD0)[i].unk_06 != 0) {
                            if (D_800E6280.frame_count % ((ActorFixed *)D_8011ECD0)[i].unk_06 == 0) {
                                ((ActorFixed *)D_8011ECD0)[i].unk_08++;
                            }
                        } else {
                            /* FAKE: T-25116, empty arm; it keeps a block on the unk_06 == 0 path, where uopt's PRE
                               places the seq and unk_18 loads of the join (the original computes them there) */
                        }
                        seq = ((s16 **)((ActorFixed *)D_8011ECD0)[i].sequence_table)[slot];
                        if (((ActorFixed *)D_8011ECD0)[i].unk_08 >= seq[step * 2 + 2]) {
                            if ((((ActorFixed *)D_8011ECD0)[i].flags & 0x10) == 0x10) {
                                ((ActorFixed *)D_8011ECD0)[i].frame++;
                                ((ActorFixed *)D_8011ECD0)[i].unk_08 = 0;
                                ((ActorFixed *)D_8011ECD0)[i].flags &= ~0x20;
                            } else if ((((ActorFixed *)D_8011ECD0)[i].flags & 0x20) == 0x20) {
                                ((ActorFixed *)D_8011ECD0)[i].frame--;
                                ((ActorFixed *)D_8011ECD0)[i].unk_08 = 0;
                                if (((ActorFixed *)D_8011ECD0)[i].frame <= 0) {
                                    ((ActorFixed *)D_8011ECD0)[i].flags &= 0xFFDE;
                                }
                            } else {
                                ((ActorFixed *)D_8011ECD0)[i].frame++;
                                ((ActorFixed *)D_8011ECD0)[i].unk_08 = 0;
                            }
                        }
                        if (((ActorFixed *)D_8011ECD0)[i].frame >= seq[0]) {
                            if ((((ActorFixed *)D_8011ECD0)[i].flags & 2) == 2) {
                                ((ActorFixed *)D_8011ECD0)[i].frame = 0;
                                ((ActorFixed *)D_8011ECD0)[i].flags &= ~3;
                            } else if ((((ActorFixed *)D_8011ECD0)[i].flags & 4) == 4) {
                                ((ActorFixed *)D_8011ECD0)[i].flags &= 0xFFFA;
                                ((ActorFixed *)D_8011ECD0)[i].frame--;
                            } else if ((((ActorFixed *)D_8011ECD0)[i].flags & 8) == 8) {
                                ((ActorFixed *)D_8011ECD0)[i].frame = 0;
                                ((ActorFixed *)D_8011ECD0)[i].flags &= 0xFFFC;
                                ((ActorFixed *)D_8011ECD0)[i].draw_flags &= ~0x80;
                            } else if ((((ActorFixed *)D_8011ECD0)[i].flags & 0x10) == 0x10) {
                                ((ActorFixed *)D_8011ECD0)[i].flags &= ~0x10;
                                ((ActorFixed *)D_8011ECD0)[i].flags |= 0x20;
                                if (((ActorFixed *)D_8011ECD0)[i].draw_flags & 8) {
                                    if (seq[0] >= 3) {
                                        ((ActorFixed *)D_8011ECD0)[i].frame -= 3;
                                    } else {
                                        ((ActorFixed *)D_8011ECD0)[i].frame = 0;
                                    }
                                } else if (seq[0] >= 2) {
                                    ((ActorFixed *)D_8011ECD0)[i].frame -= 1;
                                } else {
                                    ((ActorFixed *)D_8011ECD0)[i].frame = 0;
                                }
                            } else {
                                ((ActorFixed *)D_8011ECD0)[i].frame = 0;
                            }
                        }
                    }
                    if (((ActorFixed *)D_8011ECD0)[i].draw_flags & 0x10) {
                        if (((ActorFixed *)D_8011ECD0)[i].dx != 0) {
                            ((ActorFixed *)D_8011ECD0)[i].x.w += ((ActorFixed *)D_8011ECD0)[i].dx;
                        }
                        if (((ActorFixed *)D_8011ECD0)[i].dy != 0) {
                            ((ActorFixed *)D_8011ECD0)[i].y.w += ((ActorFixed *)D_8011ECD0)[i].dy;
                        }
                    }
                    /* FAKE: T-25606, never taken ((u32)i < 0 is false; uopt keeps the edge, ugen folds it away).
                       The extra edge into the block after the printf makes gs's loop piece reload there, so the
                       piece has no profit and gs stays in memory, while the PRE copy of gs->count stays in the
                       block after the call and gets v1 as in the original */
                    if ((u32)i < 0) goto print_done;
                }
            }
        }
    }
}

void tpage_buf_clear_all(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        if ((i & 0xF) < 5) {
            D_800E6280.tpage_buf[i] = 0;
        } else {
            D_800E6280.tpage_buf[i] = -1;
        }
    }
}

void tpage_buf_clear(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        if ((i & 0xF) < 5) {
            D_800E6280.tpage_buf[i] = 0;
        } else {
            D_800E6280.tpage_buf[i] = -1;
        }
    }
    _sys_default_tpage_set();
}

void _sys_default_tpage_set(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        D_800E6280.tpage_buf[i + 11] = 0x77;
        D_800E6280.tpage_buf[i + 43] = 0x77;
        D_800E6280.tpage_buf[i + 21] = 0x77;
        D_800E6280.tpage_buf[i + 53] = 0x77;
    }
    D_800E6280.tpage_buf[60] = *(s32 *)D_800C9730;
    D_800E6280.tpage_buf[61] = D_800C9734;
    D_800E6280.tpage_buf[62] = D_800C9738;
    D_800E6280.tpage_buf[63] = D_800C973C;
    D_800E6280.tpage_buf[31] = D_800C974C;
    D_800E6280.tpage_buf[30] = D_800C9750;
    D_800E6280.tpage_buf[29] = D_800C9754;
    D_800E6280.tpage_buf[28] = D_800C9758;
    D_800E6280.tpage_buf[27] = D_800C5FCC;
    D_800E6280.tpage_buf[59] = D_800C5FD0;
}

void load_tpage_buf_lock(arg0, arg1, arg2)
s32 arg0;
s32 arg1;
u8 arg2;
{
    /* FAKE: T-23006, the store through an integer address (index first) is `D_800E6280.tpage_buf[arg0] = arg1`; uopt then treats it as a possible alias and reloads arg0 and arg1, as the original does. */
    *(s32 *)(arg0 * 4 + (s32)D_800E6280.tpage_buf) = arg1;
    if (arg0 < 0x20) {
        load_csr_tp(arg1, (u8)(arg0 & 0x1F), (s32)arg2, 0, 0, 0x80, 0x80);
    } else {
        load_csr_tp(arg1, (u8)(arg0 & 0x1F), (s32)arg2, 0, 0x80, 0x80, 0x80);
    }
}

s32 search_load_tpage_buf_lock(s32 arg0, u8 arg1) {
    GameState *var_v0;
    s32 var_v1;

    /* FAKE: shifted cursor keeps the target's base + 0x1228 addressing, T-9580. */
    var_v0 = (GameState *)((u8 *)&D_800E6280 + 0x14);
    for (var_v1 = 5; var_v1 < 0x40; var_v1++) {
        if (var_v0->tpage_buf[0] == -1) {
            var_v0->tpage_buf[0] = arg0;
            if (var_v1 < 0x20) {
                load_csr_tp(arg0, (u8)var_v1 & 0x1F, (s32)arg1, 0, 0, 0x80, 0x80);
                return var_v1;
            }
            load_csr_tp(arg0, (u8)var_v1 & 0x1F, (s32)arg1, 0, 0x80, 0x80, 0x80);
            return var_v1;
        }
        var_v0 = (GameState *)((u8 *)var_v0 + 4);
    }
    return -1;
}

s32 search_tpage_multi(s32 arg0, u8 arg1) {
    s32 j;
    s32 i;

    for (i = 0xF; i >= 5; i--) {
        for (j = 3; j >= 0; j--) {
            if (D_800E6280.tpage_req[i + j * 16] == 0x7FFE && D_800E6280.tpage_buf[i + j * 16] == arg0) {
                return (j * 16 + i) & 0xFFFF;
            }
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (arg0 == D_800E6280.tpage_req[i]) {
            return i & 0xFFFF;
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.tpage_req[i] == 0x7FFF) {
            D_800E6280.tpage_req[i] = arg0;
            if (i < 0x20) {
                load_csr_tp(arg0, i & 0xFF, arg1, 0, 0, 0x80, 0x80);
            } else {
                load_csr_tp(arg0, i & 0xFF, arg1, 0, 0x80, 0x80, 0x80);
            }
            return i & 0xFFFF;
        }
        if (i == 0x3F) {
            return 0x3F;
        }
    }
}

s32 search_tpage(s16 arg0, s32 arg1, u8 arg2) {
    s32 j;
    s32 i;
    Actor *record;
    s16 index;

    if (arg0 == 0x7FFF) {
        for (i = 0; i < 64; i++) {
            if (D_800E6280.tpage_buf[i] == -1) {
                D_800E6280.tpage_req[i] = 0x7FFF;
            } else {
                D_800E6280.tpage_req[i] = 0x7FFE;
            }
        }
        return 0;
    }
    record = &((Actor *)D_8011ECD0)[arg1];
    index = (s16)(arg0 >> 6);
    for (i = 0xF; i >= 5; i--) {
        for (j = 3; j >= 0; j--) {
            if (D_800E6280.tpage_req[i + j * 16] == 0x7FFE &&
                D_800E6280.tpage_buf[i + j * 16] == ((s32 *)record->tpage_table)[index]) {
                return (j * 16 + i) & 0xFFFF;
            }
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.tpage_req[i] == ((s32 *)record->tpage_table)[index]) {
            return i & 0xFFFF;
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.tpage_req[i] == 0x7FFF) {
            D_800E6280.tpage_req[i] = ((s32 *)record->tpage_table)[index];
            if (i < 0x20) {
                load_csr_tp(((s32 *)record->tpage_table)[index], i & 0xFF, arg2, 0, 0, 0x80, 0x80);
            } else {
                load_csr_tp(((s32 *)record->tpage_table)[index], i & 0xFF, arg2, 0, 0x80, 0x80, 0x80);
            }
            return i & 0xFFFF;
        }
        if (i == 0x3F) {
            return 0x3F;
        }
    }
}

s32 search_tpage8(s16 arg0, s32 arg1, u8 arg2) {
    s32 j;
    s32 i;
    Actor *record;
    s16 index;

    if (arg0 == 0x7FFF) {
        for (i = 0; i < 64; i++) {
            if (D_800E6280.tpage_buf[i] == -1) {
                D_800E6280.tpage_req[i] = 0x7FFF;
            } else {
                D_800E6280.tpage_req[i] = 0x7FFE;
            }
        }
        return 0;
    }
    record = &((Actor *)D_8011ECD0)[arg1];
    index = (s16)(arg0 >> 8);
    for (i = 0xF; i >= 5; i--) {
        for (j = 3; j >= 0; j--) {
            if (D_800E6280.tpage_req[i + j * 16] == 0x7FFE &&
                D_800E6280.tpage_buf[i + j * 16] == ((s32 *)record->tpage_table)[index]) {
                return (j * 16 + i) & 0xFFFF;
            }
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.tpage_req[i] == ((s32 *)record->tpage_table)[index]) {
            return i & 0xFFFF;
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.tpage_req[i] == 0x7FFF) {
            D_800E6280.tpage_req[i] = ((s32 *)record->tpage_table)[index];
            if (i < 0x20) {
                load_csr_tp(((s32 *)record->tpage_table)[index], i & 0xFF, arg2, 0, 0, 0x80, 0x80);
            } else {
                load_csr_tp(((s32 *)record->tpage_table)[index], i & 0xFF, arg2, 0, 0x80, 0x80, 0x80);
            }
            return i & 0xFFFF;
        }
        if (i == 0x3F) {
            return 0x3F;
        }
    }
}

void func_80048CF8(void) {
    s32 i;

    if (D_800E6280.pad_held & PAD_R2) {
        printf("PRINT_TPAGE_BUF\n");
        for (i = 0; i < 0x40; i++) {
            printf("[%d]%08x", i, D_800E6280.tpage_buf[i]);
            if ((i & 3) == 3) {
                printf("\r");
            }
        }
    }
}

void func_80048DAC(s32 arg0) {
    if (arg0 != 0) {
        D_800E6280.actors_draw_enabled = 1;
    } else {
        D_800E6280.actors_draw_enabled = 0;
    }
}

void func_80048DD0(s32 count) {
    s32 i;

    printf("%d CLASS_CLEAR \n", count);
    if (count != 0) {
        for (i = 0; i < count && i < 160; i++) {
            ((Actor *)D_8011ECD0)[i].draw_flags = 0;
        }
    } else {
        for (i = 0; i < 160; i++) {
            ((Actor *)D_8011ECD0)[i].draw_flags = 0;
        }
    }
}

void func_80048E78(void) {
    s32 i;

    for (i = 0x60; i < 0xA0; i++) {
        func_80048F64(i);
    }
}

void func_80048EB8(s32 arg0) {
    s32 n;
    s32 i;
    s32 a;
    s32 b;
    s32 c;

    a = D_8011ECF6;
    b = D_8011ECFA;
    c = D_8011ECE8;
    if (arg0 == 0) {
        n = 0xA0;
    } else {
        n = (arg0 >= 0xA1) ? 0xA0 : arg0;
    }
    for (i = 1; i < n; i++) {
        func_80048F64(i);
    }
    D_8011ECF6 = a;
    D_8011ECFA = b;
    D_8011ECE8 = c;
}

void func_80048F64(s32 arg0) {
    u8 *p;

    if (arg0 < 0xA0) {
        p = D_8011ECD0 + arg0 * 0x44;
        p[0] = 0;
        *(s32 *)(p + 0x38) = 0;
        p[1] = 0;
        p[4] = 0;
        p[5] = 0;
        p[2] = 0;
        p[3] = 0;
        p[6] = 1;
        *(s16 *)(p + 8) = 0;
        *(s32 *)(p + 0xC) = (s32)sprite_frame_table;
        *(s32 *)(p + 0x10) = (s32)D_800C9A14;
        *(s32 *)(p + 0x34) = (s32)D_800C9730;
        *(s16 *)(p + 0x14) = 0;
        *(s16 *)(p + 0x16) = 0;
        *(s16 *)(p + 0x18) = 0;
        *(s16 *)(p + 0x1A) = 0x1000;
        *(s16 *)(p + 0x1C) = 0x1000;
        p[0x43] = 0;
        p[7] = 0x80;
        *(s32 *)(p + 0x20) = 0;
        *(s32 *)(p + 0x24) = 0;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0;
        *(s32 *)(p + 0x30) = 0;
        p[0x40] = 0;
        p[0x41] = 0;
        p[0x42] = 0;
    }
}

u8 func_8004901C(void) {
    return D_800E6280.screen_brightness;
}

void func_8004902C(void) {
    D_800E6280.screen_brightness = 0x80;
    D_800E6280.brightness_step = 0;
}

void func_80049044(void) {
    u8 *p;
    s32 v;

    p = &D_800E6280.screen_brightness;
    if ((s8)D_800E6280.brightness_step != 0) {
        if ((s8)D_800E6280.brightness_step > 0) {
            v = *p;
            v += (s8)D_800E6280.brightness_step;
            if (v >= 0x100) {
                v = 0xFF;
            }
            *p = v;
        } else if ((s8)D_800E6280.brightness_step < 0) {
            v = *p;
            v += (s8)D_800E6280.brightness_step;
            if (v < 0) {
                v = 0;
            }
            *p = v;
        }
    }
}

void func_800490B4(u8 arg0) {
    D_800E6280.brightness_step = arg0;
}
