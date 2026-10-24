#define SHOUGATU_OVERRIDE_func_8013D5D8 /* s32 return consumes $v0 in ugen for func_8014265C (T-24098) */
#include "common.h"
#include "ovl/SHOUGATU.h"
#include "game.h"

s32 func_8013D5D8();

void func_80141780(void) {
    D_801464B0 = 0x801DA680;
    D_801464B4 = 0x801DA68C;
    D_801464B8 = 0x801DA72C;
    /* address literals: the data belongs to another overlay; the original loads it with a separate base register */
    D_801464BC = *(s16 *)0x801DA754;
    D_801464C0 = 0x801B0000;
    D_801464C4 = 0x801DA000;
    D_801464C8 = 0x801B2000;
    D_801464CC = 0x801B6000;
    D_801464D0 = 0x801BA000;
    D_801464D4 = 0x801BE000;
    D_801464D8 = 0x801C2000;
    D_801464DC = 0x801C6000;
    D_801464E0 = 0x801D6000;
}

void func_80141850(void) {
    D_801464E4 = 0x801D2430;
    D_801464E8 = 0x801D2434;
    D_801464EC = 0x801D2498;
    /* address literal, see func_80141780 */
    D_801464F0 = *(s16 *)0x801D24B4;
    D_801464F4 = 0x801B0000;
    D_801464C4 = 0x801D2000;
    D_801464F8 = 0x801B2000;
    D_801464FC = 0x801B6000;
    D_80146500 = 0x801BA000;
    D_80146504 = 0x801BE000;
    D_80146508 = 0x801C2000;
    D_8014650C = 0x801C6000;
    D_801464E0 = 0x801CE000;
}

void func_80141920(void) {
    D_80146510 = (u8 *)0x801D22D0;
    D_80146514 = (u8 *)0x801D22D8;
    D_80146518 = (u8 *)0x801D233C;
    D_8014651C = *(s16 *)0x801D2358;
    D_80146520 = (u8 *)0x801B0000;
    D_80146524 = (u8 *)0x801B2000;
    D_80146528 = (u8 *)0x801B6000;
    D_8014652C = (u8 *)0x801BA000;
    D_80146530 = (u8 *)0x801BE000;
    D_80146534 = (u8 *)0x801C2000;
    D_80146538 = (u8 *)0x801C6000;
}

void func_801419D0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;
    s16 n;

    tbl = D_80146804;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
    if (D_800E6280.step < 0x10U) {
        ((Actor *)D_8011ECD0)[96].brightness = (u8)D_800B593C;
        if (((Actor *)D_8011ECD0)[96].unk_18 == 0 && ((Actor *)D_8011ECD0)[96].unk_08 == 0) {
            n = ((Actor *)D_8011ECD0)[96].sequence_slot + 1;
            ((Actor *)D_8011ECD0)[96].sequence_slot = n;
            if (n >= 3) {
                ((Actor *)D_8011ECD0)[96].sequence_slot = 0;
            }
        }
    }
    func_80141D20();
}

void func_80141AC0(void) {
    D_800B5A60 = 1;
    func_8004284C();
}

void func_80141AE8(void) {
    u32 r;
    u8 flag;

    flag = 1;
    r = get_now_k();
    if (r >= 2U) {
        D_800E6280.step_timer.w += 1;
    }
    if (((u32)D_800E6280.step_timer.w < 0x73U || (u32)D_800E6280.step_timer.w % 60U != 6) && !((u8)D_80120652 & 1)) {
        flag = 0;
    }
    func_8013D5D8();
    if (flag == 0 && D_80122CD8 != 0) {
        D_80120652 = 0;
    }
}

void func_80141B98(void) {
    func_80044750(0x208);
    func_8004284C();
}

void func_80141BC0(void) {
    D_8014653C = get_k_speed();
    D_80122CFC = 0x72;
    k_speed_set(0x1E);
    k_disp_start(2);
    func_8004284C();
}

void func_80141C0C(void) {
    k_speed_set(D_8014653C);
    k_disp_start(1);
    func_8004284C();
}

void func_80141C44(void) {
    LoadSquare(0x280, 0, 0x40, 0x80, D_801464E0);
    D_800E6280.tpage_buf[10] = 1;
    D_80120666 = 2;
    D_801206AA = 0;
    func_8004284C();
}

void func_80141CA0(void) {
    cd_read_sectors(0x55, 0x801B0000, 0x8065);
    func_80141780();
    func_8004284C();
}

