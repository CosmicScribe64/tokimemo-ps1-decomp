? func_800AE100(? *, ? *);                          /* extern */

void func_8013DF88(void) {
    ? sp34;
    u8 temp_a1;
    u8 temp_a1_2;
    u8 temp_t6;
    void *var_v0;

    temp_t6 = D_800E6280.unk_110D;
    switch (temp_t6) {
    case 0:
        func_8004E58C();
        func_800AE0F0(&sp34, *(void **)((unsigned char *)(&D_800B32CC[D_800E6280.unk_1118]) + (-4)));
        func_800AE100(&sp34, "番アルバム　と、");
        func_8004E788(-0x40, 0x20, 0xF, (s32) &sp34, 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "システムファイルを", 0);
        func_8004E788(-0x40, 0x40, 0xF, (s32) "作成しています。", 0);
        func_8004E884(3);
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
        if (func_80054694(0) == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else {
            func_80054704((s32) D_800E6280.unk_1118);
            func_80042908(4);
        }
        break;
    case 3:
        func_80053D10();
        temp_a1 = D_800E6280.unk_1118;
        D_800E7D10.selected_slot = temp_a1;
        D_800E7D10.slot_status[0] = 0;
        D_800E7D10.slot_status[1] = 0;
        var_v0 = &D_800E7D10 + 3;
        D_800E7D10.slot_status[2] = 0;
        do {
            var_v0 += 4;
            var_v0->unk0 = 0;
            *(s8 *)((unsigned char *)var_v0 + (1)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (2)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (3)) = 0;
        } while (var_v0 != &D_800E7D10.slot_status[0xB]);
        D_800E7D10.slot_status[temp_a1] = 1;
        D_800E7D10.slot_status[0] = 1;
        D_800E6280.unk_1118 = 0;
        func_80053CE0();
        func_80054E6C();
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        break;
    case 4:
        if (func_80054388() == 0x13) {
            func_80053D10();
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
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
        temp_a1_2 = D_800E7D10.selected_slot;
        D_800E6280.unk_1118 = temp_a1_2;
        func_800AE0B0("s->file_no %x \n", temp_a1_2);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        break;
    case 6:
        if (func_80054590() == 0x14) {
            D_800E7D10.slot_status[D_800E6280.unk_1118] = 1;
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
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
