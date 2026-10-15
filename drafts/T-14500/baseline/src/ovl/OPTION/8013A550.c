#include "common.h"
#include "ovl/OPTION.h"

s32 func_8013A550(s32 arg0, s32 arg1, s32 arg2) {
    if (D_800E6280.unk_F88 & 0x80) {
        arg0 += 1;
        if (arg1 < arg0) {
            arg0 = arg2;
        }
    } else if ((D_800E6280.unk_F80 & 0x80) && (D_800E6280.unk_F80 & 5)) {
        arg0 += 1;
        if (arg1 < arg0) {
            arg0 = arg2;
        }
    }
    if (D_800E6280.unk_F88 & 0x10) {
        arg0 -= 1;
        if (arg0 < arg2) {
            arg0 = arg1;
        }
    } else if ((D_800E6280.unk_F80 & 0x10) && (D_800E6280.unk_F80 & 5)) {
        arg0 -= 1;
        if (arg0 < arg2) {
            arg0 = arg1;
        }
    }
    return arg0;
}

void func_8013A604(void) {
    s32 i;
    s32 padx[1]; /* FAKE: one unused word above the arrays reproduces the original frame and array offsets; real source unknown. T-9310 */
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    D_800B3C6C = 1;
    func_8006BC28(0);
    for (i = 0; i < 16; i++) {
        a[i] = 0x20;
        b[i] = i * 0xC - 0x60;
        c[i] = 0x10;
        d[i] = 0xC;
    }
    func_8004F870(0, 0x10, a, b, c, d);
    (&D_800E6280.unk_036)[0] = (&D_800E6280.unk_036)[1] = (&D_800E6280.unk_036)[2] = 0x80;
    func_8004ADAC(4);
    func_800AE0F0(D_800E6280.unk_0D4, "おぷ");
    func_800AE0F0(D_800E6280.unk_0DC, "ション");
    func_800AE0F0(D_800E6280.unk_0E4, "起動");
    func_8004E58C();
    func_8004E788(-0x80, -0x18, 0xF, (s32)"設定メニュー１", 0);
    func_8004E788(-0x80, 0, 0xF, (s32)"月日関連", 0);
    func_8004E788(0x20, -0x18, 0xF, (s32)D_800E6280.unk_0D4, 0);
    func_8004E788(0x20, -0xC, 0xF, (s32)D_800E6280.unk_0DC, 0);
    func_8004E788(0x20, 0, 0xF, (s32)D_800E6280.unk_0E4, 0);
    func_8004E884(5);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/8013A550", func_8013A7EC);

/* No return value on purpose: the original is an implicit-int function (the `or v0,v1,zero` in the first delay slot). T-6070 */
s32 func_8013AC30(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        cd_read_sectors(0x95U, 0x801A0000, 0x9B94);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80068938(D_800E6280.unk_03E, D_800E6280.unk_03F, 1);
            D_800B5BC8 = 0xEF;
            func_800674B0();
            func_8004284C();
        }
        break;
    }
}

void func_8013ACD4(void) {
    if (func_80044E8C() == 1) {
        func_8004500C(0, 0);
        func_80042940(1);
    }
}

s32 func_8013AD10(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013A604();
        break;
    case 1:
        func_8013A7EC();
        break;
    case 254:
        func_8013AC30();
        break;
    case 255:
        func_8013ACD4();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD14);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD1C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD24);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD28);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD30);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD34);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD3C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD40);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD44);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD4C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD54);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD58);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD60);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD68);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD74);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CD7C);

void func_8013AD94(void) {
    s32 i;
    s32 pad[1]; /* FAKE: one unused word above the arrays reproduces the original array offsets; real source unknown. T-9310 */
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    func_8006BC28(0);
    for (i = 0; i < 16; i++) {
        a[i] = 0x20;
        b[i] = i * 0xC - 0x60;
        c[i] = 0x10;
        d[i] = 0xC;
    }
    func_8004F870(0, 0x10, a, b, c, d);
    func_8004E58C();
    func_8004E788(-0x80, -0x18, 0xF, (s32)"設定メニュー２", 0);
    func_8004E788(-0x80, 0, 0xF, (s32)"プレイヤーデータ", 0);
    func_8004E788(0x60, -0x18, 0xF, (s32)D_800E6280.unk_0D4, 0);
    func_8004E788(0x60, -0xC, 0xF, (s32)D_800E6280.unk_0DC, 0);
    func_8004E788(0x60, 0, 0xF, (s32)D_800E6280.unk_0E4, 0);
    func_8004E884(5);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/8013A550", func_8013AF10);

s32 func_8013B3BC(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013AD94();
        break;
    case 1:
        func_8013AF10();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CDAC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CDB8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CDC0);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CDCC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CDD4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CDE0);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CDE8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CDF4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CDFC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE08);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE10);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE14);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE1C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE28);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE30);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE38);

