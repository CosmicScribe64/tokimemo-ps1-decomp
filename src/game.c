#include "common.h"
#include "game.h"
#include "libapi.h"

INCLUDE_ASM("asm/nonmatchings/game", func_80041000);

INCLUDE_ASM("asm/nonmatchings/game", func_800410AC);

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, store order: the original stores h, w before x, y. */
void func_8004111C(void) {
    RECT rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = 640;
    rect.h = 480;
    func_8009C7F8(&rect, 0, 0, 0);
    func_8009C674(0);
}
#else
INCLUDE_ASM("asm/nonmatchings/game", func_8004111C);
#endif

INCLUDE_ASM("asm/nonmatchings/game", func_80041168);

INCLUDE_ASM("asm/nonmatchings/game", func_800412E0);

void func_80041584(void) {
    func_800415B4(0, 0x40);
    func_8004164C(0, 2);
}

INCLUDE_ASM("asm/nonmatchings/game", func_800415B4);

INCLUDE_ASM("asm/nonmatchings/game", func_8004164C);

INCLUDE_ASM("asm/nonmatchings/game", func_80041840);

void func_80041878(void) {
    func_800418B0();
    func_800419FC();
    func_80041C2C();
    func_80041F48();
}

INCLUDE_ASM("asm/nonmatchings/game", func_800418B0);

INCLUDE_ASM("asm/nonmatchings/game", func_800419FC);

INCLUDE_ASM("asm/nonmatchings/game", func_80041C2C);

INCLUDE_ASM("asm/nonmatchings/game", func_80041F48);

INCLUDE_ASM("asm/nonmatchings/game", func_80042058);

INCLUDE_ASM("asm/nonmatchings/game", func_800420D0);

INCLUDE_ASM("asm/nonmatchings/game", func_80042134);

INCLUDE_ASM("asm/nonmatchings/game", func_800422C8);

INCLUDE_ASM("asm/nonmatchings/game", func_800423D4);

/* The named temp gives the original's v1/v0 registers; `D += 0x377; return D;`
 * and `return D += 0x377;` load into t6 instead (T-0014). */
s32 func_80042400(void) {
    s32 t = D_800E7D10 + 0x377;

    D_800E7D10 = t;
    return t;
}

INCLUDE_ASM("asm/nonmatchings/game", func_80042418);

void func_80042458(void) {
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
}

INCLUDE_ASM("asm/nonmatchings/game", func_80042488);

INCLUDE_ASM("asm/nonmatchings/game", func_800424FC);

INCLUDE_ASM("asm/nonmatchings/game", func_80042540);

INCLUDE_ASM("asm/nonmatchings/game", func_80042798);

INCLUDE_ASM("asm/nonmatchings/game", func_80042808);

INCLUDE_ASM("asm/nonmatchings/game", func_8004284C);

INCLUDE_ASM("asm/nonmatchings/game", func_80042878);

INCLUDE_ASM("asm/nonmatchings/game", func_80042908);

INCLUDE_ASM("asm/nonmatchings/game", func_80042940);

INCLUDE_ASM("asm/nonmatchings/game", func_80042960);

INCLUDE_ASM("asm/nonmatchings/game", func_80042A00);

INCLUDE_ASM("asm/nonmatchings/game", func_80042AC8);

INCLUDE_ASM("asm/nonmatchings/game", func_80042BD4);

INCLUDE_ASM("asm/nonmatchings/game", func_80042C30);

void func_80042C74(void) {
    D_8011ECF6 = 0;
    D_8011ECFA = 0;
    D_800E71FA = 0;
    D_800E71FC = 0;
    D_800E7200 = 0;
    D_800E7204 = 0;
    D_800E7208 = 0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_80042CB0);

INCLUDE_ASM("asm/nonmatchings/game", func_80043010);

INCLUDE_ASM("asm/nonmatchings/game", func_800430C0);

INCLUDE_ASM("asm/nonmatchings/game", func_80043448);

INCLUDE_ASM("asm/nonmatchings/game", func_80043504);

INCLUDE_ASM("asm/nonmatchings/game", func_80043510);

void func_800438DC(u8 arg0, u8 arg1) {
    D_800E7394 = arg0;
    D_800E7393 = arg1;
}

void func_800438F0(s32 arg0) {
    if (arg0 != 0) {
        D_800E62B9 = 1;
    } else {
        D_800E62B9 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/game", func_80043914);

INCLUDE_ASM("asm/nonmatchings/game", func_80043980);

INCLUDE_ASM("asm/nonmatchings/game", func_80043A00);

INCLUDE_ASM("asm/nonmatchings/game", func_80043A84);

INCLUDE_ASM("asm/nonmatchings/game", func_80043B74);

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, ugen temporaries: original lhu into t8/t9, IDO t0/t1 (register allocation, not the frame). */
void func_8004435C(u16 arg0, u16 arg1, s16 arg2, s16 arg3, void *arg4) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C884(&rect, arg4);
}
#else
INCLUDE_ASM("asm/nonmatchings/game", func_8004435C);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, ugen temporaries: original lhu into t7/t8/t9, IDO t8/t9/t0 (register allocation, not the frame). */
void func_800443A0(u16 arg0, u16 arg1, u16 arg2, s16 arg3, u16 arg4, u16 arg5) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C93C(&rect, arg4, arg5);
}
#else
INCLUDE_ASM("asm/nonmatchings/game", func_800443A0);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, ugen temporaries: original lhu into t8/t9, IDO t0/t1 (register allocation, not the frame). */
void func_800443F0(u16 arg0, u16 arg1, s16 arg2, s16 arg3, void *arg4) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C8E0(&rect, arg4);
}
#else
INCLUDE_ASM("asm/nonmatchings/game", func_800443F0);
#endif

void func_80044434(void) {
}

INCLUDE_ASM("asm/nonmatchings/game", func_8004443C);

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, original frame has 8 more bytes of locals (0x38 vs 0x30); unknown extra local. */
void func_80044700(s32 arg0, s32 arg1, s32 arg2) {
    RECT rect;

    rect.x = arg1 << 4;
    rect.y = arg0 + 0x1E0;
    rect.w = 0x10;
    rect.h = 1;
    func_8009C93C(&rect, 0x100, arg2 + 0x1E0);
}
#else
INCLUDE_ASM("asm/nonmatchings/game", func_80044700);
#endif

INCLUDE_ASM("asm/nonmatchings/game", func_80044750);

INCLUDE_ASM("asm/nonmatchings/game", func_80044774);

u8 func_8004480C(void) {
    return D_800B3D40;
}

u8 func_8004481C(void) {
    return D_800B3D44;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8004482C);

INCLUDE_ASM("asm/nonmatchings/game", func_80044890);

INCLUDE_ASM("asm/nonmatchings/game", func_80044C98);

INCLUDE_ASM("asm/nonmatchings/game", func_80044D54);

INCLUDE_ASM("asm/nonmatchings/game", func_80044E8C);

INCLUDE_ASM("asm/nonmatchings/game", func_80044F94);

INCLUDE_ASM("asm/nonmatchings/game", func_8004500C);

INCLUDE_ASM("asm/nonmatchings/game", func_800450F4);

s32 func_800451D0(void) {
    return D_801255D8;
}

void func_800451E0(s32 arg0) {
    D_801255D8 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_800451EC);

INCLUDE_ASM("asm/nonmatchings/game", func_80045224);

s32 func_80045288(void) {
    if (D_80125129 == D_80125128 && D_8012512B == -1) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_800452C4);

INCLUDE_ASM("asm/nonmatchings/game", func_80045318);

INCLUDE_ASM("asm/nonmatchings/game", func_80045414);

INCLUDE_ASM("asm/nonmatchings/game", func_800455C4);

u8 func_80046094(void) {
    return D_80125130[(D_80125128 + 0x2FU) % 0x30U * 24];
}

u8 func_800460CC(void) {
    return D_8012512A;
}

u8 func_800460DC(void) {
    return D_801255B5;
}

u8 func_800460EC(void) {
    return D_801255B4;
}

INCLUDE_ASM("asm/nonmatchings/game", func_800460FC);

s32 func_80046274(void) {
    return D_801255B0;
}

s32 *func_80046284(void) {
    return &D_800B3D70;
}

INCLUDE_ASM("asm/nonmatchings/game", func_80046290);

void func_800462BC(u8 arg0, s32 arg1, s32 *arg2) {
    *arg2 = arg1;
    ((u8 *)arg2)[3] = arg0;
}

