extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E62C1;
extern u16 D_800E6374;
extern u8 D_800E644A;
extern s32 D_800E6480;
extern s32 D_800E64B8;
extern s32 D_800E64F0;
extern u8 D_800E71DE;

void func_801433C0(void) {
    u8 sp2B;
    u32 temp_t7;
    u8 temp_t3;
    u8 var_v0;

    temp_t7 = (u16) D_800E6374 >> 0xC;
    switch (temp_t7) {
    case 0:
        if ((((u8) D_800E644A >> 4) == 0) || ((var_v0 = 1, (((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 0)) && (var_v0 = 1, (((u32) (D_800E6480 << 0x1E) >> 0x1F) == 1)))) {
            var_v0 = 2;
        }
block_38:
        sp2B = var_v0;
    default:
        D_800E6280.unk_75E[D_800E71DE].unk_00 = (u8) D_80122EBC;
        D_800E6280.unk_75E[D_800E71DE].unk_02 = 0x14;
        D_800E6280.unk_75E[D_800E71DE].unk_01 = sp2B;
        D_800E6280.unk_75E[D_800E71DE].unk_03 = func_80051B48(D_80122EBC);
        D_800E6280.unk_75E[D_800E71DE].unk_04 = D_800E62BE;
        D_800E6280.unk_75E[D_800E71DE].unk_05 = D_800E62BF;
        D_800E6280.unk_75E[D_800E71DE].unk_06 = D_800E62C0;
        D_800E6280.unk_75E[D_800E71DE].unk_07 = D_800E62C1;
        temp_t3 = D_800E71DE + 1;
        D_800E71DE = temp_t3;
        if ((u32) (temp_t3 & 0xFF) >= 0xFFU) {
            D_800E71DE = 0xFE;
        }
        break;
    case 1:
        if (((u8) D_800E644A >> 4) != 1) {
            if (((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 1) {
                var_v0 = 5;
                if (((u32) (D_800E6280.unk_1BC[1].unk_0C.w << 0x1E) >> 0x1F) == 1) {
                    goto block_11;
                }
            } else {
                var_v0 = 5;
            }
        } else {
block_11:
            var_v0 = 6;
        }
        goto block_38;
    case 2:
        if ((((u8) D_800E644A >> 4) == 2) || ((var_v0 = 9, (((u8) D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 2)) && (var_v0 = 9, (((u32) (D_800E64B8 << 0x1E) >> 0x1F) == 1)))) {
            var_v0 = 0xA;
        }
        goto block_38;
    case 3:
        if ((((u8) D_800E644A >> 4) == 3) || ((var_v0 = 0xD, (((u8) D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 3)) && (var_v0 = 0xD, (((u32) (D_800E6280.unk_1BC[2].unk_0C.w << 0x1E) >> 0x1F) == 1)))) {
            var_v0 = 0xE;
        }
        goto block_38;
    case 4:
        if ((((u8) D_800E644A >> 4) == 4) || ((((u8) D_800E6280.unk_1BC[3].unk_0C.b[2] >> 4) == 4) && (((u32) (D_800E64F0 << 0x1E) >> 0x1F) == 1))) {
            var_v0 = 0x12;
            goto block_32;
        }
        var_v0 = 0x11;
        if ((D_80122EBC != 0xA) || (var_v0 = 0x11, (func_8008667C(0xA) != 4))) {
block_32:
            goto block_38;
        }
        break;
    case 5:
        if ((((u8) D_800E644A >> 4) == 5) || ((var_v0 = 0x15, (((u8) D_800E6280.unk_1BC[3].unk_0C.b[2] >> 4) == 5)) && (var_v0 = 0x15, (((u32) (D_800E6280.unk_1BC[3].unk_0C.w << 0x1E) >> 0x1F) == 1)))) {
            var_v0 = 0x16;
        }
        goto block_38;
    }
}
