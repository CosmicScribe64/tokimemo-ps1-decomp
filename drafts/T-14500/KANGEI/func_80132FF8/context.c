#include "common.h"
#include "ovl/KANGEI.h"

void func_80132E40(void) {
    D_80139A50 = 0x801C1400;
    D_80139A54 = 0x801C1D6C;
    D_80139A58 = 0x801CA47C;
}

void func_80132E74(void) {
    D_80139A5C = 0x80199CD4;
    D_80139A60 = 0x8019EE30;
    D_80139A64 = 0x801A422C;
    D_80139A68 = 0x801A82AC;
    D_80139A6C = 0x801AB394;
    D_80139A70 = 0x801AF5FC;
    D_80139A74 = 0x801B1C9C;
    D_80139A78 = 0x801B3370;
    D_80139A7C = 0x80199EF4;
    D_80139A80 = 0x8019F078;
    D_80139A84 = 0x801A447C;
    D_80139A88 = 0x801A8408;
    D_80139A8C = 0x801AB4E0;
    D_80139A90 = 0x801AF824;
    D_80139A94 = 0x801B1D10;
    D_80139A98 = 0x801B3478;
    D_80139A9C = 0x8019BC24;
    D_80139AA0 = 0x801A0FD8;
    D_80139AA4 = 0x801A63F8;
    D_80139AA8 = 0x801A96B8;
    D_80139AAC = 0x801AC800;
    D_80139AB0 = 0x801B17AC;
    D_80139AB4 = 0x801B2090;
    D_80139AB8 = 0x801B41F4;
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7200;
extern u8 D_800E7389;
extern u8 D_800E738A;
void func_801335F0(void);
void func_801337B4(void);
void func_80133A14(void);
void func_801335A0(void);
s32 func_801338EC(void);
/* end bulk_m2c declarations */
void func_80132FF8(void) {
    s32 var_at;
    u8 temp_v0;
    u8 var_v0;

    func_80083808();
    temp_v0 = D_800E7389;
    if (((temp_v0 != 2) && (temp_v0 != 5) && (temp_v0 != 9) && (temp_v0 != 0xA) && (temp_v0 != 0xC) && (temp_v0 != 0xF) && (temp_v0 != 0x11) && (temp_v0 != 0x14) && (temp_v0 != 0x16) && (temp_v0 != 0x19) && (temp_v0 != 0x1D) && (temp_v0 != 0x1F)) || (D_800E738A == 0) || ((u8) D_80139AC0 != 0)) {
        if ((temp_v0 != 0x1E) || (var_at = temp_v0 < 0x21U, ((u8) D_80139AC0 != 0))) {
            func_80065B0C(1);
            goto block_17;
        }
    } else {
block_17:
        var_at = temp_v0 < 0x21U;
    }
    if (var_at != 0) {
        switch (temp_v0) {
        case 0:
            func_801335F0();
            break;
        case 1:
            func_801337B4();
            break;
        case 2:
            func_80133C10();
            break;
        case 3:
            vacation_day_init();
            break;
        case 4:
            func_80133A14();
            break;
        case 5:
            func_80138078();
            break;
        case 6:
            func_801335A0();
            break;
        case 7:
            vacation_day_init();
            break;
        case 8:
            func_80133A14();
            break;
        case 9:
            func_80134120();
            break;
        case 10:
            func_80138078();
            break;
        case 11:
            func_801335A0();
            break;
        case 12:
            func_80135678();
            break;
        case 13:
            vacation_day_init();
            break;
        case 14:
            func_80133A14();
            break;
        case 15:
            func_80138078();
            break;
        case 16:
            func_801335A0();
            break;
        case 17:
            func_80135678();
            break;
        case 18:
            vacation_day_init();
            break;
        case 19:
            func_80133A14();
            break;
        case 20:
            func_80138078();
            break;
        case 21:
            func_801335A0();
            break;
        case 22:
            func_80135678();
            break;
        case 23:
            vacation_day_init();
            break;
        case 24:
            func_80133A14();
            break;
        case 25:
            func_80138078();
            break;
        case 26:
            func_801335A0();
            break;
        case 27:
            vacation_day_init();
            break;
        case 28:
            func_80133A14();
            break;
        case 29:
            func_80136D58();
            break;
        case 30:
            func_80138078();
            break;
        case 31:
            func_80136018();
            break;
        case 32:
            func_801338EC();
            break;
        }
    } else {
        func_80046500();
    }
    if ((D_800E7389 == 3) || (D_800E7389 == 7) || (D_800E7389 == 0xD) || (D_800E7389 == 0x12) || (D_800E7389 == 0x17) || (D_800E7389 == 0x1B)) {
        func_80065B0C(1);
    }
    parameter_show();
    check_k_scroll();
    k_disp_inc2();
    func_80066334();
    message_window_show();
    hizuke_show();
    func_80047560();
    if (((D_800E7389 == 2) || (D_800E7389 == 5) || (D_800E7389 == 9) || (D_800E7389 == 0xA) || (D_800E7389 == 0xC) || (D_800E7389 == 0xF) || (D_800E7389 == 0x11) || (D_800E7389 == 0x14) || (D_800E7389 == 0x16) || (D_800E7389 == 0x19) || (D_800E7389 == 0x1D) || (D_800E7389 == 0x1F)) && (D_800E738A != 0) && ((u8) D_80139AC0 == 0)) {
        func_80066C08(2);
        func_80083A10();
    } else if ((D_800E7389 == 0x1E) && ((u8) D_80139AC0 == 0)) {
        func_80066C08(2);
        func_80083A10();
    } else if ((u8) D_80139AC0 == 1) {
        func_800578F4(0);
        if ((D_80122D2C == 0) && (D_80122D04 == 0) && ((D_800E7200 & 0xF000F000) || (D_80122D30 == 1))) {
            D_8011ECD3 |= 0x80;
        }
        if (D_8011ECD3 & 0x80) {
            func_8006BA40();
        }
        func_80066C08(1);
    } else {
        func_800578F4(0);
        func_80066C08(1);
        if ((D_80122D2C == 0) && (D_80122D04 == 0) && ((D_800E7200 & 0xF000F000) || (D_80122D30 == 1))) {
            D_8011ECD3 |= 0x80;
        }
        var_v0 = D_8011ECD3;
        if (D_80122D04 != 0) {
            var_v0 &= 0x7F;
        }
        D_8011ECD3 = var_v0;
        if (var_v0 & 0x80) {
            func_8006BA40();
        }
    }
    func_80047560();
}

void func_801335A0(void) {
    D_800E6280.unk_71C += 1;
    D_80139AC4 = 0;
    D_80139AC8 = 0;
    func_80072338();
    hizuke_init();
    func_80042808();
}

void func_801335F0(void) {
    s16 var_s0;

    D_800B3D60 = 0;
    func_80044890(1, 0xC06F, 0xC065, 0xCC33, 0xCBE3, 0xCBD9);
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    D_80139ABC = 0;
    D_80139AC0 = 0;
    D_80139AC4 = 0;
    func_80133D28();
    D_800E6280.unk_040 -= 1;
    func_800649D4();
    D_800E6280.unk_040 += 1;
    func_8006509C();
    for (var_s0 = 0; var_s0 < 15; var_s0++) {
        func_80065048(var_s0, 0);
    }
    func_80065B0C(1);
    func_800639D8();
    func_800639D8();
    /* FAKE: dead byte test keeps the packed-record address in v0 (T-9580). */
    if (D_800E6280.unk_03A == 0) {}
    ((KangeiStateView *)&D_800E6280)->modes[(u16)D_800E6280.unk_0F4.h >> 12].mode++;
    D_800B593C = 0x80;
    func_80042808();
}

void func_80133734(void) {
    D_800B3D60 = 0;
    func_80044890(1, 0xC06F, 0xC065, 0xCC33, 0xCBE3, 0xCBD9);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_80044750(0x22);
    func_80044750(0x21);
    func_80044750(0x2F);
    func_8004284C();
}

void func_801337B4(void) {
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_80133834(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_801338B4(void) {
    if (D_800E6280.unk_1104.w == 0) {
        func_8004500C(0, 0);
    }
    func_8004284C();
}

s32 func_801338EC(void) {
    s32 pad; /* FAKE: unused local puts rect at the original offset; real source unknown. T-9320 */
    RECT rect;

    if (*(u8 *)&D_800E6280.unk_110D == 0) {
        func_800674B0();
        *(u8 *)&D_800E6280.unk_110D += 1;
    }
    if (*(u8 *)&D_800E6280.unk_110D == 1) {
        if (D_800E6280.unk_1104.u++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            *(u8 *)&D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            *(u8 *)&D_800E6280.unk_110D += 1;
        } else {
            return 0;
        }
    }
    func_800573AC();
    rect.x = 0x140;
    rect.y = 0x100;
    rect.w = 0x140;
    rect.h = 0xF8;
    func_8009C7F8(&rect, 8, 0, 0);
    rect.x = 0x2C0;
    rect.y = 0;
    func_8009C7F8(&rect, 8, 0, 0);
    func_80072338();
    return func_80072B5C(1);
}

void func_80133A14(void) {
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_110A == 0) {
        func_80133A54();
    }
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_80133A54);
