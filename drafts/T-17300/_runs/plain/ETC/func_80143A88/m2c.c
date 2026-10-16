extern u8 D_800E71DF;
extern s32 D_800E7200;
extern u32 D_800E7384;
extern s16 D_8015013E;
extern s16 D_80150140;
extern ? D_80150150;

void func_80143A88(void) {
    u32 sp30;
    s32 sp28;
    s32 temp_t9;
    s32 var_a0;
    s32 var_at;
    u32 temp_a0;
    u32 temp_hi;
    void *temp_v0;

    func_80143968();
    temp_a0 = D_800E7384;
    temp_hi = temp_a0 % 701U;
    sp30 = temp_a0 / 701U;
    if (temp_hi == 1) {
        if (D_80150148 == 0) {
            func_801436E4((s32) D_800E71DF);
        } else if (D_80150148 < 0xF) {
            temp_v0 = (D_80150148 * 4) + &D_80150150;
            var_a0 = *(s32 *)((unsigned char *)temp_v0 + (-4));
            if (D_800E71DF == var_a0) {
                var_a0 = *(s32 *)((unsigned char *)(temp_v0 + 4) + (-4));
                D_80150148 += 1;
            }
            func_801436E4(var_a0);
        } else {
            func_801436E4((s32) D_800E71DF);
        }
        D_80150148 += 1;
    } else if (((s32) sp30 < 0xF) && (sp30 != 0)) {
        if (temp_hi == 0) {
            D_80150130 = -0x60;
            D_80150134 = 0x20;
            D_80150140 = 0;
            D_8015013C.vx = 0;
            D_8015013C.vy = 0;
            D_80150138 = -0x72D8;
        } else if (temp_hi < 0x50U) {
            D_80150138 += 0x172;
        }
    }
    if (D_80150148 < 0xF) {
        var_at = temp_a0 < 0x259U;
        if ((u32) (temp_a0 % 701U) >= 0x26EU) {
            temp_t9 = sp30 * 0x23D;
            D_80150138 += 0x12C;
            sp28 = temp_t9;
            D_80150134 += func_800A0070(temp_t9) / 256;
            D_80150130 += func_800A0140(sp28) / 256;
            goto block_18;
        }
    } else {
block_18:
        var_at = temp_a0 < 0x259U;
    }
    if ((var_at == 0) && (func_800460EC() & 2)) {
        func_8004284C();
    }
    func_8004B19C((s32) D_80150130, 0x14, -0x28);
    func_8004B19C((s32) D_80150134, 0x14, -0x1E);
    func_8004B19C((s32) D_80150138, 0x14, -0x14);
    func_8004B19C((s32) D_800E7384, 0, -0xA);
    func_8004B19C((s32) sp30, 0, 0);
    func_8004B19C(D_80150148, 0, 0xA);
    func_8004B19C(D_8015014C[D_80150148], 0, 0x14);
    if (D_800E7200 & 0x8000) {
        D_80150130 += 0x14;
    }
    if (D_800E7200 & 0x2000) {
        D_80150130 -= 0x14;
    }
    if (D_800E7200 & 0x4000) {
        D_80150134 += 0x14;
    }
    if (D_800E7200 & 0x1000) {
        D_80150134 -= 0x14;
    }
    if (D_800E7200 & 4) {
        D_80150138 -= 0x14;
    }
    if (D_800E7200 & 1) {
        D_80150138 += 0x14;
    }
    if (D_800E7200 & 0xFA) {
        if (D_800E7200 & 8) {
            D_80150140 += 0xA;
        }
        if (D_800E7200 & 2) {
            D_80150140 -= 0xA;
        }
        if (D_800E7200 & 0x80) {
            D_8015013C.vx += 0xA;
        }
        if (D_800E7200 & 0x20) {
            D_8015013C.vx -= 0xA;
        }
        if (D_800E7200 & 0x10) {
            D_8015013E += 0xA;
        }
        if (D_800E7200 & 0x40) {
            D_8015013E -= 0xA;
        }
    } else {
        D_80150140 -= 3;
        D_8015013C.vx -= 4;
        D_8015013C.vy = D_8015013E - 5;
    }
}
