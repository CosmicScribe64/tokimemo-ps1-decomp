void *func_8004E970(s32);                           /* extern */
? func_8004E99C();                                  /* extern */

void func_80132F18(void) {
    void *sp50;
    s32 sp3C;
    s16 temp_v1;
    s32 temp_a0;
    s32 temp_v0;
    s32 var_a0;
    s32 var_s0;
    u32 temp_s2;
    void *temp_v0_2;
    void *temp_v0_3;

    func_8004E99C();
    temp_v0 = func_8004ECF0();
    temp_s2 = temp_v0 & 0xFF;
    temp_a0 = temp_s2 + 1;
    sp50 = func_8004E970(temp_v0);
    sp3C = temp_a0;
    func_8004E970(temp_a0);
    var_a0 = temp_a0;
    if (*(s16 *)((unsigned char *)sp50 + (2)) >= 0x68) {
        var_a0 = sp3C;
        if (D_800CA2BC != 0) {
            var_s0 = 1;
            if (temp_s2 >= 2U) {
                do {
                    temp_v0_2 = func_8004E970(var_s0);
                    temp_v1 = *(s16 *)((unsigned char *)temp_v0_2 + (2));
                    if (temp_v1 == 0x48) {
                        *(s16 *)((unsigned char *)temp_v0_2 + (2)) = (s16) (temp_v1 - 0x10);
                        *(s8 *)((unsigned char *)temp_v0_2 + (7)) = 0;
                    } else {
                        *(s16 *)((unsigned char *)temp_v0_2 + (2)) = (s16) (temp_v1 - 0x10);
                    }
                    var_s0 = (s32) (s16) (var_s0 + 1);
                } while ((u32) var_s0 < temp_s2);
            }
            D_800CA2BC = 0;
            *(s16 *)((unsigned char *)sp50 + (2)) = 0x58;
            *(s8 *)((unsigned char *)sp50 + (7)) = 1;
            var_a0 = sp3C;
        }
    }
    temp_v0_3 = func_8004E970(var_a0);
    if (*(s16 *)((unsigned char *)temp_v0_3 + (2)) >= 0x68) {
        D_800CA2BC = 1;
        *(s8 *)((unsigned char *)temp_v0_3 + (7)) = 0;
    }
}
