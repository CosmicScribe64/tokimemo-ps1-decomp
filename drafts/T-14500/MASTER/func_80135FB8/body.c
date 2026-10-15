void func_80135FB8(void) {
    s32 sp3C;
    s16 temp_v0;
    s32 temp_s1;
    s32 temp_v1;
    u32 var_s0;
    u8 *temp_a0;
    u8 *temp_v1_2;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 temp_a1;
    u8 temp_a1_2;
    u8 temp_t1;

    switch (D_800E738A) {
    case 0:
        temp_v0 = (((s16) D_800E639A >> 4) + D_800E6904) - 0x140;
        if (temp_v0 < -0x1E) {
            D_8013C578 = 0;
        } else if (temp_v0 < -0x14) {
            D_8013C578 = 1;
        } else if (temp_v0 < -0xA) {
            D_8013C578 = 1;
        } else if (temp_v0 < 0) {
            D_8013C578 = 2;
        } else if (temp_v0 >= 0x1E) {
            D_8013C578 = 5;
        } else if (temp_v0 >= 0x14) {
            D_8013C578 = 4;
        } else if (temp_v0 >= 0xA) {
            D_8013C578 = 4;
        } else if (temp_v0 >= 0) {
            D_8013C578 = 3;
        }
        var_v0 = D_8013C57C;
        do {
            var_v0 += 1;
            *(s8 *)((unsigned char *)var_v0 + (-1)) = 0;
        } while ((u32) var_v0 < (u32) &D_8013C581);
        var_s0 = 0;
        if (D_8013C578 != 0) {
            var_v0_2 = D_8013C57C;
            do {
                *var_v0_2 = 1;
                var_s0 += 1;
                var_v0_2 += 1;
            } while (var_s0 < (u8) D_8013C578);
            var_s0 = 0;
        }
        temp_v1 = func_800AE0D0() & 0xF;
        sp3C = temp_v1;
        if ((u8) D_8013C578 >= 3U) {
            temp_a1 = *D_8013C57C;
            *D_8013C57C = D_8013C580;
            D_8013C580 = temp_a1;
        }
        if (temp_v1 > 0) {
            do {
                temp_s1 = func_800AE0D0() & 3;
                temp_v1_2 = &D_8013C57C[temp_s1];
                temp_a0 = &D_8013C57C[func_800AE0D0() & 3];
                temp_a1_2 = *temp_v1_2;
                var_s0 += 1;
                *temp_v1_2 = *temp_a0;
                *temp_a0 = temp_a1_2;
            } while (var_s0 != sp3C);
        }
        D_80120693 = 0x12;
        D_8013C574 = 0;
        func_8004284C();
        break;
    case 1:
        if (D_80120652 == 0) {
            if (D_8013C574 >= 5) {
                func_8004284C();
            } else {
                D_80120653 = 0x84;
                D_80120652 = 5;
                D_80120668 = 0;
                D_80120658 = 0;
                if (D_8013C57C[D_8013C574] == 1) {
                    D_80120666 = 0;
                } else {
                    D_80120666 = 1;
                }
                D_8013C574 += 1;
            }
        }
        break;
    case 2:
        D_80120653 = 0x84;
        D_80120652 = 1;
        D_80120668 = 0;
        D_80120658 = 0;
        if ((u8) D_8013C578 >= 3U) {
            func_80044750(0x204);
            D_80120666 = 2;
            D_800CA148 = 0xE;
        } else {
            func_80044750(0x205);
            D_80120666 = 3;
            D_800CA148 = 0xF;
        }
        func_8004284C();
        break;
    case 3:
        normal_date_speak();
        break;
    case 4:
        if ((u8) D_8013C578 >= 3U) {
            D_800CA148 = 0x16;
        } else {
            D_800CA148 = 0x1B;
        }
        func_8004284C();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        temp_t1 = (D_80120657 - 4) & 0xFF;
        D_80120657 = temp_t1;
        if (temp_t1 < 4U) {
            D_80120657 = 4;
        }
        break;
    case 7:
        D_80120653 = 0;
        D_80120652 = 0;
        func_80042808();
        if ((u32) ((u32) ((s32) D_800E6904 << 5) >> 0x1E) < 3U) {
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
