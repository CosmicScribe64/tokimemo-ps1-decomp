extern u16 D_800E6374;
extern s16 D_800E6382;
extern s16 D_800E6386;
extern s16 D_800E638A;
extern s16 D_800E638E;
extern s16 D_800E6392;
extern s16 D_800E6396;
extern s16 D_800E639A;
extern u8 D_800E644A;
extern s32 D_800E6480;
extern s32 D_800E64B8;
extern s32 D_800E64F0;
extern s32 D_800E6528;
extern u8 D_800E652A;
extern s32 D_800E6560;
extern s32 D_800E6598;
extern s32 D_800E65D0;
extern s32 D_800E6608;
extern s32 D_800E71E8;
extern s32 D_800E7378;

s32 func_8006EEC4(u8 *arg0, u8 *arg1) {
    u32 temp_t4;

    if (rand() & 7) {
        goto block_48;
    }
    *arg0 = 0;
    *arg1 = 0;
    if (!(D_800E6480 & 2) && !(D_800E71E8 & 0xF) && (D_800E6382 >= 0x4B)) {
        *arg0 = 1;
        *arg1 = D_800E7378 & 1;
        return 1;
    }
    if (!(D_800E64B8 & 2) && ((D_800E71E8 & 0xF) == 1) && (D_800E6386 >= 0x55)) {
        *arg0 = 2;
        *arg1 = D_800E7378 & 3;
        return 1;
    }
    if (!(D_800E64F0 & 2) && ((D_800E71E8 & 0xF) == 2) && (D_800E638A >= 0x41)) {
        *arg0 = 3;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6560 & 2) && ((D_800E71E8 & 0xF) == 3) && (((u8) D_800E644A >> 4) != 8) && (D_800E638E >= 0x54) && (D_800E6396 >= 0x54)) {
        *arg0 = 5;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6598 & 2) && ((D_800E71E8 & 0xF) == 3) && (((u8) D_800E644A >> 4) != 9) && (D_800E638E >= 0x47) && (D_800E639A >= 0x47)) {
        *arg0 = 6;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E65D0 & 2) && ((D_800E71E8 & 0xF) == 6) && (D_800E6396 >= 0x7E)) {
        *arg0 = 7;
        *arg1 = 0;
        return 1;
    }
    if (!(D_800E6608 & 2)) {
        temp_t4 = (u32) (D_800E6608 << 0x14) >> 0x1D;
        if ((temp_t4 == 0) && (D_800E6392 >= 0x24)) {
            *arg0 = 8;
            *arg1 = 0;
            return 1;
        }
        if ((temp_t4 == 1) && (D_800E6392 >= 0x38)) {
            *arg0 = 8;
            *arg1 = 1;
            return 1;
        }
        if ((temp_t4 == 2) && (D_800E6392 >= 0x4C)) {
            *arg0 = 8;
            *arg1 = 2;
            return 1;
        }
        if ((temp_t4 == 3) && ((D_800E71E8 & 0xF) == 5) && (D_800E6392 >= 0x5B)) {
            *arg0 = 8;
            *arg1 = 3;
            return 1;
        }
        goto block_44;
    }
block_44:
    if (!(D_800E6528 & 2) && (((u16) D_800E6374 >> 0xC) != ((u8) D_800E652A >> 4)) && (D_800E639A >= 0x60)) {
        *arg0 = 4;
        *arg1 = 0;
        return 1;
    }
block_48:
    return 0;
}
