void func_800755B4(void) {
    s32 sp160;
    s32 sp15C;
    u8 sp58;
    u8 sp38;
    s32 temp_v0_2;
    u8 *temp_v0;

    D_800B6998 = 0;
    temp_v0 = &D_800B6990 + 3;
    D_800B6992 = 0xFF;
    *(s8 *)((unsigned char *)(&D_800B6992) + (-1)) = 0xFF;
    *(s8 *)((unsigned char *)(&D_800B6992) + (-2)) = 0xFF;
    *(s8 *)((unsigned char *)(&D_800B6990) + (3)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0 + (1)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0 + (2)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0 + (3)) = 0xFF;
    hizuke_init();
    if (func_800754C0() != 0) {
        D_80122CD4 = 0;
        D_80122CD0 = 8;
        parameter_show_init();
        parameter_show_init();
        main_state_set(0x5D);
        return;
    }
    func_80041584();
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_80066104((s32) "休日コマンド画面");
    k_disp_start(1);
    D_800E6280.unk_F6D = 0xE;
    func_8006509C();
    func_800654D0((s32) (u8) D_800E6280.unk_F6D);
    func_80075AF8();
    gnsx(D_800E6280.unk_0D4);
    if (D_800E6280.unk_0F6.h & 1) {
        D_800E6280.unk_F6D = 0xE;
        func_8006509C();
        func_800654D0((s32) (u8) D_800E6280.unk_F6D);
        func_8005AC70(1);
        icon_can_use_set(0xD, 0);
        icon_can_use_set(0, 1);
        icon_can_use_set(1, 1);
        icon_can_use_set(2, 1);
        sndisp((s32) "一週間強制補習を しなければならない）", 0, 0x1F);
    } else if ((D_800E6280.unk0 & 0xF) == 3) {
        D_800E6280.unk_F6D = 0xE;
        func_8006509C();
        func_800654D0((s32) (u8) D_800E6280.unk_F6D);
        func_8005AC70(1);
        icon_can_use_set(0xD, 0);
        icon_can_use_set(7, 1);
        D_800E6280.unk_1093 = 7;
        sndisp((s32) "病気で身体が動かない）", 0, 0x1F);
    } else {
        sp15C = 0;
        if (appointment_find_today((s32) D_800E6280.unk0, (s32) D_800E6280.unk_03F, (s32) D_800E6280.unk_040) > 0) {
            strcpy(&sp58, "今日は、");
            get_g_name(&sp38, (s32) ((u32) (D_800E6280.unk_54C[0].w * 8) >> 0x1C));
            strcat(&sp58, &sp38);
            strcat(&sp58, "とデートだ）");
            sndisp((s32) &sp58, 0, 0x1F);
            D_800E6280.unk_1093 = 9;
            sp15C = 1;
        }
        if ((func_8005352C() == 1) && (((u32) (D_800E6280.unk_040 + 6) / 7U) == 3) && (D_800E6280.unk_041 == 6)) {
            if (((u8) D_800E6280.unk_03F % 3U) == 0) {
                if (sp15C != 0) {
                    strcpy(&sp58, "（");
                    strcat(&sp58, "今日は、部活の練習試合だ）");
                    set_kanji_string(-0x56, 0x42, 0U, &sp58, 0);
                } else {
                    strcpy(&sp58, "今日は、部活の練習試合だ）");
                    sndisp((s32) &sp58, sp15C, 0x1F);
                }
                sp15C += 1;
            } else {
                if (sp15C != 0) {
                    strcpy(&sp58, "（");
                    strcat(&sp58, "今日は、部活の練習日だ）");
                    set_kanji_string(-0x56, 0x42, 0U, &sp58, 0);
                } else {
                    strcpy(&sp58, "今日は、部活の練習日だ）");
                    sndisp((s32) &sp58, sp15C, 0x1F);
                }
                sp15C += 1;
            }
            D_800E6280.unk_1093 = 4;
        }
        if (sp15C == 0) {
            sndisp((s32) "今日は何をしようかな？）", 0, 0x1F);
        }
    }
    k_disp_start(5);
    func_80048E78();
    func_8006BC28(0);
    parameter_show_init();
    parameter_show_init();
    message_window_init();
    temp_v0_2 = dec_bg_cd_read(D_800B5960[func_80066A2C()], 0);
    if (temp_v0_2 != -1) {
        if ((temp_v0_2 == 0) || (temp_v0_2 == 1)) {
            sp160 = temp_v0_2;
            set_dec_bri(0x80U);
            dec_bg_show_set(0, temp_v0_2);
            func_80042808();
        }
    } else {
        func_8004284C();
    }
}
