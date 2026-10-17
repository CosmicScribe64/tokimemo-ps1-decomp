#include "common.h"
#include "ovl/RPG_BAT.h"

void func_801439E0(void) {
    if ((func_801446F8() < D_8015EDF4) && !(D_8015ED98 & 0xFF) && !(D_8015EDC4 & 0xFF) && !(D_8015EE1C & 0xFF) &&
        !(D_8015EDF0 & 0xFF)) {
        func_8013D030();
        D_8015EDF4 = 0;
        D_8015EDF0 = func_80147930();
        if (!(D_8015EDF0 & 0xFF)) {
            D_8015EDF4 = func_8013EA00(0x64, 0x32);
        }
    }
    if (D_8015EDF0 == 0x10001000) {
        func_8014F88C();
    }
    if (D_8015EDF0 == 0x10001001) {
        if ((s32)D_8015EE04 < 0xF) {
            func_8013D030();
        }
        func_8014FDBC();
    }
    if (!(D_8015EE1C & 0xFF) && ((D_8015EDF0 & 0xFF) || (D_8015EDF0 & 0x80000000))) {
        switch (D_8015EB9C) {
        case 0:
            func_801420EC();
        default:
            break;
        case 1:
            func_80155A70();
            break;
        case 2:
            if (D_8015EB98 != 0x80000000) {
                func_801481C8();
            }
            break;
        case 3:
            func_80151C10();
            break;
        case 4:
            func_80136E2C();
            break;
        case 5:
            func_80159E28();
            break;
        case 6:
            func_801568A4();
            break;
        case 7:
            func_8014DF60();
            break;
        case 8:
            func_8015675C();
            break;
        case 9:
            func_80135AE0();
            break;
        case 10:
            func_80136374();
            break;
        case 13:
            func_801340F0();
            break;
        case 14:
            break;
        }
    }
}

void func_80143C28(s32 arg0, s32 arg1) {
    s32 temp_v0;

    switch (D_8015EE00.unk_08) {
    case 0:
        func_8013E7C0(0x1D, arg0, 1, 1);
        func_8014F258();
        return;
    case 1:
        func_8013E97C(0x1D, -D_8015EE10, 0);
        temp_v0 = D_8015EE10 + 2;
        D_8015EE10 = temp_v0;
        if (temp_v0 >= 0x21) {
            func_8013E7C0(0x1D, arg1, 1, 1);
            func_8013E97C(0x1D, -0x20, 0);
            func_8014F230();
        }
        return;
    }
}

void func_80143CE8(s32 arg0, s32 arg1) {
    s32 temp_v0;

    switch (D_8015EE00.unk_08) {
    case 0:
        func_8013E7C0(0x1D, arg0, 1, 1);
        func_8014F258();
        return;
    case 1:
        func_8013E97C(0x1D, D_8015EE10 - 0x20, 0);
        temp_v0 = D_8015EE10 + 2;
        D_8015EE10 = temp_v0;
        if (temp_v0 >= 0x21) {
            func_8013E7C0(0x1D, arg1, 1, 1);
            func_8013E97C(0x1D, 0, 0);
            func_8014F230();
        }
        return;
    }
}

void func_80143DA8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    D_8015EC50 = arg3 - arg1;
    D_8015EC54 = arg4 - arg2;
    func_8013E97C(arg0, (D_8015EC50 * D_8015EE10) / 1000 + arg1, (D_8015EC54 * D_8015EE10) / 1000 + arg2);
    D_8015EE10 += arg5;
    if (D_8015EE10 >= 0x3E9) {
        func_8013E97C(arg0, arg3, arg4);
        func_8014F230();
    }
}

void func_80143ECC(void) {
    if (!(D_8015EE00.unk_0C & 1)) {
        ((DisplayActor44 *)D_8011ECD0)[144].unk_03 |= 0x80;
        ((DisplayActor44 *)D_8011ECD0)[125].unk_03 |= 0x80;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[144].unk_03 &= 0xFF7F;
        ((DisplayActor44 *)D_8011ECD0)[125].unk_03 &= 0xFF7F;
    }
    D_8015EE00.unk_0C += 1;
    if (D_8015EE00.unk_0C >= 0x3C) {
        func_8014F230();
    }
}

void func_80143F68(void) {
    if (!(D_8015EE00.unk_0C & 1)) {
        ((DisplayActor44 *)D_8011ECD0)[144].unk_03 |= 0x80;
        ((DisplayActor44 *)D_8011ECD0)[125].unk_03 |= 0x80;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[144].unk_03 &= 0xFF7F;
        ((DisplayActor44 *)D_8011ECD0)[125].unk_03 &= 0xFF7F;
    }
    D_8015EE00.unk_0C += 1;
    if (D_8015EE00.unk_0C >= 0x3D) {
        func_8014F230();
    }
}
