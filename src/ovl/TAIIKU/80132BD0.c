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

void func_80133494(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80044750(0xBF);
        func_80048EB8(0);
        func_8004284C();
        break;
    case 1:
        func_80059E00();
        break;
    case 2:
        func_80059BC0();
        break;
    case 3:
        func_8004ADE4();
        func_800573AC();
        func_80057390(0);
        func_8006BC28(0);
        func_80084E4C();
        func_800649D4();
        func_80064E84();
        func_800847B8(0U);
        func_8004E58C();
        func_8004E9F4(0);
        func_8006612C((s32) "　");
        func_80065F34(0);
        D_800E6280.unk_03A = 0x80;
        func_80086424();
        func_8004284C();
        break;
    case 4:
        cd_read_sectors(0x95U, 0x801A0000, 0x9B94);
        func_80086424();
        func_8004284C();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80068938(D_800E6280.unk_03E, D_800E6280.unk_03F, 0);
            func_8004284C();
        }
        func_80086424();
        break;
    case 6:
        func_80086424();
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_8006764C(0);
    func_8006BD6C(0);
    func_80064FA4(0);
    func_80063930(0);
}

void func_80133644(void) {
    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        switch (D_800E6280.unk_F74) {
        case 0:
        case 0xFF:
            func_8004284C();
            break;
        case 1:
            func_80042808();
            break;
        }
        break;
    case 1:
        func_800866A0(0x541D);
        break;
    case 2:
        D_800E6280.unk_F5F = 0;
        func_8007C740();
        func_80083474();
        func_80083440(1U);
        break;
    case 3:
        func_8007E390();
        break;
    case 4:
        func_800847B8(0U);
        func_80132B90();
        D_800CA148 = 3;
        D_800CA14C = 0;
        D_800CA160 = D_80148F00;
        D_800CA164 = D_80148F04;
        D_800CA168 = D_80148F08;
        if (D_800E6280.unk_F74 == 0xFF) {
            func_80042940(6);
        } else {
            func_8004284C();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        D_800CA148 = 4;
        func_8004284C();
        break;
    case 7:
        func_80085A60();
        break;
    case 8:
        if (D_800E6280.unk_F74 == 1) {
            func_8004284C();
        }
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        if (D_800E6280.unk_F88 != 0) {
            func_8008585C();
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

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

void func_801339C4(arg0)
s32 arg0;
{
    u8 *p;
    s32 *ptr; /* FAKE: the pointer to the selector word moves the load into $v1 as in the original; found with decomp-permuter. T-10910 */
    int sel;

    sel = *(ptr = &D_800E6280.unk_F80) & 0xF000;

    switch (sel) {
    case 0x3000:
        p = D_8011ECD0 + arg0 * 0x44;
        *(s32 *)(p + 0x28) += -0x1E000;
        *(s32 *)(p + 0x24) += 0x22000;
        break;
    case 0x9000:
        p = D_8011ECD0 + arg0 * 0x44;
        *(s32 *)(p + 0x28) += -0x1E000;
        *(s32 *)(p + 0x24) += -0x22000;
        break;
    case 0x6000:
        p = D_8011ECD0 + arg0 * 0x44;
        *(s32 *)(p + 0x28) += 0x1E000;
        *(s32 *)(p + 0x24) += 0x22000;
        break;
    case 0xC000:
        p = D_8011ECD0 + arg0 * 0x44;
        *(s32 *)(p + 0x28) += 0x1E000;
        *(s32 *)(p + 0x24) += -0x22000;
        break;
    case 0x1000:
        p = D_8011ECD0 + arg0 * 0x44;
        *(s32 *)(p + 0x28) += -0x20000;
        break;
    case 0x4000:
        p = D_8011ECD0 + arg0 * 0x44;
        *(s32 *)(p + 0x28) += 0x20000;
        break;
    case 0x8000:
        p = D_8011ECD0 + arg0 * 0x44;
        *(s32 *)(p + 0x24) += -0x24000;
        break;
    case 0x2000:
        p = D_8011ECD0 + arg0 * 0x44;
        *(s32 *)(p + 0x24) += 0x24000;
        break;
    }
}
