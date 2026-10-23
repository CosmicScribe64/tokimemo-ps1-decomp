void func_800476C0(void) {
    GameState *gs;
    Rec24 spr;
    s32 step;
    s32 slot;
    s32 uv;
    DisplayImage78 *anim;
    DisplayImagePart0E *part;
    s32 j;
    s16 *seq;
    DisplayImagePart0E *parts;
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
    for (i = 0; i <= *(s32 *)&gs->unk_1095[3]; i++) {
        {
            if ((((DisplayFixActor44 *)D_8011ECD0)[i].unk_03 & 0x80) &&
                (((((DisplayFixActor44 *)D_8011ECD0)[i].unk_03 & 0x40) != 0x40) || (D_800E6280.unk_10F8 % (((DisplayFixActor44 *)D_8011ECD0)[i].unk_06 + 1) != 0)) &&
                !(((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 & 0x80)) {
                if (!(*(s32 *)((DisplayFixActor44 *)D_8011ECD0)[i].unk_34 & 0x80000000) || !(*(s32 *)((DisplayFixActor44 *)D_8011ECD0)[i].unk_0C & 0x80000000)) {
                    printf("cls%x[m%xs%xs%xf%x] ", i, D_800E6280.unk_1108, D_800E6280.unk_1109,
                           D_800E6280.unk_110A, D_800E6280.unk_110D);
                    continue;
                } else {
                    if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 & 0x40) {
                        tpy = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_18;
                        anim = ((DisplayImage78 **)((DisplayFixActor44 *)D_8011ECD0)[i].unk_0C)[tpy];
                    } else {
                        step = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_18;
                        slot = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_16;
                        tpy = ((s16 **)((DisplayFixActor44 *)D_8011ECD0)[i].unk_10)[slot][step * 2 + 1];
                        anim = ((DisplayImage78 **)((DisplayFixActor44 *)D_8011ECD0)[i].unk_0C)[tpy];
                    }
                    ox = anim->origin_x;
                    oy = anim->origin_y;
                    bx = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_24.h[1];
                    by = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_28.h[1];
                    parts = anim->part;
                    cnt = anim->count;
                    if (cnt != 0) {
                        {
                            for (j = cnt - 1; j >= 0; j--) {
                                if ((((DisplayFixActor44 *)D_8011ECD0)[i].unk_03 & 8) && j >= 4) {
                                    j = 3;
                                }
                                part = &parts[j];
                                spr.unk_00 = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_38;
                                if (!(((DisplayFixActor44 *)D_8011ECD0)[i].unk_03 & 1)) {
                                    if (part->unk_08 & 2) {
                                        spr.unk_04 = bx + ox;
                                        spr.unk_18 = (part->unk_0A - ox) + part->unk_00;
                                        spr.unk_1C = -0x1000;
                                    } else {
                                        spr.unk_04 = bx + ox;
                                        spr.unk_18 = ox - part->unk_00;
                                        spr.unk_1C = 0x1000;
                                    }
                                    if (part->unk_08 & 1) {
                                        spr.unk_06 = by + oy;
                                        spr.unk_1A = (part->unk_0C - oy) + part->unk_02;
                                        spr.unk_1E = -0x1000;
                                    } else {
                                        spr.unk_06 = by + oy;
                                        spr.unk_1A = oy - part->unk_02;
                                        spr.unk_1E = 0x1000;
                                    }
                                } else {
                                    if (part->unk_08 & 2) {
                                        spr.unk_04 = bx + ox;
                                        spr.unk_18 = (part->unk_0A - ox) + part->unk_00;
                                        spr.unk_1C = 0x1000;
                                    } else {
                                        spr.unk_04 = bx + ox;
                                        spr.unk_18 = ox - part->unk_00;
                                        spr.unk_1C = -0x1000;
                                    }
                                    if (part->unk_08 & 1) {
                                        spr.unk_06 = by + oy;
                                        spr.unk_1A = (part->unk_0C - oy) + part->unk_02;
                                        spr.unk_1E = 0x1000;
                                    } else {
                                        spr.unk_06 = by + oy;
                                        spr.unk_1A = oy - part->unk_02;
                                        spr.unk_1E = 0x1000;
                                    }
                                }
                                if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_03 & 4) {
                                    spr.unk_04 -= ox;
                                    spr.unk_06 -= oy;
                                }
                                if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_1A != 0x1000 && ((DisplayFixActor44 *)D_8011ECD0)[i].unk_1C != 0x1000) {
                                    spr.unk_08 = part->unk_0A + 1;
                                    spr.unk_0A = part->unk_0C + 1;
                                } else {
                                    spr.unk_08 = part->unk_0A;
                                    spr.unk_0A = part->unk_0C;
                                }
                                if ((((DisplayFixActor44 *)D_8011ECD0)[i].unk_04 & 8) == 8) {
                                    switch (((DisplayFixActor44 *)D_8011ECD0)[i].unk_38 & 0x03000000) {
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
                                    switch (((DisplayFixActor44 *)D_8011ECD0)[i].unk_38 & 0x03000000) {
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
                                if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_43 != 0) {
                                    spr.unk_10 = 0;
                                    switch (((DisplayFixActor44 *)D_8011ECD0)[i].unk_43 & 0x60) {
                                    case 0x20:
                                        spr.unk_10 = 0x100;
                                        break;
                                    case 0x40:
                                        spr.unk_10 = 0x120;
                                        break;
                                    default:
                                        if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_05 & 0x80) {
                                            switch (((DisplayFixActor44 *)D_8011ECD0)[i].unk_05 & 0x7F) {
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
                                    spr.unk_12 = (((DisplayFixActor44 *)D_8011ECD0)[i].unk_43 & 0x1F) + 0x1E0;
                                } else {
                                    switch (((DisplayFixActor44 *)D_8011ECD0)[i].unk_05 & 0x7F) {
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
                                    if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_05 & 0x80) {
                                        spr.unk_12 = ((part->unk_06 & 0xF00) >> 8) + 0x1F0;
                                    } else {
                                        spr.unk_12 = ((part->unk_06 & 0xF00) >> 8) + 0x1E0;
                                    }
                                }
                                if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_03 & 0x20) {
                                    spr.unk_14 = col;
                                    spr.unk_15 = col;
                                    spr.unk_16 = col;
                                } else if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_40 != 0 || ((DisplayFixActor44 *)D_8011ECD0)[i].unk_41 != 0 || ((DisplayFixActor44 *)D_8011ECD0)[i].unk_42 != 0) {
                                    spr.unk_14 = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_40;
                                    spr.unk_15 = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_41;
                                    spr.unk_16 = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_42;
                                } else {
                                    spr.unk_14 = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_07;
                                    spr.unk_15 = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_07;
                                    spr.unk_16 = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_07;
                                }
                                spr.unk_20 = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_20;
                                if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_03 & 8) {
                                    if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_20 != 0 || spr.unk_1C != 0x1000 || spr.unk_1E != 0x1000) {
                                        spr.unk_08 = (u16)spr.unk_08 - 1;
                                        spr.unk_0A = spr.unk_0A - 1;
                                    }
                                }
                                if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_1A != 0x1000 || ((DisplayFixActor44 *)D_8011ECD0)[i].unk_1C != 0x1000) {
                                    spr.unk_1C = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_1A * (spr.unk_1C / 0x1000);
                                    spr.unk_1E = ((DisplayFixActor44 *)D_8011ECD0)[i].unk_1C * (spr.unk_1E / 0x1000);
                                }
                                if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_01 == 0) {
                                    func_80098680(&spr, &((u8 *)&D_800E8C70)[D_8011ECA0 * 0x14], 0);
                                } else {
                                    func_80098680(&spr, &((u8 *)&D_800E8C70)[D_8011ECA0 * 0x14],
                                                  (((DisplayFixActor44 *)D_8011ECD0)[i].unk_01 - 1) & 0xFFFF);
                                }
                            }
                        }
                    }
                    if (!(((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 & 0x40) && ((((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 & 1) == 1)) {
                        if ((((DisplayFixActor44 *)D_8011ECD0)[i].unk_06 & 0x80) && (((DisplayFixActor44 *)D_8011ECD0)[i].unk_06 & 0x7F) != 0) {
                            ((DisplayFixActor44 *)D_8011ECD0)[i].unk_08 += ((DisplayFixActor44 *)D_8011ECD0)[i].unk_06 & 0x7F;
                        } else if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_06 != 0) {
                            if (D_800E6280.unk_10F8 % ((DisplayFixActor44 *)D_8011ECD0)[i].unk_06 == 0) {
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_08++;
                            }
                        } else {
                            /* FAKE: T-25116, empty arm; it keeps a block on the unk_06 == 0 path, where uopt's PRE
                               places the seq and unk_18 loads of the join (the original computes them there) */
                        }
                        seq = ((s16 **)((DisplayFixActor44 *)D_8011ECD0)[i].unk_10)[slot];
                        if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_08 >= seq[step * 2 + 2]) {
                            if ((((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 & 0x10) == 0x10) {
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_18++;
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_08 = 0;
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 &= ~0x20;
                            } else if ((((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 & 0x20) == 0x20) {
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_18--;
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_08 = 0;
                                if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_18 <= 0) {
                                    ((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 &= 0xFFDE;
                                }
                            } else {
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_18++;
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_08 = 0;
                            }
                        }
                        if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_18 >= seq[0]) {
                            if ((((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 & 2) == 2) {
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_18 = 0;
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 &= ~3;
                            } else if ((((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 & 4) == 4) {
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 &= 0xFFFA;
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_18--;
                            } else if ((((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 & 8) == 8) {
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_18 = 0;
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 &= 0xFFFC;
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_03 &= ~0x80;
                            } else if ((((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 & 0x10) == 0x10) {
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 &= ~0x10;
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_02 |= 0x20;
                                if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_03 & 8) {
                                    if (seq[0] >= 3) {
                                        ((DisplayFixActor44 *)D_8011ECD0)[i].unk_18 -= 3;
                                    } else {
                                        ((DisplayFixActor44 *)D_8011ECD0)[i].unk_18 = 0;
                                    }
                                } else if (seq[0] >= 2) {
                                    ((DisplayFixActor44 *)D_8011ECD0)[i].unk_18 -= 1;
                                } else {
                                    ((DisplayFixActor44 *)D_8011ECD0)[i].unk_18 = 0;
                                }
                            } else {
                                ((DisplayFixActor44 *)D_8011ECD0)[i].unk_18 = 0;
                            }
                        }
                    }
                    if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_03 & 0x10) {
                        if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_2C != 0) {
                            ((DisplayFixActor44 *)D_8011ECD0)[i].unk_24.w += ((DisplayFixActor44 *)D_8011ECD0)[i].unk_2C;
                        }
                        if (((DisplayFixActor44 *)D_8011ECD0)[i].unk_30 != 0) {
                            ((DisplayFixActor44 *)D_8011ECD0)[i].unk_28.w += ((DisplayFixActor44 *)D_8011ECD0)[i].unk_30;
                        }
                    }
                }
            }
        }
    }
}
