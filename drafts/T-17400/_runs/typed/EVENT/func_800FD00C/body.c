void func_800FD00C(void) {
    s32 idx; /* declared before the table: T-3330 */
    FnTbl39 sp30;

    sp30 = D_801214A0;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    sp30.f[idx](0x80);
    if (D_800EECE0 != 0) {
        *(u8 *)((unsigned char *)D_800E9620 + (6667)) = (u8) (*(u8 *)((unsigned char *)D_800E9620 + (6667)) | 0x80);
        if ((u32) ((u32) D_800B1AE4 % 40U) < 0x14U) {
            *(s32 *)((unsigned char *)D_800EC190 + (360)) = (s32) (*(s32 *)((unsigned char *)D_800EC190 + (360)) & 0x7FFFFFFF);
        } else {
            *(s32 *)((unsigned char *)D_800EC190 + (360)) = (s32) (((Rec24 *)D_800EC190)->unk_00 | 0x80000000);
        }
        *(u8 *)((unsigned char *)D_800EC190 + (382)) = (u8) D_800F19AB;
        *(u8 *)((unsigned char *)D_800EC190 + (381)) = (u8) D_800F19AB;
        *(u8 *)((unsigned char *)D_800EC190 + (380)) = (u8) D_800F19AB;
        *(u8 *)((unsigned char *)D_800E9620 + (6671)) = (u8) D_800F19AB;
        return;
    }
    ((DisplayActor44 *)D_800E9620)->unk_00 &= 0xFF7F;
    *(s32 *)((unsigned char *)D_800EC190 + (360)) = (s32) (*(s32 *)((unsigned char *)D_800EC190 + (360)) | 0x80000000);
}
