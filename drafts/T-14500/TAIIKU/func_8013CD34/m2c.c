? func_80086204(s32, s32 *, s32 *, s32 *, s32 *);   /* extern */
extern s32 D_800E7208;
extern s32 D_80149998;
extern s32 D_801499AC;

void func_8013CD34(void) {
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    s32 sp38;
    Taiiku2s32 *var_at;
    s32 temp_v0;
    s32 var_a0;

    if (D_80149990.unk_00 != 0) {
        var_at = &D_80149968;
        D_80149968.unk_00 = 0;
        goto block_17;
    }
    if ((func_8013CEF8() != 0) && (D_800E7208 & 0x20)) {
        if (D_80149978 < 0) {
            D_80149978 = 0;
        }
        temp_v0 = -((s32) ((((s16) *D_80149984 >> 3) - D_80120676) << 0xA) >> 6);
        var_a0 = temp_v0;
        if (((s32) D_80122EC4 % 3) != 0) {
            var_a0 = temp_v0 - 0x200;
        }
        sp44 = 0;
        sp40 = 0x10000;
        func_80086204(var_a0, &sp44, &sp40, &sp3C, &sp38);
        D_80149994.unk0 = sp3C;
        *(s32 *)((unsigned char *)(&D_80149994) + (4)) = sp38;
        D_80149970.unk_00 = 0;
    } else if ((u16) D_80149970.unk_00 >= 0xBU) {
        D_80149970.unk_00 = 0;
        if ((D_801499AC <= 0) && ((u16) D_801499B0->unk0 == 0)) {
            D_80149998 -= 0x4000;
            if (D_80149974.unk_00 > 0) {
                D_80149994.unk0 -= 0x4000;
            } else if (D_80149974.unk_00 < 0) {
                D_80149994.unk0 += 0x4000;
            }
        } else {
            var_at = (Taiiku2s32 *) &D_80149994;
            ((Taiiku2s32 *) &D_80149994)->unk_00 = 0;
block_17:
            var_at->unk_04 = 0;
        }
    }
    if (D_80149990.unk_00 == 0) {
        D_80149968.unk_00 = D_80149994.unk0 + D_801499A8;
        D_80149968.unk_04 = D_80149998 + D_801499AC;
    }
}
