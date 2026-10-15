#include "common.h"
#include "ovl/NAME_ENT.h"
#include "game.h"

void func_80132000(s32 arg0) {
    s32 rem;

    rem = arg0 % 15;
    func_800AE090(D_8014CC74, D_8014CE6C + rem * 8, 8);
    func_800AE090(D_8014CC7C, D_8014CEE4 + rem * 8, 8);
    func_800AE090(D_8014CC84, D_8014CF5C + rem * 0xC, 0xC);
}

void func_8013209C(void) {
    switch (D_800E6280.unk_1109) {
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
    func_800578F4(2);
    func_8004B19C(D_801220D0, 0, 0x50);
    func_8004B19C(D_801220F4, 0, 0x5A);
}

void func_80132134(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
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
  if (D_800E6280.unk_110D == 0)
  {
    func_8004E58C();
    text_set_advance(0x10);
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048E78();
    func_8006BC28(0);
    func_8006BD6C(0);
    back_clear_switch(1);
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_038 = 0;
    tpage_buf_clear();
    func_80041168(0);
    D_800E6280.unk_10A2 = 0;
    dec_bg_reset();
    dec_bg_show_switch(2);
    func_800462C8(0xAU, 0x80162000, 0xAA18);
    D_800E6280.unk_F6F = 3;
    D_800B593C = 0;
    D_800E6280.unk_110D = 1;
 ;
  }
  else
    if (D_800E6280.unk_110D == 1)
  {
    if (cd_read_sync_status() & 1)
    {
      func_80053650(0x80167000, 0x80188000, 0x80162000);
      func_800536AC(0x140, 0x100, 0x140, 0xF0);
      cd_read_sectors(0x36U, 0x80190000, 0xA9E2);
      D_800E6280.unk_110D += 1;
    }
  }
  else
    if (D_800E6280.unk_110D == 2)
  {
    if (cd_read_sync_status() & 1)
    {
      cd_read_sectors(6U, 0x801E0000, 0x9DBF);
      func_8004284C();
    }
  }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80132320);

void func_801324AC(void) {
    s32 index;
    DisplayActor44 *actor;

    func_80138F00();
    for (index = 0; index < 0x20;) {
        func_80048F64(index + 0x60);
        actor = (DisplayActor44 *)D_8011ECD0 + index;
        actor[96].unk_01 = 2;
        actor[96].unk_02 = 0x40;
        actor[96].unk_38 = 0;
        actor[96].unk_03 = 0x84;
        actor[96].unk_0C = (u8 *)D_800E6280.unk_1A4C;
        actor[96].unk_10 = (u8 *)D_800E6280.unk_1A5C;
        actor[96].unk_34 = (u8 *)D_800E6280.unk_1A3C;
        /* FAKE: re-index here for the original induction-update order. T-10890. */
        index++;
        actor = (DisplayActor44 *)D_8011ECD0 + index;
        actor[95].unk_04 = 8;
        actor[95].unk_05 = 0x81;
        actor[95].unk_18 = 0;
        actor[95].unk_43 = 0x10;
        actor[95].unk_26 = 0x190;
        actor[95].unk_2A = 0;
    }
    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 3;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x70;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x3C;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = 0;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_2A = 0;
    ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0xD;
    ((DisplayActor44 *)D_8011ECD0)[98].unk_26 = -0x60;
    ((DisplayActor44 *)D_8011ECD0)[98].unk_2A = -0x4A;
    ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0xD;
    ((DisplayActor44 *)D_8011ECD0)[99].unk_26 = -0x50;
    ((DisplayActor44 *)D_8011ECD0)[99].unk_2A = -0x4A;
    ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0xD;
    ((DisplayActor44 *)D_8011ECD0)[100].unk_26 = -0x40;
    ((DisplayActor44 *)D_8011ECD0)[100].unk_2A = -0x4A;
    ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0xD;
    ((DisplayActor44 *)D_8011ECD0)[101].unk_26 = -0x28;
    ((DisplayActor44 *)D_8011ECD0)[101].unk_2A = -0x4A;
    ((DisplayActor44 *)D_8011ECD0)[102].unk_18 = 0xD;
    ((DisplayActor44 *)D_8011ECD0)[102].unk_26 = -0x18;
    ((DisplayActor44 *)D_8011ECD0)[102].unk_2A = -0x4A;
    ((DisplayActor44 *)D_8011ECD0)[103].unk_18 = 0xD;
    ((DisplayActor44 *)D_8011ECD0)[103].unk_26 = -8;
    ((DisplayActor44 *)D_8011ECD0)[103].unk_2A = -0x4A;
    ((DisplayActor44 *)D_8011ECD0)[104].unk_18 = 0xD;
    ((DisplayActor44 *)D_8011ECD0)[104].unk_26 = 0x50;
    ((DisplayActor44 *)D_8011ECD0)[104].unk_2A = -0x4A;
    ((DisplayActor44 *)D_8011ECD0)[105].unk_18 = 0xD;
    ((DisplayActor44 *)D_8011ECD0)[105].unk_26 = 0x60;
    ((DisplayActor44 *)D_8011ECD0)[105].unk_2A = -0x4A;
    ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = 0xD;
    ((DisplayActor44 *)D_8011ECD0)[106].unk_26 = 0x70;
    ((DisplayActor44 *)D_8011ECD0)[106].unk_2A = -0x4A;
    ((DisplayActor44 *)D_8011ECD0)[107].unk_18 = 0xD;
    ((DisplayActor44 *)D_8011ECD0)[107].unk_26 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[107].unk_2A = -0x4A;
    ((DisplayActor44 *)D_8011ECD0)[108].unk_18 = 0xD;
    ((DisplayActor44 *)D_8011ECD0)[108].unk_26 = 0x90;
    ((DisplayActor44 *)D_8011ECD0)[108].unk_2A = -0x4A;
    ((DisplayActor44 *)D_8011ECD0)[109].unk_18 = 0xF;
    ((DisplayActor44 *)D_8011ECD0)[109].unk_26 = -0x8F;
    ((DisplayActor44 *)D_8011ECD0)[109].unk_2A = -0x28;
    ((DisplayActor44 *)D_8011ECD0)[110].unk_18 = 0x11;
    ((DisplayActor44 *)D_8011ECD0)[110].unk_26 = -0x8F;
    ((DisplayActor44 *)D_8011ECD0)[110].unk_2A = -8;
    ((DisplayActor44 *)D_8011ECD0)[111].unk_18 = 0x13;
    ((DisplayActor44 *)D_8011ECD0)[111].unk_26 = -0x8F;
    ((DisplayActor44 *)D_8011ECD0)[111].unk_2A = 0x18;
    ((DisplayActor44 *)D_8011ECD0)[112].unk_18 = 0x15;
    ((DisplayActor44 *)D_8011ECD0)[112].unk_26 = -0x8F;
    ((DisplayActor44 *)D_8011ECD0)[112].unk_2A = 0x38;
    ((DisplayActor44 *)D_8011ECD0)[113].unk_18 = 0x17;
    ((DisplayActor44 *)D_8011ECD0)[113].unk_26 = -0x8F;
    ((DisplayActor44 *)D_8011ECD0)[113].unk_2A = 0x58;
    ((DisplayActor44 *)D_8011ECD0)[114].unk_18 = 7;
    ((DisplayActor44 *)D_8011ECD0)[114].unk_26 = -0x8F;
    ((DisplayActor44 *)D_8011ECD0)[114].unk_2A = -0x28;
    ((DisplayActor44 *)D_8011ECD0)[115].unk_18 = 7;
    ((DisplayActor44 *)D_8011ECD0)[115].unk_26 = -0x8F;
    ((DisplayActor44 *)D_8011ECD0)[115].unk_2A = -8;
    ((DisplayActor44 *)D_8011ECD0)[116].unk_18 = 7;
    ((DisplayActor44 *)D_8011ECD0)[116].unk_26 = -0x8F;
    ((DisplayActor44 *)D_8011ECD0)[116].unk_2A = 0x18;
    ((DisplayActor44 *)D_8011ECD0)[117].unk_18 = 7;
    ((DisplayActor44 *)D_8011ECD0)[117].unk_26 = -0x8F;
    ((DisplayActor44 *)D_8011ECD0)[117].unk_2A = 0x38;
    ((DisplayActor44 *)D_8011ECD0)[118].unk_18 = 7;
    ((DisplayActor44 *)D_8011ECD0)[118].unk_26 = -0x8F;
    ((DisplayActor44 *)D_8011ECD0)[118].unk_2A = 0x58;
    /* FAKE: one line preserves as1 scheduling of the unrolled flag stores. T-10890. */
    index = 0; do { actor = (DisplayActor44 *)D_8011ECD0 + index; actor[114].unk_03 &= 0x7F; index++; } while (index < 5);
    ((DisplayActor44 *)D_8011ECD0)[119].unk_18 = 9;
    ((DisplayActor44 *)D_8011ECD0)[119].unk_26 = -0x8F;
    ((DisplayActor44 *)D_8011ECD0)[119].unk_2A = -0x28;
    ((DisplayActor44 *)D_8011ECD0)[119].unk_01 = 1;
    ((DisplayActor44 *)D_8011ECD0)[120].unk_18 = 4;
    ((DisplayActor44 *)D_8011ECD0)[120].unk_26 = 0x100;
    ((DisplayActor44 *)D_8011ECD0)[120].unk_2A = 0x100;
    ((DisplayActor44 *)D_8011ECD0)[120].unk_01 = 3;
    ((DisplayActor44 *)D_8011ECD0)[121].unk_18 = 0x19;
    ((DisplayActor44 *)D_8011ECD0)[121].unk_26 = 0x88;
    ((DisplayActor44 *)D_8011ECD0)[121].unk_2A = -0x28;
    ((DisplayActor44 *)D_8011ECD0)[121].unk_03 = 0;
    ((DisplayActor44 *)D_8011ECD0)[122].unk_18 = 0x1B;
    ((DisplayActor44 *)D_8011ECD0)[122].unk_26 = 0x88;
    ((DisplayActor44 *)D_8011ECD0)[122].unk_2A = 0x18;
    ((DisplayActor44 *)D_8011ECD0)[122].unk_03 = 0;
    ((DisplayActor44 *)D_8011ECD0)[123].unk_18 = 8;
    ((DisplayActor44 *)D_8011ECD0)[123].unk_26 = 0x88;
    ((DisplayActor44 *)D_8011ECD0)[123].unk_2A = -0x28;
    ((DisplayActor44 *)D_8011ECD0)[123].unk_03 = 0;
    ((DisplayActor44 *)D_8011ECD0)[124].unk_18 = 8;
    ((DisplayActor44 *)D_8011ECD0)[124].unk_26 = 0x88;
    ((DisplayActor44 *)D_8011ECD0)[124].unk_2A = 0x18;
    ((DisplayActor44 *)D_8011ECD0)[124].unk_03 = 0;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80132A38);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80132B78);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80132E80);

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
    func_8004E788(-0x78, 0x38, 0xA, D_8014CD28, 0);
    func_8004E788(-0x18, 0x38, 0xA, D_8014CD2C, 0);
    func_8004E788(0x48, 0x38, 0xA, D_8014CD30, 0);
    func_8004E788(-0x78, 0x48, 0xA, D_8014CD34, 0);
    func_8004E788(-0x18, 0x48, 0xA, D_8014CD38, 0);
    func_8004E788(0x48, 0x48, 0xA, D_8014CD3C, 0);
    func_8004E788(-0x78, 0x58, 0xA, D_8014CD40, 0);
    func_8004E788(-0x18, 0x58, 0xA, "やゆよ　わ", 0);
    func_8004E788(0x48, 0x58, 0xA, D_8014CD48, 0);
}

