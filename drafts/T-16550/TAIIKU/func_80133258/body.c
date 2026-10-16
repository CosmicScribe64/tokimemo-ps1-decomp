void func_80133258(void) {
    TaiBytes10 sp88;
    TaiikuHalf10 sp74;
    TaiikuHalf10 sp60;
    TaiikuHalf10 sp4C;
    DisplayActor44 *var_v0;
    s16 *var_a0;
    s16 *var_a1;
    s16 *var_a2;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_t9;
    s32 temp_s1;
    s32 var_s0;
    s32 var_s0_2;
    u8 *var_v1;
    u8 temp_t8;


    sp88 = D_80148FE8;
    sp74 = D_80148FF4;
    sp60 = D_80149008;
    sp4C = D_8014901C;
    var_s0 = 0;
    do {
        temp_s1 = var_s0 + 1;
        func_80048F64(temp_s1);
        var_s0 = temp_s1;
    } while (temp_s1 < 0x17);
    var_v0 = ((DisplayActor44 *)D_8011ECD0);
    var_s0_2 = 0;
    var_v1 = sp88.v;
    var_a0 = sp74.h;
    var_a1 = sp60.h;
    var_a2 = sp4C.h;
    do {
        *(s32 *)((unsigned char *)var_v0 + (124)) = (s32) (((var_s0_2 & 1) << 0x1E) | 0x01000000);
        temp_t8 = *var_v1;
        temp_t6 = *var_a0;
        temp_t7 = *var_a1;
        temp_t9 = *var_a2;
        var_s0_2 += 1;
        var_v0 += 1;
        var_v1 += 1;
        var_a0 += 2;
        var_a1 += 2;
        var_a2 += 2;
        var_v0->unk_05 = 0x80;
        var_v0->unk_06 = 2;
        var_v0->unk_0C = (u8 *) D_80149FCC;
        var_v0->unk_10 = (u8 *) D_8014A004;
        var_v0->unk_34 = (u8 *) D_8014A070;
        var_v0->unk_14 = D_8014A03C;
        var_v0->unk_1A = 0x1000;
        *(s16 *)((unsigned char *)var_v0 + (28)) = 0x1000;
        var_v0->unk_07 = 0x80;
        var_v0->unk_02 = 1;
        var_v0->unk_03 = 0xA4;
        var_v0->unk_01 = temp_t8;
        var_v0->unk_26 = temp_t6;
        var_v0->unk_2A = temp_t7;
        var_v0->unk_16 = temp_t9;
    } while (var_s0_2 != 0xA);
}
