/* FAKE: T-24503 unmatched reconstruction. Identity byte views preserve selected
 * independent loads; unused local homes and declaration order retain the observed
 * frame while source recovery continues. See README for remaining regions.
 * The normal path deliberately has no explicit return, as in the original. */
s32 func_80135DC0(void) {
    GameState *g = &D_800E6280;
    s16 sp62;
    s16 var_a0;
    s32 temp_t5;
    s32 temp_t7;
    s32 var_t9;
    s32 var_v0;
    u32 temp_v1;
    u32 temp_a1;
    u8 temp_t1;
    u8 temp_t4;
    u8 temp_t6;
    u8 temp_t9;
    u8 temp_v0;
    u8 temp_v0_2;
    u8 temp_v1_2;
    u8 var_v0_2;
    u8 sp3F;
    RECT rect;

    D_800E6280.unk_F5F = g->unk_69C[D_800E6280.unk_71C].unk_01;
    if (((D_800E6280.unk_F5F == 0) && ((*(u8 *)&D_800E6280.unk_71E) & 1)) || ((*(u8 *)&D_800E6280.unk_F5F != 0) && ((*(u8 *)&D_800E6280.unk_71E) & 2))) {
        func_80135AC8();
        return 0;
    } else {
        if (!(g->unk_1BC[*(u8 *)&D_800E6280.unk_F5F].unk_0C.f.b6) && (*(u8 *)&D_800E6280.unk_F5F != 0)) {
            func_80135AC8();
            return 0;
        } else {
            D_80145074 = (D_800E6280.unk_71E & 0x10) >= 1U;
            draw2d3d(1, 0);
            func_80048DAC(1);
            func_80041584();
            func_80048EB8(0);
            back_clear_switch(1);
            func_8006509C();
            parameter_show_init();
            parameter_show_init();
            sp62 = 0;
            do {
                icon_can_use_set(sp62, 0);
                sp62 += 1;
            } while (sp62 < 0xF);
            func_80065B0C(1);
            func_8004E58C();
            D_8014507C = 0;
            D_80145090 = 0;
            D_80145088 = 1;
            if ((D_800E6280.unk_03F != 0xC) || (D_800E6280.unk_040 != 0x18)) {
                temp_t1 = *(u8 *)&D_800E6280.unk_040;
                if ((D_800E6280.unk_041 != 6) && (temp_v0 = D_800E6280.unk_044[0].unk_20[temp_t1 - 1 + 1], (temp_v0 != 0xA)) && (temp_v0 != 0x10) && (temp_v0_2 = D_800E6280.unk_044[0].unk_00[temp_t1 - 1 + 1], (temp_v0_2 != 1)) && (temp_v0_2 != 2) && (temp_v0_2 != 3) && (D_80145074 == 0)) {
                    D_80145088 = 0;
                } else {
                    temp_t5 = *(u8 *)&D_800E6280.unk_03F;
                    temp_t7 = *(u8 *)&D_800E6280.unk_040;
                    if ((((u8) D_800E6280.unk_F68.b[0] >> 4) == 4) && ((*(u16 *)&D_800E6280.unk_0F4.h >> 0xC) == (D_800E6280.unk_1BC[*(u8 *)&D_800E6280.unk_F5F].unk_0C.b[2] >> 4)) && ((appointment_find_today((s32) D_800E6280.unk_03E, temp_t5, temp_t7) <= 0) || (D_800E6280.unk_F5F != ((u32) (D_800E6280.unk_54C[0].w * 8) >> 0x1C)))) {
                        D_80145088 = 0;
                        D_80145090 = 1;
                    }
                }
            }
            D_8014508C = 0;
            hizuke_init();
            message_window_init();
            tpage_buf_clear();
            func_800AE0F0(D_800CA19C, "廊下");
            func_800AE0F0(D_800CA1DC, "近所の公園");
            k_reset(1);
            func_8008585C();
            func_80084E4C();
            D_80122D2C = 0;
            D_800E6280.unk_10A2 = 0;
            D_800E6280.unk_10E8 = 1;
            if (D_800E6280.unk_F5F == 0) {
                D_800E6280.unk_71E = D_800E6280.unk_71E | 1;
            } else {
                D_800E6280.unk_71E = D_800E6280.unk_71E | 2;
            }
            D_800E6280.unk_03A = 0x80;
            D_800B593C = 0x80;
            D_800B5940 = 0;
            addr_init_bustup();
            D_80145080 = 0;
            D_80145084 = ((u32) D_800E6280.unk_10F8 % 3U) + 2;
            temp_a1 = get_g_zyotai_s(D_800E6280.unk_F5F) & 0x7F;
            temp_v1 = temp_a1 & 0xFF;
            D_80145060 = 0;
            D_80145064 = 0;
            D_800CA148 = (temp_v1 >= 2U) + (temp_v1 >= 3U) + (temp_v1 >= 4U) + 0x1E;
            sp3F = temp_a1;
            D_800CA14C = 0;
            func_800847B8(D_800E6280.unk_F5F);
            func_80134D10();
            switch (*(u8 *)&D_800E6280.unk_F5F) {
            case 0:
                func_800AE0F0(D_800CA1DC, "自宅前");
                D_80145048 = D_80144F14;
                D_8014504C = D_80144F48;
                D_80145050 = D_80144F7C;
                D_800CA160 = D_80144F14;
                D_800CA164 = D_80144F48;
                D_800CA168 = D_80144F7C;
                break;
            case 1:
                D_80145048 = D_80144F24;
                D_8014504C = D_80144F58;
                D_80145050 = D_80144F8C;
                break;
            case 2:
                D_80145048 = D_80144F18;
                D_8014504C = D_80144F4C;
                D_80145050 = D_80144F80;
                D_800CA160 = D_80144F18;
                D_800CA164 = D_80144F4C;
                D_800CA168 = D_80144F80;
                break;
            case 3:
                D_80145048 = D_80144F1C;
                D_8014504C = D_80144F50;
                D_80145050 = D_80144F84;
                break;
            case 4:
                D_80145048 = D_80144F38;
                D_8014504C = D_80144F6C;
                D_80145050 = D_80144FA0;
                break;
            case 5:
                D_80145048 = D_80144F28;
                D_8014504C = D_80144F5C;
                D_80145050 = D_80144F90;
                break;
            case 6:
                D_80145048 = D_80144F30;
                D_8014504C = D_80144F64;
                D_80145050 = D_80144F98;
                D_800CA160 = D_80144F30;
                D_800CA164 = D_80144F64;
                D_800CA168 = D_80144F98;
                if (sp3F < 2U) {
                    D_80145048 = D_80144F2C;
                    D_8014504C = D_80144F60;
                    D_80145050 = D_80144F94;
                    D_800CA160 = D_80144F2C;
                    D_800CA164 = D_80144F60;
                    D_800CA168 = D_80144F94;
                }
                break;
            case 7:
                D_80145048 = D_80144F20;
                D_8014504C = D_80144F54;
                D_80145050 = D_80144F88;
                break;
            case 8:
                D_80145048 = D_80144F10;
                D_8014504C = D_80144F44;
                D_80145050 = D_80144F78;
                break;
            case 9:
                D_80145048 = D_80144F34;
                D_8014504C = D_80144F68;
                D_80145050 = D_80144F9C;
                break;
            case 10:
                D_80145048 = D_80144F40;
                D_8014504C = D_80144F74;
                D_80145050 = D_80144FA8;
                break;
            }
            temp_v1_2 = *(u8 *)&D_800E6280.unk_03F;
            if (temp_v1_2 >= 4U) {
                var_v0_2 = D_800E6280.unk_03E;
                D_8014506C = var_v0_2 - 0x5F;
            } else {
                var_v0_2 = D_800E6280.unk_03E;
                D_8014506C = var_v0_2 - 0x60;
            }
            D_80145070 = 0;
            D_80145078 = 0;
            if ((0x5F == var_v0_2 && 6 == temp_v1_2 && *(u8 *)&D_800E6280.unk_040 == 3) ||
                (0x60 == var_v0_2 && 6 == temp_v1_2 && *(u8 *)&D_800E6280.unk_040 == 1) ||
                (0x61 == var_v0_2 && 6 == temp_v1_2 && *(u8 *)&D_800E6280.unk_040 == 7) ||
                (0x5F == var_v0_2 && 10 == temp_v1_2 && *(u8 *)&D_800E6280.unk_040 == 14) ||
                (0x60 == var_v0_2 && 10 == temp_v1_2 && *(u8 *)&D_800E6280.unk_040 == 12) ||
                (0x61 == var_v0_2 && 10 == temp_v1_2 && *(u8 *)&D_800E6280.unk_040 == 11)) {
                rect.x = 0x140;
                rect.y = 0x1A0;
                rect.w = 0x140;
                rect.h = 0x58;
                func_8009C7F8(&rect, 8, 0, 0);
                rect.x = 0x2C0;
                rect.y = 0xA0;
                func_8009C7F8(&rect, 8, 0, 0);
            }
            func_8004284C();
            if ((D_80145074 != 0) || (D_800E6280.unk_71E & 0x20)) {
                func_80042940(0x43);
            }
            if (D_80145088 != 0) {
                func_80042940(0x43);
            }
        }
    }

}