void func_80141CD8(void) {
    D_801467FC = 1;
    if (D_800E6280.current_girl == GIRL_SHIORI) {
        func_80044750(0x206);
    }
    D_801217D0[4].x = -0x64;
    func_8004284C();
}

/* Fall-through s32 return retains the early-exit return 0. T-24098 */
s32 func_80141D20(void) {
    s16 i;

    if (D_801467FC == 0) {
        return 0;
    }
    if (D_80146800 == 0x1E) {
        D_801467FC += 1;
        D_801217D0[4].x = 0xB4;
        D_80146798[0] = 1;
    } else if (D_80146800 == 0x5A) {
        D_801467FC += 1;
        D_801217D0[4].x = 0x28;
    /* FAKE: u16 pointer dereference on s16 D_800CA150 emits lhu instead of lh. T-24098 */
    } else if (*(u16 *)&D_800CA150 >= 6U && D_80146800 % 20U == 0 && D_801467FC < 0x32) {
        D_801467FC += 1;
        D_801217D0[D_801467FC + 3].unk_0E = (rand() % 2) * 8;
        if (D_801217D0[D_801467FC + 3].unk_0E == 8) {
            D_800E6280.quad_layer[D_801467FC + 3] = 0xC;
            D_80146798[D_801467FC - 1] = rand() % 2 + 3;
        } else {
            D_800E6280.quad_layer[D_801467FC + 3] = 9;
            D_80146798[D_801467FC - 1] = rand() % 2 + 1;
        }
        D_80146540[D_801467FC - 1] = 0x10000 / (D_80146798[D_801467FC - 1] + (D_80146798[D_801467FC - 1] >= 3) * 2 + 1);
    }
    for (i = 4; i < D_801467FC + 4; i++) {
        D_801217D0[i].tint_r = D_801217D0[i].tint_g = D_801217D0[i].tint_b = D_800B593C;
        D_801217D0[i].flags &= 0x7FFFFFFF;
        if (D_80146800 % (u32)D_80146798[i - 4] == 0) {
            D_801217D0[i].y += 1;
        }
        ((Fix32 *)D_80146608)[i - 4].w += D_80146540[i - 4];
        D_80146540[i - 4] += (D_801466D0[i - 4] - ((Fix32 *)D_80146608)[i - 4].w) >> 8;
        D_801217D0[i].x = ((Fix32 *)D_80146608)[i - 4].h[1];
        if (D_801217D0[i].y >= 0x29) {
            D_801217D0[i].x = rand() % 320 - 0x9F;
            ((Fix32 *)D_80146608)[i - 4].h[0] = 0;
            ((Fix32 *)D_80146608)[i - 4].h[1] = D_801217D0[i].x;
            D_801466D0[i - 4] = ((Fix32 *)D_80146608)[i - 4].w;
            D_801217D0[i].y = -0x78;
            D_801217D0[i].unk_0E = (rand() % 2) * 8;
            if (D_801217D0[i].unk_0E == 8) {
                D_800E6280.quad_layer[i] = 0xC;
                D_80146798[i - 4] = rand() % 2 + 3;
            } else {
                D_800E6280.quad_layer[i] = 9;
                D_80146798[i - 4] = rand() % 2 + 1;
            }
            D_80146540[i - 4] = 0x10000 / (D_80146798[i - 4] + (D_80146798[i - 4] >= 3) * 2 + 1);
        }
    }
    D_80146800 += 1;
}

