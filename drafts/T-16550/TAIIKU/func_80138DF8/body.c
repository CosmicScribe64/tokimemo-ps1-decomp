void func_80138DF8(void) {
    s32 pad; /* FAKE: T-11640, frame slot */
    s32 i;
    RECT rect;
    TaiWords11 tblA;
    TaiWords11 tblB;

    tblA = D_80149E90;
    tblB = D_80149EBC;
    switch (D_800E6280.unk_110A) {
    case 0:
        if (cd_read_sync_status() & 1) {
            for (i = 0; i < 18; i++) {
                D_800E6280.unk_1228[search_load_tpage_buf_lock(D_80149DAC[i], 1)] = i + 0x80000100;
                D_80149DAC[i] = i + 0x80000100;
            }
            rect.x = 0x300;
            rect.y = 0x80;
            rect.w = 0x40;
            rect.h = 0x80;
            func_8009C884(&rect, (void *)0x801BB000);
            D_800E6280.unk_1228[44] = 8;
            for (i = 0; i < 5; i++) {
                D_800E6280.unk_1228[search_load_tpage_buf_lock(D_80149DF4[i], 1)] = i + 0x80000200;
                D_80149DF4[i] = i + 0x80000200;
            }
            cd_read_sectors(3, 0x801F8000, 0xBE2A);
            Fade_In_Init();
            func_80044750(0x200);
            func_8004284C();
        }
        break;
    case 1:
        if (Fade_In_Main() != 0) {
            if (cd_read_sync_status() & 1) {
                draw2d3d(1, 1);
                func_8014045C();
                func_801404DC();
                func_8014064C();
                func_801406A8(0x801F8000);
                func_80140738(0x801F8000, 0);
                func_8004284C();
            }
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 2:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        func_80139FE4();
        Yubi(0x6B);
        if (D_800E6280.unk_F88 & 0xF0) {
            for (i = 0; i < 11; i++) {
                if (((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 <= ((DisplayActor44 *)D_8011ECD0)[107].unk_26 + 8 &&
                    ((DisplayActor44 *)D_8011ECD0)[107].unk_26 + 8 <= ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 + 0x20 &&
                    ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_2A <= ((DisplayActor44 *)D_8011ECD0)[107].unk_2A + 0x10 &&
                    ((DisplayActor44 *)D_8011ECD0)[107].unk_2A + 0x10 <= ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_2A + 0x40 &&
                    ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_03 != 0) {
                    D_80149940 = i;
                    Fade_Out_Init();
                    func_80044750(0x74);
                    func_8004284C();
                }
            }
        }
        break;
    case 3:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (func_800460EC() == 2) {
            cd_read_sectors(0x87, 0x8018F000, 0xB98F);
            func_8004284C();
        }
        break;
    case 4:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (Fade_Out_Main() != 0) {
            func_8004284C();
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80133C30();
            func_8004E44C(0, (u32 *)D_801491C4, D_801491C0);
            srn_init(0, D_801491C8, 0);
            srn_vram_set(0, 5, 0x140, 0xF0);
            func_801331D0();
            for (i = 0; i < 6; i++) {
                search_load_tpage_buf_lock(D_8014A070[i], 1);
            }
            func_8013A314();
            D_80122EA0 = 0;
            func_80044890(0, 0xBF98, 0xBF79, tblA.value[D_80149940], tblB.value[D_80149940], 0xD6B7);
            Fade_In_Init();
            func_8004284C();
        }
        break;
    case 6:
        func_800867CC();
        if (Fade_In_Main() != 0) {
            Vblnk_Timer_Init();
            func_80042808();
        }
        func_8013DDB8();
        func_8013D788();
        func_8013D2B4();
        break;
    default:
        func_80046500();
        break;
    }
}
