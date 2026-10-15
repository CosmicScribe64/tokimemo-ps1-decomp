#include "ovl/VALEN.h"

typedef struct {
    void (*f[12])();
} FnTbl12; /* size 0x30 */
extern FnTbl12 D_801346FC;

void func_80133C70(void) {
    D_80134660 = 0x801B00DC;
    D_80134664 = 0x801B02AC;
    D_80134668 = 0x801B03C8;
    D_8013466C = 0x801B04C4;
    D_80134670 = 0x801B0668;
    D_80134674 = 0x801B0808;
    D_80134678 = 0x801B09DC;
    D_8013467C = 0x801B0BE8;
    D_80134680 = 0x801B0D8C;
    D_80134684 = 0x801B0F5C;
    D_80134688 = 0x801B1164;
    D_8013468C = 0x801B12D8;
    D_80134690 = 0x801B1454;
    D_80134694 = 0x801B00F4;
    D_80134698 = 0x801B02C8;
    D_8013469C = 0x801B03D0;
    D_801346A0 = 0x801B04DC;
    D_801346A4 = 0x801B0680;
    D_801346A8 = 0x801B0820;
    D_801346AC = 0x801B09F8;
    D_801346B0 = 0x801B0C00;
    D_801346B4 = 0x801B0DA4;
    D_801346B8 = 0x801B0F78;
    D_801346BC = 0x801B1180;
    D_801346C0 = 0x801B12E0;
    D_801346C4 = 0x801B1470;
    D_801346C8 = 0x801B01A8;
    D_801346CC = 0x801B0398;
    D_801346D0 = 0x801B03F0;
    D_801346D4 = 0x801B0590;
    D_801346D8 = 0x801B0734;
    D_801346DC = 0x801B08D4;
    D_801346E0 = 0x801B0AE4;
    D_801346E4 = 0x801B0CB4;
    D_801346E8 = 0x801B0E58;
    D_801346EC = 0x801B1064;
    D_801346F0 = 0x801B126C;
    D_801346F4 = 0x801B1338;
    D_801346F8 = 0x801B155C;
}

void func_80133EE4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl12 tbl;

    tbl = D_801346FC;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80133F58(void) {
    if (D_800E6280.unk_F5F == 9) {
        func_8004284C();
        return;
    }
    normal_date_girl_in();
}

void func_80133F98(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    func_80133C70();
    func_8004284C();
}

void func_80133FD0(void) {
    D_8013448C = D_80134400;
    D_80134490 = D_80134434;
    D_80134494 = D_80134468;
    D_80134498 = 0xC;
    D_8013449C = 0;
    func_8004284C();
}

/* bulk_m2c declarations (T-11190) */
extern u8 D_800E699E;
extern u8 D_800E71DF;
/* end bulk_m2c declarations */
void func_80134030(void) {
    s32 var_t3;

    func_80133C70();
    D_800E699E = (D_800E699E & 0xFFF7) | 4;
    switch (D_800E71DF) {
    case 0:
        D_8013448C = D_80134664;
        var_t3 = D_801346CC;
        D_80134490 = D_80134698;
block_13:
        D_80134494 = var_t3;
        break;
    case 1:
        D_8013448C = D_80134678;
        D_80134490 = D_801346AC;
        D_80134494 = D_801346E0;
        break;
    case 2:
        D_8013448C = D_8013466C;
        D_80134490 = D_801346A0;
        D_80134494 = D_801346D4;
        break;
    case 3:
        D_8013448C = D_80134670;
        D_80134490 = D_801346A4;
        D_80134494 = D_801346D8;
        break;
    case 4:
        D_8013448C = D_80134688;
        D_80134490 = D_801346BC;
        D_80134494 = D_801346F0;
        break;
    case 5:
        D_8013448C = D_8013467C;
        D_80134490 = D_801346B0;
        D_80134494 = D_801346E4;
        break;
    case 6:
        D_8013448C = D_80134680;
        D_80134490 = D_801346B4;
        D_80134494 = D_801346E8;
        break;
    case 7:
        D_8013448C = D_80134674;
        D_80134490 = D_801346A8;
        D_80134494 = D_801346DC;
        break;
    case 8:
        D_8013448C = D_80134660;
        D_80134490 = D_80134694;
        D_80134494 = D_801346C8;
        break;
    case 9:
        D_8013448C = D_80134684;
        D_80134490 = D_801346B8;
        D_80134494 = D_801346EC;
        break;
    default:
        D_8013448C = D_80134690;
        var_t3 = D_801346F8;
        D_80134490 = D_801346C4;
        goto block_13;
    }
    D_80134498 = 1;
    D_8013449C = 0;
    func_8004284C();
}

void func_801342E8(void) {
    func_80042908(D_800E6280.unk_721);
    func_80042940(D_800E6280.unk_722);
}
