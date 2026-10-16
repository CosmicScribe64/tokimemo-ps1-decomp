#include "common.h"
#include "ovl/OPTION.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D360);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D364);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D368);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D36C);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D370);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D374);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D378);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D37C);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D380);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D39C);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3A0);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3A4);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3A8);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3B0);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3B4);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3B8);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3BC);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3CC);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3DC);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3E0);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3F0);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3F4);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D3F8);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D798);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D880);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013D968);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013DD58);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013E148);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013E244);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013E340);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013E43C);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013E538);
INCLUDE_RODATA("asm/ovl/OPTION/data/OPTION/801389A0.data", D_8013E634);
s32 D_8013E730 = 0;
s32 D_8013E734 = 0;
s32 D_8013E738 = 0;
s32 D_8013E73C = 0;

void func_801389A0(void) {
    D_800E6280.unk_1100 += 1;
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80138A24();
        break;
    case 1:
        func_80138A8C();
        break;
    default:
        func_80046500();
        break;
    }
    func_80066C08(0);
    func_80066334();
}

void func_80138A24(void) {
    if (D_800E6280.unk_110D == 0) {
        cd_read_sectors(0x3A, 0x80180000, 0xA1AB);
        D_800E6280.unk_110D += 1;
    } else if (cd_read_sync_status() & 1) {
        func_80042808();
    }
}

void func_80138A8C(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80139198();
        return;
    case 1:
        func_80139878();
        return;
    case 2:
        func_8013990C();
        return;
    case 3:
        func_80138C20();
        return;
    case 4:
        func_80138FC0();
        return;
    case 5:
        func_80138DF8();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80138B4C(s32 arg0) {
    s32 i;
    s16 a[16];
    s16 b[16];
    s16 c[16];
    s16 d[16];

    for (i = 0; i < 16; i++) {
        a[i] = 0;
        b[i] = i * 10 - 0x64;
        c[i] = 0x40;
        d[i] = 0xA;
        func_8004F870(1, 0x10, a, b, c, d);
    }
}

void func_80138C20(void) {
    s32 i;
    s32 y;
    s32 next;
    u8 (*rows)[16];

    if (D_800E6280.unk_110D == 0) {
        func_80138B4C(0);
        i = 0;
        do {
            next = i + 1;
            k_disp_switch(next, 0);
            i++;
        } while (i != 7);
        D_800E6280.unk_110D += 1;
    } else {
        if (D_800E6280.unk_F88 & 0x20) {
            if (D_800E6280.unk_1094 != -1) {
                D_8013D3A4 = D_800E6280.unk_1094 + D_8013D3A4;
            }
            func_8013A31C();
            i = 0;
            do {
                next = i + 1;
                k_disp_switch(next, 1);
                i++;
            } while (i != 7);
            func_80042940(2);
        }
    }
    i = 0;
    y = -0x64;
    do {
        func_8004AE54((s32) ((u32) (D_8013D3A4 + i) % 63U), -0xA, y, 6);
        i += 1;
        y += 0xA;
    } while (i < 0x10);
    rows = D_8013DD58;
    for (i = 0, y = -0x64; i != 0x10; i++, y += 0xA) {
        func_8004B338((s32) rows[((u32) (D_8013D3A4 + i) % 63U)], 0xA, y);
    }
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    menu_bar_show(0);
    func_8013A4FC();
    func_8013A484();
}

void func_80138DF8(void) {
    s32 i;
    s32 y;
    u8 (*rows)[16];

    if (D_800E6280.unk_110D == 0) {
        func_80138B4C(0);
        for (i = 0; i < 7; i++) {
            k_disp_switch(i + 1, 0);
        }
        D_800E6280.unk_110D += 1;
    } else {
        if (D_800E6280.unk_F88 & 0x20) {
            if (D_800E6280.unk_1094 != -1) {
                D_8013D3DC = D_800E6280.unk_1094 + D_8013D3DC;
            }
            for (i = 0; i < 7; i++) {
                k_disp_switch(i + 1, 1);
            }
            func_80042940(2);
        }
    }
    i = 0;
    y = -0x64;
    do {
        func_8004AE54((s32) ((u32) (D_8013D3DC + i) % 58U), -0xA, y, 6);
        i += 1;
        y += 0xA;
    } while (i < 0x10);
    rows = D_8013D3F8;
    for (i = 0, y = -0x64; i != 0x10; i++, y += 0xA) {
        func_8004B338((s32) rows[((u32) (D_8013D3DC + i) % 58U)], 0xA, y);
    }
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    menu_bar_show(0);
    func_8013A4FC();
    func_8013A484();
}

void func_80138FC0(void) {
    s32 i;
    s32 y;
    s32 next;
    u8 (*rows)[16];

    if (D_800E6280.unk_110D == 0) {
        func_80138B4C(1);
        i = 0;
        do {
            next = i + 1;
            k_disp_switch(next, 0);
            i++;
        } while (i != 7);
        D_800E6280.unk_110D += 1;
    } else {
        if (D_800E6280.unk_F88 & 0x20) {
            if (D_800E6280.unk_1094 != -1) {
                D_8013D3A0 = D_800E6280.unk_1094 + D_8013D3A0;
            }
            func_8013A304();
            i = 0;
            do {
                next = i + 1;
                k_disp_switch(next, 1);
                i++;
            } while (i != 7);
            func_80042940(2);
        }
    }
    i = 0;
    y = -0x64;
    do {
        func_8004AE54((s32) ((u32) (D_8013D3A0 + i) % 63U), -0xA, y, 6);
        i += 1;
        y += 0xA;
    } while (i < 0x10);
    rows = D_8013D968;
    for (i = 0, y = -0x64; i != 0x10; i++, y += 0xA) {
        func_8004B338((s32) rows[((u32) (D_8013D3A0 + i) % 63U)], 0xA, y);
    }
    menu_check(1, D_8011ECF6, D_8011ECFA);
    menu_bar_show(1);
    func_8004FC10(1);
    menu_bar_show(0);
    func_8013A4FC();
    func_8013A484();
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80139198);

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_801394F4);

