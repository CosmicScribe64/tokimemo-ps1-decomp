#include "common.h"
#include "game.h"
#include "psx_types.h"
#include "ovl/SHOUGATU.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146130);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146134);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146138);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_8014613C);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146140);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146144);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146148);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_8014614C);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146150);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146154);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146158);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_8014615C);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146160);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146164);
u8 D_80146168[4] = { 0 };
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_8014616C);
ShoColors6x2 D_801461D8 = { {
    { 0x00F870A0, 0x00F8A0C0 },
    { 0x0030C050, 0x0070E090 },
    { 0x005078F8, 0x0098C0F8 },
    { 0x00F8A828, 0x00F8D868 },
    { 0x00F88030, 0x00F8D078 },
    { 0x005850B0, 0x009098E8 },
} };
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146208);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146230);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146234);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146238);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_8014623C);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146240);
INCLUDE_RODATA("asm/ovl/SHOUGATU/data/SHOUGATU/8013F190.data", D_80146244);

void func_80140698(void);


void func_8013F190(void) {
    D_80146130 = (u8 *)0x801D6360;
    D_80146134 = (u8 *)0x801D6384;
    D_80146138 = (u8 *)0x801D6414;
    D_8014613C = *(s16 *)0x801D641C;
    D_80146140 = (u8 *)0x801B0000;
}

void func_8013F1E0(void) {
    D_80146144 = 0x801DDBB4;
    D_80146148 = 0x801DDBB8;
    D_8014614C = 0x801DDC70;
    D_80146154 = 0x801DB000;
}

/* Fall-through return preserves the sibling callback schedule (T-24026). */
s32 func_8013F224(void) {
    s32 idx; /* Index precedes the copied table (T-3330, T-20006). */
    FnTbl27 table;

    table = D_8014616C;
    idx = D_800E6280.step;
    table.f[idx](0x80);
    if (D_800E6280.step < 8) {
        ((DisplayActor44 *)D_8011ECD0)[96].brightness = D_800B593C;
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_18 == 0 && ((DisplayActor44 *)D_8011ECD0)[96].unk_08 == 0) {
            if (++((DisplayActor44 *)D_8011ECD0)[96].sequence_slot >= 3) {
                ((DisplayActor44 *)D_8011ECD0)[96].sequence_slot = 0;
            }
        }
    }
    if (D_800E6280.step >= 0xA && D_80146160 != 0) {
        func_80140314();
    }
    if (D_800E6280.step >= 0xA && D_80146164 != 0) {
        func_80140698();
    }
}

void func_8013F360(void) {
    if ((D_80146168[D_80146158] == 0xB) && (D_800E6280.year == 0x60)) {
        func_8013D324();
        return;
    }
    func_8004284C();
}

s32 func_8013F3C0(void) {
    if (D_800E6280.substep == 0) {
        func_8004E58C();
        func_800674B0();
        D_800E6280.substep += 1;
    }
    if (D_800E6280.substep == 1) {
        if ((u32)D_800E6280.step_timer.w++ >= 0x400) {
            func_800452C4();
            func_8004482C();
            D_800E6280.substep = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.substep += 1;
        } else {
            return 0;
        }
    }
    func_80041584();
    return func_80072B5C(1);
}

