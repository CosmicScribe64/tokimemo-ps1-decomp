/* types the draft needs; not in the production headers (T-17014) */

/* PsyQ libgpu DR_ENV, DISPENV and DRAWENV (LIBGPU.H, PsyQ SDK 3.x; sizes 0x40, 0x14 and 0x5C as the
 * frames of func_80056BA8 show). T-17014 */
typedef struct DR_ENV {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 code[15];
} DR_ENV; /* size 0x40 */

typedef struct DISPENV {
    /* 0x00 */ RECT disp;
    /* 0x08 */ RECT screen;
    /* 0x10 */ u8 isinter;
    /* 0x11 */ u8 isrgb24;
    /* 0x12 */ u8 pad0;
    /* 0x13 */ u8 pad1;
} DISPENV; /* size 0x14 */

typedef struct DRAWENV {
    /* 0x00 */ RECT clip;
    /* 0x08 */ s16 ofs[2];
    /* 0x0C */ RECT tw;
    /* 0x14 */ u16 tpage;
    /* 0x16 */ u8 dtd;
    /* 0x17 */ u8 dfe;
    /* 0x18 */ u8 isbg;
    /* 0x19 */ u8 r0;
    /* 0x1A */ u8 g0;
    /* 0x1B */ u8 b0;
    /* 0x1C */ DR_ENV dr_env;
} DRAWENV; /* size 0x5C */

/* libcd CdlFILE, the result of CdSearchFile (PsyQ layout). T-17014 */
typedef struct CdlFILE {
    /* 0x00 */ u8 pos[4];
    /* 0x04 */ u32 size;
    /* 0x08 */ char name[16];
} CdlFILE; /* size 0x18 */