void func_801330E8(s32 arg0) {
    func_8004E58C();
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
    func_8004E58C();
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

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80133510);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801338DC);

void func_80133D94(void) {
    s32 x;
    s32 y;

    y = D_8011ECFA;
    x = D_8011ECF6;
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

    x = D_8011ECF6;
    y = D_8011ECFA;
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
            D_8011ECD0[i * 0x44 + 0x1E4B] |= 0x80;
        } else {
            D_8011ECD0[i * 0x44 + 0x1E4B] &= 0x7F;
        }
    }
    if (D_8014CCCC >= 0) {
        D_80120C96 = (D_8014CCCC << 5) - 0x28;
        return;
    }
    D_80120C96 = -0xB4;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80134110);

void func_801341A8(void) {
    s32 prev;

    prev = D_8014CCB4;
    if ((D_800E6280.unk_F88 & 0x20) && D_8014CCB8 != -1 && D_8014CCBC != -1) {
        D_8014CCB4 = D_8014CCBC * 15 + D_8014CCB8 % 6 + D_8014CCB8 / 6 * 5;
        if (D_8014CCB4 != prev) {
            D_8014CCAC = 0;
        }
    }
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-10890, character-page selection. */
void func_80134248(s32 arg0) {
    s32 selected;
    u32 index;
    s32 column;
    u8 *dest;

    selected = D_8014CC70;
    switch (arg0) {
    case 0:
        if ((s32)D_8014CC70 >= 3) {
            selected = 2;
        }
        break;
    case 1:
        selected = D_8014CC70 - 3;
        if (selected >= 3) {
            selected = 2;
        }
        break;
    case 2:
        selected = D_8014CC70 - 6;
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
        column = (s32)D_8014CCC0 % 6;
        if (column == 5) {
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
            dest[selected * 2] = ((NameKanaPages44 *)&D_8014CD28)->text[D_8014CCC4 * 3 + (s32)D_8014CCC0 / 6][column * 2];
            dest[selected * 2 + 1] = ((NameKanaPages44 *)&D_8014CD28)->text[D_8014CCC4 * 3 + (s32)D_8014CCC0 / 6][((s32)D_8014CCC0 % 6) * 2 + 1];
        }
        break;
    case 1:
        column = (s32)D_8014CCC0 % 6;
        if (column == 5) {
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
            dest[selected * 2] = ((NameKanaPages44 *)&D_8014CD6C)->text[D_8014CCC4 * 3 + (s32)D_8014CCC0 / 6][column * 2];
            dest[selected * 2 + 1] = ((NameKanaPages44 *)&D_8014CD6C)->text[D_8014CCC4 * 3 + (s32)D_8014CCC0 / 6][((s32)D_8014CCC0 % 6) * 2 + 1];
        }
        break;
    case 2:
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
                if (D_8014CCC0 < func_800AE0E0(D_8014CDB0[D_8014CCB4]) / 2) {
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

#else
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80134248);
#endif

void func_80134B18(void) {
    k_sub_reset();
    set_kanji_string(-0x68, -0x58, 0xE, D_8014CC74, 0);
    set_kanji_string(-0x30, -0x58, 0xE, D_8014CC7C, 0);
    set_kanji_string(0x48, -0x58, 0xE, D_8014CC84, 0);
    k_sub_disp_start(3);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80134B94);

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
    if (D_800E6280.unk_F80 & 0x100) {
        switch (D_800E6280.unk_F88) {
        case 4:
            func_80132000(0xA);
            func_80134B18();
            return;
        case 1:
            func_80132000(1);
            func_80134B18();
            return;
        case 8:
            func_80132000(2);
            func_80134B18();
            return;
        case 2:
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

/* NON_MATCHING: T-11010, 19 differing lines: three loop pointer setups scheduled after the lui/addiu pair (as1 order), the constant 2 stored to D_8014D0A4 comes from $t6 where the original reuses the promoted $v0 */
#ifdef NON_MATCHING
void func_80135168(void) {
    u8 *p;

    if ((u8) D_8014D0A4 < 2) {
        if ((u8) D_8014D0A4 == 1) {
            func_800AE0A0(D_8014CC74, D_8014D070, 8);
            func_800AE0A0(D_8014CC7C, D_8014D078, 8);
            func_800AE0A0(D_8014CC84, D_8014D080, 0xC);
            D_8014CC90 = (u8) D_8014D08C % 12;
            D_8014CC94 = (u8) D_8014D090 % 31;
            D_8014CC98 = (u8) D_8014D094 & 3;
            D_8014CC9C = (u8) D_8014D098 % 12;
            D_8014CCA0 = (u8) D_8014D09C % 31;
            D_8014CCA4 = (u8) D_8014D0A0 & 3;
        } else {
            p = D_8014CC74;
            do {
                func_800AE0F0(p, D_800B3644);
                p += 2;
            } while (p < &D_8014CC7A);
            p = D_8014CC7C;
            do {
                func_800AE0F0(p, D_800B3644);
                p += 2;
            } while (p < &D_8014CC82);
            p = D_8014CC84;
            do {
                func_800AE0F0(p, D_800B3644);
                p += 2;
            } while (p != &D_8014CC8E);
            D_8014CC8E = 0;
            D_8014CC83 = 0;
            D_8014CC82 = 0;
            D_8014CC7B = 0;
            D_8014CC7A = 0;
            D_8014CC90 = 0xFF;
            D_8014CC94 = 0xFF;
            D_8014CC98 = 0xFF;
            D_8014CC9C = 0xFF;
            D_8014CCA0 = 0xFF;
            D_8014CCA4 = 0xFF;
            D_8014CC8F = 0;
            func_80134F6C(p);
        }
    }
    D_8014CC70 = 0;
    D_8014CCC8 = 0;
    func_801330E8(D_8014CCE4);
    D_8014CCB4 = 0;
    D_8014CCB8 = -1;
    D_8014CCBC = -1;
    func_801324AC();
    func_80046290(D_8014CC38, D_8014CC3C, 0);
    func_80044750(0x300);
    func_8004EA98();
    func_80134B18();
    D_8014D0A4 = 2;
    func_8004284C();
}
#else
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80135168);
#endif

void func_80135434(s32 arg0) {
    s32 col;

    if (arg0 != 0) {
        col = 0x90002;
    } else {
        col = 0x8081F;
    }
    func_80049A40(-0x9D, -0x30, 0x1B, 0xF, 4, col, 0x83);
    func_80049A40(-0x9D, -0x10, 0x1B, 0xF, 4, col, 0x83);
    func_80049A40(-0x9D, 0x10, 0x1B, 0xF, 4, col, 0x83);
    func_80049A40(-0x9D, 0x30, 0x1B, 0xF, 4, col, 0x83);
    func_80049A40(-0x9D, 0x50, 0x1B, 0xF, 4, col, 0x83);
    func_80049A40(-0x9A, -0x6A, 0x130, 0x20, 4, col, 0x83);
    func_80049A40(-0x7F, -0x40, 0x11D, 0x70, 4, col, 0x83);
    if (D_8014CCC8 == 3) {
        func_80049A40(-0x7A, 0x36, 0x50, 0x30, 4, col, 0x83);
        func_80049A40(-0x1A, 0x36, 0x50, 0x30, 4, col, 0x83);
        func_80049A40(0x46, 0x36, 0x50, 0x30, 4, col, 0x83);
    }
    func_800494BC(0, 0, 4, 0, 0);
    if (!(D_800E6280.unk_F88 & 0x20) && !(D_800E6280.unk_F88 & 0x800)) {
        if (!(D_800E6280.unk_F88 & 0x40)) {
            func_80134F6C();
        }
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80135650);

/* NON_MATCHING: T-11010, 23 differing lines: D_8014CC70 takes $v1 where the original has $v0 (equal priority with the unk_F88 selector, regdiff: first-use order), constant 2 then moves with it */
#ifdef NON_MATCHING
void func_801357F4(void) {
    u8 cur;

    func_8006BA40();
    func_80133D94();
    func_80133E54();
    func_80133FC4();
    func_80132B78(0);
    if (D_8014CCC8 == 3) {
        func_80134110();
        func_801341A8();
    }
    func_80135650();
    if (D_800E6280.unk_F88 & 0x40) {
        cur = D_8014CC70;
        if (cur == 0xC) {
            D_8014CC70 = 2;
        } else if (cur != 0) {
            D_8014CC70 = cur - 1;
        }
    } else if (D_800E6280.unk_F88 & 0x20) {
        if (D_8014CC70 < 3 && D_8014CCC0 >= 0 && D_8014CCC4 >= 0) {
            func_80134248(0);
            func_80134B18();
        }
        if (D_8014CCC8 == 3) {
            func_801338DC(1);
            func_8004EA98();
            func_80134B18();
        } else if (D_8014CCC8 == 2) {
            func_80133510(D_8014CCE4);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 0) {
            D_8014CCC8 = 0;
            func_801330E8(D_8014CCE4);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 1) {
            D_8014CCC8 = 1;
            func_801332FC(D_8014CCE4);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 2) {
            D_8014CCC8 = 2;
            func_80133510(D_8014CCE4);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 3) {
            D_8014CCC8 = 3;
            func_801338DC(1);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 4) {
            D_8014CCC8 = 4;
            func_80134B94(0);
            func_80134B18();
            func_80044750(0x501);
            func_8004284C();
        }
    } else if (D_800E6280.unk_F88 & 0x800) {
        func_80134B94(0);
        func_80134B18();
        func_80044750(0x501);
        func_8004284C();
    }
}
#else
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801357F4);
#endif

void func_80135A98(void) {

    func_8004E58C();
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
    func_80046290(D_8014CC40, D_8014CC44, 0);
    func_80044750(0x300);
    k_sub_reset_point_set();
    func_80134B18();
    func_8004284C();
}

void func_80135BB4(void) {
    u8 cur;
    u8 temp;

    func_8006BA40();
    func_80133D94();
    func_80133E54();
    func_80133FC4();
    func_80132B78(1);
    if (D_8014CCC8 == 3) {
        func_80134110();
        func_801341A8();
    }
    func_80135650();
    if (D_800E6280.unk_F88 & 0x40) {
        cur = (*(u16 *)&D_8014CC70);
        if (cur == 0xC) {
            cur = 6;
        }
        D_8014CC70 = cur;
        if (cur >= 4) {
            D_8014CC70 = cur - 1;
        }
    } else if (D_800E6280.unk_F88 & 0x20) {
        temp = D_8014CC70;
        if (temp < 6 && D_8014CCC0 >= 0 && D_8014CCC4 >= 0) {
            func_80134248(1);
            func_80134B18();
        }
        if (D_8014CCC8 == 3) {
            func_801338DC(2);
            func_8004EA98();
            func_80134B18();
        } else if (D_8014CCC8 == 2) {
            func_80133510(D_8014CD04);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 0) {
            D_8014CCC8 = 0;
            func_801330E8(D_8014CD04);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 1) {
            D_8014CCC8 = 1;
            func_801332FC(D_8014CD04);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 2) {
            D_8014CCC8 = 2;
            func_80133510(D_8014CD04);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 3) {
            D_8014CCC8 = 3;
            func_801338DC(2);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 4) {
            D_8014CCC8 = 4;
            func_80134B94(1);
            func_80134B18();
            func_80044750(0x501);
            func_8004284C();
        }
    } else if (D_800E6280.unk_F88 & 0x800) {
        func_80134B94(1);
        func_80134B18();
        func_80044750(0x501);
        func_8004284C();
    }
}

void func_80135E58(void) {

    func_8004E58C();
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
    func_80046290(D_8014CC48, D_8014CC4C, 0);
    func_80044750(0x300);
    k_sub_reset_point_set();
    func_80134B18();
    func_8004284C();
}

/* NON_MATCHING: T-11010, 23 differing lines: same as func_80135BB4, D_8014CC70 is promoted into $v1 where the original keeps $v0 for it */
#ifdef NON_MATCHING
void func_80135F74(void) {
    u8 cur;

    func_8006BA40();
    func_80133D94();
    func_80133E54();
    func_80133FC4();
    func_80132B78(2);
    if (D_8014CCC8 == 3) {
        func_80134110();
        func_801341A8();
    }
    func_80135650();
    if (D_800E6280.unk_F88 & 0x40) {
        cur = D_8014CC70;
        if (cur == 0xC) {
            D_8014CC70 = 0xA;
        } else if (cur >= 7) {
            D_8014CC70 = cur - 1;
        }
    } else if (D_800E6280.unk_F88 & 0x20) {
        if (D_8014CC70 < 0xB && D_8014CCC0 >= 0 && D_8014CCC4 >= 0) {
            func_80134248(2);
            func_80134B18();
        }
        if (D_8014CCC8 == 3) {
            func_801338DC(3);
            func_8004EA98();
            func_80134B18();
        } else if (D_8014CCC8 == 2) {
            func_80133510(D_8014CD10);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 0) {
            D_8014CCC8 = 0;
            func_801330E8(D_8014CD10);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 1) {
            D_8014CCC8 = 1;
            func_801332FC(D_8014CD10);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 2) {
            D_8014CCC8 = 2;
            func_80133510(D_8014CD10);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 3) {
            D_8014CCC8 = 3;
            func_801338DC(3);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 4) {
            D_8014CCC8 = 4;
            func_80134B94(2);
            func_80134B18();
            func_80044750(0x501);
            func_8004284C();
        }
    } else if (D_800E6280.unk_F88 & 0x800) {
        func_80134B94(2);
        func_80134B18();
        func_80044750(0x501);
        func_8004284C();
    }
}
#else
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80135F74);
#endif

void func_8013621C(s32 arg0) {
    s32 color;

    if (arg0 != 0) {
        color = 0x90002;
    } else {
        color = 0x8081F;
    }
    func_80049A40(-0x9A, -0x6A, 0x130, 0x20, 4, color, 0x83);
    func_80049A40(-0x7C, -0x3D, 0x118, 0x9E, 4, color, 0x83);
    func_80049A40(-0x9D, 0x50, 0x1C, 0x10, 4, color, 0x83);
}

void func_801362D0(void) {
    func_8004E58C();
    func_8013635C();
    D_8014CC70 = 0;
    func_8004E788(-0x98, -0x68, 0xA, "誕生日と血液型も教えてね？", 0);
    func_80046290(D_8014CC50, D_8014CC54, 0);
    func_80044750(0x300);
    func_8004EA98();
    func_801372A0(0);
    func_8004EAD4(0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_8013635C);

void func_8013664C(s32 arg0) {
    u8 *p = D_8011ECD0 + arg0 * 0x44;

    if (!(p[2] & 1)) {
        *(s16 *)(p + 0x18) = 0;
        p[2] = 1;
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80136688);

void func_80136808(s32 arg0) {
    s32 i;
    s32 x;
    s32 mouse_x;
    s32 y;

    if (D_8014CC70 == 0) {
        mouse_x = ((DisplayActor44 *)D_8011ECD0)[0].unk_26;
        ((DisplayActor44 *)D_8011ECD0)[104].unk_18 = 5;
        ((DisplayActor44 *)D_8011ECD0)[104].unk_26 = -256;
        ((DisplayActor44 *)D_8011ECD0)[104].unk_2A = -256;
        for (i = 0; i < 12; i++) {
            x = (i % 6) * 40 - 104;
            y = (i / 6) * 16 - 40;
            if (x < mouse_x && mouse_x < x + 32 &&
                y < ((DisplayActor44 *)D_8011ECD0)[0].unk_2A && ((DisplayActor44 *)D_8011ECD0)[0].unk_2A < y + 16) {
                ((DisplayActor44 *)D_8011ECD0)[104].unk_26 = x + 22;
                ((DisplayActor44 *)D_8011ECD0)[104].unk_2A = y + 6;
                if (arg0 != 0) {
                    D_8014CC9C = i;
                } else {
                    D_8014CC90 = i;
                }
            }
        }
    } else if (D_8014CC70 == 1) {
        mouse_x = ((DisplayActor44 *)D_8011ECD0)[0].unk_26;
        ((DisplayActor44 *)D_8011ECD0)[104].unk_18 = 4;
        ((DisplayActor44 *)D_8011ECD0)[104].unk_26 = -256;
        ((DisplayActor44 *)D_8011ECD0)[104].unk_2A = -256;
        for (i = 0; i < 31; i++) {
            x = (i % 11) * 24 - 104;
            y = (i / 11) * 16 + 8;
            if (x - 16 < mouse_x && mouse_x < x + 16 &&
                y < ((DisplayActor44 *)D_8011ECD0)[0].unk_2A && ((DisplayActor44 *)D_8011ECD0)[0].unk_2A < y + 16) {
                ((DisplayActor44 *)D_8011ECD0)[104].unk_26 = x + 1;
                ((DisplayActor44 *)D_8011ECD0)[104].unk_2A = y + 6;
                if (arg0 != 0) {
                    D_8014CCA0 = i;
                } else {
                    D_8014CC94 = i;
                }
            }
        }
    } else if (D_8014CC70 == 2) {
        mouse_x = ((DisplayActor44 *)D_8011ECD0)[0].unk_26;
        ((DisplayActor44 *)D_8011ECD0)[104].unk_18 = 5;
        ((DisplayActor44 *)D_8011ECD0)[104].unk_26 = -256;
        ((DisplayActor44 *)D_8011ECD0)[104].unk_2A = -256;
        for (i = 0; i < 4; i++) {
            x = i * 64 - 96;
            if (x < mouse_x && mouse_x < x + 32 &&
                72 < ((DisplayActor44 *)D_8011ECD0)[0].unk_2A && ((DisplayActor44 *)D_8011ECD0)[0].unk_2A < 88) {
                ((DisplayActor44 *)D_8011ECD0)[104].unk_26 = x + 16;
                ((DisplayActor44 *)D_8011ECD0)[104].unk_2A = 78;
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
    func_8004EAAC();
    if (a == 0xFF) {
        func_8004E788(-0x50, -0x58, 0xE, "　", 0);
        func_8004E788(-0x40, -0x58, 0xE, "　", 0);
    } else {
        if (a >= 9) {
            func_8004E788(-0x50, -0x58, 0xE, D_800B3288[1], 0);
        } else {
            func_8004E788(-0x50, -0x58, 0xE, "　", 0);
        }
        func_8004E788(-0x40, -0x58, 0xE, D_800B3288[(a + 1) % 10], 0);
    }
    if (b == 0xFF) {
        func_8004E788(-0x20, -0x58, 0xE, "　", 0);
        func_8004E788(-0x10, -0x58, 0xE, "　", 0);
    } else {
        if (b >= 9) {
            func_8004E788(-0x20, -0x58, 0xE, D_800B3288[(b + 1) / 10], 0);
        } else {
            func_8004E788(-0x20, -0x58, 0xE, D_800B3644, 0);
        }
        func_8004E788(-0x10, -0x58, 0xE, D_800B3288[(b + 1) % 10], 0);
    }
    switch (c) {
    case 0:
        func_8004E788(0x60, -0x58, 0xE, "　", 0);
        func_8004E788(0x70, -0x58, 0xE, "Ａ", 0);
        break;
    case 1:
        func_8004E788(0x60, -0x58, 0xE, "　", 0);
        func_8004E788(0x70, -0x58, 0xE, "Ｂ", 0);
        break;
    case 2:
        func_8004E788(0x60, -0x58, 0xE, "Ａ", 0);
        func_8004E788(0x70, -0x58, 0xE, "Ｂ", 0);
        break;
    case 3:
        func_8004E788(0x60, -0x58, 0xE, "　", 0);
        func_8004E788(0x70, -0x58, 0xE, "Ｏ", 0);
        break;
    }
    func_8004EAD4(6);
}

void func_80137608(s32 arg0) {
    s32 pad; /* FAKE: unused locals around the two cursor values, which the original reads without ever assigning them (T-3330 layout) */
    s32 sp30;
    s32 sp2C;
    s32 pad2;

    if (sp30 >= -0x9D && sp30 < -0x80 && sp2C >= 0x51 && sp2C < 0x60) {
        D_8012093F |= 0x80;
        if (D_800E6280.unk_F88 & 0x20) {
            func_801377E8();
            func_8004284C();
        }
    } else {
        D_8012093F &= 0x7F;
    }
    if (D_8014CC70 == 3) {
        D_801208FB |= 0x80;
        if (D_800E6280.unk_F88 & 0x20) {
            func_801377E8(arg0);
            func_8004284C();
        }
    } else {
        D_801208FB &= 0x7F;
    }
}

void func_80137708(void) {
    u8 temp;
    func_8006BA40();
    func_80136688();
    func_801372A0(0);
    func_80137608(0);
    if (D_800E6280.unk_F88 & 0x40) {
        temp = D_8014CC70;
        if (temp != 0) {
            D_8014CC70 -= 1;
        }
    } else if (D_800E6280.unk_F88 & 0x20) {
        D_8014CC70 += 1;
        func_801372A0(0);
        if ((u8) D_8014CC70 >= 3U) {
            func_801377E8(0);
            func_8004284C();
        }
    } else if (D_800E6280.unk_F88 & 0x800) {
        func_801377E8(0);
        func_8004284C();
    }
    func_80137AA0();
}

/* NON_MATCHING: T-11010, 40 differing lines, all register choice for the hi constant of the D_8014CC9x loads ($a0 in the draft, $v1 in the original); control flow, frame and stores match */
#ifdef NON_MATCHING
void func_801377E8(arg0)
s32 arg0;
{
    s32 a;
    u8 b;

    if (arg0 != 0) {
        if (D_8014CC9C == 0xFF) {
            D_8014CC9C = func_800AE0D0() % 12;
        }
        b = D_8014CCA0;
        if (b == 0xFF) {
            b = func_800AE0D0() % 31;
        }
        if (D_8014CCA4 == 0xFF) {
            D_8014CCA0 = b;
            D_8014CCA4 = func_800AE0D0() % 3;
            b = D_8014CCA0;
        }
        a = D_8014CC9C + 1;
        if (a == 2) {
            if (b + 1 >= 0x1D) {
                b = 0x1B;
            }
        }
        if (a == 4) {
            if (b + 1 >= 0x1F) {
                b = 0x1D;
            }
        }
        if (a == 6) {
            if (b + 1 >= 0x1F) {
                b = 0x1D;
            }
        }
        if (a == 9 && b + 1 >= 0x1F) {
            b = 0x1D;
        }
        D_8014CCA0 = b;
        if (a == 0xB) {
            D_8014CCA0 = b;
            if (b + 1 >= 0x1F) {
                D_8014CCA0 = 0x1D;
            }
        }
    } else {
        if (D_8014CC90 == 0xFF) {
            D_8014CC90 = func_800AE0D0() % 12;
        }
        b = D_8014CC94;
        if (b == 0xFF) {
            b = func_800AE0D0() % 31;
        }
        if (D_8014CC98 == 0xFF) {
            D_8014CC94 = b;
            D_8014CC98 = func_800AE0D0() % 3;
            b = D_8014CC94;
        }
        a = D_8014CC90 + 1;
        if (a == 2) {
            if (b + 1 >= 0x1D) {
                b = 0x1B;
            }
        }
        if (a == 4) {
            if (b + 1 >= 0x1F) {
                b = 0x1D;
            }
        }
        if (a == 6) {
            if (b + 1 >= 0x1F) {
                b = 0x1D;
            }
        }
        if (a == 9 && b + 1 >= 0x1F) {
            b = 0x1D;
        }
        D_8014CC94 = b;
        if (a == 0xB) {
            D_8014CC94 = b;
            if (b + 1 >= 0x1F) {
                D_8014CC94 = 0x1D;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_801377E8);
#endif

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
    func_800493A8(0x140, 0, 2, 0, 0);
}

void func_80137B84(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;
    u8 *q;

    p = func_800490F0(0x14, D_8011ECA0);
    func_8009F0A4(p);
    func_8009EF84(p, 0);
    p[4] = D_800E6280.unk_03A;
    p[5] = D_800E6280.unk_03A;
    p[6] = D_800E6280.unk_03A;
    *(s16 *)(p + 8) = D_800E6280.unk_014[D_8011ECA0] + arg1;
    *(s16 *)(p + 0xA) = D_800E6280.unk_018[D_8011ECA0] + arg2;
    q = D_8014D028 + arg0 * 2;
    p[0xC] = q[0];
    p[0xD] = q[1];
    *(s16 *)(p + 0x12) = 0x10;
    *(s16 *)(p + 0x10) = 0x10;
    *(s16 *)(p + 0xE) = func_8009ED74(0x10, 0x1F0);
    func_8009EED0((D_8011ECA0 << 10) + 8 + D_800E8CA0, p);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80137CA0);

void func_80137EE4(void) {
    u8 *p;
    s32 i;

    func_80048E78();
    for (i = 0; i < 2; i++) {
        p = D_8011ECD0 + i * 0x44;
        p[0x1981] = 2;
        p[0x1982] = 0x40;
        *(s32 *)(p + 0x19B8) = 0;
        p[0x1983] = 0x84;
        *(s32 *)(p + 0x198C) = D_800E6280.unk_1A4C;
        *(s32 *)(p + 0x1990) = D_800E6280.unk_1A5C;
        *(s32 *)(p + 0x19B4) = D_800E6280.unk_1A3C;
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
    func_8004E58C();
    func_80137EE4();
    func_8004E788(-0x18, 0x40, 0xA, "これでいい？", 0);
    func_8004E788(-0x38, -0x37, 0xA, D_8014CC74, 0);
    func_8004E788(0, -0x37, 0xA, D_8014CC7C, 0);
    func_8004E788(-0x38, -0x17, 0xA, D_8014CC84, 0);
    if (D_8014CC90 + 1 >= 10) {
        func_8004E788(-0x38, 8, 0xA, D_800B3288[(D_8014CC90 + 1) / 10], 0);
    }
    func_8004E788(-0x28, 8, 0xA, D_800B3288[(D_8014CC90 + 1) % 10], 0);
    if (D_8014CC94 + 1 >= 10) {
        func_8004E788(-8, 8, 0xA, D_800B3288[(D_8014CC94 + 1) / 10], 0);
    }
    func_8004E788(8, 8, 0xA, D_800B3288[(D_8014CC94 + 1) % 10], 0);
    switch (D_8014CC98) {
    case 0:
        func_8004E788(-0x28, 0x28, 0xA, "Ａ", 0);
        break;
    case 1:
        func_8004E788(-0x28, 0x28, 0xA, "Ｂ", 0);
        break;
    case 2:
        func_8004E788(-0x38, 0x28, 0xA, "ＡＢ", 0);
        break;
    case 3:
        func_8004E788(-0x28, 0x28, 0xA, "Ｏ", 0);
        break;
    }
    func_80046290(D_8014CC60, D_8014CC64, 0);
    func_80044750(0x300);
    func_8004EA98();
    func_8004EAD4(0);
    func_8004284C();
}

void func_80138268(s32 arg0) {
    s32 color;

    if (arg0 != 0) {
        color = 0x90002;
    } else {
        color = 0x8081F;
    }
    if (arg0 != 0) {
        func_80049A40(-0x7E, -0x3D, 0x11A, 0x58, 4, color, 0x83);
    } else {
        func_80049A40(-0x7E, -0x3D, 0x11A, 0x76, 4, color, 0x83);
    }
    func_80049A40(-0x28, 0x3B, 0x7C, 0x26, 4, color, 0x83);
}

void func_80138330(void) {
}

void func_80138338(void) {
    s32 pad; /* FAKE: unused local, the original frame has the spill slots 4 bytes higher (T-3330) */
    s32 x;
    s32 y;

    x = D_8011ECF6;
    y = D_8011ECFA;
    func_8006BA40();
    if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
        D_801206BA = -0xA;
    }
    if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
        D_801206BA = 0x2E;
    }
    if (D_800E6280.unk_F88 & 0x800) {
        func_80044750(0x501);
        func_80042808();
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
    } else if (D_800E6280.unk_F88 & 0x20) {
        if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
            func_80044750(0x501);
            func_80042808();
        }
        if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
            func_80042940(0);
        }
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7384;
extern u8 D_800E738A;
void func_801385A8(void);
void func_80138634(void);
void func_801387F4(void);
void func_80138A7C(void);
void func_80138BCC(void);
/* end bulk_m2c declarations */
void func_80138488(void) {
    u8 temp_v0;

    temp_v0 = D_800E738A;
    D_800E7384 += 1;
    switch (temp_v0) {
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
    if (temp_v0 < 4U) {
        if (temp_v0 < 2U) {
            func_8013621C(1);
            func_80136808(1);
            return;
        }
        func_80138268(1);
        func_80138330();
    }
}

void func_801385A8(void) {
    func_8004E58C();
    D_8014CC70 = 0;
    func_8004E788(-0x98, -0x68, 0xA, "藤崎詩織の誕生日と血液型は覚えてる？", 0);
    func_8013635C();
    func_80046290(D_8014CC58, D_8014CC5C, 0);
    func_80044750(0x300);
    func_8004EA98();
    func_801372A0(1);
    func_8004EAD4(0);
    func_8004284C();
}

void func_80138634(void) {
    u8 temp;
    func_8006BA40();
    func_80136688();
    func_801372A0(1);
    func_80137608(1);
    if (D_800E6280.unk_F88 & 0x40) {
        temp = D_8014CC70;
        if (temp != 0) {
            D_8014CC70 -= 1;
        }
    } else if (D_800E6280.unk_F88 & 0x20) {
        D_8014CC70 += 1;
        func_801372A0(1);
        if ((u8) D_8014CC70 >= 3U) {
            func_801377E8(1);
            func_8004284C();
        }
    } else if (D_800E6280.unk_F88 & 0x800) {
        func_801377E8(1);
        func_8004284C();
    }
    func_80137AA0();
}

void func_80138714(void) {
    u8 *p;
    s32 i;

    func_80048E78();
    for (i = 0; i < 2; i++) {
        p = D_8011ECD0 + i * 0x44;
        p[0x1981] = 2;
        p[0x1982] = 0x40;
        *(s32 *)(p + 0x19B8) = 0;
        p[0x1983] = 0x84;
        *(s32 *)(p + 0x198C) = D_800E6280.unk_1A4C;
        *(s32 *)(p + 0x1990) = D_800E6280.unk_1A5C;
        *(s32 *)(p + 0x19B4) = D_800E6280.unk_1A3C;
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
    func_8004E58C();
    func_8004E788(-0x18, 0x40, 0xA, "これでいい？", 0);
    func_8004E788(-0x38, -0x38, 0xA, "藤崎", 0);
    func_8004E788(0, -0x38, 0xA, "詩織", 0);
    if (D_8014CC9C + 1 >= 10) {
        func_8004E788(-0x38, -0x18, 0xA, D_800B3288[(D_8014CC9C + 1) / 10], 0);
    }
    func_8004E788(-0x28, -0x18, 0xA, D_800B3288[(D_8014CC9C + 1) % 10], 0);
    if (D_8014CCA0 + 1 >= 10) {
        func_8004E788(-8, -0x18, 0xA, D_800B3288[(D_8014CCA0 + 1) / 10], 0);
    }
    func_8004E788(8, -0x18, 0xA, D_800B3288[(D_8014CCA0 + 1) % 10], 0);
    switch (D_8014CCA4) {
    case 0:
        func_8004E788(-0x28, 8, 0xA, "Ａ", 0);
        break;
    case 1:
        func_8004E788(-0x28, 8, 0xA, "Ｂ", 0);
        break;
    case 2:
        func_8004E788(-0x38, 8, 0xA, "ＡＢ", 0);
        break;
    case 3:
        func_8004E788(-0x28, 8, 0xA, "Ｏ", 0);
        break;
    }
    func_80138714();
    func_80046290(D_8014CC60, D_8014CC64, 0);
    func_80044750(0x300);
    func_8004EA98();
    func_8004EAD4(0);
    func_8004284C();
}

void func_80138A7C(void) {
    s32 pad; /* FAKE: unused local, the original frame has the spill slots 4 bytes higher (T-3330) */
    s32 x;
    s32 y;

    x = D_8011ECF6;
    y = D_8011ECFA;
    func_8006BA40();
    if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
        D_801206BA = -0xA;
    }
    if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
        D_801206BA = 0x2E;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        if (x >= -0x2F && x < 0x10 && y >= 0x51 && y < 0x60) {
            func_80044750(0x501);
            func_8004284C();
        }
        if (x >= 0x11 && x < 0x50 && y >= 0x51 && y < 0x60) {
            func_80042940(0);
        }
    } else if (D_800E6280.unk_F88 & 0x800) {
        func_80044750(0x501);
        func_8004284C();
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
    }
}

void func_80138BCC(void) {
    func_80044750(5);
    func_80044750(0xC1);
    func_80044750(3);
    func_80048E78();
    func_80138F60();
    func_8004E58C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80138C1C);

void func_80138F00(void) {
    D_800E6280.unk_1A3C = 0x801AAF58;
    D_800E6280.unk_1A4C = 0x801AAF5C;
    D_800E6280.unk_1A5C = 0x801AAFD0;
    D_800E6280.unk_1A6C = 0x80190000;
    D_800E6280.unk_1A84 = 0x801AA200;
    D_800E6280.unk_1A7C = 0x80192200;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80138F60);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80139440);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80132000", func_80139684);
