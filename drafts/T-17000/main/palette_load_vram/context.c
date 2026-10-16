#include "common.h"
#include "game.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D10);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D14);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D18);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D1C);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D20);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D24);
u8 D_800B3D28 = 0;
u8 D_800B3D2C = 0;
s32 D_800B3D30 = 0;
s32 D_800B3D34 = 0;
s32 D_800B3D38 = 0;
s32 D_800B3D3C = 0;
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D40);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D44);
INCLUDE_RODATA("asm/data/main/80043510.data", D_800B3D48);
u8 D_800B3D4C = 0;
u8 D_800B3D50 = 0;
u8 D_800B3D54[12] = { 0 };

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80043510);

void draw2d3d(u8 arg0, u8 arg1) {
    D_800E6280.unk_1114 = arg0;
    D_800E6280.unk_1113 = arg1;
}

void back_clear_switch(s32 arg0) {
    if (arg0 != 0) {
        D_800E6280.unk_039 = 1;
    } else {
        D_800E6280.unk_039 = 0;
    }
}

/* Read queue counters directly and write records through g to preserve the
 * original register allocation. K&R parameters keep narrow home slots. T-9930. */
s32 load_palette(arg0, arg1, arg2, arg3, arg4)
s32 arg0;
u8 arg1;
u8 arg2;
u8 arg3;
u8 arg4;
{
    u32 index;
    GameState *g;

    g = &D_800E6280;
    index = D_800E6280.unk_1428;
    if (index < 32) {
        g->unk_1328[index].unk_00 = arg0;
        g->unk_1328[index].unk_04 = arg1;
        g->unk_1328[index].unk_05 = arg2;
        g->unk_1328[index].unk_06 = arg3;
        g->unk_1328[index].unk_07 = arg4;
        g->unk_1428++;
        return 1;
    }
    return 0;
}

s32 load_csr_ab(arg0, arg1, arg2, arg3, arg4, arg5)
s32 arg0;
u8 arg1;
u16 arg2;
u16 arg3;
u16 arg4;
u16 arg5;
{
    u32 index;
    GameState *g;

    g = &D_800E6280;
    index = D_800E6280.unk_1429;
    if (index < 32) {
        g->unk_142C[index].unk_00 = arg0;
        g->unk_142C[index].unk_04 = arg1;
        g->unk_142C[index].unk_05 = 0;
        g->unk_142C[index].unk_06 = 0;
        g->unk_142C[index].unk_07 = 0;
        g->unk_142C[index].unk_08 = arg2;
        g->unk_142C[index].unk_0A = arg3;
        g->unk_142C[index].unk_0C = arg4;
        g->unk_142C[index].unk_0E = arg5;
        g->unk_1429++;
        return 1;
    }
    return 0;
}

s32 load_csr_tp(arg0, arg1, arg2, arg3, arg4, arg5, arg6)
s32 arg0;
u8 arg1;
u8 arg2;
u16 arg3;
u16 arg4;
u16 arg5;
u16 arg6;
{
    u32 index;
    GameState *g;

    g = &D_800E6280;
    index = D_800E6280.unk_1429;
    if (index < 32) {
        g->unk_142C[index].unk_00 = arg0;
        g->unk_142C[index].unk_04 = arg1;
        g->unk_142C[index].unk_05 = arg2;
        g->unk_142C[index].unk_06 = 0;
        g->unk_142C[index].unk_07 = 0;
        g->unk_142C[index].unk_08 = arg3;
        g->unk_142C[index].unk_0A = arg4;
        g->unk_142C[index].unk_0C = arg5;
        g->unk_142C[index].unk_0E = arg6;
        g->unk_1429++;
        return 1;
    }
    return 0;
}

