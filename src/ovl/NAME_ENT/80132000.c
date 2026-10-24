#define MAIN_API_OVERRIDE_strncpy /* FAKE: implicit int view, no prototype: func_80133510 calls it without the length, and the unused $v0 result keeps the address in func_80134B94 off $v0 (main_api.h: void, three parameters; T-20810, T-21520) */
#include "common.h"
#include "ovl/NAME_ENT.h"
#include "game.h"

s32 strncpy();

/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC30);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC38);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC3C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC40);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC44);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC48);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC4C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC50);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC54);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC58);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC5C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC60);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC64);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC70);
u8 D_8014CC74[8] = { 0 };
u8 D_8014CC7C[8] = { 0 };
u8 D_8014CC84[12] = { 0 };
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC90);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC94);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC98);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CC9C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CCA0);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CCA4);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CCAC);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CCB0);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CCB4);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CCB8);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CCBC);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CCC0);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CCC4);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CCC8);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CCCC);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CCD0);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CCE4);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD04);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD10);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD28);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD2C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD30);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD34);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD38);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD3C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD40);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD44);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD48);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD4C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD50);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD54);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD58);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD5C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD60);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD64);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD68);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD6C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD70);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD74);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD78);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD7C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD80);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD84);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD88);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD8C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD90);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD94);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD98);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CD9C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CDA0);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CDA4);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CDA8);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CDAC);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CDB0);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CE64);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CE68);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CE6C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CEE4);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014CF5C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014D010);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014D014);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data", D_8014D028);
u8 D_8014D03C[16] = {
    1, 4, 3, 2,
    3, 1, 2, 2,
    2, 3, 3, 4,
    3, 2, 3, 1,
};
u8 D_8014D04C[20] = {
    0x2D, 0x0A, 0x0A,
    0x32, 0x14, 0x00,
    0x32, 0x19, 0x00,
    0x37, 0x19, 0x00,
    0, 0, 0, 0, 0, 0, 0, 0,
};

void func_80132000(s32 arg0) {
    s32 rem;

    rem = arg0 % 15;
    strncpy(D_8014CC74, D_8014CE6C + rem * 8, 8);
    strncpy(D_8014CC7C, D_8014CEE4 + rem * 8, 8);
    strncpy(D_8014CC84, D_8014CF5C + rem * 0xC, 0xC);
}

void func_8013209C(void) {
    switch (game_state.mode_stage) {
    case 0:
        func_80132134();
        break;
    case 1:
        func_80132E80();
        break;
    case 2:
        func_80138488();
        break;
    }
    dec_bg_draw(2);
    func_8004B19C(bg_layers, 0, 0x50);
    func_8004B19C(D_801220F4, 0, 0x5A);
}

void func_80132134(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        func_80132198();
        return;
    case 1:
        func_80132320();
        return;
    }
}
/* FAKE: an empty statement after initialization preserves base-load/store scheduling. T-9910. */
void func_80132198(void)
{
  if (game_state.substep == 0)
  {
    text_system_reset();
    text_set_advance(0x10);
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_upper();
    cursor_actor_init(0);
    cursor_show(0);
    back_clear_switch(1);
    game_state.clear_color_g = 0;
    game_state.clear_color_r = 0;
    game_state.clear_color_b = 0;
    tpage_buf_clear();
    func_80041168(0);
    game_state.tile_layer_enabled = 0;
    dec_bg_reset();
    dec_bg_show_switch(2);
    cd_read_sectors_nowait(0xAU, 0x80162000, 0xAA18);
    game_state.cursor_speed = 3;
    dec_bg_brightness = 0;
    game_state.substep = 1;
 ;
  }
  else
    if (game_state.substep == 1)
  {
    if (cd_read_sync_status() & 1)
    {
      func_80053650(0x80167000, 0x80188000, 0x80162000);
      func_800536AC(0x140, 0x100, 0x140, 0xF0);
      cd_read_sectors(0x36U, 0x80190000, 0xA9E2);
      game_state.substep += 1;
    }
  }
  else
    if (game_state.substep == 2)
  {
    if (cd_read_sync_status() & 1)
    {
      cd_read_sectors(6U, 0x801E0000, 0x9DBF);
      mode_step_next();
    }
  }
}

/* FAKE: the five & -1 identities retain the masked byte in the original
 * register. Removing any one loses the match; see T-20013. */
void func_80132320(void) {
    s32 i;
    s32 val;
    u8 **p;

    if (game_state.substep == 0) {
        if (cd_read_sync_status() & 1) {
            p = D_8014CE68;
            for (i = 0; i < 50; i++) {
                p[i] += 0x801E0000;
            }
            func_80132A38();
            D_801220E1 = 0;
            D_801220E0 = 0;
            game_state.substep += 1;
            D_801220E2 = 0;
            game_state.screen_brightness = 0;
        }
    } else {
        val = (dec_bg_brightness + 4) & 0xFF;
        D_801220E1 = (val & -1);
        D_801220E0 = (val & -1);
        dec_bg_brightness = (val & -1);
        D_801220E2 = (val & -1);
        game_state.screen_brightness = (val & -1);
        if ((u32)val >= 0x80) {
            func_80057640(0xF0);
            func_801324AC();
            bg_layers = 0x80000000;
            D_801220D4 = -0x18;
            cursor_show(1);
            back_clear_switch(0);
            mode_stage_next();
        }
    }
    func_8004B19C(dec_bg_brightness, 0, 0);
    func_8004B19C(D_801220E0, 0, 0x10);
}

void func_801324AC(void) {
    s32 index;
    Actor *actor;

    func_80138F00();
    for (index = 0; index < 0x20;) {
        actor_clear(index + 0x60);
        actor = (Actor *)actor_pool + index;
        actor[96].unk_01 = 2;
        actor[96].flags = 0x40;
        actor[96].quad_flags = 0;
        actor[96].draw_flags = 0x84;
        actor[96].frame_table = (u8 *)game_state.ovl_frame_table;
        actor[96].sequence_table = (u8 *)game_state.ovl_sequence_table;
        actor[96].tpage_table = (u8 *)game_state.ovl_tpage_table;
        /* FAKE: re-index here for the original induction-update order. T-10890. */
        index++;
        actor = (Actor *)actor_pool + index;
        actor[95].unk_04 = 8;
        actor[95].unk_05 = 0x81;
        actor[95].unk_18 = 0;
        actor[95].unk_43 = 0x10;
        actor[95].x = 0x190;
        actor[95].y = 0;
    }
    ((Actor *)actor_pool)[96].unk_18 = 3;
    ((Actor *)actor_pool)[96].x = -0x70;
    ((Actor *)actor_pool)[96].y = -0x3C;
    ((Actor *)actor_pool)[97].unk_18 = 0;
    ((Actor *)actor_pool)[97].x = 0;
    ((Actor *)actor_pool)[97].y = 0;
    ((Actor *)actor_pool)[98].unk_18 = 0xD;
    ((Actor *)actor_pool)[98].x = -0x60;
    ((Actor *)actor_pool)[98].y = -0x4A;
    ((Actor *)actor_pool)[99].unk_18 = 0xD;
    ((Actor *)actor_pool)[99].x = -0x50;
    ((Actor *)actor_pool)[99].y = -0x4A;
    ((Actor *)actor_pool)[100].unk_18 = 0xD;
    ((Actor *)actor_pool)[100].x = -0x40;
    ((Actor *)actor_pool)[100].y = -0x4A;
    ((Actor *)actor_pool)[101].unk_18 = 0xD;
    ((Actor *)actor_pool)[101].x = -0x28;
    ((Actor *)actor_pool)[101].y = -0x4A;
    ((Actor *)actor_pool)[102].unk_18 = 0xD;
    ((Actor *)actor_pool)[102].x = -0x18;
    ((Actor *)actor_pool)[102].y = -0x4A;
    ((Actor *)actor_pool)[103].unk_18 = 0xD;
    ((Actor *)actor_pool)[103].x = -8;
    ((Actor *)actor_pool)[103].y = -0x4A;
    ((Actor *)actor_pool)[104].unk_18 = 0xD;
    ((Actor *)actor_pool)[104].x = 0x50;
    ((Actor *)actor_pool)[104].y = -0x4A;
    ((Actor *)actor_pool)[105].unk_18 = 0xD;
    ((Actor *)actor_pool)[105].x = 0x60;
    ((Actor *)actor_pool)[105].y = -0x4A;
    ((Actor *)actor_pool)[106].unk_18 = 0xD;
    ((Actor *)actor_pool)[106].x = 0x70;
    ((Actor *)actor_pool)[106].y = -0x4A;
    ((Actor *)actor_pool)[107].unk_18 = 0xD;
    ((Actor *)actor_pool)[107].x = 0x80;
    ((Actor *)actor_pool)[107].y = -0x4A;
    ((Actor *)actor_pool)[108].unk_18 = 0xD;
    ((Actor *)actor_pool)[108].x = 0x90;
    ((Actor *)actor_pool)[108].y = -0x4A;
    ((Actor *)actor_pool)[109].unk_18 = 0xF;
    ((Actor *)actor_pool)[109].x = -0x8F;
    ((Actor *)actor_pool)[109].y = -0x28;
    ((Actor *)actor_pool)[110].unk_18 = 0x11;
    ((Actor *)actor_pool)[110].x = -0x8F;
    ((Actor *)actor_pool)[110].y = -8;
    ((Actor *)actor_pool)[111].unk_18 = 0x13;
    ((Actor *)actor_pool)[111].x = -0x8F;
    ((Actor *)actor_pool)[111].y = 0x18;
    ((Actor *)actor_pool)[112].unk_18 = 0x15;
    ((Actor *)actor_pool)[112].x = -0x8F;
    ((Actor *)actor_pool)[112].y = 0x38;
    ((Actor *)actor_pool)[113].unk_18 = 0x17;
    ((Actor *)actor_pool)[113].x = -0x8F;
    ((Actor *)actor_pool)[113].y = 0x58;
    ((Actor *)actor_pool)[114].unk_18 = 7;
    ((Actor *)actor_pool)[114].x = -0x8F;
    ((Actor *)actor_pool)[114].y = -0x28;
    ((Actor *)actor_pool)[115].unk_18 = 7;
    ((Actor *)actor_pool)[115].x = -0x8F;
    ((Actor *)actor_pool)[115].y = -8;
    ((Actor *)actor_pool)[116].unk_18 = 7;
    ((Actor *)actor_pool)[116].x = -0x8F;
    ((Actor *)actor_pool)[116].y = 0x18;
    ((Actor *)actor_pool)[117].unk_18 = 7;
    ((Actor *)actor_pool)[117].x = -0x8F;
    ((Actor *)actor_pool)[117].y = 0x38;
    ((Actor *)actor_pool)[118].unk_18 = 7;
    ((Actor *)actor_pool)[118].x = -0x8F;
    ((Actor *)actor_pool)[118].y = 0x58;
    /* FAKE: one line preserves as1 scheduling of the unrolled flag stores. T-10890. */
    index = 0; do { actor = (Actor *)actor_pool + index; actor[114].draw_flags &= 0x7F; index++; } while (index < 5);
    ((Actor *)actor_pool)[119].unk_18 = 9;
    ((Actor *)actor_pool)[119].x = -0x8F;
    ((Actor *)actor_pool)[119].y = -0x28;
    ((Actor *)actor_pool)[119].unk_01 = 1;
    ((Actor *)actor_pool)[120].unk_18 = 4;
    ((Actor *)actor_pool)[120].x = 0x100;
    ((Actor *)actor_pool)[120].y = 0x100;
    ((Actor *)actor_pool)[120].unk_01 = 3;
    ((Actor *)actor_pool)[121].unk_18 = 0x19;
    ((Actor *)actor_pool)[121].x = 0x88;
    ((Actor *)actor_pool)[121].y = -0x28;
    ((Actor *)actor_pool)[121].draw_flags = 0;
    ((Actor *)actor_pool)[122].unk_18 = 0x1B;
    ((Actor *)actor_pool)[122].x = 0x88;
    ((Actor *)actor_pool)[122].y = 0x18;
    ((Actor *)actor_pool)[122].draw_flags = 0;
    ((Actor *)actor_pool)[123].unk_18 = 8;
    ((Actor *)actor_pool)[123].x = 0x88;
    ((Actor *)actor_pool)[123].y = -0x28;
    ((Actor *)actor_pool)[123].draw_flags = 0;
    ((Actor *)actor_pool)[124].unk_18 = 8;
    ((Actor *)actor_pool)[124].x = 0x88;
    ((Actor *)actor_pool)[124].y = 0x18;
    ((Actor *)actor_pool)[124].draw_flags = 0;
}

