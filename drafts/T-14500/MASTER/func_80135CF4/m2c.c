? func_8007E5A0();                                  /* extern */
? func_8007EAAC();                                  /* extern */
extern u16 D_800E6374;
extern s32 D_800E66B0;
extern u8 D_800E71DF;
extern u8 D_800E738A;
extern ? D_8013C6A4;

void func_80135CF4(void) {
    ? sp28;

    sp28.unk0 = (s32) D_8013C6A4.unk0;
    *(s32 *)((unsigned char *)(&sp28) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8013C6A4) + (8));
    *(s32 *)((unsigned char *)(&sp28) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8013C6A4) + (4));
    *(s32 *)((unsigned char *)(&sp28) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8013C6A4) + (12));
    *(s32 *)((unsigned char *)(&sp28) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8013C6A4) + (16));
    *(u16 *)((unsigned char *)(&sp28) + (24)) = (u16) *(u16 *)((unsigned char *)(&D_8013C6A4) + (24));
    *(s32 *)((unsigned char *)(&sp28) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_8013C6A4) + (20));
    func_80083808();
    switch (D_800E738A) {
    case 0:
        if (cd_read_sync_status() & 1) {
            func_800847B8(D_8013C584);
            D_800CA148 = *(s16 *)((unsigned char *)(sp + (D_8013C584 * 2)) + (40));
            if ((D_8013C584 == 4) && (((u16) D_800E6374 >> 0xC) == 6)) {
                D_800CA148 = 0x3C;
            }
            D_800CA14C = 0;
            func_8007C740();
            D_800E71DF = D_8013C584;
            func_80083474();
            if (D_8013C584 == 0xC) {
                func_80083440(0U);
            }
        }
        break;
    case 1:
        func_8007EAAC();
        break;
    case 2:
        if (D_8013C584 == 0xB) {
            func_80044750(0x500);
            func_80047550();
        }
        func_8004500C(0, 0);
        func_8004284C();
        break;
    case 3:
        func_80085A60();
        break;
    case 4:
        func_8007E390();
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        if (D_8013C584 == 0xB) {
            if (((u32) (D_800E66B0 << 0x14) >> 0x1D) == 0) {
                D_800CA148 = 0x1E;
            } else {
                D_800CA148 = 0x1F;
            }
            func_8007E5A0();
        } else if (D_8013C584 == 0xC) {
            D_800CA148 = 0x20;
            func_8004284C();
        } else {
            D_800CA148 = 0x1D;
            func_8004284C();
        }
        break;
    case 7:
        func_80085A60();
        break;
    case 8:
        if (D_8013C584 == 0xB) {
            func_8004284C();
        } else {
            func_8007E5A0();
        }
        break;
    case 9:
        func_8007EAAC();
        break;
    case 10:
        func_8004500C(1, 0);
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}