void func_801421D4(void) {
    s16 i;

    srand(D_800E6280.frame_count);
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    func_800847B8(D_800E6280.current_girl);
    func_80065F34(0);
    func_80141780();
    func_8013C2E4();
    D_80145F34 = D_80145B9C;
    D_80145F38 = D_80145CD8;
    D_80145F3C = D_80145E14;
    load_palette(D_801464C4, 0x12, 1, 3, 0);
    for (i = 4; i < 0x36; i++) {
        D_801217D0[i].flags = 0xC1000000;
        D_801217D0[i].x = rand() % 320 - 0x9F;
        ((Fix32 *)D_80146608)[i - 4].h[0] = 0;
        ((Fix32 *)D_80146608)[i - 4].h[1] = D_801217D0[i].x;
        D_801466D0[i - 4] = ((Fix32 *)D_80146608)[i - 4].w;
        D_801217D0[i].y = -0x78;
        D_801217D0[i].w = 8;
        /* SpriteQuad.h is u8[2] in game.h; original writes a 16-bit halfword sh. T-24098 */
        *(s16 *)&D_801217D0[i].h = 8;
        D_801217D0[i].unk_0C = 0xA;
        D_801217D0[i].unk_0E = 0;
        D_801217D0[i].unk_0F = 0;
        D_801217D0[i].unk_10 = 0;
        D_801217D0[i].unk_12 = 0x1F2;
        D_801217D0[i].tint_r = 0x80;
        D_801217D0[i].tint_g = 0x80;
        D_801217D0[i].tint_b = 0x80;
        D_801217D0[i].anchor_x = 0;
        D_801217D0[i].anchor_y = 0;
        D_801217D0[i].unk_1C = 0x1000;
        D_801217D0[i].unk_1E = 0x1000;
        D_801217D0[i].unk_20 = 0;
        D_800E6280.quad_layer[i] = 9;
        D_80146798[i - 4] = 2;
        D_80146540[i - 4] = 0x8000;
    }
    load_palette(D_801464C0, 0x11, 1, 2, 0);
    func_80084E90(D_801464C8, D_801464CC, D_801464D0, D_801464D4, D_801464D8, D_801464DC);
    func_800850D4(D_801464B4, D_801464B8, D_801464B0, D_801464BC);
    ((Actor *)D_80120650)->unk_04 = ((Actor *)D_80120650)[1].unk_04 = 8;
    D_801467FC = 0;
    D_80146800 = 0;
    D_800E6280.girls[0].tokimeki += 1;
    D_800E6280.girls[0].affection += 2;
    check_para_limit();
    func_8004284C();
}

void func_801424CC(void) {
    bg_read_sub2(0x475A);
    func_80085B3C(2, 6);
    func_8004284C();
}

void func_80142500(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl51 tbl;

    tbl = D_801468A4;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
    func_8014288C();
}

void func_8014257C(void) {
    func_80062CD0(0x5C8D);
    func_8004284C();
}

void func_801425A4(void) {
    func_80044750(0x207);
    func_8004284C();
}

void func_801425CC(void) {
    func_80083440(5);
    func_8004284C();
}

void func_801425F4(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8571);
    func_80141850();
    func_8004284C();
}

void func_8014262C(void) {
    D_801206DB &= 0x7F;
    func_8004284C();
}

void func_8014265C(void) {
    func_8013D5D8();
    if (D_800E6280.step_timer.w++ == 0) {
        /* FAKE: the original has two identical block-copy loops here (both in State B, the state at
         * entry), and one copy line gives one loop; the repeated line is the only form found that
         * emits the second, the real source is unknown. T-24092, T-26002 */
        ((Actor *)D_80120650)[2] = ((Actor *)D_80120650)[0];
        ((Actor *)D_80120650)[2] = ((Actor *)D_80120650)[0];
        ((Actor *)D_80120650)[2].sequence_slot = 4;
    }
    if ((u16)D_800CA150 == 7 && D_800CA154 == 1 && check_end_k() == 0) {
        ((Actor *)D_80120650)[2].flags = 1;
    } else {
        ((Actor *)D_80120650)[2].flags = 0;
        ((Actor *)D_80120650)[2].unk_18 = 0;
    }
}

void func_80142784(void) {
    if (D_80122CDC != 0) {
        D_800E6280.step += 0x18;
        D_800E6280.event_flags[0x34] = 2;
        return;
    }
    D_800CA150 = (u16)D_800CA150 + 2;
    D_800E6280.girls[D_800E6280.current_girl].tokimeki += 1;
    D_800E6280.girls[D_800E6280.current_girl].affection += 2;
    check_para_limit();
    func_8004284C();
}

void func_80142844(void) {
    LoadSquare(0x280, 0, 0x40, 0x80, D_801464E0);
    D_800E6280.tpage_buf[10] = 1;
    func_8004284C();
}

