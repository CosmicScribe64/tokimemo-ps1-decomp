void func_8013DF88(void) {
    s32 result;
    u8 state;
    s32 i;

    u8 text[256];
    state = D_800E6280.unk_110D;
    switch (state) {
    case 0:
        func_8004E58C();
        func_800AE0F0(text, D_800B32CC[D_800E6280.unk_1118 - 1]);
        strcat(text, "番アルバム　と、");
        func_8004E788(-0x40, 0x20, 0xF, (s32) text, 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "システムファイルを", 0);
        func_8004E788(-0x40, 0x40, 0xF, (s32) "作成しています。", 0);
        func_8004E884(3);
        D_800E6280.unk_110D += 1;
    default:
        break;
    case 1:
        if ((result = func_80054694((s32) D_800E6280.unk_1118)) == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            func_80042908(4);
        }
        break;
    case 2:
        if ((result = func_80054694(0)) == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            func_80054704((s32) D_800E6280.unk_1118);
            func_80042908(4);
        }
        break;
    case 3:
        func_80053D10();
        ((NameMetricWork *)&D_800E7D10)->selected_slot = D_800E6280.unk_1118;
        i = 0;
        while (i < 15) {
            ((NameMetricWork *)&D_800E7D10)->slot_status[i] = 0;
            ++i;
        }
        D_800E7D14[D_800E6280.unk_1118] = 1;
        *D_800E7D14 = 1;
        D_800E6280.unk_1118 = 0;
        func_80053CE0();
        func_80054E6C();
        D_800E6280.unk_110D += 1;
        break;
    case 4:
        if ((result = func_80054388()) == 0x13) {
            func_80053D10();
            D_800E6280.unk_110D += 1;
        } else if (((u32) (*(u32 *)&D_800E6280.unk_1120) >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 5:
        func_80053CE0();
        func_80041878();
        func_80042058();
        func_80042058();
        D_800E6280.unk_1118 = ((NameMetricWork *)&D_800E7D10)->selected_slot;
        func_800AE0B0("s->file_no %x \n", ((NameMetricWork *)&D_800E7D10)->selected_slot);
        D_800E6280.unk_110D += 1;
        break;
    case 6:
        if ((result = func_80054590()) == 0x14) {
            D_800E7D14[D_800E6280.unk_1118] = 1;
            D_800E6280.unk_110D += 1;
        } else if (((u32) (*(u32 *)&D_800E6280.unk_1120) >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 7:
        func_80053D10();
        func_8013B77C();
        main_state_set(0x14);
        break;
    }
}
