#include "common.h"
#include "ovl/TACO.h"

void func_80135720(void) {
    D_8015F3F0 = 0;
    D_8015F3F4 = 0;
    D_8015F3F8 = 0;
    D_8015F3FC = 0;
}

s32 func_80135744(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013587C();
        break;
    case 1:
        func_801359B4();
        break;
    case 2:
        func_80135994();
        break;
    }
}

/* NON_MATCHING: T-10310, near miss */
#ifdef NON_MATCHING
void func_801357B0(arg0, arg1, arg2, arg3, arg4, arg5, arg6)
u8 *arg0;
s32 arg1;
s32 arg2;
s32 arg3;
s32 arg4;
s32 arg5;
s32 arg6;
{
    u8 *p;
    s32 i;
    s32 q;
    s32 j;

    p = arg0;
    i = j = 0;
    q = arg5;
    for (; i < 0x111 && *p != 0; i += 0xD) {
        func_80139424(*p, arg3, 3, 0x80, q, arg6, arg1, arg2, arg4);
        p++;
        q += 0xD;
    }
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_801357B0);
#endif

s32 func_8013587C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_800573AC();
        cd_read_sectors(8, 0x80162000, 0xB378);
        func_80135720();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_800536AC(0x2C0, 0, 0x140, 0xF0);
        func_80135944();
        func_8004284C();
        break;
    }
}

void func_80135944(void) {
    RECT r;

    r.x = 0x2C0;
    r.y = 0;
    r.w = 0x140;
    r.h = 0x14;
    func_8009C7F8(&r, 0, 0, 0);
    func_8009C674(0);
}

void func_80135994(void) {
    func_80042908(2);
}

void func_801359B4(void) {
    if (D_8015F3F4 == 1) {
        func_80135F6C();
    } else {
        func_80136988();
    }
    func_800578F4(2);
    func_80135A04();
}

void func_80135A04(void) {
    s32 sp58;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 sp48;
    s32 sp44;
    s32 *temp_v0;
    s32 temp_t6;
    u32 temp_hi;
    u32 temp_hi_2;
    u32 temp_hi_3;

    sp48 = 3;
    sp44 = 3;
    temp_v0 = &(&sp44)[2];
    (*(s32 *)temp_v0) = 3;
    *(s32 *)((unsigned char *)temp_v0 + (4)) = 3;
    *(s32 *)((unsigned char *)temp_v0 + (8)) = 3;
    *(s32 *)((unsigned char *)temp_v0 + (12)) = 3;
    (&sp44)[D_8015F3F0] = 5;
    func_80049A40(-0x88, -0x1C, 0x10E, 0x78, 4, 0x3F3F3F, 0x81);
    func_801357B0((s32) "SETTING", 0x1000, 0x1000, sp58, 3, -0x30, -0x40);
    func_801357B0((s32) "AUTO SHOT", 0x1000, 0x1000, sp44, 3, -0x78, -0x10);
    func_801357B0((s32) "ROLL BUTTON", 0x1000, 0x1000, sp48, 3, -0x78, 8);
    func_801357B0((s32) "SHOT BUTTON", 0x1000, 0x1000, sp4C, 3, -0x78, 0x20);
    func_801357B0((s32) "BOMB BUTTON", 0x1000, 0x1000, sp50, 3, -0x78, 0x38);
    func_801357B0((s32) "EXIT", 0x1000, 0x1000, sp54, 3, -0x78, 0x50);
    if ((D_8015F3F4 != 1) || (D_8015F3F0 != 0)) {
        temp_t6 = D_8015EDD8 & 1;
        switch (temp_t6) {
        case 0:
            func_801357B0((s32) "OFF", 0x1000, 0x1000, sp44, 3, 0x2A, -0x10);
            break;
        case 1:
            func_801357B0((s32) "ON", 0x1000, 0x1000, sp44, 3, 0x2A, -0x10);
            break;
        }
    }
    if ((D_8015F3F4 != 1) || (D_8015F3F0 != 1)) {
        temp_hi = (u8) D_8015EDDC % 3U;
        switch (temp_hi) {
        case 0:
            func_801357B0((s32) "R12-L12", 0x1000, 0x1000, sp48, 3, 0x2A, 8);
            break;
        case 1:
            func_801357B0((s32) "R1-L1", 0x1000, 0x1000, sp48, 3, 0x2A, 8);
            break;
        case 2:
            func_801357B0((s32) "R2-L2", 0x1000, 0x1000, sp48, 3, 0x2A, 8);
            break;
        }
    }
    if ((D_8015F3F4 != 1) || (D_8015F3F0 != 2)) {
        temp_hi_2 = (u8) D_8015EDE0 % 3U;
        switch (temp_hi_2) {
        case 0:
            func_801357B0((s32) "@", 0x1000, 0x1000, sp4C, 3, 0x2A, 0x20);
            break;
        case 1:
            func_801357B0((s32) "*", 0x1000, 0x1000, sp4C, 3, 0x2A, 0x20);
            break;
        case 2:
            func_801357B0((s32) "#", 0x1000, 0x1000, sp4C, 3, 0x2A, 0x20);
            break;
        }
    }
    if ((D_8015F3F4 != 1) || (D_8015F3F0 != 3)) {
        temp_hi_3 = (u8) D_8015EDE4 % 3U;
        switch (temp_hi_3) {
        case 0:
            func_801357B0((s32) "*", 0x1000, 0x1000, sp50, 3, 0x2A, 0x38);
            break;
        case 1:
            func_801357B0((s32) "#", 0x1000, 0x1000, sp50, 3, 0x2A, 0x38);
            break;
        case 2:
            func_801357B0((s32) "@", 0x1000, 0x1000, sp50, 3, 0x2A, 0x38);
            break;
        }
    }
}

