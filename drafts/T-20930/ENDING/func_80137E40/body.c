void func_80137E40(void) {
    s32 off;

    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 4;
            func_801396A4(0, 0xFF);
            D_80122D0C = 0;
        }
    } else {
        off = D_800E6280.unk_1BC[1].unk_0C.f.b14 == 0;
        off = (off ^ 1) * 2;
        if (D_8013C35C == off + 0x13) {
            if (D_8013C360 == 1) {
                *(s16 *)&D_8011ECD0[0x852] = 5;
                func_801396A4(1, 1);
                D_80122D0C = 1;
            }
        } else if (D_8013C35C == off + 0x16) {
            if (D_8013C360 == 1) {
                func_801396A4(2, 0);
                D_80122D0C = 0;
            }
        } else if (D_8013C35C == off + 0x17 && D_8013C360 == 1) {
            func_801396A4(3, 5);
            D_80122D0C = 1;
        }
    }
    D_8013C360 += 1;
}