void func_8013F494(void) {
    s32 unused; /* FAKE: unused local restores observed implicit-copy stack offsets. T-13202. */
    s16 i;

    D_8014615C = 0;
    D_80146160 = 0;
    D_80146164 = 0;
    func_8013F190();
    i = 0;
    do {
        load_palette(D_80146140, (i + 0x10) & 0xFF, 1, 2, i);
        i++;
    } while (i < 9);
    func_80048F64(0x60);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 8;
    ((DisplayActor44 *)D_8011ECD0)[96].flags = 0x40;
    ((DisplayActor44 *)D_8011ECD0)[96].quad_flags = 0x01000000;
    ((DisplayActor44 *)D_8011ECD0)[96].draw_flags = 4;
    ((DisplayActor44 *)D_8011ECD0)[96].frame_table = D_80146134;
    ((DisplayActor44 *)D_8011ECD0)[96].sequence_table = D_80146138;
    ((DisplayActor44 *)D_8011ECD0)[96].tpage_table = D_80146130;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_14 = D_8014613C;
    ((DisplayActor44 *)D_8011ECD0)[96].sequence_slot = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].blink_period = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].x = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].y = 8;
    ((DisplayActor44 *)D_8011ECD0)[96].brightness = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 0;
    ((DisplayActor44 *)D_8011ECD0)[97] = ((DisplayActor44 *)D_8011ECD0)[96];
    func_8013F1E0();
    load_palette((u8 *) D_80146154, 0x1F, 1, 2, 0);
    ((DisplayActor44 *)D_8011ECD0)[98] = ((DisplayActor44 *)D_8011ECD0)[96];
    ((DisplayActor44 *)D_8011ECD0)[98].unk_01 = 0xB;
    ((DisplayActor44 *)D_8011ECD0)[98].unk_04 = 8;
    ((DisplayActor44 *)D_8011ECD0)[98].flags = 1;
    ((DisplayActor44 *)D_8011ECD0)[98].frame_table = D_80146148;
    ((DisplayActor44 *)D_8011ECD0)[98].sequence_table = D_8014614C;
    ((DisplayActor44 *)D_8011ECD0)[98].tpage_table = D_80146144;
    ((DisplayActor44 *)D_8011ECD0)[98].unk_14 = D_80146150;
    ((DisplayActor44 *)D_8011ECD0)[98].sequence_slot = (s16) (rand() % 3);
    ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0;
    ((DisplayActor44 *)D_8011ECD0)[98].x = -0xA0;
    ((DisplayActor44 *)D_8011ECD0)[98].y = -0x78;
    ((DisplayActor44 *)D_8011ECD0)[98].unk_43 = 0x1F;
    ((DisplayActor44 *)D_8011ECD0)[101] = ((DisplayActor44 *)D_8011ECD0)[99] = ((DisplayActor44 *)D_8011ECD0)[100] = ((DisplayActor44 *)D_8011ECD0)[98];
    ((DisplayActor44 *)D_8011ECD0)[99].x += 0x48;
    ((DisplayActor44 *)D_8011ECD0)[99].sequence_slot = (s16) (rand() % 3);
    ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 4;
    ((DisplayActor44 *)D_8011ECD0)[100].x += 0x90;
    ((DisplayActor44 *)D_8011ECD0)[100].sequence_slot = (s16) (rand() % 3);
    ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0xA;
    ((DisplayActor44 *)D_8011ECD0)[101].unk_01 = 0xA;
    ((DisplayActor44 *)D_8011ECD0)[101].flags = 0x40;
    ((DisplayActor44 *)D_8011ECD0)[101].sequence_slot = 0;
    D_80145F2C = 0;
    D_80145F20 = D_80145AF8;
    D_80145F24 = D_80145B2C;
    D_80145F28 = D_80145B60;
    func_8004284C();
}

void func_8013F9C4(void) {
    s32 temp = D_80145FE4;

    if (temp != 0 && temp != 7 && temp != 9) {
        D_801206DB |= 0x80;
    }
    if (temp != 0 && temp != 7 && temp != 9) {
        D_8012071F |= 0x80;
    }
    if (temp != 0 && temp != 7 && temp != 9) {
        ((u8 *)&D_80120760)[3] |= 0x80;
    }
    D_801207A7 |= 0x80;
    D_80146164 = 1;
    func_8004284C();
}

void func_8013FA80(void) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];

    a[0] = -0x96;
    a[1] = -0x48;
    a[2] = 6;
    a[3] = 0x54;
    b[0] = b[1] = b[2] = b[3] = -0x40;
    c[0] = c[1] = c[2] = c[3] = 0x40;
    d[0] = d[1] = d[2] = d[3] = 0x40;
    menu_set(0, 4, a, b, c, d);
    D_80122D30 = 1;
    func_8004284C();
}

