#include "common.h"
#include "game.h"
#include "libapi.h"

void func_800420D0(void) {
    D_80125124 = SetSp(0x801FEFF0);
    InitHeap(0x801FF800, 0x7F0);
    bzero((void *)0x800E60A0, 0x45498);
    D_80125120 = GetGp();
    func_80042134();
}

void func_80042134(void) {
    GameState *g = &D_800E6280;
    ResetCallback();
    func_80087730();
    printf("TOKIMEKI MEMORIAL : PSX\n");
    func_80042418();
    func_8009C210(0);
    func_8004111C();
loop_1:
    func_8007B844();
    func_800452C4();
    func_80041000();
    func_800412E0(1);
    if (D_800E6280.unk_110F != 0) {
        goto loop_1;
    }
    do {
        func_800422C8();
        if (D_800E6280.unk_1110 == 0) {
            func_80042540();
        } else {
            func_80043504();
        }
        D_800E6280.unk_10F8 += 1;
        if (D_800B3220 != 0) {
            func_80059A20();
            if ((D_800E6280.unk_F80 & 0xC0000) == 0xC0000) {
                func_8004F6B0();
            }
            if (D_800E6280.unk_F88 & 0xA0000) {
                func_8004AD60(1);
            }
        }
        if (D_800E6280.unk_110F == 0) {
            func_80043510(1);
            func_800455C4();
            if (D_800E6280.unk_034 != 0) {
                func_80079B34();
            }
        }
    } while (g->unk_110F == 0);
    goto loop_1;
}

void func_800422C8(void) {
    D_8011ECA0 = func_80098370();
    if (D_800E6280.unk_1108 == 0x90) {
        func_800985E0((s32) D_800FD4A0 + D_8011ECA0 * 0x10C00);
        SetWorkBase((s32) D_800E94A0 + D_8011ECA0 * 0xA000, D_8011ECA0);
    } else {
        func_800985E0((s32) D_800E94A0 + D_8011ECA0 * 0xA000);
        SetWorkBase((s32) D_800FD4A0 + D_8011ECA0 * 0x10C00, D_8011ECA0);
    }
    func_800985F0(0, 0, (s32) &D_800E8C70 + D_8011ECA0 * 0x14);
    D_8011ECA4 = 0;
}

void func_800423D4(void) {
    D_800E6280.unk_10F4 += 1;
    D_800E7D10 += 0x377;
}

s32 func_80042400(void) {
    D_800E7D10 += 0x377;
    return D_800E7D10;
}

void func_80042418(void) {
    bzero(&D_800E7D10, 0xEE0);
    func_80042488();
    card_ev_set();
    func_80042C30();
}

void func_80042458(void) {
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
}

void func_80042488(void) {
    s32 ev;

    EnterCriticalSection();
    ev = OpenEvent(0xF2000003, 2, 0x1000, func_800423D4);
    D_8011ECA8 = ev;
    EnableEvent(ev);
    SetRCnt(0xF2000003, 1, 0x1000);
    StartRCnt(0xF2000003);
    ExitCriticalSection();
}

void func_800424FC(void) {
    EnterCriticalSection();
    CloseEvent(D_8011ECA8);
    ExitCriticalSection();
}