void func_80132A38(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused 8-byte local reproduces the original frame (buf at 0x4C); real source unknown. T-11650 */
    s32 buf[6];

    func_80138F00();
    /* FAKE: loop body on the for line gives the original as1 schedule. T-11650 */
    for (i = 0; i < 6; i++) { buf[i] = (u32)game_state.ovl_image_buffer + i * 0x4000; }
    func_8004A1A0(buf, 0, 6, 3, 2, 0x1F8, -0xA0, -0x78, 8, 0xD);
    bg_layers |= 0x40000000;
    game_state.tpage_buf[search_tpage_multi(*(s32 *)game_state.ovl_tpage_table, 2)] = *(s32 *)game_state.ovl_tpage_table;
    load_palette(game_state.ovl_palette_b, 0x18, 1, 2, 0);
    load_palette(game_state.ovl_palette_a, 0x10, 1, 2, 0);
}

void func_80132B78(s32 arg0) {
    s32 i;
    s32 x;
    s32 y;
    s32 pad; /* FAKE: unused; its stack home gives the original 72-byte frame. T-11650 */

    if (arg0 == 0 && D_8014CC70 >= 3) {
        D_8014CC70 = 0xC;
    }
    if (arg0 == 1 && D_8014CC70 >= 6) {
        D_8014CC70 = 0xC;
    }
    if (arg0 == 2 && D_8014CC70 >= 11) {
        D_8014CC70 = 0xC;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        x = ((Actor *)actor_pool)[0].x;
        y = ((Actor *)actor_pool)[0].y;
        if (arg0 == 0) {
            for (i = 0; i < 3; i++) {
                if (x < i * 16 - 0x58 && i * 16 - 0x68 < x
                    && y < -0x46 && y >= -0x55) {
                    D_8014CC70 = i;
                }
            }
        } else if (arg0 == 1) {
            for (i = 0; i < 3; i++) {
                if (x < i * 16 - 0x20 && i * 16 - 0x30 < x
                    && y < -0x46 && y >= -0x55) {
                    D_8014CC70 = i + 3;
                }
            }
        } else if (arg0 == 2) {
            for (i = 0; i < 5; i++) {
                if (x < i * 16 + 0x58 && i * 16 < x - 0x48
                    && y < -0x46 && y >= -0x55) {
                    D_8014CC70 = i + 6;
                }
            }
        }
    }
    for (i = 0; i < 11; i++) {
        if (i == D_8014CC70) {
            if (((Actor *)actor_pool)[98 + i].unk_18 == 0xD) {
                ((Actor *)actor_pool)[98 + i].unk_18 = 0;
            }
            func_8013664C(i + 0x62);
        } else {
            ((Actor *)actor_pool)[98 + i].flags = 0x40;
            ((Actor *)actor_pool)[98 + i].unk_18 = 0xD;
        }
    }
}

void func_80132E80(void) {
    if (game_state.mode_step != 0) {
        if (game_state.mode_step < 6) {
            func_80135434(0);
            func_80134EB8();
        } else if (game_state.mode_step < 8) {
            func_8013621C(0);
            func_80136808(0);
        } else {
            func_80138268(0);
            func_80138330();
        }
    }
    switch (game_state.mode_step) {
    case 0:
        func_80135168();
        break;
    case 1:
        func_801357F4();
        break;
    case 2:
        func_80135A98();
        break;
    case 3:
        func_80135BB4();
        break;
    case 4:
        func_80135E58();
        break;
    case 5:
        func_80135F74();
        break;
    case 6:
        func_801362D0();
        break;
    case 7:
        func_80137708();
        break;
    case 8:
        func_80137FC4();
        break;
    case 9:
        func_80138338();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B140);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B150);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B15C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B168);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B174);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B180);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B18C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B198);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1A4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1AC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1B8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1C0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1CC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1D8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1E4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1F0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B1FC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B208);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B214);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B220);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B22C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B238);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B244);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B250);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B25C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B268);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B270);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B27C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B284);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B290);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B29C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2A8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2B4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2C0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2CC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2D8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2E4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2F0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B2FC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B308);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B314);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B320);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B32C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B338);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B344);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B350);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B35C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B368);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B374);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B380);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B38C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B398);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3A4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3B0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3BC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3C8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3D0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3D8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3E0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3E8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B3F0);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B400);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B410);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B420);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B430);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B440);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B448);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B450);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B458);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B460);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B468);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B470);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B478);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B480);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B484);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B48C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B494);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B49C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B4A4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80132000.rodata", D_8014B4AC);

void func_80132FD4(void) {
    set_kanji_string(-0x78, 0x38, 0xA, D_8014CD28, 0);
    set_kanji_string(-0x18, 0x38, 0xA, D_8014CD2C, 0);
    set_kanji_string(0x48, 0x38, 0xA, D_8014CD30, 0);
    set_kanji_string(-0x78, 0x48, 0xA, D_8014CD34, 0);
    set_kanji_string(-0x18, 0x48, 0xA, D_8014CD38, 0);
    set_kanji_string(0x48, 0x48, 0xA, D_8014CD3C, 0);
    set_kanji_string(-0x78, 0x58, 0xA, D_8014CD40, 0);
    set_kanji_string(-0x18, 0x58, 0xA, "やゆよ　わ", 0);
    set_kanji_string(0x48, 0x58, 0xA, D_8014CD48, 0);
}

void func_801330E8(s32 arg0) {
    text_system_reset();
    set_kanji_string(-0x98, -0x68, 0, arg0, 0);
    set_kanji_string(-0x78, -0x30, 0xA, D_8014CD28, 0);
    set_kanji_string(-0x78, -0x20, 0xA, D_8014CD34, 0);
    set_kanji_string(-0x78, -0x10, 0xA, D_8014CD40, 0);
    set_kanji_string(-0x78, 0, 0xA, D_8014CD4C, 0);
    set_kanji_string(-0x78, 0x10, 0xA, D_8014CD58, 0);
    set_kanji_string(-0x78, 0x20, 0xA, D_8014CD64, 0);
    set_kanji_string(-0x18, -0x30, 0xA, D_8014CD2C, 0);
    set_kanji_string(-0x18, -0x20, 0xA, D_8014CD38, 0);
    set_kanji_string(-0x18, -0x10, 0xA, D_8014CD44, 0);
    set_kanji_string(-0x18, 0, 0xA, D_8014CD50, 0);
    set_kanji_string(-0x18, 0x10, 0xA, D_8014CD5C, 0);
    set_kanji_string(-0x18, 0x20, 0xA, D_8014CD68, 0);
    set_kanji_string(0x48, -0x30, 0xA, D_8014CD30, 0);
    set_kanji_string(0x48, -0x20, 0xA, D_8014CD3C, 0);
    set_kanji_string(0x48, -0x10, 0xA, D_8014CD48, 0);
    set_kanji_string(0x48, 0, 0xA, D_8014CD54, 0);
    set_kanji_string(0x48, 0x10, 0xA, D_8014CD60, 0);
}

void func_801332FC(s32 arg0) {
    text_system_reset();
    set_kanji_string(-0x98, -0x68, 0xA, arg0, 0);
    set_kanji_string(-0x78, -0x30, 0xA, D_8014CD6C, 0);
    set_kanji_string(-0x78, -0x20, 0xA, D_8014CD78, 0);
    set_kanji_string(-0x78, -0x10, 0xA, D_8014CD84, 0);
    set_kanji_string(-0x78, 0, 0xA, D_8014CD90, 0);
    set_kanji_string(-0x78, 0x10, 0xA, D_8014CD9C, 0);
    set_kanji_string(-0x78, 0x20, 0xA, D_8014CDA8, 0);
    set_kanji_string(-0x18, -0x30, 0xA, D_8014CD70, 0);
    set_kanji_string(-0x18, -0x20, 0xA, D_8014CD7C, 0);
    set_kanji_string(-0x18, -0x10, 0xA, D_8014CD88, 0);
    set_kanji_string(-0x18, 0, 0xA, D_8014CD94, 0);
    set_kanji_string(-0x18, 0x10, 0xA, D_8014CDA0, 0);
    set_kanji_string(-0x18, 0x20, 0xA, D_8014CDAC, 0);
    set_kanji_string(0x48, -0x30, 0xA, D_8014CD74, 0);
    set_kanji_string(0x48, -0x20, 0xA, D_8014CD80, 0);
    set_kanji_string(0x48, -0x10, 0xA, D_8014CD8C, 0);
    set_kanji_string(0x48, 0, 0xA, D_8014CD98, 0);
    set_kanji_string(0x48, 0x10, 0xA, D_8014CDA4, 0);
}