/* Fall-through s32 return preserves the early-return register ($v0 = 0). T-24137. */
s32 func_8013FB28(void) {
    s16 i;
    GameState *g;

    g = &D_800E6280;
    D_80146168[0] = g->frame_count % 4;
    i = 1;
    while (1) {
        D_80146168[1] = (g->frame_count * 2 + i) % 4;
        if (D_80146168[0] != D_80146168[1]) {
            break;
        }
        i++;
    }
    while (1) {
        D_80146168[2] = (g->frame_count / 10 + i) % 4;
        if (D_80146168[0] != D_80146168[2] && D_80146168[1] != D_80146168[2]) {
            break;
        }
        i++;
    }
    while (1) {
        D_80146168[3] = i % 4;
        if (D_80146168[0] != D_80146168[3] && D_80146168[1] != D_80146168[3] && D_80146168[2] != D_80146168[3]) {
            break;
        }
        i++;
    }
    for (i = 0; i < 4; i++) {
        if (D_80146168[i] == 1) {
            D_80146168[i] = 0xB;
        } else if (D_80146168[i] == 2) {
            D_80146168[i] = D_80146054;
        } else if (D_80146168[i] == 3) {
            D_80146168[i] = D_80146058;
        }
    }
    menu_check(0, ((DisplayActor44 *)D_8011ECD0)[0].x, ((DisplayActor44 *)D_8011ECD0)[0].y, D_80146168);
    func_8004FC10(0);
    if ((g->pad_pressed & 0xF000) && ((g->menu_mode & 3) == 1 || (g->menu_mode & 3) == 3)) {
        func_80044750(0x502);
    }
    if (g->pad_pressed & 0x200020) {
        if (check_end_k() != 0) {
            if (g->menu_items[0].hover != 0) {
                D_80146158 = 0;
            } else if (g->menu_items[2].hover != 0) {
                D_80146158 = 1;
            } else if (g->menu_items[4].hover != 0) {
                D_80146158 = 2;
            } else if (g->menu_items[6].hover != 0) {
                D_80146158 = 3;
            } else {
                return 0;
            }
            ((DisplayActor44 *)D_8011ECD0)[0].draw_flags &= 0xFF7F;
            D_80122D30 = 0;
            k_reset(1);
            ((DisplayActor44 *)D_8011ECD0)[97].draw_flags |= 0x80;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 =
                D_80146168[D_80146158]
                + (g->year == 0x60) * 0xB
                + (g->year == 0x61) * 0x17
                - (D_80146168[D_80146158] == 0xB && g->year == 0x5F)
                + 1;
            /* FAKE: *(u8 *)& keeps D_80146158 unwidened ($a2); !(!=) and * 0 keep 0x5F in $a1 without CSE. T-24137. */
            ((DisplayActor44 *)D_8011ECD0)[97].x = *(u8 *)&D_80146158 * 0x4E - *(u8 *)&D_80146158 - 0x75;
            ((DisplayActor44 *)D_8011ECD0)[97].y = -0x3C;
            ((DisplayActor44 *)D_8011ECD0)[97].scale_y = 0;
            D_80145F2C += D_80146168[D_80146158] * 3 + g->year - 0x5F;
            D_80146160 = 1;
            if ((!(g->year != 0x5F) && D_80146168[D_80146158] == 0 && g->param_art.value >= 0x64)
                || ((D_80146168[D_80146158] == 0 & g->year == 0x5F) && g->clubs[5].exp >= 0x64)
                || ((D_80146168[D_80146158] == 1 & (g->year + (g->year * 0)) == 0x5F) && g->param_liberal_arts.value >= 0x46)
                || ((D_80146168[D_80146158] == 1 & g->year == 0x61) && g->param_liberal_arts.value >= 0x64)
                || ((D_80146168[D_80146158] == 2 & g->year == 0x60) && g->param_liberal_arts.value >= 0x64)) {
                D_80145F30 = 1;
            }
            func_8004284C();
        }
    }
}

void func_8013FFA8(void) {
    bg_read_sub2(0x41E4);
    func_8004284C();
}

void func_8013FFD0(void) {
    s16 i;

    for (i = 0; i < 6; i++) {
        ((DisplayActor44 *)D_8011ECD0)[i + 96].draw_flags = 0;
    }
    D_80146160 = 0;
    D_80146164 = 0;
    func_8004284C();
}

s32 func_80140030(void) {
    D_80122D2C = 0;
    if (D_80145FE4 == 0) {
        D_800E6280.current_girl = 0;
        D_80122CD4 = 0;
        D_80122CD0 = 7;
        func_80042908(6);
        D_800E6280.event_flags[6] += 1;
        return 0;
    }
    if (D_80145FE4 == 7) {
        D_800E6280.current_girl = 7;
        D_80122CD4 = 7;
        D_80122CD0 = 7;
        func_80042908(7);
        D_800E6280.event_flags[52] += 1;
        return 0;
    }
    if (D_80145FE4 == 9) {
        D_800E6280.current_girl = 9;
        D_80122CD4 = 9;
        D_80122CD0 = 7;
        func_80042908(8);
        D_800E6280.event_flags[66] += 1;
        return 0;
    }
    func_8004284C();
}

