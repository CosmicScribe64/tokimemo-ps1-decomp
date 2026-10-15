extern s8 D_801214A9;
extern s8 D_801214ED;
extern s8 D_80121641;
extern s32 D_8015EC9C;
extern s32 D_8015ECA0;
extern s32 D_8015ECA4;
extern s32 D_8015ECA8;
extern s32 D_8015ECAC;
extern s32 D_8015ECB0;
extern s32 D_8015ECB4;
extern s32 D_8015ECB8;
extern s32 D_8015ECC4;
extern s32 D_8015ECC8;
extern s32 D_8015ECCC;
extern s32 D_8015ECD0;
extern s32 D_8015ECD4;
extern s32 D_8015ECD8;
extern s32 D_8015ECDC;
extern s32 D_8015ECE0;
extern s32 D_8015ECFC;
extern s32 D_8015ED00;
extern s32 D_8015ED04;
extern s32 D_8015ED08;

void func_80134E78(void) {
    s32 *var_s2;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 var_s3;
    s32 var_t9;
    u8 *var_s0;

    D_8015EC78 = -0x1C0;
    D_8015EC7C = 0x280;
    D_8015EC80 = -0x140;
    D_80121399 = 2;
    D_801213DD = 2;
    D_801214A9 = 8;
    D_801214ED = 8;
    D_8015EC74 = 0x300;
    func_8013E97C(0x32, 0x300, 0x88);
    func_8013E97C(0x33, D_8015EC74, 0x88);
    func_8013E97C(0x36, D_8015EC7C, 0x50);
    func_8013E97C(0x37, D_8015EC7C, 0x50);
    func_8013E810(0x32, 6, 1, 1);
    func_8013E810(0x33, 5, 1, 1);
    func_8013E810(0x36, 6, 1, 1);
    func_8013E810(0x37, 5, 1, 1);
    temp_v0 = func_8013E9A8(0x20);
    D_8015EC9C = 0x2F0;
    D_8015ECC4 = -0x190;
    D_8015ECEC = 0x7D;
    temp_v1 = temp_v0 + 0x10;
    D_8015ECA0 = temp_v1 + 0x310;
    D_8015ECC8 = temp_v1 - 0x170;
    D_8015ECF0 = 0x7A;
    temp_s0 = func_8013E9A8(0x10) + 0x20;
    temp_v0_2 = func_8013E9A8(0x10);
    D_8015ECA4 = 0x2B0;
    D_8015ECCC = -0x190;
    D_8015ECF4 = 0x6C;
    D_8015ECA8 = temp_s0 + 0x2C0;
    D_8015ECD0 = temp_s0 - 0x180;
    D_8015ECF8 = 0x69;
    temp_v1_2 = temp_v0_2 + temp_s0;
    D_8015ECAC = temp_v1_2 + 0x2F0;
    D_8015ECD4 = temp_v1_2 - 0x150;
    D_8015ECFC = 0x6F;
    temp_s0_2 = func_8013E9A8(0x10) + 0x10;
    temp_v0_3 = func_8013E9A8(0x10);
    D_8015ECB0 = temp_s0_2 + 0x2A0;
    D_8015ECD8 = temp_s0_2 - 0x160;
    D_8015ED00 = 0x61;
    temp_v1_3 = temp_v0_3 + temp_s0_2;
    D_8015ECB4 = temp_v1_3 + 0x2D0;
    D_8015ECDC = temp_v1_3 - 0x130;
    D_8015ED04 = 0x5E;
    switch (D_8015EBAC) {
    case 0:
        D_8015ECB8 = 0x2F8;
        D_8015ECE0 = -0x148;
        D_8015ED08 = 0x64;
        D_8015EC68 = 0x77;
        var_t9 = 0x3E;
block_7:
        D_8015EC6C = var_t9;
        break;
    case 1:
        D_8015ECB8 = 0x2D0;
        D_8015ECE0 = -0x130;
        D_8015ED08 = 0x5B;
        D_8015EC68 = 0x3F;
        D_8015EC6C = 0x2E;
        break;
    case 2:
        D_8015ECB8 = 0x310;
        D_8015ECE0 = -0x170;
        D_8015ED08 = 0x85;
        D_8015EC68 = 0x37;
        var_t9 = 0x5E;
        goto block_7;
    case 3:
        D_8015ECB8 = 0x310;
        D_8015ECE0 = -0x170;
        D_8015ED08 = 0x85;
        D_8015EC68 = 0x4C;
        D_8015EC6C = 0xE;
        break;
    case 4:
        D_8015ECB8 = 0x310;
        D_8015ECE0 = -0x170;
        D_8015ED08 = 0x85;
        D_8015EC68 = 0x54;
        var_t9 = 0x2A;
        goto block_7;
    }
    var_s0 = D_8011ECD0;
    var_s2 = &D_8015EBAC;
    var_s3 = 0;
    do {
        temp_s1 = var_s3 + 0x28;
        func_8013E810(temp_s1, 5, 1, 1);
        func_8013E97C(temp_s1, *(s32 *)((unsigned char *)var_s2 + (240)), *(s32 *)((unsigned char *)var_s2 + (320)));
        *(s16 *)((unsigned char *)var_s0 + (9272)) = func_8013E9A8(4);
        temp_v1_4 = *(s32 *)((unsigned char *)var_s2 + (320));
        var_s3 += 1;
        *(s16 *)((unsigned char *)var_s0 + (9256)) = func_8013E9A8(7);
        if (temp_v1_4 >= 0x85) {
            *(s8 *)((unsigned char *)var_s0 + (9249)) = 3;
        } else if (temp_v1_4 >= 0x66) {
            *(s8 *)((unsigned char *)var_s0 + (9249)) = 4;
        } else if (temp_v1_4 >= 0x64) {
            *(s8 *)((unsigned char *)var_s0 + (9249)) = 5;
        } else if (temp_v1_4 >= 0x5D) {
            *(s8 *)((unsigned char *)var_s0 + (9249)) = 6;
        } else {
            *(s8 *)((unsigned char *)var_s0 + (9249)) = 7;
        }
        var_s2 += 4;
        var_s0 += 0x44;
    } while (var_s3 != 8);
    if (D_8015EDC4 & 0x100000) {
        D_801215FD = 5;
        D_80121641 = 7;
        D_80121685 = 3;
    } else {
        D_801215FD = 3;
    }
    func_8014EBA8();
}
