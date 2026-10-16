extern ? D_800C3280;
extern ? D_800C32C0;
extern ? D_800C3300;

void func_8013A5F4(void) {
    ? var_a1;
    ? var_a2;
    s32 temp_t7;
    s32 temp_v0;
    u8 temp_t0;

    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_800744A0();
    func_80074950();
    func_8009068C();
    temp_t7 = D_800F5AAC * 4;
    D_800F62CF = *(u8 *)((unsigned char *)(&D_800F53A0 + temp_t7) + (1677));
    D_801474B8 = (u8) D_8012E660;
    temp_t0 = *(u8 *)((unsigned char *)(&D_800F53A0 + temp_t7) + (1677));
    if ((temp_t0 == 2) || (var_a1 = 0xB320, (temp_t0 == 6))) {
        var_a1 = 0xB33B;
        var_a2 = 0xB322;
    } else {
        var_a2 = 0xB2FD;
    }
    temp_v0 = temp_t0 * 4;
    func_800504CC(1, var_a1, var_a2, *(&D_800C3280 + temp_v0), *(&D_800C32C0 + temp_v0), *(&D_800C3300 + temp_v0));
    D_801474B4.unk_00 = *(s32 *)((unsigned char *)(&D_800F53A0.girl[*(u8 *)((unsigned char *)(&D_800F53A0 + (D_800F5AAC * 4)) + (1677))]) + (8));
    D_800F53A0.girl[*(u8 *)((unsigned char *)(&D_800F53A0 + (D_800F5AAC * 4)) + (1677))].unk_0A = 0x32;
    func_8004DDD8();
}
