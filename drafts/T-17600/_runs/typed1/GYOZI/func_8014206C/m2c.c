void func_8014206C(void) {
    if ((D_801486E4 != 0) && (D_801486E4 != 7)) {
        *(u8 *)((unsigned char *)D_80129F40 + (6667)) = (u8) (*(u8 *)((unsigned char *)D_80129F40 + (6667)) | 0x80);
    }
    if ((D_801486E4 != 0) && (D_801486E4 != 7)) {
        *(u8 *)((unsigned char *)D_80129F40 + (6735)) = (u8) (*(u8 *)((unsigned char *)D_80129F40 + (6735)) | 0x80);
    }
    if ((D_801486E4 != 0) && (D_801486E4 != 7)) {
        *(u8 *)((unsigned char *)D_80129F40 + (6803)) = (u8) (*(u8 *)((unsigned char *)D_80129F40 + (6803)) | 0x80);
    }
    *(s8 *)((unsigned char *)D_80129F40 + (6871)) = (s8) (D_80129F40->unk_00 | 0x80);
    D_80148824 = 1;
    func_8004DE1C();
}