void func_80133510(u8 *arg0) {
    s32 i;
    u8 *p;
    u8 *text;
    u8 line0[0x24];
    u8 line1[0x24];
    u8 line2[0x24];
    u8 line3[0x24];
    u8 line4[0x24];
    u8 line5[0x24];

    for (i = 0; i < 0x23; i++) {
        line0[i] = 0;
        line1[i] = 0;
        line2[i] = 0;
        line3[i] = 0;
        line4[i] = 0;
        line5[i] = 0;
    }
    text_system_reset();
    set_kanji_string(-0x98, -0x68, 0xA, arg0, 0);
    if (D_8014CE64[38] < D_8014CCAC * 16 + 0x50) {
        D_8014CCAC = (D_8014CE64[38] >> 4) - 5;
        if (D_8014CCAC < 0) {
            D_8014CCAC = 0;
        }
    }
    D_8014CCB0 = strlen(D_8014CE68[38]);
    text = D_8014CE68[38];
    text += D_8014CCAC * 32;
    if (strlen(text) < 0x20) {
        strcpy(line0, text);
    } else {
        strncpy(line0, text, 0x20);
        p = text + 0x20;
        if (strlen(p) < 0x20) {
            strcpy(line1, p);
        } else {
            strncpy(line1, p, 0x20);
            p = text + 0x40;
            if (strlen(p) < 0x20) {
                strcpy(line2, p);
            } else {
                strncpy(line2, p, 0x20);
                p = text + 0x60;
                if (strlen(p) < 0x20) {
                    strcpy(line3, p);
                } else {
                    strncpy(line3, p, 0x20);
                    p = text + 0x80;
                    if (strlen(p) < 0x20) {
                        strcpy(line4, p);
                    } else {
                        strncpy(line4, p);
                        p = text + 0xA0;
                        if (strlen(p) < 0x20) {
                            strcpy(line5, p);
                        } else {
                            strncpy(line5, p);
                        }
                    }
                }
            }
        }
    }
    if (line0[0] != 0) {
        set_kanji_string(-0x78, -0x30, 0xA, line0, 0);
    }
    if (line1[0] != 0) {
        set_kanji_string(-0x78, -0x20, 0xA, line1, 0);
    }
    if (line2[0] != 0) {
        set_kanji_string(-0x78, -0x10, 0xA, line2, 0);
    }
    if (line3[0] != 0) {
        set_kanji_string(-0x78, 0, 0xA, line3, 0);
    }
    if (line4[0] != 0) {
        set_kanji_string(-0x78, 0x10, 0xA, line4, 0);
    }
    if (line5[0] != 0) {
        set_kanji_string(-0x78, 0x20, 0xA, line5, 0);
    }
}

void func_801338DC(s32 arg0) {
    s32 i;
    s32 pad[2]; /* FAKE: unused; its home gives the original 0x110-byte frame. T-11650 */
    u8 *text;
    u8 line0[0x24];
    u8 line1[0x24];
    u8 line2[0x24];
    u8 line3[0x24];
    u8 line4[0x24];

    switch (arg0) {
    case 0:
        if (game_state.mode_step == 1) {
            D_8014D010 = D_8014CCE4;
        }
        if (game_state.mode_step == 3) {
            D_8014D010 = D_8014CD04;
        }
        if (game_state.mode_step == 5) {
            D_8014D010 = D_8014CD10;
        }
        break;
    case 1:
        D_8014D010 = D_8014CCE4;
        break;
    case 2:
        D_8014D010 = D_8014CD04;
        break;
    case 3:
        D_8014D010 = D_8014CD10;
        break;
    }
    for (i = 0; i < 0x23; i++) {
        line0[i] = 0;
        line1[i] = 0;
        line2[i] = 0;
        line3[i] = 0;
        line4[i] = 0;
    }
    if (D_8014CCB4 < 0 || D_8014CCB4 >= 0x2D) {
        D_8014CCB4 = 0;
    }
    if (D_8014CCC8 == 2 || D_8014CCB4 == 0x26) {
        func_80133510(D_8014D010);
    } else {
        text_system_reset();
        set_kanji_string(-0x98, -0x68, 0xA, D_8014D010, 0);
        set_kanji_string(-0x78, -0x30, 0xA, D_8014CDB0[D_8014CCB4], 0);
        D_8014CCB0 = strlen(D_8014CE68[D_8014CCB4]);
        if (D_8014CE64[D_8014CCB4] < D_8014CCAC * 16 + 0x40) {
            D_8014CCAC = (D_8014CE64[D_8014CCB4] >> 4) - 4;
            if (D_8014CCAC < 0) {
                D_8014CCAC = 0;
            }
        }
        text = D_8014CE68[D_8014CCB4];
        text += D_8014CCAC * 32;
        if (strlen(text) < 0x20) {
            strcpy(line0, text);
        } else {
            strncpy(line0, text, 0x20);
            if (strlen(text + 0x20) < 0x20) {
                strcpy(line1, text + 0x20);
            } else {
                strncpy(line1, text + 0x20, 0x20);
                if (strlen(text + 0x40) < 0x20) {
                    strcpy(line2, text + 0x40);
                } else {
                    strncpy(line2, text + 0x40, 0x20);
                    if (strlen(text + 0x60) < 0x20) {
                        strcpy(line3, text + 0x60);
                    } else {
                        strncpy(line3, text + 0x60, 0x20);
                        if (strlen(text + 0x80) < 0x20) {
                            strcpy(line4, text + 0x80);
                        } else {
                            strncpy(line4, text + 0x80);
                        }
                    }
                }
            }
        }
        if (line0[0] != 0) {
            set_kanji_string(-0x78, -0x20, 0xA, line0, 0);
        }
        if (line1[0] != 0) {
            set_kanji_string(-0x78, -0x10, 0xA, line1, 0);
        }
        if (line2[0] != 0) {
            set_kanji_string(-0x78, 0, 0xA, line2, 0);
        }
        if (line3[0] != 0) {
            set_kanji_string(-0x78, 0x10, 0xA, line3, 0);
        }
        if (line4[0] != 0) {
            set_kanji_string(-0x78, 0x20, 0xA, line4, 0);
        }
    }
    func_80132FD4();
}

void func_80133D94(void) {
    s32 x;
    s32 y;

    y = cursor_y;
    x = cursor_x;
    D_8014CCC0 = -1;
    if ((x >= -0x77) && (x < 0x98) && (y >= -0x2F) && (y < 0x30)) {
        D_8014CCC0 = (s32) (x + 0x78) / 16;
        D_8014CCC4 = (s32) (y + 0x30) / 16;
    } else {
        D_8014CCC4 = -1;
    }
    if ((D_8014CCC8 == 2) || (D_8014CCC8 == 3)) {
        if (D_8014CCC0 == 0x10) {
            D_8014CCC0 = -1;
        }
    }
}

void func_80133E54(void) {
    s32 x;
    s32 y;
    s32 i;

    x = cursor_x;
    y = cursor_y;
    D_8014CCCC = -1;
    for (i = 0; i < 5; i++) {
        if (x >= -0x9D && x < -0x80 && D_8014CCD0[i] < y && y < D_8014CCD0[i] + 0x10) {
            D_8014CCCC = i;
        }
    }
}

void func_80133FC4(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        if (i == D_8014CCC8) {
            ((Actor *)actor_pool)[i + 114].draw_flags |= 0x80;
        } else {
            ((Actor *)actor_pool)[i + 114].draw_flags &= 0x7F;
        }
    }
    if (D_8014CCCC >= 0) {
        D_80120C96 = (D_8014CCCC << 5) - 0x28;
        return;
    }
    D_80120C96 = -0xB4;
}

/* FAKE: implicit integer exit liveness is required; void differs in five
 * instructions. Callers use only the coordinate outputs. T-20011. */
s32 func_80134110(void) {
    Actor *actor;
    s32 x;
    s32 y;

    actor = (Actor *)actor_pool;
    x = actor->x;
    if (x >= -0x77 && x < 0x98) {
        y = actor->y;
        if (y >= 0x39 && y < 0x68) {
            D_8014CCB8 = (x + 0x78) / 16;
            D_8014CCBC = (y - 0x38) / 16;
            return;
        }
    }
    D_8014CCB8 = -1;
    D_8014CCBC = -1;
}

void func_801341A8(void) {
    s32 prev;

    prev = D_8014CCB4;
    if ((game_state.pad_pressed & PAD_CIRCLE) && D_8014CCB8 != -1 && D_8014CCBC != -1) {
        D_8014CCB4 = D_8014CCBC * 15 + D_8014CCB8 % 6 + D_8014CCB8 / 6 * 5;
        if (D_8014CCB4 != prev) {
            D_8014CCAC = 0;
        }
    }
}

/* FAKE: indirect remainder loads and empty column tests preserve distinct
 * value ranges; | 0 preserves comparison operand order. T-20013 records the
 * individual removal controls for these matching constraints. */
void func_80134248(s32 arg0) {
    s32 selected;
    u32 index;
    s32 column;
    u8 *dest;

    index = D_8014CC70;
    selected = index;
    switch (arg0) {
    case 0:
        if (selected >= 3) {
            selected = 2;
        }
        break;
    case 1:
        index = D_8014CC70;
        selected = index - 3;
        if (selected >= 3) {
            selected = 2;
        }
        break;
    case 2:
        index = D_8014CC70;
        selected = index - 6;
        if (selected >= 5) {
            selected = 4;
        }
        break;
    }
    switch (arg0) {
    case 0:
        dest = D_8014CC74;
        break;
    case 1:
        dest = D_8014CC7C;
        break;
    case 2:
        dest = D_8014CC84;
        break;
    }
    switch (D_8014CCC8) {
    case 0:
        column = *(s32 *)&D_8014CCC0 % 6;
        if ((column | 0) == 5) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (D_8014CCC4 == 2 && (D_8014CCC0 == 9 || D_8014CCC0 == 10)) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (D_8014CCC4 == 3 && (D_8014CCC0 == 3 || D_8014CCC0 == 4)) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (D_8014CCC4 == 5 && D_8014CCC0 >= 11) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else {
            dest[selected * 2] = ((NameKanaPages44 *)&D_8014CD28)->text[(s32)D_8014CCC0 / 6 + D_8014CCC4 * 3][column * 2];
            dest[selected * 2 + 1] = ((NameKanaPages44 *)&D_8014CD28)->text[(s32)D_8014CCC0 / 6 + D_8014CCC4 * 3][((s32)D_8014CCC0 % 6) * 2 + 1];
        }
        break;
    case 1:
        column = *(s32 *)&D_8014CCC0 % 6;
        if ((column | 0) == 5) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (D_8014CCC4 == 2 && (D_8014CCC0 == 9 || D_8014CCC0 == 10)) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (D_8014CCC4 == 3 && (D_8014CCC0 == 3 || D_8014CCC0 == 4)) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (D_8014CCC4 == 5 && D_8014CCC0 >= 11) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else {
            dest[selected * 2] = ((NameKanaPages44 *)&D_8014CD6C)->text[(s32)D_8014CCC0 / 6 + D_8014CCC4 * 3][column * 2];
            dest[selected * 2 + 1] = ((NameKanaPages44 *)&D_8014CD6C)->text[(s32)D_8014CCC0 / 6 + D_8014CCC4 * 3][((s32)D_8014CCC0 % 6) * 2 + 1];
        }
        break;
    case 2:
        if (D_8014CCC0) {
        }
        if (D_8014CCC0 < 16) {
            index = D_8014CCC4 * 16 + D_8014CCAC * 16 + D_8014CCC0;
            if (index < D_8014CE64[38]) {
                dest[selected * 2] = D_8014CE68[38][index * 2];
                dest[selected * 2 + 1] = D_8014CE68[38][index * 2 + 1];
            } else {
                dest[selected * 2] = D_800B3644[0];
                dest[selected * 2 + 1] = D_800B3644[1];
            }
        }
        break;
    case 3:
        if (D_8014CCC0) {
        }
        if (D_8014CCC0 < 16) {
            if (D_8014CCB4 == 38) {
                index = D_8014CCC4 * 16 + D_8014CCAC * 16 + D_8014CCC0;
                if (index < D_8014CE64[D_8014CCB4]) {
                    dest[selected * 2] = D_8014CE68[D_8014CCB4][index * 2];
                    dest[selected * 2 + 1] = D_8014CE68[D_8014CCB4][index * 2 + 1];
                } else {
                    dest[selected * 2] = D_800B3644[0];
                    dest[selected * 2 + 1] = D_800B3644[1];
                }
            } else if (D_8014CCC4 == 0) {
                if (D_8014CCC0 < strlen(D_8014CDB0[D_8014CCB4]) / 2) {
                    dest[selected * 2] = D_8014CDB0[D_8014CCB4][D_8014CCC0 * 2];
                    dest[selected * 2 + 1] = D_8014CDB0[D_8014CCB4][D_8014CCC0 * 2 + 1];
                } else {
                    dest[selected * 2] = D_800B3644[0];
                    dest[selected * 2 + 1] = D_800B3644[1];
                }
            } else {
                index = D_8014CCC4 * 16 + D_8014CCAC * 16 + D_8014CCC0 - 16;
                if (index < D_8014CE64[D_8014CCB4]) {
                    dest[selected * 2] = D_8014CE68[D_8014CCB4][index * 2];
                    dest[selected * 2 + 1] = D_8014CE68[D_8014CCB4][index * 2 + 1];
                } else {
                    dest[selected * 2] = D_800B3644[0];
                    dest[selected * 2 + 1] = D_800B3644[1];
                }
            }
        }
        break;
    }
    D_8014CC70++;
}

