void func_8013EA60(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    s32 temp_v0_8;
    s32 temp_v0_9;

    switch (arg1) {
    case 0:
        ((DisplayActor44 *)D_8011ECD0)[155].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[155].unk_03 & 0xFFFE);
        *D_8015EBCC = 0;
        temp_v0 = D_8015EBC0 - arg2;
        D_8015EBC0 = temp_v0;
        if (temp_v0 < 0) {
            D_8015EBC0 = 0;
        }
        if (arg2 == 0) {
            func_8014C360(0, 0x6C, 0x77);
            return;
        }
        func_8014BFE4(arg0, 0x74, 0x77, arg2, arg3);
        return;
    case 1:
        ((DisplayActor44 *)D_8011ECD0)[156].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[156].unk_03 & 0xFFFE);
        D_8015EBD0 = 0;
        temp_v0_2 = D_8015EBC4 - arg2;
        D_8015EBC4 = temp_v0_2;
        if (temp_v0_2 < 0) {
            D_8015EBC4 = 0;
        }
        if (arg2 == 0) {
            func_8014C360(0, 0x33, 0x67);
            return;
        }
        func_8014BFE4(arg0, 0x3B, 0x67, arg2, arg3);
        return;
    case 2:
        ((DisplayActor44 *)D_8011ECD0)[157].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[157].unk_03 & 0xFFFE);
        D_8015EBD4 = 0;
        temp_v0_3 = D_8015EBC8 - arg2;
        D_8015EBC8 = temp_v0_3;
        if (temp_v0_3 < 0) {
            D_8015EBC8 = 0;
        }
        if (arg2 == 0) {
            func_8014C360(0, 0x30, 0x97);
            return;
        }
        func_8014BFE4(arg0, 0x34, 0x97, arg2, arg3);
        return;
    case 3:
        temp_v0_4 = D_8015EDC0 - arg2;
        D_8015EDC0 = temp_v0_4;
        if (temp_v0_4 < 0) {
            D_8015EDC0 = 0;
        }
        if (arg2 == 0) {
            func_8014C360(0, 0x38, 0x98);
            return;
        }
        func_8014BFE4(arg0, 0x40, 0x98, arg2, arg3);
        return;
    case 4:
        temp_v0_5 = D_8015EDC0 - arg2;
        D_8015EDC0 = temp_v0_5;
        if (temp_v0_5 < 0) {
            D_8015EDC0 = 0;
        }
        if (arg2 == 0) {
            func_8014C360(0, 0x38, 0x98);
            return;
        }
        func_8014BFE4(arg0, 0x40, 0x98, arg2, arg3);
        return;
    case 5:
        temp_v0_6 = D_8015EDC0 - arg2;
        D_8015EDC0 = temp_v0_6;
        if (temp_v0_6 < 0) {
            D_8015EDC0 = 0;
        }
        if (arg2 == 0) {
            func_8014C360(0, 0x38, 0x95);
            return;
        }
        func_8014BFE4(arg0, 0x40, 0x95, arg2, arg3);
        return;
    case 16:
        if (arg2 == 0) {
            func_8014C360(0, 0x40, 0x97);
            return;
        }
        func_8014BFE4(arg0, 0x48, 0x97, arg2, arg3);
        return;
    case 17:
        if (arg2 == 0) {
            func_8014C360(0, 0x40, 0x85);
            return;
        }
        func_8014BFE4(arg0, 0x48, 0x85, arg2, arg3);
        return;
    case 18:
        if (arg2 == 0) {
            func_8014C360(0, 0x48, 0x8D);
            return;
        }
        func_8014BFE4(arg0, 0x50, 0x8D, arg2, arg3);
        return;
    case 32:
        temp_v0_7 = D_8015ED94 - arg2;
        D_8015ED94 = temp_v0_7;
        if (temp_v0_7 < 0) {
            D_8015ED94 = 0;
        }
        func_8014BFE4(arg0, 0xF4, 0x96, arg2, arg3);
        return;
    case 33:
        if (arg3 == 1) {
            temp_v0_8 = D_8015ED94 - arg2;
            D_8015ED94 = temp_v0_8;
            if (temp_v0_8 < 0) {
                D_8015ED94 = 0;
            }
        } else {
            temp_v0_9 = D_8015ED94 + arg2;
            D_8015ED94 = temp_v0_9;
            if (D_8015EDA0 < temp_v0_9) {
                D_8015ED94 = D_8015EDA0;
            }
        }
        func_8014BFE4(arg0, 0x10C, 0x66, arg2, arg3);
        return;
    case 35:
        func_8014BFE4(arg0, 0x114, 0x96, arg2, arg3);
        /* fallthrough */
    default:
        return;
    }
}