void func_80140144(void) {
    if (D_80146168[D_80146158] == 0xB) {
        func_80044750(0x200);
    } else {
        func_80044750(0x201);
        D_800E6280.girls[D_80146168[D_80146158]].affection += 1;
    }
    check_para_limit();
    func_8004284C();
}

void func_801401DC(void) {
    s16 v;

    v = D_801206B0 + 0x200;
    D_801206B0 = v;
    if (v >= 0x1000) {
        func_8004284C();
    }
}

void func_80140220(void) {
    cd_read_sectors(0x4D, 0x801B0000, 0xBDAC);
    func_8004284C();
}

void func_80140250(void) {
    cd_read_sectors(0xE, 0x801D7000, 0xBDF9);
    func_8004284C();
}

void func_80140284(void) {
    D_80145F30 = 0;
    if (D_80146168[D_80146158] == 0xB) {
        D_80145F2C = 0x27;
        D_800E6280.param_stress.value += 0xA;
    } else {
        D_80145F2C = 0x26;
        D_800E6280.param_stress.value -= 0xA;
    }
    check_para_limit();
    func_8004284C();
}

/* Fall-through return preserves the sibling callback schedule (T-24101). */
s32 func_80140314(void) {
    s32 unused; /* FAKE: unused local pads stack frame to 0xA0 bytes. T-24101. */
    s16 dy;
    s16 dx;
    ShoColors6x2 colors;
    ShoBytes3x12 sel;

    colors = D_801461D8;
    sel = D_80146208;
    dy = 0x1C - (((DisplayActor44 *)D_8011ECD0)[97].scale_y / 512) * 4;
    dx = 1;
    if (((DisplayActor44 *)D_8011ECD0)[97].scale_y == 0x1000) {
        dy = 0;
        dx = 0;
    }
    func_80140510(
        ((DisplayActor44 *)D_8011ECD0)[97].x - dx - 0x1C,
        ((DisplayActor44 *)D_8011ECD0)[97].y + dy - 0x1C,
        ((DisplayActor44 *)D_8011ECD0)[97].x - 1,
        ((DisplayActor44 *)D_8011ECD0)[97].y + dy - 0x1C,
        ((DisplayActor44 *)D_8011ECD0)[97].x - dx - 0x1C,
        ((DisplayActor44 *)D_8011ECD0)[97].y - 1,
        colors.v[sel.v[(D_800E6280.year - 0x5F) * 12 + D_80146168[D_80146158]]][0],
        colors.v[sel.v[(D_800E6280.year - 0x5F) * 12 + D_80146168[D_80146158]]][1],
        9, 0);
    func_80140510(
        ((DisplayActor44 *)D_8011ECD0)[97].x + dx + 0x1C,
        ((DisplayActor44 *)D_8011ECD0)[97].y - dy + 0x1C,
        ((DisplayActor44 *)D_8011ECD0)[97].x + dx + 0x1C,
        ((DisplayActor44 *)D_8011ECD0)[97].y,
        ((DisplayActor44 *)D_8011ECD0)[97].x,
        ((DisplayActor44 *)D_8011ECD0)[97].y - dy + 0x1C,
        colors.v[sel.v[(D_800E6280.year - 0x5F) * 12 + D_80146168[D_80146158]]][0],
        colors.v[sel.v[(D_800E6280.year - 0x5F) * 12 + D_80146168[D_80146158]]][1],
        9, 0);
    dtd_on(0xB);
}