/* NON_MATCHING: T-10850, 26 differing listing lines; see the ticket scratch evidence. */
#ifdef NON_MATCHING
void csr_load_vram(s32 arg0) {
    GameState *base;
    s32 var_s1;
    s32 temp_v0;

    base = &D_800E6280;
    var_s1 = 0;
    if (base->unk_1429 != 0) {
        do {
            temp_v0 = base->unk_142C[var_s1].unk_04;
            if (temp_v0 == 0) {
                LoadSquare(base->unk_142C[var_s1].unk_08, base->unk_142C[var_s1].unk_0A, base->unk_142C[var_s1].unk_0C, base->unk_142C[var_s1].unk_0E, (void *) base->unk_142C[var_s1].unk_00);
            } else {
                LoadSquare((((temp_v0 & 0xF) << 6) + base->unk_142C[var_s1].unk_08), (((temp_v0 & 0x10) * 0x10) + base->unk_142C[var_s1].unk_0A), ((u16) base->unk_142C[var_s1].unk_0C >> base->unk_142C[var_s1].unk_05), base->unk_142C[var_s1].unk_0E, (void *) base->unk_142C[var_s1].unk_00);
            }
            var_s1 += 1;
        } while (var_s1 < (u8) base->unk_1429);
    }
    base->unk_1429 = 0U;
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80043510", csr_load_vram);
#endif

void palette_load_vram(s32 arg0) {
    GameState *sp74;
    u16 sp72;
    u16 sp70;
    u16 sp6E;
    u32 sp68;
    GameState *sp30;
    GameState *var_t1;
    s32 *var_v0_11;
    s32 *var_v0_12;
    s32 *var_v0_14;
    s32 *var_v0_15;
    s32 *var_v0_17;
    s32 *var_v0_18;
    s32 *var_v0_2;
    s32 *var_v0_3;
    s32 *var_v0_5;
    s32 *var_v0_6;
    s32 *var_v0_8;
    s32 *var_v0_9;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_a0_4;
    s32 temp_a0_5;
    s32 temp_a0_6;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_a2_3;
    s32 temp_a2_4;
    s32 temp_t7;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_a0_5;
    s32 var_a0_6;
    s32 var_v0;
    s32 var_v0_10;
    s32 var_v0_13;
    s32 var_v0_16;
    s32 var_v0_4;
    s32 var_v0_7;
    s32 var_v1;
    s32 var_v1_10;
    s32 var_v1_11;
    s32 var_v1_12;
    s32 var_v1_13;
    s32 var_v1_14;
    s32 var_v1_15;
    s32 var_v1_16;
    s32 var_v1_17;
    s32 var_v1_18;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    s32 var_v1_5;
    s32 var_v1_6;
    s32 var_v1_7;
    s32 var_v1_8;
    s32 var_v1_9;
    u32 var_ra;
    u8 temp_t2;
    u8 temp_t8;
    u8 temp_v0;

    sp74 = &D_800E6280;
    var_ra = 0;
    if (D_800E6280.unk_1428 != 0) {
        var_t1 = &D_800E6280;
        do {
            temp_v0 = var_t1->unk_1328[0].unk_04;
            temp_t2 = var_t1->unk_1328[0].unk_07;
            temp_t7 = temp_t2 << 7;
            sp70 = (temp_v0 & 0x1F) + 0x1E0;
            if (temp_v0 < 0x20U) {
                temp_t8 = var_t1->unk_1328[0].unk_06;
                sp6E = 0x100;
                sp72 = 0;
                switch (temp_t8) {
                case 0:
                    temp_a2 = temp_t7 + 0x80;
                    var_v0 = temp_t7;
                    if (temp_t7 < temp_a2) {
                        if (((temp_t2 * 0) + 0x80) & 1) {
                            temp_a0 = temp_t7 * 4;
                            var_v0_2 = var_t1->unk_1328[0].unk_00 + temp_a0;
                            var_v1 = *var_v0_2;
                            if (var_v1 & 0x80000000) {
                                *var_v0_2 = var_v1 & 0x7FFF7FFF;
                                var_v0_2 = var_t1->unk_1328[0].unk_00 + temp_a0;
                                var_v1 = *var_v0_2;
                            }
                            if (var_v1 & 0x8000) {
                                *var_v0_2 = var_v1 & 0x7FFF7FFF;
                            }
                            var_v0 = temp_t7 + 1;
                            if (var_v0 != temp_a2) {
                                goto block_12;
                            }
                        } else {
block_12:
                            var_a0 = var_v0 * 4;
                            do {
                                var_v0_3 = var_t1->unk_1328[0].unk_00 + var_a0;
                                var_v1_2 = *var_v0_3;
                                if (var_v1_2 & 0x80000000) {
                                    *var_v0_3 = var_v1_2 & 0x7FFF7FFF;
                                    var_v0_3 = var_t1->unk_1328[0].unk_00 + var_a0;
                                    var_v1_2 = *var_v0_3;
                                }
                                if (var_v1_2 & 0x8000) {
                                    *var_v0_3 = var_v1_2 & 0x7FFF7FFF;
                                    var_v0_3 = var_t1->unk_1328[0].unk_00 + var_a0;
                                }
                                var_v1_3 = *(s32 *)((unsigned char *)var_v0_3 + (4));
                                if (var_v1_3 & 0x80000000) {
                                    *(s32 *)((unsigned char *)var_v0_3 + (4)) = (s32) (var_v1_3 & 0x7FFF7FFF);
                                    var_v0_3 = var_t1->unk_1328[0].unk_00 + var_a0;
                                    var_v1_3 = *(s32 *)((unsigned char *)var_v0_3 + (4));
                                }
                                var_a0 += 8;
                                if (var_v1_3 & 0x8000) {
                                    *(s32 *)((unsigned char *)var_v0_3 + (4)) = (s32) (var_v1_3 & 0x7FFF7FFF);
                                }
                            } while (var_a0 != (temp_a2 * 4));
                        }
                    }
                    break;
                default:
                    break;
                case 2:
                    temp_a1 = temp_t7 + 0x78;
                    var_v0_4 = temp_t7;
                    if (temp_t7 < temp_a1) {
                        if (((temp_t2 * 0) + 0x78) & 1) {
                            temp_a0_2 = temp_t7 * 4;
                            var_v0_5 = var_t1->unk_1328[0].unk_00 + temp_a0_2;
                            var_v1_4 = *var_v0_5;
                            if (var_v1_4 & 0x80000000) {
                                *var_v0_5 = var_v1_4 & 0x7FFF7FFF;
                                var_v0_5 = var_t1->unk_1328[0].unk_00 + temp_a0_2;
                                var_v1_4 = *var_v0_5;
                            }
                            if (var_v1_4 & 0x8000) {
                                *var_v0_5 = var_v1_4 & 0x7FFF7FFF;
                            }
                            var_v0_4 = temp_t7 + 1;
                            if (var_v0_4 != temp_a1) {
                                goto block_31;
                            }
                        } else {
block_31:
                            var_a0_2 = var_v0_4 * 4;
                            do {
                                var_v0_6 = var_t1->unk_1328[0].unk_00 + var_a0_2;
                                var_v1_5 = *var_v0_6;
                                if (var_v1_5 & 0x80000000) {
                                    *var_v0_6 = var_v1_5 & 0x7FFF7FFF;
                                    var_v0_6 = var_t1->unk_1328[0].unk_00 + var_a0_2;
                                    var_v1_5 = *var_v0_6;
                                }
                                if (var_v1_5 & 0x8000) {
                                    *var_v0_6 = var_v1_5 & 0x7FFF7FFF;
                                    var_v0_6 = var_t1->unk_1328[0].unk_00 + var_a0_2;
                                }
                                var_v1_6 = *(s32 *)((unsigned char *)var_v0_6 + (4));
                                if (var_v1_6 & 0x80000000) {
                                    *(s32 *)((unsigned char *)var_v0_6 + (4)) = (s32) (var_v1_6 & 0x7FFF7FFF);
                                    var_v0_6 = var_t1->unk_1328[0].unk_00 + var_a0_2;
                                    var_v1_6 = *(s32 *)((unsigned char *)var_v0_6 + (4));
                                }
                                var_a0_2 += 8;
                                if (var_v1_6 & 0x8000) {
                                    *(s32 *)((unsigned char *)var_v0_6 + (4)) = (s32) (var_v1_6 & 0x7FFF7FFF);
                                }
                            } while (var_a0_2 != (temp_a1 * 4));
                        }
                    }
                    temp_a2_2 = temp_t7 + 0x80;
                    var_v0_7 = temp_a1;
                    if (temp_a1 < temp_a2_2) {
                        temp_a0_3 = temp_a1 * 4;
                        if (((temp_t7 - temp_a1) + 0x80) & 1) {
                            var_v0_8 = var_t1->unk_1328[0].unk_00 + temp_a0_3;
                            var_v1_7 = *var_v0_8;
                            if (var_v1_7 & 0x7FFF0000) {
                                *var_v0_8 = var_v1_7 | 0x80000000;
                                var_v0_8 = var_t1->unk_1328[0].unk_00 + temp_a0_3;
                                var_v1_7 = *var_v0_8;
                            }
                            if (var_v1_7 & 0x7FFF) {
                                *var_v0_8 = var_v1_7 | 0x8000;
                            }
                            var_v0_7 = temp_a1 + 1;
                            if (var_v0_7 != temp_a2_2) {
                                goto block_48;
                            }
                        } else {
block_48:
                            var_a0_3 = var_v0_7 * 4;
                            var_v0_9 = var_t1->unk_1328[0].unk_00 + var_a0_3;
                            do {
                                var_v1_8 = *var_v0_9;
                                if (var_v1_8 & 0x7FFF0000) {
                                    *var_v0_9 = var_v1_8 | 0x80000000;
                                    var_v0_9 = var_t1->unk_1328[0].unk_00 + var_a0_3;
                                    var_v1_8 = *var_v0_9;
                                }
                                if (var_v1_8 & 0x7FFF) {
                                    *var_v0_9 = var_v1_8 | 0x8000;
                                    var_v0_9 = var_t1->unk_1328[0].unk_00 + var_a0_3;
                                }
                                var_v1_9 = *(s32 *)((unsigned char *)var_v0_9 + (4));
                                if (var_v1_9 & 0x7FFF0000) {
                                    *(s32 *)((unsigned char *)var_v0_9 + (4)) = (s32) (var_v1_9 | 0x80000000);
                                    var_v0_9 = var_t1->unk_1328[0].unk_00 + var_a0_3;
                                    var_v1_9 = *(s32 *)((unsigned char *)var_v0_9 + (4));
                                }
                                var_a0_3 += 8;
                                if (var_v1_9 & 0x7FFF) {
                                    *(s32 *)((unsigned char *)var_v0_9 + (4)) = (s32) (var_v1_9 | 0x8000);
                                }
                                var_v0_9 = var_t1->unk_1328[0].unk_00 + var_a0_3;
                            } while (var_a0_3 != (temp_a2_2 * 4));
                        }
                    }
                    break;
                case 3:
                    temp_a2_3 = temp_t7 + 0x80;
                    var_v0_10 = temp_t7;
                    if (temp_t7 < temp_a2_3) {
                        if (((temp_t2 * 0) + 0x80) & 1) {
                            temp_a0_4 = temp_t7 * 4;
                            var_v0_11 = var_t1->unk_1328[0].unk_00 + temp_a0_4;
                            var_v1_10 = *var_v0_11;
                            if (var_v1_10 & 0x7FFF0000) {
                                *var_v0_11 = var_v1_10 | 0x80000000;
                                var_v0_11 = var_t1->unk_1328[0].unk_00 + temp_a0_4;
                                var_v1_10 = *var_v0_11;
                            }
                            if (var_v1_10 & 0x7FFF) {
                                *var_v0_11 = var_v1_10 | 0x8000;
                            }
                            var_v0_10 = temp_t7 + 1;
                            if (var_v0_10 != temp_a2_3) {
                                goto block_66;
                            }
                        } else {
block_66:
                            var_a0_4 = var_v0_10 * 4;
                            do {
                                var_v0_12 = var_t1->unk_1328[0].unk_00 + var_a0_4;
                                var_v1_11 = *var_v0_12;
                                if (var_v1_11 & 0x7FFF0000) {
                                    *var_v0_12 = var_v1_11 | 0x80000000;
                                    var_v0_12 = var_t1->unk_1328[0].unk_00 + var_a0_4;
                                    var_v1_11 = *var_v0_12;
                                }
                                if (var_v1_11 & 0x7FFF) {
                                    *var_v0_12 = var_v1_11 | 0x8000;
                                    var_v0_12 = var_t1->unk_1328[0].unk_00 + var_a0_4;
                                }
                                var_v1_12 = *(s32 *)((unsigned char *)var_v0_12 + (4));
                                if (var_v1_12 & 0x7FFF0000) {
                                    *(s32 *)((unsigned char *)var_v0_12 + (4)) = (s32) (var_v1_12 | 0x80000000);
                                    var_v0_12 = var_t1->unk_1328[0].unk_00 + var_a0_4;
                                    var_v1_12 = *(s32 *)((unsigned char *)var_v0_12 + (4));
                                }
                                var_a0_4 += 8;
                                if (var_v1_12 & 0x7FFF) {
                                    *(s32 *)((unsigned char *)var_v0_12 + (4)) = (s32) (var_v1_12 | 0x8000);
                                }
                            } while (var_a0_4 != (temp_a2_3 * 4));
                        }
                    }
                    break;
                case 4:
                    temp_a1_2 = temp_t7 + 0x70;
                    var_v0_13 = temp_t7;
                    if (temp_t7 < temp_a1_2) {
                        if (((temp_t2 * 0) + 0x70) & 1) {
                            temp_a0_5 = temp_t7 * 4;
                            var_v0_14 = var_t1->unk_1328[0].unk_00 + temp_a0_5;
                            var_v1_13 = *var_v0_14;
                            if (var_v1_13 & 0x80000000) {
                                *var_v0_14 = var_v1_13 & 0x7FFF7FFF;
                                var_v0_14 = var_t1->unk_1328[0].unk_00 + temp_a0_5;
                                var_v1_13 = *var_v0_14;
                            }
                            if (var_v1_13 & 0x8000) {
                                *var_v0_14 = var_v1_13 & 0x7FFF7FFF;
                            }
                            var_v0_13 = temp_t7 + 1;
                            if (var_v0_13 != temp_a1_2) {
                                goto block_84;
                            }
                        } else {
block_84:
                            var_a0_5 = var_v0_13 * 4;
                            do {
                                var_v0_15 = var_t1->unk_1328[0].unk_00 + var_a0_5;
                                var_v1_14 = *var_v0_15;
                                if (var_v1_14 & 0x80000000) {
                                    *var_v0_15 = var_v1_14 & 0x7FFF7FFF;
                                    var_v0_15 = var_t1->unk_1328[0].unk_00 + var_a0_5;
                                    var_v1_14 = *var_v0_15;
                                }
                                if (var_v1_14 & 0x8000) {
                                    *var_v0_15 = var_v1_14 & 0x7FFF7FFF;
                                    var_v0_15 = var_t1->unk_1328[0].unk_00 + var_a0_5;
                                }
                                var_v1_15 = *(s32 *)((unsigned char *)var_v0_15 + (4));
                                if (var_v1_15 & 0x80000000) {
                                    *(s32 *)((unsigned char *)var_v0_15 + (4)) = (s32) (var_v1_15 & 0x7FFF7FFF);
                                    var_v0_15 = var_t1->unk_1328[0].unk_00 + var_a0_5;
                                    var_v1_15 = *(s32 *)((unsigned char *)var_v0_15 + (4));
                                }
                                var_a0_5 += 8;
                                if (var_v1_15 & 0x8000) {
                                    *(s32 *)((unsigned char *)var_v0_15 + (4)) = (s32) (var_v1_15 & 0x7FFF7FFF);
                                }
                            } while (var_a0_5 != (temp_a1_2 * 4));
                        }
                    }
                    temp_a2_4 = temp_t7 + 0x80;
                    var_v0_16 = temp_a1_2;
                    if (temp_a1_2 < temp_a2_4) {
                        temp_a0_6 = temp_a1_2 * 4;
                        if (((temp_t7 - temp_a1_2) + 0x80) & 1) {
                            var_v0_17 = var_t1->unk_1328[0].unk_00 + temp_a0_6;
                            var_v1_16 = *var_v0_17;
                            if (var_v1_16 & 0x7FFF0000) {
                                *var_v0_17 = var_v1_16 | 0x80000000;
                                var_v0_17 = var_t1->unk_1328[0].unk_00 + temp_a0_6;
                                var_v1_16 = *var_v0_17;
                            }
                            if (var_v1_16 & 0x7FFF) {
                                *var_v0_17 = var_v1_16 | 0x8000;
                            }
                            var_v0_16 = temp_a1_2 + 1;
                            if (var_v0_16 != temp_a2_4) {
                                goto block_101;
                            }
                        } else {
block_101:
                            var_a0_6 = var_v0_16 * 4;
                            var_v0_18 = var_t1->unk_1328[0].unk_00 + var_a0_6;
                            do {
                                var_v1_17 = *var_v0_18;
                                if (var_v1_17 & 0x7FFF0000) {
                                    *var_v0_18 = var_v1_17 | 0x80000000;
                                    var_v0_18 = var_t1->unk_1328[0].unk_00 + var_a0_6;
                                    var_v1_17 = *var_v0_18;
                                }
                                if (var_v1_17 & 0x7FFF) {
                                    *var_v0_18 = var_v1_17 | 0x8000;
                                    var_v0_18 = var_t1->unk_1328[0].unk_00 + var_a0_6;
                                }
                                var_v1_18 = *(s32 *)((unsigned char *)var_v0_18 + (4));
                                if (var_v1_18 & 0x7FFF0000) {
                                    *(s32 *)((unsigned char *)var_v0_18 + (4)) = (s32) (var_v1_18 | 0x80000000);
                                    var_v0_18 = var_t1->unk_1328[0].unk_00 + var_a0_6;
                                    var_v1_18 = *(s32 *)((unsigned char *)var_v0_18 + (4));
                                }
                                var_a0_6 += 8;
                                if (var_v1_18 & 0x7FFF) {
                                    *(s32 *)((unsigned char *)var_v0_18 + (4)) = (s32) (var_v1_18 | 0x8000);
                                }
                                var_v0_18 = var_t1->unk_1328[0].unk_00 + var_a0_6;
                            } while (var_a0_6 != (temp_a2_4 * 4));
                        }
                    }
                    break;
                }
            } else {
                sp6E = 0x20;
                if (temp_v0 < 0x40U) {
                    sp72 = 0x100;
                } else {
                    sp72 = 0x120;
                }
            }
            sp68 = var_ra;
            sp30 = var_t1;
            LoadSquare(sp72, sp70, sp6E, 1U, var_t1->unk_1328[0].unk_00 + (temp_t7 * 4));
            var_ra += 1;
            var_t1 = (GameState *) &var_t1->unk_006[2];
        } while (var_ra < (u8) sp74->unk_1428);
    }
    D_800E6280.unk_1428 = 0;
}

void LoadSquare(u16 arg0, u16 arg1, u16 arg2, u16 arg3, void *arg4) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C884(&rect, arg4);
}

