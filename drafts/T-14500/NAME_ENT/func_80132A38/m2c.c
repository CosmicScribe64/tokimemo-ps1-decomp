s32 func_800486A4(s32, ?);                          /* extern */
extern ? D_800E74A8;
extern s32 *D_800E7CBC;
extern s32 D_800E7CEC;
extern s32 D_800E7CFC;
extern s32 D_800E7D04;

void func_80132A38(void) {
    s32 sp50;
    s32 *temp_v0;
    s32 temp_v1;

    func_80138F00();
    temp_v1 = 2 << 0xE;
    sp50 = D_800E7CFC + 0x4000;
    temp_v0 = &(&sp4C[0])[2];
    sp4C[0] = D_800E7CFC;
    temp_v0->unk0 = D_800E7CFC + temp_v1;
    *(s32 *)((unsigned char *)temp_v0 + (4)) = (s32) (D_800E7CFC + temp_v1 + 0x4000);
    *(s32 *)((unsigned char *)temp_v0 + (12)) = (s32) (D_800E7CFC + temp_v1 + 0xC000);
    *(s32 *)((unsigned char *)temp_v0 + (8)) = (s32) (D_800E7CFC + temp_v1 + 0x8000);
    func_8004A1A0((s32) &sp4C[0], 0, 6, 3, 2, 0x1F8, -0xA0, -0x78, 8, 0xD);
    D_801220D0 |= 0x40000000;
    *(&D_800E74A8 + (func_800486A4(*D_800E7CBC, 2) * 4)) = *D_800E7CBC;
    func_80043914(D_800E7D04, 0x18, 1, 2, 0);
    func_80043914(D_800E7CEC, 0x10, 1, 2, 0);
}
