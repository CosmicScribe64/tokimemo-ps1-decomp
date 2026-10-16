#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80134890(void) {
    D_80143DB0 = 0x801DABE0;
    D_80143DB4 = 0x801DB684;
    D_80143DB8 = 0x801DABE8;
    D_80143DBC = 0x801DB6A0;
    D_80143DC0 = 0x801DAC48;
    D_80143DC4 = 0x801DB6CC;
    D_80143DC8 = *(s16 *)0x801DB6E0;
    D_80143DCC = *(s16 *)0x801DB6E4;
    D_80143DD0 = 0x801C0000;
    D_80143DD4 = 0x801C2000;
}

void func_80134930(void) {
    u16 idx; /* FAKE: u16 and the second call argument put idx in $a1 as the original does (permuter); real prototype unknown. T-8080 */
    FnTbl31 tbl;

    tbl = D_80143DE0;
    if (D_80143B20 != 0) {
        idx = D_800E6280.unk_110A;
        if ((void (*)())normal_date_girl_in == tbl.f[idx]) {
            tbl.f[idx] = func_8004284C;
        }
    }
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x40, idx);
    if (D_80143DDC != 0) {
        func_80085678(-0xA0, -0x78, 0x140, 0x64, 0xD, 0xE0FF, 0xFFFF);
        func_80085678(-0xA0, -0x14, 0x140, 0x3C, 0xD, 0x6060, 0xE0FF);
        func_8004955C(0xD);
    }
}

/* K&R return shape: failure returns zero; scene advance falls through. T-10030. */
s32 func_80134A54(void) {
    func_80134890();
    if (((s32 *)D_80143DB4)[0] != 0x801CC000 ||
        ((s32 *)D_80143DB4)[1] != 0x801CE000 ||
        ((s32 *)D_80143DB4)[2] != 0x801D0000 ||
        ((s32 *)D_80143DB4)[3] != 0x801D2000 ||
        ((s32 *)D_80143DB4)[4] != 0x801D4000 ||
        ((s32 *)D_80143DB4)[5] != 0x801D6000 ||
        ((s32 *)D_80143DB4)[6] != 0x801D8000 ||
        ((s32 *)D_80143DB0)[0] != 0x801C4000 ||
        ((s32 *)D_80143DB0)[1] != 0x801C8000 ||
        ((s32 *)D_80143DC4)[0] != 0x801DB634 ||
        ((s32 *)D_80143DC4)[1] != 0x801DB644 ||
        ((s32 *)D_80143DC4)[2] != 0x801DB654 ||
        ((s32 *)D_80143DC0)[0] != 0x801DAB84 ||
        ((s32 *)D_80143DC0)[1] != 0x801DAB8C ||
        ((s32 *)D_80143DC0)[2] != 0x801DABA0 ||
        ((s32 *)D_80143DC0)[3] != 0x801DABB4 ||
        ((s32 *)D_80143DC0)[4] != 0x801DABC0 ||
        ((s32 *)D_80143DC0)[5] != 0x801DABCC) {
        D_800E6280.unk_110A -= 2;
        return 0;
    }
    if (file_checksum_ok(0x37, 0x801C0000) == 0) {
        D_800E6280.unk_110A -= 2;
        return 0;
    }
    func_8004284C();
}

void func_80134C34(void) {
    if ((D_80143B20 == 0) && (D_800E6280.unk_75E[D_800E6280.unk_F5E - 1].unk_02 != 5)) {
        if (D_80143B24 != 0) {
            func_80085B3C(5, 5);
        } else {
            func_80085B3C(5, 2);
        }
    }
    bg_read_sub2(0x4200);
    func_8004284C();
}

void func_80134CB4(void) {
    s16 var_s0;
    s32 temp_lo;
    DisplayActor44 *temp_v0;

    D_80143DDC = 0;
    func_80134890();
    load_palette(D_80143DD0, 0x11, 1, 2, 0);
    var_s0 = 0;
    do {
        func_80048F64(var_s0 + 0x60);
        temp_lo = var_s0 * 0x44;
        var_s0 += 1;
        temp_v0 = &((DisplayActor44 *)D_8011ECD0)[temp_lo];
        temp_v0[96].unk_01 = 0xA;
        temp_v0[96].unk_02 = 0;
        temp_v0[96].unk_38 = 0x01000000;
        temp_v0[96].unk_03 = 4;
        *(s32 *)&temp_v0[96].unk_0C = (s32) D_80143DB8;
        *(s32 *)&temp_v0[96].unk_10 = (s32) D_80143DC0;
        *(s32 *)&temp_v0[96].unk_34 = (s32) D_80143DB0;
        temp_v0[96].unk_16 = 0;
        temp_v0[96].unk_18 = 0;
        temp_v0[96].unk_06 = 1;
        temp_v0[96].unk_43 = 0x11;
        temp_v0[96].unk_26 = -0xA0;
        temp_v0[96].unk_2A = -0x78;
        temp_v0[96].unk_07 = 0x80;
        temp_v0[96].unk_05 = 0;
        temp_v0[96].unk_04 = 0;
        temp_v0[96].unk_14 = (s16) D_80143DC8;
    } while (var_s0 < 3);
    D_80120676 = 0;
    D_801206AA = 3;
    D_801206EE = 4;
    load_palette(D_80143DD4, 0x12, 0, 0, 2);
    func_80048F64(0x63);
    D_8012071D = 0xD;
    D_8012071E = 0;
    D_80120754 = 0;
    D_8012071F = 4;
    D_80120728 = D_80143DBC;
    D_8012072C = D_80143DC4;
    D_80120750 = D_80143DB4;
    D_80120730 = D_80143DCC;
    D_80120732 = 0;
    D_80120734 = 0;
    D_80120722 = 1;
    D_8012075F = 0;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
    D_80120723 = 0x80;
    D_80120721 = 0x81;
    D_80120720 = 0;
    func_8004284C();
}

void func_80134F10(void) {
    func_80044750(0x201);
    func_8004284C();
}

void func_80134F38(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_80134F60(void) {
    if ((D_800E6280.unk_F5F == 2) && (D_80143B20 == 0)) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80134FAC(void) {
    if ((D_80143DD8 == 3) || (D_80143DD8 == 4)) {
        D_80143B00 += 2;
    } else if (D_80143DD8 != 0) {
        D_80143B00 += 1;
    }
    func_8004284C();
}

void func_80135014(void) {
    if (D_80143DD8 == 0) {
        D_80143B00 += 2;
    } else if ((D_80143DD8 != 3) && (D_80143DD8 != 4)) {
        D_80143B00 += 1;
    }
    func_8004284C();
}

void func_8013507C(void) {
    D_80120650[3] |= 0x80;
    D_80120650[0x46] = 1;
    D_80120650[0x47] |= 0x80;
    D_80120650[0x8A] = 1;
    D_80120650[0x8B] |= 0x80;
    D_80143DDC = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134890", func_801350F8);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134890", func_801359FC);

void func_80135B10(void) {
    func_80048F64(0x60);
    func_80048F64(0x61);
    func_80048F64(0x62);
    func_80048F64(0x63);
    D_80143DDC = 0;
    func_8004284C();
}

void func_80135B54(void) {
    cd_read_sectors(0x37, 0x801C0000, 0xBD5E);
    func_8004284C();
}
