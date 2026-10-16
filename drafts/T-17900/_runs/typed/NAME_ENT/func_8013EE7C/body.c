void func_8013EE7C(void) {
    s32 sp30[1];
    u8 temp_t6;

    temp_t6 = D_800E6280.unk_110D;
    switch (temp_t6) {
    case 0:
        func_8004E58C();
        func_800AE0F0(&sp30, *(void **)((unsigned char *)(&D_800B32CC[D_800E6280.unk_1118]) + (-4)));
        func_800AE100(&sp30, "番アルバム　を、");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) &sp30, 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "作成しています。", 0);
        k_disp_start(2);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    default:
        break;
    case 1:
        if (func_80054694((s32) D_800E6280.unk_1118) == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else {
            func_80042908(4);
        }
        break;
    case 2:
        func_80053CE0();
        func_80041878();
        func_80042058();
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        break;
    case 3:
        if (func_80054590() == 0x14) {
            ((NameMetricWork *)&D_800E7D10)->slot_status[D_800E6280.unk_1118] = 1;
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 4:
        func_8013B77C();
        main_state_set(0x14);
        break;
    }
}
