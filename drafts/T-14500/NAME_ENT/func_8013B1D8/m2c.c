extern s8 D_800E7313;
extern u8 D_800E7397;
extern u8 D_800E7398;
extern ? D_800E7D1D;
extern ? D_800E8568;
extern ? D_800E8578;
extern ? D_800E8588;
extern ? D_80120A08;
extern ? D_80123325;
extern ? D_80123326;
extern ? D_80123327;

void func_8013B1D8(void) {
    s32 sp28;
    s32 *var_v0;
    s32 var_a3;
    s32 var_a3_2;
    s32 var_v1;
    u8 *var_v0_2;
    u8 *var_v0_4;
    u8 temp_t7;
    void *var_v0_3;

    if (D_800E8BEE != 0) {
        var_a3 = 0;
        var_v0 = &D_800E7D10;
        do {
            temp_t7 = *(u8 *)((unsigned char *)var_v0 + (20));
            var_v0 += 1;
            if (temp_t7 != 0) {
                var_a3 += 1;
            }
        } while ((u32) var_v0 < (u32) &D_800E7D1D);
        var_v0_2 = D_80120650;
        do {
            if (var_v0_2 == (((D_800E7D36 - 1) * 0x44) + D_80120650)) {
                *(s8 *)((unsigned char *)var_v0_2 + (339)) = 0x11;
            } else {
                *(s8 *)((unsigned char *)var_v0_2 + (339)) = 0;
            }
            var_v0_2 += 0x44;
        } while (var_v0_2 != &D_80120A08);
        if (D_800E7313 != -1) {
            D_80120C6F |= 0x80;
            D_80120C92 = (((s8) D_800E7313 / 7) * 0x1F) - 0x82;
            D_80120C96 = (((s8) D_800E7313 % 7) * 0x10) - 0xB;
        }
        sp28 = var_a3;
        func_80139740((s32) *(&D_800E8568 + D_800E7D36), 0x30, -0x60);
        func_80139740((s32) *(&D_800E8578 + D_800E7D36), 0x50, -0x60);
        func_80139740((s32) *(&D_800E8588 + D_800E7D36), 0x70, -0x60);
        func_80139740(sp28, 0x5A, -0x1D);
        return;
    }
    var_a3_2 = 0;
    var_v0_3 = (D_800E7397 * 0xF00) + D_80123120;
    var_v1 = 0;
    do {
        var_v1 += 1;
        if (*(u8 *)((unsigned char *)var_v0_3 + (1672)) != 0) {
            var_a3_2 += 1;
        }
        var_v0_3 += 1;
    } while (var_v1 < 0xD);
    var_v0_4 = D_80120650;
    do {
        if (var_v0_4 == (((D_800E7398 - 1) * 0x44) + D_80120650)) {
            *(s8 *)((unsigned char *)var_v0_4 + (339)) = 0x11;
        } else {
            *(s8 *)((unsigned char *)var_v0_4 + (339)) = 0;
        }
        var_v0_4 += 0x44;
    } while (var_v0_4 != &D_80120A08);
    if (D_800E7313 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = (((s8) D_800E7313 / 7) * 0x1F) - 0x82;
        D_80120C96 = (((s8) D_800E7313 % 7) * 0x10) - 0xB;
    }
    sp28 = var_a3_2;
    func_80139740((s32) *(&D_80123325 + (D_800E7397 * 0xF00)), 0x30, -0x60);
    func_80139740((s32) *(&D_80123326 + (D_800E7397 * 0xF00)), 0x50, -0x60);
    func_80139740((s32) *(&D_80123327 + (D_800E7397 * 0xF00)), 0x70, -0x60);
    func_80139740(sp28, 0x5A, -0x1D);
}