void func_800462C8(u8 arg0, s32 arg1, s32 arg2) {
    /* FAKE: size 0x20 is taken from the original frame (local at 0x28 in 0x48);
     * the real type of this buffer is unknown. T-0016 */
    u8 buf[0x20];

    func_80044750(6);
    func_800462BC(arg0, arg1, (s32 *)buf);
    func_80045414(6, arg2, buf);
    D_800B3D60 = 0;
}

void func_80046318(u8 arg0, s32 arg1, s32 arg2) {
    /* FAKE: size 0x20 is taken from the original frame (local at 0x28 in 0x48);
     * the real type of this buffer is unknown. T-0016 */
    u8 buf[0x20];

    func_80044750(6);
    func_800462BC(arg0, arg1, (s32 *)buf);
    func_80045414(6, arg2, buf);
    D_800B3D60 = 1;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8004636C);

INCLUDE_ASM("asm/nonmatchings/game", func_800463E8);

INCLUDE_ASM("asm/nonmatchings/game", func_80046478);

INCLUDE_ASM("asm/nonmatchings/game", func_80046500);

INCLUDE_ASM("asm/nonmatchings/game", func_80046590);

INCLUDE_ASM("asm/nonmatchings/game", func_80046684);

INCLUDE_ASM("asm/nonmatchings/game", func_80046754);

INCLUDE_ASM("asm/nonmatchings/game", func_80046AC8);

void func_80047550(void) {
    D_801255DC = 4;
}

INCLUDE_ASM("asm/nonmatchings/game", func_80047560);

INCLUDE_ASM("asm/nonmatchings/game", func_800476C0);

INCLUDE_ASM("asm/nonmatchings/game", func_800482FC);

INCLUDE_ASM("asm/nonmatchings/game", func_80048390);

INCLUDE_ASM("asm/nonmatchings/game", func_800483E8);

INCLUDE_ASM("asm/nonmatchings/game", func_80048514);

INCLUDE_ASM("asm/nonmatchings/game", func_800485BC);

INCLUDE_ASM("asm/nonmatchings/game", func_800486A4);

INCLUDE_ASM("asm/nonmatchings/game", func_80048828);

INCLUDE_ASM("asm/nonmatchings/game", func_80048A90);

INCLUDE_ASM("asm/nonmatchings/game", func_80048CF8);

