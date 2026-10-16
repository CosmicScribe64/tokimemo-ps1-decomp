#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_80133870);

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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7318;
extern s32 D_800E731C;
/* end bulk_m2c declarations */
u8 func_80133E98(void) {
    u8 temp_t0;
    u8 temp_t0_2;
    u8 temp_t1;
    u8 temp_t2;
    u8 temp_t3;
    u8 temp_t4;
    u8 temp_t4_2;
    u8 temp_t5;
    u8 temp_t6;
    u8 temp_t7;
    u8 temp_t8;
    u8 temp_t8_2;
    u8 var_v0;

    var_v0 = D_800E6280.unk_110D;
    switch (var_v0) {
    case 0:
        func_800462C8(1U, 0x8018A000, 0xB3F2);
        func_800462C8(5U, 0x8018A800, 0xB413);
        func_800462C8(0x12U, 0x8018D000, 0xB438);
        temp_t8 = (D_800E6280.unk_110D + 2) & 0xFF;
        var_v0 = temp_t8;
        D_800E6280.unk_110D = temp_t8;
    default:
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            temp_t0 = (D_800E6280.unk_110D + 1) & 0xFF;
            var_v0 = temp_t0;
            D_800E6280.unk_110D = temp_t0;
        }
        break;
    case 5:
        func_800462C8(7U, 0x80196000, 0xB45B);
        D_800E6280.unk_110D += 1;
        func_800462C8(3U, 0x80199800, 0xB478);
        func_800462C8(2U, 0x8019B000, 0xB47B);
        temp_t2 = (D_800E6280.unk_110D + 1) & 0xFF;
        var_v0 = temp_t2;
        D_800E6280.unk_110D = temp_t2;
        break;
    case 6:
    case 7:
    case 8:
    case 9:
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            temp_t4 = (D_800E6280.unk_110D + 1) & 0xFF;
            var_v0 = temp_t4;
            D_800E6280.unk_110D = temp_t4;
        }
        break;
    case 10:
        func_800462C8(1U, 0x8019C000, 0xB486);
        func_800462C8(2U, 0x8019C800, 0xB487);
        func_800462C8(4U, 0x8019D800, 0xB4A9);
        func_800462C8(6U, 0x8019F800, 0xB4AD);
        temp_t5 = (D_800E6280.unk_110D + 1) & 0xFF;
        var_v0 = temp_t5;
        D_800E6280.unk_110D = temp_t5;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            temp_t7 = (D_800E6280.unk_110D + 1) & 0xFF;
            var_v0 = temp_t7;
            D_800E6280.unk_110D = temp_t7;
        }
        break;
    case 15:
        func_800462C8(2U, 0x801A2800, 0xB4B3);
        func_800462C8(4U, 0x801A3800, 0xB4B5);
        func_800462C8(3U, 0x801A5800, 0xB4B9);
        func_800462C8(0xAU, 0x801A7000, 0xB515);
        temp_t8_2 = (D_800E6280.unk_110D + 1) & 0xFF;
        var_v0 = temp_t8_2;
        D_800E6280.unk_110D = temp_t8_2;
        break;
    case 16:
    case 17:
    case 18:
    case 19:
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            temp_t0_2 = (D_800E6280.unk_110D + 1) & 0xFF;
            var_v0 = temp_t0_2;
            D_800E6280.unk_110D = temp_t0_2;
        }
        break;
    case 20:
        func_800462C8(1U, 0x801AC000, 0xB534);
        func_800462C8(3U, 0x801AC800, 0xB668);
        func_800462C8(3U, 0x801AE000, 0xB66B);
        func_800462C8(3U, 0x801AF800, 0xB66E);
        temp_t1 = (D_800E6280.unk_110D + 1) & 0xFF;
        var_v0 = temp_t1;
        D_800E6280.unk_110D = temp_t1;
        break;
    case 21:
    case 22:
    case 23:
    case 24:
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            temp_t3 = (D_800E6280.unk_110D + 1) & 0xFF;
            var_v0 = temp_t3;
            D_800E6280.unk_110D = temp_t3;
        }
        break;
    case 25:
        func_800462C8(2U, 0x801B1000, 0xB671);
        func_800462C8(3U, 0x801B2000, 0xB673);
        func_800462C8(3U, 0x801B3800, 0xB676);
        func_800462C8(2U, 0x801B5000, 0xB679);
        temp_t4_2 = (D_800E6280.unk_110D + 1) & 0xFF;
        var_v0 = temp_t4_2;
        D_800E6280.unk_110D = temp_t4_2;
        break;
    case 26:
    case 27:
    case 28:
    case 29:
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            temp_t6 = (D_800E6280.unk_110D + 1) & 0xFF;
            var_v0 = temp_t6;
            D_800E6280.unk_110D = temp_t6;
        }
        break;
    case 30:
        D_8015EDB4 = (TcActor *) D_8011ED58;
        D_800E7318 = 1;
        D_800E731C = 2;
        var_v0 = func_8004284C();
        break;
    }
    return var_v0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_801342CC);

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
        func_801347F4(0x801AC000);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80042908(1);
        break;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_801345D4);

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

#ifdef NON_MATCHING
/* NON_MATCHING: T-10520, only "move s2,s3; addiu s2,s2,8" against one addiu and the lui/addiu order of the loop constants differ (7 lines); a K&R definition with `s32 arg0` fixes the prologue but the header check rejects it */
void func_801347F4(s32 arg0) {
    s32 b;
    TcObj50 *p;
    Tc10 *q;

    arg0 += 4;
    func_8009B3C0(arg0);
    b = arg0;
    b += 8;
    p = D_80127480;
    q = D_80127080;
    do {
        func_8009B430(b, (u8 *)q, 0);
        q->unk4[0] = (s32)p;
        p++;
        q++;
        q[-1].unk0 = 0x80000000;
    } while (p != (TcObj50 *)D_80128880);
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_801347F4);
#endif

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