void func_80135F6C(void) {
    D_8015F3FC += 1;
    switch (D_8015F3F0) {
    case 0:
        func_80136024();
        break;
    case 1:
        func_801361E8();
        break;
    case 2:
        func_80136438();
        break;
    case 3:
        func_80136688();
        break;
    }
    if (D_8015F3FC >= 0x1F) {
        D_8015F3FC = 0;
        D_8015F3F4 = 0;
    }
}

void func_80136024(void) {
    s32 t;

    if (D_8015F3FC < 0xF) {
        t = (0xF - D_8015F3FC << 0xC) / 15;
        switch (D_8015F3F8 % 2) {
        case 0:
            func_801357B0("OFF", t, 0x1000, 5, 3, 0x2A, -0x10);
            return;
        case 1:
            func_801357B0("ON", t, 0x1000, 5, 3, 0x2A, -0x10);
            return;
        }
    } else {
        t = ((D_8015F3FC << 0xC) - 0xF000) / 15;
        switch (D_8015EDD8 & 1) {
        case 0:
            func_801357B0("OFF", t, 0x1000, 5, 3, 0x2A, -0x10);
            return;
        case 1:
            func_801357B0("ON", t, 0x1000, 5, 3, 0x2A, -0x10);
            return;
        }
    }
}

void func_801361E8(void) {
    s32 temp_hi;
    s32 temp_lo;
    s32 temp_lo_2;
    u32 temp_hi_2;

    if (D_8015F3FC < 0xF) {
        temp_lo = (s32) ((0xF - D_8015F3FC) << 0xC) / 15;
        temp_hi = (s32) D_8015F3F8 % 3;
        switch (temp_hi) {
        case 0:
            func_801357B0((s32) "R12-L12", temp_lo, 0x1000, 5, 3, 0x2A, 8);
            return;
        case 1:
            func_801357B0((s32) "R1-L1", temp_lo, 0x1000, 5, 3, 0x2A, 8);
            return;
        case 2:
            func_801357B0((s32) "R2-L2", temp_lo, 0x1000, 5, 3, 0x2A, 8);
            return;
        }
    } else {
        temp_lo_2 = (s32) ((D_8015F3FC << 0xC) + 0xFFFF1000) / 15;
        temp_hi_2 = (u8) D_8015EDDC % 3U;
        switch (temp_hi_2) {
        case 0:
            func_801357B0((s32) "R12-L12", temp_lo_2, 0x1000, 5, 3, 0x2A, 8);
            return;
        case 1:
            func_801357B0((s32) "R1-L1", temp_lo_2, 0x1000, 5, 3, 0x2A, 8);
            return;
        case 2:
            func_801357B0((s32) "R2-L2", temp_lo_2, 0x1000, 5, 3, 0x2A, 8);
            return;
        }
    }
}

