#ifndef OVL_BUNKASAI_H
#define OVL_BUNKASAI_H

#include "common.h"
#include "game.h"

/* BUNKASAI overlay (load address 0x80132000): externs and types. */

extern u32 D_801604F0; /* three data pointers set by the 0x34-byte setters at the start */
extern u32 D_801604F4;
extern u32 D_801604F8;

extern s32 D_8015D774;

/* defined in C in one object, called from another (T-0500) */
void func_80132000(void);
void func_80132034(void);
void func_80132068(void);
void func_8013209C(void);
void func_801320D0(void);
void func_80132104(void);
void func_80132138(void);
void func_8013216C(void);
void func_801321A0(void);
void func_801321D4(void);
void func_80132208(void);
void func_8013223C(void);
void func_80132270(void);
void func_801322A4(void);
void func_801322D8(void);
void func_8013230C(void);
void func_80132340(void);
void func_80132374(void);
extern s32 D_80160640;
extern s32 D_80160644;
extern s32 D_80160648;
extern s32 D_8016064C;
extern s32 D_80160650;
extern s32 D_80160654;
extern s32 D_80160658;
extern s32 D_8016065C;
extern s32 D_80160660;
extern s32 D_80160664;
extern s32 D_80160668;
extern s32 D_8016066C;
extern s32 D_80160670;
extern s32 D_80160674;
extern s32 D_80160678;
extern s32 D_8016067C;
extern s32 D_80160680;
extern s32 D_80160684;
extern s32 D_80160688;
extern s32 D_8016068C;
extern s32 D_80160690;
extern s32 D_80160694;
extern s32 D_80160698;
extern s32 D_8016069C;
extern s32 D_801606A0;
extern s32 D_801606A4;
extern s32 D_801606A8;
extern s32 D_801606AC;
extern s32 D_801606B0;
extern s32 D_801606B4;
extern s32 D_801606B8;
extern s32 D_801606BC;
extern s32 D_801606C0;
extern s32 D_801606C4;
extern s32 D_801606C8;
extern s32 D_801606CC;
extern s32 D_801606D0;
extern s32 D_801606D4;
extern s32 D_801606D8;

/* T-9510: adjacent words D_80122EB8/+4, read and written together in
 * func_80146030. This view preserves the observed load-before-store order. */
typedef struct BunkSelectWords {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} BunkSelectWords; /* size 0x08 */

#endif

#ifndef OVL_BUNKASAI_MODE_T9740
#define OVL_BUNKASAI_MODE_T9740
/* Two-word state at D_80122EB8, used by func_8013B190 (T-9740). */
typedef struct BunkModePair {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} BunkModePair; /* size 0x08 */
void func_8013B190(void);
#endif

#ifndef BUNKASAI_TABLE40_T9940
#define BUNKASAI_TABLE40_T9940
typedef struct BunkasaiTable40 {
    /* 0x00 */ s32 unk_00[4][4];
} BunkasaiTable40; /* size 0x40; copied by func_8015B828 */
extern BunkasaiTable40 D_80160B50;
#ifndef OVL_BUNKASAI_T9960
#define OVL_BUNKASAI_T9960
/* Byte at D_800E7D4F: continuation flags and three-bit mode (T-9960). */
typedef struct BunkasaiModeFlags {
    /* 0x00 */ u8 b0 : 1;
    /* 0x00 */ u8 b1 : 1;
    /* 0x00 */ u8 mode : 3;
    /* 0x00 */ u8 pad : 3;
} BunkasaiModeFlags; /* size 0x01 */

void func_8013B120(void);
void func_801508C4(void);
#endif
