u16 func_8007B64C(void) {
    u16 i;
    u16 ret;

    if (D_80125CC0[0] > 0x20) {
        D_80125CC0[0] = 0x20;
    }
    if (D_80125CC0[0] == 0) {
        return 0;
    }
    ret = D_80125CC0[1];
    for (i = 1; i < D_80125CC0[0]; i++) {
        D_80125CC0[i] = D_80125CC0[i + 1];
    }
    D_80125CC0[0]--;
    return ret;
}
