#include "common.h"
#include "ovl/RPG_BAT.h"

#define ACT(i) (((DisplayActor44 *) D_8011ECD0)[i])

void func_80141060(void) {
    s32 sel; /* a named copy of the switch value: its home word is the extra frame word (T-17119) */
    s32 i;
    s32 j;

    for (i = 0; i < 0x60; i++) {
        func_80048F64(i + 0x40);
        if (i < 0x3F) {
            ACT(64 + i).unk_01 = 3;
        } else if (i < 0x40) {
            ACT(64 + i).unk_01 = 9;
        } else if (i < 0x48) {
            ACT(64 + i).unk_01 = 4;
        } else {
            ACT(64 + i).unk_01 = 5;
        }
        ACT(64 + i).unk_38 = 0x41000000;
        ACT(64 + i).unk_03 = 0;
        ACT(64 + i).unk_02 = 1;
        ACT(64 + i).unk_06 = 1;
        ACT(64 + i).unk_07 = 0;
        ACT(64 + i).unk_26 = -0xA0;
        ACT(64 + i).unk_2A = -0x78;
        ACT(64 + i).unk_05 = 0x80;
        ACT(64 + i).unk_43 = 0;
        ACT(64 + i).unk_04 = 8;
        ACT(64 + i).unk_0C = (u8 *) D_8015EEA4;
        ACT(64 + i).unk_10 = (u8 *) D_8015EEBC;
        ACT(64 + i).unk_34 = (u8 *) D_8015EE8C;
    }
    for (i = 0; i < 0x48; i++) {
        ACT(64 + i).unk_0C = (u8 *) D_8015EEA4;
        ACT(64 + i).unk_10 = (u8 *) D_8015EEBC;
        ACT(64 + i).unk_34 = (u8 *) D_8015EE8C;
    }
    load_tpage_buf_lock(0x15, ((s32 *) D_8015EE8C)[0], 1);
    load_tpage_buf_lock(0x16, ((s32 *) D_8015EE8C)[1], 1);
    load_tpage_buf_lock(0x17, ((s32 *) D_8015EE8C)[2], 1);
    load_tpage_buf_lock(0x18, ((s32 *) D_8015EE8C)[3], 1);
    load_tpage_buf_lock(0x19, ((s32 *) D_8015EE8C)[4], 1);
    load_tpage_buf_lock(0x35, ((s32 *) D_8015EE98)[0], 1);
    load_tpage_buf_lock(0x39, ((s32 *) D_8015EE98)[1], 1);
    sel = D_8015EB98;
    switch (sel) {
    case 0x10000:
        load_tpage_buf_lock(0xB, ((s32 *) D_8015EE94)[1], 1);
        load_tpage_buf_lock(0xC, ((s32 *) D_8015EE94)[2], 1);
        /* fall through */
    case 0x20000:
    case 0x40000:
        load_tpage_buf_lock(0xD, ((s32 *) D_8015EE94)[0], 1);
        load_tpage_buf_lock(0x2B, ((s32 *) D_8015EE90)[0], 1);
        load_tpage_buf_lock(0x2C, ((s32 *) D_8015EE90)[1], 1);
        load_tpage_buf_lock(0x2D, ((s32 *) D_8015EE90)[2], 1);
        if (D_8015EB9C == 0xD) {
            load_tpage_buf_lock(5, ((s32 *) D_8015EEA0)[0], 1);
            load_tpage_buf_lock(6, ((s32 *) D_8015EEA0)[1], 1);
            load_tpage_buf_lock(7, ((s32 *) D_8015EEA0)[2], 1);
        }
        break;
    case 0x80000000:
        load_tpage_buf_lock(0xB, ((s32 *) D_8015EEA0)[0], 1);
        load_tpage_buf_lock(0xC, ((s32 *) D_8015EEA0)[1], 1);
        load_tpage_buf_lock(0xD, ((s32 *) D_8015EEA0)[2], 1);
        load_tpage_buf_lock(0xE, ((s32 *) D_8015EEA0)[3], 1);
        load_tpage_buf_lock(0xF, ((s32 *) D_8015EEA0)[4], 1);
        load_tpage_buf_lock(0x2B, ((s32 *) D_8015EEA0)[5], 1);
        load_tpage_buf_lock(0x2D, ((s32 *) D_8015EE90)[0], 1);
        load_tpage_buf_lock(0x2E, ((s32 *) D_8015EE90)[1], 1);
        load_tpage_buf_lock(0x2F, ((s32 *) D_8015EE90)[2], 1);
        break;
    case 0x100000:
        load_tpage_buf_lock(0xB, ((s32 *) D_8015EE90)[6], 1);
        if (D_8015EB9C != 0xE) {
            switch (D_8015EB9C) {
            case 0:
                i = 1;
                break;
            case 1:
                i = 2;
                break;
            case 2:
                i = 4;
                break;
            case 3:
                i = 3;
                break;
            case 4:
                i = 2;
                break;
            case 5:
                i = 2;
                break;
            case 6:
                i = 3;
                break;
            case 7:
                i = 2;
                break;
            case 8:
                i = 2;
                break;
            case 9:
                i = 1;
                break;
            case 10:
                i = 2;
                break;
            case 13:
                i = 3;
                break;
            }
            for (j = 0; j < i; j++) {
                load_tpage_buf_lock(j + 0x25, ((s32 *) D_8015EEA0)[j], 1);
            }
        }
        /* fall through */
    case 0x8000000:
        if (D_8015EBA4 == 1) {
            switch (D_8015EBA0) {
            case 0:
                i = 4;
                break;
            case 1:
                i = 4;
                break;
            case 2:
                i = 4;
                break;
            case 3:
                i = 5;
                break;
            case 4:
                i = 5;
                break;
            case 5:
                i = 4;
                break;
            case 6:
                i = 3;
                break;
            case 7:
                i = 3;
                break;
            case 8:
                i = 5;
                break;
            case 9:
                i = 5;
                break;
            case 10:
                i = 3;
                break;
            case 11:
                i = 6;
                break;
            }
            for (j = 0; j < 6; j++) {
                if (j < i) {
                    load_tpage_buf_lock(j + 5, ((s32 *) D_8015EE9C)[j], 1);
                } else {
                    func_80041168(j + 5);
                }
            }
        }
        load_tpage_buf_lock(0x2C, ((s32 *) D_8015EE90)[0], 1);
        load_tpage_buf_lock(0xC, ((s32 *) D_8015EE90)[1], 1);
        load_tpage_buf_lock(0xD, ((s32 *) D_8015EE90)[2], 1);
        load_tpage_buf_lock(0xE, ((s32 *) D_8015EE90)[3], 1);
        load_tpage_buf_lock(0xF, ((s32 *) D_8015EE90)[4], 1);
        load_tpage_buf_lock(0x2B, ((s32 *) D_8015EE90)[5], 1);
        break;
    }
    for (i = 0; i < 0x30; i++) {
        func_8013E8B8(i, 0x27, 0);
    }
    /* The record loops below index D_8011ECD0 as bytes (fields 0x0C, 0x10, 0x34, 0x43, 0x01 of record 96 + i), like
     * the other RPG_BAT record loops: the typed DisplayActor44 index folds 0x1980 into the start pointer and builds
     * a different loop preheader (T-17119, wiki/register-cheatsheet.md). */
    if (D_8015EB9C != 0xE) {
        for (i = 29; i < 31; i++) {
            *(s32 *) &D_8011ECD0[0x198C + i * 0x44] = D_8015EEB8;
            *(s32 *) &D_8011ECD0[0x1990 + i * 0x44] = D_8015EED0;
            *(s32 *) &D_8011ECD0[0x19B4 + i * 0x44] = D_8015EEA0;
            D_8011ECD0[0x19C3 + i * 0x44] = 0x1C;
            D_8011ECD0[0x1981 + i * 0x44] = 5;
        }
    }
    if (D_8015EBA4 == 1) {
        for (i = 40; i < 48; i++) {
            *(s32 *) &D_8011ECD0[0x198C + i * 0x44] = D_8015EEB4;
            *(s32 *) &D_8011ECD0[0x1990 + i * 0x44] = D_8015EECC;
            *(s32 *) &D_8011ECD0[0x19B4 + i * 0x44] = D_8015EE9C;
            D_8011ECD0[0x19C3 + i * 0x44] = 0x1D;
        }
    }
    if (D_8015EB98 & 0xF0000) {
        for (i = 40; i < 48; i++) {
            *(s32 *) &D_8011ECD0[0x198C + i * 0x44] = D_8015EEAC;
            *(s32 *) &D_8011ECD0[0x1990 + i * 0x44] = D_8015EEC4;
            *(s32 *) &D_8011ECD0[0x19B4 + i * 0x44] = D_8015EE94;
            D_8011ECD0[0x19C3 + i * 0x44] = 0x18;
        }
    }
    if (D_8015EB98 & 0x80000000) {
        for (i = 40; i < 48; i++) {
            *(s32 *) &D_8011ECD0[0x198C + i * 0x44] = D_8015EEA8;
            *(s32 *) &D_8011ECD0[0x1990 + i * 0x44] = D_8015EEC0;
            *(s32 *) &D_8011ECD0[0x19B4 + i * 0x44] = D_8015EE90;
            D_8011ECD0[0x19C3 + i * 0x44] = 0x12;
        }
    }
    for (i = 48; i < 50; i++) {
        *(s32 *) &D_8011ECD0[0x198C + i * 0x44] = D_8015EEB0;
        *(s32 *) &D_8011ECD0[0x1990 + i * 0x44] = D_8015EEC8;
        *(s32 *) &D_8011ECD0[0x19B4 + i * 0x44] = D_8015EE98;
        D_8011ECD0[0x19C3 + i * 0x44] = 0x1E;
    }
    if (D_8015EBA4 == 1) {
        for (i = 50; i < 56; i++) {
            *(s32 *) &D_8011ECD0[0x198C + i * 0x44] = D_8015EEB4;
            *(s32 *) &D_8011ECD0[0x1990 + i * 0x44] = D_8015EECC;
            *(s32 *) &D_8011ECD0[0x19B4 + i * 0x44] = D_8015EE9C;
            D_8011ECD0[0x19C3 + i * 0x44] = 0x1D;
        }
    }
    for (i = 56; i < 64; i++) {
        *(s32 *) &D_8011ECD0[0x198C + i * 0x44] = D_8015EEA8;
        *(s32 *) &D_8011ECD0[0x1990 + i * 0x44] = D_8015EEC0;
        *(s32 *) &D_8011ECD0[0x19B4 + i * 0x44] = D_8015EE90;
        if (D_8015EB98 == 0x20000) {
            D_8011ECD0[0x19C3 + i * 0x44] = 0x13;
        } else {
            D_8011ECD0[0x19C3 + i * 0x44] = 0x12;
        }
    }
}

