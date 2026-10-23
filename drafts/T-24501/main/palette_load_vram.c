/* T-24501 draft: full-TU resolved edit cost419. Clean indexed loops restore the
 * original switch with explicit empty case1. Frame0x50 versus target0x78 and
 * base/selector ranges and scheduling differ. Not production C. */
void palette_load_vram(s32 arg0) {
    GameState *base;
    u16 x;
    u16 y;
    u16 width;
    u32 index;
    s32 first;
    s32 j;
    GameState *g;

    base = &D_800E6280;
    index = 0;
    if (D_800E6280.unk_1428 != 0) {
        g = &D_800E6280;
        do {
            first = g->unk_1328[0].unk_07 << 7;
            y = (g->unk_1328[0].unk_04 & 31) + 0x1E0;
            if (g->unk_1328[0].unk_04 < 32) {
                width = 0x100;
                x = 0;
                switch (g->unk_1328[0].unk_06) {
                case 0:
                    for (j = first + 0; j < first + 128; j++) {
                        if (((u32 *)g->unk_1328[0].unk_00)[j] & 0x80000000) {
                            ((u32 *)g->unk_1328[0].unk_00)[j] &= 0x7FFF7FFF;
                        }
                        if (((u32 *)g->unk_1328[0].unk_00)[j] & 0x8000) {
                            ((u32 *)g->unk_1328[0].unk_00)[j] &= 0x7FFF7FFF;
                        }
                    }
                    break;
                case 1:
                default:
                    break;
                case 2:
                    for (j = first + 0; j < first + 120; j++) {
                        if (((u32 *)g->unk_1328[0].unk_00)[j] & 0x80000000) {
                            ((u32 *)g->unk_1328[0].unk_00)[j] &= 0x7FFF7FFF;
                        }
                        if (((u32 *)g->unk_1328[0].unk_00)[j] & 0x8000) {
                            ((u32 *)g->unk_1328[0].unk_00)[j] &= 0x7FFF7FFF;
                        }
                    }
                    for (j = first + 120; j < first + 128; j++) {
                        if (((u32 *)g->unk_1328[0].unk_00)[j] & 0x7FFF0000) {
                            ((u32 *)g->unk_1328[0].unk_00)[j] |= 0x80000000;
                        }
                        if (((u32 *)g->unk_1328[0].unk_00)[j] & 0x7FFF) {
                            ((u32 *)g->unk_1328[0].unk_00)[j] |= 0x8000;
                        }
                    }
                    break;
                case 3:
                    for (j = first + 0; j < first + 128; j++) {
                        if (((u32 *)g->unk_1328[0].unk_00)[j] & 0x7FFF0000) {
                            ((u32 *)g->unk_1328[0].unk_00)[j] |= 0x80000000;
                        }
                        if (((u32 *)g->unk_1328[0].unk_00)[j] & 0x7FFF) {
                            ((u32 *)g->unk_1328[0].unk_00)[j] |= 0x8000;
                        }
                    }
                    break;
                case 4:
                    for (j = first + 0; j < first + 112; j++) {
                        if (((u32 *)g->unk_1328[0].unk_00)[j] & 0x80000000) {
                            ((u32 *)g->unk_1328[0].unk_00)[j] &= 0x7FFF7FFF;
                        }
                        if (((u32 *)g->unk_1328[0].unk_00)[j] & 0x8000) {
                            ((u32 *)g->unk_1328[0].unk_00)[j] &= 0x7FFF7FFF;
                        }
                    }
                    for (j = first + 112; j < first + 128; j++) {
                        if (((u32 *)g->unk_1328[0].unk_00)[j] & 0x7FFF0000) {
                            ((u32 *)g->unk_1328[0].unk_00)[j] |= 0x80000000;
                        }
                        if (((u32 *)g->unk_1328[0].unk_00)[j] & 0x7FFF) {
                            ((u32 *)g->unk_1328[0].unk_00)[j] |= 0x8000;
                        }
                    }
                    break;
                }
            } else {
                width = 0x20;
                if (g->unk_1328[0].unk_04 < 0x40) {
                    x = 0x100;
                } else {
                    x = 0x120;
                }
            }
            LoadSquare(x, y, width, 1, (u32 *)g->unk_1328[0].unk_00 + first);
            index++;
            g = (GameState *)((u8 *)g + 8);
        } while (index < (u8)base->unk_1428);
    }
    D_800E6280.unk_1428 = 0;
}
