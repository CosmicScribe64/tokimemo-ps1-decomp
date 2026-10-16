extern s32 D_8015EE5C;
extern s32 D_8015EE68;
extern s32 D_8015EE74;
extern s32 D_8015EE80;
void func_8014C360(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp38;
    s32 sp34;
    s16 temp_a3;
    s16 temp_s0;
    s32 temp_a0;
    s32 temp_v1;
    s32 var_a1;
    u8 *temp_v0;
    u8 *var_v0;

    temp_a3 = arg1 - 0x9E;
    temp_s0 = arg2 - 0x78;
    switch (arg0) {
    case 0:
        D_8015EE5C = 0;
        D_8015EE60 = 1;
        D_8015EE64 = 0;
        break;
    case 1:
        D_8015EE68 = 0;
        D_8015EE6C = 1;
        D_8015EE70 = 0;
        break;
    case 2:
        D_8015EE74 = 0;
        D_8015EE78 = 1;
        D_8015EE7C = 0;
        break;
    case 3:
        D_8015EE80 = 0;
        D_8015EE84 = 1;
        D_8015EE88 = 0;
        break;
    }
    temp_v1 = arg0 * 0xC;
    temp_v0 = (arg0 * 0xCC * 4) + D_8011ECD0;
    temp_a0 = temp_v1 + 0x40;
    *(s16 *)((unsigned char *)temp_v0 + (4390)) = temp_a3;
    *(s16 *)((unsigned char *)temp_v0 + (4394)) = temp_s0;
    *(s16 *)((unsigned char *)temp_v0 + (4662)) = temp_a3;
    *(s16 *)((unsigned char *)temp_v0 + (4666)) = temp_s0;
    *(s16 *)((unsigned char *)temp_v0 + (4934)) = temp_a3;
    *(s16 *)((unsigned char *)temp_v0 + (4938)) = temp_s0;
    sp34 = temp_a0;
    sp38 = temp_v1;
    func_8013E864(temp_a0, 0x34, 0);
    func_8013E864(sp34 + 4, 0x35, 0);
    func_8013E864(sp34 + 8, 0x35, 0);
    var_v0 = (sp38 * 0x44) + 0x44 + D_8011ECD0;
    var_a1 = 1;
    do {
        var_a1 += 1;
        var_v0 += 0x44;
        *(s16 *)((unsigned char *)var_v0 + (4322)) = 0xA0;
        *(s16 *)((unsigned char *)var_v0 + (4326)) = 0x78;
        *(s16 *)((unsigned char *)var_v0 + (4594)) = 0xA0;
        *(s16 *)((unsigned char *)var_v0 + (4598)) = 0x78;
        *(s16 *)((unsigned char *)var_v0 + (4866)) = 0xA0;
        *(s16 *)((unsigned char *)var_v0 + (4870)) = 0x78;
    } while (var_a1 != 4);
}
