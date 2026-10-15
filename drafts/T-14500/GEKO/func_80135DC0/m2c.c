? func_800639D8();                                  /* extern */
? func_80065048(s16, ?);                            /* extern */
extern s8 D_800E62BA;
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E62C1;
extern u16 D_800E6374;
extern ? D_800E644A;
extern s32 D_800E67CC;
extern u8 D_800E699C;
extern u8 D_800E699E;
extern u8 D_800E71DF;
extern u8 D_800E71E8;
extern s8 D_800E7322;
extern s32 D_800E7368;
extern u32 D_800E7378;
extern s32 D_80145078;
extern s16 D_80145080;
extern s16 D_80145084;
extern s8 D_80145090;

s32 func_80135DC0(s32 arg0) {
    s16 sp62;
    u8 sp3F;
    s16 sp3A;
    s16 sp38;
    s16 sp36;
    s16 sp34;
    s16 var_a0;
    s32 temp_t5;
    s32 temp_t7;
    s32 var_t9;
    s32 var_v0;
    u32 temp_v1;
    u8 temp_a1;
    u8 temp_t9;
    u8 temp_v0;
    u8 temp_v0_2;

    temp_t9 = D_800E6280.unk_69C[D_800E699C].unk_01;
    D_800E71DF = temp_t9;
    if (((temp_t9 == 0) && (D_800E699E & 1)) || ((D_800E71DF != 0) && (D_800E699E & 2))) {
        func_80135AC8(arg0);
        var_v0 = 0;
    } else if (!(D_800E6280.unk_1BC[D_800E71DF].unk_0C.w & 0x40) && (D_800E71DF != 0)) {
        func_80135AC8(1);
        var_v0 = 0;
    } else {
        D_80145074 = (D_800E699E & 0x10) != 0;
        func_800438DC(1, 0);
        func_80048DAC(1);
        func_80041584();
        func_80048EB8(0);
        func_800438F0(1);
        func_8006509C();
        func_800639D8();
        func_800639D8();
        var_a0 = 0;
        do {
            sp62 = var_a0;
            func_80065048(var_a0, 0);
            var_a0 += 1;
        } while (var_a0 < 0xF);
        func_80065B0C(1);
        func_8004E58C();
        D_8014507C = 0;
        D_80145090 = 0;
        D_80145088 = 1;
        if ((D_800E62BF != 0xC) || (D_800E62C0 != 0x18)) {
            if ((D_800E62C1 != 6) && (temp_v0 = D_800E6280.unk_044[0].unk_20[D_800E62C0], (temp_v0 != 0xA)) && (temp_v0 != 0x10) && (temp_v0_2 = D_800E6280.unk_044[0].unk_00[D_800E62C0], (temp_v0_2 != 1)) && (temp_v0_2 != 2) && (temp_v0_2 != 3) && (D_80145074 == 0)) {
                D_80145088 = 0;
            } else if ((((u8) D_800E71E8 >> 4) == 4) && (((u16) D_800E6374 >> 0xC) == ((u8) *(&D_800E644A + (D_800E71DF * 0x38)) >> 4)) && ((appointment_find_today((s32) D_800E62BE, (s32) D_800E62BF, (s32) D_800E62C0) <= 0) || (D_800E71DF != ((u32) (D_800E67CC * 8) >> 0x1C)))) {
                D_80145088 = 0;
                D_80145090 = 1;
            }
        }
        D_8014508C = 0;
        func_800649D4();
        func_80064E84();
        func_80048390();
        func_800AE0F0(D_800CA19C, "廊下");
        func_800AE0F0(D_800CA1DC, "近所の公園");
        func_8004E9F4(1);
        func_8008585C();
        func_80084E4C();
        D_80122D2C = 0;
        D_800E7322 = 0;
        D_800E7368 = 1;
        if (D_800E71DF == 0) {
            D_800E699E |= 1;
        } else {
            D_800E699E |= 2;
        }
        D_800E62BA = 0x80;
        D_800B593C = 0x80;
        D_800B5940 = 0;
        func_8007C740();
        D_80145080 = 0;
        D_80145084 = ((u32) D_800E7378 % 3U) + 2;
        temp_a1 = func_80051A68(D_800E71DF) & 0x7F;
        temp_v1 = temp_a1 & 0xFF;
        temp_t5 = temp_v1 < 2U;
        temp_t7 = temp_v1 < 3U;
        D_80145060 = 0;
        D_80145064 = 0;
        D_800CA148 = (temp_t5 ^ 1) + (temp_t7 ^ 1) + (temp_v1 >= 4U) + 0x1E;
        sp3F = temp_a1;
        D_800CA14C = 0;
        func_800847B8(D_800E71DF);
        func_80134D10();
        switch (D_800E71DF) {
        case 0:
            func_800AE0F0(D_800CA1DC, "自宅前");
            D_80145048 = D_80144F14;
            D_8014504C = D_80144F48;
            D_80145050 = D_80144F7C;
            D_800CA160 = D_80144F14;
            D_800CA164 = D_80144F48;
            D_800CA168 = D_80144F7C;
            break;
        case 1:
            D_80145048 = D_80144F24;
            D_8014504C = D_80144F58;
            D_80145050 = D_80144F8C;
            break;
        case 2:
            D_80145048 = D_80144F18;
            D_8014504C = D_80144F4C;
            D_80145050 = D_80144F80;
            D_800CA160 = D_80144F18;
            D_800CA164 = D_80144F4C;
            D_800CA168 = D_80144F80;
            break;
        case 3:
            D_80145048 = D_80144F1C;
            D_8014504C = D_80144F50;
            D_80145050 = D_80144F84;
            break;
        case 4:
            D_80145048 = D_80144F38;
            D_8014504C = D_80144F6C;
            D_80145050 = D_80144FA0;
            break;
        case 5:
            D_80145048 = D_80144F28;
            D_8014504C = D_80144F5C;
            D_80145050 = D_80144F90;
            break;
        case 6:
            D_80145048 = D_80144F30;
            D_8014504C = D_80144F64;
            D_80145050 = D_80144F98;
            D_800CA160 = D_80144F30;
            D_800CA164 = D_80144F64;
            D_800CA168 = D_80144F98;
            if (sp3F < 2U) {
                D_80145048 = D_80144F2C;
                D_8014504C = D_80144F60;
                D_80145050 = D_80144F94;
                D_800CA160 = D_80144F2C;
                D_800CA164 = D_80144F60;
                D_800CA168 = D_80144F94;
            }
            break;
        case 7:
            D_80145048 = D_80144F20;
            var_t9 = D_80144F88;
            D_8014504C = D_80144F54;
block_42:
            D_80145050 = var_t9;
            break;
        case 8:
            D_80145048 = D_80144F10;
            D_8014504C = D_80144F44;
            D_80145050 = D_80144F78;
            break;
        case 9:
            D_80145048 = D_80144F34;
            D_8014504C = D_80144F68;
            D_80145050 = D_80144F9C;
            break;
        case 10:
            D_80145048 = D_80144F40;
            var_t9 = D_80144FA8;
            D_8014504C = D_80144F74;
            goto block_42;
        }
        if ((u8) D_800E62BF >= 4U) {
            D_8014506C = D_800E62BE - 0x5F;
        } else {
            D_8014506C = D_800E62BE - 0x60;
        }
        D_80145070 = 0;
        D_80145078 = 0;
        if (((D_800E62BE != 0x5F) || (D_800E62BF != 6) || (D_800E62C0 != 3)) && ((D_800E62BE != 0x60) || (D_800E62BF != 6) || (D_800E62C0 != 1))) {
            if (((D_800E62BE != 0x61) || (D_800E62BF != 6) || (D_800E62C0 != 7)) && ((D_800E62BE != 0x5F) || (D_800E62BF != 0xA) || (D_800E62C0 != 0xE)) && ((D_800E62BE != 0x60) || (D_800E62BF != 0xA) || (D_800E62C0 != 0xC))) {
                if ((D_800E62BE == 0x61) && (D_800E62BF == 0xA) && (D_800E62C0 == 0xB)) {
                    goto block_64;
                }
            } else {
                goto block_65;
            }
        } else {
block_64:
block_65:
            sp34 = 0x140;
            sp36 = 0x1A0;
            sp38 = 0x140;
            sp3A = 0x58;
            func_8009C7F8((RECT *) &sp34, 8, 0, 0);
            sp34 = 0x2C0;
            sp36 = 0xA0;
            func_8009C7F8((RECT *) &sp34, 8, 0, 0);
        }
        var_v0 = func_8004284C();
        if ((D_80145074 != 0) || (D_800E699E & 0x20)) {
            var_v0 = func_80042940(0x43);
        }
        if (D_80145088 != 0) {
            var_v0 = func_80042940(0x43);
        }
    }
    return var_v0;
}
