s32 func_80142220(s32 arg0) {
    Tc14 *var_v0;
    u8 temp_t6;

    var_v0 = D_8015E770;
loop_1:
    temp_t6 = var_v0->unk13;
    var_v0 += 0x14;
    if (temp_t6 & 0x80) {
        return -1;
    }
    if ((u32) var_v0 >= (u32) D_8015E8B0) {
        if (D_80160460 != 0) {
            return 1;
        }
        if (D_8015EDB4->unk2 & 0x80) {
            goto block_13;
        }
        if (arg0 & 0x40) {
            if (func_801421AC() == 3) {
                D_8015EDB4->unk2 |= 0xC0;
                func_801334BC();
                func_8013356C();
                D_80160460 = 1;
                return 3;
            }
            if (func_801421AC() == 4) {
                D_8015EDB4->unk2 |= 0xC0;
                func_801334BC();
                func_8013356C();
                D_8015E783 = 0x90;
                D_8015E770->unk0 = D_8015EDB4->unk74;
                D_8015E772 = D_8015EDB4->unk76;
                D_8015E774 = D_8015EDB4->unk78 - 0xC8;
                D_8015E778 = 0;
                D_8015E77A = 0;
                D_8015E77C = -8;
                D_8015E782 = 0;
                func_801335A0(1, 0x505);
                return 4;
            }
block_13:
            /* Duplicate return node #14. Try simplifying control flow for better match */
            return 0;
        }
        return 0;
    }
    goto loop_1;
}
