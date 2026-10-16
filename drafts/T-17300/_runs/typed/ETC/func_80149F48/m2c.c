? func_800AE0B0(? *, ?);                            /* extern */
extern ? D_800E6280;
extern ? D_8011ECD0;
extern s32 D_80150E94;
extern s32 D_80150E98;
extern s32 D_80150E9C;
extern s32 D_80150EA0;
extern ? D_80150EA4;

void func_80149F48(void) {
    s32 sp28;
    s16 var_a1;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_hi;
    s32 var_v1;
    u32 temp_t0;
    u32 temp_t4;
    u32 temp_t6;
    u32 var_v0;
    u32 var_v0_2;
    u32 var_v0_3;

    temp_a0 = D_80150E94;
    temp_hi = temp_a0 % 3;
    var_a1 = 0;
    switch (temp_hi) {                              /* irregular */
    case 0:
        temp_t6 = (u16) *(u16 *)((unsigned char *)(&D_800E6280) + (1644)) >> 4;
        var_v0 = temp_t6;
        if (((s32) temp_t6 > 0) && ((s32) temp_t6 >= 0x15)) {
            var_v0 = 0x14;
        }
        var_v1 = D_800E6280.unk0 + var_v0;
        if (*(u8 *)((unsigned char *)(&D_800E6280) + (245)) & 0xF) {
            var_v1 = var_v1 / 2;
        }
        break;
    case 1:
        temp_t0 = (u16) *(u16 *)((unsigned char *)(&D_800E6280) + (1652)) >> 4;
        var_v0_2 = temp_t0;
        if (((s32) temp_t0 > 0) && ((s32) temp_t0 >= 0x15)) {
            var_v0_2 = 0x14;
        }
        var_v1 = D_800E6280.unk0 + var_v0_2;
        if (*(u8 *)((unsigned char *)(&D_800E6280) + (245)) & 0xF) {
            var_v1 = var_v1 / 2;
        }
        break;
    default:
        sp28 = 0;
        func_800AE0B0("DEFAUT:", 0);
        var_a1 = 0;
        /* fallthrough */
    case 2:
        temp_t4 = (u16) *(u16 *)((unsigned char *)(&D_800E6280) + (1660)) >> 4;
        var_v0_3 = temp_t4;
        if (((s32) temp_t4 > 0) && ((s32) temp_t4 >= 0x15)) {
            var_v0_3 = 0x14;
        }
        var_v1 = *(s16 *)((unsigned char *)(&D_800E6280) + (266)) + var_v0_3;
        if (*(u8 *)((unsigned char *)(&D_800E6280) + (245)) & 0xF) {
            var_v1 = var_v1 / 2;
        }
        break;
    }
    temp_a0_2 = temp_a0 + 1;
    if (D_80150E98 == 1) {
        var_v1 += 0x26;
    }
    if (D_80150E98 == 2) {
        var_v1 += 0x36;
    }
    D_80150E94 = temp_a0_2;
    if (var_v1 < *(&D_80150EA4 + (((s32) (D_80150EA0 + temp_a0_2) % 12) * 4))) {
        var_a1 = 1;
    }
    if (var_a1 == 0) {
        D_80150E9C += 1;
    }
    *(s8 *)((unsigned char *)(&D_8011ECD0) + (6530)) = 5;
    *(s16 *)((unsigned char *)(&D_8011ECD0) + (6536)) = 0;
    *(s16 *)((unsigned char *)(&D_8011ECD0) + (6550)) = var_a1;
    *(s16 *)((unsigned char *)(&D_8011ECD0) + (6552)) = 0;
    *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
}
