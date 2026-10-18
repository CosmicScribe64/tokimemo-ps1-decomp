/* T-24029: typed source checkpoint, not matching; declarations are scratch-only. */
s32 func_801368B8(void) {
    extern s32 D_8012E674;
    extern s32 D_8012E680;
    extern s32 D_800F62F8;
    extern u8 D_800F53DC;
    s32 func_8005AE00();
    s32 func_800BCEA0();
    void func_80137188(void);
    u8 sp64[] = "おみくじを引きます。";
    u8 sp4C[] = "うおりゃあああ〜・・・";
    u8 sp3C[] = "こ、これは！！";

    func_800BCDF0("timer %d\n", D_800F6474);
    switch (D_800F6474) {
    case 0:
        func_8005AE00(-0x68, 0x30, 0, &sp64, 0);
        D_8012E674 = 1;
        ((DisplayActor44 *)D_8012B8C0)[0].unk_2E = -4;
        D_800F6474 += 1;
        return;
    case 2:
        func_8005AE00(-0x68, 0x30, 0, &sp4C, 0);
        D_8012E674 = 1;
        D_801461A0 = 0;
        D_8012B8C0[3].unk_03 |= 0x80;
        D_8012B8C0[3].unk_02 = 1;
        D_8012B8C0[3].unk_16 = 1;
        D_8012B8C0[3].unk_18 = 0;
        D_800F6474 += 1;
        return;
    case 3:
        ((DisplayFixActor44 *)D_8012B8C0)[0].unk_24.w += ((DisplayFixActor44 *)D_80129F40)[96].unk_2C;
        if (((DisplayActor44 *)D_80129F40)[96].unk_26 < -0x6F) {
            D_8012B8C0[3].unk_16 = 0;
            ((DisplayActor44 *)D_80129F40)[96].unk_2E = (s16) -((DisplayActor44 *)D_80129F40)[96].unk_2E;
            ((DisplayActor44 *)D_80129F40)[96].unk_2E = (s16) (((DisplayActor44 *)D_80129F40)[96].unk_2E + 2);
            if (((DisplayActor44 *)D_80129F40)[96].unk_2E >= 0x15) {
                ((DisplayActor44 *)D_80129F40)[96].unk_2E = 0x14;
            }
        } else if (((DisplayActor44 *)D_80129F40)[96].unk_26 >= 0x70) {
            D_8012B8C0[3].unk_16 = 1;
            ((DisplayActor44 *)D_80129F40)[96].unk_2E = (s16) -((DisplayActor44 *)D_80129F40)[96].unk_2E;
            ((DisplayActor44 *)D_80129F40)[96].unk_2E = (s16) (((DisplayActor44 *)D_80129F40)[96].unk_2E - 2);
            if (((DisplayActor44 *)D_80129F40)[96].unk_2E < -0x14) {
                ((DisplayActor44 *)D_80129F40)[96].unk_2E = -0x14;
            }
        }
        if (((DisplayActor44 *)D_80129F40)[96].unk_2E < -8 || ((DisplayActor44 *)D_80129F40)[96].unk_2E > 8) {
            D_8012B8C0[1].unk_08 += 1;
            D_8012B8C0[2].unk_08 += 1;
        }
        if (((DisplayActor44 *)D_80129F40)[96].unk_2E < -12 || ((DisplayActor44 *)D_80129F40)[96].unk_2E > 12) {
            D_8012B8C0[1].unk_08 += 2;
            D_8012B8C0[2].unk_08 += 2;
        }
        if (((DisplayActor44 *)D_80129F40)[96].unk_2E < -16 || ((DisplayActor44 *)D_80129F40)[96].unk_2E > 16) {
            D_8012B8C0[1].unk_08 += 4;
            D_8012B8C0[2].unk_08 += 4;
        }
    case 1:
    case 7:
    case 9:
block_51:
        if (D_8012E674 != 0) {
            if ((D_800F62F8 & 0x400040) && (func_8005B32C() != 0) && (D_800F53DC == 2)) {
                func_8005B06C(1);
                D_8012E674 = 0;
                D_8012E680 = 0;
                ((DisplayActor44 *)D_80129F40)[2].unk_03 &= 0xFF7F;
                D_800F6474 += 1;
                return;
            }
            if ((D_800F62F8 & 0x200020) && (func_8005B32C() != 0)) {
                func_8005B06C(1);
                D_8012E674 = 0;
                D_8012E680 = 0;
                ((DisplayActor44 *)D_80129F40)[2].unk_03 &= 0xFF7F;
                D_800F6474 += 1;
                return;
            }
            if (func_8005B32C() != 0) {
                if ((u32) ((u32) D_800F6468 % 30U) < 0xFU) {
                    ((DisplayActor44 *)D_80129F40)[2].unk_03 &= 0x7F;
                    return;
                }
                ((DisplayActor44 *)D_80129F40)[2].unk_03 |= 0x80;
            }
        } else {
            return;
        }
        break;
    case 4:
        if (((((DisplayActor44 *)D_80129F40)[96].unk_26 >= 0) && (-((DisplayActor44 *)D_80129F40)[96].unk_2E >= ((DisplayActor44 *)D_80129F40)[96].unk_26)) || ((((DisplayActor44 *)D_80129F40)[96].unk_26 <= 0) && (((DisplayActor44 *)D_80129F40)[96].unk_26 >= -((DisplayActor44 *)D_80129F40)[96].unk_2E))) {
            ((DisplayActor44 *)D_80129F40)[96].unk_26 = 0;
            D_801461A0 = 1;
            D_8012B8C0[3].unk_03 &= 0x7F;
            D_800F6474 += 1;
            return;
        }
        ((DisplayFixActor44 *)D_80129F40)[96].unk_24.w = (s32) (((DisplayFixActor44 *)D_80129F40)[96].unk_24.w + ((DisplayFixActor44 *)D_80129F40)[96].unk_2C);
        if (((DisplayActor44 *)D_80129F40)[96].unk_26 < -0x6F) {
            ((DisplayActor44 *)D_80129F40)[96].unk_2E = 0xA;
            }
        if (((DisplayActor44 *)D_80129F40)[96].unk_26 >= 0x70) {
            ((DisplayActor44 *)D_80129F40)[96].unk_2E = -0xA;
        }
        return;
    case 5:
        if (D_800F647D == 0) {
            ((DisplayFixActor44 *)D_80129F40)[96].unk_2C = 0;
            ((DisplayFixActor44 *)D_8012B8C0)[0].unk_30 = 0xFFFC0000;
            D_8012B8C0[0].unk_03 |= 0x10;
        }
        D_8012B8C0[1].unk_02 = 0;
        D_8012B8C0[2].unk_02 = 0;
        if (D_8012B8C0[0].unk_2A < -0xB4) {
            D_8012B8C0[0].unk_2A = -0xB4;
        }
        if (D_800F647D++ == 0x1E) {
            ((DisplayFixActor44 *)D_8012B8C0)[0].unk_30 = 0;
            D_8012B8C0[0].unk_16 = D_800F53DE - 0x5F;
            D_8012B8C0[0].unk_02 = 5;
        }
        if (D_800F647D == 0x4B) {
            D_801461A0 = 0;
            D_8012B8C0[0].unk_03 &= 0x7F;
            D_8012B8C0[3].unk_03 |= 0x80;
            D_8012B8C0[3].unk_02 = 9;
            D_8012B8C0[3].unk_16 = 2;
            D_8012B8C0[3].unk_18 = 0;
            func_80086AB0(0x500);
        }
        if ((u8) D_800F647D < 0x87U) {
            return;
        }
        func_80137188();
        func_8005AE00(-0x68, 0x30, 0, &sp3C, 0);
        D_800F6474 += 1;
        return;
    case 6:
        if ((((u8) D_8012B8C0[0].unk_02 != 1) | 8) != 0) {
            D_8012E674 = 1;
            D_800F6474 += 1;
        }
        return;
    case 8:
        D_8012B8C0[0].unk_03 |= 0x80;
        D_8012B8C0[0].unk_02 = 0x40;
        D_8012B8C0[0].unk_18 = D_80146178 + 0xD;
        ((DisplayActor44 *)D_80129F40)[96].unk_26 = -0xA0;
        D_8012B8C0[0].unk_2A = -0x78;
        ((DisplayFixActor44 *)D_8012B8C0)[0].unk_30 = 0;
        if ((D_80146178 == 3) || (D_80146178 == 4)) {
            func_800BCEA0(&sp4C, "%sだ・・・", (D_80146178 * 5) + D_8014617C);
            D_8012B8C0[3].unk_03 |= 0x80;
            D_8012B8C0[3].unk_02 = 0x40;
            D_8012B8C0[3].unk_18 = 7;
        } else if (D_80146178 != 0) {
            func_800BCEA0(&sp4C, "おっ、　%sだ。", (D_80146178 * 5) + D_8014617C);
            D_8012B8C0[3].unk_18 = 6;
            D_801461A0 = 1;
            D_8012B8C0[3].unk_03 &= 0x7F;
        } else {
            func_800BCEA0(&sp4C, "ラッキー！　%sだ！！", (D_80146178 * 5) + D_8014617C);
            D_8012B8C0[3].unk_18 = 6;
            D_801461A0 = 1;
            D_8012B8C0[3].unk_03 &= 0x7F;
        }
        func_8005AE00(-0x68, 0x30, 0, &sp4C, 0);
        D_8012E674 = 1;
        D_800F6474 += 1;
        return;
    default:
        func_8004DE1C();
        goto block_51;
    }
}
