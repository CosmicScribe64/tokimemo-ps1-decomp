s32 func_801342CC(void) {
    s32 temp_t1;
    s32 temp_t3;
    s32 temp_t9;
    s32 var_a0;

    tpage_buf_clear_all();
    LoadSquare(0x360U, 0x180U, 0x20U, 0x80U, 0x80180000);
    load_tpage_buf_lock(0x3F, 0x80180000, 2);
    load_tpage_buf_lock(0x3D, 0x80182000, 2);
    load_tpage_buf_lock(0x1D, 0x80184000, 2);
    load_tpage_buf_lock(0x1C, 0x80186000, 2);
    load_tpage_buf_lock(0x3C, 0x80188000, 2);
    load_palette(0x8018A000, 0xF, 1, 1, 0);
    load_palette(0x8018A000, 0, 1, 1, 0);
    LoadSquare(0x360U, 0x100U, 0x20U, 0x80U, 0x8018A200);
    load_palette(0x8018A000, 1, 1, 3, 0);
    load_tpage_buf_lock(0x1F, 0x8018C200, 1);
    var_a0 = 0;
    do {
        var_a0 += 4;
        temp_t9 = *(s32 *)((unsigned char *)(void *)0x80190200 + (512)) & 0x7FFF0000;
        temp_t1 = temp_t9 & 0x7FFF0000;
        temp_t3 = temp_t1 & 0x7FFF0000;
        *(s32 *)((unsigned char *)(void *)0x80190200 + (512)) = temp_t9;
        *(s32 *)((unsigned char *)(void *)0x80190200 + (512)) = temp_t1;
        *(s32 *)((unsigned char *)(void *)0x80190200 + (512)) = temp_t3;
        *(s32 *)((unsigned char *)(void *)0x80190200 + (512)) = (s32) (temp_t3 & 0x7FFF0000);
    } while (var_a0 != 0x10);
    LoadSquare(0x100U, 0x1F8U, 0x100U, 8U, 0x80190200);
    func_8009C674(0);
}
