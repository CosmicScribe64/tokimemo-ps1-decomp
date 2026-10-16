? func_800573F8(?);                                 /* extern */
extern s8 D_800E62B6;
extern s8 D_800E62B7;
extern s8 D_800E62B8;
extern u8 D_800E62BA;
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E66EB;
extern u8 D_800E67C8;
extern u8 D_800E6839;
extern s8 D_800E71DE;
extern u8 D_800E738D;
extern u8 D_800E7D30;

void func_8014068C(void) {
    u8 sp3B;
    u8 sp3A;
    ? sp39;
    s32 sp2C;
    s32 var_v1;
    s32 var_v1_2;
    void *temp_v0;
    void *temp_v0_2;
    void *temp_v0_3;
    void *temp_v0_4;
    void *temp_v0_5;

    switch (D_800E738D) {
    case 0:
        func_800573AC();
        func_800573F8(2);
        func_80057640(0xF0);
        func_8006BD6C(0);
        func_80041584();
        D_800E71DE = func_8006C988((s32) &sp3A, (s32) &sp39, 1);
        sp3B = sp3A;
        func_800AE0B0("girl %x \n", sp3A);
        func_800AE0B0("i %x \n", sp3A);
        if (sp3B >= 0xDU) {
            if ((D_800E62BE != 0x62) || (D_800E62BF != 3) || (D_800E62C0 != 1)) {
                if ((D_800E62BE != 0x5F) || (D_800E62BF != 4) || (D_800E62C0 != 4)) {
                    func_80044750(5);
                    func_80044750(0xC1);
                }
            }
            D_800E738D = 4;
        } else if (sp3B == 0xB) {
            if (D_800E6839 != 0) {
                temp_v0 = (sp3B * 0x10) + 8 + D_800B3B9C;
                func_80046290(temp_v0->unk0, *(s32 *)((unsigned char *)temp_v0 + (4)), (s32) sp3B);
                func_80044750(0x300);
                D_800E738D += 1;
            } else {
                temp_v0_2 = (sp3B * 0x10) + D_800B3B9C;
                func_80046290(temp_v0_2->unk0, *(s32 *)((unsigned char *)temp_v0_2 + (4)), (s32) sp3B);
                func_80044750(0x300);
                D_800E738D += 1;
            }
        } else if (sp3B == 0xC) {
            if ((D_800E7D30 != 0) || (D_800E67C8 != 0)) {
                if ((u8) D_800E66EB >= 0x29U) {
                    var_v1 = 0;
                } else {
                    var_v1 = 1;
                }
                temp_v0_3 = (sp3B * 0x10) + (var_v1 * 8) + D_800B3B9C;
                func_80046290(temp_v0_3->unk0, *(s32 *)((unsigned char *)temp_v0_3 + (4)), (s32) sp3B);
                func_800AE0B0("伊集院A");
                func_80044750(0x300);
                D_800E738D += 1;
            } else if ((u8) D_800E66EB >= 0x29U) {
                temp_v0_4 = (sp3B * 0x10) + 8 + D_800B3B9C;
                func_80046290(temp_v0_4->unk0, *(s32 *)((unsigned char *)temp_v0_4 + (4)), (s32) sp3B);
                func_80044750(0x300);
                func_800AE0B0("伊集院B");
                D_800E738D += 1;
            } else {
                func_800AE0B0("伊集院C");
                D_800E738D = 4;
            }
        } else {
            sp2C = (s32) sp3B;
            if (!(func_80051A68(sp3B) & 0x7F)) {
                var_v1_2 = 1;
            } else {
                var_v1_2 = 0;
            }
            temp_v0_5 = (sp3B * 0x10) + (var_v1_2 * 8) + D_800B3B9C;
            func_80046290(temp_v0_5->unk0, *(s32 *)((unsigned char *)temp_v0_5 + (4)), sp2C);
            func_80044750(0x300);
            D_800E738D += 1;
        }
        break;
    case 1:
        D_800E62BA = 0x80;
        func_80057390(0x80);
        func_80041584();
        if (func_800460EC() & 4) {
            D_800E738D += 1;
        }
        break;
    case 2:
        if ((func_800460EC() & 2) && ((D_800E738D += 1, (D_800E62BE != 0x62)) || (D_800E62BF != 3) || (D_800E62C0 != 1)) && ((D_800E62BE != 0x5F) || (D_800E62BF != 4) || (D_800E62C0 != 4))) {
            func_80044750(5);
            func_80044750(0xC1);
        }
        break;
    case 3:
        D_800E738D += 1;
        break;
    case 4:
        if ((u8) D_800E62BA < 4U) {
            func_800AE0B0("A\n");
            D_800E62B7 = 0;
            D_800E62B6 = 0;
            D_800E62B8 = 0;
            func_8004284C();
        }
        break;
    }
    func_80140B20();
    func_800578F4(2);
    D_8011ECA4 = 1;
}