/* Fall-through int return follows the matched DATE builder (T-24026). */
s32 func_80140510(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 c0, u32 c1, s32 z, s32 trans) {
    GameState *g;
    POLY_G3 *poly;

    g = &D_800E6280;
    poly = GetWorkBase(0x1C, D_8011ECA0);
    func_8009F004(poly);
    SetSemiTrans(poly, trans);
    /* FAKE: explicit colour masks and comma lines retain the matched sibling register schedule (T-20600, T-24026). */
    poly->r0 = (c0 >> 16) & 0xFF, poly->g0 = (c0 >> 8) & 0xFF, poly->b0 = c0 & 0xFF;
    poly->r1 = (c1 >> 16) & 0xFF, poly->g1 = (c1 >> 8) & 0xFF, poly->b1 = c1 & 0xFF;
    poly->r2 = (c1 >> 16) & 0xFF, poly->g2 = (c1 >> 8) & 0xFF, poly->b2 = c1 & 0xFF;
    poly->x0 = g->screen_center_x[D_8011ECA0] + x0;
    poly->y0 = g->screen_center_y[D_8011ECA0] + y0;
    poly->x1 = g->screen_center_x[D_8011ECA0] + x1;
    poly->y1 = g->screen_center_y[D_8011ECA0] + y1;
    poly->x2 = g->screen_center_x[D_8011ECA0] + x2;
    poly->y2 = g->screen_center_y[D_8011ECA0] + y2;
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 10) + z * 4], poly);
}

void func_80140698(void) {
    s16 i;
    u8 *p;

    for (i = 0; i < 3; i++) {
        p = D_8011ECD0 + i * 0x44;
        if (*(s16 *)(p + 0x1A20) == 0xE && *(s16 *)(p + 0x1A10) == 9) {
            *(s16 *)(p + 0x1A1E) = rand() % 3;
        }
    }
}

void func_80140780(void) {
    D_80146230 = 0x801B21E0;
    D_80146234 = 0x801B21E4;
    D_80146238 = 0x801B2228;
    D_80146240 = 0x801B0000;
}

void func_801407C0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl31 tbl;
    u32 j;

    tbl = D_80146244;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
    j = D_800E6280.step;
    if (normal_date_bg_fadein == tbl.f[j] || normal_date_bg_fadeout == tbl.f[j]) {
        D_80120653 |= 0x80;
        D_80120657 = D_800B593C;
    }
}

void func_8014088C(void) {
    func_80044750(0xBF);
    func_80044890(1, 0xBF98, 0xBF79, D_800B36AC, D_800B36EC, D_800B372C);
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    func_8004284C();
}

void func_80140908(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    if ((u32) D_800E6280.step_timer.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.step - 1) & 0xFF);
    }
}

void func_80140990(void) {
    func_80044750(0xBF);
    func_80044890(1, 0xC621, 0xC613, 0xD93B, 0xD8F0, 0xD8D0);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_801409F8(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.step_timer.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.step - 1) & 0xFF);
    }
}

void func_80140A78(void) {
    cd_read_sectors(0xD, 0x801B0000, 0xBCBB);
    func_80140780();
    func_8004284C();
}

void func_80140AB0(void) {
    func_80044750(0x203);
    func_8004284C();
}

s32 func_80140AD8(void) {
    if (D_800E6280.girls[9].flags.f.met == 0 && (D_800E6280.girls[9].tokimeki >= 0x14) && (D_800E6280.frame_count & 1)) {
        D_80146050 = 1;
        D_800E6280.girls[9].flags.f.met = 1;
        D_800E6280.girls[9].affection = 0x32;
        D_80145F2C = 5;
        D_80145F20 = D_80145AF0;
        D_80145F24 = D_80145B24;
        D_80145F28 = D_80145B58;
        D_800E6280.current_girl = 9;
        func_80085B3C(4, 1);
        func_800847B8(D_800E6280.current_girl);
        func_8004284C();
    } else {
        D_800E6280.step += 0xB;
    }
}

void func_80140BD8(void) {
    func_80065F34(0);
    k_reset(1);
    func_80140780();
    load_palette(D_80146240, 0x11, 1, 2, 0);
    func_80048F64(0x60);
    D_80120651 = 1;
    D_80120652 = 0;
    D_80120688 = 0x01000000;
    D_80120653 = 4;
    D_8012065C = (u8 *)D_80146234;
    D_80120660 = (u8 *)D_80146238;
    D_80120684 = (u8 *)D_80146230;
    D_80120664 = D_8014623C;
    D_80120666 = 0;
    D_80120668 = 0;
    D_80120656 = 1;
    D_80120693 = 0x11;
    D_80120676 = -0xA0;
    D_8012067A = -0x78;
    D_80120657 = 0;
    D_80120655 = 0;
    D_80120654 = 0;
    D_80145F2C = 0x13;
    D_80145F20 = D_80145AFC;
    D_80145F24 = D_80145B30;
    D_80145F28 = D_80145B64;
    func_8004284C();
}

