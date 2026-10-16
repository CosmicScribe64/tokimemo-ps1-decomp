extern ? D_800B0A28;

void func_801166A8(void) {
    s16 temp_t0;
    s32 temp_t4;

    if (D_800EECBC != 0) {
        if (!(*(&D_800B0A28 + (D_800B1746 * 0x34)) & 1)) {
            D_8012531C = 2;
            func_80078970(D_800947C4, "メダルゲームコーナー");
        } else {
            D_8012531C = 3;
            func_80078970(D_800947C4, "メダルゲームコーナー");
        }
        temp_t4 = D_8012531C * 3;
        D_800EECE4 = 0x10;
        D_8012439C = (s16) *((D_800B1746 * 0xC) + D_801251F8 + temp_t4);
        D_8012439E = (s16) *(u8 *)((unsigned char *)((D_800B1746 * 0xC) + D_801251F8 + temp_t4) + (1));
        D_801243A0 = (s16) *(u8 *)((unsigned char *)((D_800B1746 * 0xC) + D_801251F8 + temp_t4) + (2));
        D_8012438C = (s16) *((D_800B1746 * 0xC) + D_801251FC + temp_t4);
        D_8012438E = (s16) *(u8 *)((unsigned char *)((D_800B1746 * 0xC) + D_801251FC + temp_t4) + (1));
        D_80124390 = (s16) *(u8 *)((unsigned char *)((D_800B1746 * 0xC) + D_801251FC + temp_t4) + (2));
    }
    temp_t0 = D_80094730 + (D_8012531C * 2);
    D_80094730 = temp_t0;
    if ((D_800B1746 == 7) && ((u8) D_8012531C >= 2U)) {
        D_80094730 = temp_t0 + 3;
    }
    func_80011DFC();
}
