#include "common.h"
#include "ovl/DATE.h"

/*
 * Matching notes (T-2010):
 * - Casts such as (u16) D_800CA150 or (u8) D_800E6280.unk_03F pick the load width (lhu/lbu) the
 *   original uses where the shared header declares another type.
 * - *(s16 *)0x801CE12C reads data of another overlay by absolute address; a declared
 *   extern would change the register allocation of the load.
 */

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80132000);

void func_80132288(void) {
    D_8015B5C0 = 0x801F2820;
    D_8015B5C4 = 0x801F2830;
    D_8015B5C8 = 0x801F28B4;
    D_8015B5CC = *(s16 *)0x801F28CC;
    D_8015B5D0 = 0x801B0000;
    D_8015B5D4 = 0x801CA000;
    D_8015B5D8 = 0x801E2200;
    D_8015B5DC = 0x801B2000;
    D_8015B5E0 = 0x801B6000;
    D_8015B5E4 = 0x801BA000;
    D_8015B5E8 = 0x801BE000;
    D_8015B5EC = 0x801C2000;
    D_8015B5F0 = 0x801C6000;
    D_8015B5F4 = 0x801CA200;
    D_8015B5F8 = 0x801CE200;
    D_8015B5FC = 0x801D2200;
    D_8015B600 = 0x801D6200;
    D_8015B604 = 0x801DA200;
    D_8015B608 = 0x801DE200;
}

void func_801323B8(void) {
    D_8015B60C = 0x801F0638;
    D_8015B610 = 0x801F0C34;
    D_8015B614 = 0x801F063C;
    D_8015B618 = 0x801F0C3C;
    D_8015B61C = 0x801F071C;
    D_8015B620 = 0x801F0CEC;
    D_8015B624 = *(s16 *)0x801F0D14;
    D_8015B628 = *(s16 *)0x801F0D18;
    D_8015B62C = 0x801E0000;
    D_8015B630 = 0x801E2000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80132458);

void func_801324A8(void) {
    D_8015B648 = 0x801B0090;
    D_8015B64C = 0x801B00A4;
    D_8015B650 = 0x801B0170;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801324DC);

void func_801326B0(s16 arg0, s16 arg1, u8 arg2) {
    D_80122CD0 = arg0;
    if (arg1 != 0) {
        D_800E6280.unk_56C[arg2] = 1;
    }
    D_800CA368 = arg2;
    D_800CA364 = 1;
    D_80122D44 |= 2;
    func_80042878(0x5E);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013272C);

void func_80133DB4(void) {
    k_reset(0);
    func_8006612C(D_800CA19C);
    func_8013272C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80133DE8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80133EA0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013426C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801344C8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801347B8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801349A0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80134C64);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013518C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801353F8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80135620);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80135804);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80135A34);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80135B4C);

void func_80135BCC(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80044750(0x202);
        D_800E6280.unk_110D += 1;
    }
    normal_date_move_place();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80135C14);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80135E7C);

void func_801361C0(void) {
    if (D_800E6280.unk_110D == 0) {
        if (D_800CA2F8 == 0xE) {
            func_80044750(0x500);
        }
        D_800E6280.unk_110D = 1;
    }
    func_8007C8A4();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80136214);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80136340);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801366A4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80136870);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80136A74);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80136B70);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80136D3C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80136F2C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80136FF0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80137154);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801378BC);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80137920);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80137984);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801379E8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80137A4C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80137B14);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80137B78);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80137C2C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80137C90);

void func_80137CF4(void) {
    bg_read_sub2(0x4335);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80137D1C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80137DF4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80137FAC);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013808C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801380E4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801381B4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80138220);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80138314);

void func_801383BC(void) {
    if (D_800E6280.unk_1104.w++ == 0) {
        D_800E6280.unk_F5F = 0xE;
    } else {
        D_800E6280.unk_F5F = D_800E6280.unk_75D;
    }
    func_80085A60();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80138418);

void func_80138440(void) {
    func_80044750(0x500);
    bg_read_sub2(0x43F0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80138470);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801384D4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801385D8);

s32 func_8013867C(void) {
    s32 temp_v0;

    temp_v0 = D_800E6280.unk_1104.w + 1;
    D_800E6280.unk_1104.w = temp_v0;
    if ((u32) temp_v0 < 0x20U) {
        return 0;
    }
    func_8007ED84(0x4466);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801386C8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013872C);

void func_80138790(void) {
    bg_read_sub2(0x45A3);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801387B8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013882C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80138890);

void func_801388F4(void) {
    func_80046318(0x86, 0x801B0000, 0xBC35);
    func_80044750(0x200);
    func_80132288();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80138934);

