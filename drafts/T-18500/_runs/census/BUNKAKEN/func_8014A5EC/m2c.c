extern u8 D_800E62BE;
/* NON_MATCHING: T-14700, draft-only type reconstruction; see result.json. */
s32 func_8014A5EC(void) {
    typedef struct ByteTable297 { /* 0x00 */ u8 b[297]; } ByteTable297; /* size 0x129 */
    ByteTable297 sp28;

    sp28 = D_8015633C;
    func_800AE0F0(D_800CA25C, (sp28.b + (D_800E62BE * 0x63) + (D_80122CDC * 0x21)) - 0x24BD);
}