void func_8013B410(void) {
    s32 i;
    s32 pad[1]; /* FAKE: one unused word above the arrays reproduces the original array offsets; real source unknown. T-9310 */
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    func_8006BC28(0);
    for (i = 0; i < 16; i++) {
        a[i] = 0x20;
        b[i] = i * 0xC - 0x60;
        c[i] = 0x10;
        d[i] = 0xC;
    }
    func_8004F870(0, 0x10, a, b, c, d);
    func_8004E58C();
    func_8004E788(-0x80, -0x18, 0xF, (s32)"設定メニュー３", 0);
    func_8004E788(-0x80, 0, 0xF, (s32)"女の子データ", 0);
    func_8004E788(0x40, -0x18, 0xF, (&D_800B3538)[D_8013E740 & 0xF], 0);
    func_8004E884(3);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/8013A550", func_8013B568);

s32 func_8013BE78(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013B410();
        break;
    case 1:
        func_8013B568();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE60);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE68);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE74);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE7C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE88);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE94);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CE9C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CEA4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CEAC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CEB0);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CEB8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CEC0);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CEC8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CECC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CED4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CEDC);

void func_8013BECC(void) {
    s32 i;
    s32 pad[1]; /* FAKE: one unused word above the arrays reproduces the original array offsets; real source unknown. T-9310 */
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    func_8006BC28(0);
    for (i = 0; i < 16; i++) {
        a[i] = 0x20;
        b[i] = i * 0xC - 0x60;
        c[i] = 0x10;
        d[i] = 0xC;
    }
    func_8004F870(0, 0x10, a, b, c, d);
    func_8004E58C();
    func_8004E788(-0x80, -0x18, 0xF, (s32)"設定メニュー４", 0);
    func_8004E788(-0x80, 0, 0xF, (s32)"その他データ", 0);
    func_8004E884(2);
    func_8004284C();
    D_800E6280.unk_71C = 0;
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/8013A550", func_8013BFF8);

s32 func_8013C3B0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013BECC();
        break;
    case 1:
        func_8013BFF8();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF04);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF0C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF14);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF1C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF24);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF30);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF38);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF40);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF48);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF50);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF58);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF60);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF68);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF70);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF78);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CF80);

void func_8013C404(void) {
    s32 i;
    s32 pad[1]; /* FAKE: one unused word above the arrays reproduces the original array offsets; real source unknown. T-9310 */
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    func_8006BC28(0);
    for (i = 0; i < 16; i++) {
        a[i] = 0x20;
        b[i] = i * 0xC - 0x60;
        c[i] = 0x10;
        d[i] = 0xC;
    }
    func_8004F870(0, 0x10, a, b, c, d);
    func_8004E58C();
    func_8004E788(-0x80, -0x18, 0xF, (s32)"設定メニューＦ", 0);
    func_8004E788(-0x80, 0, 0xF, (s32)"目標ステップ", 0);
    func_8004E884(5);
    func_8004284C();
}

void func_8013C52C(void) {
    s32 i;

    menu_check(0, D_8011ECF6, D_8011ECFA);
    func_8004FC10(0);
    func_80050B54(0);
    for (i = 0; i < 16; i++) {
        func_8004B338(D_8013E844[i], -0x20, i * 0xC - 0x60);
    }
    func_800AE080(D_80122DD0, 0x100);
    if (D_800E6280.unk_F88 & 0x90) {
        switch (D_800E6280.unk_1093) {
        case 0:
            D_800B3C6C = 0;
            main_state_set(0x34);
        default:
            break;
        case 1:
            main_state_set(0x50);
            break;
        case 2:
            func_8004E58C();
            func_80048DD0(0);
            main_state_set(0x5D);
            break;
        case 3:
            main_state_set(0x57);
            break;
        case 4:
            main_state_set(0x56);
            break;
        case 5:
            main_state_set(0x5A);
            break;
        case 6:
            main_state_set(0x72);
            break;
        case 7:
            main_state_set(0x71);
            if (D_800E6280.unk_0F4.h >> 0xC < 6) {
                main_state_set(0x70);
            }
            break;
        case 8:
            main_state_set(0x51);
            break;
        case 9:
            main_state_set(0x53);
            break;
        case 10:
            main_state_set(0x55);
            break;
        case 11:
            main_state_set(0x54);
            break;
        case 12:
            main_state_set(0x58);
            break;
        case 13:
            main_state_set(0x52);
            break;
        case 14:
            main_state_set(0x5E);
            break;
        case 15:
            main_state_set(0x59);
            break;
        }
    }
}

s32 func_8013C72C(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013C404();
        break;
    case 1:
        func_8013C52C();
        break;
    }
}

void func_8013C780(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_8013AD10();
        break;
    case 1:
        func_8013B3BC();
        break;
    case 2:
        func_8013BE78();
        break;
    case 3:
        func_8013C3B0();
        break;
    case 0xFF:
    default:
        func_8013C72C();
        break;
    }
    if (D_800E6280.unk_1109 != 0xFF && (D_800E6280.unk_F88 & 0x20)) {
        func_80042808();
    }
    if (D_800E6280.unk_1109 != 0 && (D_800E6280.unk_F88 & 0x40)) {
        func_80042908((D_800E6280.unk_1109 - 1) & 0xFF);
    }
    func_80066C08(1);
}

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CFA8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CFB4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CFBC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CFC4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CFCC);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CFD4);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CFE0);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CFE8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CFF0);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013CFF8);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013D000);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013D008);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013D010);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013D01C);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013D024);

INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/8013A550.rodata", D_8013D02C);