void func_80134B18(void) {
    k_sub_reset();
    set_kanji_string(-0x68, -0x58, 0xE, D_8014CC74, 0);
    set_kanji_string(-0x30, -0x58, 0xE, D_8014CC7C, 0);
    set_kanji_string(0x48, -0x58, 0xE, D_8014CC84, 0);
    k_sub_disp_start(3);
}

/* FAKE: s32 result with no return statement: as a void function the build loses the original's `move v0,zero`; no caller
 * reads the result. T-20810, T-21520 */
s32 func_80134B94(s32 arg0) {
    s32 i;
    s32 n;
    s32 r;

    r = rand() % 15;
    n = 0;
    if (arg0 == 0) {
        for (i = 0; i < 6; i++) {
            if (D_8014CC74[i] == D_800B3644[i & 1]) {
                n++;
            }
        }
        if (D_8014CC74[n] == 0) {
            strncpy(D_8014CC74, D_8014CE6C + r * 8, 8);
        }
        if (n == 6) {
            strncpy(D_8014CC74, D_8014CE6C + r * 8, 8);
        }
        if (D_8014CC74[0] == 0) {
            strncpy(D_8014CC74, D_8014CE6C + r * 8, 8);
        }
    }
    if (arg0 == 1) {
        for (i = 0; i < 6; i++) {
            if (D_8014CC7C[i] == D_800B3644[i & 1]) {
                n++;
            }
        }
        if (D_8014CC7C[n] == 0) {
            strncpy(D_8014CC7C, D_8014CEE4 + r * 8, 8);
        }
        if (n == 6) {
            strncpy(D_8014CC7C, D_8014CEE4 + r * 8, 8);
        }
        if (D_8014CC7C[0] == 0) {
            strncpy(D_8014CC7C, D_8014CEE4 + r * 8, 8);
        }
    }
    if (arg0 == 2) {
        for (i = 0; i < 10; i++) {
            if (D_8014CC84[i] == D_800B3644[i & 1]) {
                n++;
            }
        }
        if (D_8014CC84[n] == 0) {
            strncpy(D_8014CC84, D_8014CF5C + r * 12, 12);
        }
        if (n == 10) {
            strncpy(D_8014CC84, D_8014CF5C + r * 12, 12);
        }
        if (D_8014CC84[0] == 0) {
            strncpy(D_8014CC84, D_8014CF5C + r * 12, 12);
        }
    }
}

void func_80134EB8(void) {
    if ((D_8014CCC0 >= 0) && (D_8014CCC4 >= 0)) {
        D_80120CD6 = D_8014CCC0 * 0x10 - 0x71;
        D_80120CDA = D_8014CCC4 * 0x10 - 0x29;
        return;
    }
    if ((D_8014CCB8 >= 0) && (D_8014CCBC >= 0) && (D_8014CCB8 % 6 != 5)) {
        D_80120CD6 = D_8014CCB8 * 0x10 - 0x71;
        D_80120CDA = D_8014CCBC * 0x10 + 0x3F;
        return;
    }
    D_80120CD6 = 0x100;
    D_80120CDA = 0x100;
}

void func_80134F6C(void) {
    if (game_state.pad_held & PAD_SELECT) {
        switch (game_state.pad_pressed) {
        case PAD_L1:
            func_80132000(0xA);
            func_80134B18();
            return;
        case PAD_L2:
            func_80132000(1);
            func_80134B18();
            return;
        case PAD_R1:
            func_80132000(2);
            func_80134B18();
            return;
        case PAD_R2:
            func_80132000(9);
            func_80134B18();
            return;
        case 5:
            func_80132000(4);
            func_80134B18();
            return;
        case 10:
            func_80132000(3);
            func_80134B18();
            return;
        case 12:
            func_80132000(5);
            func_80134B18();
            return;
        case 3:
            func_80132000(6);
            func_80134B18();
            return;
        case 9:
            func_80132000(7);
            func_80134B18();
            return;
        case 6:
            func_80132000(8);
            func_80134B18();
            return;
        case 13:
            func_80132000(0xB);
            func_80134B18();
            return;
        case 7:
            func_80132000(0xC);
            func_80134B18();
            return;
        case 14:
            func_80132000(0xD);
            func_80134B18();
            return;
        case 11:
            func_80132000(0xE);
            func_80134B18();
            return;
        case 15:
            func_80132000(0);
            D_8014CC90 = 0xB;
            D_8014CC94 = 9;
            D_8014CC98 = 0;
            D_8014CC9C = 1;
            D_8014CCA0 = 0xD;
            D_8014CCA4 = 3;
            func_80134B18();
            break;
        }
    }
}