void func_80139878(void) {
    u8 t;

    *(u8 *)&D_800E6280.unk_03A += 4;
    t = *(u8 *)&D_800E6280.unk_03A;
    D_801220E2 = t;
    D_801220E1 = t;
    D_801220E0 = t;
    D_801220E8 = 0xA0;
    D_801220EA = 0x78;
    /* FAKE: empty test separates raw and truncated byte live ranges. T-10850 */
    if (t) {
    }
    D_801220D4 = 0;
    D_801220D6 = 0;
    if (t >= 0x80) {
        *(u8 *)&D_800E6280.unk_03A = 0x80;
        func_8004E884(7);
        func_8004284C();
    }
}

void func_8013990C(void) {
    D_801220EC = rsin(D_800E6280.unk_1104.w << 5);
    D_801220EE = rsin(D_800E6280.unk_1104.w << 5);
    D_801220F0 = D_800E6280.unk_1104.w << 0xC;
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    func_8013A484();
    func_80139A30();
    k_disp_inc();
    func_801394F4();
    func_801399C0();
}

void func_801399C0(void) {
    if (D_800E6280.unk_F88 & 0x800) {
        D_8013D3F4 += 1;
        func_8007BFB8();
    }
    func_8004B338("reverve", 0x78, 0);
    func_8004B19C(D_8013D3F4 & 1, 0x78, 0xA);
}

INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80139A30);

void func_80139E3C(arg0)
    s32 arg0;
{
    u16 v;
    s32 *t;
    u8 g;

    if (arg0 > 0) {
        D_8013D3B8 = arg0;
    }
    v = D_8013D3B8;
    if (arg0 < 0) {
        v++;
    }
    if (D_800E6280.unk_F80 & 4) {
        v++;
    }
    if ((D_800E6280.unk_F80 & 1) && (v != 0)) {
        v--;
    }
    g = D_800E6280.unk_F5F;
    t = D_8013D360[g & 0xF];
    D_8013D3B8 = v;
    if (t != 0) {
        t += v * 2;
        D_8013D3B8 = v;
        func_80046290(t[0], t[1], g);
    }
    func_80044750(0x300);
}

void func_80139F04(void) {
    if (D_800E6280.unk_F80 & 1) {
        D_8013E730 -= 1;
    }
    if (D_800E6280.unk_F80 & 4) {
        D_8013E730 += 1;
    }
    if (D_8013E730 < 0) {
        D_8013E730 = 0x20;
    }
    if (D_8013E730 >= 0x21) {
        D_8013E730 = 0;
    }
    func_80044750((D_8013E730 | 0x500) & 0xFFFF);
}

