void func_8013DF88(void) {
    s32 sp34[1];
    u8 *var_v0;

    switch (D_800E738D) {
    case 0:
        func_8004E58C();
        func_800AE0F0(&sp34, *(void **)((unsigned char *)(&D_800B32CC[D_800E7398]) + (-4)));
        func_800AE100(&sp34, "番アルバム　と、");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) &sp34, 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "システムファイルを", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) "作成しています。", 0);
        k_disp_start(3);
        D_800E738D += 1;
    default:
        break;
    case 1:
        if (func_80054694((s32) D_800E7398) == 1) {
            D_800E738D += 1;
        } else {
            func_80042908(4);
        }
        break;
    case 2:
        if (func_80054694(0) == 1) {
            D_800E738D += 1;
        } else {
            func_80054704((s32) D_800E7398);
            func_80042908(4);
        }
        break;
    case 3:
        func_80053D10();
        D_800E7D36 = D_800E7398;
        *D_800E7D14 = 0;
        *D_800E7D15 = 0;
        var_v0 = D_800E7D13;
        D_800E7D16 = 0;
        do {
            var_v0 += 4;
            (*(s32 *)var_v0) = 0;
            *(s8 *)((unsigned char *)var_v0 + (1)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (2)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (3)) = 0;
        } while (var_v0 != &D_800E7D1F);
        D_800E7D14[D_800E7398] = 1;
        *D_800E7D14 = 1;
        D_800E7398 = 0;
        func_80053CE0();
        func_80054E6C();
        D_800E738D += 1;
        break;
    case 4:
        if (func_80054388() == 0x13) {
            func_80053D10();
            D_800E738D += 1;
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 5:
        func_80053CE0();
        func_80041878();
        func_80042058();
        func_80042058();
        D_800E7398 = D_800E7D36;
        func_800AE0B0("s->file_no %x \n", D_800E7D36);
        D_800E738D += 1;
        break;
    case 6:
        if (func_80054590() == 0x14) {
            D_800E7D14[D_800E7398] = 1;
            D_800E738D += 1;
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 7:
        func_80053D10();
        func_8013B77C();
        main_state_set(0x14);
        break;
    }
}
