/* NON_MATCHING: T-14700, draft-only type reconstruction; see result.json. */
void func_8010AE7C(void) {
    s32 index;
    RepairFnTbl56 sp2C;
    sp2C = D_80123FCC;
    index = D_800B1AF6;
    sp2C.f[index](0x80);
    if ((u8) D_800B1AF6 >= 0x1EU) {
        if ((u8) D_800B1AF6 < 0x25U) {
            func_8003C2C0();
        } else {
            goto block_4;
        }
    } else {
block_4:
        if (D_800B1AF6 == 0x2B) {
            func_8010AFE4();
        }
    }
}
