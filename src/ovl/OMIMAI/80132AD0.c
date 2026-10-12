#include "ovl/OMIMAI.h"

void func_80132AD0(void) {
    D_80134B60[0] = 0x801B00DC;
    D_80134B60[1] = 0x801B02AC;
    D_80134B60[2] = 0x801B03C8;
    D_80134B60[3] = 0x801B04C4;
    D_80134B60[4] = 0x801B0668;
    D_80134B60[5] = 0x801B0808;
    D_80134B60[6] = 0x801B09DC;
    D_80134B60[7] = 0x801B0BE8;
    D_80134B60[8] = 0x801B0D8C;
    D_80134B60[9] = 0x801B0F5C;
    D_80134B60[10] = 0x801B1164;
    D_80134B60[11] = 0x801B12D8;
    D_80134B60[12] = 0x801B1454;
    D_80134B94[0] = 0x801B00F4;
    D_80134B94[1] = 0x801B02C8;
    D_80134B94[2] = 0x801B03D0;
    D_80134B94[3] = 0x801B04DC;
    D_80134B94[4] = 0x801B0680;
    D_80134B94[5] = 0x801B0820;
    D_80134B94[6] = 0x801B09F8;
    D_80134B94[7] = 0x801B0C00;
    D_80134B94[8] = 0x801B0DA4;
    D_80134B94[9] = 0x801B0F78;
    D_80134B94[10] = 0x801B1180;
    D_80134B94[11] = 0x801B12E0;
    D_80134B94[12] = 0x801B1470;
    D_80134BC8[0] = 0x801B01A8;
    D_80134BC8[1] = 0x801B0398;
    D_80134BC8[2] = 0x801B03F0;
    D_80134BC8[3] = 0x801B0590;
    D_80134BC8[4] = 0x801B0734;
    D_80134BC8[5] = 0x801B08D4;
    D_80134BC8[6] = 0x801B0AE4;
    D_80134BC8[7] = 0x801B0CB4;
    D_80134BC8[8] = 0x801B0E58;
    D_80134BC8[9] = 0x801B1064;
    D_80134BC8[10] = 0x801B126C;
    D_80134BC8[11] = 0x801B1338;
    D_80134BC8[12] = 0x801B155C;
}

typedef struct {
    void (*f[12])();
} FnTbl12; /* size 0x30 */
extern FnTbl12 D_80134BFC;

void func_80132D44(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl12 tbl;

    tbl = D_80134BFC;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    func_80066C08(2);
}

void func_80132DC0(void) {
    func_80046318(3, 0x801B0000, 0xAF43);
    func_80132AD0();
    func_8004284C();
}

void func_80132DF8(void) {
    func_80085B3C(0xD, 0x25);
    D_800E6280.unk_71E |= 4;
    D_800CA160 = D_80134A8C;
    D_800CA164 = D_80134ABC;
    D_800CA168 = D_80134AEC;
    D_800CA148 = 2;
    D_800CA14C = 0;
    func_8004284C();
}

void func_80132E78(void) {
    func_80132EB0();
    D_800CA148 = 1;
    D_800CA14C = 0;
    func_8004284C();
}

void func_80132EB0(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_800CA160 = D_80134B60[1];
        D_800CA164 = D_80134B94[1];
        D_800CA168 = D_80134BC8[1];
        return;
    case 1:
        D_800CA160 = D_80134B60[6];
        D_800CA164 = D_80134B94[6];
        D_800CA168 = D_80134BC8[6];
        return;
    case 2:
        D_800CA160 = D_80134B60[3];
        D_800CA164 = D_80134B94[3];
        D_800CA168 = D_80134BC8[3];
        return;
    case 3:
        D_800CA160 = D_80134B60[4];
        D_800CA164 = D_80134B94[4];
        D_800CA168 = D_80134BC8[4];
        return;
    case 4:
        D_800CA160 = D_80134B60[10];
        D_800CA164 = D_80134B94[10];
        D_800CA168 = D_80134BC8[10];
        return;
    case 5:
        D_800CA160 = D_80134B60[7];
        D_800CA164 = D_80134B94[7];
        D_800CA168 = D_80134BC8[7];
        return;
    case 6:
        D_800CA160 = D_80134B60[8];
        D_800CA164 = D_80134B94[8];
        D_800CA168 = D_80134BC8[8];
        return;
    case 7:
        D_800CA160 = D_80134B60[5];
        D_800CA164 = D_80134B94[5];
        D_800CA168 = D_80134BC8[5];
        return;
    case 8:
        D_800CA160 = D_80134B60[0];
        D_800CA164 = D_80134B94[0];
        D_800CA168 = D_80134BC8[0];
        return;
    case 9:
        D_800CA160 = D_80134B60[9];
        D_800CA164 = D_80134B94[9];
        D_800CA168 = D_80134BC8[9];
        return;
    case 10:
        D_800CA160 = D_80134B60[12];
        D_800CA164 = D_80134B94[12];
        D_800CA168 = D_80134BC8[12];
    }
}

void func_80133120(void) {
    func_80083808();
    if (D_800E6280.unk_1109 == 0) {
        func_80133424();
    } else {
        func_80046500();
    }
    check_k_scroll();
    k_disp_inc2();
    func_80066C08(2);
    message_window_show();
    func_80066334();
    hizuke_show();
    func_80083A10();
}
