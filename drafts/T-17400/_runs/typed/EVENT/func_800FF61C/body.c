void func_800FF61C(void) {
    s32 idx; /* declared before the table: T-3330 */
    FnTbl48 sp2C;

    sp2C = D_80121AF8;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    sp2C.f[idx](0x80);
    if (D_800EECE0 != 0) {
        *(u8 *)((unsigned char *)D_800E9620 + (6667)) = (u8) (*(u8 *)((unsigned char *)D_800E9620 + (6667)) | 0x80);
        *(u8 *)((unsigned char *)D_800E9620 + (6671)) = (u8) D_800F19AB;
    } else {
        *(u8 *)((unsigned char *)D_800E9620 + (6667)) = (u8) (((DisplayActor44 *)D_800E9620)->unk_00 & 0xFF7F);
    }
    if ((u8) D_800B1AF6 >= 0x10U) {
        if ((u8) D_800B1AF6 < 0x13U) {
            func_8010AFE4();
        }
    }
}
