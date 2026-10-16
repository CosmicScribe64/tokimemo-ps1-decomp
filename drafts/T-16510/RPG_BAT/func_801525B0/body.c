void func_801525B0(void) {
    s32 sp28;
    s32 temp_v0;
    s32 var_a3;

    D_800E62BA = 0;
    D_8015EDBC = func_8013E934(0xB1, 0xB3, (s32) &D_800E6354) - 1;
    var_a3 = 0xC3;
    if (D_8015EB9C != 0xE) {
        sp28 = 0xD3;
        var_a3 = 0xD3;
        D_8015EE14 = func_8013E934(0xB1, 0xC3, (s32) girl_short_name_table[D_8015EB9C]) - 1;
    }
    switch (D_8015EB98) {
    case 0x10000:
        D_8015EE40 = func_8013E934(0xB1, var_a3, (s32) "マングース") - 1;
        D_8015EDE8 = func_8013E934(0x13, 0xBB, (s32) "ハブ・ロード") - 1;
        break;
    case 0x20000:
        D_8015EE40 = func_8013E934(0xB1, var_a3, (s32) "虚無僧") - 1;
        D_8015EDE8 = func_8013E934(0x13, 0xBB, (s32) "鹿・グレート") - 1;
        break;
    case 0x40000:
        D_8015EE40 = func_8013E934(0xB1, var_a3, (s32) "マタギ") - 1;
        D_8015EDE8 = func_8013E934(0x13, 0xBB, (s32) "キラーヒグマ") - 1;
        break;
    case 0x100000:
        D_8015EE40 = func_8013E934(0x13, 0xBB, (s32) "番長") - 1;
        D_8015EDE8 = func_8013E934(0x13, 0xBB, (s32) "不良達") - 1;
        break;
    case 0x80000000:
        D_8015EDE8 = func_8013E934(0x13, 0xBB, (s32) "宇宙人") - 1;
        break;
    case 0x8000000:
        D_8015EE14 = func_8013E934(0x17, 0xBB, (s32) "紐緒") - 1;
        D_8015EE40 = func_8013E934(0x17, 0xBB, (s32) "真・世界征服") - 1;
        D_8015EDE8 = func_8013E934(0x17, 0xBB, (s32) "真・世界征服ロボ") - 1;
        break;
    }
    k_disp_start(D_8015EDE8 + 1);
    k_disp_switch(D_8015EDBC, 1);
    k_disp_switch(D_8015EDE8, 1);
    k_disp_switch(D_8015EE40, 0);
    temp_v0 = D_8015EB98;
    if (temp_v0 == 0x08000000) {
        k_disp_switch(D_8015EE14, 1);
        k_disp_switch(D_8015EE40, 0);
        k_disp_switch(D_8015EDE8, 0);
    }
    if (temp_v0 == 0x80000000) {
        k_disp_switch(D_8015EDE8, 0);
    }
    if (D_8015EB9C != 0xE) {
        k_disp_switch(D_8015EE14, 1);
    }
    k_sub_reset_point_set();
}
