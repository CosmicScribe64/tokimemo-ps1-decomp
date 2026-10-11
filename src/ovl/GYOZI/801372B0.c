#include "common.h"
#include "ovl/GYOZI.h"

void func_801372B0(void) {
    D_80146220 = 0x801B0480;
    D_80146224 = 0x801B12D0;
    D_80146228 = 0x801B2668;
    D_8014622C = 0x801B344C;
    D_80146230 = 0x801B3DDC;
    D_80146234 = 0x801B46E8;
    D_80146238 = 0x801B507C;
    D_8014623C = 0x801B5EC0;
    D_80146240 = 0x801B71FC;
    D_80146244 = 0x801B8070;
    D_80146248 = 0x801B8928;
    D_8014624C = 0x801B92F8;
    D_80146250 = 0x801B9CD8;
    D_80146254 = 0x801B0510;
    D_80146258 = 0x801B1404;
    D_8014625C = 0x801B2784;
    D_80146260 = 0x801B34E4;
    D_80146264 = 0x801B3E74;
    D_80146268 = 0x801B4780;
    D_8014626C = 0x801B5114;
    D_80146270 = 0x801B5FE4;
    D_80146274 = 0x801B7320;
    D_80146278 = 0x801B8108;
    D_8014627C = 0x801B89C0;
    D_80146280 = 0x801B9400;
    D_80146284 = 0x801B9D70;
    D_80146288 = 0x801B0868;
    D_8014628C = 0x801B1CC4;
    D_80146290 = 0x801B2F40;
    D_80146294 = 0x801B3878;
    D_80146298 = 0x801B4208;
    D_8014629C = 0x801B4AF8;
    D_801462A0 = 0x801B54FC;
    D_801462A4 = 0x801B67F4;
    D_801462A8 = 0x801B7B30;
    D_801462AC = 0x801B8480;
    D_801462B0 = 0x801B8D38;
    D_801462B4 = 0x801B9820;
    D_801462B8 = 0x801BA0E8;
}

void func_80137524(void) {
    D_801462BC = 0x801B00D0;
    D_801462C0 = 0x801B0274;
    D_801462C4 = 0x801B0374;
    D_801462C8 = 0x801B0464;
    D_801462CC = 0x801B05F4;
    D_801462D0 = 0x801B0788;
    D_801462D4 = 0x801B0950;
    D_801462D8 = 0x801B0B44;
    D_801462DC = 0x801B0CDC;
    D_801462E0 = 0x801B0EA0;
    D_801462E4 = 0x801B109C;
    D_801462E8 = 0x801B1210;
    D_801462EC = 0x801B1380;
    D_801462F0 = 0x801B00E8;
    D_801462F4 = 0x801B0290;
    D_801462F8 = 0x801B037C;
    D_801462FC = 0x801B047C;
    D_80146300 = 0x801B060C;
    D_80146304 = 0x801B07A0;
    D_80146308 = 0x801B096C;
    D_8014630C = 0x801B0B5C;
    D_80146310 = 0x801B0CF4;
    D_80146314 = 0x801B0EBC;
    D_80146318 = 0x801B10B8;
    D_8014631C = 0x801B1218;
    D_80146320 = 0x801B139C;
    D_80146324 = 0x801B019C;
    D_80146328 = 0x801B0344;
    D_8014632C = 0x801B039C;
    D_80146330 = 0x801B0530;
    D_80146334 = 0x801B06C0;
    D_80146338 = 0x801B0854;
    D_8014633C = 0x801B0A58;
    D_80146340 = 0x801B0C10;
    D_80146344 = 0x801B0DA8;
    D_80146348 = 0x801B0FA8;
    D_8014634C = 0x801B11A4;
    D_80146350 = 0x801B1270;
    D_80146354 = 0x801B1488;
}

typedef struct {
    void (*f[48])();
} FnTbl48; /* size 0xC0 */
extern FnTbl48 D_80146368;

void func_80137798(void) {
    u16 idx; /* FAKE: u16 and the second call argument put idx in $a1 as the original does (permuter); real prototype unknown. T-8080 */
    FnTbl48 tbl;

    tbl = D_80146368;
    if (D_80145F60 != 0) {
        idx = D_800F647A;
        if ((void (*)())D_800896E0 == tbl.f[idx] || (void (*)())D_800898F0 == tbl.f[idx]) {
            tbl.f[idx] = func_8004DE1C;
        }
    }
    idx = D_800F647A;
    tbl.f[idx](0x80, idx);
}

