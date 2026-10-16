extern u8 D_800E738B;

void func_8013987C(void) {
    u8 *var_v0;
    u8 *var_v0_2;
    u8 temp_t6;
    u8 var_v1;

    var_v1 = D_800E738B;
    var_v0 = D_80120650;
    do {
        var_v0 += 0x44;
        *(u8 *)((unsigned char *)var_v0 + (-61)) = var_v1;
    } while ((u32) var_v0 < (u32) D_80121750);
    if (var_v1 == 0) {
        var_v1 = 2;
    }
    temp_t6 = (var_v1 - 1) & 0xFF;
    D_800E738B = temp_t6;
    if (temp_t6 == 0) {
        var_v0_2 = D_80120650;
        do {
            var_v0_2 += 0x110;
            *(s8 *)((unsigned char *)var_v0_2 + (-197)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-129)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-61)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-265)) = 0;
        } while (var_v0_2 != D_80121750);
        D_800E738B = 0;
        func_8004284C();
    }
}