void func_80135168(void) {
    u8 *p;

    /* u8 view of the s8 flag D_8014D0A4 (unsigned compare against 2). T-11650 */
    if (*(u8 *)&D_8014D0A4 < 2) {
        if (*(u8 *)&D_8014D0A4 == 1) {
            memcpy(D_8014CC74, D_8014D070, 8);
            memcpy(D_8014CC7C, D_8014D078, 8);
            memcpy(D_8014CC84, D_8014D080, 0xC);
            D_8014CC90 = (u8) D_8014D08C % 12;
            D_8014CC94 = (u8) D_8014D090 % 31;
            D_8014CC98 = (u8) D_8014D094 & 3;
            D_8014CC9C = (u8) D_8014D098 % 12;
            D_8014CCA0 = (u8) D_8014D09C % 31;
            D_8014CCA4 = (u8) D_8014D0A0 & 3;
        } else {
            for (p = D_8014CC74; p < &D_8014CC74[6]; p += 2) {
                strcpy(p, D_800B3644);
            }
            for (p = D_8014CC7C; p < &D_8014CC7C[6]; p += 2) {
                strcpy(p, D_800B3644);
            }
            /* FAKE: initialisation on the do line gives the original as1 order of the two addiu. T-11650 */
            p = D_8014CC84; do {
                strcpy(p, D_800B3644);
                p += 2;
            } while (p != &D_8014CC8E);
            D_8014CC8E = 0;
            D_8014CC7C[7] = 0;
            D_8014CC7C[6] = 0;
            D_8014CC74[7] = 0;
            D_8014CC74[6] = 0;
            D_8014CC90 = 0xFF;
            D_8014CC94 = 0xFF;
            D_8014CC98 = 0xFF;
            D_8014CC9C = 0xFF;
            D_8014CCA0 = 0xFF;
            D_8014CCA4 = 0xFF;
            D_8014CC8F = 0;
            func_80134F6C();
        }
    }
    D_8014CC70 = 0;
    D_8014CCC8 = 0;
    func_801330E8(D_8014CCE4);
    D_8014CCB4 = 0;
    D_8014CCB8 = -1;
    D_8014CCBC = -1;
    func_801324AC();
    cd_audio_request_set(D_8014CC38, D_8014CC3C, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    func_80134B18();
    *(u8 *)&D_8014D0A4 = 2;
    mode_step_next();
}

void func_80135434(s32 arg0) {
    s32 col;

    if (arg0 != 0) {
        col = 0x90002;
    } else {
        col = 0x8081F;
    }
    prim_gradient_rect(-0x9D, -0x30, 0x1B, 0xF, 4, col, 0x83);
    prim_gradient_rect(-0x9D, -0x10, 0x1B, 0xF, 4, col, 0x83);
    prim_gradient_rect(-0x9D, 0x10, 0x1B, 0xF, 4, col, 0x83);
    prim_gradient_rect(-0x9D, 0x30, 0x1B, 0xF, 4, col, 0x83);
    prim_gradient_rect(-0x9D, 0x50, 0x1B, 0xF, 4, col, 0x83);
    prim_gradient_rect(-0x9A, -0x6A, 0x130, 0x20, 4, col, 0x83);
    prim_gradient_rect(-0x7F, -0x40, 0x11D, 0x70, 4, col, 0x83);
    if (D_8014CCC8 == 3) {
        prim_gradient_rect(-0x7A, 0x36, 0x50, 0x30, 4, col, 0x83);
        prim_gradient_rect(-0x1A, 0x36, 0x50, 0x30, 4, col, 0x83);
        prim_gradient_rect(0x46, 0x36, 0x50, 0x30, 4, col, 0x83);
    }
    dtd_on_tpage(0, 0, 4, 0, 0);
    if (!(game_state.pad_pressed & PAD_CIRCLE) && !(game_state.pad_pressed & PAD_START)) {
        if (!(game_state.pad_pressed & PAD_CROSS)) {
            func_80134F6C();
        }
    }
}

/* Implicit int in the original (no return value): v0 stays live at the exit, which changes how as1 fills the delay slots. T-11650 */
s32 func_80135650(void) {
    if (D_8014CCC8 == 2 || D_8014CCC8 == 3) {
        ((Actor *)actor_pool)[121].unk_18 = 0x19;
        ((Actor *)actor_pool)[122].unk_18 = 0x1B;
        ((Actor *)actor_pool)[121].draw_flags = 0x80;
        ((Actor *)actor_pool)[122].draw_flags = 0x80;
        ((Actor *)actor_pool)[123].draw_flags = 0;
        ((Actor *)actor_pool)[124].draw_flags = 0;
        if (((Actor *)actor_pool)[0].x >= 0x89 && ((Actor *)actor_pool)[0].x < 0x98) {
            if (((Actor *)actor_pool)[0].y >= -0x2F && ((Actor *)actor_pool)[0].y < -0x20) {
                ((Actor *)actor_pool)[121].unk_18 = 0x1A;
                if (game_state.pad_held & PAD_CIRCLE) {
                    ((Actor *)actor_pool)[123].draw_flags = 0x80;
                }
                if (game_state.pad_pressed & PAD_CIRCLE) {
                    if (D_8014CCAC > 0) {
                        D_8014CCAC--;
                    }
                    func_801338DC(0);
                }
            } else if (((Actor *)actor_pool)[0].y >= 0x11 && ((Actor *)actor_pool)[0].y < 0x20) {
                ((Actor *)actor_pool)[122].unk_18 = 0x1C;
                if (game_state.pad_held & PAD_CIRCLE) {
                    ((Actor *)actor_pool)[124].draw_flags = 0x80;
                }
                if (game_state.pad_pressed & PAD_CIRCLE) {
                    D_8014CCAC++;
                    func_801338DC(0);
                }
            }
        }
    } else {
        ((Actor *)actor_pool)[121].draw_flags = 0;
        ((Actor *)actor_pool)[122].draw_flags = 0;
        ((Actor *)actor_pool)[123].draw_flags = 0;
        ((Actor *)actor_pool)[124].draw_flags = 0;
    }
}

void func_801357F4(void) {
    cursor_update();
    func_80133D94();
    func_80133E54();
    func_80133FC4();
    func_80132B78(0);
    if (D_8014CCC8 == 3) {
        func_80134110();
        func_801341A8();
    }
    func_80135650();
    if (game_state.pad_pressed & PAD_CROSS) {
        /* FAKE: address-taken u8 view of the (already u8) selector keeps it in $v0 as in the original; real source unknown. T-11650 */
        if (*(u8 *)&D_8014CC70 == 0xC) {
            *(u8 *)&D_8014CC70 = 2;
        } else if (*(u8 *)&D_8014CC70 != 0) {
            *(u8 *)&D_8014CC70 -= 1;
        }
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        if (D_8014CC70 < 3 && D_8014CCC0 >= 0 && D_8014CCC4 >= 0) {
            func_80134248(0);
            func_80134B18();
        }
        if (D_8014CCC8 == 3) {
            func_801338DC(1);
            k_sub_reset_point_set();
            func_80134B18();
        } else if (D_8014CCC8 == 2) {
            func_80133510(D_8014CCE4);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 0) {
            D_8014CCC8 = 0;
            func_801330E8(D_8014CCE4);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 1) {
            D_8014CCC8 = 1;
            func_801332FC(D_8014CCE4);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 2) {
            D_8014CCC8 = 2;
            func_80133510(D_8014CCE4);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 3) {
            D_8014CCC8 = 3;
            func_801338DC(1);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 4) {
            D_8014CCC8 = 4;
            func_80134B94(0);
            func_80134B18();
            audio_command(0x501);
            mode_step_next();
        }
    } else if (game_state.pad_pressed & PAD_START) {
        func_80134B94(0);
        func_80134B18();
        audio_command(0x501);
        mode_step_next();
    }
}

void func_80135A98(void) {

    text_system_reset();
    if (D_8014CCC8 == 4) {
        D_8014CCC8 = 0;
    }
    switch (D_8014CCC8) {
    case 0:
        func_801330E8((s32) D_8014CD04);
        break;
    case 1:
        func_801332FC((s32) D_8014CD04);
        break;
    case 2:
        func_80133510((s32) D_8014CD04);
        break;
    case 3:
        func_801338DC(1);
        break;
    }
    D_8014CC70 = 3;
    D_801206AC = 0;
    D_801206BA = 0;
    D_801206BE = 0;
    cd_audio_request_set(D_8014CC40, D_8014CC44, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    func_80134B18();
    mode_step_next();
}

void func_80135BB4(void) {
    u8 cur;
    u8 temp;

    cursor_update();
    func_80133D94();
    func_80133E54();
    func_80133FC4();
    func_80132B78(1);
    if (D_8014CCC8 == 3) {
        func_80134110();
        func_801341A8();
    }
    func_80135650();
    if (game_state.pad_pressed & PAD_CROSS) {
        cur = (*(u16 *)&D_8014CC70);
        if (cur == 0xC) {
            cur = 6;
        }
        D_8014CC70 = cur;
        if (cur >= 4) {
            D_8014CC70 = cur - 1;
        }
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        temp = D_8014CC70;
        if (temp < 6 && D_8014CCC0 >= 0 && D_8014CCC4 >= 0) {
            func_80134248(1);
            func_80134B18();
        }
        if (D_8014CCC8 == 3) {
            func_801338DC(2);
            k_sub_reset_point_set();
            func_80134B18();
        } else if (D_8014CCC8 == 2) {
            func_80133510(D_8014CD04);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 0) {
            D_8014CCC8 = 0;
            func_801330E8(D_8014CD04);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 1) {
            D_8014CCC8 = 1;
            func_801332FC(D_8014CD04);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 2) {
            D_8014CCC8 = 2;
            func_80133510(D_8014CD04);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 3) {
            D_8014CCC8 = 3;
            func_801338DC(2);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 4) {
            D_8014CCC8 = 4;
            func_80134B94(1);
            func_80134B18();
            audio_command(0x501);
            mode_step_next();
        }
    } else if (game_state.pad_pressed & PAD_START) {
        func_80134B94(1);
        func_80134B18();
        audio_command(0x501);
        mode_step_next();
    }
}

void func_80135E58(void) {

    text_system_reset();
    if (D_8014CCC8 == 4) {
        D_8014CCC8 = 0;
    }
    switch (D_8014CCC8) {
    case 0:
        func_801330E8((s32) D_8014CD10);
        break;
    case 1:
        func_801332FC((s32) D_8014CD10);
        break;
    case 2:
        func_80133510((s32) D_8014CD10);
        break;
    case 3:
        func_801338DC(2);
        break;
    }
    D_8014CC70 = 6;
    D_801206AC = 0;
    D_801206BA = 0;
    D_801206BE = 0;
    cd_audio_request_set(D_8014CC48, D_8014CC4C, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    func_80134B18();
    mode_step_next();
}

void func_80135F74(void) {
    cursor_update();
    func_80133D94();
    func_80133E54();
    func_80133FC4();
    func_80132B78(2);
    if (D_8014CCC8 == 3) {
        func_80134110();
        func_801341A8();
    }
    func_80135650();
    if (game_state.pad_pressed & PAD_CROSS) {
        /* FAKE: same u8 view of the selector as in func_801357F4. T-11650 */
        if (*(u8 *)&D_8014CC70 == 0xC) {
            *(u8 *)&D_8014CC70 = 0xA;
        } else if (*(u8 *)&D_8014CC70 >= 7) {
            *(u8 *)&D_8014CC70 -= 1;
        }
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        if (D_8014CC70 < 0xB && D_8014CCC0 >= 0 && D_8014CCC4 >= 0) {
            func_80134248(2);
            func_80134B18();
        }
        if (D_8014CCC8 == 3) {
            func_801338DC(3);
            k_sub_reset_point_set();
            func_80134B18();
        } else if (D_8014CCC8 == 2) {
            func_80133510(D_8014CD10);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 0) {
            D_8014CCC8 = 0;
            func_801330E8(D_8014CD10);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 1) {
            D_8014CCC8 = 1;
            func_801332FC(D_8014CD10);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 2) {
            D_8014CCC8 = 2;
            func_80133510(D_8014CD10);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 3) {
            D_8014CCC8 = 3;
            func_801338DC(3);
            k_sub_reset_point_set();
            func_80134B18();
        }
        if (D_8014CCCC == 4) {
            D_8014CCC8 = 4;
            func_80134B94(2);
            func_80134B18();
            audio_command(0x501);
            mode_step_next();
        }
    } else if (game_state.pad_pressed & PAD_START) {
        func_80134B94(2);
        func_80134B18();
        audio_command(0x501);
        mode_step_next();
    }
}

void func_8013621C(s32 arg0) {
    s32 color;

    if (arg0 != 0) {
        color = 0x90002;
    } else {
        color = 0x8081F;
    }
    prim_gradient_rect(-0x9A, -0x6A, 0x130, 0x20, 4, color, 0x83);
    prim_gradient_rect(-0x7C, -0x3D, 0x118, 0x9E, 4, color, 0x83);
    prim_gradient_rect(-0x9D, 0x50, 0x1C, 0x10, 4, color, 0x83);
}

void func_801362D0(void) {
    text_system_reset();
    func_8013635C();
    D_8014CC70 = 0;
    set_kanji_string(-0x98, -0x68, 0xA, "誕生日と血液型も教えてね？", 0);
    cd_audio_request_set(D_8014CC50, D_8014CC54, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    func_801372A0(0);
    k_sub_disp_start(0);
    mode_step_next();
}

void func_8013635C(void) {
    s32 i;

    actor_clear_upper();
    for (i = 0; i < 12; i++) {
        ((Actor *)actor_pool)[96 + i].unk_01 = 2;
        ((Actor *)actor_pool)[96 + i].flags = 0x40;
        ((Actor *)actor_pool)[96 + i].quad_flags = 0;
        ((Actor *)actor_pool)[96 + i].draw_flags = 0x84;
        ((Actor *)actor_pool)[96 + i].frame_table = (u8 *)game_state.ovl_frame_table;
        ((Actor *)actor_pool)[96 + i].sequence_table = (u8 *)game_state.ovl_sequence_table;
        ((Actor *)actor_pool)[96 + i].tpage_table = (u8 *)game_state.ovl_tpage_table;
        ((Actor *)actor_pool)[96 + i].unk_04 = 8;
        ((Actor *)actor_pool)[96 + i].unk_05 = 0x81;
        ((Actor *)actor_pool)[96 + i].unk_43 = 0x10;
        ((Actor *)actor_pool)[96 + i].x = 0x190;
    }
    ((Actor *)actor_pool)[96].unk_18 = 1;
    ((Actor *)actor_pool)[96].x = 0;
    ((Actor *)actor_pool)[96].y = 0;
    ((Actor *)actor_pool)[97].unk_18 = 0xD;
    ((Actor *)actor_pool)[97].x = -0x48;
    ((Actor *)actor_pool)[97].y = -0x4A;
    ((Actor *)actor_pool)[98].unk_18 = 0xD;
    ((Actor *)actor_pool)[98].x = -0x38;
    ((Actor *)actor_pool)[98].y = -0x4A;
    ((Actor *)actor_pool)[99].unk_18 = 0xD;
    ((Actor *)actor_pool)[99].x = -0x18;
    ((Actor *)actor_pool)[99].y = -0x4A;
    ((Actor *)actor_pool)[100].unk_18 = 0xD;
    ((Actor *)actor_pool)[100].x = -8;
    ((Actor *)actor_pool)[100].y = -0x4A;
    ((Actor *)actor_pool)[101].unk_18 = 0xD;
    ((Actor *)actor_pool)[101].x = 0x68;
    ((Actor *)actor_pool)[101].y = -0x4A;
    ((Actor *)actor_pool)[102].unk_18 = 0xD;
    ((Actor *)actor_pool)[102].x = 0x78;
    ((Actor *)actor_pool)[102].y = -0x4A;
    ((Actor *)actor_pool)[103].unk_18 = 9;
    ((Actor *)actor_pool)[103].x = -0x8F;
    ((Actor *)actor_pool)[103].y = 0x58;
    ((Actor *)actor_pool)[103].unk_01 = 1;
    ((Actor *)actor_pool)[104].unk_18 = 5;
    ((Actor *)actor_pool)[104].x = 0;
    ((Actor *)actor_pool)[104].y = 0;
    ((Actor *)actor_pool)[104].unk_01 = 3;
    ((Actor *)actor_pool)[105].unk_18 = 0x17;
    ((Actor *)actor_pool)[105].x = -0x8F;
    ((Actor *)actor_pool)[105].y = 0x58;
    ((Actor *)actor_pool)[106].unk_18 = 7;
    ((Actor *)actor_pool)[106].x = -0x8F;
    ((Actor *)actor_pool)[106].y = 0x58;
    ((Actor *)actor_pool)[106].draw_flags &= 0x7F;
    ((Actor *)actor_pool)[107].unk_18 = 9;
    ((Actor *)actor_pool)[107].x = -0x8F;
    ((Actor *)actor_pool)[107].y = -0x28;
    ((Actor *)actor_pool)[107].unk_01 = 1;
}

void func_8013664C(s32 arg0) {
    u8 *p = actor_pool + arg0 * 0x44;

    if (!(p[2] & 1)) {
        *(s16 *)(p + 0x18) = 0;
        p[2] = 1;
    }
}

void func_80136688(void) {
    u8 *p;
    s32 i;

    if (D_8014CC70 == 0) {
        func_8013664C(0x61);
        func_8013664C(0x62);
        ((Actor *)actor_pool)[99].unk_18 = 0xD;
        ((Actor *)actor_pool)[99].flags = 0x40;
        ((Actor *)actor_pool)[100].unk_18 = 0xD;
        ((Actor *)actor_pool)[100].flags = 0x40;
        ((Actor *)actor_pool)[101].unk_18 = 0xD;
        ((Actor *)actor_pool)[101].flags = 0x40;
        ((Actor *)actor_pool)[102].unk_18 = 0xD;
        ((Actor *)actor_pool)[102].flags = 0x40;
    } else if (D_8014CC70 == 1) {
        func_8013664C(0x63);
        func_8013664C(0x64);
        for (i = 0; i < 2; i++) {
            p = actor_pool + i * 0x44;
            *(s16 *)(p + 0x19DC) = 0xD;
            p[0x19C6] = 0x40;
        }
        for (i = 4; i < 6; i++) {
            p = actor_pool + i * 0x44;
            *(s16 *)(p + 0x19DC) = 0xD;
            p[0x19C6] = 0x40;
        }
    } else if (D_8014CC70 == 2) {
        ((Actor *)actor_pool)[97].unk_18 = 0xD;
        ((Actor *)actor_pool)[97].flags = 0x40;
        ((Actor *)actor_pool)[98].unk_18 = 0xD;
        ((Actor *)actor_pool)[98].flags = 0x40;
        ((Actor *)actor_pool)[99].unk_18 = 0xD;
        ((Actor *)actor_pool)[99].flags = 0x40;
        ((Actor *)actor_pool)[100].unk_18 = 0xD;
        ((Actor *)actor_pool)[100].flags = 0x40;
        func_8013664C(0x65);
        func_8013664C(0x66);
    }
}

void func_80136808(s32 arg0) {
    s32 i;
    s32 x;
    s32 mouse_x;
    s32 y;

    if (D_8014CC70 == 0) {
        mouse_x = ((Actor *)actor_pool)[0].x;
        ((Actor *)actor_pool)[104].unk_18 = 5;
        ((Actor *)actor_pool)[104].x = -256;
        ((Actor *)actor_pool)[104].y = -256;
        for (i = 0; i < 12; i++) {
            x = (i % 6) * 40 - 104;
            y = (i / 6) * 16 - 40;
            if (x < mouse_x && mouse_x < x + 32 &&
                y < ((Actor *)actor_pool)[0].y && ((Actor *)actor_pool)[0].y < y + 16) {
                ((Actor *)actor_pool)[104].x = x + 22;
                ((Actor *)actor_pool)[104].y = y + 6;
                if (arg0 != 0) {
                    D_8014CC9C = i;
                } else {
                    D_8014CC90 = i;
                }
            }
        }
    } else if (D_8014CC70 == 1) {
        mouse_x = ((Actor *)actor_pool)[0].x;
        ((Actor *)actor_pool)[104].unk_18 = 4;
        ((Actor *)actor_pool)[104].x = -256;
        ((Actor *)actor_pool)[104].y = -256;
        for (i = 0; i < 31; i++) {
            x = (i % 11) * 24 - 104;
            y = (i / 11) * 16 + 8;
            if (x - 16 < mouse_x && mouse_x < x + 16 &&
                y < ((Actor *)actor_pool)[0].y && ((Actor *)actor_pool)[0].y < y + 16) {
                ((Actor *)actor_pool)[104].x = x + 1;
                ((Actor *)actor_pool)[104].y = y + 6;
                if (arg0 != 0) {
                    D_8014CCA0 = i;
                } else {
                    D_8014CC94 = i;
                }
            }
        }
    } else if (D_8014CC70 == 2) {
        mouse_x = ((Actor *)actor_pool)[0].x;
        ((Actor *)actor_pool)[104].unk_18 = 5;
        ((Actor *)actor_pool)[104].x = -256;
        ((Actor *)actor_pool)[104].y = -256;
        for (i = 0; i < 4; i++) {
            x = i * 64 - 96;
            if (x < mouse_x && mouse_x < x + 32 &&
                72 < ((Actor *)actor_pool)[0].y && ((Actor *)actor_pool)[0].y < 88) {
                ((Actor *)actor_pool)[104].x = x + 16;
                ((Actor *)actor_pool)[104].y = 78;
                if (arg0 != 0) {
                    D_8014CCA4 = i;
                } else {
                    D_8014CC98 = i;
                }
            }
        }
    }
}

void func_801372A0(arg0)
s32 arg0;
{
    s32 a;
    s32 b;
    s32 c;

    if (arg0 != 0) {
        a = D_8014CC9C;
        b = D_8014CCA0;
        c = D_8014CCA4;
    } else {
        a = D_8014CC90;
        b = D_8014CC94;
        c = D_8014CC98;
    }
    k_sub_reset();
    if (a == 0xFF) {
        set_kanji_string(-0x50, -0x58, 0xE, "　", 0);
        set_kanji_string(-0x40, -0x58, 0xE, "　", 0);
    } else {
        if (a >= 9) {
            set_kanji_string(-0x50, -0x58, 0xE, D_800B3288[1], 0);
        } else {
            set_kanji_string(-0x50, -0x58, 0xE, "　", 0);
        }
        set_kanji_string(-0x40, -0x58, 0xE, D_800B3288[(a + 1) % 10], 0);
    }
    if (b == 0xFF) {
        set_kanji_string(-0x20, -0x58, 0xE, "　", 0);
        set_kanji_string(-0x10, -0x58, 0xE, "　", 0);
    } else {
        if (b >= 9) {
            set_kanji_string(-0x20, -0x58, 0xE, D_800B3288[(b + 1) / 10], 0);
        } else {
            set_kanji_string(-0x20, -0x58, 0xE, D_800B3644, 0);
        }
        set_kanji_string(-0x10, -0x58, 0xE, D_800B3288[(b + 1) % 10], 0);
    }
    switch (c) {
    case 0:
        set_kanji_string(0x60, -0x58, 0xE, "　", 0);
        set_kanji_string(0x70, -0x58, 0xE, "Ａ", 0);
        break;
    case 1:
        set_kanji_string(0x60, -0x58, 0xE, "　", 0);
        set_kanji_string(0x70, -0x58, 0xE, "Ｂ", 0);
        break;
    case 2:
        set_kanji_string(0x60, -0x58, 0xE, "Ａ", 0);
        set_kanji_string(0x70, -0x58, 0xE, "Ｂ", 0);
        break;
    case 3:
        set_kanji_string(0x60, -0x58, 0xE, "　", 0);
        set_kanji_string(0x70, -0x58, 0xE, "Ｏ", 0);
        break;
    }
    k_sub_disp_start(6);
}

void func_80137608(s32 arg0) {
    s32 pad; /* FAKE: unused locals around the two cursor values, which the original reads without ever assigning them (T-3330 layout) */
    s32 sp30;
    s32 sp2C;
    s32 pad2;

    if (sp30 >= -0x9D && sp30 < -0x80 && sp2C >= 0x51 && sp2C < 0x60) {
        D_8012093F |= 0x80;
        if (game_state.pad_pressed & PAD_CIRCLE) {
            func_801377E8();
            mode_step_next();
        }
    } else {
        D_8012093F &= 0x7F;
    }
    if (D_8014CC70 == 3) {
        D_801208FB |= 0x80;
        if (game_state.pad_pressed & PAD_CIRCLE) {
            func_801377E8(arg0);
            mode_step_next();
        }
    } else {
        D_801208FB &= 0x7F;
    }
}

void func_80137708(void) {
    u8 temp;
    cursor_update();
    func_80136688();
    func_801372A0(0);
    func_80137608(0);
    if (game_state.pad_pressed & PAD_CROSS) {
        temp = D_8014CC70;
        if (temp != 0) {
            D_8014CC70 -= 1;
        }
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        D_8014CC70 += 1;
        func_801372A0(0);
        if ((u8) D_8014CC70 >= 3U) {
            func_801377E8(0);
            mode_step_next();
        }
    } else if (game_state.pad_pressed & PAD_START) {
        func_801377E8(0);
        mode_step_next();
    }
    func_80137AA0();
}

void func_801377E8(arg0)
s32 arg0;
{
    s32 a;

    if (arg0 != 0) {
        if (D_8014CC9C == 0xFF) {
            D_8014CC9C = rand() % 12;
        }
        if (D_8014CCA0 == 0xFF) {
            D_8014CCA0 = rand() % 31;
        }
        if (D_8014CCA4 == 0xFF) {
            D_8014CCA4 = rand() % 3;
        }
        a = D_8014CC9C + 1;
        if (a == 2 && D_8014CCA0 + 1 >= 0x1D) {
            D_8014CCA0 = 0x1B;
        }
        if (a == 4 && D_8014CCA0 + 1 >= 0x1F) {
            D_8014CCA0 = 0x1D;
        }
        if (a == 6 && D_8014CCA0 + 1 >= 0x1F) {
            D_8014CCA0 = 0x1D;
        }
        if (a == 9 && D_8014CCA0 + 1 >= 0x1F) {
            D_8014CCA0 = 0x1D;
        }
        if (a == 0xB && D_8014CCA0 + 1 >= 0x1F) {
            D_8014CCA0 = 0x1D;
        }
    } else {
        if (D_8014CC90 == 0xFF) {
            D_8014CC90 = rand() % 12;
        }
        if (D_8014CC94 == 0xFF) {
            D_8014CC94 = rand() % 31;
        }
        if (D_8014CC98 == 0xFF) {
            D_8014CC98 = rand() % 3;
        }
        a = D_8014CC90 + 1;
        if (a == 2 && D_8014CC94 + 1 >= 0x1D) {
            D_8014CC94 = 0x1B;
        }
        if (a == 4 && D_8014CC94 + 1 >= 0x1F) {
            D_8014CC94 = 0x1D;
        }
        if (a == 6 && D_8014CC94 + 1 >= 0x1F) {
            D_8014CC94 = 0x1D;
        }
        if (a == 9 && D_8014CC94 + 1 >= 0x1F) {
            D_8014CC94 = 0x1D;
        }
        if (a == 0xB && D_8014CC94 + 1 >= 0x1F) {
            D_8014CC94 = 0x1D;
        }
    }
}

void func_80137AA0(void) {
    s32 i;
    s32 y;

    for (i = 1, y = -0x70; i < 0xA; i++, y += 0x18) {
        func_80137B84(i, y, 8);
    }
    for (i = 0, y = 0x70; i < 2; i++, y += 0x18) {
        func_80137CA0(i + 0xA, y, 8);
    }
    for (i = 0, y = -0x68; i < 0xB; i++, y += 0x18) {
        func_80137CA0(i + 0xC, y, 0x18);
    }
    for (i = 0, y = -0x68; i != 9; i++, y += 0x18) {
        func_80137CA0(i + 0x17, y, 0x28);
    }
    goto_tpage(0x140, 0, 2, 0, 0);
}

void func_80137B84(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;
    u8 *q;

    p = GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(p);
    SetSemiTrans(p, 0);
    p[4] = game_state.screen_brightness;
    p[5] = game_state.screen_brightness;
    p[6] = game_state.screen_brightness;
    *(s16 *)(p + 8) = game_state.screen_center_x[draw_buffer] + arg1;
    *(s16 *)(p + 0xA) = game_state.screen_center_y[draw_buffer] + arg2;
    q = D_8014D028 + arg0 * 2;
    p[0xC] = q[0];
    p[0xD] = q[1];
    *(s16 *)(p + 0x12) = 0x10;
    *(s16 *)(p + 0x10) = 0x10;
    *(s16 *)(p + 0xE) = func_8009ED74(0x10, 0x1F0);
    AddPrim((draw_buffer << 10) + 8 + D_800E8CA0, p);
}

void func_80137CA0(s32 arg0, s32 arg1, s32 arg2) {
    SPRT *sprite;
    u8 *uv;

    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    sprite->r0 = game_state.screen_brightness;
    sprite->g0 = game_state.screen_brightness;
    sprite->b0 = game_state.screen_brightness;
    sprite->x0 = game_state.screen_center_x[draw_buffer] + arg1 - 8;
    sprite->y0 = game_state.screen_center_y[draw_buffer] + arg2;
    uv = &D_8014D014[arg0 / 10 * 2];
    sprite->u0 = uv[0];
    sprite->v0 = uv[1];
    setWH(sprite, 8, 0x10);
    sprite->clut = func_8009ED74(0x10, 0x1F0);
    AddPrim((draw_buffer << 10) + 8 + D_800E8CA0, (s32)sprite);
    sprite = (SPRT *)GetWorkBase(0x14, draw_buffer);
    func_8009F0A4(sprite);
    SetSemiTrans(sprite, 0);
    sprite->r0 = game_state.screen_brightness;
    sprite->g0 = game_state.screen_brightness;
    sprite->b0 = game_state.screen_brightness;
    sprite->x0 = game_state.screen_center_x[draw_buffer] + arg1;
    sprite->y0 = game_state.screen_center_y[draw_buffer] + arg2;
    uv = &D_8014D014[arg0 % 10 * 2];
    sprite->u0 = uv[0];
    sprite->v0 = uv[1];
    setWH(sprite, 8, 0x10);
    sprite->clut = func_8009ED74(0x10, 0x1F0);
    AddPrim((draw_buffer << 10) + 8 + D_800E8CA0, (s32)sprite);
}

void func_80137EE4(void) {
    u8 *p;
    s32 i;

    actor_clear_upper();
    for (i = 0; i < 2; i++) {
        p = actor_pool + i * 0x44;
        p[0x1981] = 2;
        p[0x1982] = 0x40;
        *(s32 *)(p + 0x19B8) = 0;
        p[0x1983] = 0x84;
        *(s32 *)(p + 0x198C) = game_state.ovl_frame_table;
        *(s32 *)(p + 0x1990) = game_state.ovl_sequence_table;
        *(s32 *)(p + 0x19B4) = game_state.ovl_tpage_table;
        p[0x1984] = 8;
        p[0x1985] = 0x81;
        p[0x19C3] = 0x10;
        *(s16 *)(p + 0x19A6) = 0x190;
    }
    D_80120668 = 2;
    D_80120676 = 0;
    D_8012067A = 0;
    D_801206AC = 6;
    D_801206BA = -0xA;
    D_801206BE = 0x58;
    D_80120695 = 3;
}

void func_80137FC4(void) {
    text_system_reset();
    func_80137EE4();
    set_kanji_string(-0x18, 0x40, 0xA, "これでいい？", 0);
    set_kanji_string(-0x38, -0x37, 0xA, D_8014CC74, 0);
    set_kanji_string(0, -0x37, 0xA, D_8014CC7C, 0);
    set_kanji_string(-0x38, -0x17, 0xA, D_8014CC84, 0);
    if (D_8014CC90 + 1 >= 10) {
        set_kanji_string(-0x38, 8, 0xA, D_800B3288[(D_8014CC90 + 1) / 10], 0);
    }
    set_kanji_string(-0x28, 8, 0xA, D_800B3288[(D_8014CC90 + 1) % 10], 0);
    if (D_8014CC94 + 1 >= 10) {
        set_kanji_string(-8, 8, 0xA, D_800B3288[(D_8014CC94 + 1) / 10], 0);
    }
    set_kanji_string(8, 8, 0xA, D_800B3288[(D_8014CC94 + 1) % 10], 0);
    switch (D_8014CC98) {
    case 0:
        set_kanji_string(-0x28, 0x28, 0xA, "Ａ", 0);
        break;
    case 1:
        set_kanji_string(-0x28, 0x28, 0xA, "Ｂ", 0);
        break;
    case 2:
        set_kanji_string(-0x38, 0x28, 0xA, "ＡＢ", 0);
        break;
    case 3:
        set_kanji_string(-0x28, 0x28, 0xA, "Ｏ", 0);
        break;
    }
    cd_audio_request_set(D_8014CC60, D_8014CC64, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    mode_step_next();
}

void func_80138268(s32 arg0) {
    s32 color;

    if (arg0 != 0) {
        color = 0x90002;
    } else {
        color = 0x8081F;
    }
    if (arg0 != 0) {
        prim_gradient_rect(-0x7E, -0x3D, 0x11A, 0x58, 4, color, 0x83);
    } else {
        prim_gradient_rect(-0x7E, -0x3D, 0x11A, 0x76, 4, color, 0x83);
    }
    prim_gradient_rect(-0x28, 0x3B, 0x7C, 0x26, 4, color, 0x83);
}

void func_80138330(void) {
}

void func_80138338(void) {
    s32 pad; /* FAKE: unused local, the original frame has the spill slots 4 bytes higher (T-3330) */
    s32 x;
    s32 y;

    x = cursor_x;
    y = cursor_y;
    cursor_update();
    if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
        D_801206BA = -0xA;
    }
    if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
        D_801206BA = 0x2E;
    }
    if (game_state.pad_pressed & PAD_START) {
        audio_command(0x501);
        mode_stage_next();
    } else if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
            audio_command(0x501);
            mode_stage_next();
        }
        if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
            mode_step_set(0);
        }
    }
}

