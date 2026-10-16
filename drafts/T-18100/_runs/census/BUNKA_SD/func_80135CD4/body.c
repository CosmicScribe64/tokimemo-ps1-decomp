void func_80135CD4(void) {
    s16 temp_t0;
    s32 var_s1;
    s32 var_s1_2;
    u8 *var_s0;

    var_s1 = 0;
    do {
        load_palette(D_8013C73C, (var_s1 + 0x10) & 0xFF, 1, 1, var_s1);
        var_s1 += 1;
    } while (var_s1 < 0x10);
    var_s0 = ((DisplayActor44 *)D_8011ECD0);
    var_s1_2 = 0;
    do {
        func_80048F64(var_s1_2 + 0x60);
        *(s8 *)((unsigned char *)var_s0 + (6529)) = 4;
        *(s32 *)((unsigned char *)var_s0 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)var_s0 + (6531)) = 0x80;
        *(s32 *)((unsigned char *)var_s0 + (6540)) = (s32) D_8013C734;
        *(s32 *)((unsigned char *)var_s0 + (6544)) = (s32) D_8013C738;
        temp_t0 = var_s1_2 + 3;
        var_s1_2 += 1;
        var_s0 += 0x44;
        *(s16 *)((unsigned char *)var_s0 + (6482)) = temp_t0;
        *(s8 *)((unsigned char *)var_s0 + (6466)) = 1;
        *(s8 *)((unsigned char *)var_s0 + (6467)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (6498)) = -0xA0;
        *(s16 *)((unsigned char *)var_s0 + (6502)) = -0x78;
        *(s8 *)((unsigned char *)var_s0 + (6465)) = 0x80;
        *(s8 *)((unsigned char *)var_s0 + (6527)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6464)) = 0;
        *(void **)((unsigned char *)var_s0 + (6512)) = (void *) D_8013C730;
    } while (var_s1_2 != 6);
    D_80120666 = (s16) D_80122EAC;
    D_8012071F = 0;
    D_80120763 = 0;
    D_801207A6 = 0x40;
    D_801207BC = 0x8B;
    load_tpage_buf_lock(5, D_8013C730[0], 1);
    load_tpage_buf_lock(6, D_8013C730[1], 1);
    load_tpage_buf_lock(7, D_8013C730[2], 1);
    load_tpage_buf_lock(8, D_8013C730[3], 1);
    load_tpage_buf_lock(9, D_8013C730[4], 1);
    load_tpage_buf_lock(0xA, D_8013C730[5], 1);
    load_tpage_buf_lock(0x25, D_8013C730[6], 1);
    load_tpage_buf_lock(0x26, D_8013C730[7], 1);
    load_tpage_buf_lock(0x27, D_8013C730[8], 1);
    load_tpage_buf_lock(0x28, D_8013C730[9], 1);
    load_tpage_buf_lock(0x29, D_8013C730[10], 1);
}
