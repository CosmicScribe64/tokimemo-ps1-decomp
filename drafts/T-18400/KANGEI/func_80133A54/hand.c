void func_80133A54(void) {
    u8 delta;

    if (!(((KangeiSprite44 *)D_80120650)->unk_02 & 1)) {
        if (((KangeiSprite44 *)D_80120650)->unk_08 == 0) {
            switch (((KangeiSprite44 *)D_80120650)->unk_16 & 1) {
            case 0:
                func_80044750(0x601);
                break;
            case 1:
                func_80044750(0x602);
                break;
            }
            delta = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;
            D_800E6280.unk_66C[D_800E6280.unk_0F4.h >> 12].unk_00 += 2;
            if ((D_800E6280.unk_0F4.h >> 12) < 2) {
                *(s32 *)&D_800E6280.unk_100 += 0x18000;
            } else if ((D_800E6280.unk_0F4.h >> 12) < 4) {
                *(s32 *)&D_800E6280.unk_104 += 0x18000;
            } else if ((D_800E6280.unk_0F4.h >> 12) < 6) {
                *(s32 *)&D_800E6280.unk_108 += 0x18000;
            } else {
                *(s32 *)&D_800E6280.unk_10C += 0x18000;
                *(s32 *)&D_800E6280.unk_118 += 0x18000;
            }
            parameter_up_down(delta, 0, D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02, delta);
            parameter_show_init();
        }
        if (!(((KangeiSprite44 *)D_80120650)->unk_08++ < 5)) {
            func_80042808();
        }
    }
}
