void func_8014E5F8(s32 arg0) {
    s32 var_at;

    if (arg0 < 0x13) {
        func_80146F74(0, 0, 0, -0x190, 0, 0, 0, 0x32, 0x32);
    }
    if (arg0 == 0x13) {
        *(u8 *)((unsigned char *)D_8015EDB4 + (3126)) = 1U;
        func_8014C2E4();
    }
    if ((arg0 >= 0x14) && (arg0 < 0x27)) {
        if (*(u8 *)((unsigned char *)D_8015EDB4 + (3126)) != 0) {
            func_8014BF38(0);
        }
    }
    if (arg0 == 0x27) {
        D_80122760 = 0;
        D_80122764 = 0;
        D_80122768 = -0x64;
        D_8012276C = 0xFF;
        D_8012276D = 0xFF;
        D_8012276E = 0xFF;
        func_8009AD70(0, (s32) &D_80122760);
        D_80122770 = 0;
        D_80122774 = 0;
        D_80122778 = -0x64;
        D_8012277C = 0xFF;
        D_8012277D = 0xFF;
        D_8012277E = 0xFF;
        func_8009AD70(1, (s32) &D_80122770);
        D_80122780 = 0;
        D_80122784 = 0;
        D_80122788 = -0x64;
        D_8012278C = 0xFF;
        D_8012278D = 0xFF;
        D_8012278E = 0xFF;
        func_8009AD70(2, (s32) &D_80122780);
    }
    if ((arg0 >= 0x28) && (arg0 < 0x43)) {
        func_80146F74(0, 0, 0, 0x320, 0, 0, 0, 0x14, 0x1E);
    }
    if ((arg0 >= 0x46) && (arg0 < 0x64)) {
        func_80146F74(0, 0, 0, -0x1E0, 0, 0, 0, 0x32, 0x32);
    }
    if (arg0 == 0x64) {
        *(s8 *)((unsigned char *)D_8015EDB4 + (3262)) = 1;
        *(u8 *)((unsigned char *)D_8015EDB4 + (2994)) = 0U;
        *(u8 *)((unsigned char *)D_8015EDB4 + (2995)) = (u8) *(u8 *)((unsigned char *)D_8015EDB4 + (2994));
        D_80122760 = -0x64;
        D_80122764 = 0x1E;
        D_80122768 = -0x1E;
        D_8012276C = 0xE0;
        D_8012276D = 0xE0;
        D_8012276E = 0xE0;
        func_8009AD70(0, (s32) &D_80122760);
        D_80122770 = 0x28;
        D_80122774 = 0x32;
        D_80122778 = -0x64;
        D_8012277C = 0x40;
        D_8012277D = 0x40;
        D_8012277E = 0x40;
        func_8009AD70(1, (s32) &D_80122770);
        D_80122780 = 0xA;
        D_80122784 = -0x14;
        D_80122788 = -0x64;
        D_8012278C = 0x30;
        D_8012278D = 0x30;
        D_8012278E = 0x30;
        func_8009AD70(2, (s32) &D_80122780);
    }
    if ((arg0 >= 0x65) && (arg0 < 0x79)) {
        var_at = arg0 < 0x42;
        if (*(u8 *)((unsigned char *)D_8015EDB4 + (3126)) != 0) {
            func_8014BF38(0);
            goto block_23;
        }
    } else {
block_23:
        var_at = arg0 < 0x42;
    }
    if ((var_at == 0) && (arg0 < 0x48)) {
        *(s8 *)((unsigned char *)D_8015EDB4 + (2177)) = 0x40;
        *(s16 *)((unsigned char *)D_8015EDB4 + (2296)) = 0x800;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2308)) = 0xA;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2313)) = 0x40;
        *(s16 *)((unsigned char *)D_8015EDB4 + (2432)) = 0x800;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2444)) = 0x1E;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2449)) = 0x40;
        *(s16 *)((unsigned char *)D_8015EDB4 + (2568)) = 0x800;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2580)) = 0x1E;
        return;
    }
    if (arg0 == 0x48) {
        *(s8 *)((unsigned char *)D_8015EDB4 + (2177)) = 0x80;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2313)) = 0x80;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2449)) = 0x80;
    }
}
