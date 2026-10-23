#define MAIN_API_OVERRIDE_func_8004AC18 /* T-25604 experiment: implicit int */
/* Scratch declarations for the NON_MATCHING compile: same-file callees defined after
 * func_80143580 need prototypes before it. T-25131, T-25319. */
void func_80144654(void);
void func_8014472C(void);
void func_80144908(void);
/* col2sepia is defined K&R as void in src/main/8004AD60.c; without a declaration it is an
 * implicit int here. T-25604 */
void col2sepia();
/* NON_MATCHING: T-25604 fresh-start round 1 digest, 122 funcdiff rows, Levenshtein score 120,
 * 1077 of 1077 words (T-25319: 293). Clean-room lens body (compare statuses in index, index
 * fill loops on address, case 9 chain order, actor index i in cases 11/16, copy loop on i)
 * plus the structure lens's col2sepia declaration and case 17 order. Needs s32
 * func_80143498(void). Left: the fade value and the fill end constant in a2 for a1, 0x80000000
 * in v0 for t1, the entry fill registers, the case 9 store order. See
 * drafts/T-25604/NAME_ENT/func_80143580/BRIEF-r1.md.
 */
void func_80143580(void) {
    s32 index;
    s32 tiles[8];
    s32 address;
    s32 i;
    u8 step;

    /* FAKE: T-25319: a reference to unk_F5F before the first branch satisfies the entry rule
     * of tools/cvt_pass.py, so the CVTs of the case 16/17 compares go and F5F is one promoted
     * range (v0) as in the target; uopt removes the self-store. */
    D_800E6280.unk_F5F = D_800E6280.unk_F5F;
    for (index = 0, address = 0x80180000; address < (s32)0x801A0000; address += 0x4000) {
        tiles[index++] = address;
    }
    step = D_800E6280.unk_110D;
    switch (step) {
    case 0:
        if ((((SpriteSlot24 *)&D_801220D0)[0].unk_10 = ((SpriteSlot24 *)&D_801220D0)[0].unk_11 = ((SpriteSlot24 *)&D_801220D0)[0].unk_12++) >= 0x81U) {
            ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x01000000;
            D_800E6280.unk_110D++;
            D_8014D170 = 0;
        }
        break;
    case 1:
        if ((D_800E6280.unk_10F8 & 3) == 3) {
            /* FAKE: T-25604: the compare status of the four x-- / x++ tests (cases 1, 3, 5, 8)
             * is held in index (clean-room lens, 176 -> 146 rows on its body); the original form
             * is unknown. */
            index = ((SpriteSlot24 *)&D_801220D0)[0].unk_04-- < -0xF0;
            if (index) {
                D_800E6280.unk_110D++;
                ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x51000000;
            }
        }
        break;
    case 2:
        if ((((SpriteSlot24 *)&D_801220D0)[0].unk_10 = ((SpriteSlot24 *)&D_801220D0)[0].unk_11 = ((SpriteSlot24 *)&D_801220D0)[0].unk_12--) != 0) {
            if ((((SpriteSlot24 *)&D_801220D0)[1].unk_10 = ((SpriteSlot24 *)&D_801220D0)[1].unk_11 = ((SpriteSlot24 *)&D_801220D0)[1].unk_12++) >= 0x81U) {
                ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x80000000;
                ((SpriteSlot24 *)&D_801220D0)[1].unk_00 = 0x01000000;
                D_800E6280.unk_110D++;
            }
        }
        /* fallthrough */
    case 3:
        if ((D_800E6280.unk_10F8 & 3) == 3) {
            ((SpriteSlot24 *)&D_801220D0)[0].unk_04 -= 1;
            index = ((SpriteSlot24 *)&D_801220D0)[1].unk_04++ >= -0xEF;
            if (index) {
                func_8004A1A0((s32) tiles, 0, 8, 4, 2, 0x1EF, -0xA0, -0x78, 8, 5);
                load_palette((s32 *)0x801A0000U, 0xF, 1, 3, 0);
                ((SpriteSlot24 *)&D_801220D0)[0].unk_11 = 0;
                ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x51000000;
                ((SpriteSlot24 *)&D_801220D0)[0].unk_12 = 0;
                ((SpriteSlot24 *)&D_801220D0)[0].unk_10 = 0;
                D_800E6280.unk_110D++;
            }
        }
        break;
    case 4:
        func_80143498();
        if ((((SpriteSlot24 *)&D_801220D0)[1].unk_10 = ((SpriteSlot24 *)&D_801220D0)[1].unk_11 = ((SpriteSlot24 *)&D_801220D0)[1].unk_12--) != 0) {
            if ((((SpriteSlot24 *)&D_801220D0)[0].unk_10 = ((SpriteSlot24 *)&D_801220D0)[0].unk_11 = ((SpriteSlot24 *)&D_801220D0)[0].unk_12++) >= 0x81U) {
                ((SpriteSlot24 *)&D_801220D0)[1].unk_00 = 0x80000000;
                ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x01000000;
                D_800E6280.unk_110D++;
            }
        }
        /* fallthrough */
    case 5:
        if ((D_800E6280.unk_10F8 & 3) == 3) {
            if (((SpriteSlot24 *)&D_801220D0)[1].unk_04 < -0xA0) {
                ((SpriteSlot24 *)&D_801220D0)[1].unk_04 += 1;
            }
            index = ((SpriteSlot24 *)&D_801220D0)[0].unk_04-- < -0x140;
            if (index) {
                ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x51000000;
                D_800E6280.unk_110D++;
            }
            /* FAKE: T-25319: ^ 0 drops the a0 argument preference of the promoted field; gives
             * the target's move a0,v0 and F8 in a0. */
            func_8004B19C(((SpriteSlot24 *)&D_801220D0)[0].unk_04 ^ 0, 0, 0);
            if (((SpriteSlot24 *)&D_801220D0)[0].unk_04 == -0x118) {
                func_8004E58C();
                set_kanji_string(-0x90, 0x14, 0, "そのたもとで卒業の日に、", 0);
                set_kanji_string(-0x90, 0x24, 0, "女の子からの告白で生まれた", 0);
                set_kanji_string(-0x90, 0x34, 0, "恋人達は、永遠に幸せな関係に", 0);
                set_kanji_string(-0x90, 0x44, 0, "なれるという伝説が…", 0);
                k_disp_start(4);
                k_disp_switch(0, 0);
                k_disp_switch(1, 0);
                k_disp_switch(2, 0);
                k_disp_switch(3, 0);
            }
        }
        break;
    case 6:
        if (((D_800E6280.unk_10F8 & 3) == 3) && (((SpriteSlot24 *)&D_801220D0)[0].unk_04 >= -0x13F)) {
            ((SpriteSlot24 *)&D_801220D0)[0].unk_04 -= 1;
        }
        if ((((SpriteSlot24 *)&D_801220D0)[0].unk_10 = ((SpriteSlot24 *)&D_801220D0)[0].unk_11 = ((SpriteSlot24 *)&D_801220D0)[0].unk_12--) == 0) {
            ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x80000000;
            /* FAKE: T-25131, T-25319 (from T-20008): the target has this nested fill loop
             * (the outer loop runs once); its original source form is unknown. */
            for (address = 0; address < 8; address++) {
                for (address = 0; address < 8; address++) {
                    tiles[address] = 0x801B0000 + address * 0x4000;
                }
            }
            func_8004A1A0((s32) tiles, 1, 6, 3, 2, 0x1EE, -0xA0, -0x78, 8, 0x15);
            load_palette((s32 *)0x801C8000U, 0xE, 1, 3, 0);
            ((SpriteSlot24 *)&D_801220D0)[1].unk_10 = ((SpriteSlot24 *)&D_801220D0)[1].unk_11 = ((SpriteSlot24 *)&D_801220D0)[1].unk_12 = 0;
            for (i = 0; i < 3; i++) {
                *(u32 *)&D_801217D0[i].unk_00 = 0x01000000;
                D_801217D0[i].unk_04 = i * 0x50 - 0xD0;
                D_801217D0[i].unk_06 = -0x78;
                D_801217D0[i].unk_08 = 0xFF;
                D_801217D0[i].unk_0A = 0xF0;
                D_801217D0[i].unk_0C = i * 2 + 9;
                D_801217D0[i].unk_12 = 0x1FF - i;
                D_801217D0[i].unk_1C = 0x1000;
                D_801217D0[i].unk_1E = 0x1000;
                D_800E6280.unk_10A5[i] = 7;
            }
            D_800E6280.unk_10A5[1] = 8;
            D_801217D0[1].unk_06 = -0x6E;
            D_800E6280.unk_110D++;
        }
        break;
    case 7:
        if ((D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = D_801217D0[2].unk_14 = D_801217D0[2].unk_15 = D_801217D0[2].unk_16++) >= 0x81U) {
            D_800E6280.unk_110D++;
        } else {
            ((SpriteSlot24 *)&D_801220D0)[1].unk_10 = ((SpriteSlot24 *)&D_801220D0)[1].unk_11 = ((SpriteSlot24 *)&D_801220D0)[1].unk_12++;
        }
        break;
    case 8:
        if ((D_800E6280.unk_10F8 & 3) == 3) {
            index = D_801217D0[0].unk_04-- < -0x160;
            if (index) {
                func_80044750(0xD1);
                func_8004E58C();
                cd_read_sectors(0x31U, 0x80180000U, 0xA017);
                ((SpriteSlot24 *)&D_801220D0)[0].unk_10 = 0;
                D_800E6280.unk_110D++;
            } else {
                D_801217D0[2].unk_04 += 1;
            }
        }
        break;
    case 9:
        if ((u8) ((SpriteSlot24 *)&D_801220D0)[0].unk_10 < 0xFEU) {
            func_8004AC18(((SpriteSlot24 *)&D_801220D0)[0].unk_10++);
        } else {
            func_8004AC18(0xFF);
            *(u32 *)&D_801217D0[1].unk_00 = 0x80000000;
            ((SpriteSlot24 *)&D_801220D0)[1].unk_00 = *(u32 *)&D_801217D0->unk_00 = *(u32 *)&D_801217D0[2].unk_00 = 0x80000000;
            ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x01000000;
            ((SpriteSlot24 *)&D_801220D0)[0].unk_11 = ((SpriteSlot24 *)&D_801220D0)[0].unk_12 = ((SpriteSlot24 *)&D_801220D0)[0].unk_10 = 0x80;
            ((SpriteSlot24 *)&D_801220D0)[1].unk_10 = 0xFF;
            D_800E6280.unk_110D++;
        }
        break;
    case 10:
        func_8004AC18(0xFF);
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) tiles, 0, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0xD);
            load_palette((s32 *)0x801C8000U, 0xF, 1, 3, 0);
            ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x01000000;
            cd_read_sectors(0xAEU, 0x80180000U, 0x9DF6);
            D_800E6280.unk_110D++;
        }
        break;
    case 11:
        func_8004AC18(0xFF);
        if (cd_read_sync_status() & 1) {
            tpage_buf_clear();
            i = 0;
            do {
                load_palette((s32 *)0x80180000U, (i + 0x10) & 0xFF, 1, 2, i);
                i += 1;
            } while (i < 3);
            col2sepia(0x80180000U, &D_800E6280.unk_163C[0], 8, 4, 0);
            load_palette(&D_800E6280.unk_163C[0], 0x10, 1, 2, 0);
            i = 0x60;
            func_80048F64(i);
            ((DisplayActor44 *)D_8011ECD0)[i].unk_01 = 5;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_02 = 4;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_38 = 0x41000000;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_03 = 0x84;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_0C = (u8 *)0x801A2624;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_10 = (u8 *)0x801A263C;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_34 = (u8 *)0x801A2610;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_16 = 0;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_04 = 8;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_05 = 0x80;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_26 = -0xA0;
            ((DisplayActor44 *)D_8011ECD0)[i].unk_2A = -0x78;
            D_800E6280.unk_1104.u = 0;
            D_800E6280.unk_110D++;
        }
        break;
    case 12:
        if (D_800E6280.unk_1104.u == 0xC8) {
            D_80120652 |= 1;
        }
        if (((SpriteSlot24 *)&D_801220D0)[1].unk_10 != 0) {
            func_8004AC18(((SpriteSlot24 *)&D_801220D0)[1].unk_10--);
        } else if ((u32) D_800E6280.unk_1104.u >= 0x12DU) {
            D_800E6280.unk_110D++;
        } else if (!(D_80120652 & 1)) {
            D_800E6280.unk_110D++;
        }
        break;
    case 13:
        /* FAKE: T-25131, T-25319 (from T-20008): nested fill loop as in case 6. */
        for (address = 0; address < 6; address++) {
            for (address = 0; address < 6; address++) {
                tiles[address] = 0x80186000 + address * 0x4000;
            }
        }
        func_8004A1A0((s32) tiles, 1, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0x15);
        ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x80000000;
        ((SpriteSlot24 *)&D_801220D0)[1].unk_00 = 0x01000000;
        load_palette((s32 *)0x8019E000U, 0xF, 1, 2, 0);
        col2sepia(0x8019E000U, &D_800E6280.unk_163C[0], 8, 4, 0);
        for (i = 0; i != 128; i++) {
            if (i < 32) {
                ((s32 *)D_800E6280.unk_183C)[i] = D_800E6280.unk_163C[i];
            } else {
                ((s32 *)D_800E6280.unk_183C)[i] = ((s32 *)0x8019E000)[i];
            }
        }
        load_palette((s32 *)D_800E6280.unk_183C, 0xF, 1, 2, 0);
        col2sepia(0x80180200U, &D_800E6280.unk_163C[0], 8, 4, 0);
        load_palette(&D_800E6280.unk_163C[0], 0x11, 1, 2, 0);
        func_80048E78();
        func_8004E58C();
        func_80046290(0x39D, 0x060003A9, 0xF);
        func_80044750(0x300);
        func_8004E58C();
        set_kanji_string(-0x90, 0x44, 0, "あなたが…、", 0);
        k_disp_start(1);
        k_disp_switch(0, 0);
        D_800E6280.unk_1104.u = 0;
        D_800E6280.unk_110D++;
        break;
    case 14:
        if (func_800460EC() & 4) {
            D_800E6280.unk_1104.u = 0;
            D_800E6280.unk_110D++;
        }
        break;
    case 15:
        func_8014472C();
        func_80144908();
        if (D_800E6280.unk_F88 & 0xFFFF) {
            D_800E6280.unk_110D++;
        } else if ((u32) D_800E6280.unk_1104.u >= 0x101U) {
            D_800E6280.unk_110D++;
        } else if ((func_800460EC() & 2) && ((u32) D_800E6280.unk_1104.u >= 0x79U)) {
            D_800E6280.unk_110D++;
        }
        break;
    case 16:
        /* FAKE: T-25131, T-25319 (from T-20008): nested fill loop as in case 6. */
        for (address = 0; address < 6; address++) {
            for (address = 0; address < 6; address++) {
                tiles[address] = 0x801BA7F0 + address * 0x4000;
            }
        }
        func_8004A1A0((s32) tiles, 0, 6, 3, 2, 0x1EE, -0xA0, -0x78, 8, 0x15);
        ((SpriteSlot24 *)&D_801220D0)[1].unk_00 = 0x80000000;
        ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x01000000;
        load_palette((s32 *)0x801D27F0U, 0xE, 1, 2, 0);
        col2sepia(0x801D27F0U, (s32 *)D_800E6280.unk_183C, 8, 4, 0);
        load_palette((s32 *)D_800E6280.unk_183C, 0xE, 1, 2, 0);
        col2sepia(0x80180400U, &D_800E6280.unk_163C[0], 8, 4, 0);
        load_palette(&D_800E6280.unk_163C[0], 0x12, 1, 2, 0);
        func_80048E78();
        i = 0x60;
        func_80048F64(i);
        ((DisplayActor44 *)D_8011ECD0)[i].unk_01 = 5;
        ((DisplayActor44 *)D_8011ECD0)[i].unk_02 = 0;
        ((DisplayActor44 *)D_8011ECD0)[i].unk_38 = 0x41000000;
        ((DisplayActor44 *)D_8011ECD0)[i].unk_03 = 0xA4;
        ((DisplayActor44 *)D_8011ECD0)[i].unk_0C = (u8 *)0x801D6B60;
        ((DisplayActor44 *)D_8011ECD0)[i].unk_10 = (u8 *)0x801D6B90;
        ((DisplayActor44 *)D_8011ECD0)[i].unk_34 = (u8 *)0x801D6B54;
        ((DisplayActor44 *)D_8011ECD0)[i].unk_16 = 0;
        ((DisplayActor44 *)D_8011ECD0)[i].unk_04 = 8;
        ((DisplayActor44 *)D_8011ECD0)[i].unk_05 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[i].unk_26 = -0xA0;
        ((DisplayActor44 *)D_8011ECD0)[i].unk_2A = -0x78;
        D_800E6280.unk_1104.u = 0;
        D_800E6280.unk_110D++;
        if (D_800E6280.unk_F5F >= 0x11U) {
            D_800E6280.unk_F5F = (D_800E6280.unk_F5F & 0xF) + 0x10;
        }
        if ((D_800E6280.unk_F5F == 0x10) || (((u32) D_800E6280.unk_F80 >> 0x10) != 0)) {
            D_800E6280.unk_F5F = 0x10;
            func_80046290(0x3AA, 0x070003B5, 0xF);
            func_8004E58C();
            set_kanji_string(-0x90, 0x44, 0, "す・き・で・す", 0);
            k_disp_start(1);
            k_disp_switch(0, 0);
            func_80044750(0x300);
        }
        break;
    case 17:
        if (D_800E6280.unk_F5F == 0x10) {
            if ((func_800460EC() & 4) && ((u32) D_800E6280.unk_1104.u >= 0x21U)) {
                D_80120652 = 3;
                D_80120668 = 1;
                D_800E6280.unk_1104.u = 0;
                D_800E6280.unk_110D++;
            }
        } else {
            D_80120652 = 3;
            D_80120668 = 1;
            D_800E6280.unk_110D++;
            D_800E6280.unk_1104.u = 0;
        }
        break;
    case 18:
        if ((u32) D_800E6280.unk_1104.u >= 0x41U) {
            D_800E6280.unk_1104.u = 0;
            D_800E6280.unk_110D++;
        }
        break;
    case 19:
        func_8004AC18((s32) D_800E6280.unk_1104.u);
        if ((u32) D_800E6280.unk_1104.u >= 0xFFU) {
            func_8004284C();
        }
        break;
    }
    func_80144654();
}
