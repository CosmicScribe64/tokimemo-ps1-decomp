void func_80143758(void) {
    s16 var_a2;
    s32 var_a0;
    u8 *var_a3;
    u8 *var_v1;
    u8 *var_a1;

    var_a0 = D_80150144;
    var_a1 = (var_a0 * 2) + D_801503C8;
    var_a3 = D_80120650;
    var_v1 = D_8011ECD0;
    var_a2 = 0x8C;
    do {
        *(s16 *)((unsigned char *)var_v1 + (6552)) = (s16) (*(s32 *)var_a1);
        *(s8 *)((unsigned char *)var_v1 + (6529)) = 0;
        *(s8 *)((unsigned char *)var_v1 + (6530)) = 0x40;
        *(s32 *)((unsigned char *)var_v1 + (6584)) = 0;
        *(s8 *)((unsigned char *)var_v1 + (6531)) = 0xA4;
        *(s32 *)((unsigned char *)var_v1 + (6540)) = 0x801F86EC;
        *(s32 *)((unsigned char *)var_v1 + (6544)) = 0x801F897C;
        *(s32 *)((unsigned char *)var_v1 + (6580)) = 0x801F86A4;
        *(s8 *)((unsigned char *)var_v1 + (6532)) = 8;
        *(s8 *)((unsigned char *)var_v1 + (6595)) = 0xF;
        *(s8 *)((unsigned char *)var_v1 + (6533)) = 0x80;
        *(s16 *)((unsigned char *)var_v1 + (6566)) = (s16) (*(u8 *)((unsigned char *)var_a1 + (1)) - 0xA0);
        *(s16 *)((unsigned char *)var_v1 + (6570)) = var_a2;
        *(s16 *)((unsigned char *)var_v1 + (6574)) = 0;
        *(s32 *)((unsigned char *)var_v1 + (6576)) = -1;
        *(s8 *)((unsigned char *)var_v1 + (6535)) = 0x80;
        if (*(s16 *)((unsigned char *)var_v1 + (6552)) >= 0xF1) {
            *(s8 *)((unsigned char *)var_a3 + (3)) = 0x34;
        } else {
            *(s8 *)((unsigned char *)var_a3 + (3)) = 0xB4;
        }
        var_a3 += 0x44;
        var_v1 += 0x44;
        var_a2 += 0x28;
        var_a0 += 1;
        var_a1 += 2;
    } while (var_a3 != &D_8012082C);
    D_80150144 = var_a0;
}
