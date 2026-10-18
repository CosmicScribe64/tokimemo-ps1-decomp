/* T-24056 source-only canonical flag reconstruction; not matched. See README.md. */
/* Scratch-only declaration: target LH source for actor +0x14. No production header addition. */
extern s16 D_80145FDC;
void func_8013DF14(void) {
    func_80085B3C(4, 0);
    D_800E6280.unk_1BC[12].unk_0C.f.f9 += 1;
    if (D_800E6280.unk_1BC[12].unk_0C.f.f9 >= 4) {
        D_800E6280.unk_1BC[12].unk_0C.f.f9 = 3;
    }
    func_800AE0F0(D_800CA188, "伊集院");
    func_800AE0F0(girl_name_buf, "伊集院");
    func_800AE0F0(D_800CA1DC, "パーティー会場");
    D_80145F2C = 9;
    func_8004284C();
    func_8013DD90();
    load_palette(D_80145FE0, 0x11, 1, 3, 0);
    func_80048F64(0x60);
    D_80120651 = 0xC;
    D_80120652 = 1;
    D_80120688 = 0x41000000;
    D_80120653 = 4;
    D_8012065C = (u8 *) D_80145FD4;
    D_80120660 = (u8 *) D_80145FD8;
    D_80120684 = (u8 *) D_80145FD0;
    D_80120664 = D_80145FDC;
    D_80120666 = 0;
    D_80120668 = 0;
    D_80120693 = 0x11;
    D_80120676 = -0xA0;
    D_8012067A = -0x78;
    D_80120657 = 0;
    D_80120655 = 0;
    D_80120654 = 8;
}