/* Fall-through s32 return retains the early-exit return 0. T-24098 */
s32 func_8014288C(void) {
    s16 i;
    s16 count;

    if (D_801467FC == 0) {
        return 0;
    }
    if (D_80146800 == 0x1E) {
        D_801467FC += 1;
        D_801217D0[4].x = 0xB4;
        D_80146798[0] = 1;
    } else if (D_80146800 == 0x5A) {
        D_801467FC += 1;
        D_801217D0[4].x = 0x28;
    } else if (D_80146800 >= 0xC9U && D_80146800 % 20U == 0 && D_801467FC < 0x32) {
        D_801467FC += 1;
        D_801217D0[D_801467FC + 3].unk_0E = (rand() % 2) * 8;
        if (D_801217D0[D_801467FC + 3].unk_0E == 8) {
            D_800E6280.quad_layer[D_801467FC + 3] = 0xC;
            D_80146798[D_801467FC - 1] = rand() % 2 + 3;
        } else {
            D_800E6280.quad_layer[D_801467FC + 3] = 9;
            D_80146798[D_801467FC - 1] = rand() % 2 + 1;
        }
        D_80146540[D_801467FC - 1] = 0x10000 / (D_80146798[D_801467FC - 1] + (D_80146798[D_801467FC - 1] >= 3) * 2 + 1);
    }
    if (D_80122D04 == 0) {
        count = D_801467FC;
    } else {
        count = 0xA;
        for (i = 0xE; i < 0x36; i++) {
            D_801217D0[i].flags |= 0x80000000;
        }
    }
    for (i = 4; i < count + 4; i++) {
        D_801217D0[i].tint_r = D_801217D0[i].tint_g = D_801217D0[i].tint_b = D_800B593C;
        D_801217D0[i].flags &= 0x7FFFFFFF;
        if (D_80146800 % (u32)D_80146798[i - 4] == 0) {
            D_801217D0[i].y += 1;
        }
        ((Fix32 *)D_80146608)[i - 4].w += D_80146540[i - 4];
        D_80146540[i - 4] += (D_801466D0[i - 4] - ((Fix32 *)D_80146608)[i - 4].w) >> 8;
        D_801217D0[i].x = ((Fix32 *)D_80146608)[i - 4].h[1];
        if (D_801217D0[i].y >= 0x29) {
            D_801217D0[i].x = rand() % 320 - 0x9F;
            ((Fix32 *)D_80146608)[i - 4].h[0] = 0;
            ((Fix32 *)D_80146608)[i - 4].h[1] = D_801217D0[i].x;
            D_801466D0[i - 4] = ((Fix32 *)D_80146608)[i - 4].w;
            D_801217D0[i].y = -0x78;
            D_801217D0[i].unk_0E = (rand() % 2) * 8;
            if (D_80122D04 == 1) {
                D_801217D0[i].unk_0E = 0;
            }
            if (D_801217D0[i].unk_0E == 8) {
                D_800E6280.quad_layer[i] = 0xC;
                D_80146798[i - 4] = rand() % 2 + 3;
            } else {
                D_800E6280.quad_layer[i] = 9;
                D_80146798[i - 4] = rand() % 2 + 1;
            }
            D_80146540[i - 4] = 0x10000 / (D_80146798[i - 4] + (D_80146798[i - 4] >= 3) * 2 + 1);
        }
    }
    D_80146800 += 1;
}

void func_80142DA0(void) {
    s16 i;

    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    func_8013C2E4();
    func_800847B8(D_800E6280.current_girl);
    func_80141850();
    load_palette(D_801464F4, 0x11, 1, 2, 0);
    func_80084E90(D_801464F8, D_801464FC, D_80146500, D_80146504, D_80146508, D_8014650C);
    func_800850D4(D_801464E8, D_801464EC, D_801464E4, D_801464F0);
    ((Actor *)D_80120650)->unk_04 = ((Actor *)D_80120650)[1].unk_04 = 8;
    func_8013C2E4();
    D_80145F34 = D_80145BFC;
    D_80145F38 = D_80145D38;
    D_80145F3C = D_80145E74;
    strcpy(D_800CA19C, "伊集院邸");
    load_palette(D_801464C4, 0x12, 1, 3, 0);
    for (i = 4; i < 0x36; i++) {
        D_801217D0[i].flags = 0xC1000000;
        D_801217D0[i].x = rand() % 320 - 0x9F;
        ((Fix32 *)D_80146608)[i - 4].h[0] = 0;
        ((Fix32 *)D_80146608)[i - 4].h[1] = D_801217D0[i].x;
        D_801466D0[i - 4] = ((Fix32 *)D_80146608)[i - 4].w;
        D_801217D0[i].y = -0x78;
        D_801217D0[i].w = 8;
        /* SpriteQuad.h is u8[2] in game.h; original writes a 16-bit halfword sh. T-24098 */
        *(s16 *)&D_801217D0[i].h = 8;
        D_801217D0[i].unk_0C = 0xA;
        D_801217D0[i].unk_0E = 0;
        D_801217D0[i].unk_0F = 0;
        D_801217D0[i].unk_10 = 0;
        D_801217D0[i].unk_12 = 0x1F2;
        D_801217D0[i].tint_r = 0x80;
        D_801217D0[i].tint_g = 0x80;
        D_801217D0[i].tint_b = 0x80;
        D_801217D0[i].anchor_x = 0;
        D_801217D0[i].anchor_y = 0;
        D_801217D0[i].unk_1C = 0x1000;
        D_801217D0[i].unk_1E = 0x1000;
        D_801217D0[i].unk_20 = 0;
        D_800E6280.quad_layer[i] = 9;
        D_80146798[i - 4] = 2;
        D_80146540[i - 4] = 0x8000;
    }
    D_801467FC = 0;
    D_80146800 = 0;
    func_8004284C();
}

