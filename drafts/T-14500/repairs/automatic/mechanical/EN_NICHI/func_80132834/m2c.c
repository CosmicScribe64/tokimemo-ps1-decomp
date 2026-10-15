? func_8004D088(?, ?, ?, ?);                        /* extern */
? func_8004E350(?, s32, ?);                         /* extern */
? func_800862D0();                                  /* extern */
s32 func_800862F0();                                /* extern */
? func_80086350();                                  /* extern */
? func_80086370();                                  /* extern */
extern s8 D_800E62BA;
extern s32 D_800E7208;
extern u8 D_800E738A;
extern s32 D_800E74BC;
extern s32 D_800E74C0;
extern s32 D_800E753C;
extern s32 D_800E7540;
extern s32 D_80139200;
extern s32 D_80139214;
extern u32 *D_80139218;

void func_80132834(void) {
    void *temp_v0;

    switch (D_800E738A) {                           /* irregular */
    case 0:
        D_800E62BA = 0;
        func_8004E44C(0, D_80139218, D_80139200);
        func_8004E350(0, D_80139214, 0);
        func_8004D088(0, 5, 0x580, 0xF0);
        func_80086350();
        func_8004284C();
        break;
    case 1:
        if (func_800460EC() == 2) {
            func_8004284C();
        }
        break;
    case 2:
        func_80086370();
        if (D_800E7208 & 0x60) {
            func_80044750(0x74);
            func_800862D0();
            func_8004284C();
        }
        func_80085F0C(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 3:
        if (func_800862F0() != 0) {
            D_800E74BC = -1;
            D_800E753C = -1;
            D_800E74C0 = -1;
            D_800E7540 = -1;
            temp_v0 = (2 * 4) + &D_800E6280;
            *(s32 *)((unsigned char *)temp_v0 + (4672)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4800)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4676)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4804)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4680)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4808)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4668)) = -1;
            *(s32 *)((unsigned char *)temp_v0 + (4796)) = -1;
            func_80044750(0x201);
            func_80042808();
        }
        func_80085F0C(0, 0xF, 0, 0xF0, 0x280);
        break;
    default:
        func_80046500();
        break;
    }
}
