? func_8004D088(?, ?, ?, ?);                        /* extern */
? func_8004E350(?, s32, ?);                         /* extern */
? func_800862D0();                                  /* extern */
s32 func_800862F0();                                /* extern */
? func_80086350();                                  /* extern */
? func_80086370();                                  /* extern */
extern s8 D_800E62BA;
extern s32 D_800E7208;
extern s32 D_800E7374;
extern u8 D_800E738A;
extern s32 D_800E74BC;
extern s32 D_800E74C0;
extern s32 D_800E753C;
extern s32 D_800E7540;

void func_80136B74(void) {
    s32 var_s0;
    void *temp_v0;

    switch (D_800E738A) {
    case 0:
        func_80085E30(1, 0);
        func_8006BD6C(0);
        func_8006492C(0);
        func_80064E48(0);
        func_800AE120(D_800E7374);
        func_80044750(0xCF);
        func_8004284C();
        return;
    case 1:
        cd_read_sectors(0x6CU, 0x801B0000, 0x7D0B);
        func_8004284C();
        return;
    case 2:
        if (cd_read_sync_status() & 1) {
            D_800E62BA = 0;
            func_80132040();
            var_s0 = 0;
            do {
                func_80043914(D_801391F4, (var_s0 + 0x10) & 0xFF, 1, 1, var_s0);
                var_s0 += 1;
            } while (var_s0 != 2);
            func_8004E44C(0, (u32 *) D_801391FC, D_801391E0);
            func_8004E350(0, D_801391F8, 0);
            func_8004D088(0, 5, 0x580, 0xF0);
            func_80086350();
            func_8004284C();
            return;
        }
        return;
    case 3:
        func_80044750(0x203);
        func_8004284C();
        return;
    case 4:
        func_80086370();
        if (D_800E7208 & 0x60) {
            func_80044750(0x74);
            func_800862D0();
            func_8004284C();
        }
        func_80085F0C(0, 0xF, 0, 0xF0, 0x280);
        return;
    case 5:
        if (func_800862F0() != 0) {
            D_800E74BC = -1;
            D_800E753C = -1;
            D_800E74C0 = -1;
            D_800E7540 = -1;
            temp_v0 = (2 * 4) + &D_800E6280;
            *(s32 *)((unsigned char *)temp_v0 + (4672)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4800)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4676)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4804)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4680)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4808)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4668)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4796)) = -1;
            func_80042808();
        }
        func_80085F0C(0, 0xF, 0, 0xF0, 0x280);
        return;
    default:
        func_80046500();
        break;
    }
}
