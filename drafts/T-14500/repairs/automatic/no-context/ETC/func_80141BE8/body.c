void func_80141BE8(void) {
    s32 temp_t3;
    u8 temp_a0;
    u8 *temp_v0;
    u8 *temp_v0_2;

    switch (D_800E738D) {
    case 0:
        func_80053CE0();
        func_80141ABC();
        D_800E738D += 1;
        break;
    case 1:
        if (func_80054388() == 0x13) {
            func_80053D10();
            D_800E738D += 1;
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 2:
        func_800AE0B0("[C_GIRL]%d \n", D_800E71DF);
        temp_t3 = D_800E71DF & 0xF;
        if (*(&D_800E67BC + temp_t3) & 1) {
            temp_v0 = (temp_t3 * 8) + &D_800B383C;
            func_80046290((*(s32 *)temp_v0), *(s32 *)((unsigned char *)temp_v0 + (4)), temp_t3);
        } else {
            temp_v0_2 = (temp_t3 * 8) + &D_800B37D4;
            func_80046290((*(s32 *)temp_v0_2), *(s32 *)((unsigned char *)temp_v0_2 + (4)), temp_t3);
        }
        func_80044750(0x300);
        D_800E738D += 1;
        break;
    case 3:
        if (func_800460EC() & 4) {
            D_800E738D += 1;
        }
        break;
    case 4:
        if (func_800460EC() & 2) {
            D_800E738D += 1;
        }
        break;
    case 5:
        temp_a0 = D_800E62BA;
        D_800E62BA = temp_a0 - 1;
        if (temp_a0 < 8U) {
            D_800E62BA = 0;
            func_8004482C(temp_a0);
            main_state_set(0xC4);
            D_800E71DF += 0x10;
        }
        set_dec_bri(D_800E62BA);
        break;
    }
    func_800578F4(2);
}