void func_80139000(void) {
    if (D_8015B670 == 0) {
        D_800CA148 += 1;
        func_8004284C();
        func_8004284C();
        D_80122CDC = 0;
    }
    func_8004284C();
}

s32 func_80139054(void) {
    if ((D_800E6280.unk_F5F == 0) && (D_800CA148 == 0xA) && (D_80122CDC == 0)) {
        func_8004284C();
        return 0;
    }
    if ((D_800E6280.unk_F5F == 1) || (D_800E6280.unk_F5F == 4) || (D_800E6280.unk_F5F == 9)) {
        if ((D_800CA148 == 6) && (D_80122CDC == 0)) {
            func_8004284C();
            return 0;
        }
    }
    if ((D_800E6280.unk_F5F == 0xA) && (D_800CA148 == 6) && (D_80122CDC == 0)) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
        return 0;
    }
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
}

void func_80139154(void) {
    u8 temp_v0;

    temp_v0 = func_80051A68(D_800E6280.unk_F5F);
    if ((D_80122CDC == 0) && ((D_800E6280.unk_F5F == 0) || (D_800E6280.unk_F5F == 7)) && (D_800CA2FC == 0)) {
        D_800CA148 = D_800CA148 + ((temp_v0 & 0x7F) >= 2U) + ((temp_v0 & 0x7F) >= 3U);
    } else {
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

void func_80139210(void) {
    u32 t;

    t = (u8) func_80051A68(D_800E6280.unk_F5F) & 0x7F;
    D_800CA148 = ((D_800CA148 - (t >= 2U)) - (t >= 3U)) + 2;
    func_8004284C();
}

void func_8013926C(void) {
    func_8004284C();
}

void func_8013928C(void) {
    if (D_800E6280.unk_104.unk_02 < 0x78) {
        D_800CA148 += 2;
    }
    func_8004284C();
}

void func_801392D0(void) {
    u32 t;

    t = (u8) func_80051A68(D_800E6280.unk_F5F) & 0x7F;
    if (D_800CA2FC == 5) {
        D_800CA148 = D_800CA148 + (t >= 2U) + (t >= 3U);
        func_8013C190();
        D_80122CDC = 0;
        return;
    }
    func_8004284C();
}

void func_8013934C(void) {
    func_80083440((&D_800CA21C)[D_80122CDC]);
    func_8004284C();
}

void func_8013938C(void) {
    if (D_800CA2FC == 5) {
        func_8004284C();
        D_800E6280.unk_110A += 9;
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801393DC);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801395B4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80139664);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80139834);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80139944);

void func_80139A64(void) {
    if (D_8015B674 != 0) {
        D_800CA148 += 0x10;
    }
    if (D_8015B678 != 0) {
        D_800CA148 += 0x10;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80139AC8);

void func_80139CB0(void) {
    if (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_14[0x1A] == 0) {
        D_8015BF34 += 1;
    }
    func_8004284C();
}

void func_80139D08(void) {
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_14[0x1A] += 1;
    func_8004284C();
    D_800E6280.unk_110A += 7;
}

void func_80139D60(void) {
    D_800CA148 += D_800CA2FC * 2;
    func_8004284C();
}

void func_80139D9C(void) {
    func_800AE0F0(D_800CA1DC, "遊園地内");
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80139DD0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80139F3C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013A0C0);

void func_8013A234(void) {
    func_80042940(0x32);
}

void func_8013A254(void) {
    s32 temp_t6;
    u8 *temp_v0;
    u8 *temp_v1;

    temp_t6 = D_800E6280.unk_F5F * 3;
    temp_v0 = temp_t6 + D_8015B540;
    D_800CA22C = temp_v0[0];
    D_800CA22E = temp_v0[1];
    D_800CA230 = temp_v0[2];
    temp_v1 = temp_t6 + D_8015B544;
    D_800CA21C = temp_v1[0];
    D_800CA21E = temp_v1[1];
    D_800CA220 = temp_v1[2];
    func_80136F2C();
}

void func_8013A2E0(void) {
    func_80048EB8(0);
    func_8006BD6C(0);
    func_8004E9F4(0);
    D_800E6280.unk_720 = D_800E6280.unk_1108;
    D_800E6280.unk_721 = D_800E6280.unk_1109;
    D_800E6280.unk_722 = 0x3A;
    func_80042878(0x82);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013A344);

void func_8013A424(void) {
    if (D_800E6280.unk_03E == 0x5F) {
        func_80042940(0xC);
        return;
    }
    func_80042940(0x17);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013A464);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013A62C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013A804);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013A898);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013A94C);

s32 func_8013AE04(void) {
    if ((D_8015B654 != 0) || (D_8015BF4C == 1)) {
        func_8004284C();
        func_8004284C();
        return 0;
    }
    return func_8013AE5C();
}

