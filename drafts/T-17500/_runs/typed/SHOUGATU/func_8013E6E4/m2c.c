extern s32 D_80122D48;
extern u8 D_80146054;

void func_8013E6E4(void) {
    ? sp50;
    ? sp40;
    s16 temp_s0;
    s16 temp_t5;
    s16 var_s0;
    s16 var_s0_2;
    s16 var_s0_3;
    s16 var_s1;
    s32 temp_lo;
    s32 temp_v0;
    s32 var_v0;
    u32 temp_v1;
    u8 temp_t7;

    sp50.unk0 = (s32) D_801460EC.unk0;
    *(s32 *)((unsigned char *)(&sp50) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801460EC) + (8));
    *(s32 *)((unsigned char *)(&sp50) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801460EC) + (4));
    *(u16 *)((unsigned char *)(&sp50) + (12)) = (u16) D_801460EC.v[6];
    sp40.unk0 = (s32) D_801460FC.unk0;
    *(s32 *)((unsigned char *)(&sp40) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801460FC) + (8));
    var_s1 = 0;
    var_s0 = 0;
    *(s32 *)((unsigned char *)(&sp40) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801460FC) + (4));
    *(u16 *)((unsigned char *)(&sp40) + (12)) = (u16) D_801460FC.v[6];
    do {
        temp_lo = var_s0 * 0x38;
        var_s0 += 1;
        var_s1 += ((u32) (D_800E6280.unk_1BC[0].unk_0C.b[temp_lo] << 0x1E) >> 0x1F) != 0;
    } while (var_s0 < 0xB);
    var_s0_2 = 0;
loop_3:
    temp_t7 = ((u32) (D_800E6280.unk_10F8 + var_s0_2) % 10U) + 1;
    D_800E6280.unk_F5F = temp_t7;
    func_800AE0B0("num%d girl%d\n", var_s1, temp_t7 & 0xFF);
    temp_v1 = (u32) (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.w << 0x1E) >> 0x1F;
    var_s0_2 += 1;
    if (temp_v1 == 0) {
        if ((u8) D_800E6280.unk_F5F >= 9U) {
            goto loop_3;
        }
    }
    if ((D_800E6280.unk_F5F == 5) && (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 8)) {
        goto loop_3;
    }
    if ((D_800E6280.unk_F5F == 6) && (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 9)) {
        goto loop_3;
    }
    if ((temp_v1 == 0) && (var_s1 >= 2)) {
        goto loop_3;
    }
    if (D_80146050 != 0) {
        D_800E6280.unk_F5F = 9;
    }
    D_80146054 = D_800E6280.unk0;
    D_80145F2C = 0;
    temp_v0 = func_80051A68(D_800E6280.unk0);
    if ((D_800E6280.unk_F5F == 6) && ((u32) (temp_v0 & 0x7F) >= 2U)) {
        D_80145F2C += 1;
    }
    if ((temp_v0 & 0x7F) == 4) {
        D_80145F2C += 1;
    }
    func_8013EE30();
    func_800847B8(D_800E6280.unk_F5F);
    if (!(D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.w & 2) || ((D_80146050 != 0) && (D_800E6280.unk_F5F == 9))) {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0] |= 2;
        D_800E6280.unk_720 = D_800E6280.unk0;
        D_800E6280.unk_721 = D_800E6280.unk0;
        D_800E6280.unk_722 = 0x11;
        D_80122D48 = 3;
        if (D_80146050 != 0) {
            func_80085B3C(8, D_800E6280.unk_F5F * 0xA);
        } else {
            func_80085B3C(4, 0);
        }
        func_80042908(5);
        return;
    }
    var_s0_3 = 0;
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 4;
loop_26:
    var_v0 = var_s0_3 * 2;
    temp_s0 = var_s0_3 + 1;
    if (D_800E6280.unk_114.unk_02 >= *(s16 *)((unsigned char *)(sp + var_v0) + (80))) {
        temp_t5 = temp_s0;
        var_s0_3 = temp_t5;
        if (temp_t5 >= 7) {
            var_v0 = temp_t5 * 2;
        } else {
            goto loop_26;
        }
    }
    D_800E6280.unk_1BC[D_800E6280.unk0].unk_02 += *(s16 *)((unsigned char *)(sp + var_v0) + (64));
    func_80085B3C(4, 0);
    func_80084D3C();
    func_8004284C();
}