void func_80138488(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        func_801385A8();
        break;
    case 1:
        func_80138634();
        break;
    case 2:
        func_801387F4();
        break;
    case 3:
        func_80138A7C();
        break;
    case 4:
        func_80138BCC();
        break;
    case 5:
        func_80139684();
        break;
    }
    if (game_state.mode_step < 4) {
        if (game_state.mode_step < 2) {
            func_8013621C(1);
            func_80136808(1);
        } else {
            func_80138268(1);
            func_80138330();
        }
    }
}

void func_801385A8(void) {
    text_system_reset();
    D_8014CC70 = 0;
    set_kanji_string(-0x98, -0x68, 0xA, "藤崎詩織の誕生日と血液型は覚えてる？", 0);
    func_8013635C();
    cd_audio_request_set(D_8014CC58, D_8014CC5C, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    func_801372A0(1);
    k_sub_disp_start(0);
    mode_step_next();
}

void func_80138634(void) {
    u8 temp;
    cursor_update();
    func_80136688();
    func_801372A0(1);
    func_80137608(1);
    if (game_state.pad_pressed & PAD_CROSS) {
        temp = D_8014CC70;
        if (temp != 0) {
            D_8014CC70 -= 1;
        }
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        D_8014CC70 += 1;
        func_801372A0(1);
        if ((u8) D_8014CC70 >= 3U) {
            func_801377E8(1);
            mode_step_next();
        }
    } else if (game_state.pad_pressed & PAD_START) {
        func_801377E8(1);
        mode_step_next();
    }
    func_80137AA0();
}

void func_80138714(void) {
    u8 *p;
    s32 i;

    actor_clear_upper();
    for (i = 0; i < 2; i++) {
        p = actor_pool + i * 0x44;
        p[0x1981] = 2;
        p[0x1982] = 0x40;
        *(s32 *)(p + 0x19B8) = 0;
        p[0x1983] = 0x84;
        *(s32 *)(p + 0x198C) = game_state.ovl_frame_table;
        *(s32 *)(p + 0x1990) = game_state.ovl_sequence_table;
        *(s32 *)(p + 0x19B4) = game_state.ovl_tpage_table;
        p[0x1984] = 8;
        p[0x1985] = 0x81;
        p[0x19C3] = 0x10;
        *(s16 *)(p + 0x19A6) = 0x190;
    }
    D_80120668 = 0xE;
    D_80120676 = 0;
    D_8012067A = 0;
    D_801206AC = 6;
    D_801206BA = -0xA;
    D_801206BE = 0x58;
    D_80120695 = 3;
}

void func_801387F4(void) {
    text_system_reset();
    set_kanji_string(-0x18, 0x40, 0xA, "これでいい？", 0);
    set_kanji_string(-0x38, -0x38, 0xA, "藤崎", 0);
    set_kanji_string(0, -0x38, 0xA, "詩織", 0);
    if (D_8014CC9C + 1 >= 10) {
        set_kanji_string(-0x38, -0x18, 0xA, D_800B3288[(D_8014CC9C + 1) / 10], 0);
    }
    set_kanji_string(-0x28, -0x18, 0xA, D_800B3288[(D_8014CC9C + 1) % 10], 0);
    if (D_8014CCA0 + 1 >= 10) {
        set_kanji_string(-8, -0x18, 0xA, D_800B3288[(D_8014CCA0 + 1) / 10], 0);
    }
    set_kanji_string(8, -0x18, 0xA, D_800B3288[(D_8014CCA0 + 1) % 10], 0);
    switch (D_8014CCA4) {
    case 0:
        set_kanji_string(-0x28, 8, 0xA, "Ａ", 0);
        break;
    case 1:
        set_kanji_string(-0x28, 8, 0xA, "Ｂ", 0);
        break;
    case 2:
        set_kanji_string(-0x38, 8, 0xA, "ＡＢ", 0);
        break;
    case 3:
        set_kanji_string(-0x28, 8, 0xA, "Ｏ", 0);
        break;
    }
    func_80138714();
    cd_audio_request_set(D_8014CC60, D_8014CC64, 0);
    audio_command(0x300);
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    mode_step_next();
}

void func_80138A7C(void) {
    s32 pad; /* FAKE: unused local, the original frame has the spill slots 4 bytes higher (T-3330) */
    s32 x;
    s32 y;

    x = cursor_x;
    y = cursor_y;
    cursor_update();
    if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
        D_801206BA = -0xA;
    }
    if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
        D_801206BA = 0x2E;
    }
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
            audio_command(0x501);
            mode_step_next();
        }
        if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
            mode_step_set(0);
        }
    } else if (game_state.pad_pressed & PAD_START) {
        audio_command(0x501);
        mode_step_next();
    } else if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
    }
}

