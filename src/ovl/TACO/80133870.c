#include "common.h"
#include "ovl/TACO.h"

/* s32 with no return (the caller uses it as implicit int): v0 is live at the exit, so as1 cannot fill the delay slot of the branch before the last call with lui v0 and takes the move a1,zero instead. T-16562 */
s32 func_80133870(void) {
    D_800E6280.unk_1104.u += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80133980();
        break;
    case 1:
        func_80133AF4();
        break;
    case 2:
        func_80133C08();
        break;
    case 3:
        func_80133E98();
        break;
    case 4:
        func_80134500();
        break;
    }
    if (D_800E6280.unk_1104.u & 0x10) {
        func_801432F0((s32) "NOW LOADING DATA ", -0x60, 0, 1);
    }
    if ((D_800E6280.unk_1104.u != 0) && ((u32) D_800E6280.unk_1104.u < 0x400U)) {
        func_801430D0(D_8015EDEC--, 0, 0x32, 6);
    }
}

s32 func_80133980(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80044750(0x24);
        D_8015EDEC = 0x400;
        func_800438DC(1, 0);
        D_800E6280.unk_036 = 0;
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_038 = 0;
        func_80041584();
        func_80143E80();
        func_800438F0(1);
        func_8006BD6C(0);
        func_8004E58C();
        func_80048EB8(0);
        func_80143AF4();
        D_8015EDB8 = 0;
        cd_read_sectors(0x21, 0x80180000, 0xB4F3);
        cd_read_sectors(1, 0x801FA800, 0xB514);
        func_80041168(0xFF);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
    case 2:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
            return;
        }
        return;
    case 3:
        D_800E6280.unk_1A6C = 0x8018A000;
        D_800E6280.unk_1A4C = 0x801FAA7C;
        D_800E6280.unk_1A5C = 0x801FAA88;
        D_800E6280.unk_1A3C = 0x801FAA68;
        func_801342CC();
        func_8004284C();
        break;
    }
}

void func_80133AF4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80044890(0, 0xC56D, 0xC54D, 0xD310, 0xD2C2, 0xD29D);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        if (func_80044C98() == 1) {
            D_800E6280.unk_110D += 1;
            return;
        }
    default:
        return;
    case 2:
        func_80044890(1, 0xC54B, 0xC519, 0xC7BB, 0xC777, 0xC766);
        D_800E6280.unk_110D += 1;
        return;
    case 3:
        if (func_80044C98() == 1) {
            D_800E6280.unk_110D += 1;
            return;
        }
        break;
    case 4:
        func_8004284C();
        break;
    }
}

void func_80133C08(void) {
    s32 temp_a1;
    s32 var_s0;

    switch (D_800E6280.unk_110D) {
    case 0:
        func_800462C8(5U, 0x801B8000, 0xB473);
        func_800462C8(3U, 0x801C0000, 0xB4BC);
        func_800462C8(0x21U, 0x801C8000, 0xB57F);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
    case 2:
    case 3:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
            return;
        }
    default:
        return;
    case 4:
        func_80134450();
        D_800E6280.unk_110D += 1;
        return;
    case 5:
        func_800462C8(0x20U, 0x80180000, 0xB489);
        func_800462C8(0x20U, 0x80190000, 0xB4D3);
        func_800462C8(8U, 0x80170000, 0xB47E);
        D_800E6280.unk_110D += 1;
        return;
    case 6:
    case 7:
    case 8:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
            return;
        }
        break;
    case 9:
        func_80143730(0x80180000, 0x18);
        func_80143730(0x80190000, 0x1A);
        func_80143730(0x80170000, 0xA);
        D_800E6280.unk_110D += 1;
        return;
    case 10:
        func_800462C8(0x14U, 0x80180000, 0xB4BF);
        D_800E6280.unk_110D += 1;
        return;
    case 11:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
            return;
        }
        break;
    case 12:
        func_8004435C(0x380U, 0x100U, 0x20U, 0x80U, 0x80180000);
        func_8004435C(0x3A0U, 0x100U, 0x20U, 0x80U, 0x80182000);
        func_8004435C(0x380U, 0x180U, 0x20U, 0x80U, 0x80184000);
        func_8004435C(0x3A0U, 0x180U, 0x20U, 0x80U, 0x80186000);
        var_s0 = 0;
        do {
            temp_a1 = var_s0 & 0xFF;
            func_80043914(0x80188000, temp_a1, 1, 3, temp_a1);
            var_s0 += 1;
        } while (var_s0 != 0xF);
        func_8004284C();
        break;
    }
}

