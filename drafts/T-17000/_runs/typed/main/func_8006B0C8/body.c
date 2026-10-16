void func_8006B0C8(void) {
    s32 spA4;
    s32 sp9C;
    void *sp40;
    u8 *var_a1;
    u8 *var_t5;
    GsWord *var_ra;
    s32 *temp_t8;
    s32 *temp_v0_2;
    s32 *var_v1_2;
    s32 *var_v1_3;
    s32 temp_lo;
    s32 temp_t4;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t6_3;
    s32 temp_t7;
    s32 temp_t8_2;
    s32 temp_t8_3;
    s32 temp_t9;
    s32 temp_v0;
    s32 temp_v0_3;
    s32 var_a0_2;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_a2_3;
    s32 var_a3;
    s32 var_t3;
    s32 var_v1;
    u8 *var_a0;
    u8 *var_a1_2;
    u8 *var_a1_3;
    u8 *var_a1_4;
    u8 *var_a1_5;
    u8 *var_a1_6;
    u8 *var_a1_7;

    var_a1 = (u8 *)&D_800E6280;
    var_t3 = 0;
    do {
        if (*(s32 *)(var_a1 + 0x54C) & 0x20000000) {
            temp_t8 = (var_t3 * 4) + 0x54C + &D_800E6280;
            var_t3 += 1;
            *temp_t8 = *(s32 *)(var_a1 + 0x54C);
        }
        var_a1 += 4;
    } while ((u32) var_a1 < (u32) &D_800E6280.unk_01C[0].unk_04);
    if (var_t3 < 8) {
        var_a0 = (var_t3 * 4) + 0x54C + &D_800E6280;
        do {
            sp40 = var_a0;
            spA4 = var_t3;
            bzero(var_a0, 4);
            var_a0 += 4;
        } while (var_a0 != D_800E6280.unk_56C);
    }
    if (D_800E6280.unk_03F == 0xC) {
        var_a2 = 0;
        if (var_t3 > 0) {
            temp_v0 = var_t3 & 3;
            if (temp_v0 != 0) {
                var_a1_2 = (0 * 4) + &D_800E6280;
                do {
                    var_a2 += 1;
                    if (((u16) *(u16 *)((unsigned char *)var_a1_2 + (1356)) >> 0xC) == 1) {
                        *(u8 *)((unsigned char *)var_a1_2 + (1357)) = (u8) ((*(u8 *)((unsigned char *)var_a1_2 + (1357)) & 0xFF0F) | 0xD0);
                    }
                    var_a1_2 += 4;
                } while (temp_v0 != var_a2);
                if (var_a2 != var_t3) {
                    goto block_15;
                }
            } else {
block_15:
                var_a1_3 = (var_a2 * 4) + &D_800E6280;
                do {
                    if (((u16) *(u16 *)((unsigned char *)var_a1_3 + (1356)) >> 0xC) == 1) {
                        *(u8 *)((unsigned char *)var_a1_3 + (1357)) = (u8) ((*(u8 *)((unsigned char *)var_a1_3 + (1357)) & 0xFF0F) | 0xD0);
                    }
                    if (((u16) *(u16 *)((unsigned char *)var_a1_3 + (1360)) >> 0xC) == 1) {
                        *(u8 *)((unsigned char *)var_a1_3 + (1361)) = (u8) ((*(u8 *)((unsigned char *)var_a1_3 + (1361)) & 0xFF0F) | 0xD0);
                    }
                    if (((u16) *(u16 *)((unsigned char *)var_a1_3 + (1364)) >> 0xC) == 1) {
                        *(u8 *)((unsigned char *)var_a1_3 + (1365)) = (u8) ((*(u8 *)((unsigned char *)var_a1_3 + (1365)) & 0xFF0F) | 0xD0);
                    }
                    if (((u16) *(u16 *)((unsigned char *)var_a1_3 + (1368)) >> 0xC) == 1) {
                        *(u8 *)((unsigned char *)var_a1_3 + (1369)) = (u8) ((*(u8 *)((unsigned char *)var_a1_3 + (1369)) & 0xFF0F) | 0xD0);
                    }
                    var_a1_3 += 0x10;
                } while (var_a1_3 != ((var_t3 * 4) + &D_800E6280));
            }
        }
    }
    var_v1 = 0;
    if (var_t3 > 0) {
        var_ra = D_800E6280.unk_54C;
        var_t5 = (u8 *)&D_800E6280;
        do {
            temp_lo = ((u16) *(u16 *)(var_t5 + 0x54C) >> 0xC) * 0x64;
            temp_t4 = var_v1 + 1;
            temp_t6 = *(u16 *)(var_t5 + 0x54E) & 0x1F;
            var_a3 = var_v1;
            temp_t8_2 = (var_t3 - temp_t4) & 3;
            var_a2_2 = temp_t4;
            var_t5 += 4;
            var_a0_2 = temp_t6 + temp_lo;
            if (temp_t4 < var_t3) {
                if (temp_t8_2 != 0) {
                    var_v1_2 = &(&sp7C[0])[var_a2_2];
                    var_a1_4 = (var_a2_2 * 4) + &D_800E6280;
                    do {
                        var_v1_2 += 1;
                        temp_t9 = (*(u16 *)((unsigned char *)var_a1_4 + (1358)) & 0x1F) + (((u16) *(u16 *)((unsigned char *)var_a1_4 + (1356)) >> 0xC) * 0x64);
                        *(s32 *)((unsigned char *)var_v1_2 + (-4)) = temp_t9;
                        if (temp_t9 < var_a0_2) {
                            var_a0_2 = temp_t9;
                            var_a3 = var_a2_2;
                        }
                        var_a2_2 += 1;
                        var_a1_4 += 4;
                    } while ((temp_t8_2 + temp_t4) != var_a2_2);
                    if (var_a2_2 != var_t3) {
                        goto block_34;
                    }
                } else {
block_34:
                    var_v1_3 = &(&sp7C[0])[var_a2_2];
                    var_a1_5 = (var_a2_2 * 4) + &D_800E6280;
                    do {
                        temp_t6_2 = (*(u16 *)((unsigned char *)var_a1_5 + (1358)) & 0x1F) + (((u16) *(u16 *)((unsigned char *)var_a1_5 + (1356)) >> 0xC) * 0x64);
                        (*(s32 *)var_v1_3) = temp_t6_2;
                        if (temp_t6_2 < var_a0_2) {
                            var_a0_2 = temp_t6_2;
                            var_a3 = var_a2_2;
                        }
                        temp_t7 = (*(u16 *)((unsigned char *)var_a1_5 + (1362)) & 0x1F) + (((u16) *(u16 *)((unsigned char *)var_a1_5 + (1360)) >> 0xC) * 0x64);
                        *(s32 *)((unsigned char *)var_v1_3 + (4)) = temp_t7;
                        if (temp_t7 < var_a0_2) {
                            var_a0_2 = temp_t7;
                            var_a3 = var_a2_2 + 1;
                        }
                        temp_t8_3 = (*(u16 *)((unsigned char *)var_a1_5 + (1366)) & 0x1F) + (((u16) *(u16 *)((unsigned char *)var_a1_5 + (1364)) >> 0xC) * 0x64);
                        *(s32 *)((unsigned char *)var_v1_3 + (8)) = temp_t8_3;
                        if (temp_t8_3 < var_a0_2) {
                            var_a0_2 = temp_t8_3;
                            var_a3 = var_a2_2 + 2;
                        }
                        var_v1_3 += 4;
                        temp_t6_3 = (*(u16 *)((unsigned char *)var_a1_5 + (1370)) & 0x1F) + (((u16) *(u16 *)((unsigned char *)var_a1_5 + (1368)) >> 0xC) * 0x64);
                        *(s32 *)((unsigned char *)var_v1_3 + (-4)) = temp_t6_3;
                        if (temp_t6_3 < var_a0_2) {
                            var_a0_2 = temp_t6_3;
                            var_a3 = var_a2_2 + 3;
                        }
                        var_a2_2 += 4;
                        var_a1_5 += 0x10;
                    } while (var_a2_2 != var_t3);
                }
            }
            sp9C = var_ra->w;
            temp_v0_2 = (var_a3 * 4) + 0x54C + &D_800E6280;
            var_ra->w = *temp_v0_2;
            var_v1 = temp_t4;
            var_ra += 4;
            *temp_v0_2 = sp9C;
        } while (temp_t4 != var_t3);
    }
    var_a2_3 = 0;
    if (var_t3 > 0) {
        temp_v0_3 = var_t3 & 3;
        if (temp_v0_3 != 0) {
            var_a1_6 = (0 * 4) + &D_800E6280;
            do {
                var_a2_3 += 1;
                if (((u16) *(u16 *)((unsigned char *)var_a1_6 + (1356)) >> 0xC) == 0xD) {
                    *(u8 *)((unsigned char *)var_a1_6 + (1357)) = (u8) ((*(u8 *)((unsigned char *)var_a1_6 + (1357)) & 0xFF0F) | 0x10);
                }
                var_a1_6 += 4;
            } while (temp_v0_3 != var_a2_3);
            if (var_a2_3 != var_t3) {
                goto block_52;
            }
        } else {
block_52:
            var_a1_7 = (var_a2_3 * 4) + &D_800E6280;
            do {
                if (((u16) *(u16 *)((unsigned char *)var_a1_7 + (1356)) >> 0xC) == 0xD) {
                    *(u8 *)((unsigned char *)var_a1_7 + (1357)) = (u8) ((*(u8 *)((unsigned char *)var_a1_7 + (1357)) & 0xFF0F) | 0x10);
                }
                if (((u16) *(u16 *)((unsigned char *)var_a1_7 + (1360)) >> 0xC) == 0xD) {
                    *(u8 *)((unsigned char *)var_a1_7 + (1361)) = (u8) ((*(u8 *)((unsigned char *)var_a1_7 + (1361)) & 0xFF0F) | 0x10);
                }
                if (((u16) *(u16 *)((unsigned char *)var_a1_7 + (1364)) >> 0xC) == 0xD) {
                    *(u8 *)((unsigned char *)var_a1_7 + (1365)) = (u8) ((*(u8 *)((unsigned char *)var_a1_7 + (1365)) & 0xFF0F) | 0x10);
                }
                if (((u16) *(u16 *)((unsigned char *)var_a1_7 + (1368)) >> 0xC) == 0xD) {
                    *(u8 *)((unsigned char *)var_a1_7 + (1369)) = (u8) ((*(u8 *)((unsigned char *)var_a1_7 + (1369)) & 0xFF0F) | 0x10);
                }
                var_a1_7 += 0x10;
            } while (var_a1_7 != ((var_t3 * 4) + &D_800E6280));
        }
    }
}
