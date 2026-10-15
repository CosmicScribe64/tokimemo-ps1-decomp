/* NON_MATCHING: T-14700, draft-only type reconstruction; see result.json. */
/* NON_MATCHING: T-14700, draft-only type reconstruction; see result.json. */
s32 func_8010F74C(void) {
    RepairMultiTbl18 spD04;
    RepairExactTbl35 spC78;
    RepairMultiTbl48 spBB8;
    RepairMultiTbl32 spB38;
    RepairMultiTbl35 spAAC;
    RepairMultiTbl34 spA24;
    RepairMultiTbl36 sp994;
    RepairMultiTbl37 sp900;
    RepairMultiTbl36 sp870;
    RepairMultiTbl39 sp7D4;
    RepairMultiTbl32 sp754;
    RepairMultiTbl32 sp6D4;
    RepairMultiTbl103 sp538;
    RepairExactTbl29 sp4C4;
    RepairMultiTbl45 sp410;
    RepairExactTbl41 sp36C;
    RepairMultiTbl37 sp2D8;
    RepairExactTbl47 sp21C;
    RepairMultiTbl61 sp128;
    RepairMultiTbl31 spAC;
    RepairMultiTbl31 sp30;
    void (**sp2C)();
    void (**var_v0)();
    void (**var_v0_2)();

    spD04 = D_801244AC;
    spC78 = D_801244F4;
    spBB8 = D_80124580;
    spB38 = D_80124640;
    spAAC = D_801246C0;
    spA24 = D_8012474C;
    sp994 = D_801247D4;
    sp900 = D_80124864;
    sp870 = D_801248F8;
    sp7D4 = D_80124988;
    sp754 = D_80124A24;
    sp6D4 = D_80124AA4;
    sp538 = D_80124B24;
    sp4C4 = D_80124CC0;
    sp410 = D_80124D34;
    sp36C = D_80124DE8;
    sp2D8 = D_80124E8C;
    sp21C = D_80124F20;
    sp128 = D_80124FDC;
    spAC = D_801250D0;
    sp30 = D_8012514C;
    var_v0 = spD04.f;
    switch (D_801243B0) {
    case 0:
        var_v0 = spD04.f;
        break;
    case 1:
        var_v0 = spC78.f;
        break;
    case 2:
        var_v0 = spBB8.f;
        break;
    case 3:
        var_v0 = spB38.f;
        break;
    case 4:
        var_v0 = spAAC.f;
        break;
    case 5:
        var_v0 = spA24.f;
        break;
    case 6:
        var_v0 = sp994.f;
        break;
    case 7:
        var_v0 = sp900.f;
        break;
    case 8:
        var_v0 = sp870.f;
        break;
    case 9:
        var_v0 = sp7D4.f;
        break;
    case 10:
        var_v0 = sp754.f;
        break;
    case 11:
        var_v0 = sp6D4.f;
        break;
    case 12:
        var_v0 = sp538.f;
        break;
    case 13:
        var_v0 = sp4C4.f;
        break;
    case 14:
        var_v0 = sp410.f;
        break;
    case 15:
        var_v0 = sp36C.f;
        break;
    case 16:
        var_v0 = sp2D8.f;
        break;
    case 17:
        var_v0 = sp21C.f;
        break;
    case 18:
        var_v0 = sp128.f;
        break;
    case 19:
        var_v0 = spAC.f;
        break;
    case 20:
        var_v0 = sp30.f;
        break;
    }
    sp2C = var_v0;
    var_v0[D_800B1AF6](0x80);
    var_v0_2 = sp2C;
    if (D_800EECF0 != 0) {
        func_8011B848();
    }
    if ((var_v0_2 == spC78.f) && ((u8) D_800B1AF6 >= 0xCU) && ((u8) D_800B1AF6 < 0x15U)) {
        sp2C = var_v0_2;
        func_8011C164();
    }
    if (var_v0_2 == spC78.f) {
        if (D_800B1AF6 == 0x15) {
            func_8011C11C();
        }
    }
}
