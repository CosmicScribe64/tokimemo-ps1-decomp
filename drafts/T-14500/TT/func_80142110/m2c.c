void func_80142110(void) {
    u8 *sp44;
    u8 *sp40;
    u8 *sp3C;
    u8 *sp38;
    s32 sp34;
    u16 temp_t1;
    u16 temp_t2;
    u16 temp_t7;
    u16 temp_t7_2;
    u16 temp_t7_3;
    u16 temp_t9;
    u16 temp_t9_2;
    u32 temp_t6;
    u8 *temp_a0;
    u8 *temp_a0_2;
    u8 *temp_a1;
    u8 temp_a2;
    u8 temp_v0;
    u8 temp_v0_2;

    temp_a0 = D_80158A60 + 0x10;
    temp_a1 = D_80158A60 + 0x20;
    sp38 = temp_a1;
    sp3C = temp_a0;
    sp40 = D_80158A60;
    sp44 = D_80158A64;
    func_80042A00();
    sp34 = func_80043010();
    *(u16 *)((unsigned char *)temp_a0 + (2)) = (u16) *(u16 *)((unsigned char *)D_80158A60 + (16));
    if (*(u8 *)((unsigned char *)sp40 + (84)) != 0) {
        *(u16 *)((unsigned char *)D_80158A60 + (16)) = (u16) *(u16 *)((unsigned char *)(&sp34) + (2));
    } else {
        *(u16 *)((unsigned char *)D_80158A60 + (16)) = (u16) sp34.unk0;
    }
    *(s16 *)((unsigned char *)temp_a0 + (4)) = (s16) (~*(u16 *)((unsigned char *)temp_a0 + (2)) & *(u16 *)((unsigned char *)D_80158A60 + (16)));
    *(u16 *)((unsigned char *)temp_a1 + (2)) = (u16) *(u16 *)((unsigned char *)D_80158A60 + (32));
    temp_t7 = *(u16 *)((unsigned char *)sp40 + (80)) & 0xFFF;
    *(u16 *)((unsigned char *)sp40 + (80)) = temp_t7;
    if (*(u8 *)((unsigned char *)sp44 + (34)) != 0) {
        temp_a2 = *(*(s32 *)((unsigned char *)sp40 + (76)) + (temp_t7 & 0xFFFF));
        *(u16 *)((unsigned char *)D_80158A60 + (32)) = (u16) (((temp_a2 & 0xF0) << 8) | (temp_a2 & 3));
        temp_t1 = *(u16 *)((unsigned char *)sp40 + (80)) + 1;
        *(u16 *)((unsigned char *)sp40 + (80)) = temp_t1;
        *(u16 *)((unsigned char *)sp40 + (80)) = (u16) (temp_t1 & 0xFFF);
    } else {
        temp_t7_2 = *(u16 *)((unsigned char *)D_80158A60 + (16)) & 0xFF00;
        *(u16 *)((unsigned char *)D_80158A60 + (32)) = temp_t7_2;
        temp_t6 = (*(u16 *)((unsigned char *)D_80158A60 + (16)) << 8) & 0xFFFF;
        temp_t2 = temp_t7_2 | ((temp_t6 >> (*(u8 *)((unsigned char *)sp40 + (68)) + 8)) & 1);
        *(u16 *)((unsigned char *)D_80158A60 + (32)) = temp_t2;
        temp_t9 = (temp_t2 & 0xFFFF) | ((temp_t6 >> (*(u8 *)((unsigned char *)sp40 + (69)) + 7)) & 2);
        *(u16 *)((unsigned char *)D_80158A60 + (32)) = temp_t9;
        if (*(u8 *)((unsigned char *)sp40 + (67)) != 0) {
            if (temp_t9 & 1) {
                temp_v0 = *(u8 *)((unsigned char *)sp40 + (70));
                if (temp_v0 >= 6U) {
                    *(u8 *)((unsigned char *)sp40 + (70)) = 0U;
                    *(u16 *)((unsigned char *)D_80158A60 + (32)) = (u16) (*(u16 *)((unsigned char *)D_80158A60 + (32)) ^ 1);
                } else {
                    *(u8 *)((unsigned char *)sp40 + (70)) = (u8) (temp_v0 + 1);
                }
            } else {
                *(u8 *)((unsigned char *)sp40 + (70)) = 6U;
            }
            if (*(u16 *)((unsigned char *)D_80158A60 + (32)) & 2) {
                temp_v0_2 = *(u8 *)((unsigned char *)sp40 + (71));
                if (temp_v0_2 >= 6U) {
                    *(u8 *)((unsigned char *)sp40 + (71)) = 0U;
                    *(u16 *)((unsigned char *)D_80158A60 + (32)) = (u16) (*(u16 *)((unsigned char *)D_80158A60 + (32)) ^ 2);
                } else {
                    *(u8 *)((unsigned char *)sp40 + (71)) = (u8) (temp_v0_2 + 1);
                }
            } else {
                *(u8 *)((unsigned char *)sp40 + (71)) = 6U;
            }
        }
    }
    *(s16 *)((unsigned char *)temp_a1 + (4)) = (s16) (~*(u16 *)((unsigned char *)temp_a1 + (2)) & *(u16 *)((unsigned char *)D_80158A60 + (32)));
    temp_t9_2 = *(u16 *)((unsigned char *)temp_a0 + (8));
    temp_a0_2 = temp_a0 + 8;
    *(u16 *)((unsigned char *)temp_a0_2 + (2)) = temp_t9_2;
    temp_t7_3 = *(u16 *)((unsigned char *)(&sp34) + (2));
    *(s16 *)((unsigned char *)temp_a0_2 + (4)) = (s16) (~temp_t9_2 & temp_t7_3);
    *(u16 *)((unsigned char *)temp_a0 + (8)) = temp_t7_3;
}
