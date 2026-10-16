s32 func_80135358(void) {
    s32 sp34;
    s32 *var_s0;
    s32 temp_a1;
    s32 temp_a3;
    s32 temp_a3_2;
    s32 temp_s1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_s1;
    s32 var_v1;

    temp_a3 = D_8015EDB0.unk_04;
    if ((temp_a3 >= 0x68) && (temp_a3 < 0x450)) {
        if (!(temp_a3 & 7)) {
            func_8013F3F0(0, 1);
        } else {
            func_8013F3F0(0, -1);
        }
    }
    if ((temp_a3 >= 0x130) && (temp_a3 < 0x3B8)) {
        if (!(temp_a3 & 7)) {
            func_8013F3F0(0, 1);
        } else {
            func_8013F3F0(0, -1);
        }
    }
    if ((temp_a3 >= 0x1F8) && (temp_a3 < 0x320)) {
        if (!(temp_a3 & 7)) {
            func_8013F3F0(0, 1);
        } else {
            func_8013F3F0(0, -1);
        }
    }
    func_8013F0F4(0x50A, 0x11, 1);
    func_8013E97C(0x32, ((s32) ((D_8015EC78 - D_8015EC74) * D_8015EDB0.unk_04) / 1000) + D_8015EC74, 0x88);
    func_8013E97C(0x33, ((s32) ((D_8015EC78 - D_8015EC74) * D_8015EDB0.unk_04) / 1000) + D_8015EC74, 0x88);
    func_8013E97C(0x36, ((s32) ((D_8015EC80 - D_8015EC7C) * D_8015EDB0.unk_04) / 1000) + D_8015EC7C, 0x50);
    func_8013E97C(0x37, ((s32) ((D_8015EC80 - D_8015EC7C) * D_8015EDB0.unk_04) / 1000) + D_8015EC7C, 0x50);
    var_s0 = &D_8015EBAC;
    var_s1 = 0;
    do {
        temp_v0 = *(s32 *)((unsigned char *)var_s0 + (240));
        func_8013E97C(var_s1 + 0x28, ((s32) ((*(s32 *)((unsigned char *)var_s0 + (280)) - temp_v0) * D_8015EDB0.unk_04) / 1000) + temp_v0, *(s32 *)((unsigned char *)var_s0 + (320)));
        var_s1 += 1;
        var_s0 += 1;
    } while (var_s1 != 8);
    temp_a1 = D_8015EC58.unk_10;
    temp_s1 = D_8015ED08;
    temp_v0_2 = ((s32) ((D_8015ECE0 - D_8015ECB8) * D_8015EDB0.unk_04) / 1000) + D_8015ECB8;
    if ((temp_v0_2 >= temp_a1) && (((D_8015ED08 - D_8015EC58.unk_14) + temp_a1) >= temp_v0_2)) {
        func_8013F0F4(0x50C, 0, 3);
        func_8013E810(0x2F, 4, 1, 1);
        if (D_8015EDC4 & 0x100000) {
            func_8013E97C(0x2F, temp_v0_2, (((temp_s1 * 0) + D_8015EC58.unk_14 + temp_v0_2) - D_8015EC58.unk_10) - (func_800A0070((s32) ((temp_v0_2 - D_8015EC58.unk_10) << 0xB) / (s32) (temp_s1 - D_8015EC58.unk_14)) >> 7));
        } else {
            func_8013E97C(0x2F, temp_v0_2, (((temp_s1 * 0) + D_8015EC58.unk_14 + temp_v0_2) - D_8015EC58.unk_10) - (func_800A0070((s32) ((temp_v0_2 - D_8015EC58.unk_10) << 0xB) / (s32) (temp_s1 - D_8015EC58.unk_14)) >> 6));
        }
    }
    var_v1 = temp_a1 - ((temp_s1 - D_8015EC58.unk_14) * 2);
    if ((var_v1 < temp_v0_2) && (temp_v0_2 < temp_a1)) {
        sp34 = temp_s1 * 0;
        func_8013F0F4(0x50B, 0, 2);
        func_8013E810(0x2F, 4, 1, 1);
        if (D_8015EDC4 & 0x100000) {
            func_8013E97C(0x2F, temp_v0_2, (sp34 + D_8015EC58.unk_14 + ((s32) (D_8015EC58.unk_10 - temp_v0_2) / 2)) - (func_800A0070((s32) ((D_8015EC58.unk_10 - temp_v0_2) << 0xB) / (s32) ((temp_s1 - D_8015EC58.unk_14) * 2)) >> 7));
            var_v1 = D_8015EC58.unk_10 - ((temp_s1 - D_8015EC58.unk_14) * 2);
        } else {
            func_8013E97C(0x2F, temp_v0_2, (sp34 + D_8015EC58.unk_14 + ((s32) (D_8015EC58.unk_10 - temp_v0_2) / 2)) - (func_800A0070((s32) ((D_8015EC58.unk_10 - temp_v0_2) << 0xB) / (s32) ((temp_s1 - D_8015EC58.unk_14) * 2)) >> 6));
            var_v1 = D_8015EC58.unk_10 - ((temp_s1 - D_8015EC58.unk_14) * 2);
        }
    }
    if (temp_v0_2 == var_v1) {
        func_8013E810(0x2F, 5, 1, 1);
    }
    temp_a3_2 = D_8015EDB0.unk_04 + 4;
    D_8015EDB0.unk_04 = temp_a3_2;
    if (temp_a3_2 >= 0x4B1) {
        func_8014EBA8();
    }
}
