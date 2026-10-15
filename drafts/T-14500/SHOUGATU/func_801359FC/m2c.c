extern ? D_800CA248;
extern s16 D_800E639E;
extern u8 D_800E71DF;
extern s32 D_800E7378;
extern ? D_80143E80;

void func_801359FC(void) {
    ? sp28;
    s32 var_t5;
    u8 temp_t2;
    u8 temp_v0;
    u8 var_v0;

    M2C_MEMCPY_ALIGNED(&sp28, &D_80143E80, 0x20);
    *(? *)((unsigned char *)(&sp28 + 0x18) + (7)) = (unaligned s32) M2C_LWL(*(? *)((unsigned char *)(&D_80143E80 + 0x18) + (10)));
    temp_t2 = D_800E7378 & 7;
    var_v0 = temp_t2;
    if (temp_t2 != 0) {
        var_v0 = (temp_t2 - 1) & 0xFF;
    }
    if (D_800E71DF == 3) {
        var_t5 = var_v0 * 4;
        if (var_v0 == 3) {
            var_v0 = 4;
            goto block_5;
        }
    } else {
block_5:
        var_t5 = var_v0 * 4;
    }
    D_80143DD8 = var_v0;
    func_800AE0F0(&D_800CA248, &sp28 + (var_t5 + var_v0));
    temp_v0 = D_80143DD8;
    if (temp_v0 == 0) {
        D_800E639E -= 5;
    }
    D_80143DD8 = temp_v0;
    if (temp_v0 == 4) {
        D_800E639E += 5;
    }
    func_80084D3C();
}
