void func_8014AB50(s32 arg0) {
    s32 *var_s1;
    s32 *var_s1_2;
    s32 *var_s1_3;
    s32 temp_s0;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    u8 *var_s2;
    u8 *var_s2_2;
    u8 *var_s2_3;

    if (arg0 == 0xA) {
        func_801335A0(0, 0x500);
    }
    if (arg0 < 0x64) {
        func_80147674(0, (s16 *)0x801E5BD0, 0, 0x32, 0x64);
    } else if (arg0 == 0xC7) {
        func_80147674(0, (s16 *)0x801E5BD0, 1, 0x28, 0x28);
        D_80128880[0].z = 0;
        D_80128880->x = 0;
        D_80128880[0].y = 0;
        func_801335A0(0, 0x500);
    } else if ((arg0 >= 0xC8) && (arg0 < 0xF0)) {
        func_80147674(0, (s16 *)0x801E5BD0, 2, 0x32, 0x64);
    } else if ((arg0 >= 0xF0) && (arg0 < 0x118)) {
        func_80147674(0, (s16 *)0x801E5BD0, 3, 0x32, 0x64);
    } else if ((arg0 >= 0x118) && (arg0 < 0x140)) {
        func_80147674(0, (s16 *)0x801E5BD0, 4, 0x32, 0x64);
    } else if ((arg0 >= 0x140) && (arg0 < 0x154)) {
        func_80147674(0, (s16 *)0x801E5BD0, 5, 0x32, 0x5A);
    } else if ((arg0 >= 0x154) && (arg0 < 0x172)) {
        func_80147674(0, (s16 *)0x801E5BD0, 6, 0x32, 0x50);
    } else if ((arg0 >= 0x172) && (arg0 < 0x190)) {
        func_80147674(0, (s16 *)0x801E5BD0, 7, 0x32, 0x46);
    } else if ((arg0 >= 0x190) && (arg0 < 0x1A4)) {
        func_80147674(0, (s16 *)0x801E5BD0, 8, 0x32, 0x3C);
    } else if ((arg0 >= 0x1A4) && (arg0 < 0x1B8)) {
        func_80147674(0, (s16 *)0x801E5BD0, 9, 0x32, 0x32);
    } else if ((arg0 >= 0x1B8) && (arg0 < 0x1C2)) {
        func_80147674(0, (s16 *)0x801E5BD0, 0xA, 0x1E, 0x1E);
    } else if ((arg0 >= 0x1C2) && (arg0 < 0x1D6)) {
        func_80147674(0, (s16 *)0x801E5BD0, 0xB, 0x14, 0x14);
    } else if ((arg0 >= 0x1D6) && (arg0 < 0x1F4)) {
        func_80147674(0, (s16 *)0x801E5BD0, 0xC, 0xA, 0xA);
    } else if (arg0 == 0x1F4) {
        var_s2 = D_8015FFE2;
        var_s0 = 1;
        var_s1 = D_8015FEF8;
        do {
            func_801472D4(var_s0, 1, *(s32 *)((unsigned char *)var_s1 + (-8)), *(s32 *)((unsigned char *)var_s1 + (-4)), (s32) *(u8 *)((unsigned char *)var_s2 + (-2)), (s32) *(u8 *)((unsigned char *)var_s2 + (-1)));
            var_s0 += 1;
            var_s1 += 2;
            var_s2 += 2;
        } while (var_s0 != 0x1F);
        D_8015EDB4[20].unk84[2] = 1U;
        func_8014BC80(0xD, 0);
        func_8014BC80(9, 3);
    } else if ((arg0 >= 0x1F4) && (arg0 < 0x203)) {
        func_80147674(0, (s16 *)0x801E5BD0, 0xD, 5, 5);
    } else if ((arg0 >= 0x203) && (arg0 < 0x22B)) {
        func_80147674(0, (s16 *)0x801E5BD0, 0xE, 0xA, 0xA);
        if (D_8015EDB4[20].unk84[2] != 0) {
            func_8014B790(D_8015EDB4 + 0xA1B, D_8015EDB4 + 0xA1A, 0xD, 0, 3U);
            func_8014B790(D_8015EDB4 + 0x993, D_8015EDB4 + 0x992, 9, 3, 3U);
        }
    } else if ((arg0 >= 0x22B) && (arg0 < 0x23A)) {
        func_80147674(0, (s16 *)0x801E5BD0, 0xF, 5, 0x64);
    } else if ((arg0 >= 0x23A) && (arg0 < 0x267)) {
        func_80147674(0, (s16 *)0x801E5BD0, 0xD, 5, 0x64);
    } else if ((arg0 >= 0x267) && (arg0 < 0x289)) {
        func_80147674(0, (s16 *)0x801E5BD0, 0x10, 0x14, 0xC8);
    } else if (arg0 == 0x289) {
        var_s2_2 = D_8015FFE2;
        var_s0_2 = 1;
        var_s1_2 = D_8015FEF8;
        do {
            func_801472D4(var_s0_2, 1, *(s32 *)((unsigned char *)var_s1_2 + (-8)), *(s32 *)((unsigned char *)var_s1_2 + (-4)), (s32) *(u8 *)((unsigned char *)var_s2_2 + (-2)), (s32) *(u8 *)((unsigned char *)var_s2_2 + (-1)));
            var_s0_2 += 1;
            var_s1_2 += 2;
            var_s2_2 += 2;
        } while (var_s0_2 != 0x1F);
        *(s8 *)&D_8015EDB4[17].unk2 = 1;
        D_8015EDB4[17].unk84[2] = 1U;
    } else if ((arg0 >= 0x28A) && (arg0 < 0x32A)) {
        func_80147674(0, (s16 *)0x801E5BD0, 0x10, 0x14, 0xC8);
        if (D_8015EDB4[17].unk84[2] == 1) {
            func_8014B374(2);
        }
    } else if ((arg0 >= 0x32A) && (arg0 < 0x3B6)) {
        func_80147674(0, (s16 *)0x801E5BD0, 0x10, 0x14, 0x64);
        temp_s0 = func_800AE0D0();
        func_80147C98(0, 0x1B, (s16) ((temp_s0 & 0x3FF) - 0x200), (s16) ((func_800AE0D0() & 0x3FF) - 0x200), 0x7D0, 0x190, 0x81);
    } else if (arg0 == 0x3B6) {
        func_80147674(0, (s16 *)0x801E5BD0, 0x10, 0x14, 0x64);
        var_s2_3 = D_8015FFE2;
        var_s1_3 = D_8015FEF8;
        var_s0_3 = 1;
        do {
            func_801472D4(var_s0_3, 0x1D, *(s32 *)((unsigned char *)var_s1_3 + (-8)), *(s32 *)((unsigned char *)var_s1_3 + (-4)), (s32) *(u8 *)((unsigned char *)var_s2_3 + (-2)), (s32) *(u8 *)((unsigned char *)var_s2_3 + (-1)));
            var_s0_3 += 1;
            var_s1_3 += 2;
            var_s2_3 += 2;
        } while (var_s0_3 != 0x1F);
        D_8015EDB4[17].unk84[2] = 2U;
    } else if ((arg0 >= 0x3B7) && (arg0 < 0x47E)) {
        func_80147674(0, (s16 *)0x801E5BD0, 0x10, 0x14, 0x64);
        if (D_8015EDB4[17].unk84[2] == 2) {
            func_8014B4B4(2);
        }
    }
}