/* NON_MATCHING: T-10850, 11 differing listing lines; see the ticket scratch evidence. */
#ifdef NON_MATCHING
/* FAKE: width-staged locals preserve raw-byte and call-argument ranges. T-10850 */
void func_80139F8C(void)
{
    s32 t;
    u32 value;
    u16 code;
    t = D_800E6280.unk_F88 & 0x60;
    if (t != 0x20)
    {
        if (t == 0x40)
        {
            D_8013D3A8--;
        }
    }
    else
    {
        D_8013D3A8++;
    }
    if (D_8013D3A8 > 0x10)
    {
        value = D_8013D3A8;
        code = value | 0x1200;
        t = code & 0xFFFF;
        func_80044750(t);
    }
}
#else
INCLUDE_ASM("asm/ovl/OPTION/nonmatchings/OPTION/801389A0", func_80139F8C);
#endif

void func_8013A010(void) {
    func_8004500C(0, D_8013D3A8);
}

void func_8013A038(void) {
    func_8004500C(1, D_8013D3A8);
}

void func_8013A060(void) {
    if (D_8013D3B0 != 0) {
        func_8013A1BC();
        return;
    }
    func_8013A09C();
}

void func_8013A09C(void) {
    u8 t = D_8013D3B4;

    switch (t) {
    case 1:
        func_80044890(1, 0xBF98, 0xBF79, D_8013E340[D_8013D3A0 % 63], D_8013E244[D_8013D3A0 % 63],
                      D_8013E148[D_8013D3A0 % 63]);
        D_8013D3B4 += 1;
        break;
    case 2:
        if (func_80044E8C() == 1) {
            D_8013D3B4 = 0;
            func_800AE0F0(D_8013D3CC, D_8013D968[D_8013D3A0 % 63]);
            func_800AE0F0(D_8013D3E0, D_8013D3F8[D_8013D3DC % 58]);
        }
        break;
    }
}

void func_8013A1BC(void) {
    u8 t = D_8013D3B0;

    switch (t) {
    case 1:
        func_80044890(0, D_8013D880[D_8013D3DC % 58], D_8013D798[D_8013D3DC % 58], D_8013E634[D_8013D3A4 % 63],
                      D_8013E538[D_8013D3A4 % 63], D_8013E43C[D_8013D3A4 % 63]);
        D_8013D3B0 += 1;
        break;
    case 2:
        if (func_80044E8C() == 1) {
            D_8013D3B0 = 0;
            func_800AE0F0(D_8013D3BC, D_8013DD58[D_8013D3A4 % 63]);
            func_800AE0F0(D_8013D3E0, D_8013D3F8[D_8013D3DC % 58]);
        }
        break;
    }
}

void func_8013A304(void) {
    D_8013D3B4 += 1;
}

void func_8013A31C(void) {
    D_8013D3B0 += 1;
}

void func_8013A334(void) {
    func_8004AE54(D_8013D3B8, 0x50, -0x3C, 6);
    func_8004AE54(D_800E6280.unk_F5F & 0xF, -0x50, -0x3C, 6);
    func_8004AE54(D_8013D3B0, 0x50, -0x28, 6);
    func_8004AE54(D_8013D3B4, 0x50, -0x14, 6);
    func_8004AE54(D_8013D3A4, 0x50, 0, 6);
    func_8004B358(D_8013D3BC, -0x96, 8, 6);
    func_8004AE54(D_8013D3A0, 0x50, 0x14, 6);
    func_8004B358(D_8013D3CC, -0x96, 0x14, 6);
    func_8004AE54(D_8013D39C, -0x50, 0xA - D_8013D39C * 0xA, 6);
    func_8004AE54(D_8013E730, 0x50, 0x28, 6);
    func_8004B358(D_8013D3E0, -0x96, 0x20, 6);
    func_8004AE54(D_8013D3A8, 0x50, 0x3C, 6);
}

void func_8013A484(void) {
    func_80049A40(-0x50, -0x40, 0xB0, 0x90, 0xA, 0x1E021D, 0);
    func_80049A40(-0x96, 4, 0x50, 0x2C, 0xA, 0x101802, 0);
    dtd_on(0xA);
}

void func_8013A4FC(void) {
    func_80049A40(-0xA, -0x6E, 0x50, 0xB4, 6, 0xA08080, 2);
    dtd_on(6);
}
