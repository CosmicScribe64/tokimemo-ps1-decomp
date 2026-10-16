void func_8013230C(u8 arg0, s32 arg1) {
    u8 *sp34;
    s16 temp_t7;
    s16 temp_t7_2;
    s16 temp_t8;
    s16 var_a0;
    s16 var_a0_2;
    s32 temp_hi;
    s32 temp_t8_2;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    s32 var_v1_5;
    s32 var_v1_6;
    u8 *temp_v0;
    u8 *temp_v0_2;
    u8 *var_t1;
    u8 *var_v0;
    u8 temp_a3;

    temp_a3 = arg0 & 0xFF;
    sp34 = D_80158A64;
    var_t1 = D_80158AB0;
    var_v1 = 0;
    var_v0 = D_80158AA4 + 0x1900;
    do {
        (*(s32 *)var_t1) = 0x1000;
        *(s16 *)((unsigned char *)var_t1 + (2)) = 0x1000;
        var_v0 += 0x64;
        *(s16 *)((unsigned char *)var_t1 + (4)) = (s16) (((var_v1 / 5) * 0x48) - 0x5C);
        temp_hi = var_v1 % 5;
        var_v1 += 1;
        *(s16 *)((unsigned char *)var_t1 + (6)) = (s16) ((temp_hi * 0x48) - 0x48);
        *(s8 *)((unsigned char *)var_v0 + (-100)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (-99)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-32)) = 8;
        *(s8 *)((unsigned char *)var_v0 + (-41)) = 0x80;
        *(u8 **)((unsigned char *)var_v0 + (-28)) = var_t1;
        *(s8 *)((unsigned char *)var_v0 + (-23)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-22)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (-42)) = 0x40;
        *(s8 *)((unsigned char *)var_v0 + (-43)) = 0x40;
        *(s8 *)((unsigned char *)var_v0 + (-44)) = 0x40;
        *(s8 *)((unsigned char *)var_v0 + (-20)) = 0;
        temp_t8 = *(s16 *)((unsigned char *)var_t1 + (4));
        var_t1 += 0x10;
        *(s16 *)((unsigned char *)var_v0 + (-80)) = (s16) (temp_t8 - 0x24);
        *(s16 *)((unsigned char *)var_v0 + (-78)) = (s16) (*(s16 *)((unsigned char *)var_t1 + (-10)) - 0x24);
        *(s8 *)((unsigned char *)var_v0 + (-30)) = 0x38;
        *(s8 *)((unsigned char *)var_v0 + (-29)) = 0xB8;
        *(s8 *)((unsigned char *)var_v0 + (-21)) = (s8) (arg1 & 0xFF);
        *(s16 *)((unsigned char *)var_v0 + (-40)) = 0x48;
        *(s16 *)((unsigned char *)var_v0 + (-38)) = 0x48;
        *(s16 *)((unsigned char *)var_v0 + (-36)) = 0xA;
        *(s16 *)((unsigned char *)var_v0 + (-34)) = (s16) ((temp_a3 & 0xF) + (((temp_a3 >> 4) + 0xF0) << 6));
        *(s8 *)((unsigned char *)var_v0 + (-24)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-84)) = 0xD;
    } while (var_v1 < 0x19);
    var_v1_2 = 0;
    do {
        (*(s32 *)var_v0) = 1;
        *(s8 *)((unsigned char *)var_v0 + (1)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (68)) = 8;
        if (var_v1_2 & 1) {
            *(s16 *)((unsigned char *)var_v0 + (20)) = 0x58;
        } else {
            *(s16 *)((unsigned char *)var_v0 + (20)) = -0x60;
        }
        temp_t8_2 = var_v1_2 * 8;
        if ((var_v1_2 >> 1) != 0) {
            *(s16 *)((unsigned char *)var_v0 + (22)) = 0xB8;
        } else {
            *(s16 *)((unsigned char *)var_v0 + (22)) = 0x30;
        }
        var_v1_2 += 1;
        var_v0 += 0x64;
        *(s16 *)((unsigned char *)var_v0 + (-40)) = 8;
        *(s16 *)((unsigned char *)var_v0 + (-38)) = 8;
        *(s8 *)((unsigned char *)var_v0 + (-30)) = (s8) (temp_t8_2 + 0x40);
        *(s8 *)((unsigned char *)var_v0 + (-29)) = 0x18;
        *(s16 *)((unsigned char *)var_v0 + (-36)) = 0xA;
        *(s16 *)((unsigned char *)var_v0 + (-34)) = 0x3C0D;
        *(s8 *)((unsigned char *)var_v0 + (-41)) = 0x65;
        *(s16 *)((unsigned char *)var_v0 + (-84)) = 0;
    } while (var_v1_2 < 4);
    arg0 = temp_a3;
    var_a1 = 0;
    var_a0 = 0x30;
    do {
        (*(s32 *)var_v0) = 1;
        *(s8 *)((unsigned char *)var_v0 + (1)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (68)) = 8;
        *(s16 *)((unsigned char *)var_v0 + (20)) = -0x58;
        if (var_a0 == 0x30) {
            var_v1_3 = 1;
        } else {
            var_v1_3 = 2;
        }
        *(s16 *)((unsigned char *)var_v0 + (22)) = (s16) (var_v1_3 + var_a1 + 0x30);
        *(s16 *)((unsigned char *)var_v0 + (60)) = 0xB0;
        *(s16 *)((unsigned char *)var_v0 + (62)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (56)) = 0xF8;
        *(s8 *)((unsigned char *)var_v0 + (57)) = 0xD8;
        *(s8 *)((unsigned char *)var_v0 + (58)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (16)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (59)) = 0x61;
        temp_v0 = var_v0 + 0x64;
        *(s8 *)((unsigned char *)var_v0 + (100)) = 1;
        *(s8 *)((unsigned char *)temp_v0 + (1)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (68)) = 8;
        *(s16 *)((unsigned char *)temp_v0 + (20)) = -0x58;
        if (var_a0 == 0x30) {
            var_v1_4 = 2;
        } else {
            var_v1_4 = 1;
        }
        *(s16 *)((unsigned char *)temp_v0 + (122)) = var_a0;
        var_a0 += 0x8C;
        temp_t7 = var_v1_4 + var_a1 + 0x30;
        var_a1 += 0x8C;
        var_v0 = temp_v0 + 0x12C;
        *(s16 *)((unsigned char *)var_v0 + (-278)) = temp_t7;
        *(s16 *)((unsigned char *)var_v0 + (-240)) = 0xB0;
        *(s16 *)((unsigned char *)var_v0 + (-238)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (-244)) = 0xF8;
        *(s8 *)((unsigned char *)var_v0 + (-243)) = 0xA8;
        *(s8 *)((unsigned char *)var_v0 + (-242)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-284)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-241)) = 0x61;
        *(s8 *)((unsigned char *)var_v0 + (-200)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (-199)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-132)) = 8;
        *(s16 *)((unsigned char *)var_v0 + (-180)) = -0x58;
        *(s16 *)((unsigned char *)var_v0 + (-140)) = 0xB0;
        *(s16 *)((unsigned char *)var_v0 + (-138)) = 4;
        *(s8 *)((unsigned char *)var_v0 + (-144)) = 0xC0;
        *(s8 *)((unsigned char *)var_v0 + (-143)) = 0x70;
        *(s8 *)((unsigned char *)var_v0 + (-142)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-184)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-141)) = 0x61;
    } while (var_a0 < 0x148);
    var_a1_2 = 0;
    var_a0_2 = -0x60;
    do {
        (*(s32 *)var_v0) = 1;
        *(s8 *)((unsigned char *)var_v0 + (1)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (68)) = 8;
        if (var_a0_2 == -0x60) {
            var_v1_5 = 1;
        } else {
            var_v1_5 = 2;
        }
        *(s16 *)((unsigned char *)var_v0 + (20)) = (s16) ((var_v1_5 + var_a1_2) - 0x60);
        *(s16 *)((unsigned char *)var_v0 + (22)) = 0x38;
        *(s16 *)((unsigned char *)var_v0 + (60)) = 1;
        *(s16 *)((unsigned char *)var_v0 + (62)) = 0x80;
        *(s8 *)((unsigned char *)var_v0 + (56)) = 0xF8;
        *(s8 *)((unsigned char *)var_v0 + (57)) = 0xD8;
        *(s8 *)((unsigned char *)var_v0 + (58)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (16)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (59)) = 0x61;
        temp_v0_2 = var_v0 + 0x64;
        *(s8 *)((unsigned char *)var_v0 + (100)) = 1;
        *(s8 *)((unsigned char *)temp_v0_2 + (1)) = 0;
        *(s16 *)((unsigned char *)temp_v0_2 + (68)) = 8;
        if (var_a0_2 == -0x60) {
            var_v1_6 = 2;
        } else {
            var_v1_6 = 1;
        }
        *(s16 *)((unsigned char *)temp_v0_2 + (120)) = var_a0_2;
        var_a0_2 += 0xBC;
        temp_t7_2 = (var_v1_6 + var_a1_2) - 0x60;
        var_a1_2 += 0xBC;
        var_v0 = temp_v0_2 + 0x12C;
        *(s16 *)((unsigned char *)var_v0 + (-280)) = temp_t7_2;
        *(s16 *)((unsigned char *)var_v0 + (-278)) = 0x38;
        *(s16 *)((unsigned char *)var_v0 + (-240)) = 1;
        *(s16 *)((unsigned char *)var_v0 + (-238)) = 0x80;
        *(s8 *)((unsigned char *)var_v0 + (-244)) = 0xF8;
        *(s8 *)((unsigned char *)var_v0 + (-243)) = 0xA8;
        *(s8 *)((unsigned char *)var_v0 + (-242)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-284)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-241)) = 0x61;
        *(s8 *)((unsigned char *)var_v0 + (-200)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (-199)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-132)) = 8;
        *(s16 *)((unsigned char *)var_v0 + (-178)) = 0x38;
        *(s16 *)((unsigned char *)var_v0 + (-140)) = 4;
        *(s16 *)((unsigned char *)var_v0 + (-138)) = 0x80;
        *(s8 *)((unsigned char *)var_v0 + (-144)) = 0xC0;
        *(s8 *)((unsigned char *)var_v0 + (-143)) = 0x70;
        *(s8 *)((unsigned char *)var_v0 + (-142)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-184)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-141)) = 0x61;
    } while (var_a0_2 != 0x118);
    *(s16 *)((unsigned char *)var_v0 + (20)) = -0x5E;
    *(s8 *)((unsigned char *)temp_v0_2 + (300)) = 1;
    *(s8 *)((unsigned char *)var_v0 + (1)) = 0;
    *(s16 *)((unsigned char *)var_v0 + (22)) = 0x32;
    *(s16 *)((unsigned char *)var_v0 + (60)) = 0xBC;
    *(s16 *)((unsigned char *)var_v0 + (62)) = 0x8C;
    *(s8 *)((unsigned char *)var_v0 + (58)) = 0;
    *(s8 *)((unsigned char *)var_v0 + (57)) = 0;
    *(s8 *)((unsigned char *)var_v0 + (56)) = 0;
    *(s16 *)((unsigned char *)var_v0 + (16)) = 0xC;
    *(s8 *)((unsigned char *)var_v0 + (59)) = 0x63;
    switch (arg0) {
    case 12:
        *(s8 *)((unsigned char *)sp34 + (19)) = 0x7B;
        *(s8 *)((unsigned char *)sp34 + (20)) = 0xD5;
        *(s8 *)((unsigned char *)sp34 + (21)) = 0x94;
        return;
    case 13:
        *(s8 *)((unsigned char *)sp34 + (19)) = 0;
        *(s8 *)((unsigned char *)sp34 + (20)) = 0xCD;
        *(s8 *)((unsigned char *)sp34 + (21)) = 0xEE;
        return;
    case 14:
        *(s8 *)((unsigned char *)sp34 + (19)) = 0xFF;
        *(s8 *)((unsigned char *)sp34 + (20)) = 0x9C;
        *(s8 *)((unsigned char *)sp34 + (21)) = 0xCD;
        return;
    }
}