void func_80138BCC(void) {
    audio_command(5);
    audio_command(0xC1);
    audio_command(3);
    actor_clear_upper();
    func_80138F60();
    text_system_reset();
    mode_step_next();
}

/* FAKE: s32 with no return statement, as declared in NAME_ENT.h: with a void declaration the caller
 * func_80138F60 allocates its date registers differently; no caller reads the result. T-25149 */
s32 func_80138C1C(void) {
    s32 n;
    s32 i;
    GameState *gs;
    /* FAKE: 0x20 bytes give the original's 96-byte frame with buf at sp+0x34; the copies need only
     * 17 bytes and the real size is unknown. T-25149 */
    u8 buf[0x20];
    u8 c;

    for (n = 0; n < 8; n++) {
        game_state.player_family_name[n] = 0;
        game_state.player_given_name[n] = 0;
    }
    for (n = 0; n < 0x10; n++) {
        game_state.player_full_name[n] = 0;
        game_state.player_nickname[n] = 0;
    }
    /* Stores through gs may alias the name arrays, so uopt reloads the second byte in each test
     * while as1 still hoists it above the stores; the full-width space bytes stay promoted. T-25149 */
    gs = &game_state;
    for (i = 0, n = 0; i < 6; i += 2) {
        c = D_8014CC74[i];
        if ((c != D_800B3644[0] || D_8014CC74[i + 1] != D_800B3644[1]) && (c != 0 || D_8014CC74[i + 1] != 0)) {
            gs->player_family_name[n * 2] = c;
            gs->player_family_name[n * 2 + 1] = D_8014CC74[i + 1];
            n++;
        }
    }
    for (i = 0, n = 0; i < 6; i += 2) {
        c = D_8014CC7C[i];
        if ((c != D_800B3644[0] || D_8014CC7C[i + 1] != D_800B3644[1]) && (c != 0 || D_8014CC7C[i + 1] != 0)) {
            gs->player_given_name[n * 2] = c;
            gs->player_given_name[n * 2 + 1] = D_8014CC7C[i + 1];
            n++;
        }
    }
    for (i = 0, n = 0; i < 10; i += 2) {
        c = D_8014CC84[i];
        if ((c != D_800B3644[0] || D_8014CC84[i + 1] != D_800B3644[1]) && (c != 0 || D_8014CC84[i + 1] != 0)) {
            gs->player_nickname[n * 2] = c;
            gs->player_nickname[n * 2 + 1] = D_8014CC84[i + 1];
            n++;
        }
    }
    strncpy(buf, game_state.player_family_name, 8);
    strncat(buf, game_state.player_given_name, 8);
    strncpy(game_state.player_full_name, buf, 0x10);
    /* FAKE: the redundant `& 3` on the two-bit fields spends the ugen temporary the original spends
     * there (as1 deletes the AND); without it every later temporary is one register low. T-25149 */
    ((WordView *)&game_state.player_birthday)->date.month = D_8014CC90 + 1;
    ((WordView *)&game_state.player_birthday)->date.day = D_8014CC94 + 1;
    ((WordView *)&game_state.player_birthday)->date.blood_type = (D_8014CC98 + 1) & 3;
    game_state.girls[0].birthday.date.month = D_8014CC9C + 1;
    game_state.girls[0].birthday.date.day = D_8014CCA0 + 1;
    game_state.girls[0].birthday.date.blood_type = (D_8014CCA4 + 1) & 3;
}

