/* NON_MATCHING: T-24070; FAKE raw-index capture tests allocation only. */
void func_80144F74(void) {
    GameState *g = &D_800E6280;
    s32 temp_t6;
    s32 var_v0;
    u8 temp_a0;
    u8 temp_a0_2;

    switch (g->unk_110D) {
    case 0:
        k_sub_reset();
        temp_t6 = (var_v0 = g->unk_F5F) & 0xF;
        temp_a0 = g->unk_53C[temp_t6];
        var_v0 = temp_t6;
        if (temp_a0 < 0x63U) {
            g->unk_53C[temp_t6] = temp_a0 + 1;
            var_v0 = (var_v0 = g->unk_F5F) & 0xF;
        }
        temp_a0_2 = D_800E7D10.flags[var_v0];
        if (temp_a0_2 < 0x63U) {
            D_800E7D10.flags[var_v0] = temp_a0_2 + 1;
        }
        set_kanji_string(-0x80, 0x32, 1, (s32) "データセーブ中です", 0);
        set_kanji_string(-0x80, 0x42, 1, (s32) "しばらくお待ち下さい", 0);
        k_sub_disp_start(2);
        g->unk_110D += 1;
        break;
    case 1:
        if (func_80054590() == 0x14) {
            D_800E7D14[g->unk_1118] = 1;
            g->unk_110D += 1;
            func_80053D10();
        } else if (((u32) g->unk_1120 >= 0x201U) || (g->unk_1115 == 0xF0)) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 2:
        if ((u8) D_800E8BEE != 0) {
            func_80053CE0();
            g->unk_111B = 1;
            D_800E7D36 = g->unk_1118;
            g->unk_1118 = 0;
            func_80054E6C();
            g->unk_110D += 1;
        } else {
            func_80042908(2);
        }
        break;
    case 3:
        if (func_80054388() == 0x13) {
            func_80053D10();
            func_80042908(2);
        } else if (((u32) g->unk_1120 >= 0x201U) || (g->unk_1115 == 0xF0)) {
            func_80042940(0xFF);
        }
        break;
    }
    func_8005C7FC();
}
