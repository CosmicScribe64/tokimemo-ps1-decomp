? func_800482FC();                                  /* extern */
? func_80048514(?, ?, ?);                           /* extern */

void func_801342CC(void) {
    s32 temp_t1;
    s32 temp_t3;
    s32 temp_t9;
    s32 var_a0;

    func_800482FC();
    func_8004435C(0x360U, 0x180U, 0x20U, 0x80U, 0x80180000);
    func_80048514(0x3F, 0x80180000, 2);
    func_80048514(0x3D, 0x80182000, 2);
    func_80048514(0x1D, 0x80184000, 2);
    func_80048514(0x1C, 0x80186000, 2);
    func_80048514(0x3C, 0x80188000, 2);
    func_80043914(0x8018A000, 0xF, 1, 1, 0);
    func_80043914(0x8018A000, 0, 1, 1, 0);
    func_8004435C(0x360U, 0x100U, 0x20U, 0x80U, 0x8018A200);
    func_80043914(0x8018A000, 1, 1, 3, 0);
    func_80048514(0x1F, 0x8018C200, 1);
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
    func_8004435C(0x100U, 0x1F8U, 0x100U, 8U, 0x80190200);
    func_8009C674(0);
}
