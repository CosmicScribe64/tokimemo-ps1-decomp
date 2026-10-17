void func_800755B4(void) {
    s32 i;
    s32 r;
    s32 n;
    u8 buf[0x104];
    u8 name[0x20];

    /* FAKE: T-20940, the clear on one source line (as1 schedules a line as a unit) gives the original's order
     * of the unrolled stores; the three remainder stores still take three lui $at (the original shares one). */
    D_800B6990.pos = 0; for (i = 0; i < 7; i++) { D_800B6990.keys[i] = 0xFF; }
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
    func_80066104("休日コマンド画面");
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
        sndisp("一週間強制補習を しなければならない）", 0, 0x1F);
    } else if ((D_800E6280.unk_0F4.b[1] & 0xF) == 3) {
        D_800E6280.unk_F6D = 0xE;
        func_8006509C();
        func_800654D0((s32) (u8) D_800E6280.unk_F6D);
        func_8005AC70(1);
        icon_can_use_set(0xD, 0);
        icon_can_use_set(7, 1);
        D_800E6280.unk_1093 = 7;
        sndisp("病気で身体が動かない）", 0, 0x1F);
    } else {
        n = 0;
        if (appointment_find_today((s32) D_800E6280.unk_03E, (s32) D_800E6280.unk_03F, (s32) D_800E6280.unk_040) > 0) {
            strcpy(buf, "今日は、");
            get_g_name(name, (s32) ((u32) (D_800E6280.unk_54C[0].w * 8) >> 0x1C));
            strcat(buf, name);
            strcat(buf, "とデートだ）");
            sndisp(buf, 0, 0x1F);
            D_800E6280.unk_1093 = 9;
            n = 1;
        }
        if ((func_8005352C() == 1) && (((u32) (D_800E6280.unk_040 + 6) / 7U) == 3) && (D_800E6280.unk_041 == 6)) {
            if (((u8) D_800E6280.unk_03F % 3U) == 0) {
                if (n != 0) {
                    strcpy(buf, "（");
                    strcat(buf, "今日は、部活の練習試合だ）");
                    set_kanji_string(-0x56, 0x42, 0U, buf, 0);
                } else {
                    strcpy(buf, "今日は、部活の練習試合だ）");
                    sndisp(buf, n, 0x1F);
                }
                n += 1;
            } else {
                if (n != 0) {
                    strcpy(buf, "（");
                    strcat(buf, "今日は、部活の練習日だ）");
                    set_kanji_string(-0x56, 0x42, 0U, buf, 0);
                } else {
                    strcpy(buf, "今日は、部活の練習日だ）");
                    sndisp(buf, n, 0x1F);
                }
                n += 1;
            }
            D_800E6280.unk_1093 = 4;
        }
        if (n == 0) {
            sndisp("今日は何をしようかな？）", 0, 0x1F);
        }
    }
    k_disp_start(5);
    func_80048E78();
    func_8006BC28(0);
    parameter_show_init();
    parameter_show_init();
    message_window_init();
    r = dec_bg_cd_read(D_800B5960[func_80066A2C()], 0);
    if (r != -1) {
        if ((r == 0) || (r == 1)) {
            set_dec_bri(0x80U);
            dec_bg_show_set(0, r);
            func_80042808();
        }
    } else {
        func_8004284C();
    }
}
