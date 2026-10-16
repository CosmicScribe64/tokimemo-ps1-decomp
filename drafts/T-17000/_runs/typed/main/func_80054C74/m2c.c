extern ? D_800B4390;
extern s32 D_800B58B0;
extern s8 D_80123122;
extern s8 D_80123123;
extern u8 D_80123124;
extern ? D_80123164;
extern ? D_80123180;
extern ? D_801231A0;
extern ? D_80123320;
extern ? D_80124220;

s32 func_80054C74(s32 arg0) {
    u8 sp44;
    s32 temp_v0;
    s32 var_s0;
    u8 *var_s1;

    func_80054C4C();
    func_80056070(&sp44, arg0);
    close(D_800B58E4);
    temp_v0 = open(&sp44, 2);
    D_800B58E4 = temp_v0;
    if (temp_v0 == -1) {
        return -1;
    }
    strncpy((s32) D_80123120, (s32) "SC", 2);
    D_80123122 = 0x13;
    D_80123123 = 1;
    bzero(&D_80123124, 0x40);
    strcpy(&D_80123124, "ときめきメモリアル");
    if (arg0 != 0) {
        strcat(&D_80123124, *(u8 **)((unsigned char *)(&D_800B32CC[arg0]) + (-4)));
    } else {
        strcat(&D_80123124, "・システム");
    }
    bzero(&D_80123164, 0x1C);
    memcpy((s32) &D_80123180, (s32) &D_800B4390, 0x20);
    memcpy((s32) &D_801231A0, D_800B58B0, 0x180);
    if (arg0 != 0) {
        func_8005557C(0);
    } else {
        func_80054FC0(0);
    }
    M2C_MEMCPY_ALIGNED(&D_80124220, &D_80123320, 0xF00);
    var_s1 = D_80123120;
    var_s0 = 0;
loop_9:
    var_s0 += 0x2000;
    if (write(D_800B58E4, (s32) var_s1, 0x2000) != 0x2000) {
        close(D_800B58E4);
        return -1;
    }
    var_s1 += 0x2000;
    if (var_s0 == 0x2000) {
        return 1;
    }
    goto loop_9;
}
