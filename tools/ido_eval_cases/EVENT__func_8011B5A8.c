#include "common.h"
#include "ovl/EVENT.h"

extern s8 D_800E9E63;
extern s8 D_800EC1A4;
extern s8 D_800EC1A5;
extern s8 D_800EC1A6;
extern s8 D_800EC1C8;
extern s8 D_800EC1C9;
extern s8 D_800EC1CA;
extern s32 D_800EECD0;

void func_8011B5A8(void) {
    D_800EECD0 = 1;
    D_800EC1A6 = 0x80;
    D_800EC1A5 = 0x80;
    D_800EC1A4 = 0x80;
    D_800EC1CA = 0x80;
    D_800EC1C9 = 0x80;
    D_800EC1C8 = 0x80;
    D_800E9E63 = 0x80;
    func_80011DFC();
}
