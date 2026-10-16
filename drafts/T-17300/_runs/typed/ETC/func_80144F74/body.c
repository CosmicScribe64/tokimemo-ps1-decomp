void func_80144F74(void) {
    s32 temp_t6;
    s32 var_v0;
    u8 temp_a0;
    u8 temp_a0_2;
    u8 *temp_v1;
    u8 *temp_v1_2;

    switch (*(u8 *)((unsigned char *)(&D_800E6280) + (4365))) {
    case 0:
        k_sub_reset();
        temp_t6 = *(u8 *)((unsigned char *)(&D_800E6280) + (3935)) & 0xF;
        temp_v1 = temp_t6 + &D_800E6280;
        temp_a0 = *(u8 *)((unsigned char *)temp_v1 + (1340));
        var_v0 = temp_t6;
        if (temp_a0 < 0x63U) {
            *(u8 *)((unsigned char *)temp_v1 + (1340)) = (u8) (temp_a0 + 1);
            var_v0 = *(u8 *)((unsigned char *)(&D_800E6280) + (3935)) & 0xF;
        }
        temp_v1_2 = var_v0 + &D_800E7D10;
        temp_a0_2 = *(u8 *)((unsigned char *)temp_v1_2 + (20));
        if (temp_a0_2 < 0x63U) {
            *(u8 *)((unsigned char *)temp_v1_2 + (20)) = (u8) (temp_a0_2 + 1);
        }
        set_kanji_string(-0x80, 0x32, 1, "データセーブ中です", 0);
        set_kanji_string(-0x80, 0x42, 1, "しばらくお待ち下さい", 0);
        k_sub_disp_start(2);
        *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        break;
    case 1:
        if (func_80054590() == 0x14) {
            *(&D_800E7D14 + *(u8 *)((unsigned char *)(&D_800E6280) + (4376))) = 1;
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
            func_80053D10();
        } else if (((u32) *(u32 *)((unsigned char *)(&D_800E6280) + (4384)) >= 0x201U) || (*(u8 *)((unsigned char *)(&D_800E6280) + (4373)) == 0xF0)) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 2:
        if (D_800E8BEE != 0) {
            func_80053CE0();
            *(s8 *)((unsigned char *)(&D_800E6280) + (4379)) = 1;
            D_800E7D36 = *(u8 *)((unsigned char *)(&D_800E6280) + (4376));
            *(u8 *)((unsigned char *)(&D_800E6280) + (4376)) = 0U;
            func_80054E6C();
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        } else {
            func_80042908(2);
        }
        break;
    case 3:
        if (func_80054388() == 0x13) {
            func_80053D10();
            func_80042908(2);
        } else if (((u32) *(u32 *)((unsigned char *)(&D_800E6280) + (4384)) >= 0x201U) || (*(u8 *)((unsigned char *)(&D_800E6280) + (4373)) == 0xF0)) {
            func_80042940(0xFF);
        }
        break;
    }
    func_8005C7FC();
}
