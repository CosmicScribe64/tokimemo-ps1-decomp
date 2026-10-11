#include "common.h"
#include "ovl/TAIIKU.h"

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_80132BD0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_80132E20);

void func_801330D4(void) {
    func_801330FC();
    func_801331D0();
}

void func_801330FC(void) {
    /* FAKE: two unused word slots reproduce the eight-byte frame tail.
     * T-9560; original local declarations unknown. */
    s32 unused0;
    s32 unused1;
    RECT rect;
    s32 index;

    rect.x = 0x140;
    rect.y = 0;
    rect.w = 0x20;
    rect.h = 0x70;
    func_8004E44C(0, (u32 *)D_801491C4, D_801491C0);
    srn_init(0, D_801491C8, 0);
    srn_vram_set(0, 5, 0x580, 0xF0);
    func_8009C93C(&rect, 0x3C0, 0);
    for (index = 0; index < 11; index++) {
        D_800E6280.unk_1228[index + 5] = 8;
    }
}

void func_801331D0(void) {
    /* FAKE: unused 8-byte local reproduces the original frame (real source unknown). T-9150 */
    s32 pad[2];
    RECT r;
    s32 i;

    i = 0x140;
    do {
        r.x = i;
        r.y = 0x70;
        r.w = 0x40;
        r.h = 0x80;
        func_8009C884(&r, D_801491CC);
        i += 0x40;
    } while (i < 0x200);
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 0x25] = 8;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_80133258);

void func_80133438(void) {
    func_80132E20(0, 1);
    Scroll(0, 0xF, 0, 0x70, 0x280);
    Scroll(0, 0xF, 0x70, 0x80, 0x80);
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_80133494);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_80133644);

void func_8013384C(arg0, arg1)
s32 arg0;
u8 arg1;
{
    /* Promoted parameter locals precede the table copies (T-9560). */
    s32 index;
    s32 column;
    TaiikuWords48 a;
    TaiikuWords48 b;

    a = D_80149030;
    b = D_801490F0;
    if (arg0 == 11) {
        index = 0;
    } else {
        index = arg0;
    }
    column = arg1;
    func_80046290(a.unk_00[arg0][column], b.unk_00[arg0][column], index);
    func_80044750(0x300);
}

void func_80133928(s32 arg0) {
    u8 *p;

    func_801339C4();
    p = D_8011ECD0 + arg0 * 0x44;
    if (*(s16 *)(p + 0x26) >= 0x90) {
        *(s16 *)(p + 0x26) = 0x90;
    }
    if (*(s16 *)(p + 0x26) < -0x9F) {
        *(s16 *)(p + 0x26) = -0xA0;
    }
    if (*(s16 *)(p + 0x2A) >= 0x68) {
        *(s16 *)(p + 0x2A) = 0x68;
    }
    if (*(s16 *)(p + 0x2A) < -0x67) {
        *(s16 *)(p + 0x2A) = -0x68;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_801339C4);
