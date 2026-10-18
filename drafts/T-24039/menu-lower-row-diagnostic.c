/* T-24039 lower-row diagnostic alternative. Not production C.
 * FAKE: retain mechanical shared-store gotos, two short coordinate assignments,
 * and the unreachable case 14 to preserve the measured diagnostic shape.
 * The case 14 bound is incompatible with the target second-page bound of six.
 */
void func_8013D88C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u32 i;
    s32 byteOffset;
    s16 x;
    s16 y;
    GameState *flags;
    s16 *available;
    s16 *xs;
    s16 *ys;
    s16 *steps;
    s16 spC0[12];
    s16 spA8[12];
    s16 sp90[12];
    s16 sp78[12];

    available = sp90;
    do {
        available += 1;
        *(s16 *)((unsigned char *)available + (-2)) = 0;
    } while ((u32) available < (u32) &sp90[10]);
    spA8[10] = -0x54;
    sp90[10] = 0x64;
    sp78[10] = 0x10;
    spC0[10] = 0;
    if (arg0 == 0) {
        set_kanji_string(-0x80, -0x52, 3, (s32) "デートスポット", 0);
        set_kanji_string(0, -0x52, 3, (s32) "次のページへ", 0);
        flags = &D_800E6280;
        i = 0;
        available = sp90;
        xs = spC0;
        ys = spA8;
        steps = sp78;
        do {
            if (flags->unk_120[0] & 0x7F) {
                goto block_27;
            }
            *available = 0;
            switch (i) {
            case 9:
                break;
            case 1:
                if ((((u32) (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) << 0x1E) >> 0x1F) == 1) && (arg1 == 0x5F) && (arg2 == 6)) {
block_27:
                    *available = 0x74;
                }
                break;
            case 3:
                if ((((u32) (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) << 0x1C) >> 0x1F) == 1) && (arg1 == 0x5F) && (arg2 == 0xC)) {
                    goto block_27;
                }
                break;
            case 5:
                if ((((u32) (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) << 0x1A) >> 0x1F) == 1) && (arg1 == 0x60) && (arg2 == 6)) {
                    goto block_27;
                }
                break;
            case 6:
                if ((((u8) D_800E6280.unk_F64[0] >> 7) == 1) && (arg1 == 0x60) && (arg2 == 0xC)) {
                    goto block_27;
                }
                break;
            case 7:
                if ((((u32) (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) << 0x1D) >> 0x1F) == 1) && (arg1 == 0x5F) && (arg2 == 9)) {
                    goto block_27;
                }
                break;
            }
            *steps = 0x10;
            x = -((s32)i / 5);
            x = -x * 0x88 - 0x88;
            y = (((s32)i % 5) * 0x10) - 0x38;
            *xs = x;
            *ys = y;
            if (*available != 0) {

                set_kanji_string(x, y, 3, (s32) D_800B342C[i], 0);
            }
            i += 1;
            available += 1;
            xs += 1;
            ys += 1;
            steps += 1;
            flags = (GameState *)((u8 *)flags + 1);
        } while (i != 0xA);
    } else {
        set_kanji_string(-0x80, -0x52, 3, (s32) "デートスポット", 0);
        set_kanji_string(0, -0x52, 3, (s32) "前のページへ", 0);
        flags = &D_800E6280;
        steps = sp78;
        ys = spA8;
        i = 0;
        available = sp90;
        xs = spC0;
        do {
            if (flags->unk_120[10] & 0x7F) {
                if ((s32) i < 3) {
                    goto block_86;
                }
                if (i == 3) {
                    switch (arg2) {
                    case 12:
                    case 2:
                    case 0:
                        break;
                    default:
                        if (arg2 == D_800E6280.unk_03F) {
                            if (((u32) ((*(u32 *)D_800E6280.unk_F64) << 0x11) >> 0x1F) == 1) {
                                goto block_86;
                            }
                        } else if (((u8) D_800E6280.unk_03F % 3U) == 2) {
                            if (((u32) ((*(u32 *)D_800E6280.unk_F64) << 0x12) >> 0x1F) == 1) {
                                goto block_86;
                            }
                        } else if (((u32) ((*(u32 *)D_800E6280.unk_F64) << 0x11) >> 0x1F) == 1) {
                            goto block_86;
                        }
                        break;
                    }
                } else if (i == 4) {
                    if (arg2 == D_800E6280.unk_03F) {
                        if (((u32) ((*(u32 *)D_800E6280.unk_F64) << 0x11) >> 0x1F) == 1) {
                            goto block_86;
                        }
                    } else if (((u8) D_800E6280.unk_03F % 3U) == 2) {
                        if (((u32) ((*(u32 *)D_800E6280.unk_F64) << 0x12) >> 0x1F) == 1) {
                            goto block_86;
                        }
                    } else if (((u32) ((*(u32 *)D_800E6280.unk_F64) << 0x11) >> 0x1F) == 1) {
                        goto block_86;
                    }
                } else if (i == 5) {
                    if (arg2 == D_800E6280.unk_03F) {
                        if (((u32) ((*(u32 *)D_800E6280.unk_F64) << 0x11) >> 0x1F) == 1) {
                            goto block_86;
                        }
                    } else if (((u8) D_800E6280.unk_03F % 3U) == 2) {
                        if (((u32) ((*(u32 *)D_800E6280.unk_F64) << 0x12) >> 0x1F) == 1) {
                            goto block_86;
                        }
                    } else if (((u32) ((*(u32 *)D_800E6280.unk_F64) << 0x11) >> 0x1F) == 1) {
                        goto block_86;
                    }
                }
            } else {
                switch (i) {
                case 0:
                    if ((((u32) ((*(u32 *)D_800E6280.unk_F64) << 0x19) >> 0x1F) == 1) && (arg1 == 0x60) && (arg2 >= 9)) {
block_86:
                        *available = 0x74;
                    default:
                    case 14:;
                    }
                    break;
                case 1:
                    if (((D_800E6280.unk_F64[1] & 1) == 1) && (arg1 == 0x61) && (arg2 >= 3)) {
                        goto block_86;
                    }
                    break;
                case 3:
                    if ((((u32) ((*(u32 *)D_800E6280.unk_F64) << 0x12) >> 0x1F) == 1) && (D_800E6280.unk_03F == 2) && (arg2 == 3) && (D_800E6280.unk_03E == 0x60)) {
                        goto block_86;
                    }
                    break;
                case 4:
                    if ((((u32) ((*(u32 *)D_800E6280.unk_F64) << 0x12) >> 0x1F) == 1) && (arg1 == 0x5F) && (arg2 == 6) && (D_800E6280.unk_03F == 5)) {
                        goto block_86;
                    }
                    break;
                }
            }
            x = -((s32)i / 5);
            x = -x * 0x88 - 0x88;
            *xs = x;
            y = (((s32)i % 5) * 0x10) - 0x38;
            *ys = y;
            *steps = 0x10;
            if (*available != 0) {

                set_kanji_string(x, y, 3 & 0xFF, D_800B342C[i + 10], 0);
            }
            i += 1;
            steps += 1;
            ys += 1;
            available += 1;
            xs += 1;
            flags = (GameState *)((u8 *)flags + 1);
        } while ((s32) i < 6);
        steps = &sp78[6];
        ys = &spA8[6];
        i = 6;
        byteOffset = 0xC;
        xs = &spC0[6];
        do {
            if (i == 6) {
                available = (s16 *)((u8 *)sp90 + byteOffset);
                if (func_80066B40(arg2, arg3) == 1) {
                    set_kanji_string((s16) ((1 - ((s32)i / 5)) * -0x88), (s16) ((((s32)i % 5) * 0x10) - 0x38), 3, D_800B342C[16], 0);
                    *available = 0x74;
                } else if (func_80066B40(arg2, arg3) == 2) {
                    set_kanji_string((s16) ((- -((s32)i / 5) * 0x88) - 0x88), (s16) ((((s32)i % 5) * 0x10) - 0x38), 3, D_800B342C[19], 0);
                    *available = 0x74;
                } else {
                    *available = 0;
                }
            }
            x = -((s32)i / 5);
            x = -x * 0x88 - 0x88;
            y = (((s32)i % 5) * 0x10) - 0x38;
            if (i == 7) {
                available = (s16 *)((u8 *)sp90 + byteOffset);
                if (func_80066B40(arg2, arg3) == 2) {
                    set_kanji_string(x, y, 3, D_800B342C[20], 0);
                    *available = 0x74;
                } else if (func_80066B40(arg2, arg3) == 1) {
                    set_kanji_string(x, y, 3, D_800B342C[17], 0);
                    *available = 0x74;
                } else {
                    *available = 0;
                }
            }
            if (i == 8) {
                sp90[byteOffset / 2] = 0;
            }
            if (i == 9) {
                available = (s16 *)((u8 *)sp90 + byteOffset);
                if ((((u16) (*(u32 *)D_800E6280.unk_F64) >> 0xF) == 1) && (arg2 == 8) && (((s32) (arg3 + 6) / 7) == 1)) {
                    set_kanji_string(x, y, 3, D_800B342C[18], 0);
                    *available = 0x74;
                } else {
                    *available = 0;
                }
            }
            *xs = x;
            *ys = y;
            i += 1;
            *steps = 0x10;
            steps += 1;
            ys += 1;
            byteOffset += 2;
            xs += 1;
        } while (i != 0xA);
    }
    menu_set(0, 0xB, spC0, spA8, sp90, sp78);
    D_800E6280.unk_1093 = 0xA;
    func_80048F64(0x60);
    REC(96).unk_01 = 4;
    REC(96).unk_02 = 0x40;
    REC(96).unk_38 = 0x40000000;
    REC(96).unk_03 = 0x84;
    REC(96).unk_0C = sprite_frame_table;
    REC(96).unk_10 = D_800C9A14;
    REC(96).unk_34 = D_800C9730;
    REC(96).unk_04 = 8;
    REC(96).unk_05 = 1;
    REC(96).unk_18 = 0x40;
    REC(96).unk_26 = -0x10;
    REC(96).unk_2A = -0x13;
}
