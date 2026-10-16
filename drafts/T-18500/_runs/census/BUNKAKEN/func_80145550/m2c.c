extern s16 D_800E643E;
extern s16 D_800E6442;
extern u8 D_800E62BE;
/* NON_MATCHING: T-14700, draft-only type reconstruction; see result.json. */
void func_80145550(void) {
    typedef struct ByteTable66 { /* 0x00 */ u8 b[66]; } ByteTable66; /* size 0x42 */
    typedef struct ByteTable54 { /* 0x00 */ u8 b[54]; } ByteTable54; /* size 0x36 */
    ByteTable54 spA4;
    ByteTable66 sp60;
    ByteTable66 sp1C;
    u8 spDA; /* Original default arm reads an uninitialized stack byte. */
    s32 temp_t8;
    u8 var_v0;
    u8 var_v0_2;

    spA4 = D_80155F9C;
    sp60 = D_80155FD4;
    sp1C = D_80156018;
    temp_t8 = (D_800E62BE - 0x5F) & 0xFF;
    if (D_80122EBC == 0xB) {
        D_800E6442 += 2;
        switch (temp_t8) {
        default:
            var_v0 = spDA;
            break;
        case 0:
            var_v0 = *(spA4.b + D_80122EB8);
            break;
        case 1:
            var_v0 = *(sp60.b + D_80122EB8);
            break;
        case 2:
            var_v0 = *(sp1C.b + D_80122EB8);
            break;
        }
        if (var_v0 != 0) {
            var_v0 = (var_v0 - 1) & 0xFF;
        }
        D_800E643E += var_v0;
    } else if (D_80122EBC < 0xB) {
        D_800E6280.unk_1BC[D_80122EBC].unk_06 += 2;
        switch (temp_t8) {
        default:
            var_v0_2 = spDA;
            break;
        case 0:
            var_v0_2 = *(spA4.b + (D_80122EBC * 6) + D_80122EB8);
            break;
        case 1:
            var_v0_2 = *(sp60.b + (D_80122EBC * 6) + D_80122EB8);
            break;
        case 2:
            var_v0_2 = *(sp1C.b + (D_80122EBC * 6) + D_80122EB8);
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
