#define MAIN_API_OVERRIDE_D_800C9730 /* matched as the s32 table it is (11 words up to sprite_frame_table); main_api.h: u8[]; T-17008 */
#define MAIN_API_OVERRIDE_D_800C5FCC /* matched as the two-word table up to D_800C5FD4; main_api.h: s32; T-17008 */
#include "common.h"
#include "game.h"

extern s32 D_800C9730[11];
extern s32 D_800C5FCC[2];

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
    s32 i;
    s32 k;

    /* FAKE: the subtract loops need the 0x80000 in a register (`subu rd,rs,s0`, shared with the add loops); a literal `-= 0x80000` becomes `addu` of 0xFFF80000. Real source unknown. T-17008 */
    k = 0x80000;
    if (!(D_800C9730[0] & 0x80000)) {
        for (i = 0; i < 11; i++) {
            D_800C9730[i] += 0x80000;
        }
        for (i = 0; i < 2; i++) {
            D_800C5FCC[i] += 0x80000;
        }
    }
    load_tpage_buf_lock(0x1F, D_800C9730[7], 2);
    load_tpage_buf_lock(0x1E, D_800C9730[8], 2);
    load_tpage_buf_lock(0x1D, D_800C9730[9], 2);
    load_tpage_buf_lock(0x1C, D_800C9730[10], 2);
    load_tpage_buf_lock(0x1B, D_800C5FCC[0], 2);
    load_tpage_buf_lock(0x3B, D_800C5FCC[1], 2);
    load_tpage_buf_lock(0x3C, D_800C9730[0], 2);
    load_tpage_buf_lock(0x3D, D_800C9730[1], 2);
    load_tpage_buf_lock(0x3E, D_800C9730[2], 2);
    load_tpage_buf_lock(0x3F, D_800C9730[3], 2);
    for (i = 0; i < 11; i++) {
        D_800C9730[i] -= k;
    }
    for (i = 0; i < 2; i++) {
        D_800C5FCC[i] -= k;
    }
    for (i = 0x1B; i < 0x20; i++) {
        D_800E6280.unk_1228[i] += 0xFFF80000;
    }
    i = 0x3B;
    do {
        D_800E6280.unk_1228[i] += 0xFFF80000;
    } while (++i < 0x40);
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

void func_80059EF0(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 32; j++) {
            if (D_800C62C0[j] & 0x7FFF0000) {
                D_800C62C0[j] |= 0x80000000;
            }
            if (D_800C62C0[j] & 0x7FFF) {
                D_800C62C0[j] |= 0x8000;
            }
        }
        for (j = 0; j < 8; j++) {
            if (D_800C6200[j] & 0x7FFF0000) {
                D_800C6200[j] |= 0x80000000;
            }
            if (D_800C6200[j] & 0x7FFF) {
                D_800C6200[j] |= 0x8000;
            }
        }
        load_palette(&D_800C6020, i & 0xFF, 1, 1, i & 0xFF);
    }
    hizuke_init();
    hizuke_disp_switch(0);
    parameter_show_init();
    parameter_show_init();
    parameter_disp_switch(0);
    func_8006509C();
    icon_disp_switch(0);
    message_window_init();
    message_disp_switch(0);
    func_800673B8();
    func_8006764C(0);
    func_80065F34(0);
}

void func_8005A06C(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_80059B40();
        return;
    }
    main_state_set(0x12);
}
