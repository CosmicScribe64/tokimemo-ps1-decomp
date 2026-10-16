void func_80134418(void) {
    u8 *var_v0;
    u8 *var_v0_2;
    u8 temp_t6;

    var_v0 = D_80120650;
    do {
        var_v0 += 0x44;
        *(u8 *)((unsigned char *)var_v0 + (-61)) = (u8) D_800E738B;
    } while ((u32) var_v0 < (u32) D_80121750);
    temp_t6 = (D_800E738B + 2) & 0xFF;
    D_800E738B = temp_t6;
    if (temp_t6 >= 0x81U) {
        var_v0_2 = D_80120650;
        do {
            var_v0_2 += 0x110;
            *(s8 *)((unsigned char *)var_v0_2 + (-197)) = 0x80;
            *(s8 *)((unsigned char *)var_v0_2 + (-129)) = 0x80;
            *(s8 *)((unsigned char *)var_v0_2 + (-61)) = 0x80;
            *(s8 *)((unsigned char *)var_v0_2 + (-265)) = 0x80;
        } while (var_v0_2 != D_80121750);
        D_800E738B = 0x80;
        func_8004284C();
    }
}
