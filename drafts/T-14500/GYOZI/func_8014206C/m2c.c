extern u8 D_8012B98F;
extern ? D_8012B9D0;
extern u8 D_8012BA17;

void func_8014206C(void) {
    if ((D_801486E4 != 0) && (D_801486E4 != 7)) {
        D_8012B94B |= 0x80;
    }
    if ((D_801486E4 != 0) && (D_801486E4 != 7)) {
        D_8012B98F |= 0x80;
    }
    if ((D_801486E4 != 0) && (D_801486E4 != 7)) {
        *(u8 *)((unsigned char *)(&D_8012B9D0) + (3)) = (u8) (*(u8 *)((unsigned char *)(&D_8012B9D0) + (3)) | 0x80);
    }
    D_8012BA17 |= 0x80;
    D_80148824 = 1;
    func_8004DE1C();
}