void func_80138F00(void) {
    game_state.ovl_tpage_table = 0x801AAF58;
    game_state.ovl_frame_table = 0x801AAF5C;
    game_state.ovl_sequence_table = 0x801AAFD0;
    game_state.ovl_palette_a = 0x80190000;
    game_state.ovl_palette_b = 0x801AA200;
    game_state.ovl_image_buffer = 0x80192200;
}

void func_80138F60(void) {
    /* FAKE: register + pre-call initializers place month/day/idx on $v0/$a2/$a1 after func_80138C1C. T-24113 */
    register u32 month;
    u32 day;
    u32 idx;
    u32 var_t0;

    month = D_8014CC70;
    day = 0;
    idx = 0;
    func_80041878();
    func_80138C1C();
    month = game_state.girls[0].birthday.date.month;
    day = game_state.girls[0].birthday.date.day;
    game_state.girls[0].flags.f.club = (s32)(day * 3 + month) % 11;
    idx = D_8014D03C[game_state.girls[0].birthday.date.blood_type + (*(NameProfileFlags *)&game_state.player_birthday).blood_type * 4] - 1;
    game_state.girls[0].affection = D_8014D04C[idx * 3];
    game_state.girls[0].tokimeki = D_8014D04C[idx * 3 + 1];
    game_state.girls[0].syoushin = D_8014D04C[idx * 3 + 2];
    var_t0 = game_state.girls[0].flags.f.club;
    if (var_t0 == 0) {
        game_state.girls[1].flags.f.club = CLUB_DRAMA;
    } else if (var_t0 == 1) {
        game_state.girls[1].flags.f.club = CLUB_LITERATURE;
    } else if (rand() & 1) {
        game_state.girls[1].flags.f.club = CLUB_DRAMA;
    } else {
        game_state.girls[1].flags.f.club = CLUB_LITERATURE;
    }
    game_state.girls[1].birthday.date.blood_type = 1;
    if (var_t0 == 2) {
        game_state.girls[2].flags.f.club = CLUB_COMPUTER;
    } else if (var_t0 == 3) {
        game_state.girls[2].flags.f.club = CLUB_SCIENCE;
    } else if (rand() & 1) {
        game_state.girls[2].flags.f.club = CLUB_COMPUTER;
    } else {
        game_state.girls[2].flags.f.club = CLUB_SCIENCE;
    }
    game_state.girls[2].birthday.date.blood_type = 1;
    if (var_t0 == 4) {
        game_state.girls[3].flags.f.club = CLUB_BRASS_BAND;
    } else if (var_t0 == 5) {
        game_state.girls[3].flags.f.club = CLUB_ART;
    } else if (rand() & 1) {
        game_state.girls[3].flags.f.club = CLUB_BRASS_BAND;
    } else {
        game_state.girls[3].flags.f.club = CLUB_ART;
    }
    game_state.girls[3].birthday.date.blood_type = 2;
    if (var_t0 == 6) {
        game_state.girls[4].flags.f.club = CLUB_SOCCER;
    } else if (var_t0 == 7) {
        game_state.girls[4].flags.f.club = CLUB_BASEBALL;
    } else if (rand() & 1) {
        game_state.girls[4].flags.f.club = CLUB_SOCCER;
    } else {
        game_state.girls[4].flags.f.club = CLUB_BASEBALL;
    }
    game_state.girls[4].birthday.date.blood_type = 1;
    game_state.girls[5].flags.f.club = CLUB_TENNIS;
    game_state.girls[5].birthday.date.blood_type = 2;
    game_state.girls[6].flags.f.club = CLUB_SWIMMING;
    game_state.girls[6].birthday.date.blood_type = 1;
    game_state.girls[7].flags.f.club = CLUB_NONE;
    game_state.girls[7].birthday.date.blood_type = 0;
    game_state.girls[8].flags.f.club = CLUB_NONE;
    game_state.girls[8].birthday.date.blood_type = 2;
    game_state.girls[9].flags.f.club = CLUB_NONE;
    game_state.girls[9].birthday.date.blood_type = 1;
    game_state.girls[10].flags.f.club = CLUB_BASKETBALL;
    game_state.girls[10].birthday.date.blood_type = 0;
    game_state.girls[11].flags.f.club = CLUB_NONE;
    game_state.girls[11].birthday.date.blood_type = 1;
    game_state.girls[12].birthday.date.blood_type = 3;
    func_80139440();
}

/* The nine ParamValue members of GameState (+0xFC..+0x11C) are walked as one array, as in main
 * 8007C030: IDO unrolls each nine-record loop into one peeled store and two groups of four. */
void func_80139440(void) {
    s32 i;

    if (strcmp(D_8014CC84, "こなみまん") == 0) {
        for (i = 0; i < 9; i++) {
            (&game_state.param_condition)[i].value = 0x23D;
        }
    }
    if (strcmp(D_8014CC74, "みんな") == 0 && strcmp(D_8014CC7C, "仲良し") == 0) {
        game_state.girls[0].tokimeki = 0x20;
        game_state.girls[0].affection = 0x40;
        for (i = 1; i < 9; i++) {
            if ((game_state.girls[i].flags.b[2] >> 4) != (game_state.girls[0].flags.b[2] >> 4)) {
                game_state.girls[i].tokimeki = 0x7F;
                game_state.girls[i].flags.f.met = 1;
                game_state.girls[i].flags.f.b3 = 1;
                game_state.girls[i].affection = 0x7F;
            }
        }
    }
    if (strcmp(D_8014CC74, "上上") == 0 && strcmp(D_8014CC7C, "下下") == 0
        && strcmp(D_8014CC84, "左右左右×") == 0) {
        for (i = 0; i < 9; i++) {
            (&game_state.param_condition)[i].value = 0;
        }
    }
    if (strcmp(D_8014CC74, "↑↑") == 0 && strcmp(D_8014CC7C, "↓↓") == 0
        && strcmp(D_8014CC84, "←→←→×") == 0) {
        for (i = 0; i < 9; i++) {
            (&game_state.param_condition)[i].value = 0x3E7;
        }
    }
}

/* Implicit int like func_80135650. FAKE: the address-taken u8 views of the u8 fields give the original $v1/$a0 chain (a member access does not); real source unknown. T-11650 */
s32 func_80139684(void) {
    switch (*(u8 *)&game_state.substep) {
    case 0:
        if (game_state.mode_step_timer.u >= 0x41U) {
            *(u8 *)&game_state.substep += 1;
            return;
        }
        return;
    case 1:
        if (*(u8 *)&game_state.screen_brightness >= 4) {
            *(u8 *)&game_state.screen_brightness -= 4;
        } else {
            *(u8 *)&game_state.substep += 1;
            *(u8 *)&game_state.screen_brightness = 0;
        }
        set_dec_bri(*(u8 *)&game_state.screen_brightness);
        return;
    case 2:
        game_mode_set(GAME_MODE_PROLOGUE);
        text_set_advance(0xE);
        break;
    }
}