void func_80136438(void) {
    s32 temp_hi;
    s32 temp_lo;
    s32 temp_lo_2;
    u32 temp_hi_2;

    if (D_8015F3FC < 0xF) {
        temp_lo = (s32) ((0xF - D_8015F3FC) << 0xC) / 15;
        temp_hi = (s32) D_8015F3F8 % 3;
        switch (temp_hi) {
        case 0:
            func_801357B0((s32) "@", temp_lo, 0x1000, 5, 3, 0x2A, 0x20);
            return;
        case 1:
            func_801357B0((s32) "*", temp_lo, 0x1000, 5, 3, 0x2A, 0x20);
            return;
        case 2:
            func_801357B0((s32) "#", temp_lo, 0x1000, 5, 3, 0x2A, 0x20);
            return;
        }
    } else {
        temp_lo_2 = (s32) ((D_8015F3FC << 0xC) + 0xFFFF1000) / 15;
        temp_hi_2 = (u8) D_8015EDE0 % 3U;
        switch (temp_hi_2) {
        case 0:
            func_801357B0((s32) "@", temp_lo_2, 0x1000, 5, 3, 0x2A, 0x20);
            return;
        case 1:
            func_801357B0((s32) "*", temp_lo_2, 0x1000, 5, 3, 0x2A, 0x20);
            return;
        case 2:
            func_801357B0((s32) "#", temp_lo_2, 0x1000, 5, 3, 0x2A, 0x20);
            return;
        }
    }
}

void func_80136688(void) {
    s32 temp_hi;
    s32 temp_lo;
    s32 temp_lo_2;
    u32 temp_hi_2;

    if (D_8015F3FC < 0xF) {
        temp_lo = (s32) ((0xF - D_8015F3FC) << 0xC) / 15;
        temp_hi = (s32) D_8015F3F8 % 3;
        switch (temp_hi) {
        case 0:
            func_801357B0((s32) "*", temp_lo, 0x1000, 5, 3, 0x2A, 0x38);
            return;
        case 1:
            func_801357B0((s32) "#", temp_lo, 0x1000, 5, 3, 0x2A, 0x38);
            return;
        case 2:
            func_801357B0((s32) "@", temp_lo, 0x1000, 5, 3, 0x2A, 0x38);
            return;
        }
    } else {
        temp_lo_2 = (s32) ((D_8015F3FC << 0xC) + 0xFFFF1000) / 15;
        temp_hi_2 = (u8) D_8015EDE4 % 3U;
        switch (temp_hi_2) {
        case 0:
            func_801357B0((s32) "*", temp_lo_2, 0x1000, 5, 3, 0x2A, 0x38);
            return;
        case 1:
            func_801357B0((s32) "#", temp_lo_2, 0x1000, 5, 3, 0x2A, 0x38);
            return;
        case 2:
            func_801357B0((s32) "@", temp_lo_2, 0x1000, 5, 3, 0x2A, 0x38);
            return;
        }
    }
}

void func_801368D8(u8 *arg0, u8 arg1) {
    u8 v;

    if (D_8015F3F0 < 4) {
        D_8015F3F4 = 1;
    }
    v = *arg0;
    D_8015F3F8 = v;
    if (D_800E6280.unk_F88 & 0x2020) {
        *arg0 = ((u32)v + arg1 + 1) % (u32)arg1;
    }
    if (D_800E6280.unk_F88 & 0x8000) {
        *arg0 = (*arg0 + arg1 - 1U) % (u32)arg1;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_80136988);
