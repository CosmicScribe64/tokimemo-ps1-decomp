void func_80059BE8(void) {
    s32 *var_v0_5;
    s32 *var_v0_2;
    s32 *var_v0_4;
    s32 temp_t0;
    s32 temp_t2;
    s32 temp_t4;
    s32 temp_t6;
    s32 temp_t8;
    u8 *var_v0;
    u8 *var_v0_3;
    u8 *temp_v0;

    if (!(*D_800C9730 & 0x80000)) {
        var_v0 = D_800C9730;
        do {
            temp_t8 = *var_v0;
            var_v0 += 4;
            *(s32 *)((unsigned char *)var_v0 + (-4)) = (s32) (temp_t8 + 0x80000);
        } while ((u32) var_v0 < (u32) sprite_frame_table);
        var_v0_2 = &D_800C5FCC;
        do {
            temp_t0 = *var_v0_2;
            var_v0_2 += 4;
            *(s32 *)((unsigned char *)var_v0_2 + (-4)) = (s32) (temp_t0 + 0x80000);
        } while (var_v0_2 != D_800C5FD4);
    }
    load_tpage_buf_lock(0x1F, D_800C974C, 2);
    load_tpage_buf_lock(0x1E, D_800C9750, 2);
    load_tpage_buf_lock(0x1D, D_800C9754, 2);
    load_tpage_buf_lock(0x1C, D_800C9758, 2);
    load_tpage_buf_lock(0x1B, D_800C5FCC, 2);
    load_tpage_buf_lock(0x3B, D_800C5FD0, 2);
    load_tpage_buf_lock(0x3C, *D_800C9730, 2);
    load_tpage_buf_lock(0x3D, D_800C9734, 2);
    load_tpage_buf_lock(0x3E, D_800C9738, 2);
    load_tpage_buf_lock(0x3F, D_800C973C, 2);
    var_v0_3 = D_800C9730;
    do {
        temp_t2 = *var_v0_3;
        var_v0_3 += 4;
        *(s32 *)((unsigned char *)var_v0_3 + (-4)) = (s32) (temp_t2 - 0x80000);
    } while ((u32) var_v0_3 < (u32) sprite_frame_table);
    var_v0_4 = &D_800C5FCC;
    do {
        temp_t4 = *var_v0_4;
        var_v0_4 += 4;
        *(s32 *)((unsigned char *)var_v0_4 + (-4)) = (s32) (temp_t4 - 0x80000);
    } while ((u32) var_v0_4 < (u32) D_800C5FD4);
    var_v0_5 = &D_800E62EC;
    do {
        temp_t6 = *(s32 *)((unsigned char *)var_v0_5 + (4648));
        var_v0_5 += 4;
        *(s32 *)((unsigned char *)var_v0_5 + (4644)) = (s32) (temp_t6 + 0xFFF80000);
    } while ((u32) var_v0_5 < (u32) &D_800E6300);
    D_800E7594 += 0xFFF80000;
    temp_v0 = (0x3C * 4) + &D_800E6280;
    *(s32 *)((unsigned char *)temp_v0 + (4660)) = (s32) (*(s32 *)((unsigned char *)temp_v0 + (4660)) + 0xFFF80000);
    *(s32 *)((unsigned char *)temp_v0 + (4656)) = (s32) (*(s32 *)((unsigned char *)temp_v0 + (4656)) + 0xFFF80000);
    *(s32 *)((unsigned char *)temp_v0 + (4652)) = (s32) (*(s32 *)((unsigned char *)temp_v0 + (4652)) + 0xFFF80000);
    D_800E6280.unk_1228[0x3C] += 0xFFF80000;
}