void func_80048DAC(s32 arg0) {
    if (arg0 != 0) {
        D_800E7392 = 1;
    } else {
        D_800E7392 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/game", func_80048DD0);

INCLUDE_ASM("asm/nonmatchings/game", func_80048E78);

INCLUDE_ASM("asm/nonmatchings/game", func_80048EB8);

INCLUDE_ASM("asm/nonmatchings/game", func_80048F64);

u8 func_8004901C(void) {
    return D_800E62BA;
}

void func_8004902C(void) {
    D_800E62BA = 0x80;
    D_800E62BB = 0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_80049044);

void func_800490B4(u8 arg0) {
    D_800E62BB = arg0;
}

void func_800490C0(s32 arg0, s32 arg1) {
    u8 *p = D_800E6280 + arg1 * 12;

    *(s32 *)(p + 0x24) = arg0;
    *(s32 *)(p + 0x1C) = 0x10C00;
    *(s32 *)(p + 0x20) = 0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_800490F0);

INCLUDE_ASM("asm/nonmatchings/game", func_80049140);

INCLUDE_ASM("asm/nonmatchings/game", func_800493A8);

INCLUDE_ASM("asm/nonmatchings/game", func_80049450);

INCLUDE_ASM("asm/nonmatchings/game", func_800494BC);

INCLUDE_ASM("asm/nonmatchings/game", func_8004955C);

INCLUDE_ASM("asm/nonmatchings/game", func_800495DC);

INCLUDE_ASM("asm/nonmatchings/game", func_80049A40);

INCLUDE_ASM("asm/nonmatchings/game", func_80049FF0);

INCLUDE_ASM("asm/nonmatchings/game", func_8004A14C);

INCLUDE_ASM("asm/nonmatchings/game", func_8004A1A0);

INCLUDE_ASM("asm/nonmatchings/game", func_8004A414);

INCLUDE_ASM("asm/nonmatchings/game", func_8004A814);

INCLUDE_ASM("asm/nonmatchings/game", func_8004AC18);

INCLUDE_ASM("asm/nonmatchings/game", func_8004ACC8);

INCLUDE_ASM("asm/nonmatchings/game", func_8004AD60);

INCLUDE_ASM("asm/nonmatchings/game", func_8004ADAC);

u8 func_8004ADD4(void) {
    return D_800B3D80;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8004ADE4);

INCLUDE_ASM("asm/nonmatchings/game", func_8004AE28);

INCLUDE_ASM("asm/nonmatchings/game", func_8004AE54);

INCLUDE_ASM("asm/nonmatchings/game", func_8004B19C);

void func_8004B338(s32 arg0, s32 arg1, s32 arg2) {
    func_8004B358(arg0, arg1, arg2, 0xF);
}

INCLUDE_ASM("asm/nonmatchings/game", func_8004B358);

INCLUDE_ASM("asm/nonmatchings/game", func_8004B590);

INCLUDE_ASM("asm/nonmatchings/game", func_8004B778);

INCLUDE_ASM("asm/nonmatchings/game", func_8004BBA8);

INCLUDE_ASM("asm/nonmatchings/game", func_8004C1AC);

INCLUDE_ASM("asm/nonmatchings/game", func_8004C3F0);

INCLUDE_ASM("asm/nonmatchings/game", func_8004D088);

INCLUDE_ASM("asm/nonmatchings/game", func_8004D320);

INCLUDE_ASM("asm/nonmatchings/game", func_8004D3C8);

INCLUDE_ASM("asm/nonmatchings/game", func_8004D6B8);

/* FAKE: taking the address of the argument forces the home-slot spill (sw a0,0(sp)); real source unknown. T-0400 */
void func_8004DAC4(s32 arg0) {
    s32 *p = &arg0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8004DACC);

INCLUDE_ASM("asm/nonmatchings/game", func_8004DDF0);

INCLUDE_ASM("asm/nonmatchings/game", func_8004E0FC);

INCLUDE_ASM("asm/nonmatchings/game", func_8004E350);

INCLUDE_ASM("asm/nonmatchings/game", func_8004E44C);

INCLUDE_ASM("asm/nonmatchings/game", func_8004E500);

INCLUDE_ASM("asm/nonmatchings/game", func_8004E58C);

void func_8004E750(u8 arg0) {
    if (arg0 == 0x10) {
        D_800B3F6A = 0x10;
    } else {
        D_800B3F6A = 0xE;
    }
}

void func_8004E780(void) {
}

INCLUDE_ASM("asm/nonmatchings/game", func_8004E788);

INCLUDE_ASM("asm/nonmatchings/game", func_8004E884);

void func_8004E93C(s32 arg0, s32 arg1) {
    if (arg1 > 0) {
        D_800B3DC7[arg0 * 8] = 1;
    } else {
        D_800B3DC7[arg0 * 8] = 0;
    }
}

u8 *func_8004E970(s32 arg0) {
    if (arg0 >= 0x35) {
        return D_800B3F58;
    }
    return D_800B3DC0 + arg0 * 8;
}

s16 *func_8004E99C(void) {
    return &D_800B3F60;
}

void func_8004E9A8(s32 arg0, Entry8 arg1) {
    Entry8 *p = (Entry8 *)D_800B3DC0 + arg0;

    p->unk_00 = arg1.unk_00;
    p->unk_02 = arg1.unk_02;
    p->unk_04 = arg1.unk_04;
    p->unk_05 = arg1.unk_05;
    p->unk_06 = arg1.unk_06;
    p->unk_07 = arg1.unk_07;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8004E9F4);

void func_8004EA98(void) {
    D_800B3F64 = D_800B3F60;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8004EAAC);

INCLUDE_ASM("asm/nonmatchings/game", func_8004EAD4);

INCLUDE_ASM("asm/nonmatchings/game", func_8004EAFC);

INCLUDE_ASM("asm/nonmatchings/game", func_8004EBEC);

u8 func_8004EC14(void) {
    return D_800B3F66;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8004EC24);

s32 func_8004ECB4(void) {
    if (D_800B3F62 == D_800B3F68) {
        return 1;
    }
    return 0;
}

u8 func_8004ECE0(void) {
    return *(u8 *)&D_800B3F60;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8004ECF0);

INCLUDE_ASM("asm/nonmatchings/game", func_8004EE10);

INCLUDE_ASM("asm/nonmatchings/game", func_8004F268);

INCLUDE_ASM("asm/nonmatchings/game", func_8004F364);

INCLUDE_ASM("asm/nonmatchings/game", func_8004F4F0);

INCLUDE_ASM("asm/nonmatchings/game", func_8004F6B0);

INCLUDE_ASM("asm/nonmatchings/game", func_8004F870);

INCLUDE_ASM("asm/nonmatchings/game", func_8004F984);

INCLUDE_ASM("asm/nonmatchings/game", func_8004FA88);

INCLUDE_ASM("asm/nonmatchings/game", func_8004FB6C);

INCLUDE_ASM("asm/nonmatchings/game", func_8004FC10);

INCLUDE_ASM("asm/nonmatchings/game", func_80050324);

INCLUDE_ASM("asm/nonmatchings/game", func_80050408);

INCLUDE_ASM("asm/nonmatchings/game", func_80050B00);

INCLUDE_ASM("asm/nonmatchings/game", func_80050B54);

INCLUDE_ASM("asm/nonmatchings/game", func_80050C24);

INCLUDE_ASM("asm/nonmatchings/game", func_80050D78);

INCLUDE_ASM("asm/nonmatchings/game", func_80050DFC);

INCLUDE_ASM("asm/nonmatchings/game", func_80050E8C);

INCLUDE_ASM("asm/nonmatchings/game", func_80051010);

void func_8005142C(u8 arg0) {
    D_800E71DF = arg0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_80051438);

INCLUDE_ASM("asm/nonmatchings/game", func_80051508);

INCLUDE_ASM("asm/nonmatchings/game", func_80051A68);

INCLUDE_ASM("asm/nonmatchings/game", func_80051B48);

INCLUDE_ASM("asm/nonmatchings/game", func_80051BDC);

INCLUDE_ASM("asm/nonmatchings/game", func_80051DBC);

INCLUDE_ASM("asm/nonmatchings/game", func_80052000);

INCLUDE_ASM("asm/nonmatchings/game", func_80052060);

INCLUDE_ASM("asm/nonmatchings/game", func_8005215C);

INCLUDE_ASM("asm/nonmatchings/game", func_80052648);

INCLUDE_ASM("asm/nonmatchings/game", func_80052C88);

INCLUDE_ASM("asm/nonmatchings/game", func_80052D04);

INCLUDE_ASM("asm/nonmatchings/game", func_80052D54);

void func_80052DA4(s32 arg0) {
    func_80052DD4(arg0, D_800E62BF, D_800E62C0);
}

INCLUDE_ASM("asm/nonmatchings/game", func_80052DD4);

INCLUDE_ASM("asm/nonmatchings/game", func_80052E60);

INCLUDE_ASM("asm/nonmatchings/game", func_80052F68);

INCLUDE_ASM("asm/nonmatchings/game", func_80053418);

INCLUDE_ASM("asm/nonmatchings/game", func_8005352C);

INCLUDE_ASM("asm/nonmatchings/game", func_80053564);

INCLUDE_ASM("asm/nonmatchings/game", func_80053650);

INCLUDE_ASM("asm/nonmatchings/game", func_800536AC);

INCLUDE_ASM("asm/nonmatchings/game", func_800537F0);

INCLUDE_ASM("asm/nonmatchings/game", func_80053838);

INCLUDE_ASM("asm/nonmatchings/game", func_8005391C);

INCLUDE_ASM("asm/nonmatchings/game", func_80053A00);

INCLUDE_ASM("asm/nonmatchings/game", func_80053AC8);

INCLUDE_ASM("asm/nonmatchings/game", func_80053B20);

INCLUDE_ASM("asm/nonmatchings/game", func_80053BEC);

INCLUDE_ASM("asm/nonmatchings/game", func_80053C44);

void func_80053CAC(u8 arg0) {
    D_800E7395 = 0;
    D_800E739C = arg0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_80053CC0);

void func_80053CE0(void) {
    D_800E62B5 = 1;
    D_800E739C = 0;
    D_800E7395 = 0;
    D_800E739D = 0;
    D_800E73A0 = 0;
}

void func_80053D10(void) {
    D_800E62B5 = 0;
    D_800E73A0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_80053D24);

INCLUDE_ASM("asm/nonmatchings/game", func_80053DDC);

INCLUDE_ASM("asm/nonmatchings/game", func_80053F04);

INCLUDE_ASM("asm/nonmatchings/game", func_80054108);

INCLUDE_ASM("asm/nonmatchings/game", func_80054140);

INCLUDE_ASM("asm/nonmatchings/game", func_80054284);

INCLUDE_ASM("asm/nonmatchings/game", func_80054388);

INCLUDE_ASM("asm/nonmatchings/game", func_8005448C);

INCLUDE_ASM("asm/nonmatchings/game", func_80054590);

INCLUDE_ASM("asm/nonmatchings/game", func_80054694);

INCLUDE_ASM("asm/nonmatchings/game", func_80054704);

INCLUDE_ASM("asm/nonmatchings/game", func_8005478C);

INCLUDE_ASM("asm/nonmatchings/game", func_80054884);

INCLUDE_ASM("asm/nonmatchings/game", func_80054AF4);

INCLUDE_ASM("asm/nonmatchings/game", func_80054BA8);

INCLUDE_ASM("asm/nonmatchings/game", func_80054BE4);

INCLUDE_ASM("asm/nonmatchings/game", func_80054C4C);

INCLUDE_ASM("asm/nonmatchings/game", func_80054C74);

INCLUDE_ASM("asm/nonmatchings/game", func_80054E6C);

INCLUDE_ASM("asm/nonmatchings/game", func_80054FC0);

INCLUDE_ASM("asm/nonmatchings/game", func_800552DC);

INCLUDE_ASM("asm/nonmatchings/game", func_8005557C);

INCLUDE_ASM("asm/nonmatchings/game", func_80055A38);

INCLUDE_ASM("asm/nonmatchings/game", func_80055AFC);

INCLUDE_ASM("asm/nonmatchings/game", func_80056070);

INCLUDE_ASM("asm/nonmatchings/game", func_80056284);

INCLUDE_ASM("asm/nonmatchings/game", func_80056300);

void func_800563F0(s16 arg0, s16 arg1) {
    D_800B58F8 = arg0;
    D_800B58FC = arg1;
}

INCLUDE_ASM("asm/nonmatchings/game", func_80056414);

INCLUDE_ASM("asm/nonmatchings/game", func_8005649C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005658C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005680C);

INCLUDE_ASM("asm/nonmatchings/game", func_800568B8);

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016. The original keeps the constant 1 in v1 across the
 * loop (uopt hoists integer constants out of loops); the project flag
 * -Wo,-no_const_in_reg stops that. Frame and everything else match. */
void func_80056AA8(SyncObj *arg0, s32 arg1) {
    volatile s32 timeout = 0x800000;

    while (arg0->flag == 0) {
        if (--timeout == 0) {
            arg0->flag = 1;
            if (arg0->idx != 0) {
                arg0->idx = 0;
            } else {
                arg0->idx = 1;
            }
            arg0->unk_24 = arg0->tbl[arg0->idx].unk_00;
            arg0->unk_26 = arg0->tbl[arg0->idx].unk_02;
        }
    }
    arg0->flag = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/game", func_80056AA8);
#endif

INCLUDE_ASM("asm/nonmatchings/game", func_80056B3C);

INCLUDE_ASM("asm/nonmatchings/game", func_80056BA8);

INCLUDE_ASM("asm/nonmatchings/game", func_800570B8);

INCLUDE_ASM("asm/nonmatchings/game", func_8005715C);

void func_80057390(u8 arg0) {
    D_800B593C = arg0;
}

s32 func_8005739C(void) {
    return D_800B5948;
}

INCLUDE_ASM("asm/nonmatchings/game", func_800573AC);

void func_800573F8(s32 arg0) {
    D_800B5938[arg0] = 1 - D_800B5938[arg0];
}

void func_80057418(s32 arg0, s32 arg1) {
    D_800B5938[arg0] = arg1 & 1;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8005742C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005751C);

INCLUDE_ASM("asm/nonmatchings/game", func_80057640);

INCLUDE_ASM("asm/nonmatchings/game", func_80057710);

INCLUDE_ASM("asm/nonmatchings/game", func_800578F4);

void func_80057D1C(u8 arg0) {
    D_800B5940 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_80057D28);

INCLUDE_ASM("asm/nonmatchings/game", func_80058398);

INCLUDE_ASM("asm/nonmatchings/game", func_80058D20);

INCLUDE_ASM("asm/nonmatchings/game", func_80058DA0);

INCLUDE_ASM("asm/nonmatchings/game", func_80058EB0);

INCLUDE_ASM("asm/nonmatchings/game", func_80058F0C);

INCLUDE_ASM("asm/nonmatchings/game", func_80058F9C);

INCLUDE_ASM("asm/nonmatchings/game", func_80059048);

INCLUDE_ASM("asm/nonmatchings/game", func_8005907C);

INCLUDE_ASM("asm/nonmatchings/game", func_800590CC);

INCLUDE_ASM("asm/nonmatchings/game", func_800591D8);

INCLUDE_ASM("asm/nonmatchings/game", func_80059308);

INCLUDE_ASM("asm/nonmatchings/game", func_8005938C);

INCLUDE_ASM("asm/nonmatchings/game", func_80059688);

INCLUDE_ASM("asm/nonmatchings/game", func_80059710);

INCLUDE_ASM("asm/nonmatchings/game", func_800597A0);

INCLUDE_ASM("asm/nonmatchings/game", func_800597B0);

void func_80059808(u16 arg0, u16 arg1) {
    D_800E36E8 = arg0;
    D_800E36EA = arg1;
}

void func_8005981C(s32 arg0, u32 arg1, u32 arg2) {
    D_800E36C8[arg0] = D_800E36E8 * arg1;
    D_800E36D0[arg0] = D_800E36EA * arg2;
}

INCLUDE_ASM("asm/nonmatchings/game", func_80059860);

INCLUDE_ASM("asm/nonmatchings/game", func_80059938);

INCLUDE_ASM("asm/nonmatchings/game", func_80059A20);

INCLUDE_ASM("asm/nonmatchings/game", func_80059B04);

INCLUDE_ASM("asm/nonmatchings/game", func_80059B40);

INCLUDE_ASM("asm/nonmatchings/game", func_80059BC0);

INCLUDE_ASM("asm/nonmatchings/game", func_80059BE8);

INCLUDE_ASM("asm/nonmatchings/game", func_80059E00);

INCLUDE_ASM("asm/nonmatchings/game", func_80059EF0);

INCLUDE_ASM("asm/nonmatchings/game", func_8005A06C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005A0B0);

INCLUDE_ASM("asm/nonmatchings/game", func_8005A1A0);

INCLUDE_ASM("asm/nonmatchings/game", func_8005A2A8);

INCLUDE_ASM("asm/nonmatchings/game", func_8005A37C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005A3E8);

INCLUDE_ASM("asm/nonmatchings/game", func_8005A410);

INCLUDE_ASM("asm/nonmatchings/game", func_8005A560);

INCLUDE_ASM("asm/nonmatchings/game", func_8005A668);

INCLUDE_ASM("asm/nonmatchings/game", func_8005AB4C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005ABD0);

INCLUDE_ASM("asm/nonmatchings/game", func_8005AC70);

INCLUDE_ASM("asm/nonmatchings/game", func_8005AD1C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005AD70);

INCLUDE_ASM("asm/nonmatchings/game", func_8005AE60);

INCLUDE_ASM("asm/nonmatchings/game", func_8005AF28);

INCLUDE_ASM("asm/nonmatchings/game", func_8005AFC8);

INCLUDE_ASM("asm/nonmatchings/game", func_8005B040);

INCLUDE_ASM("asm/nonmatchings/game", func_8005B0F4);

INCLUDE_ASM("asm/nonmatchings/game", func_8005B1A8);

INCLUDE_ASM("asm/nonmatchings/game", func_8005B2BC);

INCLUDE_ASM("asm/nonmatchings/game", func_8005B39C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005B43C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005B798);

INCLUDE_ASM("asm/nonmatchings/game", func_8005B830);

INCLUDE_ASM("asm/nonmatchings/game", func_8005B8A0);

INCLUDE_ASM("asm/nonmatchings/game", func_8005B8E0);

INCLUDE_ASM("asm/nonmatchings/game", func_8005B908);

INCLUDE_ASM("asm/nonmatchings/game", func_8005BAE0);

INCLUDE_ASM("asm/nonmatchings/game", func_8005BB84);

INCLUDE_ASM("asm/nonmatchings/game", func_8005BC38);

INCLUDE_ASM("asm/nonmatchings/game", func_8005BCEC);

INCLUDE_ASM("asm/nonmatchings/game", func_8005BE8C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005C14C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005C414);

INCLUDE_ASM("asm/nonmatchings/game", func_8005C4CC);

INCLUDE_ASM("asm/nonmatchings/game", func_8005C5BC);

INCLUDE_ASM("asm/nonmatchings/game", func_8005C7C0);

INCLUDE_ASM("asm/nonmatchings/game", func_8005C7FC);

INCLUDE_ASM("asm/nonmatchings/game", func_8005C968);

INCLUDE_ASM("asm/nonmatchings/game", func_8005CA94);

INCLUDE_ASM("asm/nonmatchings/game", func_8005CC40);

INCLUDE_ASM("asm/nonmatchings/game", func_8005CF6C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005D174);

INCLUDE_ASM("asm/nonmatchings/game", func_8005D1B0);

INCLUDE_ASM("asm/nonmatchings/game", func_8005D31C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005D448);

INCLUDE_ASM("asm/nonmatchings/game", func_8005D56C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005D804);

INCLUDE_ASM("asm/nonmatchings/game", func_8005D9B4);

INCLUDE_ASM("asm/nonmatchings/game", func_8005DC4C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005DDB4);

INCLUDE_ASM("asm/nonmatchings/game", func_8005DEA0);

INCLUDE_ASM("asm/nonmatchings/game", func_8005E018);

INCLUDE_ASM("asm/nonmatchings/game", func_8005E150);

INCLUDE_ASM("asm/nonmatchings/game", func_8005E2C0);

INCLUDE_ASM("asm/nonmatchings/game", func_8005E40C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005E454);

INCLUDE_ASM("asm/nonmatchings/game", func_8005E5C8);

INCLUDE_ASM("asm/nonmatchings/game", func_8005E7A4);

INCLUDE_ASM("asm/nonmatchings/game", func_8005E7F0);

INCLUDE_ASM("asm/nonmatchings/game", func_8005E924);

INCLUDE_ASM("asm/nonmatchings/game", func_8005E9EC);

INCLUDE_ASM("asm/nonmatchings/game", func_8005EA38);

INCLUDE_ASM("asm/nonmatchings/game", func_8005EB74);

INCLUDE_ASM("asm/nonmatchings/game", func_8005EC78);

INCLUDE_ASM("asm/nonmatchings/game", func_8005ECB8);

INCLUDE_ASM("asm/nonmatchings/game", func_8005EE00);

INCLUDE_ASM("asm/nonmatchings/game", func_8005EF04);

INCLUDE_ASM("asm/nonmatchings/game", func_8005EF44);

INCLUDE_ASM("asm/nonmatchings/game", func_8005F09C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005F22C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005F26C);

INCLUDE_ASM("asm/nonmatchings/game", func_8005F478);

INCLUDE_ASM("asm/nonmatchings/game", func_8005F6E4);

INCLUDE_ASM("asm/nonmatchings/game", func_8005F730);

INCLUDE_ASM("asm/nonmatchings/game", func_8005FA20);

INCLUDE_ASM("asm/nonmatchings/game", func_8005FDA0);

INCLUDE_ASM("asm/nonmatchings/game", func_8005FDEC);

INCLUDE_ASM("asm/nonmatchings/game", func_8005FF68);

INCLUDE_ASM("asm/nonmatchings/game", func_80060104);

INCLUDE_ASM("asm/nonmatchings/game", func_80060150);

INCLUDE_ASM("asm/nonmatchings/game", func_80060178);

INCLUDE_ASM("asm/nonmatchings/game", func_800603C0);

INCLUDE_ASM("asm/nonmatchings/game", func_80060504);

INCLUDE_ASM("asm/nonmatchings/game", func_8006052C);

INCLUDE_ASM("asm/nonmatchings/game", func_800605D4);

INCLUDE_ASM("asm/nonmatchings/game", func_80060614);

INCLUDE_ASM("asm/nonmatchings/game", func_80060710);

INCLUDE_ASM("asm/nonmatchings/game", func_80060930);

INCLUDE_ASM("asm/nonmatchings/game", func_80060958);

INCLUDE_ASM("asm/nonmatchings/game", func_80060A84);

INCLUDE_ASM("asm/nonmatchings/game", func_80060B24);

INCLUDE_ASM("asm/nonmatchings/game", func_80060B78);

INCLUDE_ASM("asm/nonmatchings/game", func_80060BC8);

INCLUDE_ASM("asm/nonmatchings/game", func_80060BE8);

INCLUDE_ASM("asm/nonmatchings/game", func_80060C3C);

INCLUDE_ASM("asm/nonmatchings/game", func_80060CC4);

INCLUDE_ASM("asm/nonmatchings/game", func_80060DA4);

INCLUDE_ASM("asm/nonmatchings/game", func_80060DF8);

INCLUDE_ASM("asm/nonmatchings/game", func_80060EA0);

INCLUDE_ASM("asm/nonmatchings/game", func_80061044);

INCLUDE_ASM("asm/nonmatchings/game", func_80061118);

INCLUDE_ASM("asm/nonmatchings/game", func_800611A0);

INCLUDE_ASM("asm/nonmatchings/game", func_800611F0);

INCLUDE_ASM("asm/nonmatchings/game", func_80061250);

INCLUDE_ASM("asm/nonmatchings/game", func_800612E8);

INCLUDE_ASM("asm/nonmatchings/game", func_80061498);

INCLUDE_ASM("asm/nonmatchings/game", func_80061634);

INCLUDE_ASM("asm/nonmatchings/game", func_80061688);

INCLUDE_ASM("asm/nonmatchings/game", func_80061710);

INCLUDE_ASM("asm/nonmatchings/game", func_80061790);

INCLUDE_ASM("asm/nonmatchings/game", func_800618B0);

INCLUDE_ASM("asm/nonmatchings/game", func_8006190C);

INCLUDE_ASM("asm/nonmatchings/game", func_80061A3C);

INCLUDE_ASM("asm/nonmatchings/game", func_80061EFC);

INCLUDE_ASM("asm/nonmatchings/game", func_80061FC4);

INCLUDE_ASM("asm/nonmatchings/game", func_8006211C);

INCLUDE_ASM("asm/nonmatchings/game", func_8006218C);

INCLUDE_ASM("asm/nonmatchings/game", func_80062210);

INCLUDE_ASM("asm/nonmatchings/game", func_800623E4);

INCLUDE_ASM("asm/nonmatchings/game", func_80062524);

void func_800625C0(void) {
    u8 *p = D_8011ECD0 + D_801230D0 * 0x44;

    p[0x19C7] |= 0x40;
}

INCLUDE_ASM("asm/nonmatchings/game", func_800625F4);

INCLUDE_ASM("asm/nonmatchings/game", func_80062634);

INCLUDE_ASM("asm/nonmatchings/game", func_800626B0);

INCLUDE_ASM("asm/nonmatchings/game", func_80062764);

INCLUDE_ASM("asm/nonmatchings/game", func_800627DC);

INCLUDE_ASM("asm/nonmatchings/game", func_80062840);

INCLUDE_ASM("asm/nonmatchings/game", func_80062948);

INCLUDE_ASM("asm/nonmatchings/game", func_80062C28);

INCLUDE_ASM("asm/nonmatchings/game", func_80062CD0);

INCLUDE_ASM("asm/nonmatchings/game", func_80062D0C);

INCLUDE_ASM("asm/nonmatchings/game", func_80062DBC);

INCLUDE_ASM("asm/nonmatchings/game", func_800634FC);

INCLUDE_ASM("asm/nonmatchings/game", func_80063520);

INCLUDE_ASM("asm/nonmatchings/game", func_80063668);

INCLUDE_ASM("asm/nonmatchings/game", func_80063890);

INCLUDE_ASM("asm/nonmatchings/game", func_800638C4);

INCLUDE_ASM("asm/nonmatchings/game", func_800638E8);

INCLUDE_ASM("asm/nonmatchings/game", func_80063930);

INCLUDE_ASM("asm/nonmatchings/game", func_800639D8);

INCLUDE_ASM("asm/nonmatchings/game", func_800646CC);

INCLUDE_ASM("asm/nonmatchings/game", func_8006492C);

INCLUDE_ASM("asm/nonmatchings/game", func_800649D4);

INCLUDE_ASM("asm/nonmatchings/game", func_80064DEC);

INCLUDE_ASM("asm/nonmatchings/game", func_80064E48);

INCLUDE_ASM("asm/nonmatchings/game", func_80064E84);

INCLUDE_ASM("asm/nonmatchings/game", func_80064F48);

INCLUDE_ASM("asm/nonmatchings/game", func_80064FA4);

INCLUDE_ASM("asm/nonmatchings/game", func_80065048);

s32 func_80065074(s32 arg0) {
    if (D_80125C90[arg0] == 1) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8006509C);

INCLUDE_ASM("asm/nonmatchings/game", func_800654D0);

INCLUDE_ASM("asm/nonmatchings/game", func_80065900);

INCLUDE_ASM("asm/nonmatchings/game", func_80065964);

INCLUDE_ASM("asm/nonmatchings/game", func_80065B0C);

INCLUDE_ASM("asm/nonmatchings/game", func_80065F34);

INCLUDE_ASM("asm/nonmatchings/game", func_80066104);

INCLUDE_ASM("asm/nonmatchings/game", func_8006612C);

INCLUDE_ASM("asm/nonmatchings/game", func_80066334);

INCLUDE_ASM("asm/nonmatchings/game", func_80066A2C);

s32 func_80066A68(void) {
    return 1;
}

s32 func_80066A70(void) {
    return 2;
}

INCLUDE_ASM("asm/nonmatchings/game", func_80066A78);

INCLUDE_ASM("asm/nonmatchings/game", func_80066A84);

INCLUDE_ASM("asm/nonmatchings/game", func_80066AC0);

INCLUDE_ASM("asm/nonmatchings/game", func_80066ACC);

INCLUDE_ASM("asm/nonmatchings/game", func_80066B40);

INCLUDE_ASM("asm/nonmatchings/game", func_80066C08);

INCLUDE_ASM("asm/nonmatchings/game", func_800673B8);

INCLUDE_ASM("asm/nonmatchings/game", func_80067438);

INCLUDE_ASM("asm/nonmatchings/game", func_800674B0);

INCLUDE_ASM("asm/nonmatchings/game", func_80067610);

INCLUDE_ASM("asm/nonmatchings/game", func_8006764C);

INCLUDE_ASM("asm/nonmatchings/game", func_800676AC);

INCLUDE_ASM("asm/nonmatchings/game", func_80067870);

INCLUDE_ASM("asm/nonmatchings/game", func_80067C28);

INCLUDE_ASM("asm/nonmatchings/game", func_80067DD4);

INCLUDE_ASM("asm/nonmatchings/game", func_80067DFC);

INCLUDE_ASM("asm/nonmatchings/game", func_80067E34);

INCLUDE_ASM("asm/nonmatchings/game", func_80067F04);

INCLUDE_ASM("asm/nonmatchings/game", func_80068898);

INCLUDE_ASM("asm/nonmatchings/game", func_800688F0);

INCLUDE_ASM("asm/nonmatchings/game", func_80068938);

INCLUDE_ASM("asm/nonmatchings/game", func_80068BE4);

INCLUDE_ASM("asm/nonmatchings/game", func_80068EC0);

INCLUDE_ASM("asm/nonmatchings/game", func_80069128);

INCLUDE_ASM("asm/nonmatchings/game", func_80069178);

INCLUDE_ASM("asm/nonmatchings/game", func_800691C8);

INCLUDE_ASM("asm/nonmatchings/game", func_80069218);

INCLUDE_ASM("asm/nonmatchings/game", func_800692D8);

INCLUDE_ASM("asm/nonmatchings/game", func_80069614);

INCLUDE_ASM("asm/nonmatchings/game", func_800696DC);

INCLUDE_ASM("asm/nonmatchings/game", func_8006A044);

INCLUDE_ASM("asm/nonmatchings/game", func_8006A2CC);

INCLUDE_ASM("asm/nonmatchings/game", func_8006AEC4);

INCLUDE_ASM("asm/nonmatchings/game", func_8006B014);

INCLUDE_ASM("asm/nonmatchings/game", func_8006B0C8);

INCLUDE_ASM("asm/nonmatchings/game", func_8006B5BC);

void func_8006B640(void) {
}

INCLUDE_ASM("asm/nonmatchings/game", func_8006B648);

INCLUDE_ASM("asm/nonmatchings/game", func_8006B900);

INCLUDE_ASM("asm/nonmatchings/game", func_8006BA40);

INCLUDE_ASM("asm/nonmatchings/game", func_8006BC28);

INCLUDE_ASM("asm/nonmatchings/game", func_8006BD6C);

INCLUDE_ASM("asm/nonmatchings/game", func_8006BDA8);

INCLUDE_ASM("asm/nonmatchings/game", func_8006C308);

INCLUDE_ASM("asm/nonmatchings/game", func_8006C334);

INCLUDE_ASM("asm/nonmatchings/game", func_8006C700);

INCLUDE_ASM("asm/nonmatchings/game", func_8006C760);

INCLUDE_ASM("asm/nonmatchings/game", func_8006C848);

INCLUDE_ASM("asm/nonmatchings/game", func_8006C934);

INCLUDE_ASM("asm/nonmatchings/game", func_8006C988);

u8 *func_8006CA9C(void) {
    if (D_800E71F4 == 0) {
        if (D_800E71F5 != 0) {
            return D_800B672C;
        }
        return D_800B6730;
    }
    return D_800B6724;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8006CAE0);

INCLUDE_ASM("asm/nonmatchings/game", func_8006CB30);

INCLUDE_ASM("asm/nonmatchings/game", func_8006CB6C);

INCLUDE_ASM("asm/nonmatchings/game", func_8006CC28);

void func_8006CCE4(void) {
    if (func_8004480C() == 0) {
        func_8004500C(0, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/game", func_8006CD14);

INCLUDE_ASM("asm/nonmatchings/game", func_8006CDD4);

INCLUDE_ASM("asm/nonmatchings/game", func_8006CE84);

INCLUDE_ASM("asm/nonmatchings/game", func_8006CF38);

INCLUDE_ASM("asm/nonmatchings/game", func_8006CF80);

INCLUDE_ASM("asm/nonmatchings/game", func_8006D00C);

INCLUDE_ASM("asm/nonmatchings/game", func_8006D038);

INCLUDE_ASM("asm/nonmatchings/game", func_8006D138);

INCLUDE_ASM("asm/nonmatchings/game", func_8006D4A0);

INCLUDE_ASM("asm/nonmatchings/game", func_8006D52C);

INCLUDE_ASM("asm/nonmatchings/game", func_8006D6E0);

INCLUDE_ASM("asm/nonmatchings/game", func_8006D7EC);

INCLUDE_ASM("asm/nonmatchings/game", func_8006EA40);

INCLUDE_ASM("asm/nonmatchings/game", func_8006EB64);

INCLUDE_ASM("asm/nonmatchings/game", func_8006EEC4);

INCLUDE_ASM("asm/nonmatchings/game", func_8006F218);

INCLUDE_ASM("asm/nonmatchings/game", func_8006F524);

INCLUDE_ASM("asm/nonmatchings/game", func_8006FB4C);

INCLUDE_ASM("asm/nonmatchings/game", func_80070694);

INCLUDE_ASM("asm/nonmatchings/game", func_80070850);

INCLUDE_ASM("asm/nonmatchings/game", func_80070AD4);

INCLUDE_ASM("asm/nonmatchings/game", func_80070C44);

INCLUDE_ASM("asm/nonmatchings/game", func_80070E10);

INCLUDE_ASM("asm/nonmatchings/game", func_80070E90);

INCLUDE_ASM("asm/nonmatchings/game", func_80070ECC);

INCLUDE_ASM("asm/nonmatchings/game", func_80070F14);

INCLUDE_ASM("asm/nonmatchings/game", func_80070F80);

INCLUDE_ASM("asm/nonmatchings/game", func_80070FD0);

INCLUDE_ASM("asm/nonmatchings/game", func_80071038);

INCLUDE_ASM("asm/nonmatchings/game", func_80071110);

INCLUDE_ASM("asm/nonmatchings/game", func_80071280);

INCLUDE_ASM("asm/nonmatchings/game", func_8007132C);

INCLUDE_ASM("asm/nonmatchings/game", func_800722C4);

INCLUDE_ASM("asm/nonmatchings/game", func_80072338);

void func_8007259C(u8 arg0) {
    D_800E699E = arg0;
    D_800E699D = arg0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_800725B0);

INCLUDE_ASM("asm/nonmatchings/game", func_800726F0);

INCLUDE_ASM("asm/nonmatchings/game", func_800728A4);

INCLUDE_ASM("asm/nonmatchings/game", func_80072944);

INCLUDE_ASM("asm/nonmatchings/game", func_80072998);

INCLUDE_ASM("asm/nonmatchings/game", func_80072B20);

INCLUDE_ASM("asm/nonmatchings/game", func_80072B5C);

INCLUDE_ASM("asm/nonmatchings/game", func_80072BC0);

INCLUDE_ASM("asm/nonmatchings/game", func_80072C68);

INCLUDE_ASM("asm/nonmatchings/game", func_80072CA0);

INCLUDE_ASM("asm/nonmatchings/game", func_80072D70);

INCLUDE_ASM("asm/nonmatchings/game", func_80073198);

INCLUDE_ASM("asm/nonmatchings/game", func_800732F8);

INCLUDE_ASM("asm/nonmatchings/game", func_800733D8);

INCLUDE_ASM("asm/nonmatchings/game", func_800734C4);

INCLUDE_ASM("asm/nonmatchings/game", func_800735B8);

INCLUDE_ASM("asm/nonmatchings/game", func_800736AC);

INCLUDE_ASM("asm/nonmatchings/game", func_800737A0);

INCLUDE_ASM("asm/nonmatchings/game", func_80073800);

INCLUDE_ASM("asm/nonmatchings/game", func_800738E0);

INCLUDE_ASM("asm/nonmatchings/game", func_80073920);

INCLUDE_ASM("asm/nonmatchings/game", func_800739A0);

INCLUDE_ASM("asm/nonmatchings/game", func_800739E0);

INCLUDE_ASM("asm/nonmatchings/game", func_80073A40);

INCLUDE_ASM("asm/nonmatchings/game", func_80073AD8);

INCLUDE_ASM("asm/nonmatchings/game", func_80073CB0);

INCLUDE_ASM("asm/nonmatchings/game", func_800741B8);

INCLUDE_ASM("asm/nonmatchings/game", func_8007437C);

INCLUDE_ASM("asm/nonmatchings/game", func_80074658);

INCLUDE_ASM("asm/nonmatchings/game", func_80074798);

INCLUDE_ASM("asm/nonmatchings/game", func_800747DC);

INCLUDE_ASM("asm/nonmatchings/game", func_80074CA8);

INCLUDE_ASM("asm/nonmatchings/game", func_80074D28);

INCLUDE_ASM("asm/nonmatchings/game", func_80074DE0);

INCLUDE_ASM("asm/nonmatchings/game", func_80074F24);

INCLUDE_ASM("asm/nonmatchings/game", func_8007505C);

INCLUDE_ASM("asm/nonmatchings/game", func_80075320);

INCLUDE_ASM("asm/nonmatchings/game", func_80075468);

INCLUDE_ASM("asm/nonmatchings/game", func_800754C0);

INCLUDE_ASM("asm/nonmatchings/game", func_800755B4);

INCLUDE_ASM("asm/nonmatchings/game", func_80075A64);

INCLUDE_ASM("asm/nonmatchings/game", func_80075AF8);

INCLUDE_ASM("asm/nonmatchings/game", func_80075BE8);

INCLUDE_ASM("asm/nonmatchings/game", func_80075C24);

INCLUDE_ASM("asm/nonmatchings/game", func_80075C84);

INCLUDE_ASM("asm/nonmatchings/game", func_80075FA0);

INCLUDE_ASM("asm/nonmatchings/game", func_80076000);

INCLUDE_ASM("asm/nonmatchings/game", func_800764AC);

INCLUDE_ASM("asm/nonmatchings/game", func_80076630);

INCLUDE_ASM("asm/nonmatchings/game", func_800767A4);

INCLUDE_ASM("asm/nonmatchings/game", func_80076B48);

INCLUDE_ASM("asm/nonmatchings/game", func_80076C7C);

INCLUDE_ASM("asm/nonmatchings/game", func_80076DA0);

INCLUDE_ASM("asm/nonmatchings/game", func_80076EC0);

INCLUDE_ASM("asm/nonmatchings/game", func_80076F48);

INCLUDE_ASM("asm/nonmatchings/game", func_80076FDC);

INCLUDE_ASM("asm/nonmatchings/game", func_80077330);

INCLUDE_ASM("asm/nonmatchings/game", func_8007739C);

INCLUDE_ASM("asm/nonmatchings/game", func_80077694);

INCLUDE_ASM("asm/nonmatchings/game", func_80077900);

INCLUDE_ASM("asm/nonmatchings/game", func_80077BE0);

INCLUDE_ASM("asm/nonmatchings/game", func_80077C50);

INCLUDE_ASM("asm/nonmatchings/game", func_80077CA8);

INCLUDE_ASM("asm/nonmatchings/game", func_80077E30);

void func_80077F2C(void) {
    func_8006D138();
    func_8006492C(1);
    func_80042908(0);
}

INCLUDE_ASM("asm/nonmatchings/game", func_80077F5C);

INCLUDE_ASM("asm/nonmatchings/game", func_80077FE0);

INCLUDE_ASM("asm/nonmatchings/game", func_80078058);

INCLUDE_ASM("asm/nonmatchings/game", func_80078148);

INCLUDE_ASM("asm/nonmatchings/game", func_800782F0);

INCLUDE_ASM("asm/nonmatchings/game", func_80078604);

INCLUDE_ASM("asm/nonmatchings/game", func_8007866C);

INCLUDE_ASM("asm/nonmatchings/game", func_800787B4);

INCLUDE_ASM("asm/nonmatchings/game", func_80078858);

INCLUDE_ASM("asm/nonmatchings/game", func_80078898);

INCLUDE_ASM("asm/nonmatchings/game", func_8007891C);

INCLUDE_ASM("asm/nonmatchings/game", func_8007894C);

void func_800789E0(void) {
    D_800B6D30 = 0;
    D_800B6D34 = 0;
    D_800B6D38 = 0;
    D_800B6D3C = 0;
    D_800B6D40 = 0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_80078A0C);

INCLUDE_ASM("asm/nonmatchings/game", func_80078A94);

INCLUDE_ASM("asm/nonmatchings/game", func_80078C48);

INCLUDE_ASM("asm/nonmatchings/game", func_80078FE0);

INCLUDE_ASM("asm/nonmatchings/game", func_80079014);

INCLUDE_ASM("asm/nonmatchings/game", func_80079070);

s32 func_80079524(void) {
    if (D_800E7388 == 0xD0 || D_800E7388 == 0xD1) {
        return 1;
    }
    return 0;
}

s32 func_80079554(void) {
    if (D_800E7388 == 0x90) {
        return 1;
    }
    return 0;
}

s32 func_80079578(void) {
    if (D_800E7388 == 0x21) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8007959C);

INCLUDE_ASM("asm/nonmatchings/game", func_80079800);

INCLUDE_ASM("asm/nonmatchings/game", func_80079B10);

INCLUDE_ASM("asm/nonmatchings/game", func_80079B34);

INCLUDE_ASM("asm/nonmatchings/game", func_80079C70);

INCLUDE_ASM("asm/nonmatchings/game", func_80079D28);

INCLUDE_ASM("asm/nonmatchings/game", func_80079E00);

INCLUDE_ASM("asm/nonmatchings/game", func_80079E4C);

INCLUDE_ASM("asm/nonmatchings/game", func_80079E9C);

INCLUDE_ASM("asm/nonmatchings/game", func_80079F00);

INCLUDE_ASM("asm/nonmatchings/game", func_8007A008);

INCLUDE_ASM("asm/nonmatchings/game", func_8007A080);

INCLUDE_ASM("asm/nonmatchings/game", func_8007A254);

INCLUDE_ASM("asm/nonmatchings/game", func_8007A354);

INCLUDE_ASM("asm/nonmatchings/game", func_8007A43C);

INCLUDE_ASM("asm/nonmatchings/game", func_8007A4DC);

INCLUDE_ASM("asm/nonmatchings/game", func_8007A50C);

INCLUDE_ASM("asm/nonmatchings/game", func_8007A5BC);

INCLUDE_ASM("asm/nonmatchings/game", func_8007A618);

INCLUDE_ASM("asm/nonmatchings/game", func_8007A6AC);

INCLUDE_ASM("asm/nonmatchings/game", func_8007A868);

INCLUDE_ASM("asm/nonmatchings/game", func_8007A924);

INCLUDE_ASM("asm/nonmatchings/game", func_8007A98C);

INCLUDE_ASM("asm/nonmatchings/game", func_8007AB24);

INCLUDE_ASM("asm/nonmatchings/game", func_8007ABE0);

INCLUDE_ASM("asm/nonmatchings/game", func_8007AD6C);

INCLUDE_ASM("asm/nonmatchings/game", func_8007ADD8);

INCLUDE_ASM("asm/nonmatchings/game", func_8007AEC0);

INCLUDE_ASM("asm/nonmatchings/game", func_8007AF0C);

INCLUDE_ASM("asm/nonmatchings/game", func_8007B144);

INCLUDE_ASM("asm/nonmatchings/game", func_8007B2B4);

INCLUDE_ASM("asm/nonmatchings/game", func_8007B358);

INCLUDE_ASM("asm/nonmatchings/game", func_8007B3DC);

INCLUDE_ASM("asm/nonmatchings/game", func_8007B460);

INCLUDE_ASM("asm/nonmatchings/game", func_8007B4E4);

INCLUDE_ASM("asm/nonmatchings/game", func_8007B568);

void func_8007B5CC(void) {
    func_80090D20();
}

INCLUDE_ASM("asm/nonmatchings/game", func_8007B5EC);

void func_8007B640(void) {
    D_80125CC0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8007B64C);

INCLUDE_ASM("asm/nonmatchings/game", func_8007B6E0);

INCLUDE_ASM("asm/nonmatchings/game", func_8007B734);

INCLUDE_ASM("asm/nonmatchings/game", func_8007B7E0);

INCLUDE_ASM("asm/nonmatchings/game", func_8007B844);

INCLUDE_ASM("asm/nonmatchings/game", func_8007B8F8);

INCLUDE_ASM("asm/nonmatchings/game", func_8007B99C);

INCLUDE_ASM("asm/nonmatchings/game", func_8007BA54);

INCLUDE_ASM("asm/nonmatchings/game", func_8007BAAC);

INCLUDE_ASM("asm/nonmatchings/game", func_8007BBE8);

INCLUDE_ASM("asm/nonmatchings/game", func_8007BCFC);

INCLUDE_ASM("asm/nonmatchings/game", func_8007BDE8);

INCLUDE_ASM("asm/nonmatchings/game", func_8007BE94);

INCLUDE_ASM("asm/nonmatchings/game", func_8007BF04);

void func_8007BFA8(void) {
    D_80125D3C = -1;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8007BFB8);

void func_8007BFF8(void) {
    D_80125E60 = 0;
    D_80125E62 = 0;
    D_80125E64 = 8;
    D_80125E66 = 0x20;
    D_80125E68 = 0x212;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8007C030);

INCLUDE_ASM("asm/nonmatchings/game", func_8007C310);

INCLUDE_ASM("asm/nonmatchings/game", func_8007C414);

s16 func_8007C51C(void) {
    if (D_80125D10 & 0x400) {
        return (u32)(D_80125E60 + D_80125E62) >> 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8007C560);

INCLUDE_ASM("asm/nonmatchings/game", func_8007C5CC);

INCLUDE_ASM("asm/nonmatchings/game", func_8007C638);

INCLUDE_ASM("asm/nonmatchings/game", func_8007C6A8);

void func_8007C740(void) {
    D_800CA120 = 0x80180084;
    D_800CA124 = 0x80180098;
    D_800CA128 = 0x8018009C;
    D_800CA130 = 0x801800AC;
}

INCLUDE_ASM("asm/nonmatchings/game", func_8007C784);

INCLUDE_ASM("asm/nonmatchings/game", func_8007C844);

INCLUDE_ASM("asm/nonmatchings/game", func_8007C878);

INCLUDE_ASM("asm/nonmatchings/game", func_8007C8A4);

INCLUDE_ASM("asm/nonmatchings/game", func_8007C8D8);

INCLUDE_ASM("asm/nonmatchings/game", func_8007D384);

INCLUDE_ASM("asm/nonmatchings/game", func_8007D680);

INCLUDE_ASM("asm/nonmatchings/game", func_8007D788);

INCLUDE_ASM("asm/nonmatchings/game", func_8007D878);

INCLUDE_ASM("asm/nonmatchings/game", func_8007D8AC);

INCLUDE_ASM("asm/nonmatchings/game", func_8007E390);

INCLUDE_ASM("asm/nonmatchings/game", func_8007E3F8);

INCLUDE_ASM("asm/nonmatchings/game", func_8007E468);

INCLUDE_ASM("asm/nonmatchings/game", func_8007E530);

INCLUDE_ASM("asm/nonmatchings/game", func_8007E5A0);

INCLUDE_ASM("asm/nonmatchings/game", func_8007E608);

INCLUDE_ASM("asm/nonmatchings/game", func_8007E634);

INCLUDE_ASM("asm/nonmatchings/game", func_8007E75C);

INCLUDE_ASM("asm/nonmatchings/game", func_8007E81C);

INCLUDE_ASM("asm/nonmatchings/game", func_8007E884);

INCLUDE_ASM("asm/nonmatchings/game", func_8007E934);

INCLUDE_ASM("asm/nonmatchings/game", func_8007E99C);

INCLUDE_ASM("asm/nonmatchings/game", func_8007EAAC);

INCLUDE_ASM("asm/nonmatchings/game", func_8007EB0C);

INCLUDE_ASM("asm/nonmatchings/game", func_8007EB5C);

INCLUDE_ASM("asm/nonmatchings/game", func_8007EC68);

INCLUDE_ASM("asm/nonmatchings/game", func_8007EC9C);

void func_8007ECD0(void) {
    func_800573F8(1);
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/game", func_8007ECF8);

INCLUDE_ASM("asm/nonmatchings/game", func_8007ED84);

INCLUDE_ASM("asm/nonmatchings/game", func_8007EDF8);

INCLUDE_ASM("asm/nonmatchings/game", func_8007EF04);

INCLUDE_ASM("asm/nonmatchings/game", func_8007EF48);

INCLUDE_ASM("asm/nonmatchings/game", func_80081128);

INCLUDE_ASM("asm/nonmatchings/game", func_80081190);

INCLUDE_ASM("asm/nonmatchings/game", func_80082764);

INCLUDE_ASM("asm/nonmatchings/game", func_80082D58);

INCLUDE_ASM("asm/nonmatchings/game", func_80082EB4);

INCLUDE_ASM("asm/nonmatchings/game", func_80082F70);

INCLUDE_ASM("asm/nonmatchings/game", func_80082FD0);

INCLUDE_ASM("asm/nonmatchings/game", func_80083078);

INCLUDE_ASM("asm/nonmatchings/game", func_80083174);

INCLUDE_ASM("asm/nonmatchings/game", func_800831D4);

INCLUDE_ASM("asm/nonmatchings/game", func_80083268);

INCLUDE_ASM("asm/nonmatchings/game", func_80083338);

void func_80083378(void) {
    func_80083440(0);
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/game", func_800833A0);

INCLUDE_ASM("asm/nonmatchings/game", func_800833C8);

INCLUDE_ASM("asm/nonmatchings/game", func_800833F0);

INCLUDE_ASM("asm/nonmatchings/game", func_80083418);

INCLUDE_ASM("asm/nonmatchings/game", func_80083440);

INCLUDE_ASM("asm/nonmatchings/game", func_80083474);

INCLUDE_ASM("asm/nonmatchings/game", func_80083628);

INCLUDE_ASM("asm/nonmatchings/game", func_800836A0);

INCLUDE_ASM("asm/nonmatchings/game", func_80083808);

INCLUDE_ASM("asm/nonmatchings/game", func_80083A10);

INCLUDE_ASM("asm/nonmatchings/game", func_80083AD4);

INCLUDE_ASM("asm/nonmatchings/game", func_80083AFC);

INCLUDE_ASM("asm/nonmatchings/game", func_80083B24);

INCLUDE_ASM("asm/nonmatchings/game", func_80084150);

INCLUDE_ASM("asm/nonmatchings/game", func_80084524);

INCLUDE_ASM("asm/nonmatchings/game", func_80084658);

INCLUDE_ASM("asm/nonmatchings/game", func_80084678);

INCLUDE_ASM("asm/nonmatchings/game", func_800846C0);

INCLUDE_ASM("asm/nonmatchings/game", func_80084738);

INCLUDE_ASM("asm/nonmatchings/game", func_80084764);

INCLUDE_ASM("asm/nonmatchings/game", func_8008478C);

INCLUDE_ASM("asm/nonmatchings/game", func_800847B8);

INCLUDE_ASM("asm/nonmatchings/game", func_8008484C);

INCLUDE_ASM("asm/nonmatchings/game", func_800848AC);

INCLUDE_ASM("asm/nonmatchings/game", func_8008490C);

INCLUDE_ASM("asm/nonmatchings/game", func_80084B9C);

INCLUDE_ASM("asm/nonmatchings/game", func_80084C98);

INCLUDE_ASM("asm/nonmatchings/game", func_80084CC8);

INCLUDE_ASM("asm/nonmatchings/game", func_80084D3C);

INCLUDE_ASM("asm/nonmatchings/game", func_80084E4C);

INCLUDE_ASM("asm/nonmatchings/game", func_80084E6C);

INCLUDE_ASM("asm/nonmatchings/game", func_80084E90);

INCLUDE_ASM("asm/nonmatchings/game", func_800850D4);

INCLUDE_ASM("asm/nonmatchings/game", func_80085234);

INCLUDE_ASM("asm/nonmatchings/game", func_80085368);

INCLUDE_ASM("asm/nonmatchings/game", func_800853FC);

INCLUDE_ASM("asm/nonmatchings/game", func_80085498);

INCLUDE_ASM("asm/nonmatchings/game", func_800854C8);

INCLUDE_ASM("asm/nonmatchings/game", func_800854FC);

INCLUDE_ASM("asm/nonmatchings/game", func_80085530);

INCLUDE_ASM("asm/nonmatchings/game", func_80085564);

INCLUDE_ASM("asm/nonmatchings/game", func_8008558C);

INCLUDE_ASM("asm/nonmatchings/game", func_800855B4);

INCLUDE_ASM("asm/nonmatchings/game", func_800855E4);

INCLUDE_ASM("asm/nonmatchings/game", func_80085614);

INCLUDE_ASM("asm/nonmatchings/game", func_80085644);

INCLUDE_ASM("asm/nonmatchings/game", func_80085678);

INCLUDE_ASM("asm/nonmatchings/game", func_8008585C);

INCLUDE_ASM("asm/nonmatchings/game", func_800858E8);

INCLUDE_ASM("asm/nonmatchings/game", func_80085910);

INCLUDE_ASM("asm/nonmatchings/game", func_80085958);

INCLUDE_ASM("asm/nonmatchings/game", func_800859E8);

INCLUDE_ASM("asm/nonmatchings/game", func_80085A60);

INCLUDE_ASM("asm/nonmatchings/game", func_80085ADC);

INCLUDE_ASM("asm/nonmatchings/game", func_80085B3C);

INCLUDE_ASM("asm/nonmatchings/game", func_80085CD4);

INCLUDE_ASM("asm/nonmatchings/game", func_80085E30);

INCLUDE_ASM("asm/nonmatchings/game", func_80085ED0);

INCLUDE_ASM("asm/nonmatchings/game", func_80085EEC);

INCLUDE_ASM("asm/nonmatchings/game", func_80085F0C);

INCLUDE_ASM("asm/nonmatchings/game", func_80086204);

INCLUDE_ASM("asm/nonmatchings/game", func_800862D0);

INCLUDE_ASM("asm/nonmatchings/game", func_800862F0);

INCLUDE_ASM("asm/nonmatchings/game", func_80086350);

INCLUDE_ASM("asm/nonmatchings/game", func_80086370);

INCLUDE_ASM("asm/nonmatchings/game", func_800863D4);

INCLUDE_ASM("asm/nonmatchings/game", func_80086424);

INCLUDE_ASM("asm/nonmatchings/game", func_8008647C);

INCLUDE_ASM("asm/nonmatchings/game", func_80086640);

INCLUDE_ASM("asm/nonmatchings/game", func_8008667C);

INCLUDE_ASM("asm/nonmatchings/game", func_800866A0);

INCLUDE_ASM("asm/nonmatchings/game", func_800867CC);