/* FAKE: retain s32 fallthrough view and word stores into unk_1095[3]/[7]
 * (+0x1098/+0x109C); two consecutive increments in case 0 match the folded
 * addiu v0,t7,2 reload sequence. T-24129. */
s32 func_80133E98(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_800462C8(1, 0x8018A000, 0xB3F2);
        func_800462C8(5, 0x8018A800, 0xB413);
        func_800462C8(0x12, 0x8018D000, 0xB438);
        D_800E6280.unk_110D += 1;
        D_800E6280.unk_110D += 1;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 5:
        func_800462C8(7, 0x80196000, 0xB45B);
        D_800E6280.unk_110D += 1;
        func_800462C8(3, 0x80199800, 0xB478);
        func_800462C8(2, 0x8019B000, 0xB47B);
        D_800E6280.unk_110D += 1;
        break;
    case 6:
    case 7:
    case 8:
    case 9:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 10:
        func_800462C8(1, 0x8019C000, 0xB486);
        func_800462C8(2, 0x8019C800, 0xB487);
        func_800462C8(4, 0x8019D800, 0xB4A9);
        func_800462C8(6, 0x8019F800, 0xB4AD);
        D_800E6280.unk_110D += 1;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 15:
        func_800462C8(2, 0x801A2800, 0xB4B3);
        func_800462C8(4, 0x801A3800, 0xB4B5);
        func_800462C8(3, 0x801A5800, 0xB4B9);
        func_800462C8(0xA, 0x801A7000, 0xB515);
        D_800E6280.unk_110D += 1;
        break;
    case 16:
    case 17:
    case 18:
    case 19:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 20:
        func_800462C8(1, 0x801AC000, 0xB534);
        func_800462C8(3, 0x801AC800, 0xB668);
        func_800462C8(3, 0x801AE000, 0xB66B);
        func_800462C8(3, 0x801AF800, 0xB66E);
        D_800E6280.unk_110D += 1;
        break;
    case 21:
    case 22:
    case 23:
    case 24:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 25:
        func_800462C8(2, 0x801B1000, 0xB671);
        func_800462C8(3, 0x801B2000, 0xB673);
        func_800462C8(3, 0x801B3800, 0xB676);
        func_800462C8(2, 0x801B5000, 0xB679);
        D_800E6280.unk_110D += 1;
        break;
    case 26:
    case 27:
    case 28:
    case 29:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 30:
        D_8015EDB4 = (TcActor *) D_8011ED58;
        *(s32 *)&D_800E6280.unk_1095[3] = 1;
        *(s32 *)&D_800E6280.unk_1095[7] = 2;
        func_8004284C();
        break;
    }
}

/* implicit-int function (called before its definition): s32 with no return. The loop re-reads and re-writes one word (buf[0x80]) 16 times, as the asm does. */
s32 func_801342CC(void) {
    u32 *buf;
    s32 i;

    tpage_buf_clear_all();
    LoadSquare(0x360, 0x180, 0x20, 0x80, (void *)0x80180000);
    load_tpage_buf_lock(0x3F, 0x80180000, 2);
    load_tpage_buf_lock(0x3D, 0x80182000, 2);
    load_tpage_buf_lock(0x1D, 0x80184000, 2);
    load_tpage_buf_lock(0x1C, 0x80186000, 2);
    load_tpage_buf_lock(0x3C, 0x80188000, 2);
    load_palette(0x8018A000, 0xF, 1, 1, 0);
    load_palette(0x8018A000, 0, 1, 1, 0);
    LoadSquare(0x360, 0x100, 0x20, 0x80, (void *)0x8018A200);
    load_palette(0x8018A000, 1, 1, 3, 0);
    load_tpage_buf_lock(0x1F, 0x8018C200, 1);
    buf = (u32 *)0x80190200;
    for (i = 0; i < 0x10; i++) {
        buf[0x80] &= 0x7FFF0000;
    }
    LoadSquare(0x100, 0x1F8, 0x100, 8, buf);
    func_8009C674(0);
}

void func_80134450(void) {
    func_8004435C(0x100, 0x1E0, 0x100, 0x18, 0x801B8000);
    func_8004435C(0, 0x1F0, 0x100, 0x10, 0x801C0000);
    func_80143730(0x801C8000, 8);
    func_80143730(0x801C8000, 0xA);
    func_80143730(0x801C8000, 0xC);
    func_80143730(0x801C8000, 0xE);
    func_80143730(0x801C8000, 0x18);
    func_80143730(0x801C8000, 0x1A);
    func_8009C674(0);
}

