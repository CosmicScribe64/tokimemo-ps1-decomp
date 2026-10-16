? func_80133EA0(?);                                 /* extern */
? func_8013426C(?);                                 /* extern */
? func_801344C8(?);                                 /* extern */
? func_801347B8(?);                                 /* extern */
? func_801349A0(?);                                 /* extern */
? func_80134C64(?);                                 /* extern */
? func_8013518C(?);                                 /* extern */
? func_801353F8(?);                                 /* extern */
? func_80135620(?);                                 /* extern */
? func_80135804(?);                                 /* extern */
? func_80135A34(?);                                 /* extern */
? func_80135B4C(?);                                 /* extern */
? func_80135C14(?);                                 /* extern */
? func_80135E7C(?);                                 /* extern */
? func_80136340(?);                                 /* extern */
? func_801366A4(?);                                 /* extern */
? func_80136870(?);                                 /* extern */
? func_80136A74(?);                                 /* extern */
? func_80136B70(?);                                 /* extern */
? func_80136D3C(?);                                 /* extern */
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E69A4;
extern u8 D_800E738A;
extern ? D_8015BF50;
extern ? func_80133DE8;

void func_801449D4(void) {
    ? (**var_v0)(?);
    ? (*var_t7)(?);

    M2C_MEMCPY_ALIGNED(&sp2C[0], &D_8015BF50, 0x12C);
    var_v0 = &(&sp2C[0])[D_800E738A];
    *(s32 *)((unsigned char *)(&sp2C[0]) + (300)) = (s32) *(s32 *)((unsigned char *)(&D_8015BF50) + (300));
    if (&func_80133DE8 == *var_v0) {
        switch (D_800CA2F8) {
        case 1:
            *var_v0 = func_80133EA0;
            break;
        case 2:
            var_t7 = func_80135B4C;
block_23:
            *var_v0 = var_t7;
            break;
        case 3:
            *var_v0 = func_8013426C;
            break;
        case 4:
            *var_v0 = func_801344C8;
            break;
        case 5:
            *var_v0 = func_801347B8;
            break;
        case 6:
            *var_v0 = func_801349A0;
            break;
        case 7:
            *var_v0 = func_80134C64;
            break;
        case 8:
            *var_v0 = func_8013518C;
            break;
        case 9:
            *var_v0 = func_801353F8;
            break;
        case 10:
            *var_v0 = func_80135620;
            break;
        case 11:
            var_t7 = func_80135804;
            goto block_23;
        case 12:
            *var_v0 = func_80135A34;
            break;
        case 13:
            *var_v0 = func_80135C14;
            break;
        case 14:
            *var_v0 = func_80135E7C;
            break;
        case 15:
            *var_v0 = func_80136340;
            break;
        case 16:
            *var_v0 = func_801366A4;
            break;
        case 17:
            *var_v0 = func_80136870;
            break;
        case 18:
            *var_v0 = func_80136A74;
            break;
        case 19:
            *var_v0 = func_80136B70;
            break;
        case 20:
            var_t7 = func_80136D3C;
            goto block_23;
        }
    }
    if (D_800E738A != 0) {
        if (((D_800E62BE == 0x5F) && (D_800E62BF == 7) && (D_800E62C0 == 0x17)) || ((D_800E62BE == 0x60) && (D_800E62BF == 7) && (D_800E62C0 == 0x1C)) || ((D_800E62BE == 0x61) && (D_800E62BF == 7) && (D_800E62C0 == 0x1B))) {
            if (D_800B593C == 0x80) {
                func_8006B900();
                var_v0 = &(&sp2C[0])[D_800E738A];
                D_80122D44 = 1;
            }
        } else if ((D_800CA2F8 == 0x11) && (D_800B593C == 0x80) && (D_800E738A != 0x14)) {
            if (D_800E738A != 0x15) {
                func_8006B900();
                var_v0 = &(&sp2C[0])[D_800E738A];
                D_80122D44 = 1;
            } else {
                goto block_42;
            }
        } else {
block_42:
            if ((D_800CA2F8 == 0x10) && (D_800B593C == 0x80) && (D_800E738A != 0x14) && (D_800E738A != 0x15)) {
                func_8006B900();
                var_v0 = &(&sp2C[0])[D_800E738A];
                D_80122D44 = 1;
            } else if ((D_800E738A == 0x14) && (D_800E69A4 == 0x11)) {
                func_8006B900();
                var_v0 = &(&sp2C[0])[D_800E738A];
                D_80122D44 = 1;
            } else if ((D_800E738A == 0x14) && (D_800E69A4 == 0x10)) {
                func_8006B900();
                var_v0 = &(&sp2C[0])[D_800E738A];
                D_80122D44 = 1;
            }
        }
    }
    *var_v0(0x80);
    if ((D_800CA2F8 == 0xB) && ((u8) D_800E738A >= 0x2CU) && ((u8) D_800E738A < 0x30U) && (D_800B3CFC == 1) && (D_800CA2FC != 2)) {
        D_800CA2FC = 2;
        func_80044750(0x601);
    }
}
