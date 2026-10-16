? func_8004E93C(?, ?);                              /* extern */
extern s8 D_800E62B6;
extern s8 D_800E62B7;
extern s8 D_800E62B8;
extern s8 D_800E62BA;
extern u8 D_800E738D;

void func_80142E24(void) {
    ? sp5C;
    ? *var_v0;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_v1;

    var_v1 = 0x8018C000;
    var_a2 = 0x80188000;
    var_a1 = 0x80184000;
    var_v0 = &sp5C;
    var_a0 = 0x80180000;
    do {
        *(s32 *)((unsigned char *)var_v0 + (12)) = var_v1;
        var_v1 += 0x10000;
        var_v0->unk0 = var_a0;
        *(s32 *)((unsigned char *)var_v0 + (4)) = var_a1;
        *(s32 *)((unsigned char *)var_v0 + (8)) = var_a2;
        var_a2 += 0x10000;
        var_a1 += 0x10000;
        var_a0 += 0x10000;
        var_v0 += 0x10;
    } while (var_v1 != 0x801AC000);
    switch (D_800E738D) {
    case 0:
        D_800E62BA = 0x80;
        func_800438DC(1, 0, var_a2, 0x801AC000);
        func_80048DAC(1);
        func_8006BD6C(0);
        func_80048DD0(0);
        func_80048E78();
        func_80041584();
        func_8004E58C();
        func_800438F0(1);
        D_800E62B7 = 0;
        D_800E62B6 = 0;
        D_800E62B8 = 0;
        func_80044750(0x7F);
        func_80044890(1, 0xC592, 0xC580, 0xCFA7, 0xCF8B, 0xCF83);
        D_800E738D += 1;
        return;
    case 1:
        if (func_80044E8C() == 1) {
            func_8004500C(1, 0);
            cd_read_sectors(0x21U, 0x80180000, 0x9FD5);
            D_800E738D += 1;
            return;
        }
    default:
        return;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) &sp5C, 0, 4, 2, 2, 0x1FF, -0xA0, -0x78, 8, 9);
            func_80043914(0x80190000, 0x1F, 1, 3, 0);
            D_801220D0 = 0x80000000;
            D_800E738D += 1;
            return;
        }
        break;
    case 3:
        cd_read_sectors(0x21U, 0x80180000, 0xA048);
        D_800E738D += 1;
        return;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) &sp5C, 0, 4, 2, 2, 0x1FE, -0xA0, -0x78, 8, 0xB);
            func_80043914(0x80190000, 0x1E, 1, 3, 0);
            D_801220D0 = 0x80000000;
            D_800E738D += 1;
            return;
        }
        break;
    case 5:
        cd_read_sectors(0x21U, 0x80180000, 0x9FF6);
        D_800E738D += 1;
        return;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) &sp5C, 0, 4, 2, 2, 0x1FD, -0xA0, -0x78, 8, 0xD);
            func_80043914(0x80190000, 0x1D, 1, 3, 0);
            D_801220D0 = 0x80000000;
            D_800E738D += 1;
            return;
        }
        break;
    case 7:
        cd_read_sectors(0x41U, 0x80180000, 0x9F22);
        D_800E738D += 1;
        return;
    case 8:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) &sp5C, 1, 8, 4, 2, 0x1EE, -0x140, -0x78, 8, 0x15);
            func_80043914(0x801A0000, 0xE, 1, 3, 0);
            D_801220F4 = 0x51000000;
            D_80122105 = 0;
            D_80122104 = 0;
            D_800E738D += 1;
            D_80122106 = 0;
            return;
        }
        break;
    case 9:
        cd_read_sectors(0x31U, 0x801B0000, 0x9FA4);
        D_800E738D += 1;
        return;
    case 10:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x41U, 0x80180000, 0x9EE1);
            D_800E738D += 1;
            return;
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) &sp5C, 0, 8, 4, 2, 0x1EF, -0xA0, -0x78, 8, 5);
            func_80043914(0x801A0000, 0xF, 1, 3, 0);
            D_801220E1 = 0;
            D_801220E0 = 0;
            D_801220D0 = 0x51000000;
            D_800E738D += 1;
            D_801220E2 = 0;
            return;
        }
        break;
    case 12:
        cd_read_sectors(0x41U, 0x80180000, 0x9F63);
        D_800E738D += 1;
        return;
    case 13:
        if (cd_read_sync_status() & 1) {
            func_80046290(0x06000BC8, 0x11000CBA, 0xF);
            func_80044750(0x300);
            func_8004E58C();
            func_8004E788(-0x90, 0x14, 0, (s32) "始まりがあれば終わりがあるように、", 0);
            func_8004E788(-0x90, 0x24, 0, (s32) "出会いがあればまた、別れもあるのです。", 0);
            func_8004E788(-0x90, 0x34, 0, (s32) "永遠に続く二人の関係、", 0);
            func_8004E788(-0x90, 0x44, 0, (s32) "それはどんなに幸せなことでしょう。", 0);
            func_8004E884(4);
            func_8004E93C(0, 0);
            func_8004E93C(1, 0);
            func_8004E93C(2, 0);
            func_8004E93C(3, 0);
            func_8004284C();
        }
        break;
    }
}