s32 func_80134500(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_038 = 0;
        D_800E6280.unk_10A2 = 0;
        D_800E6280.unk_03A = 0x80;
        D_800E6280.unk_036 = 0;
        func_800438DC(1, 1);
        func_80058D20();
        func_8014394C();
        func_80058EB0();
        func_801345D4();
        func_80058F0C(0x8018A000);
        func_8013474C();
        func_801347F4((s32 *)0x801AC000);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80042908(1);
        break;
    }
}

/* File-local view of the settings block at D_800E7D10 (0x800E7D50 = +0x40), T-16562.
   The flag test reads the word at +0x40 through this view too: uopt then sees the byte
   read-modify-write of the else path overlap a word access and keeps &D_800E7D10 in $v0
   (with D_800E7D50 in the test it makes the byte a plain global with lui/%lo addresses). */
typedef struct TcSetW {
    u32 pad0 : 2;
    u32 mode : 1;
    u32 sel : 2;
} TcSetW;
typedef struct TcSetH {
    u16 pad0 : 12;
    u16 hi : 4;
} TcSetH;
typedef struct TcSetB {
    u8 b0;
    u8 lo : 4;
} TcSetB;
typedef struct TcSetF {
    u8 flag : 1;
} TcSetF;
typedef struct TcSave {
    u8 pad00[0x40];
    union {         /* 0x40: one view per load width of the original */
        s32 word;
        TcSetW w;
        TcSetH h;
        TcSetB b;
        TcSetF f;
        struct {
            u8 pad[2];
            u8 name0[3];    /* 0x42 */
            u8 name1[3];    /* 0x45 */
            u8 name2[3];    /* 0x48 */
            u8 pad4B;
            s32 unk4C;      /* 0x4C */
            s32 unk50;      /* 0x50 */
            s32 unk54;      /* 0x54 */
        } s;
    } set;
} TcSave;

void func_801345D4(void) {
    TcSave *save;

    save = (TcSave *)&D_800E7D10;
    if (save->set.word & 1) {
        D_8015EDF8 = save->set.s.unk4C;
        D_8015EDFC = save->set.s.unk50;
        D_8015EE00 = save->set.s.unk54;
        D_8015EDD8 = save->set.w.mode;
        D_8015EDDC = save->set.w.sel;
        D_8015EDE0 = save->set.h.hi;
        D_8015EDE4 = save->set.b.lo;
        func_800AE090(D_8015EE04, save->set.s.name0, 3);
        func_800AE090(D_8015EE08, save->set.s.name1, 3);
        func_800AE090(D_8015EE0C, save->set.s.name2, 3);
        return;
    }
    save->set.f.flag = 1;
    D_8015EDF8 = 0x2EA694;
    D_8015EDFC = 0x1F6454;
    D_8015EE00 = 0x102214;
    func_800AE0F0(D_8015EE04, "K.K");
    func_800AE0F0(D_8015EE08, "K.T");
    func_800AE0F0(D_8015EE0C, "N.S");
}

void func_8013474C(void) {
    TcObj50 *p;

    /* FAKE: init and loop head on one source line give the original's addiu order (as1 schedules by line, T-7020) */
    p = D_80127480; do {
        func_80099E30(0, p);
        p->unk20 = -0xFA0;
        *(s32 *)p = 0;
        p++;
    } while (p != (TcObj50 *)D_80128880);
}

void func_801347A8(void) {
    s32 i;

    func_80059048();
    for (i = 0; i < 0x40; i++) {
        D_80127080[i].unk0 = 0x80000000;
    }
}

void func_801347F4(s32 *arg0) {
    s32 *dop;
    s32 i;

    dop = arg0;
    dop++;
    func_8009B3C0((s32)dop);
    dop++;
    dop++;
    for (i = 0; i < 0x40; i++) {
        func_8009B430((s32)dop, (u8 *)&D_80127080[i], 0);
        D_80127080[i].unk4[0] = (s32)&D_80127480[i];
        D_80127080[i].unk0 = 0x80000000;
    }
}

void func_80134884(void) {
    func_800AE080((u8 *) D_8015E270, 0x140);
    func_800AE080((u8 *) D_8015E3B0, 0x140);
    func_800AE080((u8 *) D_8015E4F0, 0x140);
    func_800AE080((u8 *) D_8015E630, 0x140);
    func_800AE080((u8 *) D_8015E770, 0x140);
    func_800AE080((u8 *) D_8015E8B0, 0x140);
    func_800AE080((u8 *) D_8015E9F0, 0x140);
    func_800AE080((u8 *) D_8015EB30, 0x140);
    func_800AE080((u8 *) D_8015EC70, 0x140);
}
