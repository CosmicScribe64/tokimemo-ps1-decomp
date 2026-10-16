
void func_80134CB4(void) {
    s16 var_s0;
    s32 temp_lo;
    u8 *temp_v0;

    D_80143DDC = 0;
    func_80134890();
    load_palette(D_80143DD0, 0x11, 1, 2, 0);
    var_s0 = 0;
    do {
        func_80048F64(var_s0 + 0x60);
        temp_lo = var_s0 * 0x44;
        var_s0 += 1;
        temp_v0 = &D_8011ECD0[temp_lo];
        *(s8 *)((unsigned char *)temp_v0 + (6529)) = 0xA;
        *(s8 *)((unsigned char *)temp_v0 + (6530)) = 0;
        *(s32 *)((unsigned char *)temp_v0 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)temp_v0 + (6531)) = 4;
        *(s32 *)((unsigned char *)temp_v0 + (6540)) = (s32) D_80143DB8;
        *(s32 *)((unsigned char *)temp_v0 + (6544)) = (s32) D_80143DC0;
        *(s32 *)((unsigned char *)temp_v0 + (6580)) = (s32) D_80143DB0;
        *(s16 *)((unsigned char *)temp_v0 + (6550)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (6552)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (6534)) = 1;
        *(s8 *)((unsigned char *)temp_v0 + (6595)) = 0x11;
        *(s16 *)((unsigned char *)temp_v0 + (6566)) = -0xA0;
        *(s16 *)((unsigned char *)temp_v0 + (6570)) = -0x78;
        *(s8 *)((unsigned char *)temp_v0 + (6535)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (6533)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (6532)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (6548)) = (s16) D_80143DC8;
    } while (var_s0 < 3);
    D_80120676 = 0;
    D_801206AA = 3;
    D_801206EE = 4;
    load_palette(D_80143DD4, 0x12, 0, 0, 2);
    func_80048F64(0x63);
    D_8012071D = 0xD;
    D_8012071E = 0;
    D_80120754 = 0;
    D_8012071F = 4;
    D_80120728 = D_80143DBC;
    D_8012072C = D_80143DC4;
    D_80120750 = D_80143DB4;
    D_80120730 = D_80143DCC;
    D_80120732 = 0;
    D_80120734 = 0;
    D_80120722 = 1;
    D_8012075F = 0;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
    D_80120723 = 0x80;
    D_80120721 = 0x81;
    D_80120720 = 0;
    func_8004284C();
}