void func_80141A64(void) {
    D_80120DC1 = 1;
    func_8013E7C0(0x1F, 0, 1, 1);
    func_8013E7C0(0x26, 1, 1, 1);
    func_8013E7C0(0x23, 4, 1, 0);
    D_80120AB6 = 0x6B;
    D_80120ABA = 0x3B;
    D_80120BC6 = 0x6B;
    D_80120BCA = 0x4B;
    D_80120CD6 = 0x6B;
    D_80120CDA = 0x5B;
    D_80120AFA = 0x74;
    D_80120AFE = 0x3B;
    D_80120C0A = 0x74;
    D_80120C0E = 0x4B;
    D_80120D1A = 0x74;
    D_80120D1E = 0x5B;
    D_80120B3E = 0x7D;
    D_80120B42 = 0x3B;
    D_80120C4E = 0x7D;
    D_80120C52 = 0x4B;
    D_80120D5E = 0x7D;
    D_80120D62 = 0x5B;
    D_80120B82 = 0x86;
    D_80120B86 = 0x3B;
    D_80120C92 = 0x86;
    D_80120C96 = 0x4B;
    D_80120DA2 = 0x86;
    D_80120DA6 = 0x5B;
    D_80120F3A = 0x58;
    D_80120F3E = 0x50;
    D_80120F2A = 8;
    D_80120EF6 = 0x58;
    D_80120EFA = 0x60;
    D_80120EE6 = 8;
}
