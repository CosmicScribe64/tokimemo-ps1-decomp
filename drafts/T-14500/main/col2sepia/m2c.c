void col2sepia(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp_a1;
    s32 temp_t4;
    s32 temp_v1;
    s32 var_a2;
    s32 var_v0;
    u32 temp_a0;
    u32 temp_v1_2;
    u32 temp_v1_3;
    u32 temp_v1_4;
    u32 temp_v1_5;
    u32 temp_v1_6;

    func_8004B590(arg0, arg1);
    var_v0 = arg1;
    var_a2 = 0;
    do {
        temp_v1 = var_v0->unk0;
        var_a2 += 1;
        temp_a1 = temp_v1 & 0x1F;
        if ((temp_a1 + ((u32) (temp_v1 << 0x16) >> 0x1B) + ((u32) (temp_v1 << 0x11) >> 0x1B)) != 0) {
            temp_v1_2 = temp_a1 + arg2;
            if (temp_v1_2 >= 0x20U) {
                var_v0->unk0 = (s8) ((u8) var_v0->unk0 | 0x1F);
            } else {
                var_v0->unk0 = (s8) ((temp_v1_2 & 0x1F) | ((u8) var_v0->unk0 & 0xFFE0));
            }
            temp_v1_3 = ((u32) (var_v0->unk0 << 0x16) >> 0x1B) + arg3;
            if (temp_v1_3 >= 0x20U) {
                var_v0->unk0 = (s16) ((u16) var_v0->unk0 | 0x3E0);
            } else {
                var_v0->unk0 = (s16) (((temp_v1_3 << 5) & 0x3E0) | ((u16) var_v0->unk0 & 0xFC1F));
            }
            temp_a0 = ((u32) (var_v0->unk0 << 0x11) >> 0x1B) + arg4;
            if (temp_a0 >= 0x20U) {
                *(u8 *)((unsigned char *)var_v0 + (1)) = (u8) (*(u8 *)((unsigned char *)var_v0 + (1)) | 0x7C);
            } else {
                *(u8 *)((unsigned char *)var_v0 + (1)) = (u8) (((temp_a0 * 4) & 0x7C) | (*(u8 *)((unsigned char *)var_v0 + (1)) & 0xFF83));
            }
        }
        temp_t4 = *(u16 *)((unsigned char *)var_v0 + (2)) & 0x1F;
        temp_v1_4 = temp_t4 + arg2;
        if ((temp_t4 + ((u32) (var_v0->unk0 << 6) >> 0x1B) + ((u32) (var_v0->unk0 * 2) >> 0x1B)) != 0) {
            if (temp_v1_4 >= 0x20U) {
                *(u16 *)((unsigned char *)var_v0 + (2)) = (s8) ((u8) *(u16 *)((unsigned char *)var_v0 + (2)) | 0x1F);
            } else {
                *(u16 *)((unsigned char *)var_v0 + (2)) = (s8) ((temp_v1_4 & 0x1F) | ((u8) *(u16 *)((unsigned char *)var_v0 + (2)) & 0xFFE0));
            }
            temp_v1_5 = ((u32) (var_v0->unk0 << 6) >> 0x1B) + arg3;
            if (temp_v1_5 >= 0x20U) {
                *(u16 *)((unsigned char *)var_v0 + (2)) = (u16) (*(u16 *)((unsigned char *)var_v0 + (2)) | 0x3E0);
            } else {
                *(u16 *)((unsigned char *)var_v0 + (2)) = (u16) (((temp_v1_5 << 5) & 0x3E0) | (*(u16 *)((unsigned char *)var_v0 + (2)) & 0xFC1F));
            }
            temp_v1_6 = ((u32) (var_v0->unk0 * 2) >> 0x1B) + arg4;
            if (temp_v1_6 >= 0x20U) {
                *(u8 *)((unsigned char *)var_v0 + (3)) = (u8) (*(u8 *)((unsigned char *)var_v0 + (3)) | 0x7C);
            } else {
                *(u8 *)((unsigned char *)var_v0 + (3)) = (u8) (((temp_v1_6 * 4) & 0x7C) | (*(u8 *)((unsigned char *)var_v0 + (3)) & 0xFF83));
            }
        }
        var_v0 += 4;
    } while (var_a2 != 0x80);
}