void func_80137854(void) {
    u32 temp_t8;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    temp_t8 = (u8)func_8005E0E0(5) & 0x7F;
    if ((D_800F62CF == 5) && (temp_t8 < 2U)) {
        if (D_800F6474++ == 0) {
            D_80145EB4 = 0x2A;
        }
        func_801343A4();
        return;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_801378D0);

void func_80137A0C(void) {
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    if ((u32) ((u8)func_8005E0E0(D_800F62CF) & 0x7F) >= 2U) {
        D_80145EB4 += 2;
    }
    if (D_800F53DE == 0x62) {
        D_80145EB4 += 1;
    }
    func_8004DE1C();
}

void func_80137A80(void) {
    func_8008A0D4(0x418C);
    func_8004DE1C();
}

void func_80137AA8(void) {
    if (D_80145F60 == 0) {
        D_800F53A0.girl[D_800F62CF].unk_0A += 1;
    }
    func_8008FB00();
    func_80050D60(0, 0);
    func_80081D30(1);
}

void func_80137B18(void) {
    func_80051DD8(0x13, 0x801B0000, 0xA3C8);
    func_801372B0();
    func_8004DE1C();
}

void func_80137B50(void) {
    D_8012E66C = D_80146364;
    func_8004DE1C();
}

void func_80137B7C(void) {
    D_80146364 = (u8) D_8012E66C;
    D_800D9248 = 0;
    D_800D924C = 0;
    func_801372B0();
    D_800D9258 = D_80146224;
    D_800D925C = D_80146258;
    D_800D9260 = D_8014628C;
    func_8004DE1C();
}

void func_80137BEC(void) {
    if (D_8012E66C != 0) {
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A -= 1;
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0E += 5;
        func_8008FB00();
        D_800F647A += 0xB;
        return;
    }
    D_800D9248 += 1;
    func_80090960(0xA, 0x29);
    func_8004DE1C();
}

void func_80137CAC(void) {
    u32 temp_t6;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    temp_t6 = (u8)func_8005E0E0(D_800F62CF) & 0x7F;
    if (temp_t6 < 2U) {
        D_800D9248 = 6;
    } else if (temp_t6 == 2) {
        D_800D9248 = 8;
    } else if (temp_t6 == 3) {
        D_800D9248 = 0xA;
    } else {
        D_800D9248 = 0xC;
    }
    func_8004DE1C();
}

void func_80137D3C(void) {
    D_800D9248 = (D_8012E66C * 2) + 0xD;
    func_8004DE1C();
}

extern u8 D_800F53DF;
extern u8 D_800F53E0;
extern s32 D_800F5490;

void func_80137D70(void) {
    s32 r;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    r = (u8)func_8005E0E0(D_800F62CF);
    if (D_800F53DF == (D_800F5490 & 0xF) && D_800F53E0 == ((u32)(D_800F5490 << 0x17) >> 0x1B)) {
        if ((u32)(r & 0x7F) < 2U) {
            D_800F5AAE |= 4;
            /* one base symbol keeps as1 from hoisting this load above the store */
            (&D_800F5AAE)[0x9CC] += 5;
        } else {
            D_800F5AAE |= 8;
            func_8004DE1C();
        }
    } else {
        func_8004DE1C();
    }
}

void func_80137E24(void) {
    D_800D9248 = 0x1D;
    D_800D9258 = D_80146358;
    D_800D925C = D_8014635C;
    D_800D9260 = D_80146360;
    func_80137E84();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_80137E84);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_80137F48);

void func_80137FEC(void) {
    func_8008E408(D_8012E66C + 1);
    func_80072FE8();
    if (D_8012E66C == 0) {
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 += 3;
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A += 2;
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0E -= 0xF;
    } else if (D_8012E66C == 1) {
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 += 2;
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A += 1;
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0E -= 0xA;
    } else {
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A += 1;
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0E += 0xA;
    }
    func_8008FB00();
    func_8004DE1C();
}

void func_801381A4(void) {
    func_80051DD8(3, 0x801B0000, 0xA3FB);
    func_80137524();
    func_8004DE1C();
}

/* the load of D_800F62CF goes through the base of D_800F5AAE: as1 then keeps the byte store before it (no store in the jal slot) */
void func_801381DC(void) {
    D_800F5AAE |= 8;
    if ((u32)((u8)func_8005E0E0((&D_800F5AAE)[0x821]) & 0x7F) >= 2U) {
        D_800F647A += 6;
        return;
    }
    func_80090960(0xD, D_8012E698);
    D_800F5AAE |= 4;
    func_8004DE1C();
}

void func_80138264(void) {
    D_800D9248 = 1;
    D_800D924C = 0;
    func_8013829C();
    func_8004DE1C();
}

void func_8013829C(void) {
    switch (D_800F62CF) {
    case 0:
        D_800D9258 = D_801462C0;
        D_800D925C = D_801462F4;
        D_800D9260 = D_80146328;
        return;
    case 1:
        D_800D9258 = D_801462D4;
        D_800D925C = D_80146308;
        D_800D9260 = D_8014633C;
        return;
    case 2:
        D_800D9258 = D_801462C8;
        D_800D925C = D_801462FC;
        D_800D9260 = D_80146330;
        return;
    case 3:
        D_800D9258 = D_801462CC;
        D_800D925C = D_80146300;
        D_800D9260 = D_80146334;
        return;
    case 4:
        D_800D9258 = D_801462E4;
        D_800D925C = D_80146318;
        D_800D9260 = D_8014634C;
        return;
    case 5:
        D_800D9258 = D_801462D8;
        D_800D925C = D_8014630C;
        D_800D9260 = D_80146340;
        return;
    case 6:
        D_800D9258 = D_801462DC;
        D_800D925C = D_80146310;
        D_800D9260 = D_80146344;
        return;
    case 7:
        D_800D9258 = D_801462D0;
        D_800D925C = D_80146304;
        D_800D9260 = D_80146338;
        return;
    case 8:
        D_800D9258 = D_801462BC;
        D_800D925C = D_801462F0;
        D_800D9260 = D_80146324;
        return;
    case 9:
        D_800D9258 = D_801462E0;
        D_800D925C = D_80146314;
        D_800D9260 = D_80146348;
        return;
    case 10:
        D_800D9258 = D_801462EC;
        D_800D925C = D_80146320;
        D_800D9260 = D_80146354;
        return;
    }
}