void MoveSquare(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4, u16 arg5) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C93C(&rect, arg4, arg5);
}

void StoreSquare(u16 arg0, u16 arg1, u16 arg2, u16 arg3, void *arg4) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C8E0(&rect, arg4);
}

void func_80044434(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_8004443C);

void func_80044700(s32 arg0, s32 arg1, s32 arg2) {
    RECT rect;
    s32 x;

    x = arg1;
    rect.x = x << 4;
    rect.y = arg0 + 0x1E0;
    rect.w = 0x10;
    rect.h = 1;
    func_8009C93C(&rect, 0x100, arg2 + 0x1E0);
}

s32 func_80044750(s32 arg0) {
    func_80079B10(arg0 & 0xFFFF);
    return 1;
}


s32 func_80044774(s32 arg0) {
    s32 *p;

    if (arg0 != 0) {
        p = (s32 *)0x8002E800;
    } else {
        p = (s32 *)0x8001C000;
    }
    if ((*p & 0xFFFF) != 0x40) {
        return -1;
    }
    if (arg0 != 0) {
        func_8004500C(D_800B3D44, D_800B3D48, arg0);
    } else {
        func_80044750(0x1200);
    }
    D_800B3D40 = arg0;
    return arg0 & 0xFF;
}

u8 func_8004480C(void) {
    return D_800B3D40;
}

