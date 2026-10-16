extern ? D_800B4390;
extern ? D_800B58B0;
extern s8 D_80123122;
extern s8 D_80123123;
extern u8 D_80123124;
extern ? D_80123164;
extern ? D_80123180;
extern ? D_801231A0;
extern ? D_80123320;
extern ? D_80124220;

s32 func_80054884(s32 arg0) {
    u8 sp54;
    s32 sp50;
    u8 sp47;
    ? sp46;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_s0;
    s32 var_s0_2;
    u8 var_a3;

    var_s0 = 0;
    func_80054C4C();
    func_80056070(&sp54, arg0);
    close(D_800B58E4);
    temp_v0 = open(&sp54, 0x8002);
    D_800B58E4 = temp_v0;
    if (temp_v0 == -1) {
loop_1:
        var_s0 += 1;
        if (func_80054694(arg0) == -1) {
            return -1;
        }
        if (var_s0 >= 0x10) {
            return -1;
        }
        temp_v0_2 = open(&sp54, 0x8002);
        D_800B58E4 = temp_v0_2;
        if (temp_v0_2 != -1) {
            goto block_7;
        }
        goto loop_1;
    }
block_7:
    strncpy((s32) D_80123120, (s32) "SC", 2);
    D_80123122 = 0x13;
    D_80123123 = 1;
    bzero(&D_80123124, 0x40);
    strncpy((s32) &D_80123124, (s32) "ときめきメモリアル", 0x14);
    if (arg0 != 0) {
        strncat(&D_80123124, *(u8 **)((unsigned char *)(&D_800B32CC[arg0]) + (-4)), 6);
    } else {
        strncat(&D_80123124, "・システム", 0xC);
    }
    bzero(&D_80123164, 0x1C);
    func_8006C988((s32) &sp47, (s32) &sp46, arg0);
    var_a3 = sp47;
    if ((s32) var_a3 >= 0xD) {
        var_a3 = 0;
    }
    sp50 = (s32) var_a3;
    memcpy((s32) &D_80123180, (s32) ((var_a3 << 5) + &D_800B4390), 0x20);
    memcpy((s32) &D_801231A0, *(&D_800B58B0 + (var_a3 * 4)), 0x180);
    if (arg0 != 0) {
        func_8005557C(0);
    } else {
        func_80054FC0(0);
    }
    M2C_MEMCPY_ALIGNED(&D_80124220, &D_80123320, 0xF00);
    var_s0_2 = 1;
    if (write(D_800B58E4, (s32) D_80123120, 0x2000) == -1) {
loop_16:
        if (var_s0_2 >= 0x10) {
            return -1;
        }
        var_s0_2 += 1;
        if (write(D_800B58E4, (s32) D_80123120, 0x2000) != -1) {
            goto block_19;
        }
        goto loop_16;
    }
block_19:
    return 1;
}
