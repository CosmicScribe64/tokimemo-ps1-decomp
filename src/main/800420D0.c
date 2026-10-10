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

INCLUDE_ASM("asm/nonmatchings/main/800420D0", func_80042134);

INCLUDE_ASM("asm/nonmatchings/main/800420D0", func_800422C8);

void func_800423D4(void) {
    D_800E7374 += 1;
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
