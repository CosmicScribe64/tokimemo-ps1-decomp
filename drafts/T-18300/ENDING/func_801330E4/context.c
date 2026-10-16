#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"

typedef struct {
    void (*f[16])();
} FnTbl16; /* size 0x40 */
extern FnTbl16 D_8013C370;

void func_80133030(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl16 tbl;

    tbl = D_8013C370;
    func_80083808();
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    func_8007EDF8();
    func_800846C0();
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    func_80064DEC();
    func_80083A10();
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E66B0;
extern s32 D_800E7368;
extern s8 D_800E62BA;
extern s8 D_800E62BE;
extern s8 D_800E62BF;
extern s8 D_800E62C0;
extern s8 D_800E62C1;
extern s8 D_800E7322;
extern u8 D_800E66EB;
extern u8 D_800E6838;
extern u8 D_800E6839;
extern u8 D_800E683A;
extern u8 D_800E683B;
extern u8 D_800E71DF;
extern s32 sp;
/* end bulk_m2c declarations */
void func_801330E4(void) {
    typedef struct { s32 w[9]; } M2cBlock24; /* m2c memcpy of 36 bytes */
    s32 sp78[1];
    s16 sp58;
    s16 sp54;
    s32 sp30[9];
    s16 *temp_v1;
    s16 var_a0;
    s16 var_v1;
    u32 temp_t1;
    u8 var_v0;

    *(M2cBlock24 *)(&sp30) = *(M2cBlock24 *)(&D_8013C3B0);
    D_800E62BE = 0x62;
    D_800E62BF = 3;
    D_800E62C0 = 1;
    D_800E62C1 = 6;
    func_80053418((s32) &sp78, (s32) &sp58);
    D_800E71DF = 0xD;
    if ((sp58 < 0x1F4) && ((u8) D_800E66EB >= 0x46U) && (D_800E683B == 2)) {
        D_800E71DF = 0xC;
    } else {
        var_a0 = 0x1F3;
        var_v0 = 0;
        do {
            temp_v1 = &(&sp58)[var_v0];
            if ((D_800E6280.unk_1BC[var_v0].unk_0C.w & 2) && (var_a0 < *temp_v1)) {
                D_800E71DF = var_v0;
                var_a0 = *temp_v1;
            }
            var_v0 = (u8) (s16) (var_v0 + 1);
        } while ((s32) var_v0 < 0xB);
    }
    if (D_800E71DF == 0xD) {
        temp_t1 = (u32) (D_800E66B0 << 0x14) >> 0x1D;
        var_v1 = temp_t1 + ((D_800E6838 != 0) * 2) + ((D_800E683A != 0) * 2);
        if (var_v1 >= 0x12) {
            var_v1 = 0x11;
        }
        if (temp_t1 != 0) {
            if (D_800E6839 == 0) {
                sp54 = var_v1;
                if ((func_800AE0D0() % 256) < *(s16 *)((unsigned char *)(sp + (var_v1 * 2)) + (48))) {
                    D_800E71DF = 0xB;
                }
            }
        }
    }
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_800AE0F0(D_800CA19C, "廊下");
    func_800AE0F0(D_800CA1DC, "教室");
    D_800E7322 = 0;
    D_800E7368 = 1;
    func_8008585C();
    D_800E62BA = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    D_800CA148 = 0;
    D_800CA14C = 0;
    func_800847B8(D_800E71DF);
    func_80132000();
    D_800CA160 = D_8013C28C;
    D_800CA164 = D_8013C2D0;
    D_800CA168 = D_8013C314;
    func_8004284C();
}

void func_80133400(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_80133428(void) {
    cd_read_sectors(0x11, 0x801F0000, 0xAE07);
    func_80132000();
    func_8004284C();
}

void func_80133460(void) {
    if (((u8) D_800E6280.unk_F5F >= 0xDU) && ((u32) ((u32) (D_800E6280.unk_1BC[14].unk_0C.w << 0x14) >> 0x1D) < 3U)) {
        func_80042908(7);
        D_800E7D34 |= 8;
        return;
    }
    func_8004284C();
    D_800E7D34 |= 4;
}

void func_801334E4(void) {
    if ((((u32)(D_800E6280.unk_1BC[1].unk_0C.w << 0x1E) >> 0x1F)) && (get_h_tokimeki(1) >= 0x46U)) {
        normal_date_speak();
        return;
    }
    func_8004284C();
}
