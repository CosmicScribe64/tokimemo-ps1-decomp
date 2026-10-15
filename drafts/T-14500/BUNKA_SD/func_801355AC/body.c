void func_801355AC(void) {
    u8 *var_v0;
    u8 temp_t6;

    func_80066334();
    D_80120657 = D_800E738B;
    D_801206DF = D_800E738B;
    temp_t6 = (D_800E738B - 2) & 0xFF;
    D_800E738B = temp_t6;
    if (temp_t6 == 0) {
        var_v0 = D_80120650;
        do {
            var_v0 += 0x110;
            *(s8 *)((unsigned char *)var_v0 + (-197)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-129)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-61)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-265)) = 0;
        } while (var_v0 != D_80121750);
        D_800E738B = 0;
        func_8004284C();
    }
}
