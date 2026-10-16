void func_801505B8(void) {
    s32 temp_a1;
    s32 temp_a1_2;
    s32 var_a1;
    u32 var_v0;
    u8 *var_v0_2;
    u8 temp_t1;

    switch ((s32) D_8015EDAC) {
    case 0x0:
        func_800AE0F0(D_8015E74C, "　　　　　　　脱力漫才　　　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_80144480();
        func_8013EEAC(D_8015EBAC);
        func_80148F70();
        func_801513C8();
        func_8014EBA8();
        func_8014EBA8();
        break;
    case 0x2:
        func_8013D418();
        break;
    case 0x3:
        func_80151108();
        break;
    case 0x4:
        if (D_8015EB98 == 0x08000000) {
            var_v0 = 0xC8;
block_50:
            D_8015EDAC = var_v0;
        } else {
            func_80151380(D_8015EDB0.unk_04);
            temp_a1 = D_8015EDB0.unk_04 + 4;
            D_8015EDB0.unk_04 = temp_a1;
            if (temp_a1 >= 0xE1) {
                func_8014EBA8();
            }
        }
        break;
    case 0x5:
        func_8013D350();
        var_v0_2 = D_8011ECD0;
        if (D_8015EDC4 & 0x100000) {
            do {
                temp_t1 = *(u8 *)((unsigned char *)var_v0_2 + (10135));
                var_v0_2 += 0x44;
                *(s8 *)((unsigned char *)var_v0_2 + (10067)) = (s8) (temp_t1 & ~1);
            } while (var_v0_2 != D_8011ED9C);
        }
        func_8014EBA8();
        break;
    case 0x6:
    case 0x7:
    case 0x8:
    case 0x9:
    case 0xA:
    case 0xB:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
        func_80150AA4();
        break;
    default:
        D_8015EDAC = 0x64;
        /* fallthrough */
    case 0x64:
        func_80151380(0xE0 - D_8015EDB0.unk_04);
        temp_a1_2 = D_8015EDB0.unk_04 + 4;
        D_8015EDB0.unk_04 = temp_a1_2;
        if (temp_a1_2 >= 0xE1) {
            func_801515F0();
            func_8014EBA8();
        }
        break;
    case 0x65:
        func_8014ED44(0x14);
        break;
    case 0x66:
        func_8013D59C();
        break;
    case 0x67:
        func_8014EBEC();
        break;
    case 0xC8:
        func_8014ED44(0x3C);
        break;
    case 0xC9:
        func_800AE0F0(D_8015E74C, "ロボ「ガガッ・・理解不能 ");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_8014EBA8();
        break;
    case 0xCA:
        func_8014EE3C(0);
        break;
    case 0xCB:
        func_8013D3E4();
        func_8014EBA8();
        break;
    case 0xCC:
        func_8014ED44(0x3C);
        break;
    case 0xCD:
        func_801504E0();
        break;
    case 0xCE:
        func_8013E97C(0x38, 0x140, 0x5E);
        func_8013E810(0x38, 1, 1, 1);
        func_8013E97C(0x32, 0x140, 0x60);
        func_8013E810(0x32, 6, 5, 1);
        D_80121533 |= 1;
        D_80121531 = 4;
        D_80121399 = 4;
        func_8014EBA8();
        break;
    case 0xCF:
        D_80121556 -= 0x10;
        D_801213BE -= 0x10;
        var_a1 = D_8015EDB0.unk_04 + 1;
        if (var_a1 == 2) {
            D_8015EDB0.unk_04 = var_a1;
            func_8013F0F4(0x513, 0, 5);
            func_8013E7C0(0x34, 0xC, 1, 1);
            func_8013E7C0(0x33, 8, 1, 1);
            var_a1 = D_8015EDB0.unk_04;
            D_801213B0 = 2;
        }
        if (var_a1 == 0xA) {
            D_80121531 = 6;
            D_80121399 = 6;
        }
        D_8015EDB0.unk_04 = var_a1;
        if (var_a1 >= 0x16) {
            func_8014EBA8();
        }
        break;
    case 0xD0:
        if (D_8015EDB0.unk_08 == 0x14) {
            func_8013E7C0(0x33, 0xFF, 1, 0);
        }
        func_8014ED44(0x3C);
        break;
    case 0xD1:
        func_8013E7C0(0x34, 2, 1, 1);
        func_8013E7C0(0x33, 0xFF, 1, 0);
        var_v0 = 0x65;
        goto block_50;
    }
}
