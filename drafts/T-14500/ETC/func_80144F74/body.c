void func_80144F74(void) {
    s32 temp_t6;
    s32 var_v0;
    u8 temp_a0;
    u8 temp_a0_2;
    u8 *temp_v1;

    switch (D_800E738D) {
    case 0:
        k_sub_reset();
        temp_t6 = D_800E71DF & 0xF;
        temp_a0 = D_800E6280.unk_53C[temp_t6];
        var_v0 = temp_t6;
        if (temp_a0 < 0x63U) {
            D_800E6280.unk_53C[temp_t6] = temp_a0 + 1;
            var_v0 = D_800E71DF & 0xF;
        }
        temp_v1 = var_v0 + &D_800E7D10;
        temp_a0_2 = *(u8 *)((unsigned char *)temp_v1 + (20));
        if (temp_a0_2 < 0x63U) {
            *(u8 *)((unsigned char *)temp_v1 + (20)) = (u8) (temp_a0_2 + 1);
        }
        set_kanji_string(-0x80, 0x32, 1, (s32) "データセーブ中です", 0);
        set_kanji_string(-0x80, 0x42, 1, (s32) "しばらくお待ち下さい", 0);
        k_sub_disp_start(2);
        D_800E738D += 1;
        break;
    case 1:
        if (func_80054590() == 0x14) {
            D_800E7D14[D_800E7398] = 1;
            D_800E738D += 1;
            func_80053D10();
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 2:
        if ((u8) D_800E8BEE != 0) {
            func_80053CE0();
            D_800E739B = 1;
            D_800E7D36 = D_800E7398;
            D_800E7398 = 0;
            func_80054E6C();
            D_800E738D += 1;
        } else {
            func_80042908(2);
        }
        break;
    case 3:
        if (func_80054388() == 0x13) {
            func_80053D10();
            func_80042908(2);
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80042940(0xFF);
        }
        break;
    }
    func_8005C7FC();
}