s32 func_8013AE5C(void) {
    u8 x;
    u32 t;

    D_800CA134 = (u8 *) &D_8015BF04;
    D_800CA138 = (u8 *) &D_8015BF08;
    D_800CA13C = D_8015BE1C;
    D_800CA140 = D_8015BE88;
    D_800CA144 = D_8015BEF4;
    x = func_80051A68(D_800E6280.unk_F5F);
    t = x & 0x7F;
    /* 2U: with two plain 2 IDO shares one constant register, the original does not (T-4020) */
    if ((D_8015B654 == 2U) && (D_8015BF08 == 2) && (t >= 2U)) {
        func_80083440(3);
    }
    return func_80082764(D_80122CDC, 1, 0);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013AF1C);

void func_8013B174(void) {
    s16 sp2E;

    D_800CA134 = (u8 *) &sp2E;
    D_800CA138 = (u8 *) &(&D_800CA22C)[D_80122CDC];
    D_800CA13C = D_8015BD98;
    D_800CA140 = D_8015BDA8;
    sp2E = 0;
    D_800CA144 = D_8015BDB8;
    if (func_80082764(D_80122CDC, 0, 0) != 0) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013B208);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013B488);

s32 func_8013B7B0(void) {
    u8 x;
    u32 t;

    x = func_80051A68(D_800E6280.unk_F5F);
    if (D_800E6280.unk_110D++ == 0) {
        t = x & 0x7F;
        D_8015BF14 = (t >= 2U) + (t >= 3U) + (t >= 4U);
    }
    D_800CA134 = (u8 *) &D_8015BF14;
    D_800CA138 = (u8 *) &D_8015BF18;
    D_800CA13C = D_8015BE28;
    D_800CA140 = D_8015BE94;
    D_800CA144 = D_8015BF00;
    return func_80082764(D_80122CDC, 1, 0);
}

void func_8013B884(void) {
    D_800CA134 = (u8 *) &D_8015BF1C;
    D_800CA138 = (u8 *) &D_8015BF20;
    D_800CA13C = D_8015BE24;
    D_800CA140 = D_8015BE90;
    D_800CA144 = D_8015BEFC;
    if (func_80082764(D_80122CDC, 0, 0) != 0) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013B910);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013BA80);

void func_8013BBF0(void) {
    D_800CA134 = &D_8015BF34;
    D_800CA138 = &D_8015BF38;
    D_800CA13C = D_8015BDE4;
    D_800CA140 = D_8015BE50;
    D_800CA144 = D_8015BEBC;
    func_80082764(0xFF, 1, 0);
}

void func_8013BC68(void) {
    D_800CA134 = &D_8015BF3C;
    D_800CA138 = &D_8015BF40;
    D_800CA13C = D_8015BD94;
    D_800CA140 = D_8015BDA4;
    D_800CA144 = D_8015BDB4;
    func_80082764(0xFF, 1, 0);
}

void func_8013BCE0(void) {
    D_800CA134 = (u8 *) &D_800CA148;
    if ((D_800E6280.unk_F5F == 0) && (((u32) D_800E6280.unk_0F4.h >> 0xC) == 6) && (D_800CA2FC == 0) && (D_800CA14C == 0)) {
        D_800CA14C += 1;
    }
    D_800CA138 = (u8 *) &D_800CA14C;
    D_800CA13C = D_800CA160;
    D_800CA140 = D_800CA164;
    D_800CA144 = D_800CA168;
    if (func_80082764(D_80122CDC, 0, 0) != 0) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013BDB4);

void func_8013BE98(void) {
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013BEB8);

void func_8013BF94(void) {
    if (D_800CA2F8 == 0x12) {
        func_8013C0A4();
        func_8004284C();
    } else if (D_800CA2F8 == 0x14) {
        func_8013BFF8();
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013BFF8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013C0A4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013C190);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013C410);

void func_8013C624(void) {
    func_80083440((&D_800CA21C)[D_80122CDC]);
    func_8004284C();
}

void func_8013C664(void) {
    if (D_800E6280.unk_75C != 0) {
        func_80083440(1);
    } else {
        func_80083440((&D_800CA224)[D_80122CDC]);
    }
    func_8004284C();
}

void func_8013C6C0(void) {
    func_80083440((&D_800CA224)[D_80122CDC]);
    func_8004284C();
}

void func_8013C700(void) {
    if (D_800E6280.unk_F5F == 2) {
        func_800833F0();
        D_800E6280.unk_1BC[2].unk_02 -= 1;
        D_800E6280.unk_1BC[2].unk_06 -= 1;
    } else if ((D_800E6280.unk_F5F == 1) && (D_800E6280.unk_1BC[1].unk_14[0x1A] == 0)) {
        func_80083378();
        D_800E6280.unk_1BC[2].unk_02 += 1;
        D_800E6280.unk_1BC[2].unk_06 += 1;
    } else {
        func_800833A0();
        D_800E6280.unk_1BC[2].unk_02 += 1;
    }
    func_80084D3C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013C7D0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013CADC);

