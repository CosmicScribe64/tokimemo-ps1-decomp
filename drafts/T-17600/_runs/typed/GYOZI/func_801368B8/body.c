void func_801368B8(void) {
    s32 sp64[6];
    s32 sp4C[6];
    s32 sp3C[4];
    ? *temp_t2;
    ? *temp_t7;
    ? *temp_t8;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    u8 temp_t2_2;

    temp_t7 = "おみくじを引きます。";
    sp64.unk0 = (s32) temp_t7->unk0;
    *(s32 *)((unsigned char *)(&sp64) + (8)) = (s32) *(s32 *)((unsigned char *)temp_t7 + (8));
    *(s32 *)((unsigned char *)(&sp64) + (4)) = (s32) *(s32 *)((unsigned char *)temp_t7 + (4));
    *(s32 *)((unsigned char *)(&sp64) + (16)) = (s32) *(s32 *)((unsigned char *)temp_t7 + (16));
    temp_t2 = "うおりゃあああ〜・・・";
    *(u8 *)((unsigned char *)(&sp64) + (20)) = (u8) *(u8 *)((unsigned char *)temp_t7 + (20));
    *(s32 *)((unsigned char *)(&sp64) + (12)) = (s32) *(s32 *)((unsigned char *)temp_t7 + (12));
    sp4C.unk0 = (s32) temp_t2->unk0;
    *(s32 *)((unsigned char *)(&sp4C) + (8)) = (s32) *(s32 *)((unsigned char *)temp_t2 + (8));
    *(s32 *)((unsigned char *)(&sp4C) + (4)) = (s32) *(s32 *)((unsigned char *)temp_t2 + (4));
    *(s32 *)((unsigned char *)(&sp4C) + (16)) = (s32) *(s32 *)((unsigned char *)temp_t2 + (16));
    temp_t8 = "こ、これは！！";
    *(s32 *)((unsigned char *)(&sp4C) + (12)) = (s32) *(s32 *)((unsigned char *)temp_t2 + (12));
    *(? *)((unsigned char *)(&sp4C) + (19)) = M2C_LWL(*(? *)((unsigned char *)temp_t2 + (22)));
    sp3C.unk0 = (s32) temp_t8->unk0;
    *(s32 *)((unsigned char *)(&sp3C) + (8)) = (s32) *(s32 *)((unsigned char *)temp_t8 + (8));
    *(s32 *)((unsigned char *)(&sp3C) + (4)) = (s32) *(s32 *)((unsigned char *)temp_t8 + (4));
    *(? *)((unsigned char *)(&sp3C) + (11)) = M2C_LWL(*(? *)((unsigned char *)temp_t8 + (14)));
    func_800BCDF0("timer %d\n", D_800F6474);
    switch (D_800F6474) {
    case 0:
        func_8005AE00(-0x68, 0x30, 0, &sp64, 0);
        D_8012E674 = 1;
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) = -4;
        D_800F6474 += 1;
        return;
    case 2:
        func_8005AE00(-0x68, 0x30, 0, &sp4C, 0);
        D_8012E674 = 1;
        D_801461A0 = 0;
        *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6735)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6735)) | 0x80);
        *(s8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6734)) = 1;
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6754)) = 1;
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6756)) = 0;
        D_800F6474 += 1;
        return;
    case 3:
        *(s32 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6564)) = (s32) (*(s32 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6564)) + *(s32 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6572)));
        if (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6566)) < -0x6F) {
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6754)) = 0;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) = (s16) -*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574));
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) + 2);
            if (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) >= 0x15) {
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) = 0x14;
            }
        } else if (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6566)) >= 0x70) {
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6754)) = 1;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) = (s16) -*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574));
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) - 2);
            if (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) < -0x14) {
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) = -0x14;
            }
        }
        if ((*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) < -8) || (var_at = *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) < -0xC, ((*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) < 9) == 0))) {
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6604)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6604)) + 1);
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6672)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6672)) + 1);
            var_at = *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) < -0xC;
        }
        if ((var_at != 0) || (var_at_2 = *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) < -0x10, ((*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) < 0xD) == 0))) {
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6604)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6604)) + 2);
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6672)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6672)) + 2);
            var_at_2 = *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) < -0x10;
        }
        if ((var_at_2 != 0) || (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) >= 0x11)) {
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6604)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6604)) + 4);
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6672)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6672)) + 4);
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
                *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (139)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (139)) & 0xFF7F);
                D_800F6474 += 1;
                return;
            }
            if ((D_800F62F8 & 0x200020) && (func_8005B32C() != 0)) {
                func_8005B06C(1);
                D_8012E674 = 0;
                D_8012E680 = 0;
                *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (139)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (139)) & 0xFF7F);
                D_800F6474 += 1;
                return;
            }
            if (func_8005B32C() != 0) {
                if ((u32) ((u32) D_800F6468 % 30U) < 0xFU) {
                    *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (139)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (139)) & 0x7F);
                    return;
                }
                *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (139)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (139)) | 0x80);
            }
        } else {
            return;
        }
        break;
    case 4:
        if (((*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6566)) >= 0) && (-*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) >= *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6566)))) || ((*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6566)) <= 0) && (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6566)) >= -*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574))))) {
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6566)) = 0;
            D_801461A0 = 1;
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6735)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6735)) & 0x7F);
            D_800F6474 += 1;
            return;
        }
        *(s32 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6564)) = (s32) (*(s32 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6564)) + *(s32 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6572)));
        var_at_3 = *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6566)) < 0x70;
        if (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6566)) < -0x6F) {
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) = 0xA;
            var_at_3 = *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6566)) < 0x70;
        }
        if (var_at_3 == 0) {
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6574)) = -0xA;
        }
        return;
    case 5:
        if (D_800F647D == 0) {
            *(s32 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6572)) = 0;
            *(s32 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6576)) = 0xFFFC0000;
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6531)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6531)) | 0x10);
        }
        *(s8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6598)) = 0;
        *(s8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6666)) = 0;
        if (*(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6570)) < -0xB4) {
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6570)) = -0xB4;
        }
        temp_t2_2 = (D_800F647D + 1) & 0xFF;
        if ((D_800F647D ^ 0x1E) == 0) {
            *(s32 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6576)) = 0;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6550)) = (s16) (D_800F53DE - 0x5F);
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6530)) = 5U;
        }
        D_800F647D = temp_t2_2;
        if (temp_t2_2 == 0x4B) {
            D_801461A0 = 0;
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6531)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6531)) & 0x7F);
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6735)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6735)) | 0x80);
            *(s8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6734)) = 9;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6754)) = 2;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6756)) = 0;
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
        if (((*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6530)) != 1) | 8) != 0) {
            D_8012E674 = 1;
            D_800F6474 += 1;
        }
        return;
    case 8:
        *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6531)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6531)) | 0x80);
        *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6530)) = 0x40U;
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6552)) = (s16) (D_80146178 + 0xD);
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6566)) = -0xA0;
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6570)) = -0x78;
        *(s32 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6576)) = 0;
        if ((D_80146178 == 3) || (D_80146178 == 4)) {
            func_800BCEA0(&sp4C, "%sだ・・・", (D_80146178 * 5) + D_8014617C);
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6735)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6735)) | 0x80);
            *(s8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6734)) = 0x40;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6756)) = 7;
        } else if (D_80146178 != 0) {
            func_800BCEA0(&sp4C, "おっ、　%sだ。", (D_80146178 * 5) + D_8014617C);
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6756)) = 6;
            D_801461A0 = 1;
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6735)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6735)) & 0x7F);
        } else {
            func_800BCEA0(&sp4C, "ラッキー！　%sだ！！", (D_80146178 * 5) + D_8014617C);
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6756)) = 6;
            D_801461A0 = 1;
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6735)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6735)) & 0x7F);
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
