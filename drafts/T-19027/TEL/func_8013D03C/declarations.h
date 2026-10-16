/* Declarations the typed draft needs (T-19027). Reverted from include/ because the function stays INCLUDE_ASM. */
/* include/main_api.h, after D_800B3750: */
extern u8 D_800B3778[]; /* seven icon x offsets per girl */
/* include/ovl/TEL.h: */
typedef struct TelSignDays {
    /* 0x00 */ u32 w[16];
} TelSignDays; /* size 0x40 */
extern TelSignDays D_80146FD0;
/* The draft also uses the macro REC(n) = (((DisplayRecord44 *)D_8011ECD0)[n]) and #include "game.h". */
