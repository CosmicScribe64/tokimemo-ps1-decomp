extern s32 D_8011ED14;
void func_80134938(void) {
    s32 *var_s0;
    s16 temp_t9;
    s32 temp_t0;
    s32 var_s1;
    s32 var_s1_2;

    var_s1 = 0;
    do {
        load_palette(D_8013C71C, (var_s1 + 0x10) & 0xFF, 1, 1, var_s1);
        var_s1 += 1;
    } while (var_s1 < 0x10);
    func_80048F64(0x60);
    D_80120651 = 5;
    D_80120688 = 0x01000000;
    D_80120653 = 0x80;
    D_8012065C = D_8013C714;
    D_80120660 = D_8013C718;
    D_80120684 = D_8013C710;
    D_80120666 = 0xA;
    D_80120657 = 0;
    D_80120676 = -0xA0;
    D_8012067A = -0x78;
    D_80120655 = 0x80;
    D_80120693 = 0;
    D_80120654 = 0;
    var_s0 = &D_8011ED14;
    var_s1_2 = 1;
    do {
        func_80048F64(var_s1_2 + 0x60);
        *(s8 *)((unsigned char *)var_s0 + (6529)) = 4;
        *(s32 *)((unsigned char *)var_s0 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)var_s0 + (6531)) = 0;
        *(u8 **)((unsigned char *)var_s0 + (6540)) = (u8 *) D_8013C714;
        *(u8 **)((unsigned char *)var_s0 + (6544)) = (u8 *) D_8013C718;
        temp_t9 = var_s1_2 - 1;
        var_s1_2 += 1;
        var_s0 += 0x44;
        *(s16 *)((unsigned char *)var_s0 + (6482)) = temp_t9;
        *(s8 *)((unsigned char *)var_s0 + (6466)) = 1;
        *(s8 *)((unsigned char *)var_s0 + (6467)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (6498)) = -0xA0;
        *(s16 *)((unsigned char *)var_s0 + (6502)) = -0x78;
        *(s8 *)((unsigned char *)var_s0 + (6465)) = 0x80;
        *(s8 *)((unsigned char *)var_s0 + (6527)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6464)) = 0;
        *(u8 **)((unsigned char *)var_s0 + (6512)) = (u8 *) D_8013C710;
    } while (var_s1_2 != 5);
    temp_t0 = D_80122EAC * 2;
    D_80120732 = temp_t0 + 2;
    D_8012071F = 0x80;
    D_80120776 = temp_t0 + 3;
    search_load_tpage_buf_lock(*(s32 *)((unsigned char *)D_8013C710 + (44)), 1);
    search_load_tpage_buf_lock(*(s32 *)((unsigned char *)D_8013C710 + (48)), 1);
    search_load_tpage_buf_lock(*(s32 *)((unsigned char *)D_8013C710 + (52)), 1);
    search_load_tpage_buf_lock(*(s32 *)((unsigned char *)D_8013C710 + (56)), 1);
}