u8 func_8004481C(void) {
    return D_800B3D44;
}


void func_8004482C(void) {
    D_800B3D54[0] = 0;
    D_800B3D54[1] = 0;
    D_800B3D44 = 0xFF;
    D_800B3D30 = 0;
    D_800B3D34 = 0;
    D_800B3D38 = 0;
    D_800B3D3C = 0;
    D_800B3D28 = 0;
    D_800B3D2C = 0;
    D_800B3D4C = 0;
    D_800B3D50 = 0;
    D_800B3D48 = 0;
}

void func_80044890(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
    D_800B3D44 = 0xFF;
    D_800B3D28 = 0;
    D_800B3D2C = 0;
    D_800B3D24 = (u8) a;
    D_800B3D4C = 0;
    D_800B3D50 = 1;
    D_800B3D20 = func_800460DC();
    if (a == 0) {
        func_800451E0(0x801B8000);
        if (D_800B3D30 == b) {
            D_800B3D20 -= 2;
            D_800B3D28 = 1;
        } else {
            if (b != 0) {
                func_800462C8(3U, 0x80010000, b);
                D_800B3D30 = b;
            } else {
                D_800B3D20 -= 1;
                D_800B3D28 = 1;
            }
            if (c != 0) {
                func_800462C8(0x20U, 0x801B8000, c);
            } else {
                D_800B3D20 -= 1;
                D_800B3D28 = 1;
            }
        }
        if (D_800B3D34 == d) {
            D_800B3D20 -= 3;
            D_800B3D2C = 1;
            return;
        }
        if (f != 0) {
            func_800462C8(0x25U, 0x8001C000, f);
            D_800B3D34 = d;
        } else {
            D_800B3D20 -= 1;
            D_800B3D2C = 1;
        }
        if (e != 0) {
            func_800462C8(0x50U, 0x801C8000, e);
        } else {
            D_800B3D20 -= 1;
            D_800B3D2C = 1;
        }
        if (d != 0) {
            func_800462C8(6U, 0x80012000, d);
            D_800B3D34 = d;
            return;
        }
        D_800B3D20 -= 1;
        D_800B3D2C = 1;
        return;
    }
    func_800451E0(0x801B8000);
    if (D_800B3D38 == b) {
        D_800B3D20 -= 2;
        D_800B3D28 = 1;
    } else {
        if (b != 0) {
            func_800462C8(3U, 0x8001A000, b);
            D_800B3D38 = b;
        } else {
            D_800B3D20 -= 1;
            D_800B3D28 = 1;
        }
        if (c != 0) {
            func_800462C8(0x35U, 0x801E0000, c);
        } else {
            D_800B3D20 -= 1;
            D_800B3D28 = 1;
        }
    }
    if (D_800B3D3C == d) {
        D_800B3D20 -= 3;
        D_800B3D2C = 1;
        return;
    }
    if (f != 0) {
        func_800462C8(0x25U, 0x8002E800, f);
        D_800B3D3C = d;
    } else {
        D_800B3D20 -= 1;
        D_800B3D2C = 1;
    }
    if (e != 0) {
        func_800462C8(0x50U, 0x801B8000, e);
    } else {
        D_800B3D20 -= 1;
        D_800B3D2C = 1;
    }
    if (d != 0) {
        func_800462C8(6U, 0x80016000, d);
        D_800B3D3C = d;
        return;
    }
    D_800B3D20 -= 1;
    D_800B3D2C = 1;
}

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80044C98);

