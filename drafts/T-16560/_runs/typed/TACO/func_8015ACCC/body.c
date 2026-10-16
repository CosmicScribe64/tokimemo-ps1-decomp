s32 func_8015ACCC(s32 a, s32 b, s16 c_unk0, s16 c_unk4, s16 d_unk0, s16 d_unk4, s16 e_unk0, s16 e_unk4, s32 f, s32 g) {
    s32 sp64;
    s32 sp50;
    Tc10 *temp_s4;
    TcActor *temp_v1;
    TcActor *var_v0;
    s16 *temp_v0_3;
    s32 *temp_s3;
    s32 temp_fp;
    s32 temp_fp_2;
    s32 temp_v0;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_v1;
    u32 var_s2;
    u8 *temp_s7;
    u8 *var_s1;
    u8 *var_s1_2;
    u8 temp_t8;
    u8 var_s2_2;
    u8 *temp_v0_2;

    var_v1 = 0;
    var_s2 = 0;
    var_s0 = 0x10;
    var_v0 = &D_8015EDB4[16];
loop_1:
    if (!(var_v0->unk0[1] & 0x80)) {
        var_v1 = var_s0;
    } else {
        var_s0 += 1;
        var_v0 += 1;
        if (var_s0 < 0x3F) {
            goto loop_1;
        }
    }
    if (var_v1 == 0) {
        return -1;
    }
    var_s1 = D_80161D9C;
    do {
        temp_t8 = *var_s1;
        var_s1 += 1;
        if (temp_t8 == 0) {
            var_s2 += 1;
        }
    } while ((u32) var_s1 < (u32) &D_80161DDC);
    temp_s7 = &D_80161E20[a];
    temp_v0 = a * 4;
    if (var_s2 < (u8) *temp_s7) {
        return -1;
    }
    sp50 = temp_v0;
    temp_fp = *(&D_80161DDC + temp_v0) + 4;
    sp64 = var_v1;
    func_8009B3C0(temp_fp);
    temp_fp_2 = temp_fp + 8;
    func_80143B34(sp64);
    func_80099E30(0, &D_8015EDB4[sp64].unk14);
    if (a == 0xA) {
        func_8009B430(temp_fp_2, (u8 *) &D_8015EDB4[sp64].unk4, 1);
    } else {
        func_8009B430(temp_fp_2, (u8 *) &D_8015EDB4[sp64].unk4, 0);
    }
    var_s2_2 = *temp_s7;
    var_s0_2 = 0;
    if (var_s2_2 != 0) {
        var_s1_2 = D_80161D9C;
        do {
            if (*var_s1_2 == 0) {
                temp_s4 = &D_80127080[var_s0_2];
                temp_s3 = sp50 + D_80161E60;
                func_80099E30((s32) &D_8015EDB4[sp64].unk14, &D_80127480[var_s0_2]);
                *var_s1_2 = (u8) sp64;
                if ((a == 0xF) || (a == 0x10)) {
                    temp_v0_2 = *temp_s3 + ((*temp_s7 - var_s2_2) * 8);
                    func_8015B1B8(var_s0_2, (*(s32 *)temp_v0_2), *(s32 *)((unsigned char *)temp_v0_2 + (4)), 0);
                } else {
                    func_8015B2AC(var_s0_2, *(s32 *)(*temp_s3 + (*temp_s7 * 4) + -(var_s2_2 * 4)));
                }
                func_8009B430(temp_fp_2, (u8 *) temp_s4, *temp_s7 - var_s2_2);
                var_s2_2 -= 1;
                temp_s4->unk4[0] = (s32) &D_80127480[var_s0_2];
                temp_s4->unk0 = 0x80000000;
            }
            var_s0_2 += 1;
            var_s1_2 += 1;
        } while (var_s2_2 != 0);
    }
    D_8015EDB4[sp64].unk4.unk0 = 0x80000000;
    D_8015EDB4[sp64].unk2 = (u8) b;
    D_8015EDB4[sp64].unk7A = 0x7FFF;
    D_8015EDB4[sp64].unk84[2] = (u8) a;
    D_8015EDB4[sp64].unk3 = 1;
    D_8015EDB4[sp64].unk82 = 0;
    D_8015EDB4[sp64].unk72 = 0;
    D_8015EDB4[sp64].unk7C = c_unk0;
    D_8015EDB4[sp64].unk7E = c_unk0;
    D_8015EDB4[sp64].unk80 = c_unk4;
    D_8015EDB4[sp64].unk0[1] = 0x80;
    D_8015EDB4[sp64].unk74 = d_unk0;
    D_8015EDB4[sp64].unk76 = d_unk0;
    D_8015EDB4[sp64].unk78 = d_unk4;
    D_8015EDB4[sp64].unk64 = e_unk0;
    D_8015EDB4[sp64].unk66 = e_unk0;
    D_8015EDB4[sp64].unk68 = e_unk4;
    D_8015EDB4[sp64].unk6A = e_unk4;
    temp_v0_3 = (a * 2) + &D_80161EA4;
    D_8015EDB4[sp64].unk6C = *temp_v0_3;
    D_8015EDB4[sp64].unk6E = *temp_v0_3;
    D_8015EDB4[sp64].unk70 = *temp_v0_3;
    D_8015EDB4[sp64].unk84[1] = (u8) f;
    D_8015EDB4[sp64].unk84[0] = *(&D_80161EC8 + a);
    D_8015EDB4[sp64].unk84[3] = g | 1;
    temp_v1 = &D_8015EDB4[sp64];
    temp_v1->unk4.unk4[0] = (s32) &temp_v1->unk14;
    return sp64;
}