void func_8014306C(void) {
    bg_read_sub2(0x4752);
    func_80085B3C(2, 0x34);
    func_8004284C();
}

/* Fall-through s32 return retains the counter result in v0. T-24004 */
s32 func_801430A0(void) {
    s32 idx;
    FnTbl48 tbl;
    tbl = D_80146970;
    idx = D_800E6280.step;
    tbl.f[idx](0x3C);
    if (D_800E6280.step < 0x11U) {
        ((Actor *)D_8011ECD0)[96].brightness = D_800B593C;
        if (((Actor *)D_8011ECD0)[96].unk_18 == 0 && ((Actor *)D_8011ECD0)[96].unk_08 == 0) {
            ((Actor *)D_8011ECD0)[96].sequence_slot += 1;
            if (((Actor *)D_8011ECD0)[96].sequence_slot >= 3) ((Actor *)D_8011ECD0)[96].sequence_slot = 0;
        }
    }
    if (D_80122D04 != 0) {
        ((Actor *)D_8011ECD0)[98].draw_flags |= 0x80;
        D_801206DF = ((Actor *)D_8011ECD0)[96].brightness;
        if ((u32)D_800E6280.frame_count % 300U == 0) {
            func_80143228();
            D_801206DA = 3;
        }
    } else { ((Actor *)D_8011ECD0)[98].draw_flags &= 0xFF7F; }
}

void func_80143200(void) {
    func_80044750(0x205);
    func_8004284C();
}

void func_80143228(void) {
    func_80044750(0x504);
}

void func_80143248(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8BC9);
    func_80141920();
    func_8004284C();
}

void func_80143280(void) {
    D_801206F0 = 0;
    D_801206E0 = 0;
    D_801206DA = 3;
    func_80044750(0x503);
    func_8004284C();
}

void func_801432C4(void) {
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_800847B8(D_800E6280.current_girl);
    func_8013C2E4();
    D_80145F34 = D_80145C5C;
    D_80145F38 = D_80145D98;
    D_80145F3C = D_80145ED4;
    func_8004284C();
    D_800E6280.girls[D_800E6280.current_girl].tokimeki += 1;
    D_800E6280.girls[D_800E6280.current_girl].affection += 2;
    check_para_limit();
    strcpy(D_800CA19C, "近所の公園");
}

void func_801433A4(void) {
    D_800E6280.tpage_buf[26] = -1;
    D_800E6280.tpage_buf[58] = -1;
    func_80141920();
    load_palette(D_80146520, 0x11, 1, 2, 0);
    func_80084E90((s32) D_80146524, (s32) D_80146528, (s32) D_8014652C, (s32) D_80146530, (s32) D_80146534,
                  (s32) D_80146538);
    func_800850D4((s32) D_80146514, (s32) D_80146518, (s32) D_80146510, D_8014651C);
    ((Actor *) D_8011ECD0)[97].unk_04 = 8;
    ((Actor *) D_8011ECD0)[96].unk_04 = 8;
    ((Actor *) D_8011ECD0)[98] = ((Actor *) D_8011ECD0)[96];
    ((Actor *) D_8011ECD0)[98].sequence_slot = 4;
    ((Actor *) D_8011ECD0)[98].unk_18 = 0;
    ((Actor *) D_8011ECD0)[98].unk_08 = 0;
    ((Actor *) D_8011ECD0)[98].flags = 0;
    func_8004284C();
}

void func_801434E0(void) {
    bg_read_sub2(0x42E2);
    func_8004284C();
}

void func_80143508(void) {
    bg_read_sub2(0x47C3);
    func_80085B3C(2, 0x42);
    func_8004284C();
}
