void func_8013769C(void) {
    typedef struct { s32 w[54]; } M2cBlockD8; /* m2c memcpy of 216 bytes */
    typedef struct { s32 w[57]; } M2cBlockE4; /* m2c memcpy of 228 bytes */
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    s32 sp13C[55];
    s32 sp54[57];
    s16 temp_s0;
    s16 temp_s0_2;
    s16 var_s0;
    s16 var_s0_2;
    s32 temp_at;
    s32 temp_at_2;
    s32 temp_hi;
    void *temp_s1;
    void *temp_v1;

    *(M2cBlockD8 *)(&sp13C) = *(M2cBlockD8 *)(&D_80139FAC);
    *(s32 *)((unsigned char *)(&sp13C) + (216)) = (s32) *(s32 *)((unsigned char *)(&D_80139FAC) + (216));
    *(M2cBlockE4 *)(&sp54) = *(M2cBlockE4 *)(&D_8013A088);
    *(? *)((unsigned char *)(&sp54 + 0xE4) + (-1)) = M2C_LWL(*(? *)((unsigned char *)(&D_8013A088 + 0xE4) + (2)));
    D_800E6376 &= 0xFFEF;
    D_80139ADC = 0xD;
    D_80139AE0 = 0;
    D_80139AD0 = (KObj *) D_80139A74;
    D_80139AD4 = D_80139A94;
    D_80139AD8 = D_80139AB4;
    load_palette(D_80139DEC, 0x10, 1, 0, 0);
    load_palette(D_80139DEC, 0x11, 1, 0, 1);
    func_800AE0F0(D_800CA25C, &sp13C + (((u16) D_800E6374 >> 0xC) * 0x14));
    func_800AE0F0(&D_800CA280, &sp54 + (((u16) D_800E6374 >> 0xC) * 0x15));
    func_80048F64(0x60);
    D_80120651 = 0xB;
    D_80120652 = 0;
    D_80120688 = 0x01000000;
    D_80120653 = 4;
    D_8012065C = (u8 *) D_80139DD4;
    D_80120660 = (u8 *) D_80139DDC;
    D_80120684 = (u8 *) D_80139DCC;
    D_80120664 = D_80139DE4;
    D_80120666 = 0;
    D_80120668 = 0;
    D_80120656 = 1;
    D_80120693 = 0;
    D_80120676 = -0xA0;
    D_8012067A = -0x78;
    D_80120657 = 0;
    D_80120655 = 0x80;
    D_80120654 = 0;
    func_800AE120(D_800E7378);
    *(M2cBlock44 *)(D_80120694) = *(M2cBlock44 *)(D_80120650);
    D_80120696 = 0x40;
    D_801206A0 = D_80139DD8;
    D_801206A4 = D_80139DE0;
    D_801206C8 = D_80139DD0;
    D_801206A8 = D_80139DE8;
    var_s0 = 0x62;
    D_801206BA = (func_800AE0D0() % 320) - 0xA0;
    D_801206BE = 0x3C;
    do {
        temp_s1 = D_8011ECD0 + (var_s0 * 0x44);
        *(M2cBlock44 *)(temp_s1) = *(M2cBlock44 *)(D_80120694);
        temp_s0 = var_s0 + 1;
        temp_at = temp_s0 < 0x74;
        var_s0 = temp_s0;
        *(s16 *)((unsigned char *)temp_s1 + (24)) = (s16) (func_800AE0D0() % 10);
    } while (temp_at != 0);
    var_s0_2 = 0;
    do {
        *(&D_80139F38 + (var_s0_2 * 2)) = (func_800AE0D0() % 5) + 1;
        temp_hi = func_800AE0D0() % 320;
        temp_s0_2 = var_s0_2 + 1;
        temp_v1 = &D_80139F10 + (var_s0_2 * 4);
        temp_at_2 = temp_s0_2 < 0xA;
        var_s0_2 = temp_s0_2;
        *(s16 *)((unsigned char *)temp_v1 + (2)) = 0x3C;
        temp_v1->unk0 = (s16) (temp_hi - 0xA0);
    } while (temp_at_2 != 0);
    func_8004284C();
}
