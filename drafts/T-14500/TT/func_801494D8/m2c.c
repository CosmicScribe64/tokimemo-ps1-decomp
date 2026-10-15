void func_801494D8(void) {
    u8 *sp54;
    u8 *sp44;
    u8 *sp40;
    u8 *sp3C;
    u8 *sp38;
    u32 sp34;
    u16 temp_v1;
    u16 temp_v1_2;
    u32 var_a2;
    u8 *temp_a1;
    u8 *temp_a3;
    u8 *temp_s0;
    u8 *temp_t0;
    u8 *temp_v0;
    u8 temp_t4;
    u8 temp_t8;

    sp54 = D_80158A60;
    temp_s0 = D_80158A64;
    temp_a1 = D_80158A6C;
    temp_v0 = D_80158AA4;
    temp_t0 = D_80158AA8;
    var_a2 = 3;
    temp_a3 = D_80158A60 + 0x10;
    if (*(u8 *)((unsigned char *)D_80158A60 + (83)) == 0) {
        var_a2 = 2;
    }
    *(s8 *)((unsigned char *)(temp_t0 + (*(u8 *)((unsigned char *)temp_a1 + (10)) * 0x14)) + (2)) = 0;
    temp_v1 = *(u16 *)((unsigned char *)temp_a3 + (4));
    if (temp_v1 & 0x1000) {
        sp3C = temp_v0;
        sp44 = temp_a1;
        sp34 = var_a2;
        sp40 = temp_a3;
        sp38 = temp_t0;
        func_80133AD0(0x613);
        *(s16 *)((unsigned char *)temp_s0 + (38)) = 0;
        temp_t4 = *(u8 *)((unsigned char *)temp_a1 + (10)) - 1;
        *(u8 *)((unsigned char *)temp_a1 + (10)) = temp_t4;
        if ((temp_t4 & 0xFF) == 0xFF) {
            *(u8 *)((unsigned char *)temp_a1 + (10)) = (u8) var_a2;
        }
    } else if (temp_v1 & 0x4000) {
        sp3C = temp_v0;
        sp44 = temp_a1;
        sp34 = var_a2;
        sp40 = temp_a3;
        sp38 = temp_t0;
        func_80133AD0(0x613);
        *(s16 *)((unsigned char *)temp_s0 + (38)) = 0;
        temp_t8 = *(u8 *)((unsigned char *)temp_a1 + (10)) + 1;
        *(u8 *)((unsigned char *)temp_a1 + (10)) = temp_t8;
        if (var_a2 < (u32) (temp_t8 & 0xFF)) {
            *(u8 *)((unsigned char *)temp_a1 + (10)) = 0U;
        }
    }
    *(s8 *)((unsigned char *)(temp_t0 + (*(u8 *)((unsigned char *)temp_a1 + (10)) * 0x14)) + (2)) = 2;
    temp_v0->unk0 = 1;
    *(s8 *)((unsigned char *)temp_v0 + (1)) = 0;
    *(s16 *)((unsigned char *)temp_v0 + (68)) = 0;
    *(s16 *)((unsigned char *)temp_v0 + (20)) = -0x2C;
    *(s16 *)((unsigned char *)temp_v0 + (22)) = (s16) ((*(u8 *)((unsigned char *)temp_a1 + (10)) * 8) + 0xA0);
    *(s16 *)((unsigned char *)temp_v0 + (16)) = 1;
    *(s8 *)((unsigned char *)temp_v0 + (59)) = 0x63;
    *(s16 *)((unsigned char *)temp_v0 + (60)) = 0x58;
    *(s16 *)((unsigned char *)temp_v0 + (62)) = 8;
    *(s8 *)((unsigned char *)temp_v0 + (56)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (57)) = 0x40;
    *(s8 *)((unsigned char *)temp_v0 + (58)) = 0x40;
    temp_v1_2 = *(u16 *)((unsigned char *)temp_a3 + (4));
    if ((temp_v1_2 & 0x800) || (temp_v1_2 & 0x20)) {
        sp44 = temp_a1;
        func_80044750(0xB1);
        func_80133AD0(0x612);
        *(s8 *)((unsigned char *)temp_s0 + (34)) = 0;
        *(u16 *)((unsigned char *)temp_s0 + (2)) = (u16) (*(u16 *)((unsigned char *)temp_s0 + (2)) + 1);
        *(s16 *)((unsigned char *)temp_s0 + (4)) = 0;
        *(s16 *)((unsigned char *)temp_a1 + (16)) = 0;
        *(s16 *)((unsigned char *)temp_a1 + (18)) = 0;
        func_80149B20();
    }
    if ((*(u8 *)((unsigned char *)sp54 + (83)) == 1) && (*(u8 *)((unsigned char *)temp_s0 + (36)) == 0)) {
        func_80149400();
    }
}