void func_8013CBAC(void) {
    func_80048EB8(0);
    func_8006BD6C(0);
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    k_reset(0);
    func_80042878(0x81);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013CBF4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013CDF8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013D174);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013D23C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013D2C8);

s32 func_8013D3B0(void) {
    /* 1U: keeps the two constants 1 apart, as in the original (T-4020) */
    if ((D_800E6280.unk_F5F == 1) && (D_800E6280.unk_03F >= 6U) && (D_800E6280.unk_03F < 9U) && (D_80122CDC == 1U)) {
        func_8004284C();
        return 0;
    }
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
}

void func_8013D438(void) {
    func_8004284C();
    func_8004284C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013D468);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013D5FC);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013D8B0);

void func_8013D9E8(void) {
    if (D_800E6280.unk_F5F == 2) {
        func_80083440(4);
        D_800CA148 = 5;
        D_800E6280.unk_1BC[2].unk_02 += 1;
        D_800E6280.unk_1BC[2].unk_06 += 1;
        D_800E6280.unk_1BC[2].unk_0A -= 0x14;
        func_80084D3C();
        func_80042940(0x5E);
        return;
    }
    func_8013272C();
}

void func_8013DA7C(void) {
    if ((D_800E6280.unk_F5F == 2) || (D_800E6280.unk_F5F == 8) || (D_800E6280.unk_F5F == 5) || (D_800E6280.unk_F5F == 0xA)) {
        D_800E6280.unk_110A += 5;
    }
    func_8004284C();
}

void func_8013DAD8(void) {
    if (D_800CA14C == 1) {
        D_800E6280.unk_F5F = 0xE;
    } else {
        D_800E6280.unk_F5F = 2;
    }
    normal_date_speak();
    if (D_800CA14C == 2) {
        D_80122D20 = 1;
        return;
    }
    D_80122D20 = 0;
}

void func_8013DB4C(void) {
    func_80042940(0x3B);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013DB6C);

