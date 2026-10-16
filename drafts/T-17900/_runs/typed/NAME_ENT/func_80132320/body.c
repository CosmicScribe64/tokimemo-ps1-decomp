void func_80132320(void) {
    s32 var_a0;
    u8 **var_v0;
    u8 *temp_t3;
    u8 temp_a1;

    if (D_800E6280.unk_110D == 0) {
        if (cd_read_sync_status() & 1) {
            var_a0 = 2;
            *(s32 *)((unsigned char *)D_8014CE68 + (4)) = (s32) (*(s32 *)((unsigned char *)D_8014CE68 + (4)) + 0x801E0000);
            (*(u8 * *)D_8014CE68) += 0x801E0000;
            var_v0 = D_8014CE68 + 8;
            do {
                var_a0 += 4;
                temp_t3 = (*(u8 * *)var_v0) + 0x801E0000;
                *(s32 *)((unsigned char *)var_v0 + (12)) = (s32) (*(s32 *)((unsigned char *)var_v0 + (12)) + 0x801E0000);
                *(s32 *)((unsigned char *)var_v0 + (8)) = (s32) (*(s32 *)((unsigned char *)var_v0 + (8)) + 0x801E0000);
                *(s32 *)((unsigned char *)var_v0 + (4)) = (s32) (*(s32 *)((unsigned char *)var_v0 + (4)) + 0x801E0000);
                var_v0 += 4;
                *(u8 **)((unsigned char *)var_v0 + (-16)) = temp_t3;
            } while (var_a0 != 0x32);
            func_80132A38();
            *(u8 *)((unsigned char *)D_801217D0 + (2321)) = 0U;
            *(u8 *)((unsigned char *)D_801217D0 + (2320)) = 0U;
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            *(u8 *)((unsigned char *)D_801217D0 + (2322)) = 0U;
            D_800E6280.unk_03A = 0;
        }
    } else {
        temp_a1 = (D_800B593C + 4) & 0xFF;
        *(u8 *)((unsigned char *)D_801217D0 + (2321)) = temp_a1;
        *(u8 *)((unsigned char *)D_801217D0 + (2320)) = temp_a1;
        D_800B593C = temp_a1;
        *(u8 *)((unsigned char *)D_801217D0 + (2322)) = temp_a1;
        D_800E6280.unk_03A = temp_a1;
        if (temp_a1 >= 0x80U) {
            func_80057640(0xF0);
            func_801324AC();
            D_801217D0[64].unk_00 = 0x80000000;
            D_801217D0[64].unk_04 = -0x18;
            func_8006BD6C(1);
            back_clear_switch(0);
            func_80042808();
        }
    }
    func_8004B19C((s32) D_800B593C, 0, 0);
    func_8004B19C((s32) *(u8 *)((unsigned char *)D_801217D0 + (2320)), 0, 0x10);
}
