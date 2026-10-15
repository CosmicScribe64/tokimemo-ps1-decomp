extern u8 D_800E62BE;
extern s16 D_800E643E;
extern s16 D_800E6442;
extern ? D_80155F9C;
extern ? D_80155FD4;
extern ? D_80156018;

void func_80145550(void) {
    ? spA4;
    ? sp60;
    ? sp1C;
    s32 temp_t8;
    u8 var_v0;
    u8 var_v0_2;

    M2C_MEMCPY_ALIGNED(&spA4, &D_80155F9C, 0x34);
    *(u16 *)((unsigned char *)(&spA4 + 0x30) + (4)) = (u16) *(u16 *)((unsigned char *)(&D_80155F9C + 0x30) + (4));
    M2C_MEMCPY_ALIGNED(&sp60, &D_80155FD4, 0x40);
    *(u16 *)((unsigned char *)(&sp60 + 0x3C) + (4)) = (u16) *(u16 *)((unsigned char *)(&D_80155FD4 + 0x3C) + (4));
    M2C_MEMCPY_ALIGNED(&sp1C, &D_80156018, 0x40);
    *(u16 *)((unsigned char *)(&sp1C + 0x3C) + (4)) = (u16) *(u16 *)((unsigned char *)(&D_80156018 + 0x3C) + (4));
    temp_t8 = (D_800E62BE - 0x5F) & 0xFF;
    if (D_80122EBC == 0xB) {
        D_800E6442 += 2;
        switch (temp_t8) {                          /* switch 1; irregular */
        default:                                    /* switch 1 */
            var_v0 = spDA;
            break;
        case 0:                                     /* switch 1 */
            var_v0 = *(&spA4 + D_80122EB8);
            break;
        case 1:                                     /* switch 1 */
            var_v0 = *(&sp60 + D_80122EB8);
            break;
        case 2:                                     /* switch 1 */
            var_v0 = *(&sp1C + D_80122EB8);
            break;
        }
        if (var_v0 != 0) {
            var_v0 = (var_v0 - 1) & 0xFF;
        }
        D_800E643E += var_v0;
    } else if (D_80122EBC < 0xB) {
        D_800E6280.unk_1BC[D_80122EBC].unk_06 += 2;
        switch (temp_t8) {                          /* switch 2; irregular */
        default:                                    /* switch 2 */
            var_v0_2 = spDA;
            break;
        case 0:                                     /* switch 2 */
            var_v0_2 = *(&spA4 + (D_80122EBC * 6) + D_80122EB8);
            break;
        case 1:                                     /* switch 2 */
            var_v0_2 = *(&sp60 + (D_80122EBC * 6) + D_80122EB8);
            break;
        case 2:                                     /* switch 2 */
            var_v0_2 = *(&sp1C + (D_80122EBC * 6) + D_80122EB8);
            break;
        }
        if (var_v0_2 != 0) {
            var_v0_2 = (var_v0_2 - 1) & 0xFF;
        }
        D_800E6280.unk_1BC[D_80122EBC].unk_02 += var_v0_2;
    }
    if (D_80122EA4 != 0xFF) {
        D_800E6280.unk_1BC[D_80122EA4].unk_06 -= 0xA;
        D_800E6280.unk_1BC[D_80122EA4].unk_0A += 0xA;
    }
}
