/* NON_MATCHING: T-14700, draft-only type reconstruction; see result.json. */
s32 func_8013B10C(void) {
    s32 index;
    RepairFnTbl42 sp28;
    s32 var_v1;

    sp28 = D_80146630;
    index = D_800E738A;
    sp28.f[index](0x80);
    if (D_80122D04 != 0) {
        D_80120723 = D_800B593C;
        D_8012071F |= 0x80;
        if (D_800E62BC != 0) {
            if ((u8) D_8014662C == 0) {
                D_8012071E = 3;
            }
            if ((D_80120734 == 6) && ((u8) D_8014662C == 0)) {
                D_8012071E = 0;
            }
            var_v1 = 0;
            if (func_800460EC() & 4) {
                var_v1 = getCDlevel();
            }
            if (var_v1 != 0) {
                if ((u8) D_8014662C == 0) {
                    D_8012071E = 3;
                    D_8014662C = 1;
                }
            }
        } else if ((u8) D_8014662C == 0) {
            D_8012071E = 3;
            D_80120734 = 1;
            D_8014662C = 1;
        }
    } else {
        D_8012071F &= 0xFF7F;
    }
}
