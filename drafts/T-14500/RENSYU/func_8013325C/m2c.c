extern u16 D_800E6374;
extern ? D_800E68EE;
extern u8 D_800E738A;
extern s8 D_801342C0;
extern u8 D_801342C4;
extern u8 D_801342C8;
extern ? D_801342CC;
extern ? D_801342CD;
extern ? D_80134378;

void func_8013325C(void) {
    s32 sp64;
    s16 temp_v0;
    s32 temp_a0;
    s32 temp_s1;
    u32 temp_t1;
    u32 var_s0;
    u8 *temp_a0_2;
    u8 *temp_v1_2;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 temp_a1;
    u8 temp_a2;
    u8 temp_v1;
    u8 var_v1;

    sp40[0].unk0 = (s32) D_80134378.unk0;
    *(s32 *)((unsigned char *)(&sp40[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80134378) + (4));
    *(s32 *)((unsigned char *)(&sp40[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80134378) + (8));
    *(s32 *)((unsigned char *)(&sp40[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80134378) + (12));
    *(s32 *)((unsigned char *)(&sp40[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80134378) + (16));
    *(u16 *)((unsigned char *)(&sp40[0]) + (20)) = (u16) *(u16 *)((unsigned char *)(&D_80134378) + (20));
    switch (D_800E738A) {                           /* irregular */
    case 0:
        temp_t1 = (u16) D_800E6374 >> 0xC;
        temp_v0 = D_800E6280.unk_66C[temp_t1].unk_00 - (&sp40[0])[*(u16 *)((unsigned char *)(&D_800E6280.unk_66C[temp_t1]) + (2)) & 0xF];
        if (temp_v0 < -0x1E) {
            D_801342C4 = 0;
        } else {
            var_v1 = 1;
            if (temp_v0 < -0x14) {
                goto block_20;
            }
            var_v1 = 1;
            if (temp_v0 < -0xA) {
                goto block_20;
            }
            if (temp_v0 < 0) {
                var_v1 = 2;
                goto block_20;
            }
            var_v1 = 5;
            if (temp_v0 >= 0x1E) {
                goto block_20;
            }
            var_v1 = 4;
            if (temp_v0 >= 0x14) {
                goto block_20;
            }
            var_v1 = 4;
            if (temp_v0 >= 0xA) {
                goto block_20;
            }
            var_v1 = 3;
            if (temp_v0 >= 0) {
block_20:
                D_801342C4 = var_v1;
            }
        }
        func_8004284C();
        break;
    case 1:
        var_v0 = &D_801342C8;
        do {
            var_v0 += 1;
            *(s8 *)((unsigned char *)var_v0 + (-1)) = 0;
        } while ((u32) var_v0 < (u32) &D_801342CD);
        var_s0 = 0;
        if (D_801342C4 != 0) {
            var_v0_2 = &D_801342C8;
            do {
                var_s0 += 1;
                var_v0_2 += 1;
                *(s8 *)((unsigned char *)var_v0_2 + (-1)) = 1;
            } while (var_s0 < (u8) D_801342C4);
            var_s0 = 0;
        }
        temp_v1 = D_801342C4;
        temp_a0 = func_800AE0D0() & 0xF;
        sp64 = temp_a0;
        if (temp_v1 >= 3U) {
            temp_a2 = D_801342CC.unk0;
            D_801342CC.unk0 = (u8) D_801342C8;
            *(u8 *)((unsigned char *)(&D_801342CC) + (-4)) = temp_a2;
        }
        if (temp_a0 > 0) {
            do {
                temp_s1 = func_800AE0D0() & 3;
                temp_v1_2 = &(&D_801342C8)[temp_s1];
                temp_a0_2 = &(&D_801342C8)[func_800AE0D0() & 3];
                temp_a1 = *temp_v1_2;
                var_s0 += 1;
                *temp_v1_2 = *temp_a0_2;
                *temp_a0_2 = temp_a1;
            } while (var_s0 != sp64);
        }
        if (((u16) D_800E6374 >> 0xC) == 9) {
            if (temp_v1 >= 3U) {
                D_801342C8 = 1;
            } else {
                D_801342C8 = 0;
            }
        }
        func_8004284C();
        break;
    case 2:
        func_80048F64(0x60);
        D_80120651 = 3;
        D_80120688 = 0x01000000;
        D_80120657 = 0x80;
        D_80120655 = 0x80;
        D_8012066A = 0x1000;
        D_8012066C = 0x1000;
        D_80120656 = 1;
        D_80120693 = ((u32) (*(&D_800E68EE + (((u16) D_800E6374 >> 0xC) * 4)) & 0xF) % 5U) + 0x10;
        D_80120676 = -0x60;
        D_8012067A = 0x20;
        D_8012065C = (u8 *) D_801343D4;
        D_80120660 = (u8 *) D_801343D8;
        D_80120684 = (u8 *) D_801343D0;
        D_80120664 = D_801343DC;
        D_801342C0 = 0;
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}