void func_8013DC5C(void) {
    if ((D_800E6280.unk_F5F == 6) && (D_800E6280.unk_1BC[6].unk_14[4] != 0) && (D_80122CDC == 2)) {
        D_800CA148 += 1;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013DCC0);

void func_8013DD50(void) {
    if ((D_800E6280.unk_F5F == 7) && (D_80122CDC == 0) && (D_800CA2FC == 0)) {
        func_8004284C();
        return;
    }
    func_8004284C();
    func_8004284C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013DDC4);

void func_8013DE50(void) {
    if (D_800CA2FC == 0) {
        func_80083418();
        return;
    }
    if (D_800CA2FC == 2) {
        func_80083418();
        return;
    }
    func_8013934C();
}

void func_8013DEA8(void) {
    if (D_800CA14C == 1) {
        D_800E6280.unk_F5F = D_800E6280.unk_75D;
    }
    if (D_800CA14C == 2) {
        D_80122D20 = 0;
    }
    normal_date_speak();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013DEF8);

void func_8013DFB4(void) {
    func_8004284C();
}

void func_8013DFD4(void) {
    if ((D_800E6280.unk_F5F == 7) && (D_80122CDC == 0) && (D_800CA2FC == 1)) {
        normal_date_speak();
        return;
    }
    func_8004284C();
}

void func_8013E038(void) {
    if ((D_800E6280.unk_F5F == 7) && (D_80122CDC == 0) && (D_800CA2FC == 1)) {
        func_80083418();
        return;
    }
    func_8004284C();
}

void func_8013E09C(void) {
    if ((D_800E6280.unk_F5F == 7) && (D_80122CDC == 0) && (D_800CA2FC == 1)) {
        func_800833A0();
        return;
    }
    func_8004284C();
}

void func_8013E100(void) {
    if ((D_800E6280.unk_F5F == 7) && (D_80122CDC == 2) && (D_800CA2FC == 1)) {
        normal_date_speak();
        return;
    }
    func_8004284C();
}

void func_8013E164(void) {
    normal_date_speak();
}

void func_8013E184(void) {
    if ((D_800E6280.unk_F5F == 8) && (D_800E6280.unk_1BC[8].unk_14[15] != 0)) {
        D_800E6280.unk_110A += 0x1E;
    } else if (D_800E6280.unk_F5F == 8) {
        D_800CA234 = 1;
    }
    func_8004284C();
}

void func_8013E1EC(void) {
    if ((D_800E6280.unk_F5F == 5) && (D_800CA2FC == 0) && (D_80122CDC == 2)) {
        normal_date_speak();
        return;
    }
    if ((D_800E6280.unk_F5F == 5) && (D_800CA2FC == 1) && (D_80122CDC == 0)) {
        normal_date_speak();
        return;
    }
    func_8004284C();
}

void func_8013E288(void) {
    if ((D_800E6280.unk_F5F == 5) && (D_800CA2FC == 0) && (D_80122CDC == 2)) {
        if (D_800E6280.unk_1104.w++ == 0) {
            func_80083440(3);
        }
        func_80085A60();
        return;
    }
    func_8004284C();
}

void func_8013E30C(void) {
    if ((D_800E6280.unk_F5F == 9) && (D_80122CDC == 2)) {
        normal_date_speak();
        return;
    }
    func_8004284C();
}

void func_8013E360(void) {
    if ((D_800E6280.unk_F5F == 0xA) && (D_80122CDC == 1) && (D_800CA2FC == 2)) {
        normal_date_speak();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013E3C4);

void func_8013E4E8(void) {
    if ((D_800E6280.unk_F5F == 0xA) && (D_80122CDC == 2) && (D_800CA2FC == 4)) {
        normal_date_speak();
        return;
    }
    func_8004284C();
}

void func_8013E54C(void) {
    if ((D_800E6280.unk_F5F == 0xA) && (D_80122CDC == 0) && (D_800CA2FC == 0)) {
        normal_date_speak();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013E5B0);

void func_8013E614(void) {
    s32 cur;
    s32 prev;

    /* 1U: keeps the two constants 1 apart, as in the original (T-4020) */
    if ((D_800E6280.unk_F5F == 0xA) && (D_80122CDC == 1U) && ((u8) D_8015B68C == 2) && (D_800CA2FC == 1)) {
        prev = D_800E6280.unk_110A;
        func_80085A60();
        cur = D_800E6280.unk_110A;
        if (prev != cur) {
            D_800CA148 = 0x37;
        }
    } else {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013E6A4);

void func_8013E710(void) {
    if (D_80122CDC == 0) {
        if (D_800E6280.unk_F5F == 7) {
            func_800833A0();
            return;
        }
        func_80083378();
        return;
    }
    func_8004284C();
}

void func_8013E770(void) {
    if (D_800E6280.unk_F5F == 9) {
        func_80083378();
        return;
    }
    func_800833A0();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013E7B0);

void func_8013E980(void) {
    func_80046318(2, 0x80197000, 0xAF46);
    func_80140F40();
    func_8004284C();
}

void func_8013E9BC(void) {
    func_80046318(3, 0x801A4000, 0xAFA9);
    func_80132004();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013E9F8);

s32 func_8013EB1C(void) {
    if (D_800CA2FC == 1) {
        switch (D_800E6280.unk_F5F) {
        case 0:
        case 3:
        case 4:
        case 5:
        case 8:
        case 9:
            func_8004284C();
            return 0;
        }
    }
    return normal_date_girl_in();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013EB8C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013EE3C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013EF1C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013EFA0);

void func_8013F184(void) {
    place_init();
    if ((D_800CA2F8 == 0xB) && (D_800CA2FC == 2)) {
        D_800CA148 += D_8015B658;
    } else {
        D_800CA148 += D_800CA2FC * D_8015B658;
    }
    D_800CA14C = 0;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013F214);

void func_8013F400(void) {
    u8 *temp_v0;

    if (D_800E6280.unk_725 != D_800CA2F8) {
        D_8015BF3C = 7;
        D_800E6280.unk_110A += 0x14;
        return;
    }
    func_8004284C();
    func_8004284C();
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A += 8;
    func_80084D3C();
}

void func_8013F494(void) {
    if (D_800E6280.unk_727 != 0xFF) {
        normal_date_three_select();
        return;
    }
    normal_date_two_select();
}

void func_8013F4D4(void) {
    func_8004284C();
}

void func_8013F4F4(void) {
    addr_init_bustup();
    func_8004284C();
}

void func_8013F51C(void) {
    D_8015BF3C = 7;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013F544);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013F638);

void func_8013FA4C(void) {
    wait_sub_sub(0x3E8);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013FA6C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013FAD0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_8013FD6C);

void func_8014033C(void) {
    func_80046318(0x22, 0x801E0000, 0xBC13);
    func_801323B8();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80140374);

void func_80140788(void) {
    s16 i;

    for (i = 0; i < 20; i++) {
        D_8011ECD0[i * 0x44 + 0x1A0B] &= 0x7F;
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_801407D0);

s32 func_80140A7C(void) {
    u32 t;

    t = (u8) func_80051A68(D_800E6280.unk_75D) & 0x7F;
    if ((D_800E6280.unk_75D == 7) && (t >= 3U) && (D_8015B654 == 2)) {
        D_8015BF3C = 8;
        func_80042940(0x34);
        D_800CA2F8 += 0x32;
        return 0;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80140B08);

s32 func_80140C6C(void) {
    u8 x;
    u32 t;

    x = func_80051A68(D_800E6280.unk_F5F);
    t = x & 0x7F;
    *(s16 *) &D_800CA158 = (t >= 2U) + (t >= 3U) + ((D_800E6280.unk_03E >= 0x60U) * 3) + 0xC;
    D_800CA134 = (u8 *) &D_800CA158;
    D_800CA138 = &D_800CA15C;
    D_800CA13C = D_8015BE18;
    D_800CA140 = D_8015BE84;
    D_800CA144 = D_8015BEF0;
    return func_80082764(D_80122CDC, 1, 0);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80140D38);

void func_80140DB4(void) {
    normal_date_girl_in();
    D_800E6280.unk_54C[0].b[3] &= 0xFFDF;
}

void func_80140DE4(void) {
    func_80044890(0, 0xC043, 0xC01D, 0, 0, 0);
    if (func_80044E8C() == 1) {
        func_80044750(0xBF);
        func_8004284C();
    }
    func_8004284C();
}

void func_80140E44(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0xBF);
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80132000", func_80140ECC);

void func_80140F40(void) {
    D_8015BD90 = 0x801972E8;
    D_8015BD94 = 0x8019772C;
    D_8015BD98 = 0x80197B04;
    D_8015BD9C = 0x80197EA4;
    D_8015BDA0 = 0x801972FC;
    D_8015BDA4 = 0x80197778;
    D_8015BDA8 = 0x80197B10;
    D_8015BDAC = 0x80197EB4;
    D_8015BDB0 = 0x80197450;
    D_8015BDB4 = 0x80197994;
    D_8015BDB8 = 0x80197BD8;
    D_8015BDBC = 0x80197FEC;
}

void func_80141004(void) {
    D_8015BDC0 = 0x801986D0;
    D_8015BDC4 = 0x80198DCC;
    D_8015BDC8 = 0x801991F0;
    D_8015BDCC = 0x801996C0;
    D_8015BDD0 = 0x8019A73C;
    D_8015BDD4 = 0x8019AEC0;
    D_8015BDD8 = 0x8019BDC8;
    D_8015BDDC = 0x8019C1B8;
    D_8015BDE0 = 0x8019CDF0;
    D_8015BDE4 = 0x8019D048;
    D_8015BDE8 = 0x8019D39C;
    D_8015BDEC = 0x8019DA2C;
    D_8015BDF0 = 0x8019E030;
    D_8015BDF4 = 0x8019E3BC;
    D_8015BDF8 = 0x8019ED18;
    D_8015BDFC = 0x8019F41C;
    D_8015BE00 = 0x8019F7E0;
    D_8015BE04 = 0x8019FEE4;
    D_8015BE08 = 0x801A0540;
    D_8015BE0C = 0x801A09D4;
    D_8015BE10 = 0x801A22B8;
    D_8015BE14 = 0x801A33B0;
    D_8015BE18 = 0x80199F04;
    D_8015BE1C = 0x8019B658;
    D_8015BE20 = 0x8019C4B4;
    D_8015BE24 = 0x8019C94C;
    D_8015BE28 = 0x8019DD58;
    D_8015BE2C = 0x80198768;
    D_8015BE30 = 0x80198DFC;
    D_8015BE34 = 0x80199220;
    D_8015BE38 = 0x80199708;
    D_8015BE3C = 0x8019A7B4;
    D_8015BE40 = 0x8019AF10;
    D_8015BE44 = 0x8019BDF8;
    D_8015BE48 = 0x8019C1E8;
    D_8015BE4C = 0x8019CE28;
    D_8015BE50 = 0x8019D068;
    D_8015BE54 = 0x8019D3CC;
    D_8015BE58 = 0x8019DA9C;
    D_8015BE5C = 0x8019E060;
    D_8015BE60 = 0x8019E3EC;
    D_8015BE64 = 0x8019EDB8;
    D_8015BE68 = 0x8019F44C;
    D_8015BE6C = 0x8019F810;
    D_8015BE70 = 0x8019FF44;
    D_8015BE74 = 0x801A0588;
    D_8015BE78 = 0x801A0A04;
    D_8015BE7C = 0x801A2480;
    D_8015BE80 = 0x801A33F8;
    D_8015BE84 = 0x80199F94;
    D_8015BE88 = 0x8019B6CC;
    D_8015BE8C = 0x8019C4DC;
    D_8015BE90 = 0x8019C978;
    D_8015BE94 = 0x8019DD78;
    D_8015BE98 = 0x80198AFC;
    D_8015BE9C = 0x80198F2C;
    D_8015BEA0 = 0x80199318;
    D_8015BEA4 = 0x801998D0;
    D_8015BEA8 = 0x8019AA3C;
    D_8015BEAC = 0x8019B0DC;
    D_8015BEB0 = 0x8019BF28;
    D_8015BEB4 = 0x8019C350;
    D_8015BEB8 = 0x8019CF24;
    D_8015BEBC = 0x8019D120;
    D_8015BEC0 = 0x8019D4FC;
    D_8015BEC4 = 0x8019DC94;
    D_8015BEC8 = 0x8019E158;
    D_8015BECC = 0x8019E51C;
    D_8015BED0 = 0x8019F150;
    D_8015BED4 = 0x8019F57C;
    D_8015BED8 = 0x8019F940;
    D_8015BEDC = 0x801A01C0;
    D_8015BEE0 = 0x801A0750;
    D_8015BEE4 = 0x801A0B34;
    D_8015BEE8 = 0x801A2EB0;
    D_8015BEEC = 0x801A3614;
    D_8015BEF0 = 0x8019A3CC;
    D_8015BEF4 = 0x8019BAD8;
    D_8015BEF8 = 0x8019C608;
    D_8015BEFC = 0x8019CAF8;
    D_8015BF00 = 0x8019DDF8;
}

void func_80141518(void) {
    D_8015BDC0 = 0x801986C8;
    D_8015BDC4 = 0x80198D8C;
    D_8015BDC8 = 0x801991A0;
    D_8015BDCC = 0x801997D0;
    D_8015BDD0 = 0x8019AD68;
    D_8015BDD4 = 0x8019B548;
    D_8015BDD8 = 0x8019C404;
    D_8015BDDC = 0x8019C7D8;
    D_8015BDE0 = 0x8019D4D4;
    D_8015BDE4 = 0x8019D734;
    D_8015BDE8 = 0x8019DA64;
    D_8015BDEC = 0x8019E0A0;
    D_8015BDF0 = 0x8019E754;
    D_8015BDF4 = 0x8019EAD0;
    D_8015BDF8 = 0x8019F354;
    D_8015BDFC = 0x8019F938;
    D_8015BE04 = 0x8019FF48;
    D_8015BE08 = 0x801A05AC;
    D_8015BE0C = 0x801A0A50;
    D_8015BE10 = 0x801A1FCC;
    D_8015BE14 = 0x801A353C;
    D_8015BE18 = 0x8019A30C;
    D_8015BE1C = 0x8019BCCC;
    D_8015BE20 = 0x8019CB24;
    D_8015BE24 = 0x8019D03C;
    D_8015BE28 = 0x8019E3F4;
    D_8015BE2C = 0x80198760;
    D_8015BE30 = 0x80198DBC;
    D_8015BE34 = 0x801991D0;
    D_8015BE38 = 0x80199838;
    D_8015BE3C = 0x8019ADE0;
    D_8015BE40 = 0x8019B598;
    D_8015BE44 = 0x8019C434;
    D_8015BE48 = 0x8019C808;
    D_8015BE4C = 0x8019D50C;
    D_8015BE50 = 0x8019D754;
    D_8015BE54 = 0x8019DA94;
    D_8015BE58 = 0x8019E110;
    D_8015BE5C = 0x8019E784;
    D_8015BE60 = 0x8019EB00;
    D_8015BE64 = 0x8019F3D4;
    D_8015BE68 = 0x8019F968;
    D_8015BE70 = 0x8019FFA8;
    D_8015BE74 = 0x801A05F4;
    D_8015BE78 = 0x801A0A80;
    D_8015BE7C = 0x801A219C;
    D_8015BE80 = 0x801A35B4;
    D_8015BE84 = 0x8019A3BC;
    D_8015BE88 = 0x8019BD3C;
    D_8015BE8C = 0x8019CB50;
    D_8015BE90 = 0x8019D068;
    D_8015BE94 = 0x8019E414;
    D_8015BE98 = 0x80198AF4;
    D_8015BE9C = 0x80198EEC;
    D_8015BEA0 = 0x801992C8;
    D_8015BEA4 = 0x80199AB8;
    D_8015BEA8 = 0x8019B068;
    D_8015BEAC = 0x8019B764;
    D_8015BEB0 = 0x8019C564;
    D_8015BEB4 = 0x8019C970;
    D_8015BEB8 = 0x8019D608;
    D_8015BEBC = 0x8019D80C;
    D_8015BEC0 = 0x8019DBA8;
    D_8015BEC4 = 0x8019E308;
    D_8015BEC8 = 0x8019E87C;
    D_8015BECC = 0x8019EC30;
    D_8015BED0 = 0x8019F698;
    D_8015BED4 = 0x8019FA98;
    D_8015BEDC = 0x801A0208;
    D_8015BEE0 = 0x801A07BC;
    D_8015BEE4 = 0x801A0BB0;
    D_8015BEE8 = 0x801A2C58;
    D_8015BEEC = 0x801A3938;
    D_8015BEF0 = 0x8019A9B4;
    D_8015BEF4 = 0x8019C12C;
    D_8015BEF8 = 0x8019CCB4;
    D_8015BEFC = 0x8019D204;
    D_8015BF00 = 0x8019E4CC;
}

void func_801419FC(void) {
    D_8015BDC0 = 0x8019863C;
    D_8015BDC4 = 0x80198CE0;
    D_8015BDC8 = 0x80199074;
    D_8015BDCC = 0x801994E0;
    D_8015BDD0 = 0x8019A698;
    D_8015BDD4 = 0x8019AD74;
    D_8015BDD8 = 0x8019B96C;
    D_8015BDDC = 0x8019BD30;
    D_8015BDE0 = 0x8019C8D8;
    D_8015BDE4 = 0x8019CB24;
    D_8015BDE8 = 0x8019CD88;
    D_8015BDEC = 0x8019D268;
    D_8015BDF0 = 0x8019D7CC;
    D_8015BDF4 = 0x8019DB58;
    D_8015BDF8 = 0x8019E2B8;
    D_8015BDFC = 0x8019E7D8;
    D_8015BE04 = 0x8019EDFC;
    D_8015BE08 = 0x8019F490;
    D_8015BE0C = 0x8019F90C;
    D_8015BE10 = 0x801A0F20;
    D_8015BE14 = 0x801A2014;
    D_8015BE18 = 0x80199DDC;
    D_8015BE1C = 0x8019B35C;
    D_8015BE20 = 0x8019C008;
    D_8015BE24 = 0x8019C4D8;
    D_8015BE28 = 0x8019D584;
    D_8015BE2C = 0x801986D4;
    D_8015BE30 = 0x80198D10;
    D_8015BE34 = 0x801990A4;
    D_8015BE38 = 0x80199528;
    D_8015BE3C = 0x8019A700;
    D_8015BE40 = 0x8019ADC4;
    D_8015BE44 = 0x8019B99C;
    D_8015BE48 = 0x8019BD60;
    D_8015BE4C = 0x8019C910;
    D_8015BE50 = 0x8019CB44;
    D_8015BE54 = 0x8019CDB8;
    D_8015BE58 = 0x8019D2D8;
    D_8015BE5C = 0x8019D7FC;
    D_8015BE60 = 0x8019DB88;
    D_8015BE64 = 0x8019E328;
    D_8015BE68 = 0x8019E808;
    D_8015BE70 = 0x8019EE5C;
    D_8015BE74 = 0x8019F4D8;
    D_8015BE78 = 0x8019F93C;
    D_8015BE7C = 0x801A10EC;
    D_8015BE80 = 0x801A205C;
    D_8015BE84 = 0x80199E78;
    D_8015BE88 = 0x8019B3CC;
    D_8015BE8C = 0x8019C030;
    D_8015BE90 = 0x8019C504;
    D_8015BE94 = 0x8019D5A4;
    D_8015BE98 = 0x80198A68;
    D_8015BE9C = 0x80198E40;
    D_8015BEA0 = 0x80199180;
    D_8015BEA4 = 0x801996F0;
    D_8015BEA8 = 0x8019A92C;
    D_8015BEAC = 0x8019AF90;
    D_8015BEB0 = 0x8019BACC;
    D_8015BEB4 = 0x8019BEC8;
    D_8015BEB8 = 0x8019CA0C;
    D_8015BEBC = 0x8019CBE0;
    D_8015BEC0 = 0x8019CECC;
    D_8015BEC4 = 0x8019D4D0;
    D_8015BEC8 = 0x8019D8F4;
    D_8015BECC = 0x8019DCB8;
    D_8015BED0 = 0x8019E5AC;
    D_8015BED4 = 0x8019E938;
    D_8015BEDC = 0x8019F0BC;
    D_8015BEE0 = 0x8019F6A0;
    D_8015BEE4 = 0x8019FA6C;
    D_8015BEE8 = 0x801A1B70;
    D_8015BEEC = 0x801A2278;
    D_8015BEF0 = 0x8019A358;
    D_8015BEF4 = 0x8019B714;
    D_8015BEF8 = 0x8019C15C;
    D_8015BEFC = 0x8019C684;
    D_8015BF00 = 0x8019D624;
}
