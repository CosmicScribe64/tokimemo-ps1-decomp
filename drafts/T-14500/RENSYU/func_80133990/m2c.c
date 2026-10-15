? func_800573F8(?);                                 /* extern */
? func_8007E5A0(u8);                                /* extern */
? func_8007E884();                                  /* extern */
? func_8007E99C(u8);                                /* extern */
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u16 D_800E6374;
extern u8 D_800E652A;
extern u8 D_800E659A;
extern u8 D_800E738A;
extern u8 D_801342C4;
extern ? D_801343A8;

void func_80133990(void) {
    s16 temp_v1;
    s16 temp_v1_2;
    s16 var_v1;
    s16 var_v1_2;
    s16 var_v1_3;
    u32 temp_v0;

    sp30[0].unk0 = (s32) D_801343A8.unk0;
    *(s32 *)((unsigned char *)(&sp30[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801343A8) + (8));
    *(s32 *)((unsigned char *)(&sp30[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801343A8) + (4));
    *(s32 *)((unsigned char *)(&sp30[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_801343A8) + (12));
    *(s32 *)((unsigned char *)(&sp30[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_801343A8) + (16));
    *(u16 *)((unsigned char *)(&sp30[0]) + (24)) = (u16) *(u16 *)((unsigned char *)(&D_801343A8) + (24));
    *(s32 *)((unsigned char *)(&sp30[0]) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_801343A8) + (20));
    func_80083808();
    switch (D_800E738A) {
    case 0:
        func_8004500C(1, 0);
        func_80083474();
        func_800573F8(1);
        break;
    case 1:
        func_8007E884();
        break;
    case 2:
        if ((u8) D_801342C4 >= 3U) {
            temp_v1 = (&sp30[0])[D_801342D0] + 2;
            var_v1 = temp_v1;
            if (D_801342D0 == 4) {
                temp_v0 = (u8) D_800E652A >> 4;
                if (((u16) D_800E6374 >> 0xC) != temp_v0) {
                    if (temp_v0 == 7) {
                        var_v1 = temp_v1 + 1;
                    }
                    if (temp_v0 == 6) {
                        var_v1 += 2;
                    }
                }
            }
            D_800CA148 = var_v1;
            if (D_801342D0 == 6) {
                D_800CA148 = var_v1;
                if (((u16) D_800E6374 >> 0xC) != ((u8) D_800E659A >> 4)) {
                    D_800CA148 = var_v1 + 1;
                }
            }
        } else {
            temp_v1_2 = (&sp30[0])[D_801342D0] + 3;
            var_v1_2 = temp_v1_2;
            if (D_801342D0 == 4) {
                var_v1_2 = temp_v1_2 + 2;
            }
            D_800CA148 = var_v1_2;
            if (D_801342D0 == 6) {
                D_800CA148 = var_v1_2 + 1;
            }
        }
        func_8004284C();
        break;
    case 3:
    case 6:
        func_80085A60();
        break;
    case 4:
        if ((u8) D_801342C4 >= 3U) {
            var_v1_3 = 0x25;
            if (D_801342D0 == 0xC) {
                var_v1_3 = 0x26;
            }
        } else if ((D_800E62BE == 0x61) && (var_v1_3 = 0x28, (D_800E62BF == 0xC))) {

        } else {
            var_v1_3 = 0x27;
        }
        D_800CA148 = var_v1_3;
        func_8004284C();
        break;
    case 5:
        if (D_801342D0 == 0xC) {
            func_8007E5A0(D_801342D0);
        } else {
            func_8004284C();
        }
        break;
    case 7:
        if (D_801342D0 == 0xC) {
            func_8007E934(D_801342D0);
        } else {
            func_8007E99C(D_801342D0);
        }
        break;
    case 8:
        func_80044750(0xC1);
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}
