void func_80136DEC(void) {
    EnHalf22 frames;
    EnHalf22 xs;
    EnHalf22 ys;
    EnBytes22 kinds;
    s32 offset;
    s32 i;
    DisplayActor44 *p;

    frames = D_80139C68;
    xs = D_80139C94;
    ys = D_80139CC0;
    kinds = D_80139CEC;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80048514(5, ((s32 *)D_801391E4)[7], 1);
        for (offset = 0; offset < 0x1C; offset += 4) {
            func_800485BC(*(s32 *)(D_801391E4 + offset), 1);
        }
        for (offset = 0x20; offset != 0x28; offset += 4) {
            func_800485BC(*(s32 *)(D_801391E4 + offset), 1);
        }
        func_8004284C();
        return;
    case 1:
        p = (DisplayActor44 *)D_8011ECD0;
        for (i = 0; i < 22; i++) {
            func_80048F64(i + 96);
            p[96 + i].unk_38 = 0x01000000;
            p[96 + i].unk_07 = 0;
            p[96 + i].unk_02 = 0x40;
            p[96 + i].unk_03 = 4;
            p[96 + i].unk_01 = kinds.value[i];
            p[96 + i].unk_0C = (u8 *)D_801391E8;
            p[96 + i].unk_10 = (u8 *)D_801391EC;
            p[96 + i].unk_34 = (u8 *)D_801391E4;
            p[96 + i].unk_04 = 0;
            p[96 + i].unk_05 = 0x80;
            p[96 + i].unk_1A = 0x1000;
            p[96 + i].unk_1C = 0x1000;
            p[96 + i].unk_06 = 0;
            p[96 + i].unk_14 = D_801391F0;
            p[96 + i].unk_18 = frames.value[i];
            p[96 + i].unk_26 = xs.value[i];
            p[96 + i].unk_2A = ys.value[i];
        }
        for (i = 0; i < 21; i++) {
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_03 |= 0x80;
        }
        func_8004284C();
        return;
    case 2:
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_07 >= 0x80U) {
            D_800E6280.unk_03A = 0x80;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0x80;
            func_8004284C();
        } else {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_07 += 4;
        }
        for (i = 1; i < 22; i++) {
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_07 = ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
        }
        return;
    case 3:
        switch (D_800E6280.unk_110D) {
        case 0:
            func_80046290(0x07000002, 0x03000008, 0xE);
            func_80044750(0x300);
            D_800E6280.unk_110D += 1;
            return;
        case 1:
        case 3:
            if (func_800460EC() == 4) {
                D_800E6280.unk_110D += 1;
                return;
            }
        default:
            return;
        case 2:
            if (func_800460EC() == 2) {
                func_80046290(0x08000001, 0x02000004, 0xE);
                func_80044750(0x300);
                D_800E6280.unk_110D += 1;
                return;
            }
            break;
        case 4:
            if (func_800460EC() == 2) {
                D_80139C58 = 0;
                D_80139C30 = 5;
                *D_80139C34 = 0;
                *D_80139C3C = 0;
                *D_80139C44 = 0;
                D_80139C44[2] = 0;
                D_80139C44[3] = 0;
                D_80139C44[4] = 0;
                D_80139C3C[2] = 0;
                D_80139C3C[3] = 0;
                D_80139C3C[4] = 0;
                D_80139C34[2] = 0;
                D_80139C34[3] = 0;
                D_80139C34[4] = 0;
                D_80139C34[1] = 0;
                D_80139C3C[1] = 0;
                D_80139C44[1] = 0;
                ((DisplayActor44 *)D_8011ECD0)->unk_26 = 0;
                ((DisplayActor44 *)D_8011ECD0)->unk_2A = 0;
                func_80042808();
                return;
            }
            break;
        }
        break;
    default:
        func_80046500();
        break;
    }
}
