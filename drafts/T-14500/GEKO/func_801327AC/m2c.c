extern u8 D_800E71DF;

void func_801327AC(void) {
    s32 var_t9;

    D_80144C3C = 0;
    D_80144C40 = 0;
    switch (D_800E71DF) {
    case 0:
        D_80144C30 = D_80144BA4;
        var_t9 = D_80144C04;
        D_80144C34 = D_80144BD4;
block_13:
        D_80144C38 = var_t9;
        break;
    case 1:
        D_80144C30 = D_80144BB4;
        D_80144C34 = D_80144BE4;
        D_80144C38 = D_80144C14;
        break;
    case 2:
        D_80144C30 = D_80144BA8;
        D_80144C34 = D_80144BD8;
        D_80144C38 = D_80144C08;
        break;
    case 3:
        D_80144C30 = D_80144BAC;
        D_80144C34 = D_80144BDC;
        D_80144C38 = D_80144C0C;
        break;
    case 4:
        D_80144C30 = D_80144BC4;
        D_80144C34 = D_80144BF4;
        D_80144C38 = D_80144C24;
        break;
    case 5:
        D_80144C30 = D_80144BB8;
        D_80144C34 = D_80144BE8;
        D_80144C38 = D_80144C18;
        break;
    case 6:
        D_80144C30 = D_80144BBC;
        D_80144C34 = D_80144BEC;
        D_80144C38 = D_80144C1C;
        func_80051A68(6);
        break;
    case 7:
        D_80144C30 = D_80144BB0;
        D_80144C34 = D_80144BE0;
        D_80144C38 = D_80144C10;
        break;
    case 8:
        D_80144C30 = D_80144BA0;
        D_80144C34 = D_80144BD0;
        D_80144C38 = D_80144C00;
        break;
    case 9:
        D_80144C30 = D_80144BC0;
        D_80144C34 = D_80144BF0;
        D_80144C38 = D_80144C20;
        break;
    case 10:
        D_80144C30 = D_80144BCC;
        var_t9 = D_80144C2C;
        D_80144C34 = D_80144BFC;
        goto block_13;
    }
    func_8004284C();
}
