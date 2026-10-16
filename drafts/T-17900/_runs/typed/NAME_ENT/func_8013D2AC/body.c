void func_8013D2AC(void) {
    s32 sp48[1];
    u8 temp_v0;
    u8 temp_v1;
    u8 var_v1;
    u8 *var_a0;

    temp_v1 = D_800E6280.unk_110D;
    switch (temp_v1) {
    case 0:
        func_80053CE0();
        var_a0 = &D_800E7D10 + 1;
        var_v1 = 1;
loop_5:
        if (*(u8 *)((unsigned char *)var_a0 + (4)) == 1) {
            D_800E6280.unk_1118 = var_v1;
        } else {
            var_v1 += 1;
            var_a0 += 1;
            if (var_v1 != 0xF) {
                goto loop_5;
            }
        }
        if ((u32) var_a0 < (u32) &((NameMetricWork *)&D_800E7D10)->slot_status[0xB]) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else {
            func_80053D10();
            func_80042940(0x40);
        }
        func_800AE0B0("load file %d \n", D_800E6280.unk_1118);
        break;
    case 1:
        if (func_8005448C() == 4) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else {
            temp_v0 = D_800E6280.unk_1115;
            if (temp_v0 == 0xF0) {
                func_80056284();
                func_80042908(1);
            } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (temp_v0 == 0xD0)) {
                func_80053D10();
                func_80056284();
                func_80042908(1);
            }
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            func_8013B59C();
            func_8004284C();
        } else {
            func_8004E58C();
            func_800AE0F0(&sp48, D_800B3288[D_800E6280.unk_1118]);
            func_800AE100(&sp48, "番アルバムのデータは");
            set_kanji_string(-0x40, 0x20, 0xF, (s32) &sp48, 0);
            set_kanji_string(-0x40, 0x30, 0xF, (s32) "壊れています。", 0);
            ((NameMetricWork *)&D_800E7D10)->slot_status[D_800E6280.unk_1118] = 0xFF;
            func_80053D10();
            D_800E6280.unk_110D = 0;
        }
        break;
    }
}