s32 func_80044D54(void) {
    if (D_800B3D24 == 0) {
        if (D_800B3D28 == 0 && D_800B3D2C == 0) {
            func_800451E0(0x801B8000);
            func_80044750(0x11);
            return 1;
        }
        if (D_800B3D28 == 0) {
            func_800451E0(0x801B8000);
            func_80044750(0x13);
            return 1;
        }
        if (D_800B3D2C == 0) {
            func_800451E0(0x801C8000);
            func_80044750(0x14);
            return 1;
        }
        return 0;
    }
    if (D_800B3D28 == 0 && D_800B3D2C == 0) {
        func_800451E0(0x801B8000);
        func_80044750(0x12);
        return 1;
    }
    if (D_800B3D28 == 0) {
        func_800451E0(0x801E0000);
        func_80044750(0x16);
        return 1;
    }
    if (D_800B3D2C == 0) {
        func_800451E0(0x801B8000);
        func_80044750(0x15);
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80044E8C);

s32 func_80044F94(s32 arg0) {
    if (arg0 == 0) {
        if (func_80079E00(0) != 0 && func_80079E00(1) != 0) {
            return 1;
        }
        return 0;
    }
    if (func_80079E00(2) != 0 && func_80079E00(3) != 0) {
        return 1;
    }
    return 0;
}

s32 func_8004500C(s32 arg0, s32 arg1) {
    s32 *var_v0_2;

    if (arg0 != 0) {
        var_v0_2 = (s32 *)0x8002E800;
    } else {
        var_v0_2 = (s32 *)0x8001C000;
    }
    if ((*var_v0_2 & 0xFFFF) != 0x40) {
        return -1;
    }
    if (arg0 == 0) {
        func_80044750(0x22);
        func_80044750(0x21);
        func_80044750(0x2F);
        func_80044750((arg1 | 0x200) & 0xFFFF);
    } else {
        func_80044750(0x23);
        func_80044750(0x24);
        func_80044750(0x2E);
        func_80044750((arg1 | 0x200) & 0xFFFF);
    }
    D_800B3D44 = (u8) arg0;
    D_800B3D48 = (u8) arg1;
    D_800B3D40 = 1;
    /* no return here: the original returns the v0 of the last func_80044750 call (implicit int) */
}

s32 func_800450F4(s32 arg0, s32 arg1) {
    s32 *var_v0;

    if (arg0 != 0) {
        var_v0 = (s32 *)0x8002E800;
    } else {
        var_v0 = (s32 *)0x8001C000;
    }
    if ((*var_v0 & 0xFFFF) != 0x40) {
        return -1;
    }
    if (arg0 == 0) {
        func_80044750(0x22);
        func_80044750(0x2F);
        func_80044750((arg1 | 0x200) & 0xFFFF);
    } else {
        func_80044750(0x23);
        func_80044750(0x2E);
        func_80044750((arg1 | 0x200) & 0xFFFF);
    }
    D_800B3D44 = (u8) arg0;
    D_800B3D48 = (u8) arg1;
    D_800B3D40 = 1;
    /* no return here: the original returns the v0 of the last func_80044750 call (implicit int) */
}
