#include "common.h"
#include "game.h"

void initView(void) {
    func_8009AD30(0x3E8);
    D_80122740 = 0;
    D_80122744 = 0;
    D_80122748 = 0x3E8;
    D_8012274C = 0;
    D_80122750 = 0;
    D_80122754 = 0;
    D_80122758 = 0;
    D_8012275C = 0;
    func_80099540(&D_80122740);
    func_8009AD50(0x64);
    func_8009AD60(0x10000);
}

INCLUDE_ASM("asm/nonmatchings/main/80058D20", initLight);

void initCoordinate(void) {
    TaiikuBig *p;

    /* FAKE: init and loop head on one source line give the original's addiu order (as1 schedules by line, T-7020) */
    p = D_801227A0; do {
        func_80099E30(0, p);
        p->unk20 = -0xFA0;
        p->unk0 = 0;
        p++;
    } while (p != D_80122CA0);
}

INCLUDE_ASM("asm/nonmatchings/main/80058D20", initModelingData_init);

INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_80058F9C);

void func_80059048(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        D_80122640[i * 4] = 0x80000000;
    }
}

void func_8005907C(void) {
    func_80098380();
    func_8009B560(0, 0, 0, 0xF0);
    func_80098530();
    InitGeom();
    D_8011ECA0 = func_80098370();
}

INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_800590CC);

INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_800591D8);

/* FAKE: the RECT field stores sharing a source line are scheduled as a unit by as1, which gives the original's store order (h before w). T-8010 */
void func_80059308(void) {
    RECT rect;

    rect.w = 0x100; rect.h = 0x1E0;
    rect.x = 0;
    rect.y = 0;
    func_8009C7F8(&rect, 0, 0, 0);
    func_8009C674(0);
    rect.x = 0x100; rect.w = 0x100; rect.h = 0x1E0;
    rect.y = 0;
    func_8009C7F8(&rect, 0, 0, 0);
    func_8009C674(0);
}

INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_8005938C);

void func_80059688(s32 arg0) {
    u8 *p;

    p = D_8011ECD0 + arg0 * 0x44;
    p[0] = 1;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    *(s16 *)(p + 0x64) = 0;
    *(s16 *)(p + 0x66) = 0;
    *(s16 *)(p + 0x68) = 0;
    *(s16 *)(p + 0x6A) = 0;
    *(s16 *)(p + 0x6C) = 0x1000;
    *(s16 *)(p + 0x6E) = 0x1000;
    *(s16 *)(p + 0x70) = 0x1000;
    *(s16 *)(p + 0x72) = 0;
    *(s16 *)(p + 0x74) = 0;
    *(s16 *)(p + 0x76) = 0;
    *(s16 *)(p + 0x78) = 0;
    *(s16 *)(p + 0x7A) = 0;
    *(s16 *)(p + 0x7C) = 0;
    *(s16 *)(p + 0x7E) = 0;
    *(s16 *)(p + 0x80) = 0;
    *(s16 *)(p + 0x82) = 0;
    p[0x84] = 0;
    p[0x85] = 1;
    p[0x86] = 0;
}
