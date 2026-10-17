/* NON_MATCHING: T-20008. Archived reconstruction; production keeps ASM.
 * FAKE: explicit unsigned flag-store views are allocator controls. They reduce
 * duplicate constant ranges without changing shared Rec24 types. No match is
 * claimed; remove or justify them again before any production promotion.
 */
void func_80143580(void) {
    s32 index;
    s32 tiles[8];
    s32 address;
    Rec24 *sprite;
    s32 x;
    s32 y;
    s16 old_x;
    u8 fade;
    u8 fade2;
    u8 girl;

    index = 0;
    address = 0x80180000;
    do {
        tiles[index++] = address;
        address += 0x4000;
    } while (address < (s32)0x801A0000);
    index = D_800E6280.unk_110D;
    switch (index) {
    case 0:
        fade = ((SpriteSlot24 *)&D_801220D0)[0].unk_12;
        ((SpriteSlot24 *)&D_801220D0)[0].unk_11 = fade;
        ((SpriteSlot24 *)&D_801220D0)[0].unk_12 = fade + 1;
        ((SpriteSlot24 *)&D_801220D0)[0].unk_10 = fade;
        if (fade >= 0x81U) {
            ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x01000000;
            D_8014D170 = 0;
            D_800E6280.unk_110D = index + 1;
        }
        break;
    case 1:
        if ((D_800E6280.unk_10F8 & 3) == 3) {
            if (((SpriteSlot24 *)&D_801220D0)[0].unk_04-- < -0xF0) {
                ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x51000000;
                D_800E6280.unk_110D = index + 1;
            }
        }
        break;
    case 2:
        fade = ((SpriteSlot24 *)&D_801220D0)[0].unk_12;
        ((SpriteSlot24 *)&D_801220D0)[0].unk_11 = fade;
        ((SpriteSlot24 *)&D_801220D0)[0].unk_12 = fade - 1;
        ((SpriteSlot24 *)&D_801220D0)[0].unk_10 = fade;
        if (fade != 0) {
            fade2 = ((SpriteSlot24 *)&D_801220D0)[1].unk_12;
            ((SpriteSlot24 *)&D_801220D0)[1].unk_11 = fade2;
            ((SpriteSlot24 *)&D_801220D0)[1].unk_12 = fade2 + 1;
            ((SpriteSlot24 *)&D_801220D0)[1].unk_10 = fade2;
            if (fade2 >= 0x81U) {
                ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x80000000;
                ((SpriteSlot24 *)&D_801220D0)[1].unk_00 = 0x01000000;
                D_800E6280.unk_110D = index + 1;
            }
        }
        /* fallthrough */
    case 3:
        if ((D_800E6280.unk_10F8 & 3) == 3) {
            ((SpriteSlot24 *)&D_801220D0)[0].unk_04 -= 1;
            if (((SpriteSlot24 *)&D_801220D0)[1].unk_04++ >= -0xEF) {
                func_8004A1A0((s32) tiles, 0, 8, 4, 2, 0x1EF, -0xA0, -0x78, 8, 5);
                load_palette((s32 *)0x801A0000U, 0xF, 1, 3, 0);
                ((SpriteSlot24 *)&D_801220D0)[0].unk_11 = 0;
                ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x51000000;
                ((SpriteSlot24 *)&D_801220D0)[0].unk_12 = 0;
                ((SpriteSlot24 *)&D_801220D0)[0].unk_10 = 0;
                index = D_800E6280.unk_110D;
                D_800E6280.unk_110D = index + 1;
            }
        }
        break;
    case 4:
        func_80143498();
        fade2 = ((SpriteSlot24 *)&D_801220D0)[1].unk_12;
        ((SpriteSlot24 *)&D_801220D0)[1].unk_11 = fade2;
        ((SpriteSlot24 *)&D_801220D0)[1].unk_12 = fade2 - 1;
        ((SpriteSlot24 *)&D_801220D0)[1].unk_10 = fade2;
        if (fade2 != 0) {
            fade = ((SpriteSlot24 *)&D_801220D0)[0].unk_12;
            ((SpriteSlot24 *)&D_801220D0)[0].unk_11 = fade;
            ((SpriteSlot24 *)&D_801220D0)[0].unk_12 = fade + 1;
            ((SpriteSlot24 *)&D_801220D0)[0].unk_10 = fade;
            if (fade >= 0x81U) {
                ((SpriteSlot24 *)&D_801220D0)[1].unk_00 = 0x80000000;
                ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x01000000;
                index = D_800E6280.unk_110D;
                D_800E6280.unk_110D = index + 1;
            }
        }
        index = D_800E6280.unk_110D;
        /* fallthrough */
    case 5:
        /* FAKE: retain the original selector stores while their source form
         * remains unresolved. This draft is nonmatching. T-20008. */
        D_800E6280.unk_110D = index;
        if ((D_800E6280.unk_10F8 & 3) == 3) {
            if (((SpriteSlot24 *)&D_801220D0)[1].unk_04 < -0xA0) {
                ((SpriteSlot24 *)&D_801220D0)[1].unk_04 += 1;
            }
            old_x = ((SpriteSlot24 *)&D_801220D0)[0].unk_04 - 1;
            D_800E6280.unk_110D = index;
            if (((SpriteSlot24 *)&D_801220D0)[0].unk_04 < -0x140) {
                ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x51000000;
                D_800E6280.unk_110D = index + 1;
            }
            ((SpriteSlot24 *)&D_801220D0)[0].unk_04 = old_x;
            func_8004B19C((s32) old_x, 0, 0);
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
        fade = ((SpriteSlot24 *)&D_801220D0)[0].unk_12;
        ((SpriteSlot24 *)&D_801220D0)[0].unk_11 = fade;
        ((SpriteSlot24 *)&D_801220D0)[0].unk_12 = fade - 1;
        ((SpriteSlot24 *)&D_801220D0)[0].unk_10 = fade;
        if (fade == 0) {
            ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x80000000;
            /* FAKE: retain both observed fill loops; original source form
             * is unresolved. T-20008. */
            for (address = 0x801B0000; address < (s32)0x801D0000; address += 0x4000) {
                index = 0;
                for (address = 0x801B0000; address < (s32)0x801D0000; address += 0x4000) {
                    tiles[index++] = address;
                }
            }
            func_8004A1A0((s32) tiles, 1, 6, 3, 2, 0x1EE, -0xA0, -0x78, 8, 0x15);
            load_palette((s32 *)0x801C8000U, 0xE, 1, 3, 0);
            ((SpriteSlot24 *)&D_801220D0)[1].unk_11 = 0;
            ((SpriteSlot24 *)&D_801220D0)[1].unk_12 = 0;
            ((SpriteSlot24 *)&D_801220D0)[1].unk_10 = 0;
            sprite = D_801217D0;
            index = 0;
            x = -0xD0;
            y = 9;
            do {
                sprite->unk_04 = x;
                sprite->unk_0C = y;
                y += 2;
                x += 0x50;
                sprite++;
                *(u32 *)&sprite[-1].unk_00 = 0x01000000;
                sprite[-1].unk_06 = -0x78;
                sprite[-1].unk_08 = 0xFF;
                sprite[-1].unk_0A = 0xF0;
                sprite[-1].unk_12 = 0x1FF - index++;
                sprite[-1].unk_1C = 0x1000;
                sprite[-1].unk_1E = 0x1000;
                D_800E6280.unk_10A5[index - 1] = 7;
            } while (index != 3);
            D_800E6280.unk_10A5[1] = 8;
            D_801217D0[1].unk_06 = -0x6E;
            index = D_800E6280.unk_110D;
            D_800E6280.unk_110D = index + 1;
        }
        break;
    case 7:
        D_801217D0[2].unk_15 = D_801217D0[2].unk_16;
        D_801217D0[2].unk_14 = D_801217D0[2].unk_16;
        D_801217D0[1].unk_16 = D_801217D0[2].unk_16;
        D_801217D0[1].unk_15 = D_801217D0[2].unk_16;
        D_801217D0[1].unk_14 = D_801217D0[2].unk_16;
        D_801217D0[0].unk_16 = D_801217D0[2].unk_16;
        D_801217D0[0].unk_15 = D_801217D0[2].unk_16;
        D_801217D0[0].unk_14 = D_801217D0[2].unk_16;
        if (D_801217D0[2].unk_16++ >= 0x81U) {
            D_800E6280.unk_110D = index + 1;
        } else {
            fade2 = ((SpriteSlot24 *)&D_801220D0)[1].unk_12;
            ((SpriteSlot24 *)&D_801220D0)[1].unk_11 = fade2;
            ((SpriteSlot24 *)&D_801220D0)[1].unk_12 = fade2 + 1;
            ((SpriteSlot24 *)&D_801220D0)[1].unk_10 = fade2;
        }
        break;
    case 8:
        if ((D_800E6280.unk_10F8 & 3) == 3) {
            if (D_801217D0[0].unk_04-- < -0x160) {
                func_80044750(0xD1);
                func_8004E58C();
                cd_read_sectors(0x31U, 0x80180000U, 0xA017);
                ((SpriteSlot24 *)&D_801220D0)[0].unk_10 = 0;
                index = D_800E6280.unk_110D;
                D_800E6280.unk_110D = index + 1;
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
            *(u32 *)&D_801217D0->unk_00 = 0x80000000;
            ((SpriteSlot24 *)&D_801220D0)[1].unk_00 = 0x80000000;
            ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x01000000;
            ((SpriteSlot24 *)&D_801220D0)[0].unk_11 = 0x80;
            ((SpriteSlot24 *)&D_801220D0)[0].unk_12 = 0x80;
            ((SpriteSlot24 *)&D_801220D0)[0].unk_10 = 0x80;
            *(u32 *)&D_801217D0[2].unk_00 = 0x80000000;
            index = D_800E6280.unk_110D;
            D_800E6280.unk_110D = index + 1;
            ((SpriteSlot24 *)&D_801220D0)[1].unk_10 = 0xFF;
        }
        break;
    case 10:
        func_8004AC18(0xFF);
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) tiles, 0, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0xD);
            load_palette((s32 *)0x801C8000U, 0xF, 1, 3, 0);
            ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x01000000;
            cd_read_sectors(0xAEU, 0x80180000U, 0x9DF6);
            index = D_800E6280.unk_110D;
            D_800E6280.unk_110D = index + 1;
        }
        break;
    case 11:
        func_8004AC18(0xFF);
        if (cd_read_sync_status() & 1) {
            tpage_buf_clear();
            index = 0;
            do {
                load_palette((s32 *)0x80180000U, (index + 0x10) & 0xFF, 1, 2, index);
                index += 1;
            } while (index < 3);
            col2sepia(0x80180000U, &D_800E6280.unk_163C[0], 8, 4, 0);
            load_palette(&D_800E6280.unk_163C[0], 0x10, 1, 2, 0);
            func_80048F64(0x60);
            D_80120651 = 5;
            D_80120652 = 4;
            D_80120688 = 0x41000000;
            D_80120653 = 0x84;
            D_8012065C = (u8 *)0x801A2624;
            D_80120660 = (u8 *)0x801A263C;
            D_80120684 = (u8 *)0x801A2610;
            D_80120666 = 0;
            D_80120654 = 8;
            D_80120655 = 0x80;
            D_80120676 = -0xA0;
            D_8012067A = -0x78;
            D_800E6280.unk_1104.u = 0;
            index = D_800E6280.unk_110D;
            D_800E6280.unk_110D = index + 1;
        }
        break;
    case 12:
        if (D_800E6280.unk_1104.u == 0xC8) {
            D_80120652 |= 1;
        }
        if (((SpriteSlot24 *)&D_801220D0)[1].unk_10 != 0) {
            func_8004AC18(((SpriteSlot24 *)&D_801220D0)[1].unk_10--);
        } else if ((u32) D_800E6280.unk_1104.u >= 0x12DU) {
            D_800E6280.unk_110D = index + 1;
        } else if (!(D_80120652 & 1)) {
            D_800E6280.unk_110D = index + 1;
        }
        break;
    case 13:
        /* FAKE: same redundant nested-fill control as step 6. T-20008. */
        for (address = 0x80186000; address < (s32)0x8019E000; address += 0x4000) {
            index = 0;
            for (address = 0x80186000; address < (s32)0x8019E000; address += 0x4000) {
                tiles[index++] = address;
            }
        }
        func_8004A1A0((s32) tiles, 1, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0x15);
        ((SpriteSlot24 *)&D_801220D0)[0].unk_00 = 0x80000000;
        ((SpriteSlot24 *)&D_801220D0)[1].unk_00 = 0x01000000;
        load_palette((s32 *)0x8019E000U, 0xF, 1, 2, 0);
        col2sepia(0x8019E000U, &D_800E6280.unk_163C[0], 8, 4, 0);
        for (index = 0; index < 128; index++) {
            if (index < 32) {
                ((s32 *)D_800E6280.unk_183C)[index] = D_800E6280.unk_163C[index];
            } else {
                ((s32 *)D_800E6280.unk_183C)[index] = ((s32 *)0x8019E000)[index];
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
        index = D_800E6280.unk_110D;
        D_800E6280.unk_110D = index + 1;
        break;
    case 14:
        if (func_800460EC() & 4) {
            D_800E6280.unk_1104.u = 0;
            index = D_800E6280.unk_110D;
            D_800E6280.unk_110D = index + 1;
        }
        break;
    case 15:
        func_8014472C();
        func_80144908();
        if (D_800E6280.unk_F88 & 0xFFFF) {
            index = D_800E6280.unk_110D;
            D_800E6280.unk_110D = index + 1;
        } else if ((u32) D_800E6280.unk_1104.u >= 0x101U) {
            index = D_800E6280.unk_110D;
            D_800E6280.unk_110D = index + 1;
        } else if ((func_800460EC() & 2) && ((u32) D_800E6280.unk_1104.u >= 0x79U)) {
            index = D_800E6280.unk_110D;
            D_800E6280.unk_110D = index + 1;
        }
        break;
    case 16:
        /* FAKE: same redundant nested-fill control as step 6. T-20008. */
        for (address = 0x801BA7F0; address < (s32)0x801D27F0; address += 0x4000) {
            index = 0;
            for (address = 0x801BA7F0; address < (s32)0x801D27F0; address += 0x4000) {
                tiles[index++] = address;
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
        func_80048F64(0x60);
        D_80120651 = 5;
        D_80120652 = 0;
        D_80120688 = 0x41000000;
        D_80120653 = 0xA4;
        D_8012065C = (u8 *)0x801D6B60;
        D_80120660 = (u8 *)0x801D6B90;
        D_80120684 = (u8 *)0x801D6B54;
        D_80120666 = 0;
        D_80120654 = 8;
        D_80120655 = 0x80;
        D_80120676 = -0xA0;
        D_8012067A = -0x78;
        girl = D_800E6280.unk_F5F;
        D_800E6280.unk_1104.u = 0;
        index = D_800E6280.unk_110D;
        D_800E6280.unk_110D = index + 1;
        if (girl >= 0x11U) {
            girl = ((girl & 0xF) + 0x10) & 0xFF;
        }
        if ((girl == 0x10) || (D_800E6280.unk_F5F = girl, (((u32) D_800E6280.unk_F80 >> 0x10) != 0))) {
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
                index = D_800E6280.unk_110D;
                D_800E6280.unk_110D = index + 1;
            }
        } else {
            D_80120652 = 3;
            D_80120668 = 1;
            D_800E6280.unk_1104.u = 0;
            D_800E6280.unk_110D = index + 1;
        }
        break;
    case 18:
        if ((u32) D_800E6280.unk_1104.u >= 0x41U) {
            D_800E6280.unk_1104.u = 0;
            D_800E6280.unk_110D = index + 1;
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
