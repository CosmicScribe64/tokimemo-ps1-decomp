#include "common.h"
#include "game.h"

void func_80059B40(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80059E00();
        return;
    case 1:
        func_80059BC0();
        return;
    case 2:
        func_8004ADE4();
        func_8004284C();
        return;
    default:
        func_80042808();
        return;
    }
}

void func_80059BC0(void) {
    func_80059BE8();
    func_8004284C();
}

void func_80059BE8(void) {
    s32 *var_v0_2;
    s32 *var_v0_4;
    s32 temp_t0;
    s32 temp_t2;
    s32 temp_t4;
    s32 temp_t6;
    s32 temp_t8;
    u8 *var_v0;
    u8 *var_v0_3;
    u8 *var_v0_5;
    u8 *temp_v0;

    if (!(*D_800C9730 & 0x80000)) {
        var_v0 = D_800C9730;
        do {
            temp_t8 = *var_v0;
            var_v0 += 4;
            *(s32 *)((unsigned char *)var_v0 + (-4)) = (s32) (temp_t8 + 0x80000);
        } while ((u32) var_v0 < (u32) sprite_frame_table);
        var_v0_2 = &D_800C5FCC;
        do {
            temp_t0 = *var_v0_2;
            var_v0_2 += 1;
            *(s32 *)((unsigned char *)var_v0_2 + (-4)) = (s32) (temp_t0 + 0x80000);
        } while (var_v0_2 != D_800C5FD4);
    }
    load_tpage_buf_lock(0x1F, D_800C974C, 2);
    load_tpage_buf_lock(0x1E, D_800C9750, 2);
    load_tpage_buf_lock(0x1D, D_800C9754, 2);
    load_tpage_buf_lock(0x1C, D_800C9758, 2);
    load_tpage_buf_lock(0x1B, D_800C5FCC, 2);
    load_tpage_buf_lock(0x3B, D_800C5FD0, 2);
    load_tpage_buf_lock(0x3C, *D_800C9730, 2);
    load_tpage_buf_lock(0x3D, D_800C9734, 2);
    load_tpage_buf_lock(0x3E, D_800C9738, 2);
    load_tpage_buf_lock(0x3F, D_800C973C, 2);
    var_v0_3 = D_800C9730;
    do {
        temp_t2 = *var_v0_3;
        var_v0_3 += 4;
        *(s32 *)((unsigned char *)var_v0_3 + (-4)) = (s32) (temp_t2 - 0x80000);
    } while ((u32) var_v0_3 < (u32) sprite_frame_table);
    var_v0_4 = &D_800C5FCC;
    do {
        temp_t4 = *var_v0_4;
        var_v0_4 += 1;
        *(s32 *)((unsigned char *)var_v0_4 + (-4)) = (s32) (temp_t4 - 0x80000);
    } while ((u32) var_v0_4 < (u32) D_800C5FD4);
    var_v0_5 = &D_800E6280.unk_044[0].unk_20[8];
    do {
        temp_t6 = *(s32 *)((unsigned char *)var_v0_5 + (4648));
        var_v0_5 += 4;
        *(s32 *)((unsigned char *)var_v0_5 + (4644)) = (s32) (temp_t6 + 0xFFF80000);
    } while ((u32) var_v0_5 < (u32) &D_800E6280.unk_044[0].unk_20[0x1C]);
    D_800E6280.unk_1228[0x3B] += 0xFFF80000;
    temp_v0 = (0x3C * 4) + &D_800E6280;
    *(s32 *)((unsigned char *)temp_v0 + (4660)) = (s32) (*(s32 *)((unsigned char *)temp_v0 + (4660)) + 0xFFF80000);
    *(s32 *)((unsigned char *)temp_v0 + (4656)) = (s32) (*(s32 *)((unsigned char *)temp_v0 + (4656)) + 0xFFF80000);
    *(s32 *)((unsigned char *)temp_v0 + (4652)) = (s32) (*(s32 *)((unsigned char *)temp_v0 + (4652)) + 0xFFF80000);
    D_800E6280.unk_1228[0x3C] += 0xFFF80000;
}

/* FAKE: u8 views give the original's global access (LOD): `andi`/`or v0` copy of the increment; the s32 return keeps that copy live (T-5010) */
s32 func_80059E00(void) {
    if (*(u8 *)&D_800E6280.unk_110D == 0) {
        printf("LDVI");
        back_clear_switch(1);
        func_80041168(0xFF);
        cd_read_sectors(0x35, 0x80180000, 0x9D85);
        *(u8 *)&D_800E6280.unk_110D += 1;
    } else if (*(u8 *)&D_800E6280.unk_110D == 1) {
        if (cd_read_sync_status() & 1) {
            func_80059EF0();
            func_800462C8(8, 0x801F0000, 0x9C97);
            *(u8 *)&D_800E6280.unk_110D += 1;
        }
    } else if (*(u8 *)&D_800E6280.unk_110D == 2 && (cd_read_sync_status() & 1)) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80059B40", func_80059EF0);

void func_8005A06C(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_80059B40();
        return;
    }
    main_state_set(0x12);
}
