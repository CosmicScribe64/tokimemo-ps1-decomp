void func_800476C0(void) {
    GameState *gs;
    Rec24 spr;
    s32 step;
    s32 slot;
    DisplayFixActor44 *rec;
    DisplayImage78 *anim;
    DisplayImagePart0E *part;
    s32 j;
    s32 attr;
    s32 rot;
    s32 tpx;
    s32 tpy;
    s32 bx;
    s32 by;
    s16 ox;
    s16 oy;
    s32 i;
    u16 tpage;
    u8 col;
    s32 pad[6];

    gs = &D_800E6280;
    col = func_8004901C();
    search_tpage(0x7FFF, i, 0);
    i = 0;
    if (*(s32 *)&D_800E6280.unk_1095[3] >= 0) {
        rec = (DisplayFixActor44 *)D_8011ECD0;
        do {
            if ((rec->unk_03 & 0x80) &&
                (((rec->unk_03 & 0x40) != 0x40) || (D_800E6280.unk_10F8 % (rec->unk_06 + 1) != 0)) &&
                !(rec->unk_02 & 0x80)) {
                if (!(*(s32 *)rec->unk_34 & 0x80000000) || !(*(s32 *)rec->unk_0C & 0x80000000)) {
                    printf("cls%x[m%xs%xs%xf%x] ", i, D_800E6280.unk_1108, D_800E6280.unk_1109,
                           D_800E6280.unk_110A, D_800E6280.unk_110D);
                } else {
                    if (rec->unk_02 & 0x40) {
                        anim = ((DisplayImage78 **)rec->unk_0C)[rec->unk_18];
                    } else {
                        slot = rec->unk_16;
                        step = rec->unk_18;
                        anim = ((DisplayImage78 **)rec->unk_0C)[((s16 **)rec->unk_10)[slot][step * 2 + 1]];
                    }
                    ox = anim->origin_x;
                    oy = anim->origin_y;
                    if (anim->count != 0) {
                        j = anim->count - 1;
                        if (j >= 0) {
                            bx = rec->unk_24.h[1] + ox;
                            by = rec->unk_28.h[1] + oy;
                            do {
                                if ((rec->unk_03 & 8) && j >= 4) {
                                    j = 3;
                                }
                                part = &anim->part[j];
                                attr = rec->unk_38;
                                ((Rec24 *)&spr)->unk_00 = attr;
                                if (!(rec->unk_03 & 1)) {
                                    if (part->unk_08 & 2) {
                                        ((Rec24 *)&spr)->unk_04 = bx;
                                        ((Rec24 *)&spr)->unk_18 = (part->unk_0A - ox) + part->unk_00;
                                        ((Rec24 *)&spr)->unk_1C = -0x1000;
                                    } else {
                                        ((Rec24 *)&spr)->unk_04 = bx;
                                        ((Rec24 *)&spr)->unk_1C = 0x1000;
                                        ((Rec24 *)&spr)->unk_18 = ox - part->unk_00;
                                    }
                                    if (part->unk_08 & 1) {
                                        ((Rec24 *)&spr)->unk_06 = by;
                                        ((Rec24 *)&spr)->unk_1A = (part->unk_0C - oy) + part->unk_02;
                                        ((Rec24 *)&spr)->unk_1E = -0x1000;
                                    } else {
                                        ((Rec24 *)&spr)->unk_06 = by;
                                        ((Rec24 *)&spr)->unk_1E = 0x1000;
                                        ((Rec24 *)&spr)->unk_1A = oy - part->unk_02;
                                    }
                                } else {
                                    if (part->unk_08 & 2) {
                                        ((Rec24 *)&spr)->unk_04 = bx;
                                        ((Rec24 *)&spr)->unk_18 = (part->unk_0A - ox) + part->unk_00;
                                        ((Rec24 *)&spr)->unk_1C = 0x1000;
                                    } else {
                                        ((Rec24 *)&spr)->unk_04 = bx;
                                        ((Rec24 *)&spr)->unk_1C = -0x1000;
                                        ((Rec24 *)&spr)->unk_18 = ox - part->unk_00;
                                    }
                                    if (part->unk_08 & 1) {
                                        ((Rec24 *)&spr)->unk_06 = by;
                                        ((Rec24 *)&spr)->unk_1A = (part->unk_0C - oy) + part->unk_02;
                                        ((Rec24 *)&spr)->unk_1E = 0x1000;
                                    } else {
                                        ((Rec24 *)&spr)->unk_06 = by;
                                        ((Rec24 *)&spr)->unk_1E = 0x1000;
                                        ((Rec24 *)&spr)->unk_1A = oy - part->unk_02;
                                    }
                                }
                                if (rec->unk_03 & 4) {
                                    ((Rec24 *)&spr)->unk_04 -= ox;
                                    ((Rec24 *)&spr)->unk_06 -= oy;
                                }
                                if (rec->unk_1A != 0x1000 && rec->unk_1C != 0x1000) {
                                    ((Rec24 *)&spr)->unk_08 = part->unk_0A + 1;
                                    ((Rec24 *)&spr)->unk_0A = part->unk_0C + 1;
                                } else {
                                    ((Rec24 *)&spr)->unk_08 = part->unk_0A;
                                    ((Rec24 *)&spr)->unk_0A = part->unk_0C;
                                }
                                if ((rec->unk_04 & 8) == 8) {
                                    switch (attr & 0x03000000) {
                                    case 0:
                                        tpage = search_tpage8(part->unk_04, i, 2);
                                        tpx = tpage & 0x1F;
                                        tpy = (tpage & 0x20) * 4;
                                        break;
                                    case 0x01000000:
                                        tpage = search_tpage8(part->unk_04, i, 1);
                                        tpx = tpage & 0x1F;
                                        tpy = (tpage & 0x20) * 4;
                                        break;
                                    default:
                                        tpx = tpage & 0x1F;
                                        tpy = (tpage & 0x20) * 4;
                                        break;
                                    }
                                    ((Rec24 *)&spr)->unk_0C = tpx;
                                    ((Rec24 *)&spr)->unk_0E = ((part->unk_04 & 0xFF) & 0xF) * 8;
                                    ((Rec24 *)&spr)->unk_0F = (((part->unk_04 & 0xFF) >> 4) * 8) + tpy;
                                } else {
                                    switch (attr & 0x03000000) {
                                    case 0:
                                        tpage = search_tpage(part->unk_04, i, 2);
                                        tpx = tpage & 0x1F;
                                        tpy = (tpage & 0x20) * 4;
                                        break;
                                    case 0x01000000:
                                        tpage = search_tpage(part->unk_04, i, 1);
                                        tpx = tpage & 0x1F;
                                        tpy = (tpage & 0x20) * 4;
                                        break;
                                    default:
                                        tpx = tpage & 0x1F;
                                        tpy = (tpage & 0x20) * 4;
                                        break;
                                    }
                                    ((Rec24 *)&spr)->unk_0C = tpx;
                                    ((Rec24 *)&spr)->unk_0E = ((part->unk_04 & 0x3F) & 7) * 0x10;
                                    ((Rec24 *)&spr)->unk_0F = (((part->unk_04 & 0x3F) >> 3) * 0x10) + tpy;
                                }
                                if (rec->unk_43 != 0) {
                                    ((Rec24 *)&spr)->unk_10 = 0;
                                    switch (rec->unk_43 & 0x60) {
                                    case 0x20:
                                        ((Rec24 *)&spr)->unk_10 = 0x100;
                                        break;
                                    case 0x40:
                                        ((Rec24 *)&spr)->unk_10 = 0x120;
                                        break;
                                    default:
                                        if (rec->unk_05 & 0x80) {
                                            switch (rec->unk_05 & 0x7F) {
                                            case 1:
                                                ((Rec24 *)&spr)->unk_10 = ((part->unk_06 & 0xF0) >> 4) * 0x10;
                                                break;
                                            case 2:
                                                ((Rec24 *)&spr)->unk_10 = ((part->unk_06 & 0xE0) >> 4) * 0x10;
                                                break;
                                            case 4:
                                                ((Rec24 *)&spr)->unk_10 = ((part->unk_06 & 0xC0) >> 4) * 0x10;
                                                break;
                                            case 8:
                                                ((Rec24 *)&spr)->unk_10 = ((part->unk_06 & 0x80) >> 4) * 0x10;
                                                break;
                                            case 0x20:
                                            case 0x40:
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                    ((Rec24 *)&spr)->unk_12 = (rec->unk_43 & 0x1F) + 0x1E0;
                                } else {
                                    switch (rec->unk_05 & 0x7F) {
                                    case 1:
                                        ((Rec24 *)&spr)->unk_10 = ((part->unk_06 & 0xF0) >> 4) * 0x10;
                                        break;
                                    case 2:
                                        ((Rec24 *)&spr)->unk_10 = ((part->unk_06 & 0xE0) >> 4) * 0x10;
                                        break;
                                    case 4:
                                        ((Rec24 *)&spr)->unk_10 = ((part->unk_06 & 0xC0) >> 4) * 0x10;
                                        break;
                                    case 8:
                                        ((Rec24 *)&spr)->unk_10 = ((part->unk_06 & 0x80) >> 4) * 0x10;
                                        break;
                                    case 0x20:
                                    case 0x40:
                                    default:
                                        ((Rec24 *)&spr)->unk_10 = 0;
                                        break;
                                    }
                                    if (rec->unk_05 & 0x80) {
                                        ((Rec24 *)&spr)->unk_12 = ((part->unk_06 & 0xF00) >> 8) + 0x1F0;
                                    } else {
                                        ((Rec24 *)&spr)->unk_12 = ((part->unk_06 & 0xF00) >> 8) + 0x1E0;
                                    }
                                }
                                if (rec->unk_03 & 0x20) {
                                    ((Rec24 *)&spr)->unk_14 = col;
                                    ((Rec24 *)&spr)->unk_15 = col;
                                    ((Rec24 *)&spr)->unk_16 = col;
                                } else if (rec->unk_40 != 0 || rec->unk_41 != 0 || rec->unk_42 != 0) {
                                    ((Rec24 *)&spr)->unk_14 = rec->unk_40;
                                    ((Rec24 *)&spr)->unk_15 = rec->unk_41;
                                    ((Rec24 *)&spr)->unk_16 = rec->unk_42;
                                } else {
                                    ((Rec24 *)&spr)->unk_14 = rec->unk_07;
                                    ((Rec24 *)&spr)->unk_15 = rec->unk_07;
                                    ((Rec24 *)&spr)->unk_16 = rec->unk_07;
                                }
                                rot = *(s32 *)&rec->unk_20;
                                ((Rec24 *)&spr)->unk_20 = rot;
                                if (rec->unk_03 & 8) {
                                    if (rot != 0 || ((Rec24 *)&spr)->unk_1C != 0x1000 || ((Rec24 *)&spr)->unk_1E != 0x1000) {
                                        ((Rec24 *)&spr)->unk_08 = (u16)((Rec24 *)&spr)->unk_08 - 1;
                                        ((Rec24 *)&spr)->unk_0A = ((Rec24 *)&spr)->unk_0A - 1;
                                    }
                                }
                                if (rec->unk_1A != 0x1000 || rec->unk_1C != 0x1000) {
                                    ((Rec24 *)&spr)->unk_1C = rec->unk_1A * (((Rec24 *)&spr)->unk_1C / 0x1000);
                                    ((Rec24 *)&spr)->unk_1E = rec->unk_1C * (((Rec24 *)&spr)->unk_1E / 0x1000);
                                }
                                if (rec->unk_01 == 0) {
                                    func_80098680(&spr, &((u8 *)&D_800E8C70)[D_8011ECA0 * 0x14], 0);
                                } else {
                                    func_80098680(&spr, &((u8 *)&D_800E8C70)[D_8011ECA0 * 0x14],
                                                  (rec->unk_01 - 1) & 0xFFFF);
                                }
                            } while (--j >= 0);
                        }
                    }
                    if (!(rec->unk_02 & 0x40) && ((rec->unk_02 & 1) == 1)) {
                        if ((rec->unk_06 & 0x80) && (rec->unk_06 & 0x7F) != 0) {
                            rec->unk_08 += rec->unk_06 & 0x7F;
                        } else if (rec->unk_06 != 0) {
                            if (D_800E6280.unk_10F8 % rec->unk_06 == 0) {
                                rec->unk_08++;
                            }
                        }
                        if (rec->unk_08 >= ((s16 **)rec->unk_10)[slot][step * 2 + 2]) {
                            if ((rec->unk_02 & 0x10) == 0x10) {
                                rec->unk_18++;
                                rec->unk_08 = 0;
                                rec->unk_02 &= ~0x20;
                            } else if ((rec->unk_02 & 0x20) == 0x20) {
                                rec->unk_18--;
                                rec->unk_08 = 0;
                                if (rec->unk_18 <= 0) {
                                    rec->unk_02 &= 0xFFDE;
                                }
                            } else {
                                rec->unk_18++;
                                rec->unk_08 = 0;
                            }
                        }
                        if (rec->unk_18 >= ((s16 **)rec->unk_10)[slot][0]) {
                            if ((rec->unk_02 & 2) == 2) {
                                rec->unk_18 = 0;
                                rec->unk_02 &= ~3;
                            } else if ((rec->unk_02 & 4) == 4) {
                                rec->unk_02 &= 0xFFFA;
                                rec->unk_18--;
                            } else if ((rec->unk_02 & 8) == 8) {
                                rec->unk_18 = 0;
                                rec->unk_02 &= 0xFFFC;
                                rec->unk_03 &= ~0x80;
                            } else if ((rec->unk_02 & 0x10) == 0x10) {
                                rec->unk_02 &= ~0x10;
                                rec->unk_02 |= 0x20;
                                if (rec->unk_03 & 8) {
                                    if (((s16 **)rec->unk_10)[slot][0] >= 3) {
                                        rec->unk_18 -= 3;
                                    } else {
                                        rec->unk_18 = 0;
                                    }
                                } else if (((s16 **)rec->unk_10)[slot][0] >= 2) {
                                    rec->unk_18 -= 1;
                                } else {
                                    rec->unk_18 = 0;
                                }
                            } else {
                                rec->unk_18 = 0;
                            }
                        }
                    }
                    if (rec->unk_03 & 0x10) {
                        if (rec->unk_2C != 0) {
                            rec->unk_24.w += rec->unk_2C;
                        }
                        if (rec->unk_30 != 0) {
                            rec->unk_28.w += rec->unk_30;
                        }
                    }
                }
            }
            i++;
            rec++;
        } while (*(s32 *)&gs->unk_1095[3] >= i);
    }
}