void func_80140D30(void) {
    bg_read_sub2(0x41DC);
    func_8004284C();
}

void func_80140D58(void) {
    D_80120652 = 9;
    func_8004284C();
}

void func_80140D80(void) {
    D_800E6280.girls[D_800E6280.current_girl].affection = 0x28;
    func_8004284C();
}

s32 func_80140DC0(void) {
    if (D_800E6280.step == 0) {
        D_800E6280.girls[D_800E6280.current_girl].affection = 0x46;
        func_800847B8(D_800E6280.current_girl);
    }
    switch (D_800E6280.current_girl) {
    case 0:
        func_80046500();
        break;
    case 1:
        func_80140F54();
        break;
    case 2:
        func_8014102C();
        break;
    case 3:
        func_80141180();
        break;
    case 4:
        func_80141224();
        break;
    case 5:
        func_80141358();
        break;
    case 6:
        func_801413F4();
        break;
    case 7:
        func_801414E0();
        break;
    case 8:
        func_801415C8();
        break;
    case 9:
        func_80141678();
        break;
    case 10:
        func_80046500();
        break;
    }
    ((DisplayActor44 *)D_8011ECD0)[96].brightness = D_800B593C;
    if (((DisplayActor44 *)D_8011ECD0)[96].unk_18 == 0 && ((DisplayActor44 *)D_8011ECD0)[96].unk_08 == 0) {
        if (++((DisplayActor44 *)D_8011ECD0)[96].sequence_slot >= 3) {
            ((DisplayActor44 *)D_8011ECD0)[96].sequence_slot = 0;
        }
    }
}

void func_80140F54(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_801462C0;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

void func_80140FDC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

void func_80141004(void) {
    func_80044750(0x506);
    func_8004284C();
}

void func_8014102C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_80146304;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

void func_801410B4(void) {
    D_800E6280.girls[2].flags.f.b2 = 1;
    D_80145F2C = 3;
    func_8004284C();
}

void func_801410F0(void) {
    if (((u32) D_800E6280.girls[2].flags.b[2] >> 4) == 3) {
        D_80145F2C += 1;
    }
    func_8004284C();
}

void func_80141138(void) {
    if (((u32) D_800E6280.girls[2].flags.b[2] >> 4) != 3) {
        D_80145F2C += 1;
    }
    func_8004284C();
}

void func_80141180(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl10 tbl;

    tbl = D_8014633C;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

void func_801411FC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

void func_80141224(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_80146364;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

void func_801412AC(void) {
    func_80044750(0x501);
    func_8004284C();
}

void func_801412D4(void) {
    if (((u32) D_800E6280.girls[4].flags.b[2] >> 4) == 7) {
        D_80145F2C += 3;
    }
    func_8004284C();
}

void func_8014131C(void) {
    D_800E6280.girls[4].flags.f.b2 = 1;
    D_80145F2C = 3;
    func_8004284C();
}

void func_80141358(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl12 tbl;

    tbl = D_8014639C;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

void func_801413CC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

void func_801413F4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_801463CC;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

void func_8014147C(void) {
    D_800E6280.girls[6].flags.f.b2 = 1;
    D_80145F2C = 4;
    func_8004284C();
}

void func_801414B8(void) {
    func_80044750(0x505);
    func_8004284C();
}

void func_801414E0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_80146410;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

void func_80141568(void) {
    if (check_end_k() != 0) {
        k_reset(1);
        func_8004284C();
    }
}

void func_801415A0(void) {
    D_80145F2C = 3;
    func_8004284C();
}

void func_801415C8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl11 tbl;

    tbl = D_80146454;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

void func_80141650(void) {
    D_80145F2C = 3;
    func_8004284C();
}

void func_80141678(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl12 tbl;

    tbl = D_80146480;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

void func_801416EC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

void func_80141714(void) {
    func_80062CD0(0x56C0);
    func_8004284C();
}

void func_8014173C(void) {
    func_80042908(D_800E6280.saved_stage);
    func_80042940(D_800E6280.saved_step);
}
