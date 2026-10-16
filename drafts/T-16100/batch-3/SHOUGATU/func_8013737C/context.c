#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80136F90(void) {
    D_80144D90 = 0x801976F8;
    D_80144D94 = 0x80198BF0;
    D_80144D98 = 0x8019A37C;
    D_80144D9C = 0x8019ADCC;
    D_80144DA0 = 0x8019BC80;
    D_80144DA4 = 0x8019C918;
    D_80144DA8 = 0x8019D460;
    D_80144DAC = 0x8019E168;
    D_80144DB0 = 0x8019EB4C;
    D_80144DB4 = 0x80197774;
    D_80144DB8 = 0x80198CF8;
    D_80144DBC = 0x8019A414;
    D_80144DC0 = 0x8019ADF8;
    D_80144DC4 = 0x8019BD34;
    D_80144DC8 = 0x8019C960;
    D_80144DCC = 0x8019D4C4;
    D_80144DD0 = 0x8019E210;
    D_80144DD4 = 0x8019EBA8;
    D_80144DD8 = 0x80197C94;
    D_80144DDC = 0x80199738;
    D_80144DE0 = 0x8019AB30;
    D_80144DE4 = 0x8019B000;
    D_80144DE8 = 0x8019C4B4;
    D_80144DEC = 0x8019CC70;
    D_80144DF0 = 0x8019D910;
    D_80144DF4 = 0x8019E7B4;
    D_80144DF8 = 0x8019EE04;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80136F90", func_80137144);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6608;
extern u32 D_800E7384;
extern u8 D_800E62BF;
extern u8 D_800E64BA;
extern u8 D_800E699C;
extern u8 D_800E71DF;
extern u8 D_800E738D;
/* end bulk_m2c declarations */
s32 func_8013737C(void) {
    s8 var_t3;
    u8 temp_a0;
    u8 temp_t9;
    u8 var_v0;
    u8 *var_v0_2;

    var_v0 = D_800E738D;
    if (var_v0 == 0) {
        func_80044750(0xCF);
        func_800674B0();
        var_v0 = (D_800E738D + 1) & 0xFF;
    }
    D_800E738D = var_v0;
    if (var_v0 == 1) {
        D_800E7384 += 1;
        if ((u32) D_800E7384 >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E738D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E738D += 1;
            goto block_8;
        }
        return 0;
    }
block_8:
    D_800E71DF = D_80144E10;
    if ((D_80144E10 == 2) && ((D_80144E14 == 2) || (D_80144E14 == 3)) && (((u8) D_800E64BA >> 4) == 3)) {
        func_80085B3C(8, (D_80144E10 * 0xA) + 6);
    } else if ((D_80144E10 == 4) && (D_80144E14 == 0) && ((u8) D_800E62BF >= 6U) && ((u8) D_800E62BF < 0xAU)) {
        func_80085B3C(8, (D_80144E10 * 0xA) + 3);
    } else {
        func_80085B3C(8, (D_80144E10 * 0xA) + D_80144E14);
    }
    if (D_80144E10 == 0xA) {
        D_800E6280.unk_1BC[D_800E6280.unk_69C[D_800E699C].unk_01].unk_06 = 0x28;
    } else {
        D_800E6280.unk_1BC[D_800E6280.unk_69C[D_800E699C].unk_01].unk_06 = 0x32;
    }
    temp_a0 = D_800E6280.unk_69C[D_800E699C].unk_01;
    if (temp_a0 != 8) {
        var_v0_2 = (u8 *)&D_800E6280 + (temp_a0 * 0x38);
        var_t3 = D_800E6280.unk_1BC[temp_a0].unk_0C.b[0] | 2;
        goto block_26;
    }
    D_800E6280.unk_1BC[8].unk_0C.b[1] = (((((u32) (D_800E6608 << 0x14) >> 0x1D) + 1) * 2) & 0xE) | (D_800E6280.unk_1BC[8].unk_0C.b[1] & 0xFFF1);
    if (((u32) (D_800E6280.unk_1BC[8].unk_0C.w << 0x14) >> 0x1D) == 4) {
        temp_t9 = D_800E6280.unk_69C[D_800E699C].unk_01;
        var_v0_2 = (u8 *)&D_800E6280 + (temp_t9 * 0x38);
        var_t3 = D_800E6280.unk_1BC[temp_t9].unk_0C.b[0] | 2;
block_26:
        *(s8 *)((unsigned char *)var_v0_2 + (456)) = var_t3;
    }
    func_80044750(0x22);
    func_80044750(0x21);
    func_80044750(0x2F);
    return func_80072B5C(1);
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80136F90", func_80137664);

void func_801377CC(void) {
    func_80044750(0xCF);
    func_800674B0();
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    func_80042808();
}

void func_80137818(void) {
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042908((D_800E6280.unk_1109 - 1) & 0xFF);
    }
}

void func_80137898(void) {
    func_80044750(0xCF);
    func_8004284C();
}

void func_801378C0(void) {
    D_800B3D60 = 0;
    func_80044890(0, 0xBF98, 0xBF79, 0xD989, 0xD949, 0xD941);
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x2E);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80137940(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x23);
        func_80044750(0x24);
        func_80044750(0x2E);
        func_8004284C();
    }
    if (D_800E6280.unk_1104.u++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_801379D8(void) {
    func_8004500C(1, 0x200);
    func_8004284C();
}

void func_80137A04(void) {
    cd_read_sectors(0x10, 0x80197000, 0xAEFD);
    func_80136F90();
    D_80144E10 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;
    func_80042808();
}

void func_80137A5C(void) {
    cd_read_sectors(0x10, 0x80197000, 0xAEFD);
    func_80136F90();
    D_80144E10 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;
    func_8004284C();
}

void func_80137AB4(void) {
    D_800CA134 = &D_80144E08;
    D_800CA138 = &D_80144E0C;
    D_800CA13C = D_80144DFC;
    D_800CA140 = D_80144E00;
    D_800CA144 = D_80144E04;
    func_80082764(D_80122CDC, 1, 0);
}

void func_80137B30(void) {
    D_800CA134 = (u8 *) &D_80144E08;
    D_800CA138 = (u8 *) &D_80144E0C;
    D_800CA13C = D_80144DFC;
    D_800CA140 = D_80144E00;
    D_800CA144 = D_80144E04;
    func_80082764(D_80122CDC, 1, 1);
}
