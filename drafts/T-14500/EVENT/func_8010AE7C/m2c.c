extern ? D_80123FCC;

void func_8010AE7C(void) {
    M2C_MEMCPY_ALIGNED(&sp2C[0], &D_80123FCC, 0xE0);
    (&sp2C[0])[D_800B1AF6](0x80);
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
